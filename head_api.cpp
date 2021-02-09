#include "include/head_api.hpp"
#include <iostream>
#include <curl/curl.h>

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

HeadApi::HeadApi(const char* gender, const char *filePath){
    this->filePath=filePath;
    this->gender=gender;
    std::cout<<std::endl<<std::endl<<"Initialize: "<<filePath<<this->filePath<<std::endl<<std::endl;
}
void HeadApi::getHeadStatic(const char* gender, const std::string filePath) {
    CURL *curl;
        CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    Json::Value jsonData;
    Json::Reader jsonReader;
    if(curl) {
        std::cout<<"head api input: "<<gender<<filePath<<std::endl;
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, "https://bobblification-api-old.bobbleapp.asia/api/v2/bobble");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_mime *mime;
        curl_mimepart *part;
        mime = curl_mime_init(curl);
        part = curl_mime_addpart(mime);
        curl_mime_name(part, "gender");
        curl_mime_data(part, gender, CURL_ZERO_TERMINATED);
        part = curl_mime_addpart(mime);
        curl_mime_name(part, "image");
        curl_mime_filedata(part, filePath.c_str());
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
//            curl_easy_setopt(curl, CURLOPT_WRITEINFO,)
        res = curl_easy_perform(curl);
        std::cout<<"Response: "<<readBuffer<<std::endl;
        jsonReader.parse(readBuffer,jsonData);
        std::cout<<"Json url: "<<jsonData["faceImageURL"].asString();
//        HeadApi::headUrl=jsonData["faceImageURL"].asString().c_str();
        curl_mime_free(mime);
    }
    curl_easy_cleanup(curl);
}

void HeadApi::getHead() {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    Json::Value jsonData;
    Json::Reader jsonReader;
    if(curl) {
        std::cout<<"head api input: "<<this->gender<<this->filePath<<std::endl;
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, "https://bobblification-api-old.bobbleapp.asia/api/v2/bobble");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_mime *mime;
        curl_mimepart *part;
        mime = curl_mime_init(curl);
        part = curl_mime_addpart(mime);
        curl_mime_name(part, "gender");
        curl_mime_data(part, this->gender, CURL_ZERO_TERMINATED);
        part = curl_mime_addpart(mime);
        curl_mime_name(part, "image");
        curl_mime_filedata(part, this->filePath);
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        std::cout<<"Response: "<<readBuffer<<std::endl;
        jsonReader.parse(readBuffer,jsonData);
        std::cout<<"Json url: "<<jsonData["faceImageURL"].asString();
        this->headUrl=jsonData["faceImageURL"].asString().c_str();
        curl_mime_free(mime);
    }
    curl_easy_cleanup(curl);
}

std::thread HeadApi::spawnGetHead() {
    return std::thread(&HeadApi::getHead,this);
}
void HeadApi::saveHead() {
//    CURL *curl;
//    CURLcode res;
//    curl = curl_easy_init();
//    if(curl) {
//        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
//        curl_easy_setopt(curl, CURLOPT_URL, headUrl);
//        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
//        res = curl_easy_perform(curl);
//        std::cout<<res<<std::endl;
//    }
//    curl_easy_cleanup(curl);
}