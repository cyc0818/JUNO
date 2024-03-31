#ifndef OECEVENT_H
#define OECEVENT_H
#include "Event/EventObject.h"
#include <string>

#include "TTimeStamp.h"

using namespace std;

namespace JM{
    class OecEvt:public EventObject{
        public:
            OecEvt();
            ~OecEvt();

            //get function
            std::string getIDA();//get detector
            std::string getIDB();//get top level category
            std::string getIDC();//get sub-level category
            float getEnergy(){return f_energy;}
            //float getTime(){return f_time;}
            const TTimeStamp &getTime() const{return m_time;}
            float getVertexX(){return f_vertexX;}
            float getVertexY(){return f_vertexY;}
            float getVertexZ(){return f_vertexZ;}
            int getTag(){return i_tag;}

            //set function
            void setEnergy(float e){f_energy=e;}
            void setTime(const TTimeStamp& value){m_time=value;}
            void setVertexX(float x){f_vertexX=x;}
            void setVertexY(float y){f_vertexY=y;}
            void setVertexZ(float z){f_vertexZ=z;}
            bool addTag(int tag){i_tag=i_tag | tag;return true;}
            bool isMuon();
            bool isTTMuon();
            bool isWPMuon();
            bool isMuonRelated();
            //bool addpTag(string);
            //bool addtTag(string);
            void setMuID(const std::vector<int>& muid){
                 f_muid = muid; 
            }
            const std::vector<int>& getMuID() const {
                 return f_muid;
            }
            void setMuInX(const std::vector<float>& muinx){
                 f_muinx = muinx;
            }
            const std::vector<float>& getMuInX() const {
                 return f_muinx;
            }
            void setMuInY(const std::vector<float>& muiny){
                 f_muiny = muiny;
            }
            const std::vector<float>& getMuInY() const {
                 return f_muiny;
            }
            void setMuInZ(const std::vector<float>& muinz){
                 f_muinz = muinz;
            }
            const std::vector<float>& getMuInZ() const {
                 return f_muinz;
            }
            void setMuOutX(const std::vector<float>& muoutx){
                 f_muoutx = muoutx;
            }
            const std::vector<float>& getMuOutX() const {
                 return f_muoutx;
            }
            void setMuOutY(const std::vector<float>& muouty){
                 f_muouty = muouty;
            }
            const std::vector<float>& getMuOutY() const {
                 return f_muouty;
            }
            void setMuOutZ(const std::vector<float>& muoutz){
                 f_muoutz = muoutz;
            }
            const std::vector<float>& getMuOutZ() const {
                 return f_muoutz;
            }
            void setQBF(float Qbf){f_qbf=Qbf;}
            float getQBF(){return f_qbf;}

        private:
            float f_energy;//reconstructed energy of single event
            TTimeStamp m_time;
            int n_clusters;
            float f_vertexX;//reconstructed vertex of single event
            float f_vertexY;
            float f_vertexZ;
            int i_tag;//the tag of event //eg: CD_muonUncor_IBD
            std::vector<int> f_muid;
            std::vector<float> f_muinx;
            std::vector<float> f_muiny;
            std::vector<float> f_muinz;
            std::vector<float> f_muoutx;
            std::vector<float> f_muouty;
            std::vector<float> f_muoutz;
            float f_qbf;

        public:
            ClassDef(OecEvt,3)
    };
}
#endif
