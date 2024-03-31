
#include <cstdlib>
#include <map>
#include <vector>

#include "TtFilter.h"

#include "Identifier/Identifier.h"
#include "Identifier/TtID.h"

struct hitStrip {
    int strip;
    int IC;
    float sumPE;
};

typedef std::map<int, std::vector<struct hitStrip> > map_strips ;

std::set<int> * TtFilter(const std::list<JM::CalibTtChannel*> & calibTtHits){

    map_strips * strips = new map_strips();
    std::set<int> * list_CTflag = new std::set<int>();

    for (auto tthit = calibTtHits.begin(); tthit != calibTtHits.end(); ++tthit){
        Identifier channelID = TtID::id((*tthit)->pmtId());

        Int_t WallID  = TtID::wall_id(channelID);
        Int_t PMTID   = TtID::pmt   (channelID);
        Int_t stripID = TtID::strip (channelID);

        Int_t le_PMTID = TtID::lower_pmt_index(PMTID);

        Int_t Wall_lePMT_ID = TtID::getIntID(WallID, le_PMTID, 0);

        Int_t chID_lePMT    = TtID::getIntID(WallID, le_PMTID, stripID);

        bool found_same_strip = false;
        // Check if already found hit on same strip. This can only happen
        // if the PMTs on both sides see a hit for the same strip.
        for(unsigned int kk=0; kk<(*strips)[Wall_lePMT_ID].size() ; kk++){
            if((*strips)[Wall_lePMT_ID][kk].strip == stripID){
                (*strips)[Wall_lePMT_ID][kk].sumPE += (*tthit)->nPE();
                found_same_strip = true;
                break;
            }
        }
        if(!found_same_strip){
            struct hitStrip hit;
            hit.strip = stripID;
            hit.IC    = chID_lePMT;
            hit.sumPE = (*tthit)->nPE();
            (*strips)[Wall_lePMT_ID].push_back(hit);
        }
    }

    // Check for every physical TT panel if there are x-talk hits to be filtered
    for(map_strips::iterator iChidStrips = strips->begin(); iChidStrips != strips->end(); iChidStrips++){
        std::vector<struct hitStrip> & iStrips = iChidStrips->second;
        if(iStrips.size() < 2) continue;

        float MaxPE          = iStrips[0].sumPE;
        int   StripWithMaxPE = iStrips[0].strip;
        for(unsigned int j=1; j<iStrips.size(); j++){
            if(MaxPE < iStrips[j].sumPE){
                MaxPE          = iStrips[j].sumPE;
                StripWithMaxPE = iStrips[j].strip;
            }
        }
        Int_t m=-(StripWithMaxPE+1)%8;

        for(unsigned int j=0; j<iStrips.size(); j++){
            if(StripWithMaxPE == iStrips[j].strip) continue;
            if(iStrips[j].sumPE>3 || iStrips[j].sumPE/MaxPE>0.3) continue;

            int DStrip = abs(StripWithMaxPE-iStrips[j].strip);

            if     (abs(DStrip) == 8 ) list_CTflag->insert(iStrips[j].IC);
            else if(abs(DStrip) == 16) list_CTflag->insert(iStrips[j].IC);
            else if(m!=0 && m!=-1 && abs(DStrip) == 1) list_CTflag->insert(iStrips[j].IC);
            else if(m<-2 && m>-7  && abs(DStrip) == 2) list_CTflag->insert(iStrips[j].IC);
            else if((m==-2 || m==-1) && DStrip==-2) list_CTflag->insert(iStrips[j].IC);
            else if((m==-7 || m==0) && DStrip==2) list_CTflag->insert(iStrips[j].IC);
            else if(m<-1 && m>-8 && (abs(DStrip)==7 || abs(DStrip)==9)) list_CTflag->insert(iStrips[j].IC);
            else if(m<-1 && m>-8 && (abs(DStrip)==15 || abs(DStrip)==17)) list_CTflag->insert(iStrips[j].IC);
            else if(m<-2 && m>-7 && (abs(DStrip)==6 || abs(DStrip)==10)) list_CTflag->insert(iStrips[j].IC);
            else if(m==-1 &&  (DStrip==7 || DStrip==-9 )) list_CTflag->insert(iStrips[j].IC);
            else if(m==-1 &&  (DStrip==15 || DStrip==-17 )) list_CTflag->insert(iStrips[j].IC);
            else if(m==-1 && DStrip==-1) list_CTflag->insert(iStrips[j].IC);
            else if((m==-1 || m==-2) && (DStrip==6 || DStrip==-10)) list_CTflag->insert(iStrips[j].IC);
            else if(m==0 && DStrip==1) list_CTflag->insert(iStrips[j].IC);
            else if(m==0 && (DStrip==-7 || DStrip==9)) list_CTflag->insert(iStrips[j].IC);
            else if(m==0 && (DStrip==-15 || DStrip==17)) list_CTflag->insert(iStrips[j].IC);
            else if((m==0 || m==-7) && (DStrip==-6 || DStrip==10)) list_CTflag->insert(iStrips[j].IC);
        }
    }

    delete strips;

    return list_CTflag;
}

// vim: expandtab shiftwidth=4 softtabstop=4
