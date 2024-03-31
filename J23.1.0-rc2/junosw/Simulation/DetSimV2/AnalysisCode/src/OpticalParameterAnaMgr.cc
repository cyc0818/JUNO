
#include "OpticalParameterAnaMgr.hh"
#include "G4Run.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "RootWriter/RootWriter.h"
#include "TTree.h"

#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"

DECLARE_TOOL(OpticalParameterAnaMgr);

OpticalParameterAnaMgr::OpticalParameterAnaMgr(const std::string& name)
    : ToolBase(name) {
    m_op_tree = 0;
}

OpticalParameterAnaMgr::~OpticalParameterAnaMgr() {

}

void
OpticalParameterAnaMgr::BeginOfRunAction(const G4Run* /*run*/) {
    SniperPtr<RootWriter> svc(*getParent(), "RootWriter");
    if (svc.invalid()) {
        LogError << "Can't Locate RootWriter. If you want to use it, please "
                 << "enalbe it in your job option file."
                 << std::endl;
        return;
    }

    m_op_tree = svc->bookTree(*m_par, "SIMEVT/opticalparam", "Optical Parameters");

    // NOTE: 
    // we use geant4 9.4 now, so the property vector is using the old style.
    // If we use new version, this code needs to be upgraded.
    // -- 2015.10.14 Tao Lin
    // ======================================================================
    // * LS light yield, refractive index. 
    // * LS emission spectrum, re-emission prob. 
    // * LS absorption length and Rayleigh scattering length. 
    // ======================================================================
    G4Material* mat_LS = G4Material::GetMaterial("LS");
    G4int LS_LY_n = 0;         G4double* LS_LY_energy = 0;         G4double* LS_LY_ly = 0;
    G4int LS_RS_n = 0;         G4double* LS_RS_energy = 0;         G4double* LS_RS_rs = 0;
    G4int LS_RI_n = 0;         G4double* LS_RI_energy = 0;         G4double* LS_RI_idx = 0;
    G4int LS_FastEmission_n = 0;   G4double* LS_FastEmission_energy = 0;   G4double* LS_FastEmission_spec = 0;
    G4int LS_SlowEmission_n = 0;   G4double* LS_SlowEmission_energy = 0;   G4double* LS_SlowEmission_spec = 0;
    G4int LS_ReEmission_n = 0; G4double* LS_ReEmission_energy = 0; G4double* LS_ReEmission_prob = 0;
    G4int LS_ABS_n = 0;        G4double* LS_ABS_energy = 0;        G4double* LS_ABS_len = 0;
    G4int LS_Rayleigh_n = 0;   G4double* LS_Rayleigh_energy = 0;   G4double* LS_Rayleigh_len = 0;

    G4int LS_OpticalCon_n = 0; G4double * LS_OpticalCon_time = 0 ; G4double * LS_OpticalCon_ratio = 0;
    G4int LS_GammaCon_n = 0; G4double * LS_GammaCon_time = 0 ; G4double * LS_GammaCon_ratio = 0;
    G4int LS_AlphaCon_n = 0; G4double * LS_AlphaCon_time = 0 ; G4double * LS_AlphaCon_ratio = 0;
    G4int LS_NeutronCon_n = 0; G4double * LS_NeutronCon_time = 0 ; G4double * LS_NeutronCon_ratio = 0;
  
    G4int LS_PPOABS_n = 0; G4double * LS_PPOABS_energy = 0 ; G4double * LS_PPOABS_len = 0;
    G4int LS_PPOReEmission_n = 0; G4double * LS_PPOReEmission_energy = 0 ; G4double * LS_PPOReEmission_prob = 0;
    G4int LS_PPOEmission_n = 0; G4double * LS_PPOEmission_energy = 0 ; G4double * LS_PPOEmission_spec = 0;
    G4int LS_PPOTime_n = 0; G4double * LS_PPOTime_com = 0 ; G4double * LS_PPOTime_time = 0;
   
    G4int LS_bisABS_n = 0; G4double * LS_bisABS_energy = 0 ; G4double * LS_bisABS_len = 0;
    G4int LS_bisReEmission_n = 0; G4double * LS_bisReEmission_energy = 0 ; G4double * LS_bisReEmission_prob = 0;
    G4int LS_bisEmission_n = 0; G4double * LS_bisEmission_energy = 0 ; G4double * LS_bisEmission_spec = 0;
    G4int LS_bisTime_n = 0; G4double * LS_bisTime_com = 0 ; G4double * LS_bisTime_time = 0;
 
    
    

    if (mat_LS) {
        G4MaterialPropertiesTable* tbl_LS = mat_LS->GetMaterialPropertiesTable();
        // SCINTILLATIONYIELD
        get_matprop(tbl_LS, "SCINTILLATIONYIELD", LS_LY_n, LS_LY_energy, LS_LY_ly);
        //
        get_matprop(tbl_LS, "RESOLUTIONSCALE", LS_RS_n, LS_RS_energy, LS_RS_rs);
        // RINDEX
        get_matprop(tbl_LS, "RINDEX", LS_RI_n, LS_RI_energy, LS_RI_idx);
        // emission including Fast and Slow, here we save Fast (TODO)
        get_matprop(tbl_LS, "FASTCOMPONENT", LS_FastEmission_n, LS_FastEmission_energy, LS_FastEmission_spec);
        //
        get_matprop(tbl_LS, "SLOWCOMPONENT", LS_SlowEmission_n, LS_SlowEmission_energy, LS_SlowEmission_spec);
        // REEMISSIONPROB
        get_matprop(tbl_LS, "REEMISSIONPROB", LS_ReEmission_n, LS_ReEmission_energy, LS_ReEmission_prob);
        // ABSLENGTH
        get_matprop(tbl_LS, "ABSLENGTH", LS_ABS_n, LS_ABS_energy, LS_ABS_len);
        // RAYLEIGH
        get_matprop(tbl_LS, "RAYLEIGH", LS_Rayleigh_n, LS_Rayleigh_energy, LS_Rayleigh_len);
        //
        get_matprop(tbl_LS, "OpticalCONSTANT", LS_OpticalCon_n, LS_OpticalCon_time, LS_OpticalCon_ratio);
        
        get_matprop(tbl_LS, "AlphaCONSTANT", LS_AlphaCon_n, LS_AlphaCon_time, LS_AlphaCon_ratio);
        get_matprop(tbl_LS, "GammaCONSTANT", LS_GammaCon_n, LS_GammaCon_time, LS_GammaCon_ratio);
        get_matprop(tbl_LS, "NeutronCONSTANT", LS_NeutronCon_n, LS_NeutronCon_time, LS_NeutronCon_ratio);   
        
        get_matprop(tbl_LS, "PPOABSLENGTH", LS_PPOABS_n, LS_PPOABS_energy, LS_PPOABS_len);
        get_matprop(tbl_LS, "PPOREEMISSIONPROB", LS_PPOReEmission_n, LS_PPOReEmission_energy, LS_PPOReEmission_prob);
        get_matprop(tbl_LS, "PPOCOMPONENT", LS_PPOEmission_n, LS_PPOEmission_energy, LS_PPOEmission_spec);
        get_matprop(tbl_LS, "PPOTIMECONSTANT", LS_PPOTime_n, LS_PPOTime_com, LS_PPOTime_time);

        get_matprop(tbl_LS, "bisMSBABSLENGTH", LS_bisABS_n, LS_bisABS_energy, LS_bisABS_len);
        get_matprop(tbl_LS, "bisMSBREEMISSIONPROB", LS_bisReEmission_n, LS_bisReEmission_energy, LS_bisReEmission_prob);
        get_matprop(tbl_LS, "bisMSBCOMPONENT", LS_bisEmission_n, LS_bisEmission_energy, LS_bisEmission_spec);
        get_matprop(tbl_LS, "bisMSBTIMECONSTANT", LS_bisTime_n, LS_bisTime_com, LS_bisTime_time);   


    }
    m_op_tree->Branch("LS_LY_n", &LS_LY_n, "LS_LY_n/I");
    m_op_tree->Branch("LS_LY_energy", LS_LY_energy, "LS_LY_energy[LS_LY_n]/D");
    m_op_tree->Branch("LS_LY_ly", LS_LY_ly, "LS_LY_ly[LS_LY_n]/D");

    m_op_tree->Branch("LS_RS_n", &LS_RS_n, "LS_RS_n/I");
    m_op_tree->Branch("LS_RS_energy", LS_RS_energy, "LS_RS_energy[LS_RS_n]/D");
    m_op_tree->Branch("LS_RS_rs", LS_RS_rs, "LS_RS_rs[LS_RS_n]/D");

    m_op_tree->Branch("LS_RI_n", &LS_RI_n, "LS_RI_n/I");
    m_op_tree->Branch("LS_RI_energy", LS_RI_energy, "LS_RI_energy[LS_RI_n]/D");
    m_op_tree->Branch("LS_RI_idx", LS_RI_idx, "LS_RI_idx[LS_RI_n]/D");

    m_op_tree->Branch("LS_FastEmission_n", &LS_FastEmission_n, "LS_FastEmission_n/I");
    m_op_tree->Branch("LS_FastEmission_energy", LS_FastEmission_energy, "LS_FastEmission_energy[LS_FastEmission_n]/D");
    m_op_tree->Branch("LS_FastEmission_spec", LS_FastEmission_spec, "LS_FastEmission_spec[LS_FastEmission_n]/D");

    m_op_tree->Branch("LS_SlowEmission_n", &LS_SlowEmission_n, "LS_SlowEmission_n/I");
    m_op_tree->Branch("LS_SlowEmission_energy", LS_SlowEmission_energy, "LS_SlowEmission_energy[LS_SlowEmission_n]/D");
    m_op_tree->Branch("LS_SlowEmission_spec", LS_SlowEmission_spec, "LS_SlowEmission_spec[LS_SlowEmission_n]/D");

    m_op_tree->Branch("LS_ReEmission_n", &LS_ReEmission_n, "LS_ReEmission_n/I");
    m_op_tree->Branch("LS_ReEmission_energy", LS_ReEmission_energy, "LS_ReEmission_energy[LS_ReEmission_n]/D");
    m_op_tree->Branch("LS_ReEmission_prob", LS_ReEmission_prob, "LS_ReEmission_prob[LS_ReEmission_n]/D");

    m_op_tree->Branch("LS_ABS_n", &LS_ABS_n, "LS_ABS_n/I");
    m_op_tree->Branch("LS_ABS_energy", LS_ABS_energy, "LS_ABS_energy[LS_ABS_n]/D");
    m_op_tree->Branch("LS_ABS_len", LS_ABS_len, "LS_ABS_len[LS_ABS_n]/D");

    m_op_tree->Branch("LS_Rayleigh_n", &LS_Rayleigh_n, "LS_Rayleigh_n/I");
    m_op_tree->Branch("LS_Rayleigh_energy", LS_Rayleigh_energy, "LS_Rayleigh_energy[LS_Rayleigh_n]/D");
    m_op_tree->Branch("LS_Rayleigh_len", LS_Rayleigh_len, "LS_Rayleigh_len[LS_Rayleigh_n]/D");

    m_op_tree->Branch("LS_OpticalCon_n", &LS_OpticalCon_n, "LS_OpticalCon_n/I");
    m_op_tree->Branch("LS_OpticalCon_time", LS_OpticalCon_time, "LS_OpticalCon_time[LS_OpticalCon_n]/D");
    m_op_tree->Branch("LS_OpticalCon_ratio", LS_OpticalCon_ratio, "LS_OpticalCon_ratio[LS_OpticalCon_n]/D");

    m_op_tree->Branch("LS_AlphaCon_n", &LS_AlphaCon_n, "LS_AlphaCon_n/I");
    m_op_tree->Branch("LS_AlphaCon_time", LS_AlphaCon_time, "LS_AlphaCon_time[LS_AlphaCon_n]/D");
    m_op_tree->Branch("LS_AlphaCon_ratio", LS_AlphaCon_ratio, "LS_AlphaCon_ratio[LS_AlphaCon_n]/D");   

    m_op_tree->Branch("LS_GammaCon_n", &LS_GammaCon_n, "LS_GammaCon_n/I");
    m_op_tree->Branch("LS_GammaCon_time", LS_GammaCon_time, "LS_GammaCon_time[LS_GammaCon_n]/D");
    m_op_tree->Branch("LS_GammaCon_ratio", LS_GammaCon_ratio, "LS_GammaCon_ratio[LS_GammaCon_n]/D");

    m_op_tree->Branch("LS_NeutronCon_n", &LS_NeutronCon_n, "LS_NeutronCon_n/I");
    m_op_tree->Branch("LS_NeutronCon_time", LS_NeutronCon_time, "LS_NeutronCon_time[LS_NeutronCon_n]/D");
    m_op_tree->Branch("LS_NeutronCon_ratio", LS_NeutronCon_ratio, "LS_NeutronCon_ratio[LS_NeutronCon_n]/D");
  
    m_op_tree->Branch("LS_PPOABS_n", &LS_PPOABS_n, "LS_PPOABS_n/I");
    m_op_tree->Branch("LS_PPOABS_energy", LS_PPOABS_energy, "LS_PPOABS_energy[LS_PPOABS_n]/D");
    m_op_tree->Branch("LS_PPOABS_len", LS_PPOABS_len, "LS_PPOABS_len[LS_PPOABS_n]/D");

    m_op_tree->Branch("LS_PPOReEmission_n", &LS_PPOReEmission_n, "LS_PPOReEmission_n/I");
    m_op_tree->Branch("LS_PPOReEmission_energy", LS_PPOReEmission_energy, "LS_PPOReEmission_energy[LS_PPOReEmission_n]/D");
    m_op_tree->Branch("LS_PPOReEmission_prob", LS_PPOReEmission_prob, "LS_PPOReEmission_prob[LS_PPOReEmission_n]/D");

    m_op_tree->Branch("LS_PPOEmission_n", &LS_PPOEmission_n, "LS_PPOEmission_n/I");
    m_op_tree->Branch("LS_PPOEmission_energy", LS_PPOEmission_energy, "LS_PPOEmission_energy[LS_PPOEmission_n]/D");
    m_op_tree->Branch("LS_PPOEmission_spec", LS_PPOEmission_spec, "LS_PPOEmission_spec[LS_PPOEmission_n]/D");

    m_op_tree->Branch("LS_PPOTime_n", &LS_PPOTime_n, "LS_PPOTime_n/I");    
    m_op_tree->Branch("LS_PPOTime_com", LS_PPOTime_com, "LS_PPOTime_com[LS_PPOTime_n]/D");
    m_op_tree->Branch("LS_PPOTime_time", LS_PPOTime_time, "LS_PPOTime_time[LS_PPOTime_n]/D");

    m_op_tree->Branch("LS_bisABS_n", &LS_bisABS_n, "LS_bisABS_n/I");                
    m_op_tree->Branch("LS_bisABS_energy", LS_bisABS_energy, "LS_bisABS_energy[LS_bisABS_n]/D");
    m_op_tree->Branch("LS_bisABS_len", LS_bisABS_len, "LS_bisABS_len[LS_bisABS_n]/D");

    m_op_tree->Branch("LS_bisReEmission_n", &LS_bisReEmission_n, "LS_bisReEmission_n/I");
    m_op_tree->Branch("LS_bisReEmission_energy", LS_bisReEmission_energy, "LS_bisReEmission_energy[LS_bisReEmission_n]/D");
    m_op_tree->Branch("LS_bisReEmission_prob", LS_bisReEmission_prob, "LS_bisReEmission_prob[LS_bisReEmission_n]/D");
    
    m_op_tree->Branch("LS_bisEmission_n", &LS_bisEmission_n, "LS_bisEmission_n/I");
    m_op_tree->Branch("LS_bisEmission_energy", LS_bisEmission_energy, "LS_bisEmission_energy[LS_bisEmission_n]/D");
    m_op_tree->Branch("LS_bisEmission_spec", LS_bisEmission_spec, "LS_bisEmission_spec[LS_bisEmission_n]/D");

    m_op_tree->Branch("LS_bisTime_n", &LS_bisTime_n, "LS_bisTime_n/I");
    m_op_tree->Branch("LS_bisTime_com", LS_bisTime_com, "LS_bisTime_com[LS_bisTime_n]/D");
    m_op_tree->Branch("LS_bisTime_time", LS_bisTime_time, "LS_bisTime_time[LS_bisTime_n]/D"); 
    

   //===============================================================
   // * LAB 
   //===============================================================
    G4Material* mat_LAB = G4Material::GetMaterial("LAB");
    G4int LAB_Rayleigh_n = 0; G4double* LAB_Rayleigh_energy = 0; G4double* LAB_Rayleigh_len = 0;
    G4int LAB_RI_n = 0;  G4double* LAB_RI_energy = 0;  G4double* LAB_RI_idx = 0;
    if(mat_LAB){
     G4MaterialPropertiesTable* tbl_LAB = mat_LAB->GetMaterialPropertiesTable();
     get_matprop(tbl_LAB, "RAYLEIGH", LAB_Rayleigh_n, LAB_Rayleigh_energy, LAB_Rayleigh_len);
     get_matprop(tbl_LAB, "RINDEX", LAB_RI_n, LAB_RI_energy, LAB_RI_idx);
    }
     
    m_op_tree->Branch("LAB_RI_n", &LAB_RI_n, "LAB_RI_n/I");
    m_op_tree->Branch("LAB_RI_energy", LAB_RI_energy, "LAB_RI_energy[LAB_RI_n]/D");
    m_op_tree->Branch("LAB_RI_idx", LAB_RI_idx, "LAB_RI_idx[LAB_RI_n]/D"); 
   
    m_op_tree->Branch("LAB_Rayleigh_n", &LAB_Rayleigh_n, "LAB_Rayleigh_n/I");
    m_op_tree->Branch("LAB_Rayleigh_energy", LAB_Rayleigh_energy, "LAB_Rayleigh_energy[LAB_Rayleigh_n]/D");
    m_op_tree->Branch("LAB_Rayleigh_len", LAB_Rayleigh_len, "LAB_Rayleigh_len[LAB_Rayleigh_n]/D");
  //--------------------

    // ======================================================================
    // * Acrylic absorption length and refractive index.
    // ======================================================================
    G4Material* mat_Acrylic = G4Material::GetMaterial("Acrylic");
    G4int Acrylic_ABS_n = 0; G4double* Acrylic_ABS_energy = 0; G4double* Acrylic_ABS_len = 0;
    G4int Acrylic_RI_n = 0;  G4double* Acrylic_RI_energy = 0;  G4double* Acrylic_RI_idx = 0;
    if (mat_Acrylic) {
        G4MaterialPropertiesTable* tbl_Acrylic = mat_Acrylic->GetMaterialPropertiesTable();
        get_matprop(tbl_Acrylic, "ABSLENGTH", Acrylic_ABS_n, Acrylic_ABS_energy, Acrylic_ABS_len);
        get_matprop(tbl_Acrylic, "RINDEX", Acrylic_RI_n, Acrylic_RI_energy, Acrylic_RI_idx);
    }

    m_op_tree->Branch("Acrylic_ABS_n", &Acrylic_ABS_n, "Acrylic_ABS_n/I");
    m_op_tree->Branch("Acrylic_ABS_energy", Acrylic_ABS_energy, "Acrylic_ABS_energy[Acrylic_ABS_n]/D");
    m_op_tree->Branch("Acrylic_ABS_len", Acrylic_ABS_len, "Acrylic_ABS_len[Acrylic_ABS_n]/D");

    m_op_tree->Branch("Acrylic_RI_n", &Acrylic_RI_n, "Acrylic_RI_n/I");
    m_op_tree->Branch("Acrylic_RI_energy", Acrylic_RI_energy, "Acrylic_RI_energy[Acrylic_RI_n]/D");
    m_op_tree->Branch("Acrylic_RI_idx", Acrylic_RI_idx, "Acrylic_RI_idx[Acrylic_RI_n]/D");
    //===================================
    //ESR
    //===================================
    G4Material* mat_ESR = G4Material::GetMaterial("ESR");
    G4int ESR_ABS_n = 0; G4double* ESR_ABS_energy = 0; G4double* ESR_ABS_len = 0;
    if (mat_ESR) {
        G4MaterialPropertiesTable* tbl_ESR = mat_ESR->GetMaterialPropertiesTable();
        get_matprop(tbl_ESR, "ABSLENGTH", ESR_ABS_n, ESR_ABS_energy, ESR_ABS_len);
    }

    m_op_tree->Branch("ESR_ABS_n", &ESR_ABS_n, "ESR_ABS_n/I");
    m_op_tree->Branch("ESR_ABS_energy", ESR_ABS_energy, "ESR_ABS_energy[ESR_ABS_n]/D");
    m_op_tree->Branch("ESR_ABS_len", ESR_ABS_len, "ESR_ABS_len[ESR_ABS_n]/D");

    //=======================================
    //Tyvek
    //==========================
    G4Material* mat_Tyvek = G4Material::GetMaterial("Tyvek");
    G4int Tyvek_ABS_n = 0; G4double* Tyvek_ABS_energy = 0; G4double* Tyvek_ABS_len = 0;
   // G4int Tyvek_REF_n = 0;    G4double* Tyvek_REF_energy = 0;    G4double* Tyvek_REF_ref = 0;
    if (mat_Tyvek) {
        G4MaterialPropertiesTable* tbl_Tyvek = mat_Tyvek->GetMaterialPropertiesTable();
        get_matprop(tbl_Tyvek, "ABSLENGTH", Tyvek_ABS_n, Tyvek_ABS_energy, Tyvek_ABS_len);
    }

    m_op_tree->Branch("Tyvek_ABS_n", &Tyvek_ABS_n, "Tyvek_ABS_n/I");
    m_op_tree->Branch("Tyvek_ABS_energy", Tyvek_ABS_energy, "Tyvek_ABS_energy[Tyvek_ABS_n]/D");
    m_op_tree->Branch("Tyvek_ABS_len", Tyvek_ABS_len, "Tyvek_ABS_len[Tyvek_ABS_n]/D");

    //==============================================
    //AcrylicMask
    //=============================================
    G4Material* mat_AcrylicMask = G4Material::GetMaterial("AcrylicMask");
    G4int AcrylicMask_ABS_n = 0; G4double* AcrylicMask_ABS_energy = 0; G4double* AcrylicMask_ABS_len = 0;
    G4int AcrylicMask_RI_n = 0;  G4double* AcrylicMask_RI_energy = 0;  G4double* AcrylicMask_RI_idx = 0;
    if (mat_AcrylicMask) {
        G4MaterialPropertiesTable* tbl_AcrylicMask = mat_AcrylicMask->GetMaterialPropertiesTable();
        get_matprop(tbl_AcrylicMask, "ABSLENGTH", AcrylicMask_ABS_n, AcrylicMask_ABS_energy, AcrylicMask_ABS_len);
        get_matprop(tbl_AcrylicMask, "RINDEX", AcrylicMask_RI_n, AcrylicMask_RI_energy, AcrylicMask_RI_idx);
    }

    m_op_tree->Branch("AcrylicMask_ABS_n", &AcrylicMask_ABS_n, "AcrylicMask_ABS_n/I");
    m_op_tree->Branch("AcrylicMask_ABS_energy", AcrylicMask_ABS_energy, "AcrylicMask_ABS_energy[AcrylicMask_ABS_n]/D");  
    m_op_tree->Branch("AcrylicMask_ABS_len", AcrylicMask_ABS_len, "AcrylicMask_ABS_len[AcrylicMask_ABS_n]/D");
    
    m_op_tree->Branch("AcrylicMask_RI_n", &AcrylicMask_RI_n, "AcrylicMask_RI_n/I");
    m_op_tree->Branch("AcrylicMask_RI_energy", AcrylicMask_RI_energy, "AcrylicMask_RI_energy[AcrylicMask_RI_n]/D");
    m_op_tree->Branch("AcrylicMask_RI_idx", AcrylicMask_RI_idx, "AcrylicMask_RI_idx[AcrylicMask_RI_n]/D");

    //=========================
    //Steel
    //==========================
    G4Material* mat_Steel = G4Material::GetMaterial("Steel");
    G4int Steel_ABS_n = 0; G4double* Steel_ABS_energy = 0; G4double* Steel_ABS_len = 0;
    if (mat_Steel) {
        G4MaterialPropertiesTable* tbl_Steel = mat_Steel->GetMaterialPropertiesTable();
        get_matprop(tbl_Steel, "ABSLENGTH", Steel_ABS_n, Steel_ABS_energy, Steel_ABS_len);
    }

    m_op_tree->Branch("Steel_ABS_n", &Steel_ABS_n, "Steel_ABS_n/I");
    m_op_tree->Branch("Steel_ABS_energy", Steel_ABS_energy, "Steel_ABS_energy[Steel_ABS_n]/D");   
    m_op_tree->Branch("Steel_ABS_len", Steel_ABS_len, "Steel_ABS_len[Steel_ABS_n]/D");
    
    //=======================
    //LatticedShellSteel
    //=======================
 
    G4Material* mat_LatticedShellSteel = G4Material::GetMaterial("LatticedShellSteel");
    G4int LatticedShellSteel_ABS_n = 0; G4double* LatticedShellSteel_ABS_energy = 0; G4double* LatticedShellSteel_ABS_len = 0; 
    if (mat_LatticedShellSteel) {
        G4MaterialPropertiesTable* tbl_LatticedShellSteel = mat_LatticedShellSteel->GetMaterialPropertiesTable();
        get_matprop(tbl_LatticedShellSteel, "ABSLENGTH", LatticedShellSteel_ABS_n, LatticedShellSteel_ABS_energy, LatticedShellSteel_ABS_len);
       }  
    m_op_tree->Branch("LatticedShellSteel_ABS_n", &LatticedShellSteel_ABS_n, "LatticedShellSteel_ABS_n/I");
    m_op_tree->Branch("LatticedShellSteel_ABS_energy", LatticedShellSteel_ABS_energy, "LatticedShellSteel_ABS_energy[LatticedShellSteel_ABS_n]/D");
    m_op_tree->Branch("LatticedShellSteel_ABS_len", LatticedShellSteel_ABS_len, "LatticedShellSteel_ABS_len[LatticedShellSteel_ABS_n]/D");
    //=====================================
    //StainlessSteel
    //=====================================
    G4Material* mat_StainlessSteel = G4Material::GetMaterial("StainlessSteel");
    G4int StainlessSteel_ABS_n = 0; G4double* StainlessSteel_ABS_energy = 0; G4double* StainlessSteel_ABS_len = 0;
    if (mat_StainlessSteel) {
        G4MaterialPropertiesTable* tbl_StainlessSteel = mat_StainlessSteel->GetMaterialPropertiesTable();
        get_matprop(tbl_StainlessSteel, "ABSLENGTH", StainlessSteel_ABS_n, StainlessSteel_ABS_energy, StainlessSteel_ABS_len);
    }

    m_op_tree->Branch("StainlessSteel_ABS_n", &StainlessSteel_ABS_n, "StainlessSteel_ABS_n/I");
    m_op_tree->Branch("StainlessSteel_ABS_energy", StainlessSteel_ABS_energy, "StainlessSteel_ABS_energy[StainlessSteel_ABS_n]/D");
    m_op_tree->Branch("StainlessSteel_ABS_len", StainlessSteel_ABS_len, "StainlessSteel_ABS_len[StainlessSteel_ABS_n]/D");
    //===============================================
    //Mylar
    //==============================================
    G4Material* mat_Mylar = G4Material::GetMaterial("Mylar");    
    G4int Mylar_ABS_n = 0; G4double* Mylar_ABS_energy = 0; G4double* Mylar_ABS_len = 0;
    G4int Mylar_RI_n = 0;  G4double* Mylar_RI_energy = 0;  G4double* Mylar_RI_idx = 0;    
    G4int Mylar_Rayleigh_n = 0;   G4double* Mylar_Rayleigh_energy = 0;   G4double* Mylar_Rayleigh_len = 0;
    if (mat_Mylar) {
        G4MaterialPropertiesTable* tbl_Mylar = mat_Mylar->GetMaterialPropertiesTable();
        get_matprop(tbl_Mylar, "ABSLENGTH", Mylar_ABS_n, Mylar_ABS_energy, Mylar_ABS_len);
        get_matprop(tbl_Mylar, "RINDEX", Mylar_RI_n, Mylar_RI_energy, Mylar_RI_idx);
        get_matprop(tbl_Mylar, "RAYLEIGH", Mylar_Rayleigh_n, Mylar_Rayleigh_energy, Mylar_Rayleigh_len);  
      }
    m_op_tree->Branch("Mylar_ABS_n", &Mylar_ABS_n, "Mylar_ABS_n/I");
    m_op_tree->Branch("Mylar_ABS_energy", Mylar_ABS_energy, "Mylar_ABS_energy[Mylar_ABS_n]/D");
    m_op_tree->Branch("Mylar_ABS_len", Mylar_ABS_len, "Mylar_ABS_len[Mylar_ABS_n]/D");
    
    m_op_tree->Branch("Mylar_RI_n", &Mylar_RI_n, "Mylar_RI_n/I");
    m_op_tree->Branch("Mylar_RI_energy", Mylar_RI_energy, "Mylar_RI_energy[Mylar_RI_n]/D");    
    m_op_tree->Branch("Mylar_RI_idx", Mylar_RI_idx, "Mylar_RI_idx[Mylar_RI_n]/D");

    m_op_tree->Branch("Mylar_Rayleigh_n", &Mylar_Rayleigh_n, "Mylar_Rayleigh_n/I");
    m_op_tree->Branch("Mylar_Rayleigh_energy", Mylar_Rayleigh_energy, "Mylar_Rayleigh_energy[Mylar_Rayleigh_n]/D");
    m_op_tree->Branch("Mylar_Rayleigh_len", Mylar_Rayleigh_len, "Mylar_Rayleigh_len[Mylar_Rayleigh_n]/D");
    //========================================
    //ETFE
    //========================================
    G4Material* mat_ETFE = G4Material::GetMaterial("ETFE");
    G4int ETFE_ABS_n = 0; G4double* ETFE_ABS_energy = 0; G4double* ETFE_ABS_len = 0;
    G4int ETFE_RI_n = 0;  G4double* ETFE_RI_energy = 0;  G4double* ETFE_RI_idx = 0;
    if (mat_ETFE) {
        G4MaterialPropertiesTable* tbl_ETFE = mat_ETFE->GetMaterialPropertiesTable();
        get_matprop(tbl_ETFE, "ABSLENGTH", ETFE_ABS_n, ETFE_ABS_energy, ETFE_ABS_len);
        get_matprop(tbl_ETFE, "RINDEX", ETFE_RI_n, ETFE_RI_energy, ETFE_RI_idx);
  }

    m_op_tree->Branch("ETFE_ABS_n", &ETFE_ABS_n, "ETFE_ABS_n/I");
    m_op_tree->Branch("ETFE_ABS_energy", ETFE_ABS_energy, "ETFE_ABS_energy[ETFE_ABS_n]/D");
    m_op_tree->Branch("ETFE_ABS_len", ETFE_ABS_len, "ETFE_ABS_len[ETFE_ABS_n]/D");

    m_op_tree->Branch("ETFE_RI_n", &ETFE_RI_n, "ETFE_RI_n/I");
    m_op_tree->Branch("ETFE_RI_energy", ETFE_RI_energy, "ETFE_RI_energy[ETFE_RI_n]/D");
    m_op_tree->Branch("ETFE_RI_idx", ETFE_RI_idx, "ETFE_RI_idx[ETFE_RI_n]/D");
    //================================
    //FEP
    //====================
    G4Material* mat_FEP = G4Material::GetMaterial("FEP");
    G4int FEP_ABS_n = 0; G4double* FEP_ABS_energy = 0; G4double* FEP_ABS_len = 0; 
    G4int FEP_RI_n = 0;  G4double* FEP_RI_energy = 0;  G4double* FEP_RI_idx = 0; 
    if (mat_FEP) {
        G4MaterialPropertiesTable* tbl_FEP = mat_FEP->GetMaterialPropertiesTable();
        get_matprop(tbl_FEP, "ABSLENGTH", FEP_ABS_n, FEP_ABS_energy, FEP_ABS_len);
        get_matprop(tbl_FEP, "RINDEX", FEP_RI_n, FEP_RI_energy, FEP_RI_idx);
    }

    m_op_tree->Branch("FEP_ABS_n", &FEP_ABS_n, "FEP_ABS_n/I");
    m_op_tree->Branch("FEP_ABS_energy", FEP_ABS_energy, "FEP_ABS_energy[FEP_ABS_n]/D");
    m_op_tree->Branch("FEP_ABS_len", FEP_ABS_len, "FEP_ABS_len[FEP_ABS_n]/D");

    m_op_tree->Branch("FEP_RI_n", &FEP_RI_n, "FEP_RI_n/I");
    m_op_tree->Branch("FEP_RI_energy", FEP_RI_energy, "FEP_RI_energy[FEP_RI_n]/D");
    m_op_tree->Branch("FEP_RI_idx", FEP_RI_idx, "FEP_RI_idx[FEP_RI_n]/D");
    //==========================================
    //PE_PA
    //==========================================
    G4Material* mat_PE_PA = G4Material::GetMaterial("PE_PA");
    G4int PE_PA_ABS_n = 0; G4double* PE_PA_ABS_energy = 0; G4double* PE_PA_ABS_len = 0;
    G4int PE_PA_RI_n = 0;  G4double* PE_PA_RI_energy = 0;  G4double* PE_PA_RI_idx = 0;
    if (mat_PE_PA) {
        G4MaterialPropertiesTable* tbl_PE_PA = mat_PE_PA->GetMaterialPropertiesTable();
        get_matprop(tbl_PE_PA, "ABSLENGTH", PE_PA_ABS_n, PE_PA_ABS_energy, PE_PA_ABS_len);
        get_matprop(tbl_PE_PA, "RINDEX", PE_PA_RI_n, PE_PA_RI_energy, PE_PA_RI_idx);
    }

    m_op_tree->Branch("PE_PA_ABS_n", &PE_PA_ABS_n, "PE_PA_ABS_n/I");
    m_op_tree->Branch("PE_PA_ABS_energy", PE_PA_ABS_energy, "PE_PA_ABS_energy[PE_PA_ABS_n]/D");
    m_op_tree->Branch("PE_PA_ABS_len", PE_PA_ABS_len, "PE_PA_ABS_len[PE_PA_ABS_n]/D");

    m_op_tree->Branch("PE_PA_RI_n", &PE_PA_RI_n, "PE_PA_RI_n/I");
    m_op_tree->Branch("PE_PA_RI_energy", PE_PA_RI_energy, "PE_PA_RI_energy[PE_PA_RI_n]/D");
    m_op_tree->Branch("PE_PA_RI_idx", PE_PA_RI_idx, "PE_PA_RI_idx[PE_PA_RI_n]/D");
    //=============================================
    //PA
    //===============================================
    G4Material* mat_PA = G4Material::GetMaterial("PA");
    G4int PA_ABS_n = 0; G4double* PA_ABS_energy = 0; G4double* PA_ABS_len = 0;
    G4int PA_RI_n = 0;  G4double* PA_RI_energy = 0;  G4double* PA_RI_idx = 0;
    if (mat_PA) {
        G4MaterialPropertiesTable* tbl_PA = mat_PA->GetMaterialPropertiesTable();
        get_matprop(tbl_PA, "ABSLENGTH", PA_ABS_n, PA_ABS_energy, PA_ABS_len);
        get_matprop(tbl_PA, "RINDEX", PA_RI_n, PA_RI_energy, PA_RI_idx);
    }

    m_op_tree->Branch("PA_ABS_n", &PA_ABS_n, "PA_ABS_n/I");
    m_op_tree->Branch("PA_ABS_energy", PA_ABS_energy, "PA_ABS_energy[PA_ABS_n]/D");
    m_op_tree->Branch("PA_ABS_len", PA_ABS_len, "PA_ABS_len[PA_ABS_n]/D");

    m_op_tree->Branch("PA_RI_n", &PA_RI_n, "PA_RI_n/I");
    m_op_tree->Branch("PA_RI_energy", PA_RI_energy, "PA_RI_energy[PA_RI_n]/D");
    m_op_tree->Branch("PA_RI_idx", PA_RI_idx, "PA_RI_idx[PA_RI_n]/D");
    //==========================================
    //Air
    //==========================================
     G4Material* mat_Air = G4Material::GetMaterial("Air");
    G4int Air_ABS_n = 0; G4double* Air_ABS_energy = 0; G4double* Air_ABS_len = 0;
    G4int Air_RI_n = 0;  G4double* Air_RI_energy = 0;  G4double* Air_RI_idx = 0;
    if (mat_Air) {
        G4MaterialPropertiesTable* tbl_Air = mat_Air->GetMaterialPropertiesTable();
        get_matprop(tbl_Air, "ABSLENGTH", Air_ABS_n, Air_ABS_energy, Air_ABS_len);
        get_matprop(tbl_Air, "RINDEX", Air_RI_n, Air_RI_energy, Air_RI_idx);
    }

    m_op_tree->Branch("Air_ABS_n", &Air_ABS_n, "Air_ABS_n/I");
    m_op_tree->Branch("Air_ABS_energy", Air_ABS_energy, "Air_ABS_energy[Air_ABS_n]/D");
    m_op_tree->Branch("Air_ABS_len", Air_ABS_len, "Air_ABS_len[Air_ABS_n]/D");

    m_op_tree->Branch("Air_RI_n", &Air_RI_n, "Air_RI_n/I");
    m_op_tree->Branch("Air_RI_energy", Air_RI_energy, "Air_RI_energy[Air_RI_n]/D");
    m_op_tree->Branch("Air_RI_idx", Air_RI_idx, "Air_RI_idx[Air_RI_n]/D");
    //=======================================
    //Vacuum
    //=======================
    G4Material* mat_Vacuum = G4Material::GetMaterial("Vacuum");
    G4int Vacuum_ABS_n = 0; G4double* Vacuum_ABS_energy = 0; G4double* Vacuum_ABS_len = 0;
    G4int Vacuum_RI_n = 0;  G4double* Vacuum_RI_energy = 0;  G4double* Vacuum_RI_idx = 0;
    if (mat_Vacuum) {
        G4MaterialPropertiesTable* tbl_Vacuum = mat_Vacuum->GetMaterialPropertiesTable();
        get_matprop(tbl_Vacuum, "ABSLENGTH", Vacuum_ABS_n, Vacuum_ABS_energy, Vacuum_ABS_len);
        get_matprop(tbl_Vacuum, "RINDEX", Vacuum_RI_n, Vacuum_RI_energy, Vacuum_RI_idx);
    }

    m_op_tree->Branch("Vacuum_ABS_n", &Vacuum_ABS_n, "Vacuum_ABS_n/I");
    m_op_tree->Branch("Vacuum_ABS_energy", Vacuum_ABS_energy, "Vacuum_ABS_energy[Vacuum_ABS_n]/D");
    m_op_tree->Branch("Vacuum_ABS_len", Vacuum_ABS_len, "Vacuum_ABS_len[Vacuum_ABS_n]/D");

    m_op_tree->Branch("Vacuum_RI_n", &Vacuum_RI_n, "Vacuum_RI_n/I");
    m_op_tree->Branch("Vacuum_RI_energy", Vacuum_RI_energy, "Vacuum_RI_energy[Vacuum_RI_n]/D");
    m_op_tree->Branch("Vacuum_RI_idx", Vacuum_RI_idx, "Vacuum_RI_idx[Vacuum_RI_n]/D");
    //============================================================
    //VacuumT
    //========================
    G4Material* mat_VacuumT = G4Material::GetMaterial("VacuumT");
    G4int VacuumT_ABS_n = 0; G4double* VacuumT_ABS_energy = 0; G4double* VacuumT_ABS_len = 0;
    G4int VacuumT_RI_n = 0;  G4double* VacuumT_RI_energy = 0;  G4double* VacuumT_RI_idx = 0;
    if (mat_VacuumT) {
        G4MaterialPropertiesTable* tbl_VacuumT = mat_VacuumT->GetMaterialPropertiesTable();
        get_matprop(tbl_VacuumT, "ABSLENGTH", VacuumT_ABS_n, VacuumT_ABS_energy, VacuumT_ABS_len);
        get_matprop(tbl_VacuumT, "RINDEX", VacuumT_RI_n, VacuumT_RI_energy, VacuumT_RI_idx);
  }

    m_op_tree->Branch("VacuumT_ABS_n", &VacuumT_ABS_n, "VacuumT_ABS_n/I");
    m_op_tree->Branch("VacuumT_ABS_energy", VacuumT_ABS_energy, "VacuumT_ABS_energy[VacuumT_ABS_n]/D");
    m_op_tree->Branch("VacuumT_ABS_len", VacuumT_ABS_len, "VacuumT_ABS_len[VacuumT_ABS_n]/D");

    m_op_tree->Branch("VacuumT_RI_n", &VacuumT_RI_n, "VacuumT_RI_n/I");
    m_op_tree->Branch("VacuumT_RI_energy", VacuumT_RI_energy, "VacuumT_RI_energy[VacuumT_RI_n]/D");
    m_op_tree->Branch("VacuumT_RI_idx", VacuumT_RI_idx, "VacuumT_RI_idx[VacuumT_RI_n]/D");
    //============================================================
    //Pyrex
    //============================================================
    G4Material* mat_Pyrex = G4Material::GetMaterial("Pyrex");
    G4int Pyrex_ABS_n = 0; G4double* Pyrex_ABS_energy = 0; G4double* Pyrex_ABS_len = 0; 
    G4int Pyrex_RI_n = 0;  G4double* Pyrex_RI_energy = 0;  G4double* Pyrex_RI_idx = 0; 
    if (mat_Pyrex) {
        G4MaterialPropertiesTable* tbl_Pyrex = mat_Pyrex->GetMaterialPropertiesTable();
        get_matprop(tbl_Pyrex, "ABSLENGTH", Pyrex_ABS_n, Pyrex_ABS_energy, Pyrex_ABS_len);
        get_matprop(tbl_Pyrex, "RINDEX", Pyrex_RI_n, Pyrex_RI_energy, Pyrex_RI_idx);
    }
    
    m_op_tree->Branch("Pyrex_ABS_n", &Pyrex_ABS_n, "Pyrex_ABS_n/I");
    m_op_tree->Branch("Pyrex_ABS_energy", Pyrex_ABS_energy, "Pyrex_ABS_energy[Pyrex_ABS_n]/D");
    m_op_tree->Branch("Pyrex_ABS_len", Pyrex_ABS_len, "Pyrex_ABS_len[Pyrex_ABS_n]/D");
    
    m_op_tree->Branch("Pyrex_RI_n", &Pyrex_RI_n, "Pyrex_RI_n/I");
    m_op_tree->Branch("Pyrex_RI_energy", Pyrex_RI_energy, "Pyrex_RI_energy[Pyrex_RI_n]/D");
    m_op_tree->Branch("Pyrex_RI_idx", Pyrex_RI_idx, "Pyrex_RI_idx[Pyrex_RI_n]/D");    




    // ======================================================================
    // * Water absorption length and refractive index.
    // ======================================================================
    G4Material* mat_Water = G4Material::GetMaterial("Water");
    G4int Water_ABS_n = 0; G4double* Water_ABS_energy = 0; G4double* Water_ABS_len = 0;
    G4int Water_RI_n = 0;  G4double* Water_RI_energy = 0;  G4double* Water_RI_idx = 0;
    if (mat_Water) {
        G4MaterialPropertiesTable* tbl_Water = mat_Water->GetMaterialPropertiesTable();
        // ABSLENGTH
        get_matprop(tbl_Water, "ABSLENGTH", Water_ABS_n, Water_ABS_energy, Water_ABS_len);
        // RINDEX
        get_matprop(tbl_Water, "RINDEX", Water_RI_n, Water_RI_energy, Water_RI_idx);
    }

    m_op_tree->Branch("Water_ABS_n", &Water_ABS_n, "Water_ABS_n/I");
    m_op_tree->Branch("Water_ABS_energy", Water_ABS_energy, "Water_ABS_energy[Water_ABS_n]/D");
    m_op_tree->Branch("Water_ABS_len", Water_ABS_len, "Water_ABS_len[Water_ABS_n]/D");

    m_op_tree->Branch("Water_RI_n", &Water_RI_n, "Water_RI_n/I");
    m_op_tree->Branch("Water_RI_energy", Water_RI_energy, "Water_RI_energy[Water_RI_n]/D");
    m_op_tree->Branch("Water_RI_idx", Water_RI_idx, "Water_RI_idx[Water_RI_n]/D");

    // ======================================================================
    // * PMT QE (photocathode)
    // * refractive index of PMT glass (Pyrex)
    // ======================================================================
    G4int PC_RI_n = 0;    G4double* PC_RI_energy = 0;    G4double* PC_RI_idx = 0;
    G4int PC_KI_n = 0;    G4double* PC_KI_energy = 0;    G4double* PC_KI_idx = 0;
    G4int PC_REF_n = 0;    G4double* PC_REF_energy = 0;    G4double* PC_REF_ref = 0;
    G4int PC_THI_n = 0;    G4double* PC_THI_posz = 0;    G4double* PC_THI_thi = 0;
    G4int PC_EFF_n = 0;    G4double* PC_EFF_energy = 0;    G4double* PC_EFF_eff = 0;
    G4Material* mat_Photocathode = G4Material::GetMaterial("photocathode");
    if (mat_Photocathode) {
        G4MaterialPropertiesTable* tbl_PC = mat_Photocathode->GetMaterialPropertiesTable();
        get_matprop(tbl_PC, "RINDEX", PC_RI_n, PC_RI_energy, PC_RI_idx);
        get_matprop(tbl_PC, "KINDEX", PC_KI_n, PC_KI_energy, PC_KI_idx);
        get_matprop(tbl_PC, "REFLECTIVITY", PC_REF_n, PC_REF_energy, PC_REF_ref);
        get_matprop(tbl_PC, "THICKNESS", PC_THI_n, PC_THI_posz, PC_THI_thi);
        get_matprop(tbl_PC, "EFFICIENCY", PC_EFF_n, PC_EFF_energy, PC_EFF_eff);
    }
    m_op_tree->Branch("PC_RI_n", &PC_RI_n, "PC_RI_n/I");
    m_op_tree->Branch("PC_RI_energy", PC_RI_energy, "PC_RI_energy[PC_RI_n]/D");
    m_op_tree->Branch("PC_RI_idx", PC_RI_idx, "PC_RI_idx[PC_RI_n]/D");
     
    m_op_tree->Branch("PC_KI_n", &PC_KI_n, "PC_KI_n/I");
    m_op_tree->Branch("PC_KI_energy", PC_KI_energy, "PC_KI_energy[PC_KI_n]/D");
    m_op_tree->Branch("PC_KI_idx", PC_KI_idx, "PC_KI_idx[PC_KI_n]/D");
 
    m_op_tree->Branch("PC_REF_n", &PC_REF_n, "PC_REF_n/I");
    m_op_tree->Branch("PC_REF_energy", PC_REF_energy, "PC_REF_energy[PC_REF_n]/D");
    m_op_tree->Branch("PC_REF_ref", PC_REF_ref, "PC_REF_ref[PC_REF_n]/D");   
    
    m_op_tree->Branch("PC_THI_n", &PC_THI_n, "PC_THI_n/I");
    m_op_tree->Branch("PC_THI_posz", PC_THI_posz, "PC_THI_posz[PC_THI_n]/D");
    m_op_tree->Branch("PC_THI_thi", PC_THI_thi, "PC_THI_thi[PC_THI_n]/D");

    m_op_tree->Branch("PC_EFF_n", &PC_EFF_n, "PC_EFF_n/I");
    m_op_tree->Branch("PC_EFF_energy", PC_EFF_energy, "PC_EFF_energy[PC_EFF_n]/D");
    m_op_tree->Branch("PC_EFF_eff", PC_EFF_eff, "PC_EFF_eff[PC_EFF_n]/D");
   
    //==============================================
    //photocathode_3inch
    //=================================================

   
    G4int PC3in_RI_n = 0;    G4double* PC3in_RI_energy = 0;    G4double* PC3in_RI_idx = 0;
    G4int PC3in_KI_n = 0;    G4double* PC3in_KI_energy = 0;    G4double* PC3in_KI_idx = 0;
    G4int PC3in_REF_n = 0;    G4double* PC3in_REF_energy = 0;    G4double* PC3in_REF_ref = 0;
    G4int PC3in_THI_n = 0;    G4double* PC3in_THI_posz = 0;    G4double* PC3in_THI_thi = 0;
    G4int PC3in_EFF_n = 0;    G4double* PC3in_EFF_energy = 0;    G4double* PC3in_EFF_eff = 0;
    G4Material* mat_photocathode_3inch = G4Material::GetMaterial("photocathode_3inch");
    if (mat_Photocathode) {
        G4MaterialPropertiesTable* tbl_PC3in = mat_photocathode_3inch->GetMaterialPropertiesTable();
        get_matprop(tbl_PC3in, "RINDEX", PC3in_RI_n, PC3in_RI_energy, PC3in_RI_idx);
        get_matprop(tbl_PC3in, "KINDEX", PC3in_KI_n, PC3in_KI_energy, PC3in_KI_idx);
        get_matprop(tbl_PC3in, "REFLECTIVITY", PC3in_REF_n, PC3in_REF_energy, PC3in_REF_ref);
        get_matprop(tbl_PC3in, "THICKNESS", PC3in_THI_n, PC3in_THI_posz, PC3in_THI_thi);
        get_matprop(tbl_PC3in, "EFFICIENCY", PC3in_EFF_n, PC3in_EFF_energy, PC3in_EFF_eff);
    }   
    m_op_tree->Branch("PC3in_RI_n", &PC3in_RI_n, "PC3in_RI_n/I");
    m_op_tree->Branch("PC3in_RI_energy", PC3in_RI_energy, "PC3in_RI_energy[PC3in_RI_n]/D");
    m_op_tree->Branch("PC3in_RI_idx", PC3in_RI_idx, "PC3in_RI_idx[PC3in_RI_n]/D");
    
    m_op_tree->Branch("PC3in_KI_n", &PC3in_KI_n, "PC3in_KI_n/I");
    m_op_tree->Branch("PC3in_KI_energy", PC3in_KI_energy, "PC3in_KI_energy[PC3in_KI_n]/D");
    m_op_tree->Branch("PC3in_KI_idx", PC3in_KI_idx, "PC3in_KI_idx[PC3in_KI_n]/D");
    
    m_op_tree->Branch("PC3in_REF_n", &PC3in_REF_n, "PC3in_REF_n/I");
    m_op_tree->Branch("PC3in_REF_energy", PC3in_REF_energy, "PC3in_REF_energy[PC3in_REF_n]/D");
    m_op_tree->Branch("PC3in_REF_ref", PC3in_REF_ref, "PC3in_REF_ref[PC3in_REF_n]/D");
    
    m_op_tree->Branch("PC3in_THI_n", &PC3in_THI_n, "PC3in_THI_n/I");
    m_op_tree->Branch("PC3in_THI_posz", PC3in_THI_posz, "PC3in_THI_posz[PC3in_THI_n]/D");
    m_op_tree->Branch("PC3in_THI_thi", PC3in_THI_thi, "PC3in_THI_thi[PC3in_THI_n]/D");
    
    m_op_tree->Branch("PC3in_EFF_n", &PC3in_EFF_n, "PC3in_EFF_n/I");
    m_op_tree->Branch("PC3in_EFF_energy", PC3in_EFF_energy, "PC3in_EFF_energy[PC3in_EFF_n]/D");
    m_op_tree->Branch("PC3in_EFF_eff", PC3in_EFF_eff, "PC3in_EFF_eff[PC3in_EFF_n]/D");
   
    //==================================
    //photocathode_MCP20inch
    //==================================


    G4int PCMCP20in_RI_n = 0;    G4double* PCMCP20in_RI_energy = 0;    G4double* PCMCP20in_RI_idx = 0;
    G4int PCMCP20in_KI_n = 0;    G4double* PCMCP20in_KI_energy = 0;    G4double* PCMCP20in_KI_idx = 0;
    G4int PCMCP20in_REF_n = 0;    G4double* PCMCP20in_REF_energy = 0;    G4double* PCMCP20in_REF_ref = 0;
    G4int PCMCP20in_THI_n = 0;    G4double* PCMCP20in_THI_posz = 0;    G4double* PCMCP20in_THI_thi = 0;
    G4int PCMCP20in_EFF_n = 0;    G4double* PCMCP20in_EFF_energy = 0;    G4double* PCMCP20in_EFF_eff = 0;
    G4Material* mat_photocathode_MCP20inch = G4Material::GetMaterial("photocathode_MCP20inch");
    if (mat_Photocathode) {
        G4MaterialPropertiesTable* tbl_PCMCP20in = mat_photocathode_MCP20inch->GetMaterialPropertiesTable();
        get_matprop(tbl_PCMCP20in, "RINDEX", PCMCP20in_RI_n, PCMCP20in_RI_energy, PCMCP20in_RI_idx);
        get_matprop(tbl_PCMCP20in, "KINDEX", PCMCP20in_KI_n, PCMCP20in_KI_energy, PCMCP20in_KI_idx);
        get_matprop(tbl_PCMCP20in, "REFLECTIVITY", PCMCP20in_REF_n, PCMCP20in_REF_energy, PCMCP20in_REF_ref);
        get_matprop(tbl_PCMCP20in, "THICKNESS", PCMCP20in_THI_n, PCMCP20in_THI_posz, PCMCP20in_THI_thi);
        get_matprop(tbl_PCMCP20in, "EFFICIENCY", PCMCP20in_EFF_n, PCMCP20in_EFF_energy, PCMCP20in_EFF_eff);
    }
    m_op_tree->Branch("PCMCP20in_RI_n", &PCMCP20in_RI_n, "PCMCP20in_RI_n/I");
    m_op_tree->Branch("PCMCP20in_RI_energy", PCMCP20in_RI_energy, "PCMCP20in_RI_energy[PCMCP20in_RI_n]/D");
    m_op_tree->Branch("PCMCP20in_RI_idx", PCMCP20in_RI_idx, "PCMCP20in_RI_idx[PCMCP20in_RI_n]/D");

    m_op_tree->Branch("PCMCP20in_KI_n", &PCMCP20in_KI_n, "PCMCP20in_KI_n/I");
    m_op_tree->Branch("PCMCP20in_KI_energy", PCMCP20in_KI_energy, "PCMCP20in_KI_energy[PCMCP20in_KI_n]/D");
    m_op_tree->Branch("PCMCP20in_KI_idx", PCMCP20in_KI_idx, "PCMCP20in_KI_idx[PCMCP20in_KI_n]/D");

    m_op_tree->Branch("PCMCP20in_REF_n", &PCMCP20in_REF_n, "PCMCP20in_REF_n/I");
    m_op_tree->Branch("PCMCP20in_REF_energy", PCMCP20in_REF_energy, "PCMCP20in_REF_energy[PCMCP20in_REF_n]/D");
    m_op_tree->Branch("PCMCP20in_REF_ref", PCMCP20in_REF_ref, "PCMCP20in_REF_ref[PCMCP20in_REF_n]/D");

    m_op_tree->Branch("PCMCP20in_THI_n", &PCMCP20in_THI_n, "PCMCP20in_THI_n/I");
    m_op_tree->Branch("PCMCP20in_THI_posz", PCMCP20in_THI_posz, "PCMCP20in_THI_posz[PCMCP20in_THI_n]/D");
    m_op_tree->Branch("PCMCP20in_THI_thi", PCMCP20in_THI_thi, "PCMCP20in_THI_thi[PCMCP20in_THI_n]/D");

    m_op_tree->Branch("PCMCP20in_EFF_n", &PCMCP20in_EFF_n, "PCMCP20in_EFF_n/I");
    m_op_tree->Branch("PCMCP20in_EFF_energy", PCMCP20in_EFF_energy, "PCMCP20in_EFF_energy[PCMCP20in_EFF_n]/D");
    m_op_tree->Branch("PCMCP20in_EFF_eff", PCMCP20in_EFF_eff, "PCMCP20in_EFF_eff[PCMCP20in_EFF_n]/D");

    //===========================
    //photocathode_MCP8inch
    //===========================

    G4int PCMCP8in_RI_n = 0;    G4double* PCMCP8in_RI_energy = 0;    G4double* PCMCP8in_RI_idx = 0;
    G4int PCMCP8in_KI_n = 0;    G4double* PCMCP8in_KI_energy = 0;    G4double* PCMCP8in_KI_idx = 0;
    G4int PCMCP8in_REF_n = 0;    G4double* PCMCP8in_REF_energy = 0;    G4double* PCMCP8in_REF_ref = 0;
    G4int PCMCP8in_THI_n = 0;    G4double* PCMCP8in_THI_posz = 0;    G4double* PCMCP8in_THI_thi = 0;
    G4int PCMCP8in_EFF_n = 0;    G4double* PCMCP8in_EFF_energy = 0;    G4double* PCMCP8in_EFF_eff = 0;
    G4Material* mat_photocathode_MCP8inch = G4Material::GetMaterial("photocathode_MCP8inch");
    if (mat_Photocathode) {
        G4MaterialPropertiesTable* tbl_PCMCP8in = mat_photocathode_MCP8inch->GetMaterialPropertiesTable();
        get_matprop(tbl_PCMCP8in, "RINDEX", PCMCP8in_RI_n, PCMCP8in_RI_energy, PCMCP8in_RI_idx);
        get_matprop(tbl_PCMCP8in, "KINDEX", PCMCP8in_KI_n, PCMCP8in_KI_energy, PCMCP8in_KI_idx);
        get_matprop(tbl_PCMCP8in, "REFLECTIVITY", PCMCP8in_REF_n, PCMCP8in_REF_energy, PCMCP8in_REF_ref);
        get_matprop(tbl_PCMCP8in, "THICKNESS", PCMCP8in_THI_n, PCMCP8in_THI_posz, PCMCP8in_THI_thi);
        get_matprop(tbl_PCMCP8in, "EFFICIENCY", PCMCP8in_EFF_n, PCMCP8in_EFF_energy, PCMCP8in_EFF_eff);
    }
    m_op_tree->Branch("PCMCP8in_RI_n", &PCMCP8in_RI_n, "PCMCP8in_RI_n/I");
    m_op_tree->Branch("PCMCP8in_RI_energy", PCMCP8in_RI_energy, "PCMCP8in_RI_energy[PCMCP8in_RI_n]/D");
    m_op_tree->Branch("PCMCP8in_RI_idx", PCMCP8in_RI_idx, "PCMCP8in_RI_idx[PCMCP8in_RI_n]/D");

    m_op_tree->Branch("PCMCP8in_KI_n", &PCMCP8in_KI_n, "PCMCP8in_KI_n/I");
    m_op_tree->Branch("PCMCP8in_KI_energy", PCMCP8in_KI_energy, "PCMCP8in_KI_energy[PCMCP8in_KI_n]/D");
    m_op_tree->Branch("PCMCP8in_KI_idx", PCMCP8in_KI_idx, "PCMCP8in_KI_idx[PCMCP8in_KI_n]/D");

    m_op_tree->Branch("PCMCP8in_REF_n", &PCMCP8in_REF_n, "PCMCP8in_REF_n/I");
    m_op_tree->Branch("PCMCP8in_REF_energy", PCMCP8in_REF_energy, "PCMCP8in_REF_energy[PCMCP8in_REF_n]/D");
    m_op_tree->Branch("PCMCP8in_REF_ref", PCMCP8in_REF_ref, "PCMCP8in_REF_ref[PCMCP8in_REF_n]/D");

    m_op_tree->Branch("PCMCP8in_THI_n", &PCMCP8in_THI_n, "PCMCP8in_THI_n/I");
    m_op_tree->Branch("PCMCP8in_THI_posz", PCMCP8in_THI_posz, "PCMCP8in_THI_posz[PCMCP8in_THI_n]/D");
    m_op_tree->Branch("PCMCP8in_THI_thi", PCMCP8in_THI_thi, "PCMCP8in_THI_thi[PCMCP8in_THI_n]/D");

    m_op_tree->Branch("PCMCP8in_EFF_n", &PCMCP8in_EFF_n, "PCMCP8in_EFF_n/I");
    m_op_tree->Branch("PCMCP8in_EFF_energy", PCMCP8in_EFF_energy, "PCMCP8in_EFF_energy[PCMCP8in_EFF_n]/D");
    m_op_tree->Branch("PCMCP8in_EFF_eff", PCMCP8in_EFF_eff, "PCMCP8in_EFF_eff[PCMCP8in_EFF_n]/D");
  
    //=============================
    //photocathode_Ham20inch
    //============================

    G4int PCHam20in_RI_n = 0;    G4double* PCHam20in_RI_energy = 0;    G4double* PCHam20in_RI_idx = 0;
    G4int PCHam20in_KI_n = 0;    G4double* PCHam20in_KI_energy = 0;    G4double* PCHam20in_KI_idx = 0;
    G4int PCHam20in_REF_n = 0;    G4double* PCHam20in_REF_energy = 0;    G4double* PCHam20in_REF_ref = 0;
    G4int PCHam20in_THI_n = 0;    G4double* PCHam20in_THI_posz = 0;    G4double* PCHam20in_THI_thi = 0;
    G4int PCHam20in_EFF_n = 0;    G4double* PCHam20in_EFF_energy = 0;    G4double* PCHam20in_EFF_eff = 0;
    G4Material* mat_photocathode_Ham20inch = G4Material::GetMaterial("photocathode_Ham20inch");
    if (mat_Photocathode) {
        G4MaterialPropertiesTable* tbl_PCHam20in = mat_photocathode_Ham20inch->GetMaterialPropertiesTable();
        get_matprop(tbl_PCHam20in, "RINDEX", PCHam20in_RI_n, PCHam20in_RI_energy, PCHam20in_RI_idx);
        get_matprop(tbl_PCHam20in, "KINDEX", PCHam20in_KI_n, PCHam20in_KI_energy, PCHam20in_KI_idx);
        get_matprop(tbl_PCHam20in, "REFLECTIVITY", PCHam20in_REF_n, PCHam20in_REF_energy, PCHam20in_REF_ref);
        get_matprop(tbl_PCHam20in, "THICKNESS", PCHam20in_THI_n, PCHam20in_THI_posz, PCHam20in_THI_thi);
        get_matprop(tbl_PCHam20in, "EFFICIENCY", PCHam20in_EFF_n, PCHam20in_EFF_energy, PCHam20in_EFF_eff);
    }
    m_op_tree->Branch("PCHam20in_RI_n", &PCHam20in_RI_n, "PCHam20in_RI_n/I");
    m_op_tree->Branch("PCHam20in_RI_energy", PCHam20in_RI_energy, "PCHam20in_RI_energy[PCHam20in_RI_n]/D");
    m_op_tree->Branch("PCHam20in_RI_idx", PCHam20in_RI_idx, "PCHam20in_RI_idx[PCHam20in_RI_n]/D");

    m_op_tree->Branch("PCHam20in_KI_n", &PCHam20in_KI_n, "PCHam20in_KI_n/I");
    m_op_tree->Branch("PCHam20in_KI_energy", PCHam20in_KI_energy, "PCHam20in_KI_energy[PCHam20in_KI_n]/D");
    m_op_tree->Branch("PCHam20in_KI_idx", PCHam20in_KI_idx, "PCHam20in_KI_idx[PCHam20in_KI_n]/D");

    m_op_tree->Branch("PCHam20in_REF_n", &PCHam20in_REF_n, "PCHam20in_REF_n/I");
    m_op_tree->Branch("PCHam20in_REF_energy", PCHam20in_REF_energy, "PCHam20in_REF_energy[PCHam20in_REF_n]/D");
    m_op_tree->Branch("PCHam20in_REF_ref", PCHam20in_REF_ref, "PCHam20in_REF_ref[PCHam20in_REF_n]/D");

    m_op_tree->Branch("PCHam20in_THI_n", &PCHam20in_THI_n, "PCHam20in_THI_n/I");
    m_op_tree->Branch("PCHam20in_THI_posz", PCHam20in_THI_posz, "PCHam20in_THI_posz[PCHam20in_THI_n]/D");
    m_op_tree->Branch("PCHam20in_THI_thi", PCHam20in_THI_thi, "PCHam20in_THI_thi[PCHam20in_THI_n]/D");

    m_op_tree->Branch("PCHam20in_EFF_n", &PCHam20in_EFF_n, "PCHam20in_EFF_n/I");
    m_op_tree->Branch("PCHam20in_EFF_energy", PCHam20in_EFF_energy, "PCHam20in_EFF_energy[PCHam20in_EFF_n]/D");
    m_op_tree->Branch("PCHam20in_EFF_eff", PCHam20in_EFF_eff, "PCHam20in_EFF_eff[PCHam20in_EFF_n]/D");
 
    //============================
    //photocathode_Ham8inch
    //============================

    G4int PCHam8in_RI_n = 0;    G4double* PCHam8in_RI_energy = 0;    G4double* PCHam8in_RI_idx = 0;
    G4int PCHam8in_KI_n = 0;    G4double* PCHam8in_KI_energy = 0;    G4double* PCHam8in_KI_idx = 0;
    G4int PCHam8in_REF_n = 0;    G4double* PCHam8in_REF_energy = 0;    G4double* PCHam8in_REF_ref = 0;
    G4int PCHam8in_THI_n = 0;    G4double* PCHam8in_THI_posz = 0;    G4double* PCHam8in_THI_thi = 0;
    G4int PCHam8in_EFF_n = 0;    G4double* PCHam8in_EFF_energy = 0;    G4double* PCHam8in_EFF_eff = 0;
    G4Material* mat_photocathode_Ham8inch = G4Material::GetMaterial("photocathode_Ham8inch");
    if (mat_Photocathode) {
        G4MaterialPropertiesTable* tbl_PCHam8in = mat_photocathode_Ham8inch->GetMaterialPropertiesTable();
        get_matprop(tbl_PCHam8in, "RINDEX", PCHam8in_RI_n, PCHam8in_RI_energy, PCHam8in_RI_idx);
        get_matprop(tbl_PCHam8in, "KINDEX", PCHam8in_KI_n, PCHam8in_KI_energy, PCHam8in_KI_idx);
        get_matprop(tbl_PCHam8in, "REFLECTIVITY", PCHam8in_REF_n, PCHam8in_REF_energy, PCHam8in_REF_ref);
        get_matprop(tbl_PCHam8in, "THICKNESS", PCHam8in_THI_n, PCHam8in_THI_posz, PCHam8in_THI_thi);
        get_matprop(tbl_PCHam8in, "EFFICIENCY", PCHam8in_EFF_n, PCHam8in_EFF_energy, PCHam8in_EFF_eff);
    }
    m_op_tree->Branch("PCHam8in_RI_n", &PCHam8in_RI_n, "PCHam8in_RI_n/I");
    m_op_tree->Branch("PCHam8in_RI_energy", PCHam8in_RI_energy, "PCHam8in_RI_energy[PCHam8in_RI_n]/D");
    m_op_tree->Branch("PCHam8in_RI_idx", PCHam8in_RI_idx, "PCHam8in_RI_idx[PCHam8in_RI_n]/D");

    m_op_tree->Branch("PCHam8in_KI_n", &PCHam8in_KI_n, "PCHam8in_KI_n/I");
    m_op_tree->Branch("PCHam8in_KI_energy", PCHam8in_KI_energy, "PCHam8in_KI_energy[PCHam8in_KI_n]/D");
    m_op_tree->Branch("PCHam8in_KI_idx", PCHam8in_KI_idx, "PCHam8in_KI_idx[PCHam8in_KI_n]/D");

    m_op_tree->Branch("PCHam8in_REF_n", &PCHam8in_REF_n, "PCHam8in_REF_n/I");
    m_op_tree->Branch("PCHam8in_REF_energy", PCHam8in_REF_energy, "PCHam8in_REF_energy[PCHam8in_REF_n]/D");
    m_op_tree->Branch("PCHam8in_REF_ref", PCHam8in_REF_ref, "PCHam8in_REF_ref[PCHam8in_REF_n]/D");

    m_op_tree->Branch("PCHam8in_THI_n", &PCHam8in_THI_n, "PCHam8in_THI_n/I");
    m_op_tree->Branch("PCHam8in_THI_posz", PCHam8in_THI_posz, "PCHam8in_THI_posz[PCHam8in_THI_n]/D");
    m_op_tree->Branch("PCHam8in_THI_thi", PCHam8in_THI_thi, "PCHam8in_THI_thi[PCHam8in_THI_n]/D");

    m_op_tree->Branch("PCHam8in_EFF_n", &PCHam8in_EFF_n, "PCHam8in_EFF_n/I");
    m_op_tree->Branch("PCHam8in_EFF_energy", PCHam8in_EFF_energy, "PCHam8in_EFF_energy[PCHam8in_EFF_n]/D");
    m_op_tree->Branch("PCHam8in_EFF_eff", PCHam8in_EFF_eff, "PCHam8in_EFF_eff[PCHam8in_EFF_n]/D");
 
    //=============================
    //photocathode_HZC9inch
    //==============================

    G4int PCHZC9in_RI_n = 0;    G4double* PCHZC9in_RI_energy = 0;    G4double* PCHZC9in_RI_idx = 0;
    G4int PCHZC9in_KI_n = 0;    G4double* PCHZC9in_KI_energy = 0;    G4double* PCHZC9in_KI_idx = 0;
    G4int PCHZC9in_REF_n = 0;    G4double* PCHZC9in_REF_energy = 0;    G4double* PCHZC9in_REF_ref = 0;
    G4int PCHZC9in_THI_n = 0;    G4double* PCHZC9in_THI_posz = 0;    G4double* PCHZC9in_THI_thi = 0;
    G4int PCHZC9in_EFF_n = 0;    G4double* PCHZC9in_EFF_energy = 0;    G4double* PCHZC9in_EFF_eff = 0;
    G4Material* mat_photocathode_HZC9inch = G4Material::GetMaterial("photocathode_HZC9inch");
    if (mat_Photocathode) {
        G4MaterialPropertiesTable* tbl_PCHZC9in = mat_photocathode_HZC9inch->GetMaterialPropertiesTable();
        get_matprop(tbl_PCHZC9in, "RINDEX", PCHZC9in_RI_n, PCHZC9in_RI_energy, PCHZC9in_RI_idx);
        get_matprop(tbl_PCHZC9in, "KINDEX", PCHZC9in_KI_n, PCHZC9in_KI_energy, PCHZC9in_KI_idx);
        get_matprop(tbl_PCHZC9in, "REFLECTIVITY", PCHZC9in_REF_n, PCHZC9in_REF_energy, PCHZC9in_REF_ref);
        get_matprop(tbl_PCHZC9in, "THICKNESS", PCHZC9in_THI_n, PCHZC9in_THI_posz, PCHZC9in_THI_thi);
        get_matprop(tbl_PCHZC9in, "EFFICIENCY", PCHZC9in_EFF_n, PCHZC9in_EFF_energy, PCHZC9in_EFF_eff);
    }
    m_op_tree->Branch("PCHZC9in_RI_n", &PCHZC9in_RI_n, "PCHZC9in_RI_n/I");
    m_op_tree->Branch("PCHZC9in_RI_energy", PCHZC9in_RI_energy, "PCHZC9in_RI_energy[PCHZC9in_RI_n]/D");
    m_op_tree->Branch("PCHZC9in_RI_idx", PCHZC9in_RI_idx, "PCHZC9in_RI_idx[PCHZC9in_RI_n]/D");

    m_op_tree->Branch("PCHZC9in_KI_n", &PCHZC9in_KI_n, "PCHZC9in_KI_n/I");
    m_op_tree->Branch("PCHZC9in_KI_energy", PCHZC9in_KI_energy, "PCHZC9in_KI_energy[PCHZC9in_KI_n]/D");
    m_op_tree->Branch("PCHZC9in_KI_idx", PCHZC9in_KI_idx, "PCHZC9in_KI_idx[PCHZC9in_KI_n]/D");

    m_op_tree->Branch("PCHZC9in_REF_n", &PCHZC9in_REF_n, "PCHZC9in_REF_n/I");
    m_op_tree->Branch("PCHZC9in_REF_energy", PCHZC9in_REF_energy, "PCHZC9in_REF_energy[PCHZC9in_REF_n]/D");
    m_op_tree->Branch("PCHZC9in_REF_ref", PCHZC9in_REF_ref, "PCHZC9in_REF_ref[PCHZC9in_REF_n]/D");

    m_op_tree->Branch("PCHZC9in_THI_n", &PCHZC9in_THI_n, "PCHZC9in_THI_n/I");
    m_op_tree->Branch("PCHZC9in_THI_posz", PCHZC9in_THI_posz, "PCHZC9in_THI_posz[PCHZC9in_THI_n]/D");
    m_op_tree->Branch("PCHZC9in_THI_thi", PCHZC9in_THI_thi, "PCHZC9in_THI_thi[PCHZC9in_THI_n]/D");

    m_op_tree->Branch("PCHZC9in_EFF_n", &PCHZC9in_EFF_n, "PCHZC9in_EFF_n/I");
    m_op_tree->Branch("PCHZC9in_EFF_energy", PCHZC9in_EFF_energy, "PCHZC9in_EFF_energy[PCHZC9in_EFF_n]/D");
    m_op_tree->Branch("PCHZC9in_EFF_eff", PCHZC9in_EFF_eff, "PCHZC9in_EFF_eff[PCHZC9in_EFF_n]/D");


    // Finale
    m_op_tree->Fill();
}

void
OpticalParameterAnaMgr::EndOfRunAction(const G4Run* /*run*/) {

}

void
OpticalParameterAnaMgr::BeginOfEventAction(const G4Event* /*event*/) {

}

void
OpticalParameterAnaMgr::EndOfEventAction(const G4Event* /*event*/) {

}

void
OpticalParameterAnaMgr::get_matprop(const G4MaterialPropertyVector* propvec,
        int& N, double*& x, double*& y) {
    if (!propvec) {
        N = 0;
        x = 0;
        y = 0;
        return;
    }
    int entries = propvec->GetVectorLength();
    N = entries;
    x = new double[entries];
    y = new double[entries];
    for (int i = 0; i < entries; ++i) {
        x[i] = propvec->Energy(i);
        y[i] = (*propvec)[i];
    }

}

void
OpticalParameterAnaMgr::get_matprop(G4MaterialPropertiesTable* table, 
        const char* label, int& N, double*& x, double*& y) {
    G4MaterialPropertyVector* propvec = NULL;
    propvec = table->GetProperty(label);
    if(propvec){
    return get_matprop(propvec, N, x, y);
    }
    else
    {
      N = 1 ;
      x = new double[1];
      y = new double[1];
      x[0] = 1;
      y[0] = table->GetConstProperty(label);    
    }
}
