#include "JVisLib/JVisGeom.h"
#include "Geometry/CdGeom.h"
#include "Geometry/WpGeom.h"

// ROOT 
#include "TGeoManager.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TString.h"

// C/C++
#include <iostream>

using namespace std;

JVisGeom::JVisGeom()
{

}

JVisGeom::~JVisGeom()
{

}

bool JVisGeom::initialize(TString geomFile)
{
    std::cout << "  init CdGeom" << std::endl;
    m_CdGeom = new CdGeom();
    m_CdGeom->setVerb(1);
    m_CdGeom->setGeomFileName(geomFile.Data());
    m_CdGeom->setGeomPathName("JunoGeom");
    m_CdGeom->init();

    std::cout << "  init WpGeom" << std::endl;
    m_WpGeom = new WpGeom();
    m_WpGeom->setVerb(1);
    m_WpGeom->setGeomFileName(geomFile.Data());
    m_WpGeom->setGeomPathName("JunoGeom");
    m_WpGeom->init();
    
    return true;
}

CdGeom* JVisGeom::getCdGeom()
{
    if (m_CdGeom == 0) {
        std::cerr << "JVisGeom::getCdGeom CdGeom not initialized" << std::endl;
    }
    
    return m_CdGeom;
}

WpGeom* JVisGeom::getWpGeom()
{
    if (m_WpGeom == 0) {
        std::cerr << "JVisGeom::getWpGeom WpGeom not initialized" << std::endl;
    }

    return m_WpGeom;
}


