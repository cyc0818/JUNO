#ifndef OPERATOR_TEST
#define OPERATOR_TEST

#include "DatabaseSvc/DBIResultPtr.h"
#include "test.h"

test& operator >> (test& vc, DBIResultPtr& ptr)
 {
  std::string TableName = vc.GetTableName();
  std::string fircolumn = vc.GetMemberNamebyIndex(0).erase(0,1);
  std::string Query = "insert into offline_db." + TableName + " (" + fircolumn;
  int memnum = vc.GetNumMember();
  for(int i=1; i<memnum-1;i++)
  Query = Query + ", " + vc.GetMemberNamebyIndex(i).erase(0,1);
  std::string firvalue = vc.GetMemberValuebyIndex(0);
  Query = Query + ") values (" + firvalue;
  for(int j=1; j<memnum-1;j++)
  Query = Query + ", " + vc.GetMemberValuebyIndex(j);
  Query = Query + ");";
  //std::cout << Query << std::endl;
  RequestType tp = query;
  DBIRequest request(Query,tp);
  ptr.SetRequest(request);
  ptr.Session();
  //std::cout << "****the end of >>******" << std::endl;
  return vc;
 }

std::vector<test>& operator << (std::vector<test>& vc, DBIResultPtr& ptr)
 {
  if(!vc.empty())
  {
   std::vector<test> temp;
   vc.swap(temp);
  }
  test sample;
  std::string TableName = sample.GetTableName();
  std::string query = "select * from offline_db."+ TableName;
  DBIRequest request(query);
  ptr.SetRequest(request);
  ptr.Session();
  int row = ptr.GetMaxRowcount();
  //std::cout << "*****rownumbermax=" << row << "**********" << std::endl;
  int memnum = sample.GetNumMember();
  for(int i=0; i<row; i++)
  {
   ptr.GetResByRowNum(i);
   vc.push_back(sample);
   //std::cout << "*****result has been stored*******" << std::endl;
   for(int j=0; j<memnum-1; j++)
   {
    std::string membername  = sample.GetMemberNamebyIndex(j);//delete the first letter f
    membername.erase(0,1);
    //std::cout << "column=" << membername << std::endl;
    std::string membervalue = ptr.GetString(membername);
    membername = "f" + membername;
    //std::cout << "classmembername=" << membername << std::endl;
    //std::cout << "classmembervalue=" << membervalue << std::endl;
    vc[i].SetMemberValuebyName(membervalue, membername);
    //std::cout << "it is the " << i+1 << " times after inserting" << std::endl;
   }
  }
 return vc;
 }

#endif
