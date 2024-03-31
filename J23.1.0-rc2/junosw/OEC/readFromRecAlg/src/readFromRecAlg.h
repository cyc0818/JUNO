#ifndef READFROMRECALG_H
#define READFROMRECALG_H

#include "SniperKernel/AlgBase.h"
#include "EvtStore/EvtStoreSvc.h"  
#include "EvtDataTypes/CriteriaItemValue.h"
#include "EvtNavigator/NavBuffer.h"
#include "SniperKernel/AlgBase.h"

class readFromRecAlg: public AlgBase{

    public:
        readFromRecAlg(const std::string& name);
        ~readFromRecAlg();

        bool initialize(); 
        bool execute(); 
        bool finalize(); 

    private:
        JM::NavBuffer* m_buf;

        CriteriaItemValue* m_energy;
        CriteriaItemValue* m_detID;
        CriteriaItemValue* m_vtxR;
        CriteriaItemValue* m_dEdx;
        EvtStoreSvc *m_storeSvc;

        double TotaltrackL;
        double TotalPE;
        std::vector<int> MuID;                                                
        std::vector<float> MuInX;                                             
        std::vector<float> MuInY;                                             
        std::vector<float> MuInZ;                                             
        std::vector<float> MuOutX;                                            
        std::vector<float> MuOutY;                                            
        std::vector<float> MuOutZ;
};
#endif
