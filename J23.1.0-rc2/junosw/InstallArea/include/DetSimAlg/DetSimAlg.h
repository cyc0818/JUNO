#ifndef DetSimAlg_h
#define DetSimAlg_h

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/SvcBase.h"
#include "DetSimAlg/IDetSimFactory.h"
#include "G4Svc/G4SvcRunManager.h"
#include <string>
#include <vector>

class DetSimAlg: public AlgBase
{
    public:
        DetSimAlg(const std::string& name);
        ~DetSimAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        bool SetG4Init();
        bool SetG4RunMac();
        bool SetG4RunCmds();
        bool StartG4Vis();


    private:
        std::string m_factory_name;
        std::string m_run_mac;
        std::vector<std::string> m_run_cmds;
        std::string m_vis_mac;
        std::string m_g4svc_name;

        IDetSimFactory* det_factory;
        G4SvcRunManager* run_manager;

        int i_event;
        bool is_mt; // single thread or multi-threading

};

#endif
