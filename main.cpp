#include "include/head_api.h"
#include "include/webp_manip.h"
#include <thread>
int main(int argc, const char* argv[]) {
    std::string input_file="/home/maddyb/Downloads/booble/gif_1611561785696.webp",output_file="./temp/final.webp",gender="male";
    for(int i=1;i<argc;i++){
        if(argv[i][0]=='-'){
            if(!std::strcmp(argv[i],"-o"))
                output_file=argv[++i];
            else if(!std::strcmp(argv[i],"-g")||!std::strcmp(argv[i],"-gender"))
                gender=argv[++i];
        } else input_file=argv[i];
    }
    WebpManipulator webp_manipulator=WebpManipulator();
    webp_manipulator.DecodeWebP(input_file);
    int num_frames= webp_manipulator.SaveFrames("./temp/frames/");
    std::cout << num_frames << std::endl;
    std::vector<std::thread> th,th2;
    std::string file_path;
    for (int i=0; i < num_frames; i++) {
        file_path= "./temp/frames/" + std::to_string(i) + ".jpg";
        th.emplace_back(&HeadApi::GetHeadUrl, gender.c_str(), file_path, &webp_manipulator, i);
    }
    for (int i=0; i < num_frames; i++) {
        th[i].join();
        th2.emplace_back(HeadApi::GetHead, &webp_manipulator, i);
    }
    th.clear();
    for (int i=0; i < num_frames; i++)
        th2[i].join();
    webp_manipulator.EncodeWebP(output_file);
    return 0;
}
