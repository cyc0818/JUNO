#ifndef RecDummyWpTool_h
#define RecDummyWpTool_h
#include "RecTools/IRecMuonTool.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "Event/WpRecHeader.h"

class RecDummyWpTool : public IRecMuonTool, public ToolBase{
    public:
        RecDummyWpTool (const std::string& name);
        virtual ~RecDummyWpTool (){};

        bool reconstruct(RecTrks*) ;
        bool configure(const Params*,const PmtTable*); 

    private:
        const PmtTable* m_ptab; 

};
#endif

DECLARE_TOOL(RecDummyWpTool); 


RecDummyWpTool::RecDummyWpTool(const std::string& name)
    :ToolBase(name){
    }

bool
RecDummyWpTool::reconstruct(RecTrks* trks){
    TVector3 start(9, 9, 9);
    TVector3 end(99, 99, 99);
    trks->addTrk(start, end, 9, 99, 1024, 1,0);
    /*CLHEP::HepLorentzVector start(9, 9, 9, 9); 
    CLHEP::HepLorentzVector end(99, 99, 99, 99); 
    JM::WPRecEvent* evt = new JM::WPRecEvent();
    JM::RecTrack* mtrk = new JM::RecTrack(start, end); 
    mtrk->setPESum(1024); 
    mtrk->setQuality(1); 
    evt->addTrack(mtrk);
    rh->setWPEvent(evt);
    */
    LogDebug <<  "Dummy test to  reconstruct the event!"
        << std::endl; 
    return true; 
}

bool
RecDummyWpTool::configure(const Params* pars, const PmtTable* ptab){
    LogDebug  << "configure the Dummy reconstruct tool!"
        << std::endl; 
    LogDebug  << "Retrieve the key/value [Pmt20inchTimeReso: " 
         << pars->get("Pmt20inchTimeReso") << "] from Params" << std::endl; 
    m_ptab = ptab; 
    return true; 
}

