#ifndef Rest_object_h
#define Rest_object_h

/*
 * Rest::object is used to simplify the usage of a cJSON object.
 * Instead of using the map container to access the values, the JSON object 
 * is mapping to a C++ object.
 *
 * The users need to define their own classes in order to load the cJSON object.
 * Please have a look at tests/main_object.cc.
 *
 * -- Tao Lin <lintao@ihep.ac.cn>
 *
 */

#include <stdio.h>
#include "cJSON/cJSON.h"
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
namespace Rest {
    struct stub {
        virtual bool from_json(cJSON* json) = 0;
        virtual bool to_json(cJSON* json) = 0;
        virtual std::string val_str() = 0;
    };

    template <class T>
    struct stubT: stub {
        stubT(const std::string& k, T& v)
            : key(k), val(v), type(cJSON_Object) {
        }
        
        bool from_json(cJSON* json) {
            cJSON* json_value = cJSON_GetObjectItem(json, key.c_str());
            if (!json_value) {
                std::cerr << "missing key " << key << " in json. " << std::endl;
                return false;
            }
            // check type
            val.from_json(json_value);
            char* val_json_data = cJSON_PrintUnformatted(json_value);
            val_ = val_json_data;
            free(val_json_data);
            return true;
        }

        bool to_json(cJSON* json) {
            // before add to json, we need to sync the val and val_.
            cJSON* json_child = cJSON_CreateObject();
            val.to_json(json_child);
            cJSON_AddItemToObject(json, key.c_str(), json_child);

            char* val_json_data = cJSON_PrintUnformatted(json_child);
            val_ = val_json_data;
            free(val_json_data);

            return true;
        }

        std::string val_str() {
            return val_;
        }
        
        std::string key;
        T& val; // assuming T is derived from Rest::object 
        std::string val_;
        const int type; // for cJSON
    };

    template <>
    struct stubT<std::string>: stub {
        stubT(const std::string& k, std::string& v)
            : key(k), val(v), type(cJSON_String) {
        }
        
        bool from_json(cJSON* json) {
            cJSON* json_value = cJSON_GetObjectItem(json, key.c_str());
            if (!json_value) {
                std::cerr << "missing key " << key << " in json. " << std::endl;
                return false;
            }
            // check type
            val = json_value->valuestring;
            val_ = json_value->valuestring;
            return true;
        }

        bool to_json(cJSON* json) {
            // before add to json, we need to sync the val and val_.
            val_ = val;
            cJSON_AddStringToObject(json, key.c_str(), val_.c_str());

            return true;
        }

        std::string val_str() {
            return val_;
        }
        
        std::string key;
        std::string& val;
        std::string val_;
        const int type; // for cJSON
    };

    template <>
    struct stubT<int>: stub {
    stubT(const std::string& k, int& v)
        : key(k), val(v), type(cJSON_Number) {
        }
        
        bool from_json(cJSON* json) {
            cJSON* json_value = cJSON_GetObjectItem(json, key.c_str());
            // check type
            val = json_value->valueint;
            std::stringstream ss; ss << val; ss >> val_;
            return true;
        }

        bool to_json(cJSON* json) {
            std::stringstream ss; ss << val; ss >> val_;
            cJSON_AddNumberToObject(json, key.c_str(), val);
            return true;
        }

        std::string val_str() {
            return val_;
        }
        
        std::string key;
        int& val;
        std::string val_;
        const int type; // for cJSON

    };

    template <>
    struct stubT<double>: stub {
    stubT(const std::string& k, double& v)
        : key(k), val(v), type(cJSON_Number) {
        }
        
        bool from_json(cJSON* json) {
            cJSON* json_value = cJSON_GetObjectItem(json, key.c_str());
            // check type
            val = json_value->valuedouble;
            std::stringstream ss; ss << val; ss >> val_;
            return true;
        }

        bool to_json(cJSON* json) {
            std::stringstream ss; ss << val; ss >> val_;
            cJSON_AddNumberToObject(json, key.c_str(), val);
            return true;
        }

        std::string val_str() {
            return val_;
        }
        
        std::string key;
        double& val;
        std::string val_;
        const int type; // for cJSON

    };

    template <>
    struct stubT< std::vector<std::string> >: stub {
    stubT(const std::string& k, std::vector<std::string>& v)
        : key(k), val(v), type(cJSON_Array) {
        }
        
        bool from_json(cJSON* json) {
            cJSON* json_value = cJSON_GetObjectItem(json, key.c_str());
            /* // check type */
            /* val = json_value->valuedouble; */
            /* std::stringstream ss; ss << val; ss >> val_; */

            val.clear();

            int size = cJSON_GetArraySize(json_value);
            std::cout << "SIZE: " << size << std::endl;
            for (int i = 0; i < size; ++i) {
                cJSON* elem = cJSON_GetArrayItem(json_value, i);
                val.push_back(elem->valuestring);
                std::cout << val.back() << std::endl;
            }
            return true;
        }

        bool to_json(cJSON* json) {
            /* std::stringstream ss; ss << val; ss >> val_; */
            /* cJSON_AddNumberToObject(json, key.c_str(), val); */
            return true;
        }

        std::string val_str() {
            return val_;
        }
        
        std::string key;
        std::vector<std::string>& val;
        std::string val_;
        const int type; // for cJSON

    };


    struct object {

        template<class T>
        void decl(const std::string& key, T& val) {
            m_stubs[key] = new stubT<T>(key, val);
        }

        bool from_json(cJSON* json);
        bool to_json(cJSON* json);

        virtual bool show();

        std::map<std::string, stub*> m_stubs;

    };

}

#endif
