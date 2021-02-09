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

class HeadApi{
    const char *gender;
    const char *filePath;
    const char *headUrl;
    void getHead();
public:
    HeadApi(const char* gender, const char* filePath);
    std::thread spawnGetHead();
    static void getHeadStatic(const char* gender, std::string filePath);
    void saveHead();
};
#endif //BOBBLE_OPENCV_HEAD_API_HPP
