#ifndef TABLEROWEXAMPLE
#define TABLEROWEXAMPLE

#include "DatabaseSvc/DBITableRow.h"

class TableRowExample: public DBITableRow
 {
 public:
 TableRowExample():
 fColumn_3(2){std::cout << "constructing TableRowExample..." << std::endl;}
 virtual ~TableRowExample(){std::cout << "destroying TableRowExample..." << std::endl;}

 private:
 char*  fColumn_1;
 double fColumn_2;
 long   fColumn_3;
 float  fColumn_4;

 public:
 void SetColumn_1(char* a){fColumn_1 = a;}
 void SetColumn_2(double b){fColumn_2 = b;}
 void SetColumn_3(long c) {fColumn_3 = c;}
 void SetColumn_4(float d) {fColumn_4 = d;}
 char* GetColumn_1(){return fColumn_1;}
 double GetColumn_2(){return fColumn_2;}
 long GetColumn_3(){return fColumn_3;}
 float GetColumn_4(){return fColumn_4;}

 ClassDef(TableRowExample, 1)
 };

#endif
