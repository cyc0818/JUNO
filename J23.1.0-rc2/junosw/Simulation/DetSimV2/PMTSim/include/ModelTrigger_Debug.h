#pragma once
/**
ModelTrigger_Debug.h
=====================

See j/PMTSim/PMTSim.cc for example of use which must plant the statics::

      45 #ifdef POM_DEBUG
      46 #include "NPX.h"
      47 #include "ModelTrigger_Debug.h"
      48 std::vector<ModelTrigger_Debug> ModelTrigger_Debug::RECORD = {} ;
      49 UName                           ModelTrigger_Debug::UPV = {} ;
      50 
      51 
      52 NP* PMTSim::ModelTrigger_Debug_Array()   // static
      53 {
      54     return ModelTrigger_Debug::Array() ;
      55 }
      56 #endif

**/


#if defined(PMTSIM_STANDALONE) && defined(POM_DEBUG)
#include "NPX.h"

struct ModelTrigger_Debug
{
    static std::vector<ModelTrigger_Debug> RECORD ; 
    static UName PV ;  
    static UName MLV ;  

    void add(){ RECORD.push_back(*this); }
    static NP* Array(); 
    static constexpr const char* NAME = "ModelTrigger_Debug.npy" ; 
    static void Save(const char* savedir); 
    
    double pos_x ;       // 00
    double pos_y ;       // 01
    double pos_z ;       // 02
    double time ;        // 03
   
    double dir_x ;       // 10
    double dir_y ;       // 11
    double dir_z ;       // 12
    double energy ;      // 13

    double   dist1 ;     // 20
    double   dist2 ;     // 21
    uint64_t mlv   ;     // 22
    uc8      etrig ;     // 23

    uint64_t index ;     // 30 
    uint64_t pv ;        // 31
    uint64_t whereAmI ;  // 32
    uint64_t trig ;      // 33

    double next_pos_x ;  // 40 
    double next_pos_y ;  // 41 
    double next_pos_z ;  // 42 
    double next_mct   ;  // 43 

    double next_norm_x ;  // 50 
    double next_norm_y ;  // 51 
    double next_norm_z ;  // 52 
    uint64_t impl      ;  // 53 

    uint64_t EInside1  ;  // 60
    uint64_t s61       ;  // 61
    uint64_t s62       ;  // 62
    uint64_t s63       ;  // 63

    static const int NUM_QUAD = 7 ; 
};


inline NP* ModelTrigger_Debug::Array()
{
    NP* a = NPX::ArrayFromVec<double, ModelTrigger_Debug>( ModelTrigger_Debug::RECORD, NUM_QUAD, 4 ) ; 

    a->set_names( ModelTrigger_Debug::PV.names ); 
    a->set_meta("PV",  ModelTrigger_Debug::PV.as_str() );
    a->set_meta("MLV", ModelTrigger_Debug::MLV.as_str() );
    return a ; 
} 

inline void ModelTrigger_Debug::Save(const char* savedir)
{
    NP* a = Array(); 
    a->save(savedir, NAME) ;     
}

#endif


/**

     
    In [11]: mtd = t.ModelTrigger_Debug  

    In [4]: imtd = mtd[:,3,0].view(np.uint64)


    In [13]: etrig = mtd[:,2,3].view("|S8")      

    In [6]: pv = mtd[:,3,1].view(np.uint64)        
    In [9]: u_pv, n_pv = np.unique(pv, return_counts=True )       

    In [21]: np.c_[ u_pv, n_pv, t.ModelTrigger_Debug_meta.PV ]
    Out[21]: 
    array([['0', '1405', 'nnvt_body_phys'],
           ['1', '622', 'nnvt_inner1_phys'],
           ['2', '721', 'nnvt_inner2_phys'],
           ['3', '12', 'nnvt_tube_phy'],
           ['4', '22', 'nnvt_edge_phy'],
           ['5', '48', 'hama_body_phys'],
           ['6', '36', 'nnvt_plate_phy'],
           ['7', '10', 'hama_inner1_phys'],
           ['8', '15', 'hama_inner2_phys'],
           ['9', '2', 'hama_outer_edge_phy'],
           ['10', '1', 'hama_plate_phy'],
           ['11', '1', 'hama_dynode_tube_phy'],
           ['12', '1', 'hama_inner_ring_phy']], dtype='<U32')

    In [26]: mlv = mtd[:,2,2].view(np.uint64)   
    In [24]: u_mlv, n_mlv = np.unique( mlv, return_counts=True )

    In [30]: np.c_[u_mlv, n_mlv, t.ModelTrigger_Debug_meta.MLV]
    Out[30]: 
    array([['0', '1405', 'nnvt_log'],
           ['1', '1343', 'nnvt_body_log'],
           ['2', '70', 'nnvt_inner2_log'],
           ['3', '48', 'hama_log'],
           ['4', '25', 'hama_body_log'],
           ['5', '5', 'hama_inner2_log']], dtype='<U32')

    In [14]: u_etrig, n_etrig = np.unique( etrig, return_counts=True )
    In [32]: np.c_[n_etrig, u_etrig]
    Out[32]: 
    array([[b'233', b'N_I1>I2'],
           [b'186', b'N_MIS_I2'],
           [b'75', b'N_MLV_I2'],
           [b'736', b'N_PV_I2'],
           [b'351', b'N_TAIL'],
           [b'1034', b'Y_GLASS'],
           [b'281', b'Y_VACUUM']], dtype='|S21')

**/

