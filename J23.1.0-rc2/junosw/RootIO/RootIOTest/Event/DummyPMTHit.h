#ifndef DummyPMTHit_h
#define DummyPMTHit_h

#include "TObject.h"
#include <vector>
#include <iostream>

    class DummyPMTHit: public TObject {
        private:
            int pmtid;
            int npe;
            double hittime;
            double timewindow;
            int trackid; // ref to Track ID. but if hits are merged, unknown.
        public:
            DummyPMTHit() {
                pmtid = -1;
                npe = -1;
                hittime = -1;
                timewindow = 0;
                trackid = -1;
            }

            virtual ~DummyPMTHit() {}

        public:
            int getPMTID() {return pmtid;}
            int getNPE() {return npe;}
            double getHitTime() {return hittime;}
            double getTimeWindow() {return timewindow;}
            int getTrackID() { return trackid; }

            void setPMTID(int val) {pmtid=val;}
            void setNPE(int val) {npe=val;}
            void setHitTime(double val) {hittime = val;}
            void setTimeWindow(double val) {timewindow = val;}
            void setTrackID(int val) {trackid = val;}

            ClassDef(DummyPMTHit, 6)
    };

#endif
