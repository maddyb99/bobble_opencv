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
        frames[frameNumber]=newImg;
        imwrite(filePath,newImg);
        std::cout<<std::endl<<img.empty();

    }
}

void WebpManipulator::update_frame(std::vector<uchar>stream,int num){
    Mat frame=imdecode(Mat(stream), 1);
    frames[num]=frame;
}

//static int MyWriter(const uint8_t* data, size_t data_size,
//                    FILE *fp) {
//    FILE* const out = fp;
//    return  (fwrite(data, data_size, 1, out) == 1) ;
//}

//static int ReadImage(const char filename[], WebPPicture* const pic) {
//    const uint8_t* data = NULL;
//    size_t data_size = 0;
//
//    WebPImageReader reader;
//    int ok;
//#ifdef HAVE_WINCODEC_H
//    // Try to decode the file using WIC falling back to the other readers for
//  // e.g., WebP.
//  ok = ReadPictureWithWIC(filename, pic, 1, NULL);
//  if (ok) return 1;
//#endif
//    if (!ImgIoUtilReadFile(filename, &data, &data_size)) return 0;
//    reader = WebPGuessImageReader(data, data_size);
//    ok = reader(data, data_size, pic, 1, NULL);
//    WebPFree((void*)data);
//    return ok;
//}
int WebpManipulator::encode_webp(const std::string &videoFilePath){
    std::cout<<videoFilePath<<std::endl;
    int status,height,width,timestamp_ms=0;
    FILE *out;
    WebPAnimEncoderOptions encoderOptions;
    WebPAnimEncoder *encoder;
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
    int err = WebPValidateConfig(&config);
    std::cout<<"setup config: "<<err<<std::endl;

//    WebPPicture pic;
//    if (!WebPPictureInit(&pic)) return 0;
    for(int i=0;i<frames.size();i++) {
        pic.height=frames[i].rows;
        pic.width=frames[i].cols;
        WebPPictureImportRGB(&pic, frames[i].data, frames[i].step);
        if (encoder == NULL) {
            width  = pic.width;
            height = pic.height;
            encoder = WebPAnimEncoderNew(width, height, &encoderOptions);
            status = (encoder != NULL);
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
//        pic.custom_ptr = out;
//        pic.writer = MyWriter;
//        std::cout << "about to encode\n";
////    WebPWri
//        int status=WebPEncode(&config, &pic);
//        std::cout<<"\n status : "<<status;
//        if(!status)
//            std::cout<<"\n error: "<<pic.error_code<<std::endl;
        timestamp_ms+=100;
        WebPPictureFree(&pic);
    }

    status = status && WebPAnimEncoderAdd(encoder, NULL, timestamp_ms, NULL);
    status =  WebPAnimEncoderAssemble(encoder, &webp_data);
    WebPAnimEncoderDelete(encoder);
    fopen(videoFilePath.c_str(),"wb");
    fwrite(webp_data.bytes,webp_data.size,1,out);
//    MyWriter(webp_data.bytes,webp_data.size,out);
    fclose(out);

//    auto myfile = std::fstream("file.binary", std::ios::out | std::ios::binary);
//    myfile.write((char*)&webp_data.bytes[0], webp_data.size);
//    myfile.close();

    WebPDataClear(&webp_data);
}