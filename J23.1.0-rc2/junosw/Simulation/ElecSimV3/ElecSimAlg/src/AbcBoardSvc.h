#ifndef AbcBoardSvc_h
#define AbcBoardSvc_h

#include <map>
#include <vector>

#include "EvtNavigator/EvtNavigator.h"
#include "Event/CdSpmtElecHeader.h"
#include "Event/CdSpmtElecTruthHeader.h"

#include "Pulse.h"
#include "AbcBoard.h"

#include "SpmtElecConfigSvc/SpmtElecConfigSvc.h"
#include "TTree.h"

/*
 * This is a service to deal with the conversion of pulses into SpmtElec workds
 *
 */

#include "SniperKernel/SvcBase.h"

class AbcBoardSvc: public SvcBase {
 public:
    AbcBoardSvc(const std::string& name);
    ~AbcBoardSvc();

    bool initialize();
    bool finalize();

    void pushPulses(int pmt, std::vector<Pulse*> pulses);
    bool addElecDataModel(JM::EvtNavigator* evtnav, bool storeTruth=true);
    void clearVectors();
    
    friend class AbcBoard; 

  private:

    SpmtElecConfigSvc* m_SpmtConfigSvc;
    
    std::vector<AbcBoard> boards;
    
    TTree* m_spmt_tree;
    int m_eventID;
    int m_SPMT_Id;
    double m_Charge;
    double m_Time; 
    
    int PC2ADC_THRES;      
    double PC2ADC_HG;     
    double PC2ADC_LG;     
    double Const_HG;      
    double Const_LG;
    double Q_Spe; 
    unsigned int channel_index;
    unsigned int channel_mask;  
    unsigned int abc_index;  
    unsigned int abc_mask;  
};

#endif
