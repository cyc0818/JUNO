#include "OECTagSvc/OECTagSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SharedElemFactory.h"
#include <fstream>
#include <algorithm>
#include "OnlineConfigManager/OnlineConfigManager.h"

DECLARE_SERVICE(OECTagSvc);
DECLARE_CAN_BE_SHARED(OECTagSvc);

OECTagSvc::OECTagSvc(const std::string& name):SvcBase(name){
    declProp("OECTagFile",s_configFile="tag.json");
}

OECTagSvc::~OECTagSvc(){
}

bool OECTagSvc::initialize(){
    if (!resolveTag()){
        LogError<<"Fail to resolve the configuration file!"<<std::endl;
        return false;
    }

    //----------print out the map---------
    LogInfo<<"=================tag map==============="<<std::endl;
    for (auto it : m_tags){
        LogInfo<<it.first<<"  =====>  "<<std::endl;
        LogInfo<<"            format     tag"<<std::endl;
        LogInfo<<"   prompt:  "<<it.second.storeFormat_p<<"    "<<it.second.ptag<<std::endl;
        LogInfo<<"   delayed: "<<it.second.storeFormat_d<<"    "<<it.second.dtag<<std::endl;
    }
    LogInfo<<"================================"<<std::endl;
    return true;
}

bool OECTagSvc::finalize(){
    return true;
}

std::vector<std::string> OECTagSvc::getTagName(){
    std::vector<std::string> vtagnames;
    for (auto it : m_tags){
        vtagnames.push_back(it.first);
    }
    return vtagnames;
}

int OECTagSvc::getStoreFormat(int tag){
    int rval = 0;
    for (auto it : m_format){
        if (it.first<1){//tags not to store waveform
            continue;
        }
        else{//tags to store waveform
            for (auto ait : it.second){
                //if ((tag | ait)==ait){
                if ((tag | ~ait)==0xFFFFFFFF){
                    rval = 1;
                    break;
                }
            }
        }
    }
    return rval;
}

int OECTagSvc::getStoreFormat(std::string tagname, bool isDelayed){
    if (isDelayed) return m_tags.at(tagname).storeFormat_d;
    else return m_tags.at(tagname).storeFormat_p;
}

std::string OECTagSvc::getpTag(std::string tagname){
    return m_tags.at(tagname).ptag;
}

std::string OECTagSvc::getdTag(std::string tagname){
    return m_tags.at(tagname).dtag;
}

bool OECTagSvc::resolveTag(){
    //First, get the map from tagname to tag
    m_tags.clear();

    s_configFile = OnlineConfigManager::resolveConfigFilePath(s_configFile);
    std::ifstream icfg(s_configFile);
    SniperJSON json = SniperJSON::load(icfg);
    for (int ith=0;ith<json.size();ith++){
        SniperJSON &json_tag = json[ith];
        THETAG thistag;
        //The tag name
        if (json_tag.find("name")==json_tag.map_end()) return false;
        std::string tagname = json_tag["name"].get<std::string>();
        //The storage strategy
        if (json_tag.find("storeFormat")==json_tag.map_end()) return false;
        int stf = json_tag["storeFormat"].get<int>();
        //The tag value
        if (json_tag.find("tag")!=json_tag.map_end()){//single tag
            thistag.ptag = json_tag["tag"].get<std::string>();
            thistag.storeFormat_p = stf%10;
        }
        else if (json_tag.find("ptag")!=json_tag.map_end() && json_tag.find("dtag")!=json_tag.map_end()){//correlated tag
            thistag.ptag = json_tag["ptag"].get<std::string>();
            thistag.dtag = json_tag["dtag"].get<std::string>();
            thistag.storeFormat_p = stf/10;
            thistag.storeFormat_d = stf%10;
        }
        else{
            return false;
        }

        m_tags[tagname] = thistag;
    }

    //Then, calculate the map from format to tagvalues
    m_format.clear();
    for (auto it : m_tags){
        int theformat_p = it.second.storeFormat_p;
        int theformat_d = it.second.storeFormat_d;

        std::string ptag = it.second.ptag;
        int i_ptag = strtoll(ptag.c_str(),NULL,16);
        m_format[theformat_p].push_back(i_ptag);

        std::string dtag = it.second.dtag;
        if (!dtag.compare("")) continue;
        int i_dtag = strtoll(dtag.c_str(),NULL,16);
        m_format[theformat_d].push_back(i_dtag);
    }
    return true;
}
