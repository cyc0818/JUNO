#include "SpmtElecConfigSvc/SpmtElecConfigSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SharedElemFactory.h"

#include <fstream>
#include <sstream>
#include <locale>
#include <algorithm>
#include <stdexcept>

DECLARE_SERVICE(SpmtElecConfigSvc);
DECLARE_CAN_BE_SHARED(SpmtElecConfigSvc);

SpmtElecConfigSvc::SpmtElecConfigSvc(const std::string& name)
    : SvcBase(name)
{
      declProp("ConfigFile", m_config_file="");
}

SpmtElecConfigSvc::~SpmtElecConfigSvc()
{

}

bool SpmtElecConfigSvc::initialize()
{
  LogDebug << " Initializing SpmtElecConfigSvc " << std::endl;

  if(m_config_file.size() > 0 && m_config_file[0] != '/' && getenv("OEC_ONLINE_CONFIG") != nullptr){//when using a relative path in online, use ENV to get a full path
    std::string prefix = getenv("OEC_ONLINE_CONFIG");
    m_config_file = prefix + "/" + m_config_file;
  }
  bool status = load(m_config_file);

  return true;
}

bool SpmtElecConfigSvc::finalize()
{
  LogDebug << " Finalizing SpmtElecConfigSvc " << std::endl;
  return true;
}

bool SpmtElecConfigSvc::load(std::string path)
{
  std::string line;
  std::ifstream inFile(path.c_str());
  if(inFile.is_open()){
    while( getline (inFile,line) ){

      // skip blanck lines
      bool isBlanck = true;
      for (std::string::const_iterator it = line.begin(); it != line.end(); ++it){
        if (!isspace(*it)) isBlanck = false;
      }
      if(isBlanck) continue;

      // if not blanck parse
      LogDebug << line << '\n';

      std::istringstream iss(line);
      std::string tag;
      iss >> tag; 
      if(tag=="#") continue;
      if(tag=="int"){
      std::string key;
        int value;
        iss >> key;
        iss >> value;
        intParams.insert(std::pair<std::string,int>(key,value));
        LogDebug << " int param " << std::endl;
      }
      if(tag=="double"){
      std::string key;
        double value;
        iss >> key;
        iss >> value;
        doubleParams.insert(std::pair<std::string,double>(key,value));
        LogDebug << " double param " << std::endl;
      }
      if(tag=="string"){
      std::string key;
        std::string value;
        iss >> key;
        iss >> value;
        stringParams.insert(std::pair<std::string,std::string>(key,value));
        LogDebug << " string param " << std::endl;
      }
     
      if(tag=="pmt"){
        int key,value;
        iss >> value;
        iss >> key;
        ChannelParams.insert(std::pair<int,int>(key,value));
	PMTParams.insert(std::pair<int,int>(value,key));
        LogDebug << " Channel param " << std::endl;
      }
 
    }
    inFile.close();
  }
  else return false;


  return true;
}

int SpmtElecConfigSvc::getInt(std::string name){
  if(intParams.find(name)==intParams.end()) throw std::out_of_range("Unable to find "+name);
  return intParams.find(name)->second;
}

double SpmtElecConfigSvc::getDouble(std::string name){
  if(doubleParams.find(name)==doubleParams.end()) throw std::out_of_range("Unable to find "+name);
  return doubleParams.find(name)->second;
}

std::string SpmtElecConfigSvc::getString(std::string name){
  if(stringParams.find(name)==stringParams.end()) throw std::out_of_range("Unable to find "+name);
  return stringParams.find(name)->second;
}

int SpmtElecConfigSvc::getChannel(int name){
  if(ChannelParams.find(name)==ChannelParams.end()) throw std::out_of_range("Unale to find"+name);
  return ChannelParams.find(name)->second;
}
int SpmtElecConfigSvc::getPMT(int name){
  if(PMTParams.find(name)==PMTParams.end()) throw std::out_of_range("Unale to find"+name);
  return PMTParams.find(name)->second;
}

