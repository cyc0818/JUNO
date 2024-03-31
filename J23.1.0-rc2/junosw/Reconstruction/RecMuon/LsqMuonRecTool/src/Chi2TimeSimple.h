#ifndef Chi2TimeSimple_h
#define Chi2TimeSimple_h 1
#include "RecTools/PmtProp.h"

class Chi2TimeSimple 
{
  public:
    Chi2TimeSimple(const PmtTable&);
    double operator()(const double *xx);

    void setLSRadius(double r){m_LSRadius = r;}
    void setMuonSpeed(double v){m_vmuon = v;}
    void setLightSpeed(double c){m_clight = c;}
    void setLSRefraction(double index){m_nLSlight = index;}

  private:
    const PmtTable& m_ptable;

    double m_LSRadius;
    double m_vmuon;
    double m_nLSlight;
    double m_clight;
};
#endif /* ifndef Chi2TimeSimple_h */
