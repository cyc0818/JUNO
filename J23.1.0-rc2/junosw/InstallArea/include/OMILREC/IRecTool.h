#ifndef IRecTool_h
#define IRecTool_h

#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "EvtNavigator/NavBuffer.h"

#include "BufferMemMgr/IDataMemMgr.h"
#include "Geometry/PMTParamSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"
#include "PMTCalibSvc/PMTCalibSvc.h"
#include "SniperKernel/SniperDataPtr.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdSpmtCalibHeader.h"
#include "Geometry/RecGeomSvc.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include <fstream>
#include <vector>
#include <string>

#include "TVector3.h"



struct IRecParaTool {
    virtual bool init() = 0;
    virtual bool LoadPMTPara() = 0;

    virtual bool   GetIsDyn(const int pmtid) = 0;
    virtual TH1D*  GetSPEs(const int pmtid) = 0;
    virtual double GetTimeOffset(const int pmtid) = 0;
    virtual double GetLTTS(const int pmtid) = 0;
    virtual double GetLGain(const int pmtid) = 0;
    virtual double GetLQRes(const int pmtid) = 0;
    virtual double GetLPDE(const int pmtid) = 0;
    virtual double GetLDNR(const int pmtid) = 0;
    virtual double GetSPDE(const int pmtid) = 0;
    virtual double GetSTTS(const int pmtid) = 0;
    virtual double GetSDNR(const int pmtid) = 0;
    virtual double GetLDarkMu() = 0;
    virtual double GetSDarkMu() = 0;

};
 

struct IRecInputTool {
    virtual bool init() = 0;
    virtual bool LoadPMTInput(IRecParaTool* m_pmtpara_tool) = 0;

    virtual const std::list<JM::CalibPmtChannel*>& Get_chhlist() = 0;
    virtual const std::list<JM::CalibPmtChannel*>& Get_schhlist() = 0;
    virtual JM::EvtNavigator* Get_nav() = 0;

    virtual double Get_LHIT_Charge(const int pmtid) = 0;
    virtual double Get_LHIT_Time(const int pmtid) = 0;
    virtual double Get_LHIT_NPE(const int pmtid) = 0;
    virtual double Get_SHIT_Charge(const int pmtid) = 0;
    virtual double Get_SHIT_Time(const int pmtid) = 0;

    virtual Int_t  Get_nPMT() = 0;
    virtual Int_t  Get_PMTID(const int k) = 0;
    virtual double Get_hitTime_Toy(const int k) = 0;
 
    virtual int    Get_nDNphotons() = 0;
    virtual int    Get_DNpmtID(const int k) = 0;
    virtual float  Get_DNhitQ(const int k) = 0;
    virtual float  Get_DNhitTime(const int k) = 0;
    virtual double Get_Buff_PEQ(const int k) = 0; //d_a2

    virtual void   Set_LHIT_Charge(const int pmtid, const double charge) = 0;
    virtual void   Set_SHIT_Charge(const int pmtid, const double charge) = 0;
    virtual void   Set_LHIT_Time(const int pmtid, const double time) = 0;
    virtual void   Set_SHIT_Time(const int pmtid, const double time) = 0;

};

#endif
