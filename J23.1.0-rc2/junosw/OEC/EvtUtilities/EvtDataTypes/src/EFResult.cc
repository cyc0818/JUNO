#include <iostream>
#include "EvtDataTypes/EFResult.h"

EFResult::EFResult(std::vector<std::string> ans)
{
}

EFResult::~EFResult(){}

bool EFResult::push_back(uint32_t val){
  if(m_efVec.size()>0&&m_efVec.size()<54) return false;
  if(m_efVec.size()==0) m_efVec.resize(54,0);
  m_efVec.push_back(val);
  return true;
}

void EFResult::setMidAnswer(std::string &answer)
{
  m_midanswerIndex++;
  m_midAnswerVector.push_back(answer);
}

void EFResult::reset()
{
  m_answer = "Unknown";
  m_mark = "Unknown";
  m_midanswerIndex = -1;
  
}

void EFResult::setAnswer(std::string &answer){ m_answer=answer; }
const std::string EFResult::getAnswer() const { return m_answer; }

void EFResult::setMark(std::string &mark){ m_mark=mark; }
const std::string EFResult::getMark() const { return m_mark; }


const std::string EFResult::getMidAnswer() const { return m_midAnswerVector[m_midanswerIndex]; }
const int EFResult::getMidAnswerIndex() const { return m_midanswerIndex; }
