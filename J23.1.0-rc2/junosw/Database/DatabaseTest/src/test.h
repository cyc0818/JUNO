#ifndef TEST
#define TEST

#include "DatabaseSvc/DBITableRow.h"
//#include "DatabaseSvc/DBIResultPtr.h"
//std::vector<test>& operator << (std::vector<test>&, DBIResultPtr&);

class test: public DBITableRow
 {
 public:
 test(){/*std::cout << "constructing test..." << std::endl;*/}
 virtual ~test(){/*std::cout << "destroying test..." << std::endl;*/}

 private:
 long   fid;

 public:
 void Setid(long a){fid = a;}
 long Getid(){return fid;}

 ClassDef(test, 1)
 };

#endif
