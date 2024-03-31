#ifndef Rest_cURL_h
#define Rest_cURL_h

#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <vector>

namespace Rest {
struct cURL {
    cURL();
    ~cURL();

    // request a URL:
    bool request(const std::string& uri);
    // request a URL (POST)
    bool request(const std::string& uri, const std::string& data);
    bool request(const std::string& uri,
                 const std::vector<std::string>& header_list,
                 const std::string& data);
    // get the result of request:
    std::string result();
    void reset_result_buffer();
private:
    static  size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
    CURL *curl;
    std::string m_result;

};

}

#endif
