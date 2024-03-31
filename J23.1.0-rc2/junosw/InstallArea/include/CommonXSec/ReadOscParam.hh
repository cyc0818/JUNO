#ifndef __READOSCPARAM__H__
#define __READOSCPARAM__H__

# include <stdio.h>
# include <yaml.h>
# include <iostream>
# include <string>
# include <map>
# include <TSystem.h>


class ReadOscParam
{
  public:
    ReadOscParam();
    ~ReadOscParam() {;}
    void Initialize();
    void GenerateOscParamMap();
    double GetOscParamMap(std::string map_key);
    

  private:
    
    FILE *fh;
    std::map<std::string, double> OscParam;
    std::string m_OscFile;
    yaml_parser_t parser;
    yaml_token_t token;

};


#endif
