//
//  Author: Zhengyun You  2013-11-20
//

#include "QCtrRecAlg/QCtrToySimAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/DataBufSvc.h"
#include "RootWriter/RootWriter.h"
#include "Identifier/Identifier.h"
#include "Event/SimHeader.h"
#include "Event/SimPMTHeader.h"
#include "Event/SimPMTHit.h"

#include "TTree.h"
#include "TVector3.h"
#include "TRandom3.h"
#include "TMath.h"

DECLARE_ALGORITHM(QCtrToySimAlg);

QCtrToySimAlg::QCtrToySimAlg(const std::string& name)
    : AlgBase(name)
{
    m_iEvt = -1;
}

QCtrToySimAlg::~QCtrToySimAlg()
{
}

bool QCtrToySimAlg::initialize()
{
    // Initialize geometry service
    initGeom();
    // Initialize data buffer service
    initBuf();
    // Initialize output interface
    initOutput();

    // Valid log level: LogDebug, LogInfo, LogWarn, LogError, LogFatal
    LogInfo << name() << " initialized successfully" << std::endl;

    return true;
}

bool QCtrToySimAlg::execute()
{
    ++m_iEvt;

    // Make toy data   
    makeToyData();
    // Calucations and fill output 
    fillOutput();
    printEvent();   
 
    return true;
}   
    
bool QCtrToySimAlg::finalize()
{   
    LogInfo << name() << " finalized successfully" << std::endl;
    
    return true;
}   

bool QCtrToySimAlg::initGeom()
{
    // Initialize RecGeom interface
    m_recGeomSvc = dynamic_cast<RecGeomSvc*>(service("RecGeomSvc"));
    if ( m_recGeomSvc == 0 ) {
        LogError << "Failed to get RecGeomSvc instance!" << std::endl;
        return false;
    }
    m_cdGeom = m_recGeomSvc->getCdGeom(); 

    // Test getPmt correctness
    //m_cdGeom->getPmt(m_iEvt%107, m_cdGeom->getAzimuthNum(m_iEvt%107)-1, 0)->print();

    // Test Identifier
    //Identifier *id_1 = new Identifier();
    //*id_1 = 3; LogInfo << "id_1 " <<  id_1->getString() << std::endl;
    //*id_1 = 4; LogInfo << "id_1 " <<  id_1->getString() << std::endl;

    return true;
}

bool QCtrToySimAlg::initBuf()
{
    // Initialize buffer service
    m_bufsvc= dynamic_cast<DataBufSvc*>(service("DataBufSvc"));
    if (0 == m_bufsvc) {
        LogError  << name()<< "Failed to get DataBufSvc instance!" << std::endl;
        return false;
    }

    return true;
}

bool QCtrToySimAlg::initOutput()
{
    // Initalized output service, root tree, RecEvent interface
    RootWriter* m_rw = dynamic_cast<RootWriter*>(service("RootWriter"));
    if ( 0 == m_rw ) {
	LogError << "Failed to get RootWriter instance!" << std::endl;
	return false;
    }

    m_treeToySim = new TTree("toySim", "QCtrToySimAlg Output");
    m_treeToySim->Branch("iEvt",  &m_iEvt,  "iEvt/I");
    m_treeToySim->Branch("genNPhoton",  &m_genNPhoton,  "genNPhoton/I");
    m_treeToySim->Branch("genx",        &m_genx,        "genx/D");
    m_treeToySim->Branch("geny",        &m_geny,        "geny/D");
    m_treeToySim->Branch("genz",        &m_genz,        "genz/D");
    m_treeToySim->Branch("genr",        &m_genr,        "genr/D");
    m_treeToySim->Branch("gentheta",    &m_gentheta,    "gentheta/D");
    m_treeToySim->Branch("genphi",      &m_genphi,      "genphi/D");
    m_treeToySim->Branch("simNHit",     &m_simNHit,     "simNHit/I");
    m_treeToySim->Branch("simAccpt",    &m_simAccpt,    "simAccpt/D");

    m_rw->attach("FILE1", m_treeToySim);

    LogInfo << name() << " initialized successfully" << std::endl;

    return true;
}

bool QCtrToySimAlg::makeToyData()
{
    // Randomly generate vertex in a sphere
    randomGenVtx(16.0e3);

    // Randomly generate n particles from vertex
    m_genNPhoton = (int)gRandom->Gaus(1000, 100);

    // Propogate every particle to a crossed Pmt to make hits or escape Cd 
    propogate();

    return true;
}

bool QCtrToySimAlg::randomGenVtx(double rMax, double rMin, double thetaMax, double thetaMin, double phiMax, double phiMin)
{
    double r     = gRandom->Rndm() * (rMax - rMin);
    double theta = gRandom->Rndm() * (thetaMax - thetaMin);
    double phi   = gRandom->Rndm() * (phiMax - phiMin);
    //LogDebug << "rMax " << rMax << " rMin " << rMin << " r " << r << " theta " << theta << " phi " << phi << std::endl;
    m_genVtx.SetMagThetaPhi(r, theta, phi);
    //std::cout << "GenVtx "; m_genVtx.Print();
    
    return true;
}

bool QCtrToySimAlg::propogate()
{
     // speed of light * (mm/ns) / n in Liquid Scintillator
    const double k_sl = 3.0e8 * (1e3/1e9) / 1.5;

    m_simNHit = 0;
    int nLines = m_genNPhoton;
    for (int iLine = 0; iLine < nLines; iLine++) {
        double z = 2.0*gRandom->Rndm()-1.0;
        double theta = gRandom->Rndm() * 2.0*TMath::Pi();
        double x = sqrt(1-z*z)*cos(theta);
        double y = sqrt(1-z*z)*sin(theta);
        TVector3 dir(x, y, z);
        //std::cout << "dir "; dir.Print();

        PmtGeom *pmt = m_cdGeom->findCrossedPmt(m_genVtx, dir);  
        //PmtGeom *pmt = m_cdGeom->findCrossedPmtFast(m_genVtx, dir);
        if (pmt) {
            //pmt->print();     
            double distance = (pmt->getCenter() - m_genVtx).Mag();
            double t = distance / k_sl;

            addSimHit(pmt->getId(), t);
            m_simNHit++;
        }
        else {
            //std::cout << "no Pmt crossed " << std::endl;
        }
    }
    m_simAccpt = double(m_simNHit)/m_genNPhoton;

    return true;
}

void QCtrToySimAlg::addSimHit(const Identifier& id, const double t)
{
    JM::SimPMTHit *sphit = new JM::SimPMTHit;
    sphit->setPmtId( id );
    sphit->setHitTime( t );
    //LogDebug << "sphit PmtId " << sphit->pmtId() << " HitTime " << sphit->hitTime() << std::endl;

    //LogDebug << "sphmap size " << m_sphmap.size() << std::endl;
    std::map<int, JM::SimPMTHeader*>::iterator iter = m_sphmap.find( id.getValue() );
    JM::SimPMTHeader* sph = 0;
    if ( iter == m_sphmap.end() ) {
        sph = new JM::SimPMTHeader();
        sph->setPmtId( id );
        sph->setNPE(0);
        sph->setFirstHitTime(9e99);
        m_sphmap.insert( std::pair<int, JM::SimPMTHeader*>(id.getValue(), sph) ); 
        //LogDebug << "No SimPMTHeader for " << id << " add a new one, nPE " << sph->nPE() << std::endl;
    }
    else {
        sph = iter->second;
        //LogDebug << "Found SimPMTHeader for " << id << " size  " << sph->nPE() << std::endl;
    }

    sph->addSimPMTHit( sphit );
    if ( sphit->hitTime() < sph->firstHitTime() ) {
        sph->setFirstHitTime( sphit->hitTime() );
    }
    //LogDebug << "SimPMTHeader for " << id << " nPE  " << sph->nPE() << std::endl; 
}

void QCtrToySimAlg::printEvent()
{
    LogDebug << "GenVtx at r=" << m_genVtx.Mag()
        << "\tNPhoton=" << getGenNPhoton()
        << "\tNHit=" << getSimNHit()
        << "\tAccpt=" << getSimAccpt()
        << std::endl;
}

bool QCtrToySimAlg::fillOutput()
{
    m_shbuf = m_bufsvc->getRWDataBuf<JM::SimHeader>("/Event/Sim/SimHeader");
    m_shbuf -> clear();
    if (0 == m_shbuf) {
        LogError << name() << "Failed to get DataBuffer \"/Event/Sim/SimHeader\"" << std::endl;
        return false;
    }

    // Create SimHeader
    m_simheader = new JM::SimHeader;
    m_simheader->setSimPMT(m_sphmap);

    m_genx = m_genVtx.x();
    m_geny = m_genVtx.y();
    m_genz = m_genVtx.z();
    m_genr = m_genVtx.Mag();
    m_gentheta = m_genVtx.Theta();
    m_genphi   = m_genVtx.Phi();

    // Particle History
    JM::SimParticleHistory* history = new JM::SimParticleHistory;
    JM::SimTrack* st = new JM::SimTrack;
    st->setGenX( m_genVtx.x() );
    st->setGenY( m_genVtx.y() );
    st->setGenZ( m_genVtx.z() );
    st->setGenT( 0.0 );
    st->setPdgid( 22 );
    st->setMass( 0.0 );
    st->setPx( 0.0 );
    st->setPy( 0.0 );
    st->setPz( 0.0 );
    st->setEdepEnergy( 1.0 );
    st->setEdepPosX( m_genVtx.x() );
    st->setEdepPosY( m_genVtx.y() );
    st->setEdepPosZ( m_genVtx.z() );
    history->addPrimarySimTrack(st);
    m_simheader->setParticleHistory(history);

    LogDebug << "SimHeader : " << m_simheader->simPMT().size() << " SimPMTHeader" << std::endl;
    m_shbuf->push_back(m_simheader);

    // Fill the trees
    m_treeToySim->Fill();

    m_sphmap.clear();

    //LogDebug  << "Done to fillOutput" << std::endl;

    return true;
}
