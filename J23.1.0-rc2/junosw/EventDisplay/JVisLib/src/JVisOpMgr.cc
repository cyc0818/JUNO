#include "JVisLib/JVisOpMgr.h"

// ROOT 
#include "TTree.h"
#include "TEveTrack.h"
#include "TEveTrackPropagator.h"

// C/C++
#include <iostream>

using namespace std;

JVisOpMgr::JVisOpMgr()
    : m_opstepsTree(0)
    , m_trkList(0)
    , m_verb(1)
    , m_opstepsTrackID(0)
    , m_opstepsParentID(0)
    , m_opstepsPreT(0)
    , m_opstepsPreX(0)
    , m_opstepsPreY(0)
    , m_opstepsPreZ(0)
    , m_opstepsPostT(0)
    , m_opstepsPostX(0)
    , m_opstepsPostY(0)
    , m_opstepsPostZ(0)
    , m_opstepsOPType(0)
{

}

JVisOpMgr::~JVisOpMgr()
{

}

bool JVisOpMgr::init()
{
    initOpSteps();

    //m_prop = new TEveTrackPropagator();

    return true;
}

bool JVisOpMgr::setOpStepsTree(TTree* tree)
{
    if (tree == 0) {
        std::cout << __func__ << " opstepsTree does not exist " << std::endl;
        return false;
    }
    else {
        std::cout << __func__ << "  opstepsTree set" << std::endl;
        m_opstepsTree = tree;
    }

    return true;
}

bool JVisOpMgr::initOpSteps()
{
    if ( !m_opstepsTrackID  ) m_opstepsTrackID  = new std::vector<int>;
    if ( !m_opstepsParentID ) m_opstepsParentID = new std::vector<int>;
    if ( !m_opstepsPreT     ) m_opstepsPreT     = new std::vector<float>;
    if ( !m_opstepsPreX     ) m_opstepsPreX     = new std::vector<float>;
    if ( !m_opstepsPreY     ) m_opstepsPreY     = new std::vector<float>;
    if ( !m_opstepsPreZ     ) m_opstepsPreZ     = new std::vector<float>;
    if ( !m_opstepsPostT    ) m_opstepsPostT    = new std::vector<float>;
    if ( !m_opstepsPostX    ) m_opstepsPostX    = new std::vector<float>;
    if ( !m_opstepsPostY    ) m_opstepsPostY    = new std::vector<float>;
    if ( !m_opstepsPostZ    ) m_opstepsPostZ    = new std::vector<float>;
    if ( !m_opstepsOPType   ) m_opstepsOPType   = new std::vector<int>;

    if (m_opstepsTree == 0) {
        std::cout << __func__ << "m_opstepsTree == 0" << std::endl;
    }

    m_opstepsTree->SetBranchAddress("TrackID",  &m_opstepsTrackID);
    m_opstepsTree->SetBranchAddress("ParentID", &m_opstepsParentID);
    m_opstepsTree->SetBranchAddress("PreT",     &m_opstepsPreT);
    m_opstepsTree->SetBranchAddress("PreX",     &m_opstepsPreX);
    m_opstepsTree->SetBranchAddress("PreY",     &m_opstepsPreY);
    m_opstepsTree->SetBranchAddress("PreZ",     &m_opstepsPreZ);
    m_opstepsTree->SetBranchAddress("PostT",    &m_opstepsPostT);
    m_opstepsTree->SetBranchAddress("PostX",    &m_opstepsPostX);
    m_opstepsTree->SetBranchAddress("PostY",    &m_opstepsPostY);
    m_opstepsTree->SetBranchAddress("PostZ",    &m_opstepsPostZ);
    m_opstepsTree->SetBranchAddress("OPType",   &m_opstepsOPType);

    return true;
}


bool JVisOpMgr::clearOpSteps()
{
    if ( m_opstepsTrackID  ) m_opstepsTrackID->clear();
    if ( m_opstepsParentID ) m_opstepsParentID->clear();
    if ( m_opstepsPreT     ) m_opstepsPreT->clear();
    if ( m_opstepsPreX     ) m_opstepsPreX->clear();
    if ( m_opstepsPreY     ) m_opstepsPreY->clear();
    if ( m_opstepsPreZ     ) m_opstepsPreZ->clear();
    if ( m_opstepsPostT    ) m_opstepsPostT->clear();
    if ( m_opstepsPostX    ) m_opstepsPostX->clear();
    if ( m_opstepsPostY    ) m_opstepsPostY->clear();
    if ( m_opstepsPostZ    ) m_opstepsPostZ->clear();
    if ( m_opstepsOPType   ) m_opstepsOPType->clear();

    return true;
}

bool JVisOpMgr::readEvt(int iEvt)
{
    if ( 0 == m_opstepsTree ) {
        std::cout << __func__ << " m_opstepsTree does not exist" << std::endl;
        return false;
    }
    else {
        clearOpSteps();
        m_opstepsTree->GetEntry(iEvt);
        if ( 0 == m_opstepsTrackID ) {
            std::cout << __func__ << " failed to get opsteps TrackID" << std::endl;
            return false;
        }
    }

    return true;
}

bool JVisOpMgr::printCurEvt()
{
    if ( 0 == m_opstepsTrackID ) {
        std::cout << __func__ << " failed to get opsteps TrackID" << std::endl;
        return false;
    }
    else {
        std::cout << "  SimusEvent OP steps " << m_opstepsTrackID->size() << std::endl;
        for (int istep = 0; istep < (int)m_opstepsTrackID->size(); istep+=1000) {
            std::cout << "  step " << istep 
                      << " TrackID="  << (*m_opstepsTrackID)[istep] 
                      << " ParentID=" << (*m_opstepsParentID)[istep]
                      << " PreT="     << (*m_opstepsPreT)[istep]
                      << " PreXYZ=("  << (*m_opstepsPreX)[istep]  
                      << ", "         << (*m_opstepsPreY)[istep]  
                      << ", "         << (*m_opstepsPreZ)[istep]
                      << ")"
                      << " PostT="    << (*m_opstepsPostT)[istep]
                      << " PostXYZ=(" << (*m_opstepsPostX)[istep]
                      << ", "         << (*m_opstepsPostY)[istep]
                      << ", "         << (*m_opstepsPostZ)[istep]
                      << ")" 
                      << " OpType "   << (*m_opstepsOPType)[istep]
                      << std::endl;
        }

    }

    return true;
}

bool JVisOpMgr::clearTrks()
{
    if ( 0 != m_trkList ) {
        m_trkList->DestroyElements();
    }

    return true;
}

bool JVisOpMgr::buildTrks()
{
    if ( 0 == m_opstepsTrackID ) {
        std::cout << __func__ << " m_opstepsTrackID does not exist" << std::endl;
        return false;
    }

    if ( 0 == m_trkList ) {
        std::cout << __func__ << " m_trkList does not exist" << std::endl;
        return false;
    }

    int nsteps = m_opstepsTrackID->size();
    if ( m_verb > 1 ) {
        std::cout << "  SimusEvent OP steps " << nsteps << std::endl;
    }

    bool trkPassed = false;
    int preTrackID = -1;
    TEveTrack* trk = 0;
    for (int istep = 0; istep < nsteps; istep++) {
        int trackID  = (*m_opstepsTrackID)[istep];
        int parentID = (*m_opstepsParentID)[istep];
        float preT   = (*m_opstepsPreT)[istep];
        float preX   = (*m_opstepsPreX)[istep];
        float preY   = (*m_opstepsPreY)[istep];
        float preZ   = (*m_opstepsPreZ)[istep];
        float postT  = (*m_opstepsPostT)[istep];
        float postX  = (*m_opstepsPostX)[istep];
        float postY  = (*m_opstepsPostY)[istep];
        float postZ  = (*m_opstepsPostZ)[istep];
        int opType   = (*m_opstepsOPType)[istep];

        float preR   = sqrt(preX*preX+preY*preY+preZ*preZ);
        float postR  = sqrt(postX*postX+postY*postY+postZ*postZ);

        if ( m_verb > 2 ) {
            std::cout << "  step " << istep
                      << " TrackID="  << trackID << " ParentID=" << parentID
                      << " PreT="     << preT
                      << " PreXYZ=("  << preX << ", " << preY << ", " << preZ << ")"
                      << " PostT="     << postT
                      << " PostXYZ=("  << postX << ", " << postY << ", " << postZ << ")"
                      << " OPType=" << opType
                      << std::endl;
        }

        if ( opType == 3 ) trkPassed = true;

        if (trackID != preTrackID) {
            TEveRecTrackD rc;
            rc.fV.Set(preX, preY, preZ);
            rc.fSign = 0;

            if ( 0 != trk && trkPassed == true ) {
                m_trkList->AddElement(trk);
            } 

            trkPassed = false;
            trk = new TEveTrack(&rc, m_trkList->GetPropagator());
            trk->SetNextPoint(preX, preY, preZ);
            trk->SetNextPoint(postX, postY, postZ);

            trk->SetName(Form("trk%d", trackID));
            TString title = Form("OP ID=%d\n", trackID);
            title += Form("VTX T=%5.1fns pos(%8.1f, %8.1f, %8.1f) r=%7.1f\n", preT,  preX,  preY,  preZ, preR);
            title += Form("    T=%5.1fns pos(%8.1f, %8.1f, %8.1f) r=%7.1f type=%d\n", postT, postX, postY, postZ, postR, opType);
            trk->SetTitle(title);
            trk->SetLineColor(3);
            trk->SetMarkerColor(2);
            trk->SetMarkerSize(2);

            //m_trkList->AddElement(trk);
        }
        else {
            trk->SetNextPoint(postX, postY, postZ);
            TString title = trk->GetTitle();
            title += Form("    T=%5.1fns pos(%8.1f, %8.1f, %8.1f) r=%7.1f type=%d\n", postT, postX, postY, postZ, postR, opType);
            trk->SetTitle(title);            
        }
        preTrackID = trackID;
    }
    if ( 0 != trk && trkPassed == true ) {
        m_trkList->AddElement(trk);
    }

    //m_trkList->MakeTracks(1);  // no tracks shown if add this line

    return true;
}
