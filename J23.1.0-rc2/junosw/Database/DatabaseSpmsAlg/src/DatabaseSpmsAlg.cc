#include "time.h"
#include "DatabaseSpmsAlg.h"
#include "SpmsRowIncludes.h"
#include "SniperKernel/AlgFactory.h"
#include "DatabaseSvc/DatabaseSvc.h"
#include "DatabaseSvc/DBIResultPtr.h"
#include "DatabaseSvc/SqlString.h"
#include "DatabaseSvc/DBI.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/binary_from_base64.hpp"
#include "boost/archive/iterators/transform_width.hpp"

DECLARE_ALGORITHM(DatabaseSpmsAlg);


bool Base64Encode(const std::string& input, std::string* output) {

  typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8> > Base64EncodeIterator;

  std::stringstream result;

  copy(Base64EncodeIterator(input.begin()) , Base64EncodeIterator(input.end()), std::ostream_iterator<char>(result));

  size_t equal_count = (3 - input.length() % 3) % 3;

  for (size_t i = 0; i < equal_count; i++) {

    result.put('=');

  }

  *output = result.str();

  return output->empty() == false;

}

bool Base64Decode(const std::string& input, std::string* output) {

  typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<std::string::const_iterator>, 8, 6> Base64DecodeIterator;

  std::stringstream result;

  try {

    copy(Base64DecodeIterator(input.begin()) , Base64DecodeIterator(input.end()), std::ostream_iterator<char>(result));

  } catch(...) {

    return false;

  }

  *output = result.str();

  return output->empty() == false;

}

DatabaseSpmsAlg::DatabaseSpmsAlg(const std::string& name)
     :AlgBase(name)
{
 declProp("id", id);
 declProp("Mode", Mode);
}

DatabaseSpmsAlg::~DatabaseSpmsAlg()
{
 LogInfo << "Destroying alg" << std::endl;
}

bool DatabaseSpmsAlg::initialize()
{
 return true;
}

bool DatabaseSpmsAlg::execute()
{
 SniperPtr<DatabaseSvc> dbsvc(getScope(), "DatabaseSvc");
 if(!dbsvc.valid())
 {
 LogError << "Failed to get DatabaseSvc instance!" << std::endl;
 return false;
 }

 //this one is to test lookupquery function
 //DBIRequest request("select * from offline_db.test");

 //DBIResultPtr dbptr("DatabaseSvc", request);
 //DBIResultPtr dbptr("DatabaseSvc");
 //std::vector<test> vc;
 /*vc << dbptr;
 for(unsigned i = 0; i < vc.size(); i++)
 {
  std::cout << vc[i].Getid() << std::endl;
 }*/

 //read input file

 std::string forread = "read";
 std::string forwrite = "write";
 std::string forrollback = "rollback";
 if(!strcmp(Mode.c_str(), forwrite.c_str()))
 {
  TimeStamp tmstart(2018, 7, 22, 0, 0, 0, 0, 0, 0);
  TimeStamp tmstop(2018, 7, 23, 0, 0, 0, 0, 0, 0);
  DBI::ContextRange contra(tmstart, tmstop);
  DBIResultPtr dbptr("DatabaseSvc", contra);
  ifstream myfile("/afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC6/Release/J17v1r1/data/Simulation/DetSim/Material/LS/ABSLENGTH");
 if (!myfile.is_open())
    {
        std::cout << "error open" << std::endl;
    }
 std::string temp;
 std::stringstream tempstream;
 std::string tempstore;
 char* data;
 char* dataid;
    while(getline(myfile,temp))
    {
        tempstream << temp;
        tempstream << std::endl;
    }
 myfile.close();
 std::string showstr, showstr2;
 tempstore = tempstream.str();
 Base64Encode(tempstore, &showstr);
 data = const_cast<char*>(showstr.c_str());
 dataid = const_cast<char*>(id.c_str());
 SpmsRow ins;
 ins.Setid(dataid);
 ins.Setcreate_by("huangwh");
 ins.Setcreate_date("2018-05-30");
 ins.Setupdate_by("huangwh");
 ins.Setupdate_date("2018-05-30");
 ins.Setremarks("remarks");
 ins.Setdel_flag("del_flag");
 ins.Setversion("version");
 ins.SetlightYield("lightYield");
 ins.Setuser("user");
 ins.Setspeedratio("speedratio");
 ins.Setstatus(1);
 ins.Settag("tag");
 ins.Setmodifylog("modifylog");
 ins.SetrefractiveIndex("refractiveIndex");
 ins.Setabslength(data);
 ins.Setscalength("scalength");
 ins.Setemissionspectrum("emissionspectrum");
 ins.Setheavyabs("heavyabs");
 ins.Setrefractiveindexpath("refractiveindexpath");
 ins.Setabslengthpath("abslengthpath");
 ins.Setscalengthpath("scalengthpath");
 ins.Setemissionspectrumpath("emissionspectrumpath");
 ins.Setheavyabspath("heavyabspath");

 ins >> dbptr;
 }

 if(!strcmp(Mode.c_str(), forread.c_str()))
 {

  TimeStamp stm(2018, 7, 20, 12, 0, 0, 0, 0, 0);
  DBI::Context cont_st(stm);
  DBIResultPtr dbptr("DatabaseSvc", cont_st);
  SpmsRow vc;
  vc << dbptr;
  const char* tempid = vc.Getid();
  const char* tempstd = "13";
  if(!strcmp(tempid, tempstd))
  {
   std::cout << "************" << std::endl;
   std::string tempstr, tempout;
   tempstr = vc.Getabslength();
   Base64Decode(tempstr, &tempout);
   std::cout << tempout << std::endl;
   ofstream outfile("./out.txt", std::ios::app);
   outfile << tempout;
   outfile.close();
  }
 }
 /*std::vector<SpmsRow> vc;
 vc << dbptr;
 //std::cout << vc.size() << std::endl;
 for(unsigned i = 0; i < vc.size(); i++)
 {
  const char* tempid = vc[i].Getid();
  const char* tempstd = "13";
  if(!strcmp(tempid, tempstd))
  {
   std::string tempstr, tempout;
   tempstr = vc[i].Getabslength();
   Base64Decode(tempstr, &tempout);
   std::cout << tempout << std::endl;
   ofstream outfile("./out.txt", std::ios::app);
   outfile << tempout;
   outfile.close();
   TimeStamp stm(2018, 07, 19, 19, 40, 0, 0, 0, 0);
   std::string testtim = stm.AsString("s");
   std::cout << testtim << std::endl;
  }
 }*/
 if(!strcmp(Mode.c_str(), forrollback.c_str()))
 {
  TimeStamp stm(2018, 7, 20, 12, 0, 0, 0, 0, 0);
  DBI::DBIRollback rback(stm);
  DBIResultPtr dbptr("DatabaseSvc", rback);
  rback >> dbptr;
 }
 return true;
}
bool DatabaseSpmsAlg::finalize()
{
 LogInfo << " finalized successfully" << std::endl;

 return true;
}
