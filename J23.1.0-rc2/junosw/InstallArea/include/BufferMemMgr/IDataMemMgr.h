#ifndef SNIPER_I_DATA_MEM_MGR_H
#define SNIPER_I_DATA_MEM_MGR_H

#include <string>
#include <memory>

namespace JM {
    class EvtNavigator;
}

class IDataMemMgr
{
    public :

        virtual ~IDataMemMgr() {}

        virtual bool adopt(JM::EvtNavigator* nav, const std::string& path) = 0;
        virtual bool adopt(std::shared_ptr<JM::EvtNavigator> nav, const std::string& path) = 0;

        virtual bool reset(const std::string& path, int entry) = 0;
};

#endif
