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
    std::thread th[numFrames];
    std::string filePath;
    for (int i=0;i<numFrames;i++) {
        filePath="/home/maddyb/Downloads/booble/frames/"+std::to_string(i)+".jpg";
//        HeadApi headApi("male", filePath.c_str());
//        headApi.getHead();
        th[i]=std::thread(&HeadApi::getHeadStatic,"male",filePath);
//        th[i].join();
    }
    for (int i=0;i<numFrames;i++)
        th[i].join();
    return 0;
}
