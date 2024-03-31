#include "BundleRecTool.h"
#include "SniperKernel/ToolFactory.h"
#include <iostream>
#include <fstream>
#include <TMath.h>
DECLARE_TOOL(BundleRecTool);

BundleRecTool::BundleRecTool(const std::string& name)
  :ToolBase(name){
 //declProp("output",output_file);
 //declProp("truedis",truedis);
 declProp("x",x);
 declProp("y",y);
 declProp("chargecut",chargecut);
 declProp("nei1data", nei1data="nei1.dat");
 //declProp("PI",3.1415926);
 //declProp("file1", file1);
 //declProp("file2", file2);
 //declProp("normal",q_normal);
 //declProp("gauss",q_gauss);
 //declProp("mean",q_mean);
 declProp("qdistance",q_distance);
  }

bool BundleRecTool::inilink(){
     int x[119944];
     int y[119944];
/*using namespace std;
     struct test{
            int num;
            int link[10];
};
test Table[17745];*/
     ifstream in;
     in.open(nei1data.c_str());
     if (!in.good()) {
         LogError << "open file " << nei1data << " failed. " << std::endl;
         return false;
     }
     for(int n=0;n<119944;n++){
     in>>x[n]>>y[n];
     }
     in.close();
     for(int k=0;k<17746;k++){
     //m_PmtPropTable[k].a = 0;
     Table[k].num = 0;
       for(int l=0;l<10;l++){
         //m_PmtPropTable[k].link[l]=-1;
         Table[k].link[l]=-1;
       }
     }
     for(int i=0;i<17746;i++){
     for(int j=0;j<119944;j++){
        if(i == x[j]){
        //m_PmtPropTable[i].link[m_PmtPropTable[i].num]=y[j];
        //m_PmtPropTable[i].num++;
        Table[i].link[Table[i].num]=y[j];
        Table[i].num++;
      }
     }
    }
    return true;
}
bool BundleRecTool::reconstruct(RecTrks* trks){
    //JM::CDTrackRecEvent* evt = new JM::CDTrackRecEvent();
     vector<int>cell;
     for(int id=0;id<17746;id++){
         if((*m_ptab)[id].q>chargecut){
             cell.push_back(id);
           }
     }
    cout<<"cell"<<" "<<cell.size()<<endl;
    vector<int>cir;
    vector<int>seed;
    double charge1,charge2;
    int number=0;
    double RecX_in,RecY_in,RecZ_in,RecX_out,RecY_out,RecZ_out,RecX1,RecX2,RecX3,RecX4,RecY1,RecY2,RecY3,RecY4,RecZ1,RecZ2,RecZ3,RecZ4,RecX_in_1,RecY_in_1,RecZ_in_1,RecX_in_2,RecY_in_2,RecZ_in_2,RecX_out_1,RecY_out_1,RecZ_out_1,RecX_out_2,RecY_out_2,RecZ_out_2;
    //double RecDis,RecAng;
    //double RecD12,RecD34,RecD13,RecD24,RecD14,RecD23,k1,k2,k3;
    double k1,k2,k3;
    //ofstream of(output_file.c_str(),ios::app);
        for(vector<int>::iterator iter = cell.begin();iter != cell.end();iter++){
            //if(q_mean){charge1=BundleRecTool::chargeone(*iter);}
               //charge1 = BundleRecAlg::MiddleCharge(*iter);
               //if(q_normal){charge1 = m_PmtPropTable[*iter].Charge;}
               //if(q_gauss){charge1 = BundleRecAlg::gauss(*iter);}
               if(q_distance){charge1 = BundleRecTool::qsmooth(*iter,y);}
               //of<<charge1<<endl;
               cir=BundleRecTool::pmtlink(*iter,x);
               //of<<*iter<<" "<<cir.size()<<endl;
               for(vector<int>::iterator it = cir.begin();it != cir.end();it++){
               //if(q_mean){charge2=BundleRecAlg::chargeone(*it);}
               //charge2 = BundleRecAlg::MiddleCharge(*it);
               //if(q_normal){charge2 = m_PmtPropTable[*it].Charge;}
               //if(q_gauss){charge2 = BundleRecTool::gauss(*it);}
               if(q_distance){charge2 = BundleRecTool::qsmooth(*it,y);}
            //of<<charge2<<endl;
               if(charge1>=charge2){
                  number++;
                                   }
                                                        }
               if(cir.size() == number){
                  seed.push_back(*iter);
                  //cout<<*iter<<" "<<m_PmtPropTable[*iter].Position.X()<<" "<<m_PmtPropTable[*iter].Position.Y()<<" "<<m_PmtPropTable[*iter].Position.Z()<<" "<<BundleRecAlg::gauss(*iter)<<endl;
                                       }
                  number=0;
                  cir.clear();
                  }
   switch(seed.size()){
          case 2:
          RecX_in = BundleRecTool::recXYZ(seed[0],1);RecY_in = BundleRecTool::recXYZ(seed[0],2);RecZ_in = BundleRecTool::recXYZ(seed[0],3);
          RecX_out = BundleRecTool::recXYZ(seed[1],1);RecY_out = BundleRecTool::recXYZ(seed[1],2);RecZ_out = BundleRecTool::recXYZ(seed[1],3);
          break;
          case 4:
          RecX1 = BundleRecTool::recXYZ(seed[0],1);RecY1 = BundleRecTool::recXYZ(seed[0],2);RecZ1 = BundleRecTool::recXYZ(seed[0],3);
          RecX2 = BundleRecTool::recXYZ(seed[1],1);RecY2 = BundleRecTool::recXYZ(seed[1],2);RecZ2 = BundleRecTool::recXYZ(seed[1],3);
          RecX3 = BundleRecTool::recXYZ(seed[2],1);RecY3 = BundleRecTool::recXYZ(seed[2],2);RecZ3 = BundleRecTool::recXYZ(seed[2],3);
          RecX4 = BundleRecTool::recXYZ(seed[3],1);RecY4 = BundleRecTool::recXYZ(seed[3],2);RecZ4 = BundleRecTool::recXYZ(seed[3],3);
          k1 = BundleRecTool::parallel(RecX1,RecY1,RecZ1,RecX2,RecY2,RecZ2,RecX3,RecY3,RecZ3,RecX4,RecY4,RecZ4);
          k2 = BundleRecTool::parallel(RecX1,RecY1,RecZ1,RecX3,RecY3,RecZ3,RecX2,RecY2,RecZ2,RecX4,RecY4,RecZ4);
          k3 = BundleRecTool::parallel(RecX1,RecY1,RecZ1,RecX4,RecY4,RecZ4,RecX2,RecY2,RecZ2,RecX3,RecY3,RecZ3);
          if(k1<k2){
             if(k1<k3){
                RecX_in_1=RecX1;RecY_in_1=RecY1;RecZ_in_1=RecZ1;RecX_out_1=RecX2;RecY_out_1=RecY2;RecZ_out_1=RecZ2;
                RecX_in_2=RecX3;RecY_in_2=RecY3;RecZ_in_2=RecZ3;RecX_out_2=RecX4;RecY_out_2=RecY4;RecZ_out_2=RecZ4;                   
                      }else{
                RecX_in_1=RecX1;RecY_in_1=RecY1;RecZ_in_1=RecZ1;RecX_out_1=RecX4;RecY_out_1=RecY4;RecZ_out_1=RecZ4;
                RecX_in_2=RecX3;RecY_in_2=RecY3;RecZ_in_2=RecZ3;RecX_out_2=RecX2;RecY_out_2=RecY2;RecZ_out_2=RecZ2;                
                }
                   }else{
              if(k2<k3){
                RecX_in_1=RecX1;RecY_in_1=RecY1;RecZ_in_1=RecZ1;RecX_out_1=RecX3;RecY_out_1=RecY3;RecZ_out_1=RecZ3;
                RecX_in_2=RecX2;RecY_in_2=RecY2;RecZ_in_2=RecZ2;RecX_out_2=RecX4;RecY_out_2=RecY4;RecZ_out_2=RecZ4;
               }else{
                RecX_in_1=RecX1;RecY_in_1=RecY1;RecZ_in_1=RecZ1;RecX_out_1=RecX4;RecY_out_1=RecY4;RecZ_out_1=RecZ4;
                RecX_in_2=RecX3;RecY_in_2=RecY3;RecZ_in_2=RecZ3;RecX_out_2=RecX2;RecY_out_2=RecY2;RecZ_out_2=RecZ2;      
           }
          }
          break;
   }
   //switch(seed.size()){
   //case 2:
  if(seed.size()==2){
          TVector3 start(RecX_in, RecY_in, RecZ_in);
          TVector3 end(RecX_out, RecY_out, RecZ_out);
          trks->addTrk(start, end, -1, -1, 1024, 1, 0);
          // CLHEP::HepLorentzVector start(RecX_in, RecY_in, RecZ_in, -1);
          // CLHEP::HepLorentzVector end(RecX_out, RecY_out, RecZ_out, -1);
          // JM::RecTrack* mtrk = new JM::RecTrack(start, end);
          // mtrk->setPESum(1024);
          // mtrk->setQuality(1);
          // evt->addTrack(mtrk);
  //break;
  //case 4:
  }else if(seed.size()==4){
          TVector3 start1(RecX_in_1, RecY_in_1, RecZ_in_1);
          TVector3 end1(RecX_out_1, RecY_out_1, RecZ_out_1);
          trks->addTrk(start1, end1, -1, -1, 1024, 1, 0);
          // CLHEP::HepLorentzVector start1(RecX_in_1, RecY_in_1, RecZ_in_1, -1);
          // CLHEP::HepLorentzVector end1(RecX_out_1, RecY_out_1, RecZ_out_1, -1);
          // JM::RecTrack* mtrk1 = new JM::RecTrack(start1, end1);
          // mtrk1->setPESum(1024);
          // mtrk1->setQuality(1);
          // evt->addTrack(mtrk1);
          TVector3 start2(RecX_in_2, RecY_in_2, RecZ_in_2);
          TVector3 end2(RecX_out_2, RecY_out_2, RecZ_out_2);
          trks->addTrk(start2, end2, -1, -1, 1024, 1, 0);
          // CLHEP::HepLorentzVector start2(RecX_in_2, RecY_in_2, RecZ_in_2, -1);
          // CLHEP::HepLorentzVector end2(RecX_out_2, RecY_out_2, RecZ_out_2, -1);
          // JM::RecTrack* mtrk2 = new JM::RecTrack(start2, end2);
          // mtrk2->setPESum(1024);
          // mtrk2->setQuality(1);
          // evt->addTrack(mtrk2);
  //break;
}       

 // rh->setCDTrackEvent(evt);
 return true;
}
double BundleRecTool::qsmooth(int id,int a)
{
     //LogInfo << "id: " << id << std::endl;
          //LogInfo << "a:  " << a << std::endl;
      vector<int>cluster;
      double distance;
      double smoothcharge,charge=0;
      cluster = BundleRecTool::pmtlink(id,a);
      for(vector<int>::iterator it = cluster.begin();it != cluster.end();it++){
          distance=sqrt(TMath::Power((*m_ptab)[id].pos.x()-(*m_ptab)[*it].pos.x(), 2)+
                        TMath::Power((*m_ptab)[id].pos.y()-(*m_ptab)[*it].pos.y(), 2)+
                        TMath::Power((*m_ptab)[id].pos.z()-(*m_ptab)[*it].pos.z(), 2));
          charge=(1000/(1000+distance))*(*m_ptab)[*it].q + charge;}
             smoothcharge = charge/cluster.size();
             return smoothcharge;
              }                                                                                                                                                     
 vector<int> BundleRecTool::pmtlink(int id,int a){
        vector<int> cir1;
        int cirnum;
        /*for(int j = 0;j<m_PmtPropTable[id].num;j++){
            cir1.push_back(m_PmtPropTable[id].link[j]);
                      }*/
        cir1.push_back(id);
        for(int m = 0;m<a;m++){
                cirnum = cir1.size();
                for(int i=0;i<cirnum;i++){
                        for(int k=0;k<Table[cir1[i]].num;k++){
                                if(find(cir1.begin(),cir1.end(),Table[cir1[i]].link[k]) != cir1.end()){
                                }else{
                                        //LogInfo << "push_back: " << m_PmtPropTable[cir1[i]].link[k] << std::endl;
                                     cir1.push_back(Table[cir1[i]].link[k]);
                                     }
                                                                      } 
                                        }
                               }
                                       return cir1;
                                                } 
double BundleRecTool::recXYZ(int id,int a){
       vector<int>rec;
       double X=0,Y=0,Z=0,Charge=0;
       double recvalue;
       rec=BundleRecTool::pmtlink(id,3);
       for(vector<int>::iterator it = rec.begin();it != rec.end();it++){
           X=(*m_ptab)[*it].pos.x()*(*m_ptab)[*it].q+X;
           Y=(*m_ptab)[*it].pos.y()*(*m_ptab)[*it].q+Y;
           Z=(*m_ptab)[*it].pos.z()*(*m_ptab)[*it].q+Z;
           Charge=(*m_ptab)[*it].q+Charge;
       }
       switch(a){
              case 1:
              recvalue = X/Charge;
              break;
              case 2:
              recvalue = Y/Charge;
              break;
              case 3:
              recvalue = Z/Charge;
              break;
       }
       return recvalue;
     }
double BundleRecTool::parallel(double x_1,double y_1,double z_1,double x_2,double y_2,double z_2,double x_3,double y_3,double z_3,double x_4,double y_4,double z_4){
       double vectorX1,vectorY1,vectorZ1,vectorX2,vectorY2,vectorZ2,v1v2,vv,angle;
       vectorX1=x_1-x_2;
       vectorY1=y_1-y_2;
       vectorZ1=z_1-z_2;
       vectorX2=x_3-x_4;
       vectorY2=y_3-y_4;
       vectorZ2=z_3-z_4;
       v1v2=vectorX1*vectorX2+vectorY1*vectorY2+vectorZ1*vectorZ2;
       vv=(sqrt(vectorX1*vectorX1+vectorY1*vectorY1+vectorZ1*vectorZ1))*(sqrt(vectorX2*vectorX2+vectorY2*vectorY2+vectorZ2*vectorZ2));
       angle=fabs(acos(v1v2/vv));
       return angle;

}
bool
BundleRecTool::configure(const Params* pars, const PmtTable* ptab){
  LogDebug  << "configure the Dummy reconstruct tool!"
    << std::endl;
    LogDebug  << "Retrieve the key/value [Pmt3inchTimeReso: "
    << pars->get("Pmt3inchTimeReso") << "] from Params" << std::endl;
    m_ptab = ptab;
    return true;
}
