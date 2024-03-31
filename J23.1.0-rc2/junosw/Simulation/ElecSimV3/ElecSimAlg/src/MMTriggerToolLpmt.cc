#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "Trigger.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "Pulse.h"
#include "ElecSimSvc.h"
#include "TMath.h"

#include <map>
#include <deque>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "Geometry/PMT.h"

// Feiyang Zhang 2021.5.22

using namespace std;
double PI = 3.141592654;
int nhitsArr[12][12][12]; // binning for 12*12*12 because time resolution in FPGA is 16 ns, and the default time window is 192 ns. For PMT location, there are 12*12 = 144 BECs 
	
double binexp = 1.58508;  // expected number dark hits per bin assume 30 kHz/LPMT. (binning: 4*4*4)  30*1000*192*1e-9*17612/4/4/4 = 1.58508
class MMTriggerToolLpmt: public ToolBase, public ITriggerTool {
public:
    MMTriggerToolLpmt(const std::string& name): ToolBase(name),
                                              m_elecsvc(0) {

    declProp("Trigger_FiredPmtNum",m_Trigger_FiredPmtNum=300);// 
    declProp("Trigger_window",m_Trigger_window=192);// ns
    declProp("Trigger_slip_window",m_Trigger_slip_window=12);// ns
		declProp("darkRateScale", m_darkRateScale=1.);
    }

    ~MMTriggerToolLpmt() {

    }

    float m_Trigger_FiredPmtNum;
    float m_Trigger_window;
    float m_Trigger_slip_window;
		float m_darkRateScale;
   // std::deque<Pulse> buffer;
    bool cmp(Pulse x,Pulse y)
    {
      return (x.pulseHitTime)<(y.pulseHitTime);
    }  

		bool MM_Trigger(){
		
		  double sum = 0;
			int nhits =0;
			
			for(int i=0;i<4;i++){
				for(int j=0;j<4;j++){
					for(int k=0;k<4;k++){
						nhits = 0;
						for(int ii=0;ii<3;ii++){
							for(int jj=0;jj<3;jj++){
								for(int kk=0;kk<3;kk++){
									nhits += nhitsArr[i*3+ii][j*3+jj][k*3+kk];
								}
							}
						}
						sum += -log(TMath::Poisson(nhits,binexp));
					}
				}
			}
			
			//sum *= -1;
			//cout << "likehood value is "<< sum << "\n";
		  if(sum > 126)  // based on the toy MC, this value will reject 99.87% dark noise.
		    return true;
		  else
		    return false;
		}


    bool has_trigger() {
	
        // get position
        ifstream pmt_pos_file;
        map<int,double> pmtx;
        map<int,double> pmty;
        map<int,double> pmtz;
        pmt_pos_file.open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre2/data/Detector/Geometry/PMTPos_CD_LPMT.csv");
        int id;
        double PosX;
        double PosY;
        double PosZ;
        double tmp;
	
				binexp = 1.58508*m_darkRateScale;
        for(int i=0;i<17613;i++){
                pmt_pos_file>>id>>PosX>>PosY>>PosZ>>tmp>>tmp;
                pmtx[i] = PosX;
                pmty[i] = PosY;
                pmtz[i] = PosZ;
        }


        // check if there is any existing trigger.
        if (triggers.size()!=0) {
            return true;
        }

        // try to find a trigger.
        // note, in trigger tool, we don't modify pulse buffer
        std::deque<Pulse>& LPMT_buffer = m_elecsvc->get_CDLPMT_pulse_buffer();
        std::deque<Pulse> buffer(LPMT_buffer);
        auto bound_cmp=boost::bind(&MMTriggerToolLpmt::cmp, this, boost::placeholders::_1, boost::placeholders::_2);
        sort(buffer.begin(),buffer.end(),bound_cmp);

        // if buffer is empty, we can't find any trigger.
        if (buffer.size()==0) {
            return false;
        }

        LogInfo << "Pulse buffer size: " << buffer.size() << std::endl;
        // npmt trigger
        TimeStamp beginTime = buffer.front().pulseHitTime;
        TimeStamp backTime = buffer.back().pulseHitTime;
        double deltaTime = backTime - beginTime;

        double m_tof = buffer.front().tof;// tof of the triggering hit.
        
        int beginIdx = 0;
        int buffersize = buffer.size();

        float triggerWindow = m_Trigger_window; // ns
        float slipWindow = m_Trigger_slip_window; //
        int firedPmtNum = m_Trigger_FiredPmtNum;

        bool found = false;

        pmtfired.clear();
			
        while(true) {
            if (beginIdx >= buffersize) {
                break;
            }


            TimeStamp endTime = beginTime;
            endTime.Add(triggerWindow*1e-9); // trigger window

            pmtfired.clear();

	
            for(int i=0;i<12;i++){
              for(int j=0;j<12;j++){
                for(int k=0;k<12;k++){
                  nhitsArr[i][j][k] = 0;
                }
              }
            }

            for (int i = beginIdx; i < buffersize; ++i) {
                const Pulse& p = buffer[i];

                if (PMT::IsCD(p.pmtID) and PMT::Is20inch(p.pmtID)) { // LPMT

                } else {
                    continue;
                }

                // if the pulse is beyond the time window,
                // we will stop
							 //cout << beginIdx << "\t"<< p.pulseHitTime << "\t" <<  beginTime << "\t" << endTime << "\n";
                if (p.pulseHitTime > endTime) {
                    break;
                }

               //  LogInfo << "pulse info: "
               //          << p.pmtID << " -> "
               //          << p.pulseHitTime << std::endl;
               double costheta = pmtz[p.pmtID]/sqrt(pmtx[p.pmtID]*pmtx[p.pmtID] + pmty[p.pmtID]*pmty[p.pmtID] + pmtz[p.pmtID]*pmtz[p.pmtID]);
							
               double phi;
							 if(pmty[p.pmtID] == 0) phi = 0;
               else phi = acos(pmtx[p.pmtID]/sqrt(pmtx[p.pmtID]*pmtx[p.pmtID] + pmty[p.pmtID]*pmty[p.pmtID]))*pmty[p.pmtID]/abs(pmty[p.pmtID]) + PI;
               int ii = (costheta + 1)/(2./12);
               int jj = phi/(2*PI/12.0);
               int kk = (p.pulseHitTime - beginTime)/16e-9*0.99999;
							 
               nhitsArr[ii][jj][kk]++;


               ++pmtfired[p.pmtID];
            }

            if(MM_Trigger()){
                found = true;
                break;
            }


            //if (pmtfired.size()>= firedPmtNum) {
            //    // found
            //    LogInfo << "fired pmt numbers: " << pmtfired.size() << std::endl;
            //    found = true;
            //    break;
            //}

            // slip trigger window
            endTime = beginTime;
            endTime.Add(slipWindow*1e-9);

            while (++beginIdx < buffersize) {
                const Pulse& p = buffer[beginIdx];

                if (PMT::IsCD(p.pmtID) and PMT::Is20inch(p.pmtID)) { // LPMT

                } else {
                    continue;
                }

                // found the next trigger window
                if (p.pulseHitTime > endTime) {
                    beginTime = p.pulseHitTime;
                    m_tof = p.tof;
                    break;
                }

            }
//            LogInfo << "slip window: beginIdx -> "
//                    << beginIdx
//                    << std::endl;
        }
        pmtx.clear();
        pmty.clear();
        pmtz.clear();

        if (not found) {
           //clear pulses vector.
           //beginTime.Add(m_tof*1e-9);
           for (std::deque<Pulse>::iterator it = LPMT_buffer.begin();
               it != LPMT_buffer.end(); ) {
             if (it->pulseHitTime>=beginTime) {
                break;
              }
             it = LPMT_buffer.erase(it);
         }
            return false;
        }
        //beginTime.Add(m_tof*1e-9);
        // if found, create an trigger object.
        Trigger t;
        t.type = kTriggerLpmt;
        t.time = beginTime;
        triggers.push_back(t);
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
DECLARE_TOOL(MMTriggerToolLpmt);
