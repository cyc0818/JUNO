#include "HBeamConstruction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Polyhedra.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "RoundBottomFlaskSolidMaker.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "DetSimAlg/DetSimAlg.h"
#include "DetSimAlg/IDetElement.h"
using namespace CLHEP;
DECLARE_TOOL(HBeamConstruction);

struct HBeamParam {
    std::string name;
    double H;
    double B;
    double tw;
    double tf;
};

static const HBeamParam hbeam_models[] = {
    {"BEGIN",  0,   0,  0,  0},
    {"GLw1", 500, 300, 12, 16},
    {"GLw2", 500, 300, 16, 25},
    {"GLw3", 500, 300, 16, 30},
    {"GLb1", 400, 200,  8, 12},
    {"GLb2", 400, 200, 12, 20},
    {"GLb3", 500, 300, 16, 25},
    {"GLb4", 500, 300, 16, 30},
    {"GLc1", 600, 300, 30, 30},
    {"GZ1",  300, 300, 12, 16},
    {"ZC1",  200, 200,  8,  8},
    {"ZC2",  160, 160,  8,  8},
    {"END",    0,   0,  0,  0}
};

struct ShellData {
    std::string name; // model.label
    double theta;     // theta (deg, NOTE: *deg when use it.)
};

static const double shell_inner_r = 20050.*mm;

// layer (angle)
static const ShellData shell_layer_angle[] = {
    {"BEGIN", 0},
    {"GLw1.up10_up11", 6.0}, 
    {"GLw1.up09_up10", 6.2}, 
    {"GLw1.up08_up09", 7.0}, 
    {"GLw1.up07_up08", 7.6}, 
    {"GLw1.up06_up07", 7.8}, 
    {"GLw1.up05_up06", 8.0}, 
    {"GLw1.up04_up05", 8.2}, 
    {"GLw1.up03_up04", 8.4}, 
    {"GLw1.up02_up03", 8.4}, 
    {"GLw1.up01_up02", 8.4}, 
    {"GLw2.equ_up01",  8.4}, 
    {"GLw2.equ_bt01",  8.4}, 
    {"GLw3.bt01_bt02", 8.4}, 
    {"GLw3.bt02_bt03", 8.4}, 
    {"GLw2.bt03_bt04", 8.4}, 
    {"GLw2.bt04_bt05", 8.2}, 
    {"GLw1.bt05_bt06", 8.0}, 
    {"GLw1.bt06_bt07", 7.8}, 
    {"GLw1.bt07_bt08", 7.6}, 
    {"GLw1.bt08_bt09", 7.0}, 
    {"GLw1.bt09_bt10", 6.2}, 
    {"GLw1.bt10_bt11", 6.0}, 
    {"END", 0}
};

// layer
static const ShellData shell_data_layer[] = {
    {"BEGIN", 0},
    {"GLb3.up11", 90.-8.4-8.4-8.4-8.4-8.2-8.0-7.8-7.6-7.0-6.2-6.0},
    {"GLb4.up10", 90.-8.4-8.4-8.4-8.4-8.2-8.0-7.8-7.6-7.0-6.2},
    {"GLb3.up09", 90.-8.4-8.4-8.4-8.4-8.2-8.0-7.8-7.6-7.0},
    {"GLb2.up08", 90.-8.4-8.4-8.4-8.4-8.2-8.0-7.8-7.6},
    {"GLb2.up07", 90.-8.4-8.4-8.4-8.4-8.2-8.0-7.8},
    {"GLb2.up06", 90.-8.4-8.4-8.4-8.4-8.2-8.0},
    {"GLb1.up05", 90.-8.4-8.4-8.4-8.4-8.2},
    {"GLb1.up04", 90.-8.4-8.4-8.4-8.4},
    {"GLb1.up03", 90.-8.4-8.4-8.4},
    {"GLb1.up02", 90.-8.4-8.4},
    {"GLb1.up01", 90.-8.4},
    {"GLb2.equ", 90}, // equator
    {"GLb2.bt01", 90.+8.4}, 
    {"GLb1.bt02", 90.+8.4+8.4}, 
    {"GLb2.bt03", 90.+8.4+8.4+8.4}, 
    {"GLb2.bt04", 90.+8.4+8.4+8.4+8.4}, // FIXME
    {"GLb1.bt05", 90.+8.4+8.4+8.4+8.4+8.2}, 
    {"GLb1.bt06", 90.+8.4+8.4+8.4+8.4+8.2+8.0}, 
    {"GLb1.bt07", 90.+8.4+8.4+8.4+8.4+8.2+8.0+7.8}, 
    {"GLb1.bt08", 90.+8.4+8.4+8.4+8.4+8.2+8.0+7.8+7.6}, 
    {"GLb3.bt09", 90.+8.4+8.4+8.4+8.4+8.2+8.0+7.8+7.6+7.0}, 
    {"GLb3.bt10", 90.+8.4+8.4+8.4+8.4+8.2+8.0+7.8+7.6+7.0+6.2}, 
    {"GLb3.bt11", 90.+8.4+8.4+8.4+8.4+8.2+8.0+7.8+7.6+7.0+6.2+6.0}, 
    {"END", 180}
};

// column (between two layer)
static const ShellData shell_data_col[] = {
    {"BEGIN", 0},
    {"GLw1.up10_up11", 90.-8.4-8.4-8.4-8.4-8.2-8.0-7.8-7.6-7.0-6.2-6.0/2},
    {"GLw1.up09_up10", 90.-8.4-8.4-8.4-8.4-8.2-8.0-7.8-7.6-7.0-6.2/2},
    {"GLw1.up08_up09", 90.-8.4-8.4-8.4-8.4-8.2-8.0-7.8-7.6-7.0/2},
    {"GLw1.up07_up08", 90.-8.4-8.4-8.4-8.4-8.2-8.0-7.8-7.6/2},
    {"GLw1.up06_up07", 90.-8.4-8.4-8.4-8.4-8.2-8.0-7.8/2},
    {"GLw1.up05_up06", 90.-8.4-8.4-8.4-8.4-8.2-8.0/2},
    {"GLw1.up04_up05", 90.-8.4-8.4-8.4-8.4-8.2/2},
    {"GLw1.up03_up04", 90.-8.4-8.4-8.4-8.4/2},
    {"GLw1.up02_up03", 90.-8.4-8.4-8.4/2},
    {"GLw1.up01_up02", 90.-8.4-8.4/2},
    {"GLw2.equ_up01",  90.-8.4/2}, 
    {"GLw2.equ_bt01",  90.+8.4/2}, 
    {"GLw3.bt01_bt02", 90.+8.4+8.4/2}, 
    {"GLw3.bt02_bt03", 90.+8.4+8.4+8.4/2}, 
    {"GLw2.bt03_bt04", 90.+8.4+8.4+8.4+8.4/2}, // FIXME
    {"GLw2.bt04_bt05", 90.+8.4+8.4+8.4+8.4+8.2/2}, 
    {"GLw1.bt05_bt06", 90.+8.4+8.4+8.4+8.4+8.2+8.0/2}, 
    {"GLw1.bt06_bt07", 90.+8.4+8.4+8.4+8.4+8.2+8.0+7.8/2}, 
    {"GLw1.bt07_bt08", 90.+8.4+8.4+8.4+8.4+8.2+8.0+7.8+7.6/2}, 
    {"GLw1.bt08_bt09", 90.+8.4+8.4+8.4+8.4+8.2+8.0+7.8+7.6+7.0/2}, 
    {"GLw1.bt09_bt10", 90.+8.4+8.4+8.4+8.4+8.2+8.0+7.8+7.6+7.0+6.2/2}, 
    {"GLw1.bt10_bt11", 90.+8.4+8.4+8.4+8.4+8.2+8.0+7.8+7.6+7.0+6.2+6.0/2}, 
    {"END", 180}
};

HBeamConstruction::HBeamConstruction(const std::string& name)
    : ToolBase(name), m_hbeam_lv(0), Steel(0), m_name(name)
    , m_radial(0), m_z(0)
{
    try_init_model(name);
    declProp("H", m_H);
    declProp("B", m_B);
    declProp("tw", m_tw);
    declProp("tf", m_tf);
    declProp("L", m_L);
    declProp("theta", m_theta);
    declProp("direct_theta", m_direct_theta);
    declProp("radial", m_radial);
    declProp("z", m_z);
}

HBeamConstruction::~HBeamConstruction() {

}

G4LogicalVolume*
HBeamConstruction::getLV() {
    if (!m_hbeam_lv) {
        makeIt();
    }
    return m_hbeam_lv;
}

bool
HBeamConstruction::inject(std::string /*motherName*/, IDetElement* /*other*/, IDetElementPos* /*pos*/) {
    return false;
}


double
HBeamConstruction::geom_info(const std::string& param) {
    if (param == "H") {
        return m_H;
    } else if (param == "B") {
        return m_B;
    } else if (param == "tw") {
        return m_tw;
    } else if (param == "tf") {
        return m_tf;
    } else if (param == "L") {
        return m_L;
    } else if (param == "theta") {
        return m_theta;
    } else if (param == "direct_theta") {
        return m_direct_theta;
    } else if (param == "R") {
        return m_radial;
    } else if (param == "z") {
        return m_z;
    } else {
        LogError << "Unknown parameter: " << param << std::endl;
    }
    return 0.0;
}

void
HBeamConstruction::makeIt() {
    LogInfo << "HBeam LV constructing" << std::endl;
    // * PART Flange I
    // * PART Web
    // * PART Flange II
    G4Box* sFlangeI =  new G4Box( m_name+"_FlangeI",  m_B/2,  m_L/2, m_tf/2);
    G4Box* sWeb =      new G4Box( m_name+"_Web",      m_tw/2, m_L/2, (m_H-2*m_tf)/2);
    G4Box* sFlangeII = new G4Box( m_name+"_FlangeII", m_B/2,  m_L/2, m_tf/2);

    G4UnionSolid* sFlangeI_Web = new G4UnionSolid(
        m_name+"_FlangeI_Web", sWeb, sFlangeI, 0, 
        G4ThreeVector(0, 0, (m_H-m_tf)/2)
        );

    G4UnionSolid* sFlangeI_Web_FlangeII = new G4UnionSolid(
        m_name+"_FlangeI_Web_FlangeII", sFlangeI_Web, sFlangeII, 0,
        G4ThreeVector(0, 0, -(m_H-m_tf)/2)
        );

    G4VSolid* sHBeam = sFlangeI_Web_FlangeII;

    // Material
    Steel = G4Material::GetMaterial("Steel");

    // Logical Volume
    m_hbeam_lv = new G4LogicalVolume(sHBeam,
                                     Steel,
                                     m_name+"_HBeam",
                                     0,
                                     0,
                                     0);
    LogInfo << "HBeam LV done" << std::endl;

    G4VisAttributes* visatt = new G4VisAttributes(G4Colour(0.5, 0.8, 0));
    visatt -> SetForceSolid(true);
    m_hbeam_lv -> SetVisAttributes(visatt);
}

void
HBeamConstruction::try_init_model(const std::string& name) {
    std::string model_name = name;
    std::string label_name = name; // label name could be used to 
                                   // initialize L in the future
    std::string::size_type pseg = name.find('.');
    if ( pseg != std::string::npos ) {
        model_name = model_name.substr(0, pseg);
        label_name = label_name.substr(pseg+1, std::string::npos);
    }

    // search in hbeam_models
    const HBeamParam* p_result = 0;
    const HBeamParam* p = hbeam_models;

    while(true) {
        if (p->name == "END") {
            break;
        }
        if (p->name == model_name) {
            p_result = p;
            break;
        }
        ++p;
    }
    if (!p_result) {
        LogError << "Unknown HBeam model " << model_name << ". "
                 << "Please register your model in file " << __FILE__ << std::endl;
        assert(0);
    }

    LogInfo << "Initialize model " << model_name << std::endl;
    m_H = p_result->H;
    m_B = p_result->B;
    m_tw = p_result->tw;
    m_tf = p_result->tf;

    // FIXME: In the future, L could be also a map.

    if (model_name.find("GL")==0) {
        try_init_model_shell(name);
    } else if (model_name.find("GZ")==0 or model_name.find("ZC")==0) {
        try_init_model_pillar(name);
    } else {
        LogError << "Unknown HBeam model prefix: "  << model_name << ". "
                 << "Please register your model in file " << __FILE__ << std::endl;
        assert(0);
    }
}

void
HBeamConstruction::try_init_model_shell(const std::string& name) {

    // calculate L
    // if name contains '_', use different calculation

    std::string::size_type pseg = name.find('_');
    bool isCol = true;
    const ShellData* p_shell = 0;
    const ShellData* p_shell_begin = 0;
    if ( pseg != std::string::npos ) {
        // column
        p_shell_begin = shell_layer_angle;
        isCol = true;
    } else {
        p_shell_begin = shell_data_layer;
        isCol = false;
    }

    for (const ShellData* p=p_shell_begin; p->name!="END"; ++p) {
        if (p->name==name) {
            p_shell = p;
            break;
        }
    }
    if (!p_shell) {
        LogError << "Unknown HBeam part " << name << ". "
                 << "Please register your part in file " << __FILE__ << std::endl;
        assert(0);
    }
    if (isCol) {
        m_L = 2*shell_inner_r*sin(0.5*p_shell->theta*deg);
        for (const ShellData* p=shell_data_col; p->name!="END"; ++p) {
            if (p->name==name) { m_theta = p->theta*deg; }
        }
    } else {
        m_L = 2*shell_inner_r*sin(p_shell->theta*deg)*sin(0.5*12*deg);
        m_theta = p_shell->theta*deg;
    }
}

// ===========================================================================
// PILLAR
// ===========================================================================

struct PillarData {
    std::string name; // model.label
    double theta; // rotation?
    double direct_theta; // without rotate 90deg. rotate, y axis
    double L;     // mm
    // others?
    double radial;
    double z;
};

static const PillarData pillar_data[] = {
    {"BEGIN", 0, 0, 0, 0, 0},

    {"GZ1.A01_02", atan(1108.831/2885.), 0., 3090.749, 20200.863-1108.831/2, -6099-2885./2},
    {"GZ1.A02_03", atan(1516.734/2693.), 0., 3090.749, 20200.863-1108.831-1516.734/2, -8984.-2693./2},

    {"GZ1.A03_04", 3.362*deg, 0., 2522.342, 20200.863-3200.+574.435/8*7, -11677-2518./2},
    {"GZ1.A04_05", 3.362*deg, 0., 2522.342, 20200.863-3200.+574.435/8*5, -14195-2518./2},
    {"GZ1.A05_06", 3.362*deg, 0., 2522.342, 20200.863-3200.+574.435/8*3, -16713-2518./2},
    {"GZ1.A06_07", 3.362*deg, 0., 2522.342, 20200.863-3200.+574.435/8*1, -19231-2518./2},

    {"GZ1.B01_02", 0.0, 0., 2885., 20200.863, -6099.-2885./2},
    {"GZ1.B02_03", 0.0, 0., 2693., 20200.863, -8984.-2693./2},
    {"GZ1.B03_04", 0.0, 0., 2518., 20200.863, -11677-2518./2},
    {"GZ1.B04_05", 0.0, 0., 2518., 20200.863, -14195-2518./2},
    {"GZ1.B05_06", 0.0, 0., 2518., 20200.863, -16713-2518./2},
    {"GZ1.B06_07", 0.0, 0., 2518., 20200.863, -19231-2518./2},

    {"ZC2.A02_B02", 90.*deg, 0., 1108.831, 20200.863-1108.831/2, -8984},

    {"ZC2.A03_B03", 90.*deg, 0., 3200.-574.435/4*4, 20199.-(3200.-574.435/4*4)/2, -11677},
    {"ZC2.A04_B04", 90.*deg, 0., 3200.-574.435/4*3, 20199.-(3200.-574.435/4*3)/2, -14195},
    {"ZC2.A05_B05", 90.*deg, 0., 3200.-574.435/4*2, 20199.-(3200.-574.435/4*2)/2, -16713},
    {"ZC2.A06_B06", 90.*deg, 0., 3200.-574.435/4*1, 20199.-(3200.-574.435/4*1)/2, -19231},

    {"ZC2.A02_B03", -atan(1108.831/2693.), 0., 2912.345, 20200.863-1108.831/2, -8984.-2693./2},

    {"ZC2.A03_B04", -atan((3200.-574.435/4*4)/2518.), 0., sqrt(pow(3200.-574.435/4*4, 2)+pow(2518., 2)), 20199.-(3200.-574.435/4*4)/2, -11677-2518./2},
    {"ZC2.A04_B05", -atan((3200.-574.435/4*3)/2518.), 0., sqrt(pow(3200.-574.435/4*3, 2)+pow(2518., 2)), 20199.-(3200.-574.435/4*3)/2, -14195-2518./2},
    {"ZC2.A05_B06", -atan((3200.-574.435/4*2)/2518.), 0., sqrt(pow(3200.-574.435/4*2, 2)+pow(2518., 2)), 20199.-(3200.-574.435/4*2)/2, -16713-2518./2},
    {"ZC2.A06_B07", -atan((3200.-574.435/4*1)/2518.), 0., sqrt(pow(3200.-574.435/4*1, 2)+pow(2518., 2)), 20199.-(3200.-574.435/4*1)/2, -19231-2518./2},

    {"ZC2.B01_B01", 0., 90.*deg, 4223.130, 20090.201, -6099},
    {"ZC2.B03_B03", 0., 90.*deg, 4223.130, 20090.201, -11677},
    {"ZC2.B05_B05", 0., 90.*deg, 4223.130, 20090.201, -16713},

    {"ZC2.A03_A03", 0., 90.*deg, 3673.848, 17477.166, -11677},
    {"ZC2.A05_A05", 0., 90.*deg, 3613.804, 17191.522, -16713},

    {"END", 0, 0, 0, 0, 0}
};

void
HBeamConstruction::try_init_model_pillar(const std::string& name) {

    const PillarData* p_pillar = 0;

    for (const PillarData* p=pillar_data; p->name!="END"; ++p) {
        if (p->name==name) {
            p_pillar = p;
            break;
        }
    }
    if (!p_pillar) {
        LogError << "Unknown HBeam part " << name << ". "
                 << "Please register your part in file " << __FILE__ << std::endl;
        assert(0);
    }

    m_theta = p_pillar->theta;
    m_direct_theta = p_pillar->direct_theta;
    m_L = p_pillar->L;

    m_radial = p_pillar->radial;
    m_z = p_pillar->z;
}
