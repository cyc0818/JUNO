#ifndef OECConfigSvc_h
#define OECConfigSvc_h
#include "OECData/OECTag.h"
#include "OECTagSvc/OECTagSvc.h"
#include <vector>

class OECConfigSvc: public SvcBase{
    public:
        OECConfigSvc(const std::string&);
        ~OECConfigSvc();

        bool initialize();
        bool finalize();

        const std::vector<OECTag* >& getOECTagVec() const;

    private:
        bool resolveTag();

        OECTagSvc* m_tagsvc;
        std::vector<std::string> m_tagNames;

        std::string s_configFile;
        std::vector<OECTag*> m_OECTagVec;
};
#endif
