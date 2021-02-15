#include "include/head_api.hpp"
#include "include/webp_manip.hpp"
#include<thread>
int main(int argc, const char* argv[]) {
//    std::vector<cv::Mat> frames;
    if(argc<3){
        std::cout<<"Insufficient parameters";
        exit(1);
    }
    std::string inputFile,outputFile="./temp/final.webp",gender;
    for(int i=1;i<argc;i++){
        if(argv[i][0]=='-'){
            if(!std::strcmp(argv[i],"-o"))
                outputFile=argv[++i];
            else if(!std::strcmp(argv[i],"-g")||!std::strcmp(argv[i],"-gender"))
                gender=argv[++i];
        } else inputFile=argv[i];
    }
    WebpManipulator webpManipulator=WebpManipulator();
    webpManipulator.decode_webp(inputFile);
    int numFrames=webpManipulator.save_frames("./temp/frames/");
    std::cout<<numFrames<<std::endl;
    std::vector<std::thread> th,th2;
    std::string filePath;
    for (int i=0;i<numFrames;i++) {
        filePath="./temp/frames/"+std::to_string(i)+".jpg";
//        HeadApi headApi("male", filePath.c_str());
//        headApi.getHead();
        th.emplace_back(&HeadApi::getHeadStatic,gender.c_str(),filePath,&webpManipulator,i);
//        th[i].join();
    }
    for (int i=0;i<numFrames;i++) {
        th[i].join();
        th2.emplace_back(HeadApi::saveHead,&webpManipulator,i);
    }
    th.clear();
    for (int i=0;i<numFrames;i++)
        th2[i].join();
    std::cout<<"*****sizes*******\n";
    webpManipulator.resize_frames();
    std::cout<<"\n********end sizes*****\n";
    webpManipulator.encode_webp(outputFile);
//        th2[i].join();
    return 0;
}
