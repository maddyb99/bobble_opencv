//
// Created by maddyb on 22/2/21.
//

#include <jni.h>
#include <webp_manip.hpp>
#include <android/log.h>
#include <android/asset_manager.h>
//#include <android/asset_manager.h>

//template <typename T, int N>
//char (&ArraySizeHelper(T (&array)[N]))[N];
//#define NELEMS(x) (sizeof(ArraySizeHelper(x)))
//
//int register_BinaryDictionary(JNIEnv *env);

extern "C" JNIEXPORT jlong JNICALL
Java_tech_maddybcorp_bobbleopencv_MainActivity_webPObjectInit(JNIEnv *env, jobject /*this*/, jstring path, jstring cache_path) {
    const char *nativeString = env->GetStringUTFChars(path, 0);


    std::string newPath = std::string(nativeString);
//AAssetManager_open(mngr,newPath.c_str(),AASSET_MODE_BUFFER);
//android_fo
    WebpManipulator *webpManipulator=new WebpManipulator();
    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "%s", ("PATH IN C++: "+newPath).c_str());
    webpManipulator->DecodeWebP(newPath);
    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "%s", ("Decoded frames: "+std::to_string(webpManipulator->get_frames().size())).c_str());
    nativeString = env->GetStringUTFChars(cache_path, 0);
    newPath = std::string(nativeString);
    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "%s", ("CACHE PATH IN C++: "+newPath).c_str());
    webpManipulator->SaveFrames(newPath+"/");

    return reinterpret_cast<jlong>(webpManipulator);
};

extern "C" JNIEXPORT jlong JNICALL
Java_tech_maddybcorp_bobbleopencv_MainActivity_webPUpdateFrames(JNIEnv *env, jobject /*this*/, jstring frame, jint num, jlong decoder) {

    const char *nativeString = env->GetStringUTFChars(frame, 0);
    std::vector<uchar> newFrame;
    for(int i=0;nativeString[i]!=NULL;i++)
        newFrame.emplace_back(nativeString[i]);
    WebpManipulator webpManipulator=*(reinterpret_cast<WebpManipulator*>(decoder));
    webpManipulator.UpdateFrames(newFrame,num);

    return reinterpret_cast<jlong>(&webpManipulator);
};

extern "C" JNIEXPORT jstring JNICALL
Java_tech_maddybcorp_bobbleopencv_MainActivity_webPMergeFrames(JNIEnv *env, jobject /*this*/, jstring path, jlong decoder) {

    __android_log_print(ANDROID_LOG_DEBUG, "MERGE FRAMES", "%s", "START");
    const char *nativeString = env->GetStringUTFChars(path, 0);
    std::string newPath = std::string(nativeString);
    __android_log_print(ANDROID_LOG_DEBUG, "MERGE FRAMES", "%s", ("NEW WebP Path: "+newPath).c_str());
    WebpManipulator *webpManipulator= reinterpret_cast<WebpManipulator *>(decoder);
    if(webpManipulator!=NULL) {
        __android_log_print(ANDROID_LOG_DEBUG, "MERGE FRAMES", "%s", "SUCCESS");
        __android_log_print(ANDROID_LOG_DEBUG, "MERGE FRAMES", "%s", ("IS DECODED: " +
                                                                      std::to_string(
                                                                              webpManipulator->get_frames().size())).c_str());
        newPath = std::string(nativeString)+ "/finalWebP.webp";
        webpManipulator->EncodeWebP(newPath);
        __android_log_print(ANDROID_LOG_DEBUG, "MERGE FRAMES", "%s", ("Complete encoding: "+newPath).c_str());
    }
    return env->NewStringUTF(newPath.c_str());
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
