//
//  Author: Zhengyun You  2013-11-20
//

#include "QCtrRecAlg/QCtrRecAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/DataBufSvc.h"
#include "Identifier/Identifier.h"
#include "Event/RecHeader.h"
#include "Event/CalibPMTHeader.h"
#include "Event/CalibHeader.h"

#include "TTree.h"
#include "TVector3.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TF1.h"

DECLARE_ALGORITHM(QCtrRecAlg);

QCtrRecAlg::QCtrRecAlg(const std::string& name)
    : AlgBase(name)
{
    m_iEvt = -1;
    m_usePerfectVtx = false;

    // Using declare property
    declProp("UsePerfectVtx", m_usePerfectVtx);
    getProp("UsePerfectVtx")->declareRead(
            boost::bind(&QCtrRecAlg::f_bool_handler, this, _1)
            );
    getProp("UsePerfectVtx")->declareUpdate(
            boost::bind(&QCtrRecAlg::f_bool_updater, this, _1)
            );
}

QCtrRecAlg::~QCtrRecAlg()
{
    clear();
}

bool QCtrRecAlg::initialize()
{
    // Initialize geometry service
    initGeom();
    // Initialize event input interface
    initInput();
    // Initialize output interface
    initOutput();
    // Initialize parameters
    initParameters();

    LogInfo << name() << " initialized successfully" << std::endl;

    return true;
}

bool QCtrRecAlg::execute()
{
    ++m_iEvt;

    // Valid log level: LogDebug, LogInfo, LogWarn, LogError, LogFatal
    LogDebug << "Processing event " << m_iEvt << std::endl;
    
    // Read event input
    readInput(); 

    // Reconstruct vertex from Pmt hits
    reconstruct();

    // Calucations and fill output 
    fillOutput();
    
    // Clear event based container
    clear();
    
    return true;
}   
    
bool QCtrRecAlg::finalize()
{   
    LogInfo << name() << " finalized successfully" << std::endl;
    
    return true;
}   

bool QCtrRecAlg::initGeom()
{
    // Initialize RecGeom interface
    m_recGeomSvc = dynamic_cast<RecGeomSvc*>(service("RecGeomSvc"));
    if ( m_recGeomSvc == 0 ) {
        LogError << "Failed to get RecGeomSvc instance!" << std::endl;
        return false;
    }
    m_cdGeom = m_recGeomSvc->getCdGeom(); 

    return true;
}

bool QCtrRecAlg::initInput()
{
    // Initialize buffer service
    m_bufsvc= dynamic_cast<DataBufSvc*>(service("DataBufSvc"));
    if (0 == m_bufsvc) {
        LogError  << name()<< "Failed to get DataBufSvc instance!" << std::endl;
        return false;
    }

    return true;
}

bool QCtrRecAlg::initOutput()
{
    LogInfo << name() << " initialized successfully" << std::endl;

    return true;
}

bool QCtrRecAlg::initParameters()
{
    // gamma correction function
    m_efun = new TF1("efun", "[0]/x-1100.35*(1+0.061766*log(x)-0.006967*log(x)*log(x)-0.00156527*log(x)*log(x)*log(x))");
    // eplus correction fuction
    //m_efun = new TF1("efun", "[0]-1273.57*(1+0.78184*log(x)+0.320446*log(x)*log(x)+0.37355*log(x)*log(x)*log(x))");

    //Non-linearity Correction for positron, copied from RecTimeLikeAlg
    double  non_li_parameter[4] = {0.122495, 1.04074, 1.78087, 0.00189743};
    m_non_li_positron = new TF1("pos","(([1]+[3]*(x-1.022))/(1+[0]*exp(-[2]*(x-1.022)))*(x-1.022)+0.935802158)",0.2,12);
    m_non_li_positron->SetParameters(non_li_parameter);

    return true;
}

void QCtrRecAlg::clear()
{
}

bool QCtrRecAlg::readInput()
{
    //read CalibHeader
    m_chbuf = m_bufsvc->getDataBuf<JM::CalibHeader>("/Event/Calib/CalibHeader");
    if (0 == m_chbuf) {
        LogError << name() << "Failed to get DataBuffer \"/Event/Calib/CalibHeader\"" << std::endl;
        return false;
    }

    m_chcol = m_chbuf->curEvt();
    const std::list<JM::CalibPMTHeader*>& cphlist = m_chcol->calibPMTCol();
    LogDebug << "calibPMTCol size " << cphlist.size() << std::endl;

    int printFirstNPmt = 10;
    int printCount = 0;
    std::list<JM::CalibPMTHeader*>::const_iterator cphIter = cphlist.begin();
    while (cphIter != cphlist.end()) {
        JM::CalibPMTHeader *cph = *cphIter;
        //unsigned int pmtId = cph->pmtId();
        //unsigned int nPE   = cph->nPE();
        //double firstHitTime = cph->firstHitTime();

        if (printCount < printFirstNPmt) {
            LogDebug << "Insert CalibPMTHeader id " << Identifier(cph->pmtId())
                     << " nPE " << cph->nPE() << " firstHitTIme " << cph->firstHitTime()
                     << std::endl;
        }
        ++printCount;
        cphIter++;
    }
    LogDebug << "Insert CalibPMTHeader... (skip printing), Insert " << cphlist.size() << " in total " << std::endl;
    LogDebug << "Done to read CalibHit" << std::endl;

    return true;
}

bool QCtrRecAlg::reconstruct()
{
    reconstructVtx();
    reconstructEnergy();
    printEvent();

    return true;
}

bool QCtrRecAlg::reconstructVtx()
{
    TVector3 qCtr(0, 0, 0);
    double nPESum = 0;

    const std::list<JM::CalibPMTHeader*>& cphlist = m_chcol->calibPMTCol();
    std::list<JM::CalibPMTHeader*>::const_iterator cphIter = cphlist.begin();
    while (cphIter != cphlist.end()) {
        JM::CalibPMTHeader *cph = *cphIter;
        Identifier id = Identifier(cph->pmtId());
        double nPE = cph->nPE();

        PmtGeom *pmt = m_cdGeom->getPmt(id);
        if ( !pmt ) {
            LogError << name() << " Wrong Pmt Id " << id << std::endl;
            return false;
        }
        TVector3 center = pmt->getCenter();
        qCtr += center * nPE;
        nPESum += nPE;

        cphIter++;
    }

    if (nPESum != 0) {
        qCtr *= (1.0/nPESum);
    }
    else {
        LogError << name() << " nPESum = 0" << std::endl;
        return false;
    }

    double ratioVtxCorrection = 1.1444;
    if (m_usePerfectVtx) ratioVtxCorrection = 1.5;
    m_recVtx = ratioVtxCorrection*qCtr;

    return true;
}

bool QCtrRecAlg::reconstructEnergy()
{
    m_recPESum = 0;
    std::map<Identifier, int> pmtNHitMap;

    const std::list<JM::CalibPMTHeader*>& cphlist = m_chcol->calibPMTCol();
    std::list<JM::CalibPMTHeader*>::const_iterator cphIter = cphlist.begin();
    while (cphIter != cphlist.end()) {
        JM::CalibPMTHeader *cph = *cphIter;
        Identifier id = Identifier(cph->pmtId());
        double nPE = cph->nPE();
        m_recPESum += nPE;

        std::map<Identifier, int>::iterator idIter = pmtNHitMap.find(id);
        if (idIter == pmtNHitMap.end()) {
            pmtNHitMap.insert( std::pair<Identifier, int>(id, 1) );
        }
        else {
            idIter->second = idIter->second + 1;
        }

        cphIter++;
    }
    m_recNPmt = pmtNHitMap.size();

    correctEnergy();

    return true;
}

bool QCtrRecAlg::correctEnergy()
{
    // Use Calib function to correct energy
    double a = 10000.0;
    double r = 0.001*m_recVtx.Mag();
    double r3 = r*r*r;
    double k = m_recPESum / (a+r3);
    m_efun->SetParameter(0, k*a);
    m_recEnergy = m_efun->GetX(0, 0.1, 11.0);
    //double y = m_efun->Eval(1.1);
    //std::cout << "y= " << y << std::endl;

    // deposit energy with Non-linear correction for positron
    m_eprec = m_non_li_positron->GetX(m_recEnergy);

    return true;
}

void QCtrRecAlg::printEvent()
{
    LogDebug << "RecVtx at r=" << m_recVtx.Mag()
        << "\tNPmt=" << m_recNPmt
        << "\tPESum=" << m_recPESum
        << "\tEnergy=" << m_recEnergy
        << "\teprec=" << m_eprec
        << std::endl;
}

bool QCtrRecAlg::fillOutput()
{
    // Fill Reconstruction data
    m_rhbuf = m_bufsvc->getRWDataBuf<JM::RecHeader>("/Event/Rec/RecHeader");
    if (0 == m_rhbuf) { 
        LogError << name() << "Failed to get DataBuffer \"/Event/Rec/RecHeader\"" << std::endl;
        return false;
    }

    // Fill RecEvent
    m_rh = new JM::RecHeader();
    m_rh->setX( m_recVtx.x() );
    m_rh->setY( m_recVtx.y() );
    m_rh->setZ( m_recVtx.z() );
    m_rh->setEnergy( m_recEnergy );
    m_rh->setEprec( m_eprec );
    m_rh->setPESum( int(m_recPESum) );
    m_rh->setPx( m_recEnergy * m_recVtx.x() / m_recVtx.Mag() ); // needs check
    m_rh->setPy( m_recEnergy * m_recVtx.y() / m_recVtx.Mag() );
    m_rh->setPz( m_recEnergy * m_recVtx.z() / m_recVtx.Mag() );

    m_rhbuf->push_back( m_rh );

    LogDebug  << "Done to write RecHeader" << std::endl;

    return true;
}

void QCtrRecAlg::f_bool_handler(MyProperty* p)
{
    LogInfo << name()
            << " call "
            << p->key()
            << std::endl;
}

void QCtrRecAlg::f_bool_updater(MyProperty* p)
{
    LogInfo << name()
            << " set "
            << p->key()
            << std::endl;
}
