//
//  HTTPRequest
//

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <curl/curl.h>

namespace http{
    inline size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    inline CURL *curl;
    inline CURLcode res;

    inline void init(){
        /* In windows, this will init the winsock stuff */
        curl_global_init(CURL_GLOBAL_ALL);

        /* get a curl handle */
        curl = curl_easy_init();
    }

    inline std::string post(const char* url, const char* body){
        if(curl) {
            std::string readBuffer;

            /* First set the URL that is about to receive our POST. This URL can
               just as well be a https:// URL if that is what should receive the
               data. */
            curl_easy_setopt(curl, CURLOPT_URL, url);
            /* Now specify the POST data */
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

            struct curl_slist *hs=NULL;
            hs = curl_slist_append(hs, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl);

            return readBuffer;
        }
        return std::string{};
    }

    inline void cleanup(){
        if (curl){
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
    }

}

#endif
