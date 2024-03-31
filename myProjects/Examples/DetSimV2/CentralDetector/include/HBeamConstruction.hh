/*
 * =====================================================================================
 *
 *       Filename:  HBeamConstruction.hh
 *
 *    Description:  Implement a H beam or I beam geometry.
 *                  This is used in latticed stainless steel shell.
 *                  * https://en.wikipedia.org/wiki/I-beam
 *
 *                  H x B x tw x tf
 *
 *                             B
 *                     |<------------->|              ^ z
 *                                                    |
 *          -----      +---------------+              |           y
 *            ^        |               |              |         .
 *            |        +------   ------+              |       .
 *            |              |   |                    |     .
 *            |              |   |                    |   .
 *       H    |            ->|   |<- tw               | .
 *            |              |   |                    +-----------------------> x
 *            |              |   |
 *            |              |   |   |-- tf
 *            |              |   |   v
 *            |        +------   ------+
 *            v        |               |
 *          -----      +---------------+
 *                                   ^
 *                                   |
 *
 *
 *                  * To identify the same model in different layers, we use following
 *                    naming convention:
 *
 *                      Model.label
 *                    
 *                    So if it is in up06 layer, we name it:
 *                      
 *                      GLb2.up05
 *
 *         Author:  Tao Lin (lintao@ihep.ac.cn), 
 *   Organization:  IHEP
 *
 * =====================================================================================
 */

#ifndef HBeamConstruction_hh
#define HBeamConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class HBeamConstruction: public IDetElement,
                         public ToolBase {
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    // allow to query
    double geom_info(const std::string& /* param */);
    double geom_info(const std::string& /* param */, int /* idx */) {return 0.0;}

    HBeamConstruction(const std::string& name);
    ~HBeamConstruction();

private:
    void makeIt();

    // There are several models in the design. We try to initialize HxBxtwxtf
    // based on its name.
    // See .cc file for detail parameters.
    // NOTE: L is not initialized.
    void try_init_model(const std::string& name);

    void try_init_model_shell(const std::string& name);

    // pillar or model name starts with GZ or ZC
    void try_init_model_pillar(const std::string& name);

private:
    double m_H;
    double m_B;
    double m_tw;
    double m_tf;
    double m_L;

    double m_theta;
    double m_direct_theta;

    // allow user to specify the center position of HBeam
    // This is useful when constructing pillars
    double m_radial;
    double m_z;

private:
    G4LogicalVolume* m_hbeam_lv;
    G4Material* Steel;
    std::string m_name;
};

#endif
