#ifndef FILTER_MEM_MGR_H
#define FILTER_MEM_MGR_H

#include "SniperKernel/SvcBase.h"
#include <list>

class IIncidentHandler;

class FilterMemMgr : public SvcBase
{
    public :

        FilterMemMgr(const std::string& name);

        virtual ~FilterMemMgr();

        bool initialize();
        bool finalize();
        

    private :

        std::vector<double>          m_bufBounds;
        std::list<IIncidentHandler*> m_icdts;

        // options
        // - whether stop task self (0x01) or root task (0x00, default)
        int m_stoptask_option;
};

#endif