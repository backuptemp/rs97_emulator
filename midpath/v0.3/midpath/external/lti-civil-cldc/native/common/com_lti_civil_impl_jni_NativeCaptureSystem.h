/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_lti_civil_impl_jni_NativeCaptureSystem */

#ifndef _Included_com_lti_civil_impl_jni_NativeCaptureSystem
#define _Included_com_lti_civil_impl_jni_NativeCaptureSystem
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_lti_civil_impl_jni_NativeCaptureSystem
 * Method:    dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_lti_civil_impl_jni_NativeCaptureSystem_dispose
  (JNIEnv *, jobject);

/*
 * Class:     com_lti_civil_impl_jni_NativeCaptureSystem
 * Method:    getCaptureDeviceInfoList
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_com_lti_civil_impl_jni_NativeCaptureSystem_getCaptureDeviceInfoList
  (JNIEnv *, jobject);

/*
 * Class:     com_lti_civil_impl_jni_NativeCaptureSystem
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_lti_civil_impl_jni_NativeCaptureSystem_init
  (JNIEnv *, jobject);

/*
 * Class:     com_lti_civil_impl_jni_NativeCaptureSystem
 * Method:    openCaptureDeviceStream
 * Signature: (Ljava/lang/String;)Lcom/lti/civil/CaptureStream;
 */
JNIEXPORT jobject JNICALL Java_com_lti_civil_impl_jni_NativeCaptureSystem_openCaptureDeviceStream
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif
