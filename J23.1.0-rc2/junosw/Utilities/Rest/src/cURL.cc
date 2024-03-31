#include "Rest/cURL.h"

namespace Rest {

    cURL::cURL() {
        /* In windows, this will init the winsock stuff */ 
        curl_global_init(CURL_GLOBAL_ALL);
 
        /* get a curl handle */ 
        curl = curl_easy_init();
    }

    cURL::~cURL() {
        if (curl) {
            curl_easy_cleanup(curl);
            curl = NULL;
        }

        curl_global_cleanup();

    }

    bool cURL::request(const std::string& uri) {
        if (!curl) { return false; }

        reset_result_buffer();

        curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this); 

        /* Perform the request, res will get the return code */ 
        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }

        return true;
    }

    bool cURL::request(const std::string& uri, const std::string& data) {
        if (!curl) { return false; }

        reset_result_buffer();

        curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this); 

        /* Perform the request, res will get the return code */ 
        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }

        return true;
    }

    bool cURL::request(const std::string& uri, 
                       const std::vector<std::string>& header_list,
                       const std::string& data) {
        if (!curl) { return false; }

        reset_result_buffer();

        struct curl_slist *headers = NULL;
        for (std::vector<std::string>::const_iterator it = header_list.begin();
             it != header_list.end(); ++it) {
            // headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, (*it).c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this); 

        /* Perform the request, res will get the return code */ 
        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }

        curl_slist_free_all(headers);

        return true;
    }

    std::string cURL::result() {
        return m_result;
    }

    void cURL::reset_result_buffer() {
        m_result.clear();
    }

    size_t cURL::write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
        cURL* self = (cURL*)(userp);

        size_t realsize = size * nmemb;
        self->m_result.append((char*)buffer, realsize);

        return realsize;
    }
}
