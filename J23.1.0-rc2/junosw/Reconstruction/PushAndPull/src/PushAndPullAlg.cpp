/*
Vertex Reconstruction: Push and Pull(learn from KamLand)
Ennergy Reconstruction: 
1.Calculate the total expect charge by a simple model
2.e_rec = experiment_total/expect_total
expect_total: the total expect charge per 1MeV
experiment_total: the total charge of the PMTs
3.correct the residual non-uniformity by calibration


The parameters need to be optimized !!!

Author: xzh
*/

//#include "Event/SimHeader.h"
//#include "Event/SimEvent.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdVertexRecHeader.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"
#include "convert.h"
#include "TH1D.h"
#include "TMath.h"

#include "PushAndPullAlg.h"
#include "SniperKernel/AlgFactory.h"



DECLARE_ALGORITHM(PushAndPullAlg);

PushAndPullAlg::PushAndPullAlg(const std::string& name)
	:AlgBase(name)
{
  // Set Default Values of Some Parameters
  m_pmt_num = 18306;
  m_pmt_center_r = 19.5; //m
  m_pmt_r = 0.254;
  m_pmt_front_r = m_pmt_center_r - m_pmt_r;
  m_ls_r = 17.7;  

  m_eff_att_length = 1/28.6;
  m_evt_id = 0;
  declProp("Det_Type",m_det_type);
  // calibration function
  m_ene_cor1 = new TF1("ene_cor1","pol5",0,6000);
  m_ene_cor2 = new TF1("ene_cor2","pol5",0,6000);

  m_ene_nonl = new TF1("ene_nonl","(([1]+[3]*(x-1.022))/(1+[0]*exp(-[2]*(x-1.022)))*(x-1.022)+0.935802158)",0.2,12);
  double  ene_nonl_para[4] = {0.122495, 1.04074, 1.78087, 0.00189743};
  m_ene_nonl -> SetParameters(ene_nonl_para);

}

PushAndPullAlg::~PushAndPullAlg()
{
  delete m_ene_cor1;
  delete m_ene_cor2;
  delete m_ene_nonl;
}

bool
PushAndPullAlg::initialize()
{
  if (m_det_type == "Balloon")
  {
    m_pmt_num = 18306;
    low = 10;
    up = 1;
    SPEED_LS = 5.133;
    SPEED_BUFFER = 5.133;
    m_ccm_ratio = 1.45;
// 3900 0MeV edep
//    m_ene_cor1 -> SetParameters(2605.63864,0.01752022836,1.060166261e-05,-2.225060339e-08,8.294115677e-12,-1.084700915e-15);
//    m_ene_cor2 -> SetParameters(-334356.6667,331.3145645,-0.1284233418,2.450312794e-05,-2.298898937e-09,8.459974787e-14);
// 4500 4.46MeV edep
    m_ene_cor1 -> SetParameters(14259.09246,0.07006400758,0.0001474273225,-1.957384004e-07,6.738485546e-11,-8.112207491e-15);
    m_ene_cor2 -> SetParameters(53617247.01,-53887.13783,21.65216339,-0.004346573311,4.359240038e-07,-1.74736193e-11);


  }
  if (m_det_type == "Acrylic")
  {
    m_pmt_num = 17746;
    low = 10;
    up = 2;
    SPEED_LS = 5.143;
    SPEED_BUFFER = 4.5;//3.0;
    m_ccm_ratio = 1.35;
// 3900 0MeV edep
//    m_ene_cor1 -> SetParameters(2987.422037,0.005482097587,2.922953393e-05,-2.192594088e-08,6.441267004e-12,-7.667842118e-16);
//    m_ene_cor2 -> SetParameters(-1183811.844,1266.727258,-0.5386206567,0.0001141209626,-1.20564065e-08,5.0825949e-13);
// 3900 4.46MeV edep
    m_ene_cor1 -> SetParameters(16368.90387,-0.1113718706,0.0003298024099,-2.030428538e-07,5.246904622e-11,-5.441085801e-15);
    m_ene_cor2 -> SetParameters(-8770104.313,9417.78391,-4.021057884,0.0008552850904,-9.067372259e-08,3.834130057e-12);
  }

// Get Data Navigator
  SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
  if( navBuf.invalid() )
  {
    LogError << "cannot get the NavBuffer @ /Event" << std::endl;
    return false;
  }
  m_buf = navBuf.data();

// Get Geometry Information of PMT
  SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc");
  if ( rgSvc.invalid()) 
  {
    LogError << "Failed to get RecGeomSvc instance!" << std::endl;
    return false;
  }
  m_cdGeom = rgSvc->getCdGeom();
  
  for(int i=0; i<m_pmt_num; ++i)
  {
    unsigned int all_pmt_id = (unsigned int)i;
    Identifier all_id = Identifier(CdID::id(all_pmt_id,0));
    PmtGeom* all_pmt = m_cdGeom->getPmt(all_id);
    if ( !all_pmt )
      LogError << "Wrong Pmt ID: " << i << std::endl;
    TVector3 all_pmtCenter = all_pmt->getCenter();
    m_pmt_pos.push_back(all_pmtCenter);

    m_pmt_hit.push_back(0.0);
    m_first_time.push_back(0.0);
  }

  for (int i=0;i<m_pmt_num; i+=1000)
  {
    LogDebug << "Pmt ID:" << i << std::endl;
    LogDebug << "Position:" << m_pmt_pos[i].X() << "\t" 
	     << m_pmt_pos[i].Y() << "\t"
	     << m_pmt_pos[i].Z() << std::endl;
  } 

  LogInfo << objName() << " initialized successfully." << std::endl;
  return true;
}

bool 
PushAndPullAlg::execute()
{
  LogDebug << "---------------------------------------" << std::endl;
  LogDebug << "Processing event " << m_evt_id << std::endl;

  // Get Input Data
  // initialize
  m_expect_total = 0.0;
  m_experiment_total = 0.0;
  for (int i=0;i<m_pmt_num;++i)
  {
    m_pmt_hit[i] = 0;
    m_first_time[i] = 0;
  }
  auto nav = m_buf->curEvt();

  auto chcol =(JM::CdLpmtCalibHeader*) nav->getHeader("/Event/CalibEvent");
  const auto& chhlist = chcol->event()->calibPMTCol();
  for (auto chit = chhlist.begin(); chit!=chhlist.end(); ++chit) 
  {
    auto calib = *chit;

    unsigned int pmtId = calib->pmtId();
    Identifier id = Identifier(pmtId);

    double nPE = calib->nPE();
    double firstHitTime = calib->firstHitTime();
    
    m_pmt_hit[CdID::module(id)] = nPE;
    m_first_time[CdID::module(id)] = firstHitTime;
    m_experiment_total += nPE;

    PmtGeom *pmt = m_cdGeom->getPmt(id);
    if ( !pmt ) 
      LogError << "Wrong Pmt Id " << id << std::endl;
    
    TVector3 pmtCenter = pmt->getCenter();

    if (m_evt_id == 0) 
    {
            LogDebug << "   pmtId : " << pmtId
                << "    nPE : " << nPE
                << "    firstHitTime : " << firstHitTime
                << "  pmtCenter : " << pmtCenter.x()
                << "  " <<  pmtCenter.y()
                << "  "  << pmtCenter.z() 
		<< " R:" << pmtCenter.Mag() << std::endl;
    }
  }

// Vertex Reconstruction 
  // MC Trueth
/*
  JM::SimHeader* sh = dynamic_cast<JM::SimHeader*>(nav->getHeader("/Event/SimEvent"));
  if (not sh) 
    return false;
  JM::SimEvent* se  = dynamic_cast<JM::SimEvent*>(sh->event());
  if (not se) 
    return false;
  TIter nexttrk(se -> getTracks());
  JM::SimTrack* trk = (JM::SimTrack*)nexttrk();
  double x_edep = trk -> getEdepX();
  double y_edep = trk -> getEdepY();
  double z_edep = trk -> getEdepZ();
  double r_edep = TMath::Sqrt(x_edep*x_edep+y_edep*y_edep+z_edep*z_edep);
  double e_edep = trk -> getEdep();
  LogDebug << "MC Trueth:" 
  	  << trk -> getEdepX() << "\t"
	    << trk -> getEdepY() << "\t"
	    << trk -> getEdepZ() << std::endl;
*/
  chargeCenter();
  searchVer(m_ccm_ratio*x_ccm,m_ccm_ratio*y_ccm,m_ccm_ratio*z_ccm);
// Energy Reconstruction
  searchEne();

// Output data to buffer 
  auto aDataHdr = new JM::CdVertexRecHeader(); //unit: mm,  MeV, ...
  auto aDataEvt = new JM::CdVertexRecEvt();
  auto aData = new JM::RecVertex();
//  aData->setEdep_x(x_edep);
//  aData->setEdep_y(y_edep);
//  aData->setEdep_z(z_edep);
//  aData->setEdep_r(r_edep);
//  aData->setCcm_x(x_ccm);
//  aData->setCcm_y(y_ccm);
//  aData->setCcm_z(z_ccm);
//  aData->setCcm_r(r_ccm);
  aData->setX(x_rec); 
  aData->setY(y_rec);
  aData->setZ(z_rec); 

  aData->setPx(x_edep);
  aData->setPy(y_edep);
  aData->setPz(z_edep);
  
  //  aData->setR(r_rec);
  //aData->setEnergy(e_edep);
  aData->setEprec(ene_rec); 
  aData->setPESum(m_experiment_total); 
  aDataEvt->addVertex(aData);
  aDataHdr->setEvent(aDataEvt);
/*
  double dR = TMath::Sqrt((x_edep-x_rec)*(x_edep-x_rec)+(y_edep-y_rec)*(y_edep-y_rec)
		+(z_edep-z_rec)*(z_edep-z_rec));
  if (dR > 1000)
    LogError << "Rec Fail." << std::endl;
  if (x_edep>15000||x_edep<-15000) 
    LogDebug << "Check" << std::endl;
*/
  JM::addHeaderObject(nav, aDataHdr); 


  m_evt_id ++;
  return true;
}

bool
PushAndPullAlg::finalize()
{
  LogInfo << objName() << " finalized successfully." << std::endl;
  return true;
}


// Private Functions
bool
PushAndPullAlg::searchVer(double input_x,double input_y,double input_z)
{
  double tmp_pos_x = input_x;
  double tmp_pos_y = input_y;
  double tmp_pos_z = input_z;

  TVector3 delta_r;
  double r = 0;
  r = TMath::Sqrt(tmp_pos_x*tmp_pos_x+tmp_pos_y*tmp_pos_y+tmp_pos_z*tmp_pos_z);
  if (r>m_ls_r*1000)
  {
    tmp_pos_x *= (m_ls_r*1000-100)/r ;
    tmp_pos_y *= (m_ls_r*1000-100)/r ;
    tmp_pos_z *= (m_ls_r*1000-100)/r ;
  }

  bool flag = false;
  for (int i=0;i<100;++i)
  {
    r = TMath::Sqrt(tmp_pos_x*tmp_pos_x+tmp_pos_y*tmp_pos_y+tmp_pos_z*tmp_pos_z);
    if (r>m_ls_r*1000)
    {
      LogInfo << "Invaid vertex. " << std::endl;
      if (!flag)
      {
        i = 0;
        flag = true;
        tmp_pos_x *= (m_ls_r*1000-10)/r ;
        tmp_pos_y *= (m_ls_r*1000-10)/r ;
        tmp_pos_z *= (m_ls_r*1000-10)/r ;
      }
      else
      {
        tmp_pos_x *= (m_ls_r*1000)/r ;
        tmp_pos_y *= (m_ls_r*1000)/r ;
        tmp_pos_z *= (m_ls_r*1000)/r ;
        break;
      }
    }
    delta_r = calDeltaR(tmp_pos_x,tmp_pos_y,tmp_pos_z);
    tmp_pos_x += delta_r.X();
    tmp_pos_y += delta_r.Y();
    tmp_pos_z += delta_r.Z();
//    if (delta_r.Mag() < 1e-7) break;
  }
  LogDebug << delta_r.Mag() << std::endl;
  for (int i=0;i<4;++i)
  {
    LogDebug << "PE:" << i+1 << "\tfired num:" << fired_pmt_pe[i] << std::endl; 
  }
  LogInfo << "Final Vertex: " << tmp_pos_x << "\t"
            << tmp_pos_y << "\t"
            << tmp_pos_z << std::endl;

  x_rec = tmp_pos_x;
  y_rec = tmp_pos_y;
  z_rec = tmp_pos_z;
  r_rec = TMath::Sqrt(x_rec*x_rec+y_rec*y_rec+z_rec*z_rec);
  return true;
}

void
PushAndPullAlg::chargeCenter()
{
  double x_sum = .0;
  double y_sum = .0;
  double z_sum = .0;
  double pe_sum = .0;

  for (int i=0; i<m_pmt_num; ++i)
  {
    x_sum += m_pmt_hit[i]*m_pmt_pos[i].X()*m_pmt_front_r/m_pmt_center_r;
    y_sum += m_pmt_hit[i]*m_pmt_pos[i].Y()*m_pmt_front_r/m_pmt_center_r;
    z_sum += m_pmt_hit[i]*m_pmt_pos[i].Z()*m_pmt_front_r/m_pmt_center_r;
    pe_sum += m_pmt_hit[i];
  }

  x_ccm = x_sum/pe_sum;
  y_ccm = y_sum/pe_sum;
  z_ccm = z_sum/pe_sum;
  r_ccm = TMath::Sqrt(x_ccm*x_ccm+y_ccm*y_ccm+z_ccm*z_ccm);

  LogInfo << "Charge Center:" 
	    << x_ccm << "\t"
            << y_ccm << "\t"
            << z_ccm << std::endl;

}


TVector3
PushAndPullAlg::calDeltaR(double x,double y,double z)
{ 
  // use the PMTs of 1pe  
  bool flag_pe[4] = {true,false,false,false};
  TH1D* tmp_hit_time[4];
  for (int i=0;i<4;++i)
  {
    tmp_hit_time[i] = new TH1D("tmp_"+convert<TString>(i)+"pe","",5000,-20,2000);
  }

  // find the peak time of t_res
  std::vector<double> v_res_time(m_pmt_num);
  for (int i=0; i<m_pmt_num; ++i)
  {
    v_res_time[i] = 1e9;
    if (m_pmt_hit[i]<0.5 || m_pmt_hit[i]>=5) continue;

    // for electronics study
    //if (m_pmt_pos[i].X() < 0) continue;

    double tof = 0;
    int pe_num = int(m_pmt_hit[i]); 
    if (!flag_pe[pe_num-1]) continue;
    tof = calTOF(x,y,z,i);
    tmp_hit_time[pe_num-1] -> Fill(m_first_time[i]-tof);
    v_res_time[i] = (m_first_time[i]-tof);
  }
  int max_bin_id_pe[4];
  double peak_time_pe[4];
  for (int i=0;i<4;++i)
  {
    max_bin_id_pe[i] = tmp_hit_time[i] -> GetMaximumBin();
    peak_time_pe[i] = tmp_hit_time[i] -> GetBinCenter(max_bin_id_pe[i]);
    delete tmp_hit_time[i];
  }
  // use t_res in [t_peak -low, t_peak +up]
  double mean_res_time_pe[4];
  for (int i=0;i<4;++i)
  {
    fired_pmt_pe[i]=0;
    mean_res_time_pe[i]=0.0;
  }

  for (int i=0;i<m_pmt_num; ++i)
  {
    int pe_num = int (m_pmt_hit[i]);
    if ( pe_num < 1||pe_num >=5) 
      continue;
    else
      --pe_num;
    if (v_res_time[i]<peak_time_pe[pe_num]-low || v_res_time[i]>peak_time_pe[pe_num]+up ) continue;
    mean_res_time_pe[pe_num] += v_res_time[i];
    fired_pmt_pe[pe_num] ++;
  }

  for (int i=1;i<4;++i)
    if (fired_pmt_pe[i] < 200) flag_pe[i] = false;

  for (int i=0;i<4;++i)
    mean_res_time_pe[i] /= fired_pmt_pe[i];
  TVector3 delta_r(0,0,0);
  double delta_x = 0.0;
  double delta_y = 0.0;
  double delta_z = 0.0; 

  for (int i=0;i<m_pmt_num; ++i)
  {     
    double tof = m_first_time[i] - v_res_time[i]; 
    int pe_num = int (m_pmt_hit[i]);
    if ( pe_num < 1|| pe_num>=5) 
      continue;
    else
      --pe_num;
    
    if (!flag_pe[pe_num]) continue;
    if (v_res_time[i]<peak_time_pe[pe_num]-low || v_res_time[i]>peak_time_pe[pe_num]+up ) continue;

    delta_x += (v_res_time[i]-mean_res_time_pe[pe_num])*(x-m_pmt_pos[i].X())/(tof*fired_pmt_pe[pe_num]);
    delta_y += (v_res_time[i]-mean_res_time_pe[pe_num])*(y-m_pmt_pos[i].Y())/(tof*fired_pmt_pe[pe_num]);
    delta_z += (v_res_time[i]-mean_res_time_pe[pe_num])*(z-m_pmt_pos[i].Z())/(tof*fired_pmt_pe[pe_num]);
  }
  delta_r.SetX(delta_x);
  delta_r.SetY(delta_y);
  delta_r.SetZ(delta_z);
  return delta_r;
}

double 
PushAndPullAlg::calTOF(double x,double y,double z,int id)
{
  // use pmt front 
  double pmt_pos_x=m_pmt_pos[id].X()*m_pmt_front_r/m_pmt_center_r;
  double pmt_pos_y=m_pmt_pos[id].Y()*m_pmt_front_r/m_pmt_center_r;
  double pmt_pos_z=m_pmt_pos[id].Z()*m_pmt_front_r/m_pmt_center_r;

  double dx = (x-pmt_pos_x)/1000;  // m
  double dy = (y-pmt_pos_y)/1000;  // m
  double dz = (z-pmt_pos_z)/1000;  // m

  double r0 = TMath::Sqrt(x*x+y*y+z*z)/1000;            // vertex radius
  double dist = TMath::Sqrt(dx*dx+dy*dy+dz*dz);         // distance between pmt and vertex

  double cos_theta = (m_pmt_front_r*m_pmt_front_r+dist*dist-r0*r0)/(2*m_pmt_front_r*dist);

  double theta = TMath::ACos(cos_theta);

  double dist_buffer = m_pmt_front_r*cos_theta-TMath::Sqrt(
                       m_ls_r*m_ls_r-m_pmt_front_r*m_pmt_front_r*TMath::Sin(theta)*TMath::Sin(theta));

  return (dist-dist_buffer)*SPEED_LS+dist_buffer*SPEED_BUFFER;

}

bool
PushAndPullAlg::searchEne()
{
  calExpectPe(x_rec,y_rec,z_rec);
  ene_rec = m_experiment_total / m_expect_total;
  LogDebug << "m_expect_total:" << m_expect_total << std::endl;
  LogDebug << "m_experiment_total:" << m_experiment_total << std::endl;
  double r3 = TMath::Power(r_rec,3)/1e9;
  double cut = 4000;
  if (m_det_type == "Balloon")
    cut = 4500;
  else if (m_det_type == "Acrylic")
    cut = 3900;
  if (r3<cut) 
    ene_rec = ene_rec / m_ene_cor1 -> Eval(r3);
  else 
    ene_rec = ene_rec / m_ene_cor2 -> Eval(r3);
    
  ene_gamma = ene_rec;
//  ene_rec = m_ene_nonl -> GetX(ene_rec*9.35802157999999995e-01);
  ene_rec = m_ene_nonl -> GetX(ene_rec*5.10565190450509654e+00);
  return true;
}

void
PushAndPullAlg::calExpectPe(double x, double y,double z)
{
  m_pmt_expect.clear();
  double pmt_pos_x;
  double pmt_pos_y;
  double pmt_pos_z;
 
  x /= 1000;  
  y /= 1000;  
  z /= 1000;  

  double dx;
  double dy;
  double dz;
  m_expect_total = 0.0;

  for (int i=0;i<m_pmt_num; ++i)
  {
    pmt_pos_x = m_pmt_pos[i].X() / 1000;
    pmt_pos_y = m_pmt_pos[i].Y() / 1000;
    pmt_pos_z = m_pmt_pos[i].Z() / 1000;

    dx = x - pmt_pos_x;
    dy = y - pmt_pos_y;
    dz = z - pmt_pos_z;
    double dist = TMath::Sqrt(dx*dx+dy*dy+dz*dz);
    double r0 = TMath::Sqrt(x*x+y*y+z*z);
    double cos_theta = (dist*dist+m_pmt_center_r*m_pmt_center_r-r0*r0)/(2*dist*m_pmt_center_r);

    double expect_pe = cos_theta*m_pmt_r*m_pmt_r/(4*dist*dist) * TMath::Exp(-dist*m_eff_att_length);
    m_pmt_expect.push_back(expect_pe);
    m_expect_total += expect_pe;
  }

}

