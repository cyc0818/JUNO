#ifndef OECTAGSVC_H
#define OECTAGSVC_H

#include "SniperKernel/SvcBase.h"

#include <string>
#include <map>

class OECTagSvc: public SvcBase{
    public:
        OECTagSvc(const std::string &name);
        ~OECTagSvc();

        bool initialize();
        bool finalize();

        struct THETAG{
            //xy: x:format for prompt  y:format for delayed
            int storeFormat_p;
            int storeFormat_d;
            std::string ptag;
            std::string dtag;
            THETAG(){
                storeFormat_p = -1;
                storeFormat_d = -1;
                ptag="";
                dtag="";
            }
        };

        std::vector<std::string> getTagName();
        int getStoreFormat(int);
        int getStoreFormat(std::string, bool isDelayed=0);
        std::string getpTag(std::string);
        std::string getdTag(std::string);

    private:
        bool resolveTag();

        std::string s_configFile;
        std::map<std::string, THETAG> m_tags;
        std::map<int, std::vector<int>> m_format;
};
#endif
