#include "include/head_api.hpp"
#include <iostream>
#include <curl/curl.h>
HeadApi::HeadApi(const char* gender, const char* filePath){this->filePath=filePath;this->gender=gender;}
void HeadApi::getHead() {
        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if(curl) {
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
            curl_mime_filedata(part, filePath);
            curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
            res = curl_easy_perform(curl);
            std::cout<<res<<std::endl;
            curl_mime_free(mime);
        }
        curl_easy_cleanup(curl);
}