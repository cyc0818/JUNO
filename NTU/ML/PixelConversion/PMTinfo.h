#include <iostream>
#include <utility>
#include <algorithm>
#include <cmath>
#include "TVector3.h"
#include "TMath.h"


using namespace std;
const vector<int> ypixel_map = {19365, 19338, 19278, 19203, 19114, 19010, 18893, 18761, 18615, 18454, 18307, 18121, 17952, 17741, 17550, 17314, 17102, 16842, 16609, 16370, 16077, 15815, 15548, 15222, 14932, 14638, 14338, 13973, 13650, 13322, 12989, 12588, 12234, 11876, 11514, 11148, 10708, 10321, 9930, 9535, 9138, 8664, 8249, 7832, 7411, 6988, 6562, 6134, 5626, 5184, 4739, 4293, 3845, 3396, 2945, 2494, 2041, 1588, 1135, 681, 227, -227, -681, -1135, -1588, -2041, -2494, -2945, -3396, -3845, -4293, -4739, -5184, -5626, -6134, -6562, -6988, -7411, -7832, -8249, -8664, -9138, -9535, -9930, -10321, -10708, -11148, -11514, -11876, -12234, -12588, -12989, -13322, -13650, -13973, -14338, -14638, -14932, -15222, -15548, -15815, -16077, -16370, -16609, -16842, -17102, -17314, -17550, -17741, -17952, -18121, -18307, -18454, -18615, -18761, -18893, -19010, -19114, -19203, -19278, -19338, -19365};

class PMTinfo{

    public:

        PMTinfo(const int& id = -1, const TVector3& position = {0,0,0}, const double& TrueHitTime = 0, const double& FirstHitTime = 0)
            : m_id(id), m_position(position), m_TrueHitTime(TrueHitTime), m_FirstHitTime(FirstHitTime){
                
                m_IsHit = false;
                m_pixel = pair<int,int>(-1,-1);

            }

        int GetPMTID() const{
            return m_id;
        }

        TVector3 GetPosition() const{
            return m_position;
        }

        double GetTrueHitTime() const{
            return m_TrueHitTime;
        }

        double GetFirstHitTime() const{
            return m_FirstHitTime;
        }

        bool IsHit() const{
            return m_IsHit;
        }

        pair<int,int> GetPixel() const{
            return m_pixel;
        }

        int GetPixelX() const{
            return m_pixel.first;
        }

        int GetPixelY() const{
            return m_pixel.second;
        }

        bool Initialize(){
            m_TrueHitTime = 0;
            m_FirstHitTime = 0;
            m_IsHit = false;
            return true;
        }

        bool SetPMTID(const int& id){
            m_id = id;
            return true;
        }

        bool SetPosition(const TVector3& position){
            m_position = position;
            return true;
        }

        bool SetPosition(const double& x, const double& y, const double& z){
            m_position.SetXYZ(x,y,z);
            return true;
        }

        bool CheckandSetTrueHitTime(const double&);

        bool SetFirstHitTime(const double& FirstHitTime){
            m_FirstHitTime = FirstHitTime;
            return true;
        }

        bool ConvertToPixel();
        bool ShiftandNormalize(double shift, double normalization);


    private:

        int m_id;
        TVector3 m_position;
        pair<int,int> m_pixel;
        double m_TrueHitTime;   // true first hit time
        double m_FirstHitTime;  // simulated and reconstructed first hit time
        bool m_IsHit;


};


bool PMTinfo::CheckandSetTrueHitTime(const double& TrueHitTime){

    if(m_IsHit){

        double realfirst = min(TrueHitTime, m_TrueHitTime);
        m_TrueHitTime = realfirst;

    }

    else{

        m_TrueHitTime = TrueHitTime;
        m_IsHit = true;
    }

    return true;

}


bool PMTinfo::ConvertToPixel(){

    int xpixel = 0;
    int ypixel = 0;
    const int Nmax = 229;

    int Neff = Nmax*sqrt(m_position.Mag2()-m_position.Z()*m_position.Z())/m_position.Mag();
    xpixel = Neff*atan(m_position.X()/m_position.Y())/TMath::Pi() + Nmax/2;
    vector<int>::const_iterator it = find(ypixel_map.begin(), ypixel_map.end(), static_cast<int>(m_position.Z()));
    ypixel = distance(ypixel_map.begin(), it);

    m_pixel = pair<int,int>(xpixel, ypixel);

    return true;

}


bool PMTinfo::ShiftandNormalize(double shift, double normalization){

    if(m_IsHit){
        m_TrueHitTime -= shift;
        m_TrueHitTime *= normalization;
    }
    if(m_FirstHitTime != 0) m_FirstHitTime *= normalization;

    return true;

}



