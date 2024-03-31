#ifndef ParticleDescription_hh
#define ParticleDescription_hh

#include <string>
#include <vector>

#include "globals.hh"

// this is the per entry for HEPEVT.
//

/** Generates one or more particles with type, momentum, and
    optional time offset, spatial offset, and polarization,
    based on lines read from file via SetState().

    The file has the format

<PRE>
 NHEP
  ISTHEP IDHEP JDAHEP1 JDAHEP2 PHEP1 PHEP2 PHEP3 PHEP5 DT X Y Z PLX PLY PLZ
  ISTHEP IDHEP JDAHEP1 JDAHEP2 PHEP1 PHEP2 PHEP3 PHEP5 DT X Y Z PLX PLY PLZ
  ISTHEP IDHEP JDAHEP1 JDAHEP2 PHEP1 PHEP2 PHEP3 PHEP5 DT X Y Z PLX PLY PLZ
  ... [NHEP times]
 NHEP
  ISTHEP IDHEP JDAHEP1 JDAHEP2 PHEP1 PHEP2 PHEP3 PHEP5 DT X Y Z PLX PLY PLZ
  ISTHEP IDHEP JDAHEP1 JDAHEP2 PHEP1 PHEP2 PHEP3 PHEP5 DT X Y Z PLX PLY PLZ
  ISTHEP IDHEP JDAHEP1 JDAHEP2 PHEP1 PHEP2 PHEP3 PHEP5 DT X Y Z PLX PLY PLZ
  ... [NHEP times]
</PRE>
where
<PRE>
    ISTHEP   == status code
    IDHEP    == HEP PDG code
    JDAHEP   == first daughter
    JDAHEP   == last daughter
    PHEP1    == px in GeV
    PHEP2    == py in GeV
    PHEP3    == pz in GeV
    PHEP5    == mass in GeV
    DT       == vertex _delta_ time, in ns (*)
    X        == x vertex in mm
    Y        == y vertex in mm 
    Z        == z vertex in mm 
    PLX      == x polarization
    PLY      == y polarization
    PLZ      == z polarization
</PRE>

   PHEP5, DT, X, Y, Z, PLX, PLY, and PLZ are all optional.
   If omitted, the respective quantity is left unchanged.
   If DT is specified, the time offset of this and subsequent vertices
   is increased by DT: (*) note DT is a relative shift from the previous line!
   (This is because there is often a very large dynamic range of time offsets
   in certain types of events, e.g., in radioactive decay chains, and this
   convention allows such events to be represented using a reasonable number
   of significant digits.)
   If X, Y, Z, PLX, PLY, and/or PLZ is specified, then the values replace
   any previously specified.
*/



namespace Generator {
  namespace Utils {

struct ParticleInfo {
  /*
   * `stat` indicates the particle stat.
   * = 0 :
   *     null entry. 
   * = 1 :
   *     an existing entry, which has not decayed or fragmented.
   *     This is the main class of entries, which represents the 
   *     `final state' given by the generator. 
   * = 2 :
   *     an entry which has decayed or fragmented and is therefore not 
   *     appearing in the final state, but is retained for event history information. 
   * = 3 :
   *     a documentation line, defined separately from the event history.
   *     This could include the two incoming reacting particles, etc. 
   * = 4 - 10 :
   *     undefined, but reserved for future standards. 
   * = 11 - 200 :
   *     at the disposal of each model builder for constructs specific 
   *     to his program, but equivalent to a null line in the context of any other program. 
   * = 201 - :
   *     at the disposal of users, in particular for event tracking in the detector. 
   */
  G4int stat;
  /*
   * We need:
   *  * particle id
   *  * px   ( GeV ) 
   *  * py   ( GeV )
   *  * pz   ( GeV )
   *  * mass ( GeV )
   */
  G4int pid;
  G4double px;
  G4double py;
  G4double pz;
  G4double mass;
  /*
   * Optional:
   *  * dt (ns)
   *    This is from an Event start.
   *    Different from HepEvt.
   *    in HepEvt, the dt is the time 
   *    begins from the previous line.
   */
  G4double dt;
  /*
   * Optional:
   *  * x y z (mm)
   */
  G4double x;
  G4double y;
  G4double z;
};

typedef std::vector< ParticleInfo > ParticleInfoContainer;

  }

}

#endif
