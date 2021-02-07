//
// Created by maddyb on 07/02/21.
//
#ifndef BOBBLE_OPENCV_HEAD_API_HPP
#define BOBBLE_OPENCV_HEAD_API_HPP
class HeadApi{
    const char *gender;
    const char *filePath;
public:
    HeadApi(const char* gender, const char* filePath);
    void getHead();
};
#endif //BOBBLE_OPENCV_HEAD_API_HPP
