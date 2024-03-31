#ifndef __READIBDXSECPARAM__H__
#define __READIBDXSECPARAM__H__

# include <stdio.h>
# include <stdlib.h>
# include <yaml.h>
# include <iostream>
# include <string>
# include <map>
# include <TSystem.h>

class ReadIBDXSecParam
{
  public:
    ReadIBDXSecParam();
    ~ReadIBDXSecParam() {;}
    void Initialize();
    void GenerateIBDXSecParamMap();
    double GetIBDXSecParamMap(std::string map_key);

  private:
    FILE *fh;
    std::map<std::string, double> IBDXSecParam;
    std::string m_IBDXSecFile;
    yaml_parser_t parser;
    yaml_token_t token;

};


#endif
