#ifndef SNIPER_BUFFER_MEM_MGR_H
#define SNIPER_BUFFER_MEM_MGR_H

#include "BufferMemMgr/IDataMemMgr.h"
#include "SniperKernel/SvcBase.h"
#include <list>

class IIncidentHandler;

class BufferMemMgr : public SvcBase, public IDataMemMgr
{
    public :

        BufferMemMgr(const std::string& name);

        virtual ~BufferMemMgr();

        bool initialize();
        bool finalize();

        bool adopt(JM::EvtNavigator* nav, const std::string& path);
        bool adopt(std::shared_ptr<JM::EvtNavigator> nav, const std::string& path);

        bool reset(const std::string& path, int entry);

    private :

        std::vector<double>          m_bufBounds;
        std::list<IIncidentHandler*> m_icdts;

        // options
        // - whether stop task self (0x01) or root task (0x00, default)
        int m_stoptask_option;
};

#endif
