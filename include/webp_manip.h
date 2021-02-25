//
// Created by maddyb on 07/02/21.
//
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <webp/encode.h>
#include <webp/decode.h>
#include <webp/mux.h>
#include <webp/demux.h>
#include <map>
#ifndef BOBBLE_OPENCV_WEBP_MANIP_HPP
#define BOBBLE_OPENCV_WEBP_MANIP_HPP
class WebpManipulator{
    std::vector<cv::Mat> frames;
    std::map<int,std::string> head_urls;
    static int readFile(const char *file_name, WebPData *webp_data);
    void ResizeFrames();
public:
    std::vector<cv::Mat> get_frames(){return frames;}
    void DecodeWebP(const std::string &videoFilePath);
    int SaveFrames(const std::string &outputDir);
    void UpdateFrames(std::vector<uchar>stream, int num);
    void EncodeWebP(const std::string &video_file_path);
    std::map<int,std::string> get_head_urls(){return head_urls;};
    void set_head_urls(const std::string& head_url, int num){this->head_urls.insert(std::pair<int, std::string>(num, head_url));};
};
#endif //BOBBLE_OPENCV_WEBP_MANIP_HPP
