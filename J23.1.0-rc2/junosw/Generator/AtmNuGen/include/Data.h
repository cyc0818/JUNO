#ifndef DATA_hh
#define DATA_hh


const int MAX_PAR = 50;
struct Data{

  Data();
  
  int evtID;
  int tPdg;
  double tPx;
  double tPy;
  double tPz;
  double tRemovalE;
  double tE;
  double tMass;
  
  int pPdg;
  int interType;
  int ctag;
  double pPx;
  double pPy;
  double pPz;
  double pEn;

  int m_isoPDG;
  int m_isoPdg;
  int m_isoTag;
  double m_isoPx;
  double m_isoPy;
  double m_isoPz;
  double m_isoE;
  double m_isoMass;
  double m_isoEffMass;


  int Npars;
  int pdg[MAX_PAR];
  double px[MAX_PAR];      // GeV
  double py[MAX_PAR];      // GeV
  double pz[MAX_PAR];      // GeV
  double energy[MAX_PAR];  // GeV
  double mass[MAX_PAR];    // GeV
  double energyT;

 
  double Kpx ;
  double Kpy  ;           
  double Kpz   ;          
  double Kenergy;         

  double weight;
};
#endif
