/*
TotTracker Tracking


Author: A.Meregaglia, C.Jollet (IPHC)
*/

#include "Event/TtCalibHeader.h"
#include "Event/TtRecHeader.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"

#include "Identifier/Identifier.h"
#include "Identifier/TtID.h"
#include "TMinuit.h"

#include "TTTrackingAlg.h"
#include "Geometry/TTGeomSvc.h"
#include "Geometry/TtGeom.h"
#include "Geometry/RecGeomSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "RootWriter/RootWriter.h"

#include "TtFilter.h"
#include "TtMake3D.h"

static int np;
static float xfit[100];
static float yfit[100];
static float zfit[100];
static double error[100];

void fcn(Int_t &, Double_t *, Double_t &, Double_t *, Int_t);

/////////////////////////////////////////
Double_t func(float xx,float yy,float zz,Double_t *par){
  Double_t value=(
		  pow((xx-par[0])*par[4]-(yy-par[1])*par[3],2)+
		  pow((yy-par[1])*par[5]-(zz-par[2])*par[4],2)+
		  pow((zz-par[2])*par[3]-(xx-par[0])*par[5],2)
		  )/(par[3]*par[3]+par[4]*par[4]+par[5]*par[5]);

  return value;
}
/////////////////////////////////////////////

void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
  Int_t i;
  Double_t chisq = 0;
  for(i=0;i<np;i++){
    if(xfit[i]!=999&&yfit[i]!=999&&zfit[i]!=999){
      chisq+=func(xfit[i],yfit[i],zfit[i],par)/pow(error[i],2);
    }
  }
  f=chisq;
}

/////////////////////////////////////////

DECLARE_ALGORITHM(TTTrackingAlg);

TTTrackingAlg::TTTrackingAlg(const std::string& name)
	:AlgBase(name)
{
  // Set Default Values of Some Parameters

  declProp("TimingThreshold",m_time_thresh);
  declProp("ApplyXTalkFilter",m_do_xtalk_filter);

  m_time_thresh = -1;
  m_tt_make3d = NULL;
  m_do_xtalk_filter = true;


}

TTTrackingAlg::~TTTrackingAlg()
{
  if(m_tt_make3d != NULL) delete m_tt_make3d;
}

bool
TTTrackingAlg::initialize()
{
  // if (m_det_type == "TT")
  {
    SniperPtr<TTGeomSvc> svc_ttgeom(getParent(), "TTGeomSvc");
    if (svc_ttgeom.invalid()) {
      LogError << "Can't get TTGeomSvc. We can't initialize TT position." << std::endl;
      return false;
    }
    if(m_time_thresh > 0){
      LogInfo << "TTTracking using time-based 3D make point." << std::endl;
      SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc");
      if(rgSvc.invalid()){
        LogError << "Failed to get RecGeomSvc instance!" << std::endl;
        return false;
      }
      TtGeom * tt_geom = rgSvc->getTtGeom();

      m_tt_make3d = new TtMake3DTime(svc_ttgeom.data(), tt_geom, m_time_thresh);
    }
    else{
      LogDebug << "TTTracking using usual 3D make point." << std::endl;
      m_tt_make3d = new TtMake3D(svc_ttgeom.data());
    }

    m_evt_treeTTReco = 0;
    // check the RootWriter is Valid.
    SniperPtr<RootWriter> svc(getParent(), "RootWriter");
    if (svc.invalid()) {
      LogError << "Can't Locate RootWriter. If you want to use it, please "
               << "enable it in your job option file."
	       << std::endl;
      return false;
    }
    m_evt_treeTTReco = svc->bookTree(*m_par, "USER_OUTPUT/TT", "TT Tracking");
    m_evt_treeTTReco->Branch("evtID", &m_eventID, "evtID/I");
    m_evt_treeTTReco->Branch("NTotPoints",&m_NTotPoints,"NTotPoints/I");
    m_evt_treeTTReco->Branch("PointX",&m_PointX,"PointX[NTotPoints]/F");
    m_evt_treeTTReco->Branch("PointY",&m_PointY,"PointY[NTotPoints]/F");
    m_evt_treeTTReco->Branch("PointZ",&m_PointZ,"PointZ[NTotPoints]/F");
    m_evt_treeTTReco->Branch("NTracks",&m_NTracks,"NTracks/I");
    m_evt_treeTTReco->Branch("NPoints",&m_NPoints,"NPoints[NTracks]/I");
    m_evt_treeTTReco->Branch("Coeff0",&m_Coeff0,"Coeff0[NTracks]/D");
    m_evt_treeTTReco->Branch("Coeff1",&m_Coeff1,"Coeff1[NTracks]/D");
    m_evt_treeTTReco->Branch("Coeff2",&m_Coeff2,"Coeff2[NTracks]/D");
    m_evt_treeTTReco->Branch("Coeff3",&m_Coeff3,"Coeff3[NTracks]/D");
    m_evt_treeTTReco->Branch("Coeff4",&m_Coeff4,"Coeff4[NTracks]/D");
    m_evt_treeTTReco->Branch("Coeff5",&m_Coeff5,"Coeff5[NTracks]/D");
    m_evt_treeTTReco->Branch("Chi2",&m_Chi2,"Chi2[NTracks]/D");
   
       
  }

  if (not m_tt_make3d) {
      LogError << "Failed to initialize m_tt_make3d. " << std::endl;
      return false;
  }


// Get Data Navigator
  SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
  if( navBuf.invalid() )
  {
    LogError << "cannot get the NavBuffer @ /Event" << std::endl;
    return false;
  }
  m_buf = navBuf.data();

  LogInfo << objName() << " initialized successfully." << std::endl;
  return true;
}

bool 
TTTrackingAlg::execute()
{
  LogDebug << "---------------------------------------" << std::endl;
  LogDebug << "Processing event " << m_evt_id << std::endl;

/*       
   if (m_evt_id<83013)
    {
      m_evt_id++;
      return true;
    }
  */
  // Get Input Data
  // initialize

  m_NTracks=0; 
  m_NTotPoints=0;



  auto nav = m_buf->curEvt();



  auto sh = JM::getHeaderObject<JM::TtCalibHeader>(nav);
  if (not sh) 
    return false;

  auto se  = sh->event();
  if (not se) 
    return false;



  // create a TTRecEvent
  auto rec_hdr = new JM::TtRecHeader();
  auto rec_event = new JM::TtRecEvt();

  const auto& tmp_calibttcol = se->calibTtCol();

  //  const std::vector<JM::SimTTHit*>& tmp_hits_tt = se->getTTHitsVec();
  //JM::SimTTHit* tthit = 0; 
 //TIter nexttthit(se -> getTTHitsVec());
  
  //--------------cross-talk filter-----------------------
  std::set<int> * CTFlag = NULL;
  if(m_do_xtalk_filter) CTFlag = TtFilter(tmp_calibttcol);
  else                  CTFlag = new std::set<int>;

  //---------------end cross-talk filter------------------
  
  struct points_3D * pts3D = m_tt_make3d->makePoint(tmp_calibttcol, CTFlag);
  delete CTFlag;
  if(pts3D == NULL){
	  m_evt_id++;
	  return true;
  }

  //combinations of 3D points an fit
 
  int loop(pts3D->ntouchedwalls);
  int minnpt(3);
  int foundtrack(0);
  int redoloop3(0);
  int additionalchi2(7);

  float newx3D[100];
  float newy3D[100];
  float newz3D[100];
  int leftnp(0);

  //avoid very long loops or segmentation fault
  if(loop==2 && pts3D->n3D<10)
   minnpt=2;

  if(loop>4 && pts3D->n3D>30)
    loop=4;

  while(loop>=minnpt && pts3D->n3D>1)
    {

       
      float ztest[pts3D->n3D];
      
      std::vector<bool> v(pts3D->n3D);
      //std::fill(v.begin() + ntouchedwalls, v.end(), true);
      std::fill(v.begin() + loop, v.end(), true);
      foundtrack=0;
      
      static Double_t vstart[6];
      Double_t step[6]={0.1,0.1,0.1,0.1,0.1,0.1};
      float xmin,xmax,ymin,ymax,zmin,zmax;
      
      do{

	np=0;
	leftnp=0;
	

	bool Isfit(true);
	xmin=1E5;xmax=-1E5;
	ymin=1E5;ymax=-1E5;
	zmin=1E5; zmax=-1E5;
	


	for (int i = 0; i < pts3D->n3D; ++i) {
	  if (!v[i]) {
	    xfit[np]=pts3D->x3D[i];
	    yfit[np]=pts3D->y3D[i];
	    zfit[np]=pts3D->z3D[i];
	    ztest[np]=pts3D->z3D[i];
	    np++;
	  }
	  else{
	    newx3D[leftnp]=pts3D->x3D[i];
	    newy3D[leftnp]=pts3D->y3D[i];
	    newz3D[leftnp]=pts3D->z3D[i];
	    leftnp++;
	  }
	}
	
	std::sort(ztest, ztest+ np );
	
	for(int uu=1;uu<np;uu++)
	  if(ztest[uu]==ztest[uu-1])
	    Isfit=false;
	
	if(Isfit)  //do not try the fit with points in the same wall
	  {
	    m_NPoints[m_NTracks]=np;
	    
	    for(int uu=0;uu<np;uu++)
	      {
		xmin=(xmin<xfit[uu])?xmin:xfit[uu];
		xmax=(xmax>xfit[uu])?xmax:xfit[uu];
		ymin=(ymin<yfit[uu])?ymin:yfit[uu];
		ymax=(ymax>yfit[uu])?ymax:yfit[uu];
		zmin=(zmin<zfit[uu])?zmin:zfit[uu];
		zmax=(zmax>zfit[uu])?zmax:zfit[uu];
                
		error[uu]=13.;
	      }
	    
	    //DO FIT
	    TMinuit *gMinuit2R= new TMinuit(6);
	    gMinuit2R->SetPrintLevel(-1);
	    gMinuit2R->SetFCN(fcn);
	    
	    vstart[0]=(xmax+xmin)/2;
	    vstart[1]=(ymax+ymin)/2;
	    vstart[2]=(zmax+zmin)/2;
	    vstart[3]=0;
	    vstart[4]=0;
	    vstart[5]=-1;
	    
	    gMinuit2R->DefineParameter(0,"x1",vstart[0],step[0],-1E5,1E5);
	    gMinuit2R->DefineParameter(1,"x2",vstart[1],step[1],-1E5,1E5);
	    gMinuit2R->DefineParameter(2,"x3",vstart[2],step[2],-1E5,1E5);
	    gMinuit2R->FixParameter(2);
	    gMinuit2R->DefineParameter(3,"v1",vstart[3],step[3],-1E5,1E5);
	    gMinuit2R->DefineParameter(4,"v2",vstart[4],step[4],-1E5,1E5);
	    gMinuit2R->DefineParameter(5,"v3",vstart[5],step[5],-1E5,1E5);
	    gMinuit2R->FixParameter(5);
	    
	    
	    gMinuit2R->Command("SET NOW");
	    gMinuit2R->Command("MINI");
	    // Print results
	     Double_t amin,edm,errdef;
	    Int_t nvpar,nparx,icstat;
	    gMinuit2R->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
	    double chi2R=amin;
	    Double_t Rp[6];
	    Double_t Rep[6];
	    
	    gMinuit2R->GetParameter(0,Rp[0],Rep[0]);
	    gMinuit2R->GetParameter(1,Rp[1],Rep[1]);
	    gMinuit2R->GetParameter(2,Rp[2],Rep[2]);
	    gMinuit2R->GetParameter(3,Rp[3],Rep[3]);
	    gMinuit2R->GetParameter(4,Rp[4],Rep[4]);
	    gMinuit2R->GetParameter(5,Rp[5],Rep[5]);

	    delete gMinuit2R;
	    
	    
	    //xRbottom=Rp[3]/Rp[5]*(zbottom-Rp[2])+Rp[0];
	    //yRbottom=Rp[4]/Rp[5]*(zbottom-Rp[2])+Rp[1];
	    

	    if((np>3 && chi2R<3) || (np==3 && chi2R<(3+redoloop3*additionalchi2) ) || (np==2 && chi2R<0.1))
	      {
	

		for(int uu=0;uu<np;uu++)
		  {
		    m_PointX[m_NTotPoints]=xfit[uu];
		    m_PointY[m_NTotPoints]=yfit[uu];
		    m_PointZ[m_NTotPoints]=zfit[uu];
		    m_NTotPoints++;

                    rec_event->addPoint(xfit[uu], yfit[uu], zfit[uu]);
		  }
		m_Chi2[m_NTracks]=chi2R;
		m_Coeff0[m_NTracks]=Rp[0];
		m_Coeff1[m_NTracks]=Rp[1];
		m_Coeff2[m_NTracks]=Rp[2];
		m_Coeff3[m_NTracks]=Rp[3];
		m_Coeff4[m_NTracks]=Rp[4];
		m_Coeff5[m_NTracks]=Rp[5];
	
		m_NTracks++;
		foundtrack=1;

                // convert from double to float
                float Rp_f[6];
                for (int i = 0; i < 6; ++i) {
                    Rp_f[i] = static_cast<float>(Rp[i]);
                };
                rec_event->addTrack(np, Rp_f, static_cast<float>(chi2R));
	      }	    
	  }
	
	
      } while (std::next_permutation(v.begin(), v.end()) && foundtrack==0);

      if(foundtrack==0)//if not track is found try with less point else continue with the same number of points after removing the used 3D hits
	{
	  loop--;
	}   
      else
	{
	  for(int uu=0;uu<leftnp;uu++)
	    {
	      pts3D->x3D[uu]=newx3D[uu];
	      pts3D->y3D[uu]=newy3D[uu];
	      pts3D->z3D[uu]=newz3D[uu];
	    }

	  pts3D->n3D-=np;
	  if(loop>pts3D->n3D)
	    loop=pts3D->n3D;
	}

       if(m_NTracks==0 && loop==2 && redoloop3==0)
	 {
           redoloop3=1;
	   loop=3;
	 }

       if(m_NTracks==0 && loop==2 && pts3D->n3D<4)
	 {
	   minnpt=2;
	 }

    }//end if 3D points
  delete pts3D;
  
  
   
  m_eventID=m_evt_id;
  
  
  if (m_evt_treeTTReco) {
    m_evt_treeTTReco -> Fill();
  }

  rec_hdr->setEvent(rec_event);
  JM::addHeaderObject(nav, rec_hdr);
  
  m_evt_id ++;
  return true;
}



bool TTTrackingAlg::finalize()
{
  LogInfo << objName() << " finalized successfully." << std::endl;
  return true;
}

