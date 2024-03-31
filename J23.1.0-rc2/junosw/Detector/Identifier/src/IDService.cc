//
//  Author: Zhengyun You  2013-11-20   
//  wuchx 2022-11-20
//

#include "Identifier/JunoDetectorID.h"
#include "Identifier/CdID.h"
#include "Identifier/IDService.h"
#include "Identifier/Identifier.h"

//root
#include "TSystem.h"

//c++
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <map>
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


#include "TString.h"
#include "stdlib.h"

IDService* IDService::m_idServ = new (std::nothrow) IDService;


IDService::IDService(void) 
{

}

IDService::~IDService(void)
{
}

bool IDService::init()
{


    if (!IDS_mapinit ){
        initMap();
        IDS_mapinit = true;
        std::cout << "have initialized the IDService!" << std::endl;    
    }
    return true;
}
IDService* IDService::getIdServ()
{
    return m_idServ;
}

bool IDService::initMap()
{
    std::cout << "IDMapping Service is initializing!" << std::endl;
    // if Path is not set, using the default one.
    std::string pathCDL = CdLCsvFileName;
    std::string pathCDS = CdSCsvFileName;

    if (pathCDL.empty()) {
        if (getenv("JUNOTOP")) {
            std::string sL = getenv("JUNOTOP");
            sL += "/data/Detector/Identifier/pmt_20230720_CDLPMT.csv";
            if (!gSystem->AccessPathName(sL.c_str())){
                pathCDL = sL;
            }
        }
        if (getenv("WORKTOP")) {
            std::string sL = getenv("WORKTOP");
            sL += "/data/Detector/Identifier/pmt_20230720_CDLPMT.csv";
            if (!gSystem->AccessPathName(sL.c_str())){
                pathCDL = sL;
            }
        }
    }
    if (pathCDS.empty()) {
        if (getenv("JUNOTOP")) {
            std::string sS = getenv("JUNOTOP");
            sS += "/data/Detector/Identifier/pmt_20230326_CDSPMT.csv";
            if (!gSystem->AccessPathName(sS.c_str())) {
                pathCDS = sS;
            }
        }
        if (getenv("WORKTOP")) {
            std::string sS = getenv("WORKTOP");
            sS += "/data/Detector/Identifier/pmt_20230326_CDSPMT.csv";
            if (!gSystem->AccessPathName(sS.c_str())) {
                pathCDS = sS;
            }
        }
    }
    setCdLCsvFileName(pathCDL);
    setCdSCsvFileName(pathCDS);

    readCsvAndMap(CdLCsvFileName);
    readCsvAndMap(CdSCsvFileName);
    return true;
}



bool IDService::readCsvAndMap(const TString& inCsvFileName){

    namespace fs = boost::filesystem;
   
    fs::path fullp(inCsvFileName);
    std::ifstream ifs(fullp.string().c_str());

    while (ifs.good())
    {
        // vecCopyNoToInf.push_back (each_row_string);
        std::string each_row_string;
        std::getline(ifs, each_row_string);
        each_row_string.erase( std::find( each_row_string.begin(), each_row_string.end(), '#' ), each_row_string.end() );// Parameter/utils.hh
        
        if (each_row_string.empty()) { continue;}

        std::stringstream row_string;
        row_string << each_row_string;
        if (row_string.fail()) { continue; }

        std::string CopyNo;
        std::string LargeOrSmall;
        std::string NorthOrSouth;
        std::string CircleNo;
        std::string PositionNo;
        std::string UpOrDown;
        std::string PmtType;
        std::string GCU;
        std::string Channel;

        std::getline(row_string, CopyNo, ',');  
        std::getline(row_string, LargeOrSmall, ',');   
        std::getline(row_string, NorthOrSouth, ',');  
        std::getline(row_string, CircleNo, ',');  
        std::getline(row_string, PositionNo, ',');  
        std::getline(row_string, UpOrDown, ',');  
        std::getline(row_string, PmtType, ',');  
        std::getline(row_string, GCU, ','); 
        std::getline(row_string, Channel, ','); 

        Identifier id =CdID::id(atoi(LargeOrSmall.c_str()), atoi(NorthOrSouth.c_str()), atoi(CircleNo.c_str()), atoi(PositionNo.c_str()), atoi(UpOrDown.c_str()), atoi(PmtType.c_str()));    
        CopyNo2Id[atoi(CopyNo.c_str())] = id;

        Id2CopyNo[id] = atoi(CopyNo.c_str());///20000  to 17612
        //IdToCDNum[id] = CDNum;

        int GCUC = atoi(GCU.c_str())*10 + atoi(Channel.c_str());
        Id2GCUC[id] = GCUC;
        GCUPlusC2Id[GCUC] = id;

        // if (!(each_row_string.empty())) {

        // }
    }

    return true;
}


Identifier IDService::copyNo2Id(const int& CopyNo)
{
    auto it = CopyNo2Id.find(CopyNo);
    if ( it == CopyNo2Id.end() ) {
        std::cerr << "CopyNo " << CopyNo <<  "'s id does not exist " << std::endl;
        return Identifier{};
    }
    return it-> second;  //CopyNo2Id[CopyNo]
}

int IDService::id2CopyNo(const Identifier& id)
{
    auto it = Id2CopyNo.find(id);
    if ( it == Id2CopyNo.end() ) {
        std::cerr << "id " << id <<  "'s Copy number does not exist " << std::endl;
        return Identifier::INVALID_COPYNO;
    }
    return it-> second;  //Id2CopyNo[id]
}

// std::string IDService::getIdCDNum(Identifier Pmt_Id)
// {
//     std::map<Identifier, std::string>::iterator it = IdToCDNum.find(Pmt_Id);
//     if ( it == IdToCDNum.end() ) {
//         std::cerr << "id " << Pmt_Id <<  "'s CDNumber does not exist " << std::endl;
//         //return 0;
//     }
//     return IdToCDNum[Pmt_Id];
// }

int IDService::id2GCU(const Identifier& id)
{
    auto it = Id2GCUC.find(id);
    if ( it == Id2GCUC.end() ) {
        std::cerr << "id " << id <<  "'s GCU does not exist now" << std::endl;
        return -1;
    }

    if(it-> second ==0){
        std::cerr << "id " << id <<  "'s GCU is blank one now" << std::endl;
        return -1;
    }

    return (it-> second)/10; //Id2GCU[id]
}

int IDService::id2Channel(const Identifier& id)
{
    auto it = Id2GCUC.find(id);

    if ( it == Id2GCUC.end() ) {
        std::cerr << "id " << id <<  "'s channel does not exist now " << std::endl;
        return -1;
    }

    if(it-> second ==0){
        std::cerr << "id " << id <<  "'s channel is blank one now" << std::endl;
        return -1;
    }

    return (it-> second)%10; //Id2Chnnel[id]
}


Identifier IDService::GCUC2Id(const int& GCU, const int& Channel)
{
    if(GCU ==0){
        std::cerr <<  "GCU is blank one now" << std::endl;
        return Identifier{};
    }
    if(Channel ==0){
        std::cerr << "Channel is blank one now" << std::endl;
        return Identifier{};
    }
    int tem_GCUC = GCU*10 + Channel;
    auto it = GCUPlusC2Id.find(tem_GCUC);
    if ( it == GCUPlusC2Id.end() ) {
        std::cerr << "GCU="<< GCU << ", channel="<< Channel  <<  "'s Pmt does not exist " << std::endl;
        return Identifier{};
    }
    return it-> second; //GCUC2Id[GCUC]
}






