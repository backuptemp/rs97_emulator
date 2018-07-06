/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_lti_civil_impl_jni_NativeCaptureStream */

#ifndef _Included_com_lti_civil_impl_jni_NativeCaptureStream
#define _Included_com_lti_civil_impl_jni_NativeCaptureStream
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_lti_civil_impl_jni_NativeCaptureStream
 * Method:    enumVideoFormats
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_com_lti_civil_impl_jni_NativeCaptureStream_enumVideoFormats
  (JNIEnv *, jobject);

/*
 * Class:     com_lti_civil_impl_jni_NativeCaptureStream
 * Method:    setVideoFormat
 * Signature: (Lcom/lti/civil/VideoFormat;)V
 */
JNIEXPORT void JNICALL Java_com_lti_civil_impl_jni_NativeCaptureStream_setVideoFormat
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_lti_civil_impl_jni_NativeCaptureStream
 * Method:    getVideoFormat
 * Signature: ()Lcom/lti/civil/VideoFormat;
 */
JNIEXPORT jobject JNICALL Java_com_lti_civil_impl_jni_NativeCaptureStream_getVideoFormat
  (JNIEnv *, jobject);

/*
 * Class:     com_lti_civil_impl_jni_NativeCaptureStream
 * Method:    setObserver
 * Signature: (Lcom/lti/civil/CaptureObserver;)V
 */
JNIEXPORT void JNICALL Java_com_lti_civil_impl_jni_NativeCaptureStream_setObserver
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_lti_civil_impl_jni_NativeCaptureStream
 * Method:    nativeStart
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_lti_civil_impl_jni_NativeCaptureStream_nativeStart
  (JNIEnv *, jobject);

/*
 * Class:     com_lti_civil_impl_jni_NativeCaptureStream
 * Method:    nativeStop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_lti_civil_impl_jni_NativeCaptureStream_nativeStop
  (JNIEnv *, jobject);

/*
 * Class:     com_lti_civil_impl_jni_NativeCaptureStream
 * Method:    nativeDispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_lti_civil_impl_jni_NativeCaptureStream_nativeDispose
  (JNIEnv *, jobject);

/*
 * Class:     com_lti_civil_impl_jni_NativeCaptureStream
 * Method:    threadMain
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_lti_civil_impl_jni_NativeCaptureStream_threadMain
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
