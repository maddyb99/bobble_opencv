//
// Created by maddyb on 07/02/21.
//

#include "include/webp_manip.hpp"
using namespace  cv;
void WebpManipulator::extract_frames(const std::string &videoFilePath){
    try{
        //open the video file
        VideoCapture cap(videoFilePath); // open the video file
        if(!cap.isOpened())  // check if we succeeded
            CV_Error(1, "Can not open Video file");

        //cap.get(CV_CAP_PROP_FRAME_COUNT) contains the number of frames in the video;
        for(int frameNum = 0; frameNum < cap.get(CAP_PROP_FRAME_COUNT);frameNum++)
        {
            std::cout<<frameNum;
            Mat frame;
            cap >> frame; // get the next frame from video
            frames.push_back(frame);
        }
    }
    catch( cv::Exception& e ){
        std::cerr <<"error:"<< e.msg << std::endl;
        exit(1);
    }
}

int WebpManipulator::save_frames(const std::string& outputDir){
    std::vector<int> compression_params;
    compression_params.push_back(IMWRITE_JPEG_QUALITY);
    compression_params.push_back(100);
    int frameNumber;
    std::vector<Mat>::iterator frame;

    for(frame = frames.begin(),frameNumber=0; frame != frames.end(); ++frame,++frameNumber){
        std::string filePath = outputDir + std::to_string(static_cast<long long>(frameNumber))+ ".jpg";
        imwrite(filePath,*frame,compression_params);
    }
    return frameNumber;
}

