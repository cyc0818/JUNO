#ifndef JUNO_VIS_OPMGR_H
#define JUNO_VIS_OPMGR_H

//
//  Juno Visualization Optical Photon Track Manager
//
//  Author: Zhengyun You  2016-04-19
//

#include <vector>
#include <map>

class TTree;
class TEveTrackPropagator;
class TEveTrackList;

class JVisOpTrack;

class JVisOpMgr 
{
    public :

        JVisOpMgr();
        virtual ~JVisOpMgr();

        bool init();
        bool initOpSteps();
        bool clearOpSteps();

        bool setOpStepsTree(TTree* tree);
        bool readEvt(int iEvt);
        bool printCurEvt();

        bool clearTrks();
        bool buildTrks();
        void setTrkList(TEveTrackList* trkList) { m_trkList = trkList; }
        TEveTrackList* getTrkList() { return m_trkList; }

    private :

        TTree* m_opstepsTree;
        TEveTrackList *m_trkList;
        //TEveTrackPropagator* m_prop;

        int m_verb;

    public :

        std::vector<int>*   m_opstepsTrackID;
        std::vector<int>*   m_opstepsParentID;
        std::vector<float>* m_opstepsPreT;
        std::vector<float>* m_opstepsPreX;
        std::vector<float>* m_opstepsPreY;
        std::vector<float>* m_opstepsPreZ;
        std::vector<float>* m_opstepsPostT;
        std::vector<float>* m_opstepsPostX;
        std::vector<float>* m_opstepsPostY;
        std::vector<float>* m_opstepsPostZ;
        std::vector<int>*   m_opstepsOPType;

};

#endif // JUNO_VIS_OPMGR_H


