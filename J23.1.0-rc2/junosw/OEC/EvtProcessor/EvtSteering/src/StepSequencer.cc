#include "EvtSteering/StepSequencer.h"
#include "SniperKernel/SniperJSON.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/Sniper.h"
#include "EvtSteerData/Sequence.h"
#include "SniperKernel/AlgBase.h"
#include "OnlineConfigManager/OnlineConfigManager.h"

#include <fstream>

DECLARE_TOOL(StepSequencer);

StepSequencer::StepSequencer(const std::string &name): ToolBase(name){
    declProp("IsEnabled", m_isEnabled=false);
    declProp("ConfigFile", m_cfg="");
}

StepSequencer::~StepSequencer(){
}

bool StepSequencer::initialize(){
    if (!m_cfg.compare("")){
        LogWarn<<"The seq.json file is not defined!"<<std::endl;
        return true;
    }
    m_cfg = OnlineConfigManager::resolveConfigFilePath(m_cfg);
    std::ifstream icfg(m_cfg);
    SniperJSON json = SniperJSON::load(icfg);

    d_algtime = 0;
    m_timeTree.clear();
    for (int i = 0; i < json.size(); ++i){
        SniperJSON &json_i = json[i];
        std::string id = json_i["sequence_id"].get<std::string>();
        LogInfo << "sequence: " << id << std::endl;
        Sequence* m_helpSequence = new Sequence();
        m_helpSequence->setSeqID(id);
        m_helpSequence->setInputID("");
        m_helpSequence->setOutputID("");
        m_seqs[id] = m_helpSequence;

        SniperJSON &json_algs = json_i["algorithms"];
        for (int j = 0; j < json_algs.size(); ++j){
            SniperJSON &json_alg = json_algs[j];
            DLElement* alg = Sniper::create(json_alg.str(-1));
            AlgBase *thisalg = dynamic_cast<AlgBase*>(alg);
            thisalg->setParent(getParent());
            std::string algname = thisalg->tag();
            m_timeTree[algname] = NULL;
            if (!thisalg->initialize()){
                LogError<<"Error when initializing"<<json_alg["identifier"].get<std::string>()<<std::endl;
                return false;
            }
            m_seqs[id]->makeVectors(thisalg);
        }
        m_seqs[id]->print();
    }

#ifndef BUILD_ONLINE
    //For testing the time of reconstruction algorithms
    //Get the rootwriter
    SniperPtr<RootWriter> rwsvc(getParent(),"RootWriter");
    if ( rwsvc.invalid() ) {
        LogError << "cannot get the rootwriter service." << std::endl;
        return false;
    }
    //Get the JunoTimer for time testing
    SniperPtr<IJunoTimerSvc>  junotimersvc(getParent(), "JunoTimerSvc");
    if ( junotimersvc.invalid() ) {
        LogError << "cannot get the junotimer service." << std::endl;
        return false;
    }
    IJunoTimerSvc* thetimersvc = junotimersvc.data();
    m_timer = thetimersvc->get("RECTIMER");
    //Define the tree to store time for algorithms
    LogInfo<<"Reconstruction algorithms used:"<<std::endl;
    for (auto &timetree : m_timeTree){
        std::string algname = timetree.first;
        LogInfo<<algname<<std::endl;
        TTree *tmptree = rwsvc->bookTree(*m_par, Form("TIME_REC/Time/%s", algname.c_str()), Form("The time consumption for %s", algname.c_str()));
        tmptree->Branch("time", &d_algtime, "time/D");
        timetree.second = tmptree;
    }
#endif

    return true;
}

bool StepSequencer::finalize(){
    for (auto &it : m_seqs){
        std::vector<AlgBase*>& algs = it.second->algoVector();
        for (AlgBase* thisalg : algs){
            if (!thisalg->finalize()){
                LogError<<"Error when finalizing algorithm:"<<thisalg->objName()<<std::endl;
                return false;
            }
            delete thisalg;
        }
        delete it.second;
    }
    m_seqs.clear();

    return true;
}

bool StepSequencer::execSequence(const std::string &seq_id){
    std::vector<AlgBase*>& algs = m_seqs[seq_id]->algoVector();
    for (auto alg : algs) {
#ifndef BUILD_ONLINE
        m_timer->start();
#endif
        if (!alg->execute()){
            return false;
        }
#ifndef BUILD_ONLINE
        m_timer->stop();
        d_algtime = m_timer->elapsed();
        m_timeTree[alg->tag()]->Fill();
#endif
    }

    return true;
}

std::vector<std::string> StepSequencer::getSeqIDs(){
    std::vector<std::string> vseqid;
    for (auto it : m_seqs){
        vseqid.push_back(it.first);
    }
    return vseqid;
}
