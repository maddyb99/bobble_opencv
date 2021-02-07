#include "include/head_api.hpp"
#include "include/webp_manip.hpp"
int main() {
//    std::vector<cv::Mat> frames;
    std::string file="/home/maddyb/Downloads/booble/gif_1611561785696.mp4";
    WebpManipulator webpManipulator=WebpManipulator();
    webpManipulator.extract_frames(file);
    int numFrames=webpManipulator.save_frames("/home/maddyb/Downloads/booble/frames/");
    for (int i=0;i<numFrames;i++) {
        std::string filePath="/home/maddyb/Downloads/booble/frames/"+std::to_string(i)+".jpg";
        HeadApi headApi("male", filePath.c_str());
        headApi.getHead();
    }
//    std::cout << "Hello, World!" << std::endl;
    return 0;
}
