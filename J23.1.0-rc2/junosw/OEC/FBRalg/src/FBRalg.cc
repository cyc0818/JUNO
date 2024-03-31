#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/SniperPtr.h"
#include "Event/WpCalibHeader.h"
#include "Event/OecHeader.h"
#include "EvtNavigator/EvtNavHelper.h"  

#include "TVector3.h"
#include "TSpectrum2.h"
#include "TH2F.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <math.h>
#include <numeric>
#include <vector>

#include "FBRalg.h"
DECLARE_ALGORITHM(FBRalg);

FBRalg::FBRalg(const std::string &name):AlgBase(name){
    declProp("threQ", threQ); 
    declProp("Rf", Rf);
}

FBRalg::~FBRalg(){
    
}

bool FBRalg::initialize(){
    LogInfo<<"initializing FBRalg..."<<std::endl;
#ifdef PERFORMANCE_CHECK 
    // for resources test
    ProcInfo_t mem_info0;
    gSystem->GetProcInfo(&mem_info0);
    mem_resident[0] = mem_info0.fMemResident / 1024./1024.;
#endif
    //get navbuffer
    SniperDataPtr<JM::NavBuffer> navbuf(getParent(), "/Event");
    if (navbuf.invalid()){
        LogError<<"can not get navbuffer!!!"<<std::endl;
        return false;
    }
    m_buf = navbuf.data();
    i_nEvt = 0;
    Qbf = -1.;
    wpmt_range = 20600.;
    
    SniperPtr<IPMTParamSvc> pmtsvc(getParent(), "PMTParamSvc");
    m_pmtsvc = pmtsvc.data();

    pmtid.reserve(MaxWpPMTnum);
    total_Q.reserve(MaxWpPMTnum);

#ifdef PERFORMANCE_CHECK    
    //timer service
    SniperPtr<IJunoTimerSvc> junotimersvc(getRoot(), "JunoTimerSvc");
    m_junotimersvc = junotimersvc.data();
    m_timerevent=m_junotimersvc->get("HECALGEVT/timerevt");

    // user data definitions
    SniperPtr<RootWriter> svc(getRoot(), "RootWriter");
    if (svc.invalid()) {
    LogError << "Can't Locate RootWriter. If you want to use it, please "
             << "enalbe it in your job option file." << std::endl;
    return false;
    }
    m_fbralg = svc->bookTree(*m_par, "USER_OUTPUT/fbralg", "simple output of qctr rec data");    
    m_fbralg->Branch("time_total",&time_total,"time_total/F");
    N_mem = 4;   
    m_fbralg->Branch("N_mem", &N_mem, "N_mem/I");   
    m_fbralg->Branch("mem_resident",mem_resident,"mem_resident[N_mem]/F"); 

    // for resources test
    ProcInfo_t mem_info1;
    gSystem->GetProcInfo(&mem_info1);
    mem_resident[1] = mem_info1.fMemResident / 1024./1024.;
#endif

    return true;
}

bool FBRalg::finalize(){
    LogInfo<<"finalizing FBRalg..."<<std::endl;

    return true;
}

bool FBRalg::execute(){
#ifdef PERFORMANCE_CHECK
    // for resources test
    ProcInfo_t mem_info2;
    gSystem->GetProcInfo(&mem_info2);
    mem_resident[2] = mem_info2.fMemResident / 1024./1024.;
    m_timerevent->start();
#endif

    i_nEvt++;
    LogDebug<<"The "<<i_nEvt<<"th event"<<std::endl;

    // --------------------------- Variables Initialization
    Qbf = -1.;
    pmtid.clear();
    total_Q.clear();
    
    // Get current event
    JM::EvtNavigator* tnav=m_buf->curEvt();
    if(tnav==NULL){
        LogError<<"nav is null"<<std::endl;
    }
    JM::OecHeader* tHeaderOEC = JM::getHeaderObject<JM::OecHeader>(tnav);  
    if(tHeaderOEC==NULL) {
        LogError<<"oecheader is null!"<<std::endl;
        return true;
    }
    JM::OecEvt* m_tEvent = dynamic_cast<JM::OecEvt*>(tHeaderOEC->event("JM::OecEvt"));
    if(m_tEvent==NULL) {
        LogError<<"oecevt is null!"<<std::endl;
        return true;
    }
    double energy = m_tEvent->getEnergy(); 
    JM::WpCalibHeader* tHeaderCalib = JM::getHeaderObject<JM::WpCalibHeader>(tnav);  

    if(tHeaderCalib!=NULL){
        JM::WpCalibEvt* m_tEvent = dynamic_cast<JM::WpCalibEvt*>(tHeaderCalib->event("JM::WpCalibEvt"));
        const std::list<JM::CalibPmtChannel*>& vec_CDL = m_tEvent->calibPMTCol();
        for(std::list<JM::CalibPmtChannel*>::const_iterator it = vec_CDL.begin(); it != vec_CDL.end(); ++it) {
            JM::CalibPmtChannel* hit_it = *it;
            double npe = hit_it->nPE();
            if(npe>threQ){
                pmtid.push_back(WpID2pmtID(hit_it->pmtId()));
                total_Q.push_back(npe);
            }
        }    
    }
    else{
        std::cout<<"There is no WP trigger."<<std::endl;
        m_tEvent->setQBF(Qbf);
#ifdef PERFORMANCE_CHECK    
    // for resources test
    ProcInfo_t mem_info3;
    gSystem->GetProcInfo(&mem_info3);
    mem_resident[3] = mem_info3.fMemResident / 1024./1024.;
    m_timerevent->stop();
    time_total=m_timerevent->elapsed()/1000.;
    LogInfo << "Time consumes: " << time_total << " s"<<endl;  
    m_fbralg->Fill();
#endif 
        return true;
    }
    if(pmtid.empty()){
        LogDebug<<"No default pulse."<<std::endl;
        m_tEvent->setQBF(Qbf);
#ifdef PERFORMANCE_CHECK    
    // for resources test
    ProcInfo_t mem_info3;
    gSystem->GetProcInfo(&mem_info3);
    mem_resident[3] = mem_info3.fMemResident / 1024./1024.;
    m_timerevent->stop();
    time_total=m_timerevent->elapsed()/1000.;
    LogInfo << "Time consumes: " << time_total << " s"<<endl;  
    m_fbralg->Fill();
#endif 
        return true;
    }

    // 1. Find the PMT with Max NPE.
    double max_npe = *std::max_element(total_Q.begin(), total_Q.end());
    int max_pmtid = pmtid.at(std::max_element(total_Q.begin(), total_Q.end())-total_Q.begin());

    // 2. Rotate the 3-D spherical coordinates in terms of the position of the PMT with Max NPE.
    // (Then every PMT will own a new position, which will be used to get criterion.)
    // (LogDebug << "the max npe pmt position after coordinates transform")
    double ref_x = m_pmtsvc->getPMTX(max_pmtid);;
    double ref_y = m_pmtsvc->getPMTY(max_pmtid);;
    double ref_z = m_pmtsvc->getPMTZ(max_pmtid);;
    double ref_theta = m_pmtsvc->getPMTTheta(max_pmtid);
    double ref_phi = m_pmtsvc->getPMTPhi(max_pmtid);
    check_xyz = Rot3D(ref_x, ref_y, ref_z, ref_theta, ref_phi);
    check_tp = ConAxis(get<0>(check_xyz), get<1>(check_xyz), get<2>(check_xyz));
    LogDebug << "After rotation, the cluster point: " << get<0>(check_xyz) << "[mm] " << get<1>(check_xyz) << "[mm] " << get<2>(check_xyz) << "[mm] " << std::endl;
    LogDebug << get<0>(check_tp) << " " << get<1>(check_tp) << " " << std::endl;
    
    // 3. Count the number of npe in lower area and upper area, and then, obtain the ratio as criteria.
    double Qf = 0.;
    double Qb = 0.;
    for(int i = 0; i < pmtid.size(); i++){
        double x_ = m_pmtsvc->getPMTX(pmtid.at(i));
        double y_ = m_pmtsvc->getPMTY(pmtid.at(i));
        double z_ = m_pmtsvc->getPMTZ(pmtid.at(i));
        vec = Rot3D(x_, y_, z_, ref_theta, ref_phi);
        double x_c = get<0>(vec);
        double y_c = get<1>(vec);
        double z_c = get<2>(vec);
        double rr = std::sqrt(x_c*x_c+y_c*y_c);
        if(z_c > std::sqrt(wpmt_range*wpmt_range-Rf*Rf)){
            Qf += total_Q.at(i);
        }
        else{
            Qb += total_Q.at(i);
        }
    }
    Qbf = Qb / Qf;
    LogDebug << "Qbf: " << Qbf << std::endl;
    LogDebug << "max_pmtid: " << std::dec << max_pmtid << std::endl;
    LogDebug << "max_npe: " << max_npe << std::endl;
    m_tEvent->setQBF(Qbf);

#ifdef PERFORMANCE_CHECK    
    // for resources test
    ProcInfo_t mem_info3;
    gSystem->GetProcInfo(&mem_info3);
    mem_resident[3] = mem_info3.fMemResident / 1024./1024.;
    m_timerevent->stop();
    time_total=m_timerevent->elapsed()/1000.;
    LogInfo << "Time consumes: " << time_total << " s"<<endl;  
    m_fbralg->Fill();
#endif 

    return true;
}
