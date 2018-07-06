/*
 * jvmti hprof
 *
 * Copyright  1990-2008 Sun Microsystems, Inc. All Rights Reserved.	 
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER	 
 *	 
 * This program is free software; you can redistribute it and/or  
 * modify it under the terms of the GNU General Public License version	
 * 2 only, as published by the Free Software Foundation.   
 *	 
 * This program is distributed in the hope that it will be useful, but	
 * WITHOUT ANY WARRANTY; without even the implied warranty of  
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU	 
 * General Public License version 2 for more details (a copy is	 
 * included at /legal/license.txt).	  
 *	 
 * You should have received a copy of the GNU General Public License  
 * version 2 along with this work; if not, write to the Free Software  
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  
 * 02110-1301 USA	
 *	 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa  
 * Clara, CA 95054 or visit www.sun.com if you need additional	
 * information or have any questions. 
 *
 */

/* Tracker class support functions. */

/*
 * This file contains the native support calls for the Tracker
 *   class. These native methods are registered and not made extern.
 *   Tracking is engaged by using JNI to assign to a static field in the
 *   Tracker class.
 *
 * Just like JVMTI callbacks, it's best that we keep track of these so that
 *   when the VM_DEATH happens we know to wait for them to complete.
 *
 * This file also contains the functions that will initialize the Tracker
 *   interface for BCI and identify the Tracker methods to make sure
 *   they are not included in any stack traces obtained from JVMTI.
 *
 * RFE: The performance of the java injected code calling native methods
 *        could be an issue here, cpu=times seems to be the worst where
 *        a native call is made for entry and exit, even on the smallest
 *        Java method. The alternative would be to cache the data on
 *        the Java side, and either push it out to the native side, or
 *        use some kind of pull from the native side, or even using
 *        shared memory or a socket.  However having said that, the
 *        current performance issues are more around sheer memory needed,
 *        and repeated calls to GetThreadCpuTime(), which is being investigated.
 *
 */

#include "jvmti_hprof.h"

/* Macros to surround tracker based callback code.
 *   Also see BEGIN_CALLBACK and END_CALLBACK in hprof_init.c.
 *   If the VM_DEATH callback is active in the begining, then this callback
 *   just blocks (it is assumed we don't want to return to the VM).
 *   If the VM_DEATH callback is active at the end, then this callback
 *   will notify the VM_DEATH callback if it's the last one.
 *
 *   WARNING: No not 'return' or 'goto' out of the BEGIN_TRACKER_CALLBACK/END_TRACKER_CALLBACK
 *            block, this will mess up the count.
 */

#define BEGIN_TRACKER_CALLBACK()                                        \
{ /* BEGIN OF TRACKER_CALLBACK */                                       \
    jboolean bypass = JNI_TRUE;                                         \
    rawMonitorEnter(gdata->callbackLock); {                             \
        if ( gdata->tracking_engaged != 0 ) {				\
	    if (!gdata->vm_death_callback_active) {                     \
		gdata->active_callbacks++;                              \
		bypass = JNI_FALSE;                                     \
	    }                                                           \
	}                                                               \
    } rawMonitorExit(gdata->callbackLock);                              \
    if ( !bypass ) {                                                    \
        /* BODY OF TRACKER_CALLBACK CODE */

#define END_TRACKER_CALLBACK() /* Part of bypass if body */             \
        rawMonitorEnter(gdata->callbackLock); {                         \
            gdata->active_callbacks--;                                  \
            if (gdata->active_callbacks < 0) {                          \
                HPROF_ERROR(JNI_TRUE, "Problems tracking callbacks");   \
            }                                                           \
            if (gdata->vm_death_callback_active) {                      \
                if (gdata->active_callbacks == 0) {                     \
                    rawMonitorNotifyAll(gdata->callbackLock);           \
                }                                                       \
            }                                                           \
        } rawMonitorExit(gdata->callbackLock);                          \
    }                                                                   \
} /* END OF TRACKER_CALLBACK */


/*
 * Class:     Tracker
 * Method:    nativeNewArray
 * Signature: (Ljava/lang/Object;Ljava/lang/Object;)V
 */
static void JNICALL 
Tracker_nativeNewArray
  (JNIEnv *env, jclass clazz, jobject thread, jobject obj)
{
    BEGIN_TRACKER_CALLBACK() {
	event_newarray(env, thread, obj);
    } END_TRACKER_CALLBACK();
}

/*
 * Class:     Tracker
 * Method:    nativeObjectInit
 * Signature: (Ljava/lang/Object;Ljava/lang/Object;)V
 */
static void JNICALL 
Tracker_nativeObjectInit
  (JNIEnv *env, jclass clazz, jobject thread, jobject obj)
{
    BEGIN_TRACKER_CALLBACK() {
	event_object_init(env, thread, obj);
    } END_TRACKER_CALLBACK();
}

/*
 * Class:     Tracker
 * Method:    nativeCallSite
 * Signature: (Ljava/lang/Object;II)V
 */
static void JNICALL 
Tracker_nativeCallSite
  (JNIEnv *env, jclass clazz, jobject thread, jint cnum, jint mnum)
{
    BEGIN_TRACKER_CALLBACK() {
	event_call(env, thread, cnum, mnum);
    } END_TRACKER_CALLBACK();
}

/*
 * Class:     Tracker
 * Method:    nativeReturnSite
 * Signature: (Ljava/lang/Object;II)V
 */
static void JNICALL 
Tracker_nativeReturnSite
  (JNIEnv *env, jclass clazz, jobject thread, jint cnum, jint mnum)
{
    BEGIN_TRACKER_CALLBACK() {
	event_return(env, thread, cnum, mnum);
    } END_TRACKER_CALLBACK();
}


/* ------------------------------------------------------------------- */
/* Set Java static field to turn on native code calls in Tracker. */

static void
set_engaged(JNIEnv *env, jint engaged)
{
    LOG3("set_engaged()", "engaging tracking", engaged);
    
    if ( ! gdata->bci ) {
        return;
    }
    rawMonitorEnter(gdata->callbackLock); {
	if ( gdata->tracking_engaged != engaged ) {
            jfieldID field;
            jclass   tracker_class;
    
            tracker_class = class_get_class(env, gdata->tracker_cnum);
	    gdata->tracking_engaged = 0;
	    /* Activate or deactivate the injection code on the Java side */
	    HPROF_ASSERT(tracker_class!=NULL);
	    exceptionClear(env);
	    field = getStaticFieldID(env, tracker_class,
				    TRACKER_ENGAGED_NAME, TRACKER_ENGAGED_SIG);
	    setStaticIntField(env, tracker_class, field, engaged);
	    exceptionClear(env);

	    LOG3("set_engaged()", "tracking engaged", engaged);
	    
	    gdata->tracking_engaged = engaged;
	}
    } rawMonitorExit(gdata->callbackLock);
}

void
tracker_engage(JNIEnv *env)
{
    set_engaged(env, 0xFFFF);
}

void
tracker_disengage(JNIEnv *env)
{
    set_engaged(env, 0);
}

jboolean
tracker_method(jmethodID method)
{
    int      i;

    if ( ! gdata->bci ) {
        return JNI_FALSE;
    }
    
    HPROF_ASSERT(method!=NULL);
    HPROF_ASSERT(gdata->tracker_method_count > 0);
    for ( i = 0 ; i < gdata->tracker_method_count ; i++ ) {
        HPROF_ASSERT(gdata->tracker_methods[i].method!=NULL);
        if ( method == gdata->tracker_methods[i].method ) {
            return JNI_TRUE;
        }
    }
    return JNI_FALSE;
}

static JNINativeMethod registry[4] =
{
        { TRACKER_NEWARRAY_NATIVE_NAME,    TRACKER_NEWARRAY_NATIVE_SIG,
		(void*)&Tracker_nativeNewArray },
        { TRACKER_OBJECT_INIT_NATIVE_NAME, TRACKER_OBJECT_INIT_NATIVE_SIG,
		(void*)&Tracker_nativeObjectInit },
        { TRACKER_CALL_NATIVE_NAME,        TRACKER_CALL_NATIVE_SIG,
		(void*)&Tracker_nativeCallSite },
        { TRACKER_RETURN_NATIVE_NAME,      TRACKER_RETURN_NATIVE_SIG,
		(void*)&Tracker_nativeReturnSite }
};

static struct {
    char *name;
    char *sig;
} tracker_methods[] =
    {
        { TRACKER_NEWARRAY_NAME,           TRACKER_NEWARRAY_SIG            },
        { TRACKER_OBJECT_INIT_NAME,        TRACKER_OBJECT_INIT_SIG         },
        { TRACKER_CALL_NAME,               TRACKER_CALL_SIG                },
        { TRACKER_RETURN_NAME,             TRACKER_RETURN_SIG              },
        { TRACKER_NEWARRAY_NATIVE_NAME,    TRACKER_NEWARRAY_NATIVE_SIG     },
        { TRACKER_OBJECT_INIT_NATIVE_NAME, TRACKER_OBJECT_INIT_NATIVE_SIG  },
        { TRACKER_CALL_NATIVE_NAME,        TRACKER_CALL_NATIVE_SIG         },
        { TRACKER_RETURN_NATIVE_NAME,      TRACKER_RETURN_NATIVE_SIG       } 
    };

void
tracker_setup_class(void)
{
    ClassIndex  cnum;
    LoaderIndex loader_index;
    
    HPROF_ASSERT(gdata->tracker_cnum==0);
    loader_index = loader_find_or_create(NULL,NULL);
    cnum = class_find_or_create(TRACKER_CLASS_SIG, loader_index);
    gdata->tracker_cnum = cnum;
    HPROF_ASSERT(cnum!=0);
    class_add_status(cnum, CLASS_SPECIAL);
}

void
tracker_setup_methods(JNIEnv *env)
{
    ClassIndex  cnum;
    LoaderIndex loader_index;
    int         i;
    jclass      object_class;
    jclass      tracker_class;
   
    if ( ! gdata->bci ) {
        return;
    }

    loader_index = loader_find_or_create(NULL,NULL);
    cnum = class_find_or_create(OBJECT_CLASS_SIG, loader_index);
    object_class = class_get_class(env, cnum);
    tracker_class = class_get_class(env, gdata->tracker_cnum);
    
    CHECK_EXCEPTIONS(env) {
	registerNatives(env, tracker_class, registry, 
				(int)sizeof(registry)/(int)sizeof(registry[0]));
    } END_CHECK_EXCEPTIONS;
    
    HPROF_ASSERT(tracker_class!=NULL);
    
    gdata->tracker_method_count =
        (int)sizeof(tracker_methods)/(int)sizeof(tracker_methods[0]);
    
    HPROF_ASSERT(gdata->tracker_method_count <= 
      (int)(sizeof(gdata->tracker_methods)/sizeof(gdata->tracker_methods[0])));
    
    CHECK_EXCEPTIONS(env) {
	gdata->object_init_method = getMethodID(env, object_class,
				    OBJECT_INIT_NAME, OBJECT_INIT_SIG);
        for ( i=0 ; i < gdata->tracker_method_count ; i++ ) {
            gdata->tracker_methods[i].name =
                        string_find_or_create(tracker_methods[i].name);
            gdata->tracker_methods[i].sig =
                        string_find_or_create(tracker_methods[i].sig);
            gdata->tracker_methods[i].method = 
                      getStaticMethodID(env, tracker_class,
                            tracker_methods[i].name, tracker_methods[i].sig);
            HPROF_ASSERT(gdata->tracker_methods[i].method!=NULL);
            LOG2("tracker_setup_methods(): Found", tracker_methods[i].name);
        }
    } END_CHECK_EXCEPTIONS;
}
