#include "EvtSteering/StepDecision.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include <fstream>
#include <algorithm>
#include "OnlineConfigManager/OnlineConfigManager.h"

DECLARE_TOOL(StepDecision);

StepDecision::StepDecision(const std::string &name): ToolBase(name){
    declProp("IsEnabled", m_isEnabled=false);
    declProp("ConfigFile", m_cfg="");
}

StepDecision::~StepDecision(){
}

bool StepDecision::initialize(){
    //get the tagName --> tag map
    SniperPtr<OECTagSvc> tagsvc(getParent(),"OECTagSvc");
    if (tagsvc.invalid()){
        LogError << "Unable to locate OECTagSvc" << std::endl;
        return false;
    }
    m_tagsvc = tagsvc.data();
    m_tagNames = m_tagsvc->getTagName();

    //fill m_SigMap
    m_cfg = OnlineConfigManager::resolveConfigFilePath(m_cfg);
    std::ifstream icfg(m_cfg);
    SniperJSON json = SniperJSON::load(icfg);
    for (int i_sig = 0;i_sig<json.size();i_sig++){//the signature
        SniperJSON &json_sig = json[i_sig];
        Signature *thisSignature = getSignature(json_sig);
        m_SigMap[thisSignature->label()] = thisSignature;
        thisSignature->print();
    }

    return true;
}

Signature *StepDecision::getSignature(SniperJSON &json_sig){
    std::string id = json_sig["signature_id"].get<std::string>();
    int prescale = 1;
    if (json_sig.find("prescale")!=json_sig.map_end()){
        prescale = json_sig["prescale"].get<int>();
    }
    float forcedAccept = 1;
    if (json_sig.find("forced_accept")!=json_sig.map_end()){
        prescale = json_sig["forced_accept"].get<float>();
    }
    Signature *thisSignature = new Signature();
    thisSignature->setSigLabel(id);
    thisSignature->setPrescale(prescale);
    thisSignature->setForcedAccept(forcedAccept);
    //get criterias
    SniperJSON &json_crits = json_sig["criterias"];
    i_critID = 0;
    for (int i_crit=0;i_crit<json_crits.size();i_crit++){
        SniperJSON &json_crit = json_crits[i_crit];
        EvtCriteria *thisCrit = getCriteria(json_crit);
        thisSignature->makeVectors(thisCrit);
        i_critID++;
    }
    return thisSignature;
}

EvtCriteria *StepDecision::getCriteria(SniperJSON &json_crit){
    EvtCriteria *thisCrit = new EvtCriteria(i_critID);
    std::string behaviour = "";
    if (json_crit.find("behaviour")!=json_crit.map_end()){
        behaviour = json_crit["behaviour"].get<std::string>();
        if (!behaviour.compare("reject")) behaviour+="_reject";
    }
    if (json_crit.find("next")!=json_crit.map_end()){
        if (behaviour.compare("jump")){
            LogError<< "Unconsistent of behaviour and next" << std::endl;
            exit(1);
        }
        std::string nextID = json_crit["next"].get<std::string>();
        behaviour = behaviour + '_' + nextID;
    }
    if (json_crit.find("mark")!=json_crit.map_end()){
        if ((behaviour!= "accept")&&(behaviour!= "continue")) {
            LogError<< "Unconsistent of behaviour and mark" << std::endl;
            exit(1);
        }
        std::string theMark = json_crit["mark"].get<std::string>();
        //check if theMark is legal
        if (find(m_tagNames.begin(), m_tagNames.end(), theMark)==m_tagNames.end()){
            LogError<<"Illegal mark!please check the sig.json file!"<<std::endl;
            exit(1);
        }
        behaviour = behaviour + '_' + theMark + '-' + m_tagsvc->getpTag(theMark);
    }
    //behaviour get done, check it
    if (behaviour.find('_') == std::string::npos) {
        LogError<< "Unvalid behaviour ==> " << behaviour << std::endl;
        exit(1);
    }
    thisCrit->setBehaviour(behaviour);
    thisCrit->setScope(dynamic_cast<Task*>(getParent())); 
    //get items
    SniperJSON &json_items = json_crit["items"];
    i_itemID = 0;
    for (int i_item=0;i_item<json_items.size();i_item++){
        SniperJSON &json_item = json_items[i_item];
        addItem(thisCrit, json_item);
        i_itemID++;
    }
    return thisCrit;
}

void StepDecision::addItem(EvtCriteria *thisCrit, SniperJSON &json_item){
    //check this item
    //item should have: "name", "value", "type"
    if (json_item.size()!=3){
        LogError<<"Illegal item!"<<std::endl;
        exit(1);
    }
    std::string itemName = "";
    float itemValue = 0;
    std::string itemType = "";
    if (json_item.find("name")!=json_item.map_end()){
        itemName = json_item["name"].get<std::string>();
    }
    else{
        LogError<<"Item key 'name' is not defined!"<<std::endl;
        exit(1);
    }
    if (json_item.find("value")!=json_item.map_end()){
        itemValue = json_item["value"].get<float>();
    }
    else{
        LogError<<"Item key 'value' is not defined!"<<std::endl;
        exit(1);
    }
    if (json_item.find("type")!=json_item.map_end()){
        itemType = json_item["type"].get<std::string>();
    }
    else{
        LogError<<"Item key 'type' is not defined!"<<std::endl;
        exit(1);
    }
    thisCrit->addItem(itemName, itemValue, itemType, i_itemID);
}

bool StepDecision::finalize(){
    //delete m_SigMap
    for (auto it : m_SigMap){
        delete it.second;
    }
    m_SigMap.clear();
    return true;
}

int StepDecision::execDecision(const std::string& sigID){
    LogDebug << " Decision execute: " << sigID << std::endl;

    Signature* sig = m_SigMap[sigID];
    int decision = sig->isCriteriaMatched();

    m_continue =sig->midresult();
    m_behaviour = sig->result();
    return decision;
}

std::vector<std::string> StepDecision::getSigIDs(){
    std::vector<std::string> vsigid;
    for (auto it : m_SigMap){
        vsigid.push_back(it.first);
    }
    return vsigid;
}
