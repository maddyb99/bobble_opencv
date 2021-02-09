//
// Created by maddyb on 07/02/21.
//
#ifndef BOBBLE_OPENCV_HEAD_API_HPP
#define BOBBLE_OPENCV_HEAD_API_HPP
#include <string>
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <thread>
#include "webp_manip.hpp"
class HeadApi{
//    void getHead();
public:
//    std::thread spawnGetHead();
    static void getHeadStatic(const char* gender, std::string filePath,WebpManipulator* webpManipulator,int num);
    static void saveHead(WebpManipulator* webpManipulator,int num);
};
#endif //BOBBLE_OPENCV_HEAD_API_HPP
