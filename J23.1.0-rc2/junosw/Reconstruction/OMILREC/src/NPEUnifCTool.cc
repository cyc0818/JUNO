//need a review

#include "OMILREC/NPEUnifCTool.h"
#include "SniperKernel/ToolBase.h"

static const double rrange[2] = {0., 17400.}; // mm

NPEUnifCTool::NPEUnifCTool(const std::string& name): ToolBase(name) {
    NormNPEMap = NULL;
}

NPEUnifCTool::~NPEUnifCTool() {
}

bool NPEUnifCTool::configure() {
    
    SniperPtr<PMTCalibSvc> calSvc(getParent(), "PMTCalibSvc");
    if(calSvc.invalid()) {
        LogError << "Can't Locate  PMTCalibSvc."
                 << std::endl;
        return false;
    }
    if(NormNPEMap==NULL) NormNPEMap = calSvc->getNormNPEMap();
    
    LogDebug << calSvc->getNormNPEMap()->Interpolate(0.1, 0.1) << std::endl; 
    LogInfo << "Hello! I am in the NPEUnifCTool ! "<< std::endl;
    return true;
} 



void NPEUnifCTool::rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo) {
    double evtx = recinfo.vtx.X();
    double evty = recinfo.vtx.Y();
    double evtz = recinfo.vtx.Z();
    double r = sqrt(evtx*evtx + evty*evty + evtz*evtz);
    double theta = 0.;
    if(r>1.e-16) theta = TMath::ACos(evtz/r);

    if(r>rrange[1]-1.e-8) r = rrange[1]-1.e-8; 

    double RVar = TMath::Power(r, 3.)*180./TMath::Power(rrange[1], 3.);
    double corrF = NormNPEMap->Interpolate(RVar, theta);
    recinfo.energy =  recinfo.energy/corrF;
    LogDebug<< "corrE:" << recinfo.energy  << std::endl;
}


DECLARE_TOOL(NPEUnifCTool);
