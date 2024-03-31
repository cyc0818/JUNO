#ifndef SpmtElecConfigSvc_h
#define SpmtElecConfigSvc_h

#include "SniperKernel/SvcBase.h"

#include <string>
#include <map>

class SpmtElecConfigSvc: public SvcBase {
 public:
    SpmtElecConfigSvc(const std::string& name);
    ~SpmtElecConfigSvc();

    bool initialize();
    bool finalize();

    bool load(std::string path);

    int           getInt(std::string name);
    double        getDouble(std::string name);
    std::string   getString(std::string name);
    int           getChannel(int);
    int           getPMT(int);
  

  private:

    std::string m_config_file;

    std::map<std::string, int> intParams;
    std::map<std::string, double> doubleParams;
    std::map<std::string, std::string> stringParams;
    std::map<int, int> ChannelParams;
    std::map<int, int> PMTParams;
};

#endif
