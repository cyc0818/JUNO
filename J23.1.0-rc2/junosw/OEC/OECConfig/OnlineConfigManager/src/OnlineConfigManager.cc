#include "OnlineConfigManager/OnlineConfigManager.h"

std::string OnlineConfigManager::resolveConfigFilePath(const std::string& path){
    // if the path is relative path and there is "OEC_ONLINE_CONFIG" ENV 
    //then full path path is obtained with ENV
    if(path.size() > 0 && path[0] != '/' && getenv("OEC_ONLINE_CONFIG")){
        std::string prefix = getenv("OEC_ONLINE_CONFIG");
        return prefix + "/" + path;
    }
    else{
        return path;
    }
}