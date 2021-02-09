//
// Created by maddyb on 07/02/21.
//
#include <string>
#include <vector>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <map>
#ifndef BOBBLE_OPENCV_WEBP_MANIP_HPP
#define BOBBLE_OPENCV_WEBP_MANIP_HPP
class WebpManipulator{
    std::vector<cv::Mat> frames;
    std::map<int,std::string> headUrls;
public:
    std::vector<cv::Mat> get_frames(){return frames;}
    void extract_frames(const std::string &videoFilePath);
    int save_frames(const std::string &outputDir);
    std::map<int,std::string> get_HeadUrls(){return headUrls;};
    void set_HeadUrls(std::string headUrl, int num){this->headUrls.insert(std::pair<int, std::string>(num,headUrl));};
};
#endif //BOBBLE_OPENCV_WEBP_MANIP_HPP
