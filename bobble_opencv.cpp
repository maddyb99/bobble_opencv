//
// Created by maddyb on 22/2/21.
//

#include <jni.h>
#include <webp_manip.hpp>
#include <android/log.h>
#include <android/asset_manager.h>

extern "C" JNIEXPORT jlong JNICALL
Java_tech_maddybcorp_bobbleopencv_MainActivity_webPObjectInit(JNIEnv *env, jobject /*this*/, jstring path, jstring cache_path) {
    const char *nativeString = env->GetStringUTFChars(path, 0);


    std::string newPath = std::string(nativeString);
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
Java_tech_maddybcorp_bobbleopencv_MainActivity_webPCountFrames(JNIEnv *env, jobject /*this*/, jlong decoder) {

    WebpManipulator *webpManipulator= reinterpret_cast<WebpManipulator *>(decoder);

    return webpManipulator->get_frames().size();
};

extern "C" JNIEXPORT jlong JNICALL
Java_tech_maddybcorp_bobbleopencv_MainActivity_webPUpdateFrames(JNIEnv *env, jobject /*this*/, jbyteArray frame, jint num, jlong decoder) {

    __android_log_print(ANDROID_LOG_DEBUG, "UPDATE FRAMES", "%s", "START");

    int len = env->GetArrayLength (frame);
    unsigned char* buf = new unsigned char[len];
    env->GetByteArrayRegion (frame, 0, len, reinterpret_cast<jbyte*>(buf));
    std::vector<uchar> newFrame(buf,buf+len);
    __android_log_print(ANDROID_LOG_DEBUG, "UPDATE FRAMES", "%s", "cast frames");
    WebpManipulator *webpManipulator= reinterpret_cast<WebpManipulator *>(decoder);
    webpManipulator->UpdateFrames(newFrame,num);

    __android_log_print(ANDROID_LOG_DEBUG, "UPDATE FRAMES", "%s", ("new frame num rows"+std::to_string(webpManipulator->get_frames()[num].rows)).c_str());

    return reinterpret_cast<jlong>(webpManipulator);
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
//        webpManipulator->SaveFrames(newPath+"/temp/");
        newPath = std::string(nativeString)+ "/finalWebP.webp";
        webpManipulator->EncodeWebP(newPath);
        __android_log_print(ANDROID_LOG_DEBUG, "MERGE FRAMES", "%s", ("Complete encoding: "+newPath).c_str());
    }
    return env->NewStringUTF(newPath.c_str());
};
