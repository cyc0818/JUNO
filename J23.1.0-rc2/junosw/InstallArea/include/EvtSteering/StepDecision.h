#ifndef STEPDECISSION_H
#define STEPDECISSION_H
#include "SniperKernel/ToolBase.h"
#include "EvtSteerData/Signature.h"
#include "OECTagSvc/OECTagSvc.h"

class StepDecision: public ToolBase{
    public:
        StepDecision(const std::string&);
        ~StepDecision();

        bool initialize();
        bool finalize();

        int execDecision(const std::string& sigID);  
        std::vector<std::string> getSigIDs();
        const std::string& getBehaviour() const { return m_behaviour; }
        const std::string& getContinue() const { return m_continue; }

    private:
        Signature *getSignature(SniperJSON&);
        EvtCriteria *getCriteria(SniperJSON&);
        void addItem(EvtCriteria*, SniperJSON&);

        OECTagSvc* m_tagsvc;
        std::vector<std::string> m_tagNames;

        bool m_isEnabled;
        std::string m_cfg;

        std::string m_behaviour;
        std::string m_continue;
        std::map<std::string, Signature*> m_SigMap;
        int i_critID;
        int i_itemID;

};
#endif
