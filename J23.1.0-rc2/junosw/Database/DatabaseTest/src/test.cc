#include "test.h"
//#include "DatabaseSvc/DBIResultPtr.h"
ClassImp(test);

/*std::vector<test>& operator << (std::vector<test>& vc, DBIResultPtr& ptr)
 {
  test sample;
  std::string TableName = sample.GetTableName();
  std::string query = "select * from offline_db."+ TableName;
  DBIRequest request(query);
  ptr.SetRequest(request);
  ptr.Session();
  int row = ptr.GetMaxRowcount();
  int memnum = sample.GetNumMember();
  for(int i=0; i<row; i++)
  {
   ptr.GetResByRowNum(i);
   for(int j=0; j<memnum; j++)
   {
    std::string membername  = sample.GetMemberTypebyIndex(j);
    std::string membervalue = ptr.GetString(membername);
    //const char* memvalue = membervalue.c_str();
    vc[i].SetMemberValuebyName(membervalue, membername);
   }
  }
 }*/
