//
// Created by maddyb on 07/02/21.
//

#include "include/webp_manip.hpp"
using namespace  cv;

int readFile(const char* const file_name,
             WebPData *webp_data){
    uint8_t* file_data;
    size_t file_size;
    FILE* in;
    if (in == nullptr) {
        std::cout<< "cannot open input file: "<< file_name;
        return 0;
    }
    fseek(in, 0, SEEK_END);
    file_size = ftell(in);
    fseek(in, 0, SEEK_SET);
    (fread(file_data, file_size, 1, in) == 1);
    fclose(in);
    file_data[file_size] = '\0';  // convenient 0-terminator
    webp_data->bytes = file_data;
    webp_data->size = file_size;
    return 1;
}


void WebpManipulator::DecodeWebP(const std::string &videoFilePath){
    WebPData webp_data;
    WebPDataInit(&webp_data);
    WebPDemuxer* demux = WebPDemux(&webp_data);
    WebPDecoderConfig config;
//    uint32_t width = WebPDemuxGetI(demux, WEBP_FF_CANVAS_WIDTH);
//    uint32_t height = WebPDemuxGetI(demux, WEBP_FF_CANVAS_HEIGHT);
//    uint32_t flags = WebPDemuxGetI(demux, WEBP_FF_FORMAT_FLAGS);
    WebPIterator iter;

    try{
        readFile(videoFilePath.c_str(),&webp_data);
        WebPInitDecoderConfig(&config);
//        int err = WebPValidateConfig(&config);
        if (WebPDemuxGetFrame(demux, 1, &iter)) {
            do {
                WebPDecode(iter.fragment.bytes, iter.fragment.size, &config);
                frames.push_back(imdecode(Mat(config.output.height,config.output.height,CV_8UC3,config.output.private_memory), IMREAD_UNCHANGED));

                // ... (Consume 'iter'; e.g. Decode 'iter.fragment' with WebPDecode(),
                // ... and get other frame properties like width, height, offsets etc.
                // ... see 'struct WebPIterator' below for more info).
            } while (WebPDemuxNextFrame(&iter));
            WebPDemuxReleaseIterator(&iter);
        }

//        VideoCapture cap(videoFilePath); // open the video file
//        if(!cap.isOpened())  // check if we succeeded
//            CV_Error(1, "Can not open Video file");
//        //cap.get(CV_CAP_PROP_FRAME_COUNT) contains the number of frames in the video;
//        for(int frameNum = 0; frameNum < cap.get(CAP_PROP_FRAME_COUNT);frameNum++)
//        {
//            std::cout<<frameNum;
//            Mat frame;
//            cap >> frame; // get the next frame from video
////            cvtColor(frame,frame,COLOR_BGR2BGRA);
//            frames.push_back(frame);
//        }
    }
    catch( cv::Exception& e ){
        std::cerr <<"error:"<< e.msg << std::endl;
        exit(1);
    }
}

int WebpManipulator::SaveFrames(const std::string& outputDir){
    std::vector<int> compression_params;
    compression_params.push_back(IMWRITE_JPEG_QUALITY);
    compression_params.push_back(100);
    int frame_num;
    std::vector<Mat>::iterator frame;

    for(frame = frames.begin(), frame_num=0; frame != frames.end(); ++frame,++frame_num){
        std::string filePath = outputDir + std::to_string(static_cast<long long>(frame_num)) + ".jpg";
        imwrite(filePath,*frame,compression_params);
    }
    return frame_num;
}

int WebpManipulator::ResizeFrames() {
    for(int frame_num=0; frame_num < frames.size(); frame_num++){
        int dim=512;
//        std::string filePath = outputDir + std::to_string(frame_num)+ ".jpg";
        Mat img=frames[frame_num];
        if(! img.data )                              // Check for invalid input
        {
            std::cout << "Could not open or find the image" << frame_num << std::endl ;
            return 0;
        }
        std::cout<<img.rows<<img.cols<<std::endl;
        Mat new_img;
        new_img.create(dim, dim, img.type());
        new_img.setTo(Scalar::all(0));
        std::cout<<"new\n";
        try {
            img.copyTo(new_img(Rect((dim - img.cols) / 2, (dim - img.rows) / 2, img.cols, img.rows)));
        }
        catch (Exception err){
            std::cout<<"err";
        }
        std::cout<<"before write new\n";
//        std::string filePath = outputDir + std::to_string(static_cast<long long>(frame_num))+ ".png";
        frames[frame_num]=new_img;
//        imwrite(filePath,img);
//        std::cout<<std::endl<<img.empty();

    }
}

void WebpManipulator::UpdateFrames(std::vector<uchar>stream, int num){
    Mat frame=imdecode(Mat(stream), IMREAD_UNCHANGED);
    frames[num]=frame;
}

int WebpManipulator::EncodeWebP(const std::string &video_file_path){
    std::cout << video_file_path << std::endl;
    int status=1,height,width,timestamp_ms=0;
//    FILE *out;
    WebPAnimEncoderOptions encoder_options;
    WebPAnimEncoder *encoder= nullptr;
    WebPConfig config;
    WebPData webp_data;
    WebPPicture pic;
    WebPDataInit(&webp_data);
    if (!WebPAnimEncoderOptionsInit(&encoder_options) ||
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
//    encoder_options.allow_mixed=0;
//    encoder_options.anim_params.bgcolor=0;
    int err = WebPValidateConfig(&config);
    std::cout<<"setup config: "<<err<<std::endl;

    for(int i=0;i<frames.size();i++) {
        if(frames[i].channels()==3)
            continue;
        pic.height=frames[i].rows;
        pic.width=frames[i].cols;
        WebPPictureImportBGRA(&pic, frames[i].data, frames[i].step);
        if (encoder == nullptr) {
            width  = pic.width;
            height = pic.height;
            encoder = WebPAnimEncoderNew(width, height, &encoder_options);
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

    auto file = std::fstream(video_file_path, std::ios::out | std::ios::binary);
    file.write((char*)&webp_data.bytes[0], webp_data.size);
    file.close();

    WebPDataClear(&webp_data);
}