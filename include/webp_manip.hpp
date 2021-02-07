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
#ifndef BOBBLE_OPENCV_WEBP_MANIP_HPP
#define BOBBLE_OPENCV_WEBP_MANIP_HPP
class WebpManipulator{
    std::vector<cv::Mat> frames;
public:
    std::vector<cv::Mat> get_frames(){return frames;}
    void extract_frames(const std::string &videoFilePath);
    int save_frames(const std::string &outputDir);
};
#endif //BOBBLE_OPENCV_WEBP_MANIP_HPP
