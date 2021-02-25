//
// Created by maddyb on 07/02/21.
//
#ifndef BOBBLE_OPENCV_HEAD_API_H
#define BOBBLE_OPENCV_HEAD_API_H
#include <string>
#include <iostream>
#include <json/json.h>
#include <curl/curl.h>
#include <thread>
#include "webp_manip.h"
class HeadApi{
public:
    static void GetHeadUrl(const char* gender, std::string filePath, WebpManipulator* webpManipulator, int num);
    static void GetHead(WebpManipulator* webpManipulator, int num);
    //Remove one of these 2 functions and move conversion to driver code
    static size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata);
    static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp);
};
#endif //BOBBLE_OPENCV_HEAD_API_H
