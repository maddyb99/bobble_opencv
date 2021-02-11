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

int WebpManipulator::resize_frames(const std::string &outputDir) {
    for(int frameNumber=0;frameNumber<frames.size();frameNumber++){
        int dim=512;
//        std::string filePath = outputDir + std::to_string(frameNumber)+ ".jpg";
        Mat img=frames[frameNumber];
        if(! img.data )                              // Check for invalid input
        {
            std::cout <<  "Could not open or find the image" <<frameNumber<< std::endl ;
            return 0;
        }
        std::cout<<img.rows<<img.cols<<std::endl;
        Scalar color = (255,255,255);
        Mat newImg;
        newImg.create(dim,dim,img.type());
        newImg.setTo(Scalar::all(255));
        std::cout<<"new\n";
        try {
            img.copyTo(newImg(Rect((dim-img.cols)/2, (dim-img.rows)/2, img.cols, img.rows)));
        }
        catch (Exception err){
            std::cout<<"err";
        }
        std::cout<<"before write new\n";
        std::string filePath = outputDir + std::to_string(static_cast<long long>(frameNumber))+ ".jpg";
        imwrite(filePath,newImg);
        std::cout<<std::endl<<img.empty();

    }
}

void WebpManipulator::update_frame(std::vector<uchar>stream,int num){
    Mat frame=imdecode(Mat(stream), 1);
    frames[num]=frame;
}