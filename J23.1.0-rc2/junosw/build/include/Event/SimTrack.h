#ifndef SimTrack_h
#define SimTrack_h

#include "TObject.h"
#include <vector>

namespace JM
{

    /*
     * Description:
     *   This class represents a track in simulation. 
     *
     * History:
     *   - v5: change double to float
     *   - v6: add parent ID, ancestor ID
     *
     * Author:
     *   Tao Lin <lintao AT ihep.ac.cn>
     */

    class SimTrack: public TObject {
        private:
            Int_t obj_idx;     // The idx used by SimVertex, which is the idx in the SimTrack collection
            Int_t pdg_id;      // PDG ID
            Int_t track_id;    // track ID in Geant4 Simulation
            Int_t parent_id;   // parent track ID. If it is zero, the current track is primary. if -1, unknown.
            Int_t ancestor_id; // ancestor track ID. if -1, unknown
            // == Init ==
            Float_t init_mass; // Unit: MeV
            Float_t init_px;   // Unit: MeV
            Float_t init_py;   // Unit: MeV
            Float_t init_pz;   // Unit: MeV
            Float_t init_x;    // Unit: mm
            Float_t init_y;    // Unit: mm
            Float_t init_z;    // Unit: mm
            Double_t init_t;   // Unit: ns

            // == Exit ==
            Float_t exit_px;   // Unit: MeV
            Float_t exit_py;   // Unit: MeV
            Float_t exit_pz;   // Unit: MeV
            Float_t exit_x;    // Unit: mm
            Float_t exit_y;    // Unit: mm
            Float_t exit_z;    // Unit: mm
            Double_t exit_t;   // Unit: ns

            Float_t track_length; // Unit: mm

            // == Visible or Deposit Energy Related ==
            Float_t edep;      // Unit: MeV
            Float_t edep_x;    // Unit: mm
            Float_t edep_y;    // Unit: mm
            Float_t edep_z;    // Unit: mm

            Float_t Qedep;     // Unit: MeV
            Float_t Qedep_x;   // Unit: mm
            Float_t Qedep_y;   // Unit: mm
            Float_t Qedep_z;   // Unit: mm

            Float_t edep_notinLS; // Unit: MeV
        public:

            SimTrack() {
                obj_idx = -1;
                pdg_id = 0;
                track_id = -1;
                parent_id = -1;
                ancestor_id = -1;

                // == Init ==
                init_mass = 0;
                init_px = 0;
                init_py = 0;
                init_pz = 0;

                init_x = 0;
                init_y = 0;
                init_z = 0;
                init_t = 0;
                // == Exit ==
                exit_px = 0;
                exit_py = 0;
                exit_pz = 0;

                exit_x = 0;
                exit_y = 0;
                exit_z = 0;
                exit_t = 0;

                track_length = 0;
                // == Visible or Deposit Energy Related ==
                edep = 0;
                edep_x = 0;
                edep_y = 0;
                edep_z = 0;
                Qedep = 0;
                Qedep_x = 0;
                Qedep_y = 0;
                Qedep_z = 0;
                edep_notinLS = 0;
            }
            virtual ~SimTrack() {}

            // = getter =
            int getObjID() {return obj_idx;}
            int getPDGID() {return pdg_id;}
            int getTrackID() {return track_id;}
            int getParentID() {return parent_id;}
            int getAncestorID() {return ancestor_id;}
            float getInitMass() {return init_mass;}
            // == Init ==
            float getInitPx() {return init_px;}
            float getInitPy() {return init_py;}
            float getInitPz() {return init_pz;}
            float getInitX() {return init_x;}
            float getInitY() {return init_y;}
            float getInitZ() {return init_z;}
            double getInitT() {return init_t;}
            // == Exit or Died ==
            float getExitPx() {return exit_px;}
            float getExitPy() {return exit_py;}
            float getExitPz() {return exit_pz;}
            float getExitX() {return exit_x;}
            float getExitY() {return exit_y;}
            float getExitZ() {return exit_z;}
            double getExitT() {return exit_t;}
            float getTrackLength() {return track_length;}
            // == Visible or Deposit Energy Related ==
            float getEdep() {return edep;}
            float getEdepX() {return edep_x;}
            float getEdepY() {return edep_y;}
            float getEdepZ() {return edep_z;}
            float getQEdep() {return Qedep;}
            float getQEdepX() {return Qedep_x;}
            float getQEdepY() {return Qedep_y;}
            float getQEdepZ() {return Qedep_z;}
            float getEdepNotInLS() {return edep_notinLS;}

            // setter
            void setObjID(int val) {obj_idx = val;}
            void setPDGID(int val) {pdg_id = val;}
            void setTrackID(int val) {track_id = val;}
            void setParentID(int val) {parent_id = val;}
            void setAncestorID(int val) {ancestor_id = val;}
            void setInitMass(float val) {init_mass = val;}
            // == Init ==
            void setInitPx(float val) {init_px = val;}
            void setInitPy(float val) {init_py = val;}
            void setInitPz(float val) {init_pz = val;}
            void setInitX(float val) {init_x = val;}
            void setInitY(float val) {init_y = val;}
            void setInitZ(float val) {init_z = val;}
            void setInitT(double val) {init_t = val;}
            // == Exit or Died ==
            void setExitPx(float val) {exit_px = val;}
            void setExitPy(float val) {exit_py = val;}
            void setExitPz(float val) {exit_pz = val;}
            void setExitX(float val) {exit_x = val;}
            void setExitY(float val) {exit_y = val;}
            void setExitZ(float val) {exit_z = val;}
            void setExitT(double val) {exit_t = val;}
            void setTrackLength(float val) {track_length = val;}
            // == Visible or Deposit Energy Related ==
            void setEdep(float val) {edep = val;}
            void setEdepX(float val) {edep_x = val;}
            void setEdepY(float val) {edep_y = val;}
            void setEdepZ(float val) {edep_z = val;}
            void setQEdep(float val) {Qedep = val;}
            void setQEdepX(float val) {Qedep_x = val;}
            void setQEdepY(float val) {Qedep_y = val;}
            void setQEdepZ(float val) {Qedep_z = val;}
            void setEdepNotInLS(float val) {edep_notinLS = val;}

            ClassDef(SimTrack, 6)
    };

}

#endif
