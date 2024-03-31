#ifndef SimPMTHit_h
#define SimPMTHit_h

#include "TObject.h"
#include <vector>
#include <iostream>

namespace JM
{
    /*
     * Description: SimPMTHit
     * ChangeLog
     * - v7: use float for timewindow/localtheta/localphi
     *
     */
    class SimPMTHit: public TObject {
        private:
            int pmtid;
            int npe;
            double hittime;
            float timewindow;
            int trackid; // ref to Track ID. but if hits are merged, unknown.
            float localtheta;
            float localphi;
        public:
            SimPMTHit() {
                pmtid = -1;
                npe = -1;
                hittime = -1;
                timewindow = 0;
                trackid = -1;
                localtheta = -1;
                localphi = -1;
            }

            virtual ~SimPMTHit() {}

        public:
            int getPMTID() {return pmtid;}
            int getNPE() {return npe;}
            double getHitTime() {return hittime;}
            float getTimeWindow() {return timewindow;}
            int getTrackID() { return trackid; }
            float getLocalTheta(){return localtheta;}
            float getLocalPhi(){ return localphi;}

            void setPMTID(int val) {pmtid=val;}
            void setNPE(int val) {npe=val;}
            void setHitTime(double val) {hittime = val;}
            void setTimeWindow(float val) {timewindow = val;}
            void setTrackID(int val) {trackid = val;}
            void setLocalTheta(float val) {localtheta = val;}
            void setLocalPhi(float val) {localphi = val;}
            ClassDef(SimPMTHit, 7)
    };
}

#endif
