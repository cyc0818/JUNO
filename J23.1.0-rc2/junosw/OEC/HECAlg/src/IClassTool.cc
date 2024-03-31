#include "HECAlg/IClassTool.h"
#include "SniperKernel/SniperLog.h"

classifyQuantity::classifyQuantity(){
}

classifyQuantity::classifyQuantity(const std::vector<std::string>& qnames){
    addName(qnames);
}

classifyQuantity::~classifyQuantity(){
}

void classifyQuantity::addName(std::string qname){
    if (m_classQuantity.find(qname)==m_classQuantity.end()){
        v_qname.push_back(qname);
        m_classQuantity[qname] = 0;
    }
    else{
        LogWarn<<"The quantity has been already defined!"<<std::endl;
    }
}

void classifyQuantity::addName(const std::vector<std::string>& qnames){
    for (auto thisname : qnames){
        addName(thisname);
    }
}

bool classifyQuantity::setValue(std::string qname, float qvalue){
    if (m_classQuantity.find(qname)==m_classQuantity.end()){
        LogError<<"The quantity ("<<qname<<") is not defined!"<<std::endl;
        return false;
    }
    m_classQuantity[qname] = qvalue;
    return true;
}

bool classifyQuantity::setValue(const std::map<std::string, float>& qvalues){
    for (auto thisvalue : qvalues){
        setValue(thisvalue.first, thisvalue.second);
    }
    return true;
}

float classifyQuantity::getValue(std::string name) const{
    if (m_classQuantity.find(name)==m_classQuantity.end()){
        LogError<<"The quantity ("<<name<<") is not defined!"<<std::endl;
        return 0;
    }
    return m_classQuantity.at(name);
}

std::vector<float> classifyQuantity::getValue() const{
    std::vector<float> vvalue;
    for (auto thepair : m_classQuantity){
        vvalue.push_back(thepair.second);
    }
    return vvalue;
}

IClassTool::IClassTool(){
}

IClassTool::~IClassTool(){
}
