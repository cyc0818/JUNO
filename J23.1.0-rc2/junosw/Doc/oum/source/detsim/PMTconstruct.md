# Create a new PMT and its visualization

In this part, we will try to create a new type of PMT.

The PMT we would implement is Hamamatsu R12199. This figure is taken from <https://www.hamamatsu.com/resources/pdf/etd/R12199_TPMH1356E.pdf>

![Fig. Hamamatsu R12199](figs/R12199.png)

## Analysis this geometry 
We will simplify the geometry in simulation. Following figure shows the ellipsoid approximation. I use Windows Paint program to draw this ellipse.

![Fig. Ellipsoid approximation of Hamamatsu R12199](figs/R12199-ellipse.png)

The we could get the pixel position of several points:

![Fig. Ellipsoid approximation of Hamamatsu R12199](figs/R12199-ellipse-pixel.png)

* P1: (284 px, 180 px)
* P2: (132 px, 271 px)
* P3: (177 px, 493 px)
* P4: (177 px, 620 px)

So now, we could have several input parameters:

![Fig. Ellipsoid approximation of Hamamatsu R12199](figs/R12199-ellipse-pixel-length.png)

* P1: (0 mm, 22.5 mm)
* P2: (40 mm, 0 mm)
* P3: (28.25 mm, -55.5 mm)
* P4: (27,25 mm, -87.5 mm)

## Create a solid maker
Because for both PMT body and its inner part, the geometry is almost same, so we would like to create a solid maker first. With parameter thickness, we could create different size solids.

File `R12199_PMTSolid.hh`:

    #ifndef R12199_PMTSolid_hh
    #define R12199_PMTSolid_hh
    
    #include "globals.hh"
    
    class G4VSolid;
    
    class R12199_PMTSolid {
    public:
        R12199_PMTSolid();
    
        G4VSolid* GetSolid(G4String solidname, double thickness=0.0);
    
    private:
        double P1r, P1z;
        double P2r, P2z;
        double P3r, P3z;
        double P4r, P4z;
    };

    #endif

File `R12199_PMTSolid.cc`:

    #include "R12199_PMTSolid.hh"
    
    #include "G4Sphere.hh"
    #include "G4Ellipsoid.hh"
    #include "G4Tubs.hh"
    #include "G4Torus.hh"
    #include "G4Polycone.hh"
    #include "G4UnionSolid.hh"
    
    #include <cmath>
    
    R12199_PMTSolid::R12199_PMTSolid() {
        P1r = 0.0; P1z = 22.5*mm;
        P2r = 40.0*mm; P2z = 0.0*mm;
        P3r = 28.25*mm; P3z = -55.5*mm;
        P4r = 28.25*mm; P4z = -87.5*mm;
    }
    
    G4VSolid*
    R12199_PMTSolid::GetSolid(G4String solidname, double thickness) {
    
        double _p1r = P1r;           double _p1z = P1z + thickness;
        double _p2r = P2r+thickness; double _p2z = P2z;
        double _p3r = P3r+thickness; double _p3z = P3z+thickness;
        double _p4r = P4r+thickness; double _p4z = P4z-thickness;
    
        double r_bottom = 51.9*mm/2;
        double r_botthin = r_bottom+thickness;
    
        std::cout << "R12199_PMTSolid::GetSolid " << __LINE__ << std::endl;
        // PART: ellipsoid
        G4Ellipsoid* pmttube_solid_sphere = new G4Ellipsoid(
                                                solidname+"_1_Ellipsoid",
                                                _p2r, // pxSemiAxis
                                                _p2r, // pySemiAxis
                                                _p1z // pzSemiAxis
                                                );
        std::cout << "R12199_PMTSolid::GetSolid " << __LINE__ << std::endl;
        // return pmttube_solid_sphere;
    
        // PART: polycone
        double zplane[] = {_p4z, _p3z, _p3z, _p2z};
        double rinner[] = {0, 0, 0, 0};
        double router[] = {_p4r, _p3r, r_botthin, r_botthin};
    
        G4VSolid* pmttube_solid_tube = new G4Polycone(
                                    solidname+"_2_polycone",
                                    0,
                                    360*deg,
                                    4,
                                    zplane,
                                    rinner,
                                    router
                                    );
        G4UnionSolid* pmttube_solid = new G4UnionSolid
            (solidname,
             pmttube_solid_sphere ,
             pmttube_solid_tube ,
             0,
             G4ThreeVector(0,0,0)
        ) ;
    
        return pmttube_solid;
    }

File `R12199PMTManager.hh`:

    #ifndef R12199PMTManager_hh
    #define R12199PMTManager_hh
    
    #include "SniperKernel/ToolBase.h"
    #include "DetSimAlg/IPMTElement.h"
    
    class G4OpticalSurface;
    class G4Material;
    class G4VSensitiveDetector;
    class G4PVPlacement;
    class G4VSolid;
    class G4Tubs;
    
    class R12199_PMTSolid;
    
    class R12199PMTManager: public IPMTElement,
                            public ToolBase {
    public:
        // Interface
        G4LogicalVolume* getLV();
        G4double GetPMTRadius();
        G4double GetPMTHeight();
        G4double GetZEquator();
        G4ThreeVector GetPosInPMT();
    
    public:
        // Constructor
        R12199PMTManager
        (const G4String& plabel // label -- subvolume names are derived from this
        );
        ~R12199PMTManager();
    private:
        void init();
        void init_material();
        void init_pmt();
    private:
        G4LogicalVolume* m_logical_pmt;
        G4Material* GlassMat;
        G4Material* PMT_Vacuum;
    };
    
    #endif

File `R12199PMTManager.cc`:

    #include <boost/python.hpp>
    #include "R12199PMTManager.hh"
    #include "R12199_PMTSolid.hh"
    
    #include "G4LogicalVolume.hh"
    #include "G4Material.hh"
    #include "G4PVPlacement.hh"
    
    #include "SniperKernel/SniperPtr.h"
    #include "SniperKernel/SniperLog.h"
    #include "SniperKernel/ToolFactory.h"
    
    DECLARE_TOOL(R12199PMTManager);
    
    R12199PMTManager::R12199PMTManager(const G4String& plabel)
        : ToolBase(plabel), m_logical_pmt(0), GlassMat(0), PMT_Vacuum(0)
    {
    
    }
    
    R12199PMTManager::~R12199PMTManager() {
    
    }
    
    // Interface
    G4LogicalVolume* 
    R12199PMTManager::getLV() {
        LogInfo << __LINE__ << std::endl;
        if (!m_logical_pmt) {
            init();
        }
        LogInfo << __LINE__ << std::endl;
        return m_logical_pmt;
    }
    
    G4double
    R12199PMTManager::GetPMTRadius() {
        return 40.*mm;
    }
    
    G4double
    R12199PMTManager::GetPMTHeight() {
        return 110.*mm;
    }
    
    G4double
    R12199PMTManager::GetZEquator() {
        return 22.5*mm;
    }
    
    G4ThreeVector
    R12199PMTManager::GetPosInPMT() {
        // not used.
        return G4ThreeVector();
    }
    
    void
    R12199PMTManager::init() {
        LogInfo << __LINE__ << std::endl;
        init_material();
        LogInfo << __LINE__ << std::endl;
        init_pmt();
        LogInfo << __LINE__ << std::endl;
    }
    
    void
    R12199PMTManager::init_material() {
         GlassMat = G4Material::GetMaterial("Pyrex");
         PMT_Vacuum = G4Material::GetMaterial("Vacuum"); 
    }
    
    void
    R12199PMTManager::init_pmt() {
        ////////////////////////////////////////////////////////////////
        // MAKE SOLIDS
        ////
        R12199_PMTSolid* pmtsolid_maker = new R12199_PMTSolid();
        LogInfo << __LINE__ << std::endl;
        G4VSolid* body_solid = pmtsolid_maker->GetSolid(objName() + "_body_solid");
        LogInfo << __LINE__ << std::endl;
    
    
        ////////////////////////////////////////////////////////////////
        // MAKE LOGICAL VOLUMES (add materials)
        ////
        G4LogicalVolume* body_log= new G4LogicalVolume
            ( body_solid,
              GlassMat,
              objName()+"_body_log" );
    
        m_logical_pmt = body_log;
    }

In this example, we directly use the solid returned by solid maker.

Then please modify according to following:

    Index: ../share/examples/prototype/pyjob_prototype_onepmt.py
    ===================================================================
    --- ../share/examples/prototype/pyjob_prototype_onepmt.py       (revision 2416)
    +++ ../share/examples/prototype/pyjob_prototype_onepmt.py       (working copy)
    @@ -22,6 +22,7 @@
                                                    "Tub3inchV2",
                                                    "Hello3inch",
                                                    "PMTMask",
    +                                               "R12199",
                                                    ])
    Index: ../src/LSExpDetectorConstruction.cc
    ===================================================================
    --- ../src/LSExpDetectorConstruction.cc (revision 2416)
    +++ ../src/LSExpDetectorConstruction.cc (working copy)
    @@ -696,6 +696,19 @@
                   Steel,
                   NULL,
                   not_a_leak);
    +  } else if (current_test_pmt == "R12199"){
    +      std::cout << "start R12199..." << std::endl;
    +      IDetElement* t = det_elem("R12199PMTManager/PMT_3inch");
    +      std::cout << "... R12199... " << __LINE__ << std::endl;
    +      if (t) {
    +          pmt_det = dynamic_cast<IPMTElement*>(t);
    +      }
    +      std::cout << "... R12199... " << __LINE__ << std::endl;
    +      assert(pmt_det);
    +      std::cout << "... R12199... " << __LINE__ << std::endl;
    +      assert(pmt_det -> getLV());
    +      std::cout << "... done R12199... " << __LINE__ << std::endl;
    +
       } else if (current_test_pmt == "PMTMask"){

Then we could visualize it:

![Fig. Visualization of solid created by solid maker](figs/R12199-g4-one-volume.png)

## Create inner parts
Before creating optical surface, we need to use boolean operations to create inner I and inner II.
Update `R12199PMTManager.cc`

    #include <boost/python.hpp>
    #include "R12199PMTManager.hh"
    #include "R12199_PMTSolid.hh"
    #include "G4Tubs.hh"
    #include "G4IntersectionSolid.hh" // for boolean solids
    #include "G4SubtractionSolid.hh" // for boolean solids
    
    #include "G4LogicalVolume.hh"
    #include "G4Material.hh"
    #include "G4PVPlacement.hh"
    #include "G4VisAttributes.hh" // for G4VisAttributes::Invisible
    
    #include "SniperKernel/SniperPtr.h"
    #include "SniperKernel/SniperLog.h"
    #include "SniperKernel/ToolFactory.h"
    
    DECLARE_TOOL(R12199PMTManager);
    
    R12199PMTManager::R12199PMTManager(const G4String& plabel)
        : ToolBase(plabel), m_logical_pmt(0), GlassMat(0), PMT_Vacuum(0)
    {
    
    }
    
    R12199PMTManager::~R12199PMTManager() {
    
    }
    
    // Interface
    G4LogicalVolume* 
    R12199PMTManager::getLV() {
        LogInfo << __LINE__ << std::endl;
        if (!m_logical_pmt) {
            init();
        }
        LogInfo << __LINE__ << std::endl;
        return m_logical_pmt;
    }
    
    G4double
    R12199PMTManager::GetPMTRadius() {
        return 40.*mm;
    }
    
    G4double
    R12199PMTManager::GetPMTHeight() {
        return 110.*mm;
    }
    
    G4double
    R12199PMTManager::GetZEquator() {
        return 22.5*mm;
    }
    
    G4ThreeVector
    R12199PMTManager::GetPosInPMT() {
        // not used.
        return G4ThreeVector();
    }
    
    void
    R12199PMTManager::init() {
        LogInfo << __LINE__ << std::endl;
        init_material();
        LogInfo << __LINE__ << std::endl;
        init_pmt();
        LogInfo << __LINE__ << std::endl;
    }
    
    void
    R12199PMTManager::init_material() {
         GlassMat = G4Material::GetMaterial("Pyrex");
         PMT_Vacuum = G4Material::GetMaterial("Vacuum"); 
    }
    
    void
    R12199PMTManager::init_pmt() {
        ////////////////////////////////////////////////////////////////
        // MAKE SOLIDS
        ////
        R12199_PMTSolid* pmtsolid_maker = new R12199_PMTSolid();
        LogInfo << __LINE__ << std::endl;
        G4VSolid* body_solid = pmtsolid_maker->GetSolid(objName()+"_body_solid");
        G4VSolid* inner_solid= pmtsolid_maker->GetSolid(objName()+"_inner_solid", -1*mm);
        LogInfo << __LINE__ << std::endl;
    
        G4double helper_sep_tube_r = GetPMTRadius();
        G4double helper_sep_tube_h = GetZEquator();
        G4double helper_sep_tube_hh = helper_sep_tube_h/2; // half
    
        G4VSolid * pInnerSep = new G4Tubs(objName()+"_inner_separator",
                0.,
                helper_sep_tube_r+1E-9*mm,
                helper_sep_tube_hh+1E-9*mm,
                0.,360.*degree);
        G4ThreeVector innerSepDispl(0.,0.,helper_sep_tube_hh-1E-9*mm);
        G4VSolid* inner1_solid = new G4IntersectionSolid( objName()
                + "_inner1_solid", inner_solid, pInnerSep, NULL, innerSepDispl);
        G4VSolid* inner2_solid = new G4SubtractionSolid( objName()
                + "_inner2_solid", inner_solid, pInnerSep, NULL, innerSepDispl);
    
        ////////////////////////////////////////////////////////////////
        // MAKE LOGICAL VOLUMES (add materials)
        ////
        G4LogicalVolume* body_log= new G4LogicalVolume
            ( body_solid,
              GlassMat,
              objName()+"_body_log" );
    
        G4LogicalVolume* inner1_log= new G4LogicalVolume
            ( inner1_solid,
              PMT_Vacuum,
              objName()+"_inner1_log" );
    
        G4LogicalVolume* inner2_log= new G4LogicalVolume
            ( inner2_solid,
              PMT_Vacuum,
              objName()+"_inner2_log" );
    
        ////////////////////////////////////////////////////////////////
        // MAKE PHYSICAL VOLUMES (place logical volumes)
        ////
    
        G4ThreeVector noTranslation(0.,0.,0.);
        // inner 1
        new G4PVPlacement
            ( 0,                   // no rotation
              noTranslation,       // puts face equator in right place
              inner1_log,          // the logical volume
              objName()+"_inner1_phys", // a name for this physical volume
              body_log,            // the mother volume
              false,               // no boolean ops
              0 );                 // copy number;
    
        // inner 2
        new G4PVPlacement
            ( 0,                   // no rotation
              noTranslation,       // puts face equator in right place
              inner2_log,          // the logical volume
              objName()+"_inner1_phys", // a name for this physical volume
              body_log,            // the mother volume
              false,               // no boolean ops
              0 );                 // copy number;
    
        ////////////////////////////////////////////////////////////////
        // Set colors and visibility
        ////
        G4VisAttributes* visAtt = 0;
        visAtt= new G4VisAttributes(G4Color(0.7,0.5,0.3,1.00));
        visAtt -> SetForceSolid(true);
        inner1_log->SetVisAttributes (visAtt);
        visAtt= new G4VisAttributes(G4Color(0.6,0.7,0.8,0.99));
        visAtt -> SetForceSolid(true);
        inner2_log->SetVisAttributes (visAtt);
    
        m_logical_pmt = body_log;
    }


![Fig. Visualization of solid created by solid maker](figs/R12199-g4-inner-color.png)

## Attach optical surface
Now, let's attach optical surface to this PMT. But note the `G4LogicalBorderSurface` constructor, it only accepts two physical volumes. We need to add an additional logical volume to contain the body, so a physical volume could be created.

Update `R12199PMTManager` again. First create additional solid which is a little bigger than body solid:

    void
    R12199PMTManager::init_pmt() {
        ////////////////////////////////////////////////////////////////
        // MAKE SOLIDS
        ////
        R12199_PMTSolid* pmtsolid_maker = new R12199_PMTSolid();
        LogInfo << __LINE__ << std::endl;
        G4VSolid* pmt_solid  = pmtsolid_maker->GetSolid(objName()+"_pmt_solid", 1E-3*mm);
        G4VSolid* body_solid = pmtsolid_maker->GetSolid(objName()+"_body_solid");
        G4VSolid* inner_solid= pmtsolid_maker->GetSolid(objName()+"_inner_solid", -1*mm);
        LogInfo << __LINE__ << std::endl;

Then create corresponding logical volume:

        ////////////////////////////////////////////////////////////////
        // MAKE LOGICAL VOLUMES (add materials)
        ////
        G4LogicalVolume* pmt_log = new G4LogicalVolume
            ( pmt_solid,
              GlassMat,
              objName()+"_log" );

Place body into it:

        // body
        G4VPhysicalVolume* body_phys = new G4PVPlacement
            ( 0,                   // no rotation
              noTranslation,       // puts face equator in right place
              body_log,          // the logical volume
              objName()+"_body_phys", // a name for this physical volume
              pmt_log,            // the mother volume
              false,               // no boolean ops
              0 );                 // copy number;
    
Then you need to create corresponding optical surface. Detail could be found in other PMT's implementation.

Associate optical surfaces:

        new G4LogicalBorderSurface(objName()+"_photocathode_logsurf1",
                inner1_phys, body_phys,
                Photocathode_opsurf);
        new G4LogicalBorderSurface(objName()+"_photocathode_logsurf2",
                body_phys, inner1_phys,
                Photocathode_opsurf);
        new G4LogicalBorderSurface(objName()+"_mirror_logsurf1",
                inner2_phys, body_phys,
                m_mirror_opsurf);
        new G4LogicalBorderSurface(objName()+"_mirror_logsurf2",
                body_phys, inner2_phys,
                m_mirror_opsurf);

Please note the order of this constructor. It means when photon flies from one volume to another volume, this surface will be used.

Try to run the code using:

    $ python pyjob_prototype_onepmt.py --pmt-name R12199

But if you look at the user data, you will find there is no hit. That's because we don't have sensitive detector.

## Attach sensitive detector

Now, let's associate sensitive detector to both body and inner 1. If you don't attach sensitive detector to inner 1, the photons from inner 1 to body will not convert to hit.

Add following code:

        ////////////////////////////////////////////////////////////////
        // GET and ATTACH SENSITIVE DETECTOR 
        ////
        G4SDManager* SDman = G4SDManager::GetSDMpointer();
        G4VSensitiveDetector* m_detector = SDman->FindSensitiveDetector("PMTSDMgr");
        assert(m_detector);

        body_log->SetSensitiveDetector(m_detector);
        inner1_log->SetSensitiveDetector(m_detector);

For your convenience, you could download them:

* [`R12199_PMTSolid.hh`](https://juno.ihep.ac.cn/trac/browser/offline/trunk/Doc/oum/source/detsim/examples/R12199_PMTSolid.hh)
* [`R12199_PMTSolid.cc`](https://juno.ihep.ac.cn/trac/browser/offline/trunk/Doc/oum/source/detsim/examples/R12199_PMTSolid.cc)
* [`R12199PMTManager.hh`](https://juno.ihep.ac.cn/trac/browser/offline/trunk/Doc/oum/source/detsim/examples/R12199PMTManager.hh)
* [`R12199PMTManager.cc`](https://juno.ihep.ac.cn/trac/browser/offline/trunk/Doc/oum/source/detsim/examples/R12199PMTManager.cc)
