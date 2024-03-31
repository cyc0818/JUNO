#ifndef SPMSROW_H
#define SPMSROW_H

#include "DatabaseSvc/DBITableRow.h"

class SpmsRow: public DBITableRow
 {
 public:
 SpmsRow(){std::cout << "constructing SpmsRow..." << std::endl;}
 virtual ~SpmsRow(){std::cout << "destroying SpmsRow..." << std::endl;}

 private:
 char*  fid;
 char*  fcreate_by;
 char*  fcreate_date;
 char*  fupdate_by;
 char*  fupdate_date;
 char*  fremarks;
 char*  fdel_flag;
 char*  fversion;
 char*  flightYield;
 char*  fuser;
 char*  fspeedratio;
 long   fstatus;
 char*  ftag;
 char*  fmodifylog;
 char*  frefractiveIndex;
 char*  fabslength;
 char*  fscalength;
 char*  femissionspectrum;
 char*  fheavyabs;
 char*  frefractiveindexpath;
 char*  fabslengthpath;
 char*  fscalengthpath;
 char*  femissionspectrumpath;
 char*  fheavyabspath;

 public:
 void Setid(char* id){fid = id;}
 void Setcreate_by(char* create_by){fcreate_by = create_by;}
 void Setcreate_date(char* create_date) {fcreate_date = create_date;}
 void Setupdate_by(char* update_by) {fupdate_by = update_by;}
 void Setupdate_date(char* update_date){fupdate_date = update_date;}
 void Setremarks(char* remarks){fremarks = remarks;}
 void Setdel_flag(char* del_flag){fdel_flag = del_flag;}
 void Setversion(char* version){fversion = version;}
 void SetlightYield(char* lightYield){flightYield = lightYield;}
 void Setuser(char* user){fuser = user;}
 void Setspeedratio(char* speedratio){fspeedratio = speedratio;}
 void Setstatus(long status){fstatus = status;}
 void Settag(char* tag){ftag = tag;}
 void Setmodifylog(char* modifylog){fmodifylog = modifylog;}
 void SetrefractiveIndex(char* refractiveIndex){frefractiveIndex = refractiveIndex;}
 void Setabslength(char* abslength){fabslength = abslength;}
 void Setscalength(char* scalength){fscalength = scalength;}
 void Setemissionspectrum(char* emissionspectrum){femissionspectrum = emissionspectrum;}
 void Setheavyabs(char* heavyabs){fheavyabs = heavyabs;}
 void Setrefractiveindexpath(char* refractiveindexpath){frefractiveindexpath = refractiveindexpath;}
 void Setabslengthpath(char* abslengthpath){fabslengthpath = abslengthpath;}
 void Setscalengthpath(char* scalengthpath){fscalengthpath = scalengthpath;}
 void Setemissionspectrumpath(char* emissionspectrumpath){femissionspectrumpath = emissionspectrumpath;}
 void Setheavyabspath(char* heavyabspath){fheavyabspath = heavyabspath;}


 char* Getid(){return fid;}
 char* Getcreate_by(){return fcreate_by;}
 char* Getcreate_date() {return fcreate_date;}
 char* Getupdate_by() {return fupdate_by;}
 char* Getupdate_date(){return fupdate_date;}
 char* Getremarks(){return fremarks;}
 char* Getdel_flag(){return fdel_flag;}
 char* Getversion(){return fversion;}
 char* GetlightYield(){return flightYield;}
 char* Getuser(){return fuser;}
 char* Getspeedratio(){return fspeedratio;}
 long  Getstatus(){return fstatus;}
 char* Gettag(){return ftag;}
 char* Getmodifylog(){return fmodifylog;}
 char* GetrefractiveIndex(){return frefractiveIndex;}
 char* Getabslength(){return fabslength;}
 char* Getscalength(){return fscalength;}
 char* Getemissionspectrum(){return femissionspectrum;}
 char* Getheavyabs(){return fheavyabs;}
 char* Getrefractiveindexpath(){return frefractiveindexpath;}
 char* Getabslengthpath(){return fabslengthpath;}
 char* Getscalengthpath(){return fscalengthpath;}
 char* Getemissionspectrumpath(){return femissionspectrumpath;}
 char* Getheavyabspath(){return fheavyabspath;}

 ClassDef(SpmsRow, 1)
 };

#endif
