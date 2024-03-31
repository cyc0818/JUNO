#ifndef ID_SERVICE_H
#define ID_SERVICE_H


//

//
//  Author: Zhengyun You  2013-11-20
//  wuchx 2022-11-20
//

#include "Identifier/JunoDetectorID.h"
#include "Identifier/CdID.h"
#include "Identifier/Identifier.h"

//root
#include "TSystem.h"

// #include "G4UnitsTable.hh"
// #include "G4SystemOfUnits.hh"
// #include "G4Types.hh"

//c++
#include <iostream>
#include <list>
#include <cassert>
#include <stdio.h>
#include <assert.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_io.hpp"


class IDService  
{
    public:    

        static IDService *getIdServ();

        //initial
        bool init();
        bool initMap();

        void setCdLCsvFileName(std::string name) { CdLCsvFileName = name; }
        void setCdSCsvFileName(std::string name) { CdSCsvFileName = name; }
        void setWpCsvFileName(std::string name) { WpCsvFileName = name; }
        void setTtCsvFileName(std::string name) { TtCsvFileName = name; }


        //work functions
        Identifier copyNo2Id(const int& CopyNo);
        int id2CopyNo(const Identifier& id);
        Identifier GCUC2Id(const int& GCU, const int& Channel);
        int id2GCU(const Identifier& id);
        int id2Channel(const Identifier& id);
        // Identifier getGcucId(int GCU, int Channel);

        //

    protected:
 

    private:
        IDService();
        ~IDService(void);

        IDService (const IDService &idServ);
        static IDService *m_idServ;


        //static std::vector<std::string> vecCopyNoToInf;

        std::unordered_map<int, Identifier> CopyNo2Id;
        std::unordered_map<Identifier, int> Id2CopyNo;
        std::unordered_map<int, Identifier> GCUPlusC2Id;
        std::unordered_map<Identifier, int> Id2GCUC;

        //  std::map<Identifier, std::string> IdToCDNum;
        // static std::map<std::string, std::string> CDNumToGCU;
        // static std::map<Identifier, std::string> IDToCDNum;

        bool IDS_mapinit = false;
        bool readCsvAndMap(const TString& inCsvFileName);//const string & inCvsFilename
        //bool mapCdInf(int PMT_ID);
        //bool mapCd(int initnum, int finnum);

        std::string IDMapping_DirPath;
        std::string CdLCsvFileName;
        std::string CdSCsvFileName;
        std::string WpCsvFileName;
        std::string TtCsvFileName;

};

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>
    

#endif // ID_SERVICE_H
