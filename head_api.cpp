#include "include/head_api.h"

size_t HeadApi::WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void HeadApi::GetHeadUrl(const char* gender, std::string filePath, WebpManipulator* webpManipulator, int num) {
    CURL *curl;
        CURLcode res;
    std::string readBuffer;
    long http_code = 0;
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
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HeadApi::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        res = curl_easy_perform(curl);
        std::cout<<"Response: "<<num<<": "<<http_code<<": "<<readBuffer<<std::endl;
        jsonReader.parse(readBuffer,jsonData);
        std::cout << "Json url: " << jsonData["faceImageURL"].asString() << std::endl;
        webpManipulator->set_head_urls(jsonData["faceImageURL"].asString(), num);
        HeadApi::GetHead(webpManipulator,num);
        curl_mime_free(mime);
    }
    curl_easy_cleanup(curl);
}

size_t HeadApi::write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    std::vector<uchar> *stream = (std::vector<uchar>*)userdata;
    size_t count = size * nmemb;
    stream->insert(stream->end(), ptr, ptr + count);
    return count;
}

void HeadApi::GetHead(WebpManipulator* webpManipulator, int num) {
    if(webpManipulator->get_head_urls().at(num).empty())
        return;
    std::vector<uchar> stream;
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, webpManipulator->get_head_urls().at(num).c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HeadApi::write_data); // pass the writefunction
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);
        res = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);
    webpManipulator->UpdateFrames(stream, num);
}