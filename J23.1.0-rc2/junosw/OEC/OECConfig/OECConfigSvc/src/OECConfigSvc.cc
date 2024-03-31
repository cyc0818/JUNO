#include "OECConfigSvc/OECConfigSvc.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"
#include <fstream>
#include <float.h>
#include "OnlineConfigManager/OnlineConfigManager.h"

DECLARE_SERVICE(OECConfigSvc);

OECConfigSvc::OECConfigSvc(const std::string& name): SvcBase(name), m_OECTagVec(0){
    declProp("OECListFile", s_configFile= "sig.xml");
}

OECConfigSvc::~OECConfigSvc(){
    for (unsigned int i = 0; i < m_OECTagVec.size(); i++) delete m_OECTagVec[i];
    m_OECTagVec.clear();
}

bool OECConfigSvc::initialize(){
    LogDebug << "Initializing..." << std::endl;
    SniperPtr<OECTagSvc> tagsvc(getParent(),"OECTagSvc");
    if (tagsvc.invalid()){
        LogError << "Unable to locate OECTagSvc" << std::endl;
        return false;
    }
    m_tagsvc = tagsvc.data();
    m_tagNames = m_tagsvc->getTagName();

    if (!resolveTag()){
        LogError<<"Fail to resolve the configuration file!"<<std::endl;
        return false;
    }

    //Print the tags and their conditions
    LogInfo<<"===============tag condition=============="<<std::endl;
    for (auto it : m_OECTagVec){
        LogInfo<<"----------------------"<<std::endl;
        LogInfo<<"tag name:"<<it->getTagName()<<std::endl;
        LogInfo<<"tag conditions:"<<std::endl;
        std::vector<OECTag::Condition> vconds = it->getConditionVec();
        for (auto oit : vconds){
            LogInfo<<oit.name<<" ----- "<<oit.min<<" - "<<oit.max<<std::endl;
        }
    }
    LogInfo<<"----------------------"<<std::endl;
    LogInfo<<"=========================================="<<std::endl;
    return true;
}

bool OECConfigSvc::finalize(){
    return true;
}

bool OECConfigSvc::resolveTag(){
    s_configFile = OnlineConfigManager::resolveConfigFilePath(s_configFile);
    std::ifstream icfg(s_configFile);
    SniperJSON json = SniperJSON::load(icfg);
    int m_idTag = 0;
    for (int ith=0;ith<json.size();ith++){
        m_idTag++;
        SniperJSON &json_tag = json[ith];
        OECTag* tmp_oectag = new OECTag();
        //The tag name
        if (json_tag.find("tagname")==json_tag.map_end()){
            delete tmp_oectag;
            return false;
        }
        std::string tagname = json_tag["tagname"].get<std::string>();
        tmp_oectag->setTagName(tagname);
        //The tag value
        std::string helppTagValue = m_tagsvc->getpTag(tagname);
        std::string helpdTagValue = m_tagsvc->getdTag(tagname);
        if (!helppTagValue.compare("") || !helpdTagValue.compare("")){
            LogError<<"there should be 2 tag value for correlated event!please check the configuration file!"<<std::endl;
            delete tmp_oectag;
            return false;
        }
        int ptagValue=strtoll(helppTagValue.c_str(), NULL, 16);
        int dtagValue=strtoll(helpdTagValue.c_str(), NULL, 16);
        tmp_oectag->setTagValue(ptagValue, dtagValue);
        tmp_oectag->setId(m_idTag);
        //The conditions
        if (json_tag.find("conditions")==json_tag.map_end()){
            delete tmp_oectag;
            return false;
        }
        SniperJSON &json_conds = json_tag["conditions"];
        int condid = 0;
        for (int jth=0;jth<json_conds.size();jth++){
            condid++;
            SniperJSON &json_cond = json_conds[jth];
            //The condition name
            if (json_cond.find("name")==json_cond.map_end()){
                delete tmp_oectag;
                return false;
            }
            std::string condname = json_cond["name"].get<std::string>();
            //The min
            float condmin = -FLT_MAX;
            if (json_cond.find("min")!=json_cond.map_end()){
                condmin = json_cond["min"].get<float>();
            }
            //The max
            float condmax = FLT_MAX;
            if (json_cond.find("max")!=json_cond.map_end()){
                condmax = json_cond["max"].get<float>();
            }
            tmp_oectag->addCondition(condname, condmin, condmax, condid);
        }
        m_OECTagVec.push_back(tmp_oectag);
    }
    return true;
}

const std::vector<OECTag*>& OECConfigSvc::getOECTagVec() const{
    return m_OECTagVec; 
}
