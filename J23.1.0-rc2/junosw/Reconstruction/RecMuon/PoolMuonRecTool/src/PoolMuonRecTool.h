#ifndef PoolMuonRecTool_h
#define PoolMuonRecTool_h
#include "RecTools/IRecMuonTool.h"
#include "SniperKernel/ToolBase.h"
#include "Event/WpRecHeader.h"
#include <vector>
#include "TVector3.h"

class PoolMuonRecTool :public IRecMuonTool,  public ToolBase
{
    public:
        PoolMuonRecTool (const std::string& name);
        virtual ~PoolMuonRecTool ();

        bool reconstruct(RecTrks*); 
        bool configure(const Params*, const PmtTable* ); 

    private:
        //get track hit points with sphere and cylinder
        double hitSphereAll(double tarR, TVector3 &center, TVector3 &vpos, TVector3 &dir, TVector3 &X1, TVector3 &X2);

    private:
        const PmtTable* m_ptable; 
        int maxpositions;//fitting 
        double PMTThreshold_pe ; //for fitting used, pe selected cut for each pmt, >PMTThreshold_pe cut, not >=
        int PMTThreshold_n; //for fitting used, total npmt cut, >=PMTThreshold_n, not >PMTThreshold_n
        double AroundPePlus_DisCut;//meter, <=AroundPePlus_DisCut, not <AroundPePlus_DisCut
        //std::string m_fname; //name of TFile contain fix line; 
        std::vector<double>    PmtPe;//the charge of picked pmt
        std::vector<double>    OptPmtPe;//the charge of picked pmt
        std::vector<double>    vpe_around;
        std::vector<TVector3>  PmtPos;
        std::vector<TVector3>  OptPmtPos;
        std::vector<TVector3>  vfitp;
        std::vector<TVector3>  rectracks;
        std::vector<TVector3>  qwpoints;//charge weight center
        std::vector<TVector3>  vrec_in;//charge weight center
        std::vector<TVector3>  vrec_out;//charge weight center

};

#endif
