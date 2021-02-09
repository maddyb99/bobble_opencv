#include "include/head_api.hpp"
#include "include/webp_manip.hpp"
#include<thread>
int main() {
//    std::vector<cv::Mat> frames;
    std::string file="/home/maddyb/Downloads/booble/gif_1611561785696.mp4";
    WebpManipulator webpManipulator=WebpManipulator();
    webpManipulator.extract_frames(file);
    int numFrames=webpManipulator.save_frames("/home/maddyb/Downloads/booble/frames/");
    std::cout<<numFrames<<std::endl;
    std::vector<std::thread> th,th2;
    std::string filePath;
    for (int i=0;i<numFrames;i++) {
        filePath="/home/maddyb/Downloads/booble/frames/"+std::to_string(i)+".jpg";
//        HeadApi headApi("male", filePath.c_str());
//        headApi.getHead();
        th.emplace_back(&HeadApi::getHeadStatic,"male",filePath,&webpManipulator,i);
//        th[i].join();
    }
    for (int i=0;i<numFrames;i++) {
        th[i].join();
//        th.emplace_back(std::thread(&HeadApi::saveHead,&webpManipulator,i));
    }
    for (int i=0;i<numFrames;i++){

        std::cout << webpManipulator.get_HeadUrls().at(i);
        HeadApi::saveHead(&webpManipulator,i);
    }
//        th2[i].join();


    return 0;
}
