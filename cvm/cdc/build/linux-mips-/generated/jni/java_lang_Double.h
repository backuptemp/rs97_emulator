/* DO NOT EDIT THIS FILE - it is machine generated */
#include "javavm/export/jni.h"
/* Header for class java/lang/Double */

#ifndef _CVM_JNI_java_lang_Double
#define _CVM_JNI_java_lang_Double
#undef java_lang_Double_POSITIVE_INFINITY
#define java_lang_Double_POSITIVE_INFINITY InfinityD
#undef java_lang_Double_NEGATIVE_INFINITY
#define java_lang_Double_NEGATIVE_INFINITY -InfinityD
#undef java_lang_Double_NaN
#define java_lang_Double_NaN NaND
#undef java_lang_Double_MAX_VALUE
#define java_lang_Double_MAX_VALUE 1.7976931348623157E308D
#undef java_lang_Double_serialVersionUID
#define java_lang_Double_serialVersionUID -9172774392245257468LL
#ifdef __cplusplus
extern "C"{
#endif
/*
 * Class:	java/lang/Double
 * Method:	doubleToLongBits
 * Signature:	(D)J
 */
JNIEXPORT jlong JNICALL Java_java_lang_Double_doubleToLongBits
  (JNIEnv *, jclass, jdouble);

/*
 * Class:	java/lang/Double
 * Method:	doubleToRawLongBits
 * Signature:	(D)J
 */
JNIEXPORT jlong JNICALL Java_java_lang_Double_doubleToRawLongBits
  (JNIEnv *, jclass, jdouble);

/*
 * Class:	java/lang/Double
 * Method:	longBitsToDouble
 * Signature:	(J)D
 */
JNIEXPORT jdouble JNICALL Java_java_lang_Double_longBitsToDouble
  (JNIEnv *, jclass, jlong);

#ifdef __cplusplus
}
#endif
#endif
