#ifndef MasterRunMgrSvc_hh
#define MasterRunMgrSvc_hh

/*  
 * MasterRunMgrSvc is a Service to manage the MasterRunMgr.
 * 
 * It is like the main() function to create RunMgr.
 **/

#include <vector>
#include "SniperKernel/SvcBase.h"
#include "DetSimMT/tbbMasterRunManager.hh"

class IDetSimFactory;

class MasterRunMgrSvc: public SvcBase {
    public:
        MasterRunMgrSvc(const std::string& name);
        ~MasterRunMgrSvc();

        bool initialize();
        bool finalize();

    private:
        tbbMasterRunManager* m_runmgr;

        std::string m_runmac;
        int m_ntasks;
        int m_event_modulo;
        int m_seed_once;

        std::vector<std::string> m_recorder_names;

        IDetSimFactory* det_factory;
        std::string m_factory_name;

};

#endif
