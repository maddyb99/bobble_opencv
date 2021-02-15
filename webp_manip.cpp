//
// Created by maddyb on 07/02/21.
//

#include "include/webp_manip.hpp"
using namespace  cv;
void WebpManipulator::decode_webp(const std::string &videoFilePath){
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
//            cvtColor(frame,frame,COLOR_BGR2BGRA);
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

int WebpManipulator::resize_frames() {
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
        Mat newImg;
        newImg.create(dim,dim,img.type());
        newImg.setTo(Scalar::all(0));
        std::cout<<"new\n";
        try {
            img.copyTo(newImg(Rect((dim-img.cols)/2, (dim-img.rows)/2, img.cols, img.rows)));
        }
        catch (Exception err){
            std::cout<<"err";
        }
        std::cout<<"before write new\n";
//        std::string filePath = outputDir + std::to_string(static_cast<long long>(frameNumber))+ ".png";
        frames[frameNumber]=newImg;
//        imwrite(filePath,img);
//        std::cout<<std::endl<<img.empty();

    }
}

void WebpManipulator::update_frame(std::vector<uchar>stream,int num){
//    if(stream.empty()){
//        frames.erase(frames.at(num));
//    }
    Mat frame=imdecode(Mat(stream), IMREAD_UNCHANGED);
    frames[num]=frame;
}

int WebpManipulator::encode_webp(const std::string &videoFilePath){
    std::cout<<videoFilePath<<std::endl;
    int status=1,height,width,timestamp_ms=0;
//    FILE *out;
    WebPAnimEncoderOptions encoderOptions;
    WebPAnimEncoder *encoder= nullptr;
    WebPConfig config;
    WebPData webp_data;
    WebPPicture pic;
    WebPDataInit(&webp_data);
    if (!WebPAnimEncoderOptionsInit(&encoderOptions) ||
        !WebPConfigInit(&config) ||
        !WebPPictureInit(&pic)) {
        fprintf(stderr, "Library version mismatch!\n");
        status = 0;
    }
//    if (!WebPConfigPreset(&config, WEBP_PRESET_PHOTO,90)) return 0;   // version error
    config.sns_strength = 90;
    config.filter_sharpness = 6;
    config.alpha_quality = 90;
//    config.lossless=1;
//    encoderOptions.allow_mixed=0;
//    encoderOptions.anim_params.bgcolor=0;
    int err = WebPValidateConfig(&config);
    std::cout<<"setup config: "<<err<<std::endl;

    for(int i=0;i<frames.size();i++) {
        pic.height=frames[i].rows;
        pic.width=frames[i].cols;
        WebPPictureImportBGRA(&pic, frames[i].data, frames[i].step);
        if (encoder == nullptr) {
            width  = pic.width;
            height = pic.height;
            encoder = WebPAnimEncoderNew(width, height, &encoderOptions);
            status = (encoder != nullptr);
            if (!status) {
                fprintf(stderr, "Could not create WebPAnimEncoder object.\n");
            }
        }

        if (status) {
            status = WebPAnimEncoderAdd(encoder, &pic, timestamp_ms, &config);
            if (!status) {
                fprintf(stderr, "Error while adding frame #%d\n", i);
            }
        }
        timestamp_ms+=100;
        WebPPictureFree(&pic);
    }

    status = status && WebPAnimEncoderAdd(encoder, NULL, timestamp_ms, NULL);
    status =  status && WebPAnimEncoderAssemble(encoder, &webp_data);
    WebPAnimEncoderDelete(encoder);
    auto myfile = std::fstream(videoFilePath, std::ios::out | std::ios::binary);
    myfile.write((char*)&webp_data.bytes[0], webp_data.size);
    myfile.close();

    WebPDataClear(&webp_data);
}