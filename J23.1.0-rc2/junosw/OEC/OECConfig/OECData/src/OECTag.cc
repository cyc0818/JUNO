#include "OECData/OECTag.h"
#include "SniperKernel/SniperLog.h"
OECTag::OECTag(){}
OECTag::~OECTag() {
  m_conditionVec.clear();
}

/// Print method. //
void OECTag::print()
{
  LogDebug<<"start tag"<<std::endl;
  LogDebug<< "tag_name: " << m_tagName
       << "; ptag_value: " << m_ptagValue 
       << "; dtag_value: " << m_dtagValue 
       << "; tag_id: " << m_Id<<std::endl;
  for(std::vector<Condition>::iterator i =m_conditionVec.begin();i!=m_conditionVec.end(); ++i){
       LogDebug<<"Condition: "<<(*i).name<<" - "<<(*i).min<<" - "<<(*i).max<<" - "<<(*i).id<<std::endl;
  }
  LogDebug<<"end show tag!"<<std::endl;
}

/// Returns the id //
int OECTag::getId() const { return m_Id; }

/// set the id //
void OECTag::setId(int id){ m_Id=id; }

///tagValue //
int OECTag::getTagValue() const
{
  return m_ptagValue;
}

int OECTag::getpTagValue() const
{
  return m_ptagValue;
}

int OECTag::getdTagValue() const
{
  return m_dtagValue;
}

void OECTag::setTagValue(int pvalue, int dvalue)
{
  m_ptagValue = pvalue;
  m_dtagValue = dvalue;
}
//tagName
void OECTag::setTagName(std::string label) {
  m_tagName = label;
}

std::string OECTag::getTagName() const {
  return m_tagName;
}
//condition
void OECTag::addCondition(const std::string& name, float min, float max,int id) {
  Condition it = {name, min, max, id};
  m_conditionVec.push_back(it);
}


