#ifndef G4OpticksAnaMgr_hh
#define G4OpticksAnaMgr_hh

/*
 * detsim                                          Opticks
 *   \
 *    ---> begin of run --------> (geom) ---------> OpMgr
 *     \---> begin of event                        /  |
 *     |---> collect steps  ---->------------>-----   |
 *           (scint/ceren)                           /
 *     |---> end of event   ----<------------<------
 *                            (op prop, save hits)
 */

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IAnalysisElement.h"
#include <vector>
#include <map>
class OpMgr;

typedef union {
    float f ;
    int i ;
    unsigned int u ;
} uif_t ;

class G4OKCerenkovStep {
    public:

    enum {

       _Id,                      //  0
       _ParentID,
       _Material,
       _NumPhotons,
      
       _x0_x,                    //  1
       _x0_y,
       _x0_z,
       _t0,

       _DeltaPosition_x,         // 2
       _DeltaPosition_y,
       _DeltaPosition_z,
       _step_length,

       _code,                    // 3
       _charge, 
       _weight, 
       _MeanVelocity,

       _BetaInverse,             //  4
       _Pmin,  
       _Pmax,   
       _maxCos,

       _maxSin2,                 // 5
       _MeanNumberOfPhotons1,
       _MeanNumberOfPhotons2,
       _BialkaliMaterialIndex,

       SIZE

    };

};
class G4OKScintillationStep {
    public:

    enum {

       _Id, 
       _ParentID,
       _Material,
       _NumPhotons,
      
       _x0_x,
       _x0_y,
       _x0_z,
       _t0,

       _DeltaPosition_x,
       _DeltaPosition_y,
       _DeltaPosition_z,
       _step_length,

       _code,
       _charge, 
       _weight, 
       _MeanVelocity,

       _scnt,  
       _slowerRatio,   
       _slowTimeConstant,    
       _slowerTimeConstant,

       _ScintillationTime,
       _ScintillationIntegralMax,
       _Spare1,
       _Spare2,

       SIZE

    };

};



class G4OpticksAnaMgr: public IAnalysisElement,
                       public ToolBase {

public:

    G4OpticksAnaMgr(const std::string& name);
    ~G4OpticksAnaMgr();

    // Run Action
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    // Event Action
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void addGenstep( float* data, unsigned num_float );
private:
    OpMgr* m_opmgr;
    std::map<std::string, int> m_mat_g; // geant4 mat name: index
    std::vector<int> m_g2c; // mapping of mat idx: geant4 to opticks
};



#endif
