#include "Event/OecEvt.h"
#include <iostream>

ClassImp(JM::OecEvt)
namespace JM{
    OecEvt::OecEvt(){
        m_time=TTimeStamp(0,0);
        f_energy=-1;
        f_vertexX=0;
        f_vertexY=0;
        f_vertexZ=0;
        i_tag=0;
        f_muinx.clear() ;
        f_muiny.clear();
        f_muinz.clear();
        f_muoutx.clear();
        f_muouty.clear();
        f_muoutz.clear();
        f_muid.clear();
        n_clusters = 0;
        f_qbf = -1.;
    }

    OecEvt::~OecEvt(){
    }

    string OecEvt::getIDA(){
        int detid = unsigned(i_tag<<2)>>30;
        if (detid==1) return "CD";
        else if (detid==2) return "WP";
        else if (detid==3) return "TT";
        return "";
    }

    bool OecEvt::isMuon(){
        int i_temptag=unsigned(i_tag<<11)>>31;  
        if (i_temptag==1) return true;
        else return false;
    }

    bool OecEvt::isTTMuon(){
        if (!getIDA().compare("TT")) return true;
        else return false;
        //int i_temptag=unsigned(i_tag<<1)>>31;
        //if (i_temptag==1) return true;
        //else return false;
    }

    bool OecEvt::isWPMuon(){
        if (!getIDA().compare("WP")) return true;
        else return false;
        //int i_temptag=unsigned(i_tag<<2)>>31;
        //if (i_temptag==1) return true;
        //else return false;
    }   

    bool OecEvt::isMuonRelated(){
        int i_temptag=unsigned(i_tag<<9)>>31;
        if (i_temptag==1) return true;
        else return false;
    }
}
