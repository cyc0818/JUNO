//
//  Author: Zhengyun You  2013-12-20
//

#include "QCtrRecAlg/QCtrEvalAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/DataBufSvc.h"
#include "RootWriter/RootWriter.h"
#include "Identifier/Identifier.h"
#include "Event/RecHeader.h"
#include "Event/CalibPMTHeader.h"
#include "Event/CalibHeader.h"
#include "Event/SimHeader.h"
#include "Event/SimPMTHeader.h"
#include "Event/SimPMTHit.h"

#include "TTree.h"
#include "TVector3.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

DECLARE_ALGORITHM(QCtrEvalAlg);

QCtrEvalAlg::QCtrEvalAlg(const std::string& name)
    : AlgBase(name)
{
    m_iEvt = -1;
}

QCtrEvalAlg::~QCtrEvalAlg()
{
}

bool QCtrEvalAlg::initialize()
{
    // Initialize event input interface
    initInput();

    // Initialize output interface
    initOutput();

    LogInfo << name() << " initialized successfully" << std::endl;

    return true;
}

bool QCtrEvalAlg::initInput()
{
    return true;
}

bool QCtrEvalAlg::initOutput()
{
    // Initalized output service, root tree, RecEvent interface
    RootWriter* rw = dynamic_cast<RootWriter*>(service("RootWriter"));
    if ( 0 == rw ) {
        LogError << "Failed to get RootWriter instance!" << std::endl;
        return false;
    }

    m_hdr = new TH1F("hdr", "rec_r-gen_r", 100, -1000, 1000);
    m_hdx = new TH1F("hdx", "rec_x-gen_x", 100, -1000, 1000);
    m_hdy = new TH1F("hdy", "rec_y-gen_y", 100, -1000, 1000);
    m_hdz = new TH1F("hdz", "rec_z-gen_z", 100, -1000, 1000);
    m_hde = new TH1F("hde", "rec_e-edep",  100, -0.2,  0.2);
    m_hdeprec = new TH1F("hdeprec", "eprec-edep",  100, -0.2,  0.2); 
    m_hrratio = new TH1F("hrratio", "rec_r/gen_r", 200, 0.8, 1.2);
    m_hnpevsr = new TH2F("hnpevsr", "npe:r", 160, 0, 16000, 100, 0, 20000);
    m_pnpevsr3 = new TProfile("pnpevsr3", "npe:r^3", 100, 0, 3500, 0, 20000);
    m_heratio = new TH1F("heratio", "rec_e/gen_e", 200, 0.8, 1.2);
    m_heprecratio = new TH1F("heprecratio", "eprec/gen_e", 200, 0.8, 1.2);

    rw->attach("FILE1", m_hdr);
    rw->attach("FILE1", m_hdx);
    rw->attach("FILE1", m_hdy);
    rw->attach("FILE1", m_hdz);
    rw->attach("FILE1", m_hde);
    rw->attach("FILE1", m_hdeprec);
    rw->attach("FILE1", m_hrratio);
    rw->attach("FILE1", m_hnpevsr);
    rw->attach("FILE1", m_pnpevsr3);
    rw->attach("FILE1", m_heratio);
    rw->attach("FILE1", m_heprecratio);

    LogInfo << name() << " initialized successfully" << std::endl;

    return true;
}

bool QCtrEvalAlg::execute()
{   
    ++m_iEvt;

    // Valid log level: LogDebug, LogInfo, LogWarn, LogError, LogFatal
    LogInfo << "Processing event " << m_iEvt << std::endl;

    // Initialize Data Buffer
    m_bufsvc= dynamic_cast<DataBufSvc*>(service("DataBufSvc"));
    if (m_bufsvc == NULL) {
        LogError  << "Failed to get DataBufSvc instance!" << std::endl;
    }

    //read SimHeader data
    m_shbuf = (DataBuffer<JM::SimHeader>*)m_bufsvc->getDataBuf<JM::SimHeader>("/Event/Sim/SimHeader");
    if (0 == m_shbuf) {
        LogError << name() << "Failed to get DataBuffer \"/Event/Sim/SimHeader\"" << std::endl;
        return false;
    }
    const JM::SimHeader *sh = m_shbuf->curEvt();
    if (0 == sh) {
        LogError << name() << "Failed to get SimHeader" << std::endl;
    }
    LogDebug << "SimHeader PMT Hits " << sh->simPMT().size() << std::endl; 

    const JM::SimParticleHistory *history = sh->particleHistory();
    const std::vector<JM::SimTrack*>& stc = history->primaryTracks();
    LogDebug << "sh primary =" << stc.size() << std::endl;
    TVector3 simVtx(0,0,0);
    double simEdep = 0.0;
    double simEnergy = 0.0;
    std::vector<JM::SimTrack*>::const_iterator stit = stc.begin();
    while (stit!=stc.end()) {
        JM::SimTrack* st = *stit;
        // LogDebug << "sh gen pos (" << st->genX() << ", " << st->genY() << ", " << st->genZ() << ")" << std::endl;
        simVtx += TVector3(st->genX(), st->genY(), st->genZ());
        simEdep += st->edepEnergy();
        simEnergy += sqrt(st->mass()*st->mass()+st->px()*st->px()+st->py()*st->py()+st->pz()*st->pz());
        stit++;
    }
    simVtx *= (1.0/stc.size());
    LogDebug << "SimHeader vtx pos (" << simVtx.x() << ", " << simVtx.y() << ", " << simVtx.z() << ")" << std::endl;
    LogDebug << "SimHeader simEdep " << simEdep << " energy " << simEnergy << std::endl;

    //read RecHeader data
    m_rhbuf = (DataBuffer<JM::RecHeader>*)m_bufsvc->getDataBuf<JM::RecHeader>("/Event/Rec/RecHeader");
    if (0 == m_shbuf) {
        LogError << name() << "Failed to get DataBuffer \"/Event/Rec/RecHeader\"" << std::endl;
        return false;
    }
    const JM::RecHeader *rh = m_rhbuf->curEvt();
    if (0 == rh) {
        LogError << name() << "Failed to get RecHeader" << std::endl;
    }
    TVector3 recVtx(rh->x(), rh->y(), rh->z());
    LogDebug << "RecHeader vtx pos (" << rh->x() << ", " << rh->y() << ", " << rh->z() << ")" << std::endl;
    LogDebug << "RecHeader energy  " << rh->energy() << " eprec " << rh->eprec() << " nPE " << rh->peSum() << std::endl;
    
    double recEnergy = rh->energy();
    double eprec = rh->eprec();

    m_hdr->Fill( recVtx.Mag() - simVtx.Mag() );
    m_hdx->Fill( recVtx.x() - simVtx.x() );
    m_hdy->Fill( recVtx.y() - simVtx.y() );
    m_hdz->Fill( recVtx.z() - simVtx.z() );
    m_hde->Fill( recEnergy - simEdep );
    m_hdeprec->Fill( eprec - simEdep );
    m_hrratio->Fill( recVtx.Mag()/simVtx.Mag()); 
    m_hnpevsr->Fill( simVtx.Mag(), rh->peSum() );    
    m_pnpevsr3->Fill( pow(0.001,3)*simVtx.Mag2()*simVtx.Mag(), rh->peSum() );
    m_heratio->Fill( recEnergy/simEdep );
    m_heprecratio->Fill( eprec/simEdep );

    return true;
}

bool QCtrEvalAlg::finalize()
{   
    //m_hdr->Fit("gaus", "Q");
    //m_hdx->Fit("gaus", "Q");
    //m_hdy->Fit("gaus", "Q");
    //m_hdz->Fit("gaus", "Q");
    //m_hde->Fit("gaus", "Q");
    //m_hrratio->Fit("gaus", "Q");
    //m_hrratio->Fit("gaus", "Q");

    LogInfo << name() << " finalized successfully" << std::endl;

    return true;
}

