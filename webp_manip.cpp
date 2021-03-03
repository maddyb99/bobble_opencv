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

    WebPMux *mux=WebPMuxNew();
    WebPData chunk_data,webp_data;
    WebPPicture pic;
    WebPDataInit(&chunk_data);
    WebPMuxError error;

    this->readFile("/home/maddyb/Documents/PROJECTS/C++/bobble_opencv/cmake-build-debug/temp/exif_data",&chunk_data);
    if(mux == NULL) {
        std::cout<< "Unable to allocate new WebPMux";
        return;
    }


    std::cout<<"setup config: "<<status<<std::endl;

    for(int i=0;i<frames.size();i++) {
        if(frames[i].channels()==3)
            continue;

        WebPDataInit(&webp_data);

        uint8_t *compressedData;
        WebPMuxFrameInfo info;

        webp_data.size=WebPEncodeBGR(frames[i].data, frames[i].rows, frames[i].cols, frames[i].step, 90, &compressedData);
        webp_data.bytes = compressedData;
        info.bitstream = webp_data;
        info.blend_method = WEBP_MUX_NO_BLEND;
        info.dispose_method = WEBP_MUX_DISPOSE_NONE;
        info.id = WEBP_CHUNK_ANMF;
        info.duration = timestamp_ms;
        info.x_offset = 0;
        info.y_offset = 0;
        status = WebPMuxPushFrame(mux,&info, 1);
        if (!status) {
            std::cout<<"Error while adding frame #"<<i;
        }
        WebPDataClear(&webp_data);
    }

    WebPDataInit(&webp_data);
    WebPMuxAnimParams params;
    params.bgcolor = 0xFFFFFFFF;
    params.loop_count = 0;

    status = status && WebPMuxSetAnimationParams(mux, &params);
    status = status && WebPMuxAssemble(mux, &webp_data);

    std::cout<<"MUX create "<<mux<<std::endl<<"Chunk data: "<<chunk_data.size<<std::endl;
    error=WebPMuxSetChunk(mux,"XMP ",&chunk_data,1);
    std::cout<<"EXIF ERROR "<<error;
    WebPMuxAssemble(mux, &webp_data);

    auto file = std::fstream(video_file_path, std::ios::out | std::ios::binary);
    file.write((char*)&webp_data.bytes[0], webp_data.size);
    file.close();
    WebPMuxDelete(mux);
    WebPDataClear(&webp_data);
}