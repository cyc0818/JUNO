#include <boost/python.hpp>
#include <boost/filesystem.hpp>
#include <Geometry/TTGeomSvc.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <SniperKernel/SniperLog.h>
#include "SniperKernel/SvcFactory.h"

DECLARE_SERVICE(TTGeomSvc);

TTGeomSvc::TTGeomSvc(const std::string& name)
  : SvcBase(name)
  , m_db_type("File"), m_db_path()
{
  declProp("DBType", m_db_type);
  declProp("Path", m_db_path);
}

TTGeomSvc::~TTGeomSvc()
{

}

bool TTGeomSvc::initialize() {
  bool st;

  // init global info
  st = init_default();
  if (!st) { return st; }

  // FILE MODE
  if (m_db_type == "File") {
    st = init_file();
  } else if (m_db_type == "ROOT") {

  } else if (m_db_type == "MySQL") {

  } else {
  }

  return st;
}

bool TTGeomSvc::finalize() {
  return true;
}

int TTGeomSvc::getCT(int channel,int position) {
  return CT[channel][position]; 
}

double TTGeomSvc::getChannelPos(unsigned int channel, int xyz) {
  for(int jj=0;jj<sizeof(ChannelID)/sizeof(ChannelID[0]);jj++)
    {
      if(ChannelID[jj]==channel)
	return ChannelPos[jj][xyz];
    } 
  LogError << "Can't find TT channel" << channel << std::endl;
  return 0;
}

// END User Interface

// Below is private methods

bool TTGeomSvc::init_default() {
  
  return true;
}

bool TTGeomSvc::init_file() {
  bool st = true;
  

  //Cross talk file
  if (getenv("JUNO_GEOMETRY_PATH")) {
    std::string s = getenv("JUNO_GEOMETRY_PATH");
    s += "/TopTracker/CrossTalk.txt";
    m_db_path = s;
  }
  else{
      LogError << "JUNO_GEOMETRY_PATH not defined. This should have been defined when configuring offline!" << std::endl;
      return false;
  }

  if (!m_db_path.empty() && boost::filesystem::exists(m_db_path)) {
      LogInfo << "Loading parameters from file: " << m_db_path << std::endl;
  } else {
      LogError << "Can't find TTGeom parameters file '" << m_db_path << "'." << std::endl;
      st = false;
      return st;
  }

  std::ifstream input(m_db_path.c_str());
  std::string tmp_line;
  int channel;  
  int neighbour;
  int position;

  while (input.good()) {
    std::getline(input, tmp_line);
    if (input.fail()) { break; }

    std::stringstream ss;
    ss << tmp_line;
    
    ss >> channel;
    if (ss.fail()) { continue; }

    // reset the default value if failed to parse
    ss >> position;
    if (ss.fail()) { continue; }

    // reset the default value if failed to parse
    ss >> neighbour;
    if (ss.fail()) { continue; }

    CT[channel][position]=neighbour;

    LogDebug << "CT saved: " <<channel<<" "<<position<<" "<<neighbour << std::endl;
  }

  m_db_path="";

  //Channel position file
  if (getenv("JUNO_GEOMETRY_PATH")) {
    std::string s = getenv("JUNO_GEOMETRY_PATH");
    s += "/TopTracker/ChannelPosition.txt";
    m_db_path = s;
  }
  else{
      LogError << "JUNO_GEOMETRY_PATH not defined. This should have been defined when configuring offline!" << std::endl;
      return false;
  }

  if (!m_db_path.empty() && boost::filesystem::exists(m_db_path)) {
    LogInfo << "Loading parameters from file: " << m_db_path << std::endl;
  } else {
    LogError << "Can't find TTGeom parameters file '" << m_db_path << "'." << std::endl;
    st = false;
    return st;
  }

  std::ifstream input2(m_db_path.c_str());
  std::string tmp_line2;
  unsigned int channelDM;  
  double posx,posy,posz;
  int counter(0);

  while (input2.good()) {
    std::getline(input2, tmp_line2);
    if (input2.fail()) { break; }

    std::stringstream ss2;
    ss2 << tmp_line2;
    
    ss2 >> channelDM;
    if (ss2.fail()) { continue; }

    // reset the default value if failed to parse
    ss2 >> posx;
    if (ss2.fail()) { continue; }

    // reset the default value if failed to parse
    ss2 >> posy;
    if (ss2.fail()) { continue; }

    // reset the default value if failed to parse
    ss2 >> posz;
    if (ss2.fail()) { continue; }

    ChannelPos[counter][0]=posx;
    ChannelPos[counter][1]=posy;
    ChannelPos[counter][2]=posz;
    ChannelID[counter]=channelDM;
    counter++;

    LogDebug << "Channel position saved: " <<counter<<" "<<channelDM<<" "<<posx<<" "<<posy<<" "<<posz<< std::endl;
  }




  return st;
}
