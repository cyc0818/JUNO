#ifndef GtNeutronTool_h
#define GtNeutronTool_h

/*
 * Load data from capture of spallation neutrons.
 *
 * ROOT format:
 * root [2] spall_neutron_gen->Print()
 * ******************************************************************************
 * *Tree    :spall_neutron_gen: a tree with spallation neutron information      *
 * *Entries :       11 : Total =            4638 bytes  File  Size =       2015 *
 * *        :          : Tree compression factor =   1.01                       *
 * ******************************************************************************
 * *Br    0 :evtId     : evtId/I                                                *
 * *Entries :       11 : Total  Size=        614 bytes  File Size  =        129 *
 * *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.00     *
 * *............................................................................*
 * *Br    1 :multi     : multi/I                                                *
 * *Entries :       11 : Total  Size=        614 bytes  File Size  =        119 *
 * *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.08     *
 * *............................................................................*
 * *Br    2 :captureT  : captureT[multi]/D                                      *
 * *Entries :       11 : Total  Size=        888 bytes  File Size  =        292 *
 * *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.00     *
 * *............................................................................*
 * *Br    3 :posx      : posx[multi]/F                                          *
 * *Entries :       11 : Total  Size=        784 bytes  File Size  =        212 *
 * *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.00     *
 * *............................................................................*
 * *Br    4 :posy      : posy[multi]/F                                          *
 * *Entries :       11 : Total  Size=        784 bytes  File Size  =        212 *
 * *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.00     *
 * *............................................................................*
 * *Br    5 :posz      : posz[multi]/F                                          *
 * *Entries :       11 : Total  Size=        784 bytes  File Size  =        212 *
 * *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.00     *
 * *............................................................................*
 * 
 */

#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"

#include "HepMC/SimpleVector.h"
#include "HepMC/GenParticle.h"

class TTree;
class TFile;

class GtNeutronTool: public ToolBase,
                     public IGenTool
{
 public:
    GtNeutronTool(const std::string& name);
    ~GtNeutronTool();

    bool configure();
    bool mutate(HepMC::GenEvent& event);

 private:
    bool initTree();
 private:
    std::string m_neutron_filename;
    int m_start_idx;
    double m_neutron_energy;

 private:
    TFile* file;
    TTree* evt;

    static const int kN = 10000;

    int multi;
    double captureT[kN];
    float posx[kN];
    float posy[kN];
    float posz[kN];

};

#endif
