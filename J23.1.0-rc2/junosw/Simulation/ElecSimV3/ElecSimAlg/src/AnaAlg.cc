#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/Incident.h"

#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Event/SimHeader.h"
#include "Event/CdLpmtElecTruthHeader.h"
#include "Event/CdWaveformHeader.h"


class AnaAlg : public AlgBase {
public:
    AnaAlg(const std::string& name)
        : AlgBase(name) {


    }

    bool initialize() {
        //Event navigator
        SniperDataPtr<JM::NavBuffer>  navBuf(*getRoot(), "/Event");
        if ( navBuf.invalid() ) {
            LogError << "cannot get the NavBuffer @ /Event" << std::endl;
            return false;
        }
        m_buf = navBuf.data();

        return true;
    }

    bool execute() {
        auto nav = m_buf->curEvt();

        // try to access data model.
        auto elec_truth_header = JM::getHeaderObject<JM::CdLpmtElecTruthHeader>(nav);
        auto elec_header = JM::getHeaderObject<JM::CdWaveformHeader>(nav);

        auto sim_hdrs = nav->getHeaders("/Event/Sim");

        LogInfo << "elec_truth_header: " << elec_truth_header << std::endl;
        LogInfo << "elec_header: " << elec_header << std::endl;
        for (size_t i = 0; i < sim_hdrs.size(); ++i) {
            LogInfo << "sim header [" << i << "]: " << sim_hdrs[i] << std::endl;
        }


        return true;
    }

    bool finalize() {

        return true;
    }


private:
    JM::NavBuffer* m_buf;
};

DECLARE_ALGORITHM(AnaAlg);
