#ifndef ONLINE_CONFIG_MANAGER
#define ONLINE_CONFIG_MANAGER

#include <string>

class OnlineConfigManager{
    public:
        static std::string resolveConfigFilePath(const std::string& path);
};

#endif