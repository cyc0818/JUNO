#ifndef BundleRecTool_h
#define BundleRecTool_h
#include "RecTools/IRecMuonTool.h"
#include "SniperKernel/ToolBase.h"
#include "Event/CdTrackRecHeader.h"
#include "TVector3.h"
#include <vector>
using namespace std;
struct Pmtlink{
               unsigned int num;
               int link[10];
}Table[17746];
class BundleRecTool : public IRecMuonTool, public ToolBase{
  public:
         BundleRecTool (const std::string& name);
            virtual ~BundleRecTool (){};
 
            bool reconstruct(RecTrks*) ;
            bool configure(const Params*,const PmtTable*);
 
     private:
            //bool extrmalpoint();
            bool inilink();
            double qsmooth(int,int);
            vector<int> pmtlink(int,int);
            double recXYZ(int,int);
            double parallel(double,double,double,double,double,double,double,double,double,double,double,double); 
            const PmtTable* m_ptab;
            double truedis;
            int x,y;
            int chargecut;
            double PI;
            bool q_normal;
            bool q_gauss;
            bool q_distance;
            bool q_mean;
            std::string file1;
            std::string file2;
            std::string nei1data;
            std::string output_file;
    /*private:
            struct PmtProp{
               int num;
               int link[10];
            };
            PmtProp Table[17745];*/
   };
#endif
