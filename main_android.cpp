//
// Created by maddyb on 22/2/21.
//

#include <jni.h>
#include <webp_manip.hpp>
#include <android/log.h>

//template <typename T, int N>
//char (&ArraySizeHelper(T (&array)[N]))[N];
//#define NELEMS(x) (sizeof(ArraySizeHelper(x)))
//
//int register_BinaryDictionary(JNIEnv *env);

extern "C" JNIEXPORT jlong JNICALL
Java_tech_maddybcorp_bobbleopencv_MainActivity_WebPObject(JNIEnv *env, jobject /*this*/, jstring path) {
    const char *nativeString = env->GetStringUTFChars(path, 0);

    std::string newPath = std::string(nativeString);
    WebpManipulator webpManipulator=WebpManipulator();
    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "%s", ("PATH IN C++: "+newPath).c_str());
//    webpManipulator.DecodeWebP(newPath);

    return reinterpret_cast<jlong>(&webpManipulator);
};

//jint JNI_OnLoad(JavaVM *vm, void *reserved){
//    JNIEnv *env = 0;
//
//    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
////        AKLOGE("ERROR: GetEnv failed");
//        return -1;
//    }
//
////    ASSERT(env);
//    if (!env) {
////        AKLOGE("ERROR: JNIEnv is invalid");
//        return -1;
//    }
//    if(!register_BinaryDictionary(env))
//        return -1;
//    return JNI_VERSION_1_6;
//}
//
//static const JNINativeMethod sMethods[] = {
//        {
//                const_cast<char *>("WebPObject"),
//                const_cast<char *>("(Ljava/lang/String;JJZ)J"),
//                reinterpret_cast<void *>(WebPObject)
//        },
//};
//int registerNativeMethods(JNIEnv *env, const char *const className, const JNINativeMethod *methods,
//                          const int numMethods) {
//    jclass clazz = env->FindClass(className);
//    if (!clazz) {
////        AKLOGE("Native registration unable to find class '%s'", className);
//        return JNI_FALSE;
//    }
//    if (env->RegisterNatives(clazz, methods, numMethods) != 0) {
////        AKLOGE("RegisterNatives failed for '%s'", className);
//        env->DeleteLocalRef(clazz);
//        return JNI_FALSE;
//    }
//    env->DeleteLocalRef(clazz);
//    return JNI_TRUE;
//}
//int register_BinaryDictionary(JNIEnv *env) {
//    const char *const kClassPathName = "com/android/inputmethod/latin/BinaryDictionary";
//    return registerNativeMethods(env, kClassPathName, sMethods, NELEMS(sMethods));
//}
