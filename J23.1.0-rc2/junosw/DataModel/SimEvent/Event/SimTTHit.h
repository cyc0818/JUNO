#ifndef SimTTHit_h
#define SimTTHit_h

#include "TObject.h"
#include <vector>
#include <iostream>

namespace JM {
    class SimTTHit: public TObject {

        private:
            int channelid; //Channel ID of energy deposit
            float edep;    //Energy deposit per channel (MeV)
            float time;    //Time of energy deposit (ns)


            //float pe;
            //float ADC;
            //float x;
            //float y;
            //float z;

        public:

            SimTTHit();
            virtual ~SimTTHit();

            // getter
            int getChannelID() { return channelid; }
            float getEdep() { return edep; }
            float getTime(){ return time; }

            //float getPE() { return pe; }
            //float getADC() { return ADC; }
            //float getX() { return x; }
            //float getY() { return y; }
            //float getZ() { return z; }

            // setter
            void setChannelID(int v) { channelid = v; }
            void setEdep(float v) { edep = v; }
            void setTime(float v) { time = v; }

            //void setPE(float v) { pe = v; }
            //void setADC(float v) { ADC = v; }
            //void setX(float v) { x = v; }
            //void setY(float v) { y = v; }
            //void setZ(float v) { z = v; }

            ClassDef(SimTTHit, 3)
    };
}

#endif
