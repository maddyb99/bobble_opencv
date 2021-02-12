//
// Created by maddyb on 07/02/21.
//
#include <string>
#include <vector>
#include <fstream>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <webp/encode.h>
#include <webp/decode.h>
#include <webp/mux.h>
//#include <image>
//#include <webp/mux_types.h>
//#include <webp/types.h>
#include <map>
#ifndef BOBBLE_OPENCV_WEBP_MANIP_HPP
#define BOBBLE_OPENCV_WEBP_MANIP_HPP
class WebpManipulator{
    std::vector<cv::Mat> frames;
    std::map<int,std::string> headUrls;
public:
    std::vector<cv::Mat> get_frames(){return frames;}
    void decode_webp(const std::string &videoFilePath);
    int save_frames(const std::string &outputDir);
    int resize_frames();
    void update_frame(std::vector<uchar>stream,int num);
    int encode_webp(const std::string &videoFilePath);
    std::map<int,std::string> get_HeadUrls(){return headUrls;};
    void set_HeadUrls(std::string headUrl, int num){this->headUrls.insert(std::pair<int, std::string>(num,headUrl));};
};
#endif //BOBBLE_OPENCV_WEBP_MANIP_HPP
