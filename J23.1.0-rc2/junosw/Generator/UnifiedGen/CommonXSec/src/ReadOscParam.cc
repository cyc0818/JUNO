# include "CommonXSec/ReadOscParam.hh"
# include <stdio.h>
#include <stdlib.h>
# include <yaml.h>
# include <iostream>
# include <string>
# include <map>
# include <TSystem.h>


using namespace std;

ReadOscParam::ReadOscParam()
{
  Initialize();
}

void ReadOscParam::Initialize()
{
  if(m_OscFile.empty()){
    if (getenv("JUNOTOP")){
      string s = getenv("JUNOTOP");
      s += "/data/Generator/UnifiedGen/CommonXSec/data/OscParam/OscParamPDG2020.yaml";
      if (!gSystem -> AccessPathName(s.c_str())){
        m_OscFile = s;
      }
    }
    if (getenv("WORKTOP")){
      string s = getenv("WORKTOP");
      s += "/data/Generator/UnifiedGen/CommonXSec/data/OscParam/OscParamPDG2020.yaml";
      if (!gSystem -> AccessPathName(s.c_str())){
        m_OscFile = s;
      }
    }
  }
  fh = fopen(m_OscFile.c_str(), "r");
}



void ReadOscParam::GenerateOscParamMap()
{
  // Initialize parser
  if(!yaml_parser_initialize(&parser))
    fputs("Failed to initialize parser!\n", stderr);
  if(fh == NULL)
    fputs("Failed to open file!\n", stderr);
  // Set input file
  yaml_parser_set_input_file(&parser, fh);
  string map_key;
  double map_value;
  int status;
  // Begin read
  do {
    yaml_parser_scan(&parser, &token);
    switch(token.type)
    {
    // Stream start/end 
    case YAML_STREAM_START_TOKEN:  break;
    case YAML_STREAM_END_TOKEN:    break;
    // Token types (read before actual token) 
    case YAML_KEY_TOKEN:   {              
                            status = 0;
                            break;}
    case YAML_VALUE_TOKEN: {
                            status = 1;
                            break;}
    // Block delimeters
    case YAML_BLOCK_SEQUENCE_START_TOKEN: break;
    case YAML_BLOCK_ENTRY_TOKEN:          break;
    case YAML_BLOCK_END_TOKEN:            break;
    // Data 
    case YAML_BLOCK_MAPPING_START_TOKEN:  break;
    case YAML_SCALAR_TOKEN:  {
                              if (status==0){
                                map_key = (char*)token.data.scalar.value;
                              }
                              else if (status==1){
                                string map_value_string = (char*)token.data.scalar.value;
                                map_value = stod(map_value_string);
                                OscParam.insert(pair<string, double>(map_key,map_value));
                              }
                              break;}
    // Others
    default:
      printf("Got token of type %d\n", token.type);
    }
    if(token.type != YAML_STREAM_END_TOKEN)
      yaml_token_delete(&token);
  } while(token.type != YAML_STREAM_END_TOKEN);
  yaml_token_delete(&token);
}

double ReadOscParam::GetOscParamMap(string map_key)
{
  return OscParam[map_key];
}






