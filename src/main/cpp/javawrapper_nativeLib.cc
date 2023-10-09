#include "edu_school_calc_CppLib.h"
#include "calcWrapper.h"
#include <iostream>
#include <string>

std::string jstringToString(JNIEnv *env, jstring jStr) {
    if (!jStr) return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

jstring stringToJstring(JNIEnv *env, std::string nativeString) {
    return env->NewStringUTF(nativeString.c_str());
}

JNIEXPORT jstring JNICALL Java_edu_school_calc_CppLib_getNativeResult
(JNIEnv* env, jobject thisObject, jstring mode, jstring exp, jstring x) {
    std::cout << "WOO-HOO, WE IN >>> Java_edu_school_calc_CppLib_getNativeResult" << std::endl;

    std::string stdString = getNativeResult(
        jstringToString(env, mode),
        jstringToString(env, exp),
        jstringToString(env, x)
    );

    return stringToJstring(env, stdString);
}