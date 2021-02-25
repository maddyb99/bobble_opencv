//
// Created by maddyb on 07/02/21.
//

#include "include/webp_manip.h"
#include <fstream>

using namespace  cv;

int WebpManipulator::readFile(const char* const file_name,
             WebPData *webp_data){
    uint8_t* file_data;
    size_t file_size;
    std::ifstream in;
    in.open(file_name,std::ios::in|std::ios::binary);
    if (!in.is_open()) {
        std::cout<< "cannot open input file: "<< file_name<<std::endl;
        return 0;
    }
    in.seekg(0,std::ios::end);
    file_size = in.tellg();
    in.seekg(0,std::ios::beg);
    file_data = (uint8_t *)malloc((file_size+1) * sizeof(uint8_t));
    in.read(reinterpret_cast<char *>(file_data), file_size);
//    std::cout<<"DID READ: "<<(in.read(reinterpret_cast<char *>(file_data), file_size) == 1);
    in.close();
    file_data[file_size] = '\0';  // convenient 0-terminator
    webp_data->bytes = file_data;
    webp_data->size = file_size;
    return 1;
}

void WebpManipulator::DecodeWebP(const std::string &videoFilePath){
    WebPData webp_data;
    WebPDataInit(&webp_data);
    WebPDemuxState state;
    this->readFile(videoFilePath.c_str(),&webp_data);
    WebPDemuxer* demux = WebPDemuxPartial(&webp_data,&state);
    WebPDecoderConfig config;
    WebPIterator iter;
    try{
        WebPInitDecoderConfig(&config);
        if (WebPDemuxGetFrame(demux, 1, &iter)) {
            do {
                WebPDecode(iter.fragment.bytes, iter.fragment.size, &config);
                Mat mat=Mat(config.output.height,config.output.height,CV_8UC3,config.output.private_memory);
                cvtColor(mat,mat,COLOR_RGB2BGR);
                frames.push_back(mat);
                WebPInitDecoderConfig(&config);
            } while (WebPDemuxNextFrame(&iter));
            WebPDemuxReleaseIterator(&iter);
        }
    }
    catch( cv::Exception& e ){
        std::cerr <<"error:"<< e.msg << std::endl;
        exit(1);
    }
}

int WebpManipulator::SaveFrames(const std::string& outputDir){
    std::vector<int> compression_params;
    compression_params.push_back(1);
    compression_params.push_back(100);
    int frame_num;
    std::vector<Mat>::iterator frame;
    for(frame = frames.begin(), frame_num=0; frame != frames.end(); ++frame,++frame_num){
        std::string filePath = outputDir + std::to_string(static_cast<long long>(frame_num)) + ".jpg";
        imwrite(filePath,*frame,compression_params);
    }
    return frame_num;
}

void WebpManipulator::ResizeFrames() {
    for(int frame_num=0; frame_num < frames.size(); frame_num++){
        int dim=512;
        Mat img=frames[frame_num];
        if(! img.data )                              // Check for invalid input
        {
            std::cout << "Could not open or find the image" << frame_num << std::endl ;
            return;
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
        frames[frame_num]=new_img;
    }
}

void WebpManipulator::UpdateFrames(std::vector<uchar>stream, int num){
    Mat frame=imdecode(Mat(stream), -1);
    frames[num]=frame;
}

void WebpManipulator::EncodeWebP(const std::string &video_file_path){
    std::cout << video_file_path << std::endl;
    int status=1,height,width,timestamp_ms=0;
//    FILE *out;
    this->ResizeFrames();
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
    config.sns_strength = 90;
    config.filter_sharpness = 6;
    config.alpha_quality = 90;
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