#ifndef DummyTTHit_h
#define DummyTTHit_h

#include "TObject.h"
#include <vector>
#include <iostream>

    class DummyTTHit: public TObject {

        private:
            int barid;
            // pe
            float peL;
            float peR;
            // time
            float timeL;
            float timeR;
            // ADC
            float ADCL;
            float ADCR;
            // position
            float x;
            float y;
            float z;
        public:

            DummyTTHit();
            virtual ~DummyTTHit();

            // getter
            int getBarID() { return barid; }

            float getPEL() { return peL; }
            float getPER() { return peR; }

            float getTimeL() { return timeL; }
            float getTimeR() { return timeR; }

            float getADCL() { return ADCL; }
            float getADCR() { return ADCR; }

            float getX() { return x; }
            float getY() { return y; }
            float getZ() { return z; }
            // setter
            void setBarID(int v) { barid = v; }

            void setPEL(float v) { peL = v; }
            void setPER(float v) { peR = v; }

            void setTimeL(float v) { timeL = v; }
            void setTimeR(float v) { timeR = v; }

            void setADCL(float v) { ADCL = v; }
            void setADCR(float v) { ADCR = v; }

            void setX(float v) { x = v; }
            void setY(float v) { y = v; }
            void setZ(float v) { z = v; }

            ClassDef(DummyTTHit, 2)
    };

#endif
