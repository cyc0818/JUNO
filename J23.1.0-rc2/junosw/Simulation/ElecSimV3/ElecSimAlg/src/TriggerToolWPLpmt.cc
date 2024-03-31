#include <boost/python.hpp>
#include "Trigger.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "Pulse.h"
#include "ElecSimSvc.h"
#include "TMath.h"

#include <array>
#include <map>
#include <deque>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "TString.h"

#include "Geometry/PMT.h"

using namespace std;
class TriggerToolWPLpmt: public ToolBase, public ITriggerTool {
public:
    TriggerToolWPLpmt(const std::string& name): ToolBase(name),
                                                m_elecsvc(0), is_wppmt_group_initialized(false) {

    declProp("Trigger_FiredPmtNum_WP",m_Trigger_FiredPmtNum=20);// threshold for single group region 
    declProp("Trigger_FiredPmtNum_WP_low",m_Trigger_FiredPmtNum_low=15); //threshold for two adjacent region trigger
    declProp("Trigger_window_WP",m_Trigger_window=200);// ns
    declProp("Trigger_slip_window_WP",m_Trigger_slip_window=25);// ns
    declProp("WP_pmtGroupFile", m_WPpmtGroupFile="GCU_region_WP.txt");  
    }

    ~TriggerToolWPLpmt() {

    }

    float m_Trigger_FiredPmtNum;
    float m_Trigger_FiredPmtNum_low;
    float m_Trigger_window;
    float m_Trigger_slip_window;

    typedef std::array<int,800> IntArray;

    IntArray m_WPpmtGroupId;
    // find the WP PMT group
    std::string m_WPpmtGroupFile;
    IntArray becid;
    IntArray gcuid;
    IntArray elec_channelid;
    IntArray groupid;
    bool is_wppmt_group_initialized;

    bool has_trigger() {
        if (not is_wppmt_group_initialized) {
            std::string line;
            int j = 0; 
            std::string s = getenv("ELECSIMROOT") ;
            s += "/share/";
            s += m_WPpmtGroupFile;
            std::ifstream in;
            in.open(s.c_str());
            while(getline(in, line)) {
                std::istringstream ss(line);
                if (j >= std::tuple_size<IntArray>::value) {
                    LogWarn << "More lines then expected found during initialize the WP PMT Group data. Skip... " << std::endl;
                    break;
                }

                ss >>becid[j] >>gcuid[j]>>elec_channelid[j]>>groupid[j];
                j++;
            }
            in.close();

            is_wppmt_group_initialized = true;
        } 
 
        // check if there is any existing trigger.
        if (triggers.size()!=0) {
            return true;
        }
        // try to find a trigger.
        // note, in trigger tool, we don't modify pulse buffer
        const std::deque<Pulse>& buffer = m_elecsvc->get_WPLPMT_pulse_buffer();
       // const std::deque<Pulse>& buffer = m_elecsvc->get_pulse_buffer();
        // if buffer is empty, we can't find any trigger.
        if (buffer.size()==0) {
            return false;
        }

        // npmt trigger
        TimeStamp beginTime = buffer.front().pulseHitTime;
        TimeStamp backTime = buffer.back().pulseHitTime;
           TimeStamp deltaTime = backTime - beginTime; 
       // LogInfo << "WP Pulse buffer delta time: " << deltaTime.GetSeconds()<< "s"  << std::endl;

        int beginIdx = 0;
        int buffersize = buffer.size();

        float triggerWindow = m_Trigger_window; // ns
        float slipWindow = m_Trigger_slip_window; //
        int firedPmtNum = m_Trigger_FiredPmtNum;
        int firedPmtNum_low = m_Trigger_FiredPmtNum_low;
        int gcu_id;
        int groupfiredN[10]={0};
        bool tag1=false;
        bool tag2=false;
        bool found = false;
        
        pmtfired.clear();


        while(true) {
            if (beginIdx >= buffersize) {
                break;
            }


            TimeStamp endTime = beginTime;
            endTime.Add(triggerWindow*1e-9); // trigger window

            pmtfired.clear();
            
            for (int i = beginIdx; i < buffersize; ++i) {
                const Pulse& p = buffer[i];
                if (PMT::IsWP(p.pmtID)) { // WP-LPMT

                } else {
                    continue;
                }

                // if the pulse is beyond the time window,
                // we will stop
                if (p.pulseHitTime > endTime) {
                    break;
                }

            //     LogInfo << "WP pulse info: "
              //           << p.pmtID << " -> "
                //         << p.pulseHitTime << std::endl;

                ++pmtfired[p.pmtID];
                gcu_id = (p.pmtID-kOFFSET_WP_PMT)/3; 
                groupfiredN[groupid[gcu_id]-1]++;
            }

            //LogInfo << "WP Event fired pmt numbers: " << pmtfired.size() << std::endl;
            for(int i=0;i<10;i++){
              if(groupfiredN[i]>firedPmtNum) tag1 = true; 
            }
            if(groupfiredN[0]>firedPmtNum_low&&groupfiredN[1]>firedPmtNum_low) tag2=true;
            if(groupfiredN[2]>firedPmtNum_low&&groupfiredN[3]>firedPmtNum_low) tag2=true;
            if(groupfiredN[4]>firedPmtNum_low&&groupfiredN[5]>firedPmtNum_low) tag2=true;
            if(groupfiredN[6]>firedPmtNum_low&&groupfiredN[7]>firedPmtNum_low) tag2=true;
            if(groupfiredN[8]>firedPmtNum_low&&groupfiredN[9]>firedPmtNum_low) tag2=true;
         
            if(groupfiredN[0]>firedPmtNum_low&&groupfiredN[2]>firedPmtNum_low) tag2=true;
            if(groupfiredN[2]>firedPmtNum_low&&groupfiredN[4]>firedPmtNum_low) tag2=true;
            if(groupfiredN[4]>firedPmtNum_low&&groupfiredN[6]>firedPmtNum_low) tag2=true;
            if(groupfiredN[6]>firedPmtNum_low&&groupfiredN[8]>firedPmtNum_low) tag2=true;
            if(groupfiredN[0]>firedPmtNum_low&&groupfiredN[8]>firedPmtNum_low) tag2=true;
           
            if(groupfiredN[1]>firedPmtNum_low&&groupfiredN[3]>firedPmtNum_low) tag2=true;
            if(groupfiredN[3]>firedPmtNum_low&&groupfiredN[5]>firedPmtNum_low) tag2=true;
            if(groupfiredN[5]>firedPmtNum_low&&groupfiredN[7]>firedPmtNum_low) tag2=true;
            if(groupfiredN[7]>firedPmtNum_low&&groupfiredN[9]>firedPmtNum_low) tag2=true;
            if(groupfiredN[1]>firedPmtNum_low&&groupfiredN[9]>firedPmtNum_low) tag2=true;
            
            if (tag1==true||tag2==true) {
                // found
                LogInfo << "fired pmt numbers: " << pmtfired.size() << std::endl;
                found = true;
                break;
            }

            // slip trigger window
            endTime = beginTime;
            endTime.Add(slipWindow*1e-9);
            for(int i=0;i<10;i++) {
             groupfiredN[i]=0;
            }

            while (++beginIdx < buffersize) {
                const Pulse& p = buffer[beginIdx];
               //LogInfo << "fired pmt id: " << p.pmtID <<";beginIdx ="<<beginIdx<< std::endl;
                if (PMT::IsWP(p.pmtID)) { // LPMT

                } else {
                    continue;
                }

                // found the next trigger window
                if (p.pulseHitTime > endTime) {
                    beginTime = p.pulseHitTime;
                    break;
                }

            }
//            LogInfo << "slip window: beginIdx -> "
//                    << beginIdx
//                    << std::endl;
        }

        if (not found) {
            return false;
        }

        // if found, create an trigger object.
        Trigger t;
        t.type = kTriggerWP;
        t.time = beginTime;
        triggers.push_back(t);
       // std::cout<<"-------------------find a Wp trigger!!---"<<std::endl;
        return true;
    }

    Trigger get_trigger() {
        Trigger t = triggers.front();
        triggers.pop_front();
        return t;
    }

    bool init() {

        SniperPtr<ElecSimSvc> svc(*getRoot(), "ElecSimSvc");
        if (svc.invalid()) {
            LogError << "can't find service ElecSimSvc" << std::endl;
            return false;
        }
        m_elecsvc = svc.data();

        LogInfo << "init trigger tool (LPMT). " << std::endl;

        return true;
    }

    std::deque<Trigger> triggers;

    ElecSimSvc* m_elecsvc;

    std::map<int, int> pmtfired; // key: pmtid, value: nhits

};

// Please declare this tool, otherwise we can't locate it.
DECLARE_TOOL(TriggerToolWPLpmt);
