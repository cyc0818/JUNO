#ifndef _SIGNATURE_
#define _SIGNATURE_

#include <utility>
#include <map>
#include <vector>
#include <string>
#include <list>

class OECTag
 {
 public:
  typedef struct {
    std::string      name;
    float            min;
    float            max;
    int              id;
  } Condition;

 public:
  OECTag();
  ~OECTag();
  int getId() const;
  void setId(int); 
  void print();
  
  int getTagValue() const;
  int getpTagValue() const;
  int getdTagValue() const;
  void setTagValue(int, int);
  void setTagName(std::string label);
  std::string getTagName() const;

  void addCondition(const std::string& name, float min, float max,int id);
  std::vector<Condition> getConditionVec(){
      return m_conditionVec;
  } 

 private:
  std::vector<Condition> m_conditionVec;

  int m_Id; 
  int m_ptagValue; 
  int m_dtagValue; 
  std::string m_tagName;
};
#endif
