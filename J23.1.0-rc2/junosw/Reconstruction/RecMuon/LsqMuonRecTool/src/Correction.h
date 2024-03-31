#ifndef Correction_h
#define Correction_h 
#include "IFhtCorrection.h"
#include "SniperKernel/ToolBase.h"
#include "TProfile.h"
#include "TProfile2D.h"
class TH2F;
class TFile;
class Correctiond2ad : public IFhtCorrection, public ToolBase{
  public:
    Correctiond2ad(const std::string& name);
    virtual double correction(const PmtProp&,const TVector3&,const TVector3&)const;
    virtual double sigma(const PmtProp&,const TVector3&,const TVector3&,double* )const;
    virtual bool setCorrectionFileAndNumBinDist(std::string filename, int nBin);
    virtual ~Correctiond2ad();

    std::string objName()const {return "Correctiond2ad";}
    
  private:
    
    bool retrieveCorrectionTables();
    void mallocTablesPointVector(int n);
    void freeTablesPointVector();

    TProfile2D** m_prof2dtad;
    TH2F** m_hdtaderr;
    
    int m_numBinDist;
    double m_rangeDist;

    std::string m_corrfilename;
    TFile* m_corrfile;
};


class Correctiond2qd : public IFhtCorrection, public ToolBase{
  public:
    Correctiond2qd(const std::string& name);
    virtual double correction(const PmtProp&,const TVector3&,const TVector3&)const;
    virtual double sigma(const PmtProp&,const TVector3&,const TVector3&,double* )const;
    virtual bool setCorrectionFileAndNumBinDist(std::string filename, int nBin);
    virtual ~Correctiond2qd();

    std::string objName()const {return "Correctiond2qd";}
    
  private:
    
    bool retrieveCorrectionTables();
    void mallocTablesPointVector(int n);
    void freeTablesPointVector();

    TProfile2D** m_prof2dtqd;
    TH2F** m_hdtqderr;
    
    int m_numBinDist;
    double m_rangeDist;

    std::string m_corrfilename;
    TFile* m_corrfile;
};

class Correctiond1qdqd : public IFhtCorrection, public ToolBase{
  public:
    Correctiond1qdqd(const std::string& name);
    virtual double correction(const PmtProp&,const TVector3&,const TVector3&)const;
    virtual double sigma(const PmtProp&,const TVector3&,const TVector3&,double* )const;
    virtual bool setCorrectionFileAndNumBinDist(std::string filename, int nBin);
    virtual ~Correctiond1qdqd();

    std::string objName()const {return "Correctiond21qdqd";}
    
  private:
    
    bool retrieveCorrectionTables();
    void mallocTablesPointVector(int n);
    void freeTablesPointVector();

    TProfile** m_prof1dt_q;
    TProfile** m_prof1dtq_d;
    TProfile** m_prof1dtqd_q;
    TProfile** m_prof1dtqdq_d;
    TProfile** m_prof1dtqdqd_d;
    int m_numBinDist;
    
    double m_rangeDist;

    std::string m_corrfilename;
    TFile* m_corrfile;
};

class Correctiond1dqdq : public IFhtCorrection, public ToolBase{
  public:
    Correctiond1dqdq(const std::string& name);
    virtual double correction(const PmtProp&,const TVector3&,const TVector3&)const;
    virtual double sigma(const PmtProp&,const TVector3&,const TVector3&,double* )const;
    virtual bool setCorrectionFileAndNumBinDist(std::string filename, int nBin);
    virtual ~Correctiond1dqdq();

    std::string objName()const {return "Correctiond21dqdq";}
    
  private:
    
    bool retrieveCorrectionTables();
    void mallocTablesPointVector(int n);
    void freeTablesPointVector();

    TProfile** m_prof1dt_d;
    TProfile** m_prof1dtd_q;
    TProfile** m_prof1dtdq_d;
    TProfile** m_prof1dtdqd_q;
    TProfile** m_prof1dtdqdq_q;
    int m_numBinDist;
    
    double m_rangeDist;

    std::string m_corrfilename;
    TFile* m_corrfile;
};
class Correctiond1adad : public IFhtCorrection, public ToolBase{
  public:
    Correctiond1adad(const std::string& name);
    virtual double correction(const PmtProp&,const TVector3&,const TVector3&)const;
    virtual double sigma(const PmtProp&,const TVector3&,const TVector3&,double* )const;
    virtual bool setCorrectionFileAndNumBinDist(std::string filename, int nBin);
    virtual ~Correctiond1adad();

    std::string objName()const {return "Correctiond21adad";}
    
  private:
    
    bool retrieveCorrectionTables();
    void mallocTablesPointVector(int n);
    void freeTablesPointVector();

    TProfile** m_prof1dt_a;
    TProfile** m_prof1dta_d;
    TProfile** m_prof1dtad_a;
    TProfile** m_prof1dtada_d;
    TProfile** m_prof1dtadad_d;
    int m_numBinDist;
    
    double m_rangeDist;

    std::string m_corrfilename;
    TFile* m_corrfile;
};
class Correctiond1dada : public IFhtCorrection, public ToolBase{
  public:
    Correctiond1dada(const std::string& name);
    virtual double correction(const PmtProp&,const TVector3&,const TVector3&)const;
    virtual double sigma(const PmtProp&,const TVector3&,const TVector3&,double* )const;
    virtual bool setCorrectionFileAndNumBinDist(std::string filename, int nBin);
    virtual ~Correctiond1dada();

    std::string objName()const {return "Correctiond21dada";}
    
  private:
    
    bool retrieveCorrectionTables();
    void mallocTablesPointVector(int n);
    void freeTablesPointVector();

    TProfile** m_prof1dt_d;
    TProfile** m_prof1dtd_a;
    TProfile** m_prof1dtda_d;
    TProfile** m_prof1dtdad_a;
    TProfile** m_prof1dtdada_a;
    int m_numBinDist;
    
    double m_rangeDist;

    std::string m_corrfilename;
    TFile* m_corrfile;
};


class Correctiond1q : public IFhtCorrection, public ToolBase{
  public:
    Correctiond1q(const std::string& name);
    virtual double correction(const PmtProp&,const TVector3&,const TVector3&)const;
    virtual double sigma(const PmtProp&,const TVector3&,const TVector3&,double* )const;
    virtual bool setCorrectionFileAndNumBinDist(std::string filename, int nBin);
    virtual ~Correctiond1q();

    std::string objName()const {return "Correctiond21q";}
    
  private:
    
    bool retrieveCorrectionTables();
    void mallocTablesPointVector(int n);
    void freeTablesPointVector();

    TProfile** m_prof1dt_q;
    int m_numBinDist;
    
    double m_rangeDist;

    std::string m_corrfilename;
    TFile* m_corrfile;
};
#endif /* ifndef Correction_h */
