#include "time.h"
#include "DatabaseSvcAlg.h"
#include "TableRowExample.h"
//#include "test.h"
//#include "operator_test.h"
#include "testIncludes.h"
#include "SniperKernel/AlgFactory.h"
#include "DatabaseSvc/DatabaseSvc.h"
#include "DatabaseSvc/DBIResultPtr.h"
#include "DatabaseSvc/SqlString.h"

DECLARE_ALGORITHM(DatabaseSvcAlg);

DatabaseSvcAlg::DatabaseSvcAlg(const std::string& name)
     :AlgBase(name)
{}

DatabaseSvcAlg::~DatabaseSvcAlg()
{}

bool DatabaseSvcAlg::initialize()
{
 return true;
}

bool DatabaseSvcAlg::execute()
{
 SniperPtr<DatabaseSvc> dbsvc(getScope(), "DatabaseSvc");
 if(!dbsvc.valid())
 {
 LogError << "Failed to get DatabaseSvc instance!" << std::endl;
 return false;
 }

 //this stuff is used to test lookup function
 /*std::vector<std::string> column;
 column.push_back("Name");
 column.push_back("LocalName");
 column.push_back("Capital");
 std::string table = "world.Country";

 DBIRequest request(column, table, lookup);*/

 //this stuff is used to test update function
 /*DBIRequest request("name='rp',seqno='15'",
                 "world.pet",
                 "",
                 "",
                 "");*/

 //this one is to test lookupquery function
 //DBIRequest request("select * from offline_db.test");

 //DBIResultPtr dbptr("DatabaseSvc", request);
 DBIResultPtr dbptr("DatabaseSvc");
 std::vector<test> vc;
 /*vc << dbptr;
 for(unsigned i = 0; i < vc.size(); i++)
 {
  std::cout << vc[i].Getid() << std::endl;
 }*/
 /*test ins;
 ins.Setid(10);

 time_t start_time=time(NULL);
 for (int i=0; i<100000; i++)
 ins >> dbptr;
 time_t end_time=time(NULL);
 std::cout<< "Running time is: "
          <<(end_time-start_time)
          <<"s"<<std::endl;*/
 //std::cout << "******this one should be out*******" << std::endl;
 time_t start_time=time(NULL);
 vc << dbptr;
 time_t end_time=time(NULL);
 std::cout<< "Running time is: "
          <<(end_time-start_time)
          <<"s"<<std::endl;
 std::cout << "first" << vc[0].Getid() << std::endl;
 /*for(unsigned i = 0; i < vc.size(); i++)
 {
  std::cout << vc[i].Getid() << std::endl;
 }*/
 //dbptr.Session();
 //dbsvc.Session(request);
 /*int row = dbptr.GetMaxRowcount();
 for(int i = row; i >= 0; i--)
 {
 dbptr.GetResByRowNum(i);
 std::cout << "id= " << dbptr.GetInt("id") << std::endl;
 }*/
 /*test exa;
 std::cout << exa.GetTableName() << std::endl;
 int n = exa.GetNumMember();
 std::cout << "n=" << n << std::endl;
 for(int i=0; i< n-1; i++)
 {
  std::cout << "type: " << exa.GetMemberTypebyIndex(i) << std::endl;
  std::cout << "name: " << exa.GetMemberNamebyIndex(i) << std::endl;
 }
 //std::cout << exa.GetMemberValuebyName("fColumn_3") << std::endl;
 std::string v = "4";
 std::string b = "fid";
 exa.SetMemberValuebyName(v, b);
 std::cout << exa.GetMemberValuebyName("fid") << std::endl;*/
 return true;
}

bool DatabaseSvcAlg::finalize()
{
 LogInfo << " finalized successfully" << std::endl;

 return true;
}
