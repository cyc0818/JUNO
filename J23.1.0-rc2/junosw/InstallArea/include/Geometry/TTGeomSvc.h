#ifndef TTGeomSvc_h
#define TTGeomSvc_h

//
//  Description: allow user query TTCross talk paramters from this service.
//               The parameters could be loaded from plain text or database.
//
//


#include "SniperKernel/SvcBase.h"
#include <map>


class TTGeomSvc: public SvcBase {
 public:
  TTGeomSvc(const std::string& name);
  virtual ~TTGeomSvc();
  
  bool initialize();
  bool finalize();
  
  // User interface

  int getCT(int channel,int position);
    double getChannelPos(unsigned int DMchannel, int xyz);

private:
  bool init_default();
  bool init_file();
private:
  std::string m_db_type; // * File * ROOT * MySQL ...
  std::string m_db_path; // file path of data


  int CT[64][21];
  double ChannelPos[3 * 7 * 3 * 16 * 64][3];
  unsigned int ChannelID[3 * 7 * 3 * 16 * 64];
};


#endif
