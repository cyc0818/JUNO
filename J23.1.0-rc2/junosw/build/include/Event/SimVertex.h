#ifndef SimVertex_h
#define SimVertex_h

#include "TObject.h"
#include <vector>

namespace JM
{

    /*
     * Description:
     *   This class represents a vertex in simulation.
     *   It contains position, time, process name.
     *   The index of SimTrack is also stored. 
     *
     * History:
     *   - v1: initial version. 20 March 2023
     *
     * Author:
     *   Tao Lin <lintao AT ihep.ac.cn>
     */

    class SimVertex: public TObject {
    private:
        Int_t obj_idx;     // The idx in the SimVertex collection
        Float_t x; // Unit: mm
        Float_t y; // Unit: mm
        Float_t z; // Unit: mm
        Double_t t; // Unit: ns

        std::string process_name;

        // relations
        std::vector<int> track_idx_in; // index in track collection
        std::vector<int> track_idx_out;

    public:

        SimVertex()
            : obj_idx(-1), x(0), y(0), z(0), t(0) {
            
        }

        // = getter =
        int getObjID() {return obj_idx;}

        Float_t getX() { return x; }
        Float_t getY() { return y; }
        Float_t getZ() { return z; }
        Double_t getT() { return t; }

        const std::string& getProcName() { return process_name; }

        const std::vector<int>& getTrackIdxIn() { return track_idx_in; }
        const std::vector<int>& getTrackIdxOut() { return track_idx_out; } 

        // = setter =
        void setObjID(int val) {obj_idx = val;}

        void setXYZT(Float_t _x, Float_t _y, Float_t _z, Double_t _t) {
            x = _x;
            y = _y;
            z = _z;
            t = _t;
        }

        void setProcName(const std::string& _proc_name) {
            process_name = _proc_name;
        }

        void setTrackIdxIn(const std::vector<int>& _in) {
            track_idx_in = _in;
        }

        void setTrackIdxOut(const std::vector<int>& _out) {
            track_idx_out = _out;
        }

        void addTrackIdxIn(int idx) {
            track_idx_in.push_back(idx);
        }

        void addTrackIdxOut(int idx) {
            track_idx_out.push_back(idx);
        }

        ClassDef(SimVertex, 1)
    };

}

#endif
