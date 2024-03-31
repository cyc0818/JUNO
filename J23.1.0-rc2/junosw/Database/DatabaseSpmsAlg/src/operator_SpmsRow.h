#ifndef OPERATOR_SPMSROW_H
#define OPERATOR_SPMSROW_H

#include "DatabaseSvc/DBIResultPtr.h"
#include "DatabaseSvc/DBI.h"
#include "SpmsRow.h"
#include <sstream>

DBI::DBIRollback& operator >> (DBI::DBIRollback& vc, DBIResultPtr& ptr)
{
 if(ptr.Getifvalidrollback())
 {
  std::string rolldatestr = vc.GetTimStamp().AsString("s");
  std::string Query = "select * from offline_db.SpmsRowValid";
  Query = Query + " where `TimeStart` <= \'" + rolldatestr + "\' and "
        + "`TimeStop` >= \'" + rolldatestr + "\' order by `rollrec` desc, "
        + "`update_time` desc, 'TimeStop' desc";

  DBIRequest request(Query);
  ptr.SetRequest(request);
  ptr.Session();
  ptr.GetResByRowNum(0);
  std::string idvalid = ptr.GetString("id");
  std::string maxrollrec = ptr.GetString("rollrec");
  int maxrollrecint;
  std::stringstream temstream;
  temstream << maxrollrec;
  temstream >> maxrollrecint;
  maxrollrecint++;
  temstream.clear();
  temstream << maxrollrecint;
  std::string maxrollrecstr;
  temstream >> maxrollrecstr;
  std::string querynew = "update offline_db.SpmsRowValid set `rollrec` = \'"
                       + maxrollrecstr + "\' where `id` = \'" + idvalid + "\'";

  DBIRequest requestnew(querynew);
  ptr.SetRequest(requestnew);
  ptr.Session();
 }
 return vc;
}

SpmsRow& operator >> (SpmsRow& vc, DBIResultPtr& ptr)
 {
  if(ptr.Getifvalidwrite())
  {
   std::string TableNamevalid = vc.GetTableName() + "Valid";
   std::string queryvalid = "insert into offline_db." + TableNamevalid
                          + " (`id`, `TimeStart`, `TimeStop`, `update_time`, `rollrec`) VALUES (";
   std::string strid = vc.Getid();
   std::string strtmstart = ptr.GetContextRange().GetTimStart().AsString("s");
   std::string strtmstop = ptr.GetContextRange().GetTimStop().AsString("s");
   queryvalid = queryvalid + "\'" + strid + "\', \'" + strtmstart
              + "\', \'" + strtmstop + "\', \'2018-07-13 00:00:00\', "
              + "\'1\')";

   RequestType valid = query;
   DBIRequest requestvalid(queryvalid,valid);
   ptr.SetRequest(requestvalid);
   ptr.Session();
  }
  std::string TableName = vc.GetTableName();
  std::string fircolumn = vc.GetMemberNamebyIndex(0).erase(0,1);
  std::string Query = "insert into offline_db." + TableName + " (" + fircolumn;
  int memnum = vc.GetNumMember();
  for(int i=1; i<memnum-1;i++)
  Query = Query + ", " + vc.GetMemberNamebyIndex(i).erase(0,1);
  std::string firvalue = vc.GetMemberValuebyIndex(0);
  Query = Query + ") values (\"" + firvalue + "\"";
  for(int j=1; j<memnum-1;j++)
  Query = Query + ", \"" + vc.GetMemberValuebyIndex(j) + "\"";
  Query = Query + ");";
  //std::cout << Query << std::endl;
  RequestType tp = query;
  DBIRequest request(Query,tp);
  ptr.SetRequest(request);
  ptr.Session();
  //std::cout << "****the end of >>******" << std::endl;
  return vc;
 }

std::vector<SpmsRow>& operator << (std::vector<SpmsRow>& vc, DBIResultPtr& ptr)
 {
  if(!vc.empty())
  {
   std::vector<SpmsRow> temp;
   vc.swap(temp);
  }
  SpmsRow sample;
  std::string TableName = sample.GetTableName();
  std::string query = "select * from offline_db."+ TableName;
  //std::cout << query << std::endl;
  DBIRequest request(query);
  ptr.SetRequest(request);
  ptr.Session();
  int row = ptr.GetMaxRowcount();
  std::cout << "*****rownumbermax=" << row << "**********" << std::endl;
  int memnum = sample.GetNumMember();
  for(int i=0; i<=row; i++)
  {
   ptr.GetResByRowNum(i);
   vc.push_back(sample);
   std::cout << "*****result has been stored*******" << std::endl;
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
 //std::cout << "*****this is before return*********" << std::endl;
 return vc;
 }

 SpmsRow& operator << (SpmsRow& vc, DBIResultPtr& ptr)
 {
  if(ptr.Getifvalidread())
  {
   std::string timstm = ptr.GetContext().GetTimeStamp().AsString("s");
   std::string TableName = vc.GetTableName();
   TableName = TableName + "Valid";
   std::string query = "select `id` from offline_db." + TableName + " where `TimeStart` <= \'"
                     + timstm + "\' and `TimeStop` >= \'" + timstm + "\' order by "
                     + "`rollrec` desc, `TimeStop` desc, `update_time` desc";

   DBIRequest request(query);
   ptr.SetRequest(request);
   ptr.Session();
   ptr.GetResByRowNum(0);
   std::string idvalid = ptr.GetString("id");

   std::string TableNamereal = vc.GetTableName();
   std::string queryreal = "select * from offline_db." + TableNamereal
                         + " where `id` = \'" + idvalid + "\'";
   std::cout << queryreal << std::endl;
   int memnum = vc.GetNumMember();
   DBIRequest requestreal(queryreal);
   ptr.SetRequest(requestreal);
   ptr.Session();
   ptr.GetResByRowNum(0);

   for(int j=0; j<memnum-1; j++)
   {
    std::string membername  = vc.GetMemberNamebyIndex(j);//delete the first letter f
    membername.erase(0,1);
    std::string membervalue = ptr.GetString(membername);
    membername = "f" + membername;
    vc.SetMemberValuebyName(membervalue, membername);
  }

  return vc;
 }
}
#endif
