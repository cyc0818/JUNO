#include "SniperKernel/ToolFactory.h"

#include "TMath.h"
#include "CdWpFHTMuonRecTool.h"
#include "Chi2TimeSimple.h"
#include "Chi2TimeSimple2.h"
#include "Correction.h"

#include "Math/Factory.h"
#include "Math/Functor.h"

#include "EvtNavigator/NavBuffer.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"

DECLARE_TOOL(CdWpFHTMuonRecTool);

std::ostream& operator << (std::ostream& s, const TVector3& v){
	s << "(" << v.x() <<  "," << v.y() << "," << v.z() << ")";
	return s;
}
	CdWpFHTMuonRecTool::CdWpFHTMuonRecTool(const std::string& name)
	:ToolBase(name)
	,m_evtid(0)
	 ,m_oriptable(NULL)
{
	declProp("FhtCorrFile", m_corrfilename="");
	declProp("LSRadius", m_LSRadius = 17700);
	declProp("LightSpeed", m_clight =299.792458);
	declProp("LSRefraction", m_nLSlight = 1.485);
	declProp("MuonSpeed", m_vmuon = 299.792458);
	declProp("FhtCorrMethod",m_corrmethod="nocorr");
	declProp("NumBinsDist",m_nBinDist = 18);
	declProp("RelativeTimeCut",m_relativetimecut=140);
	declProp("ChargeCut",m_qcut=0);
	declProp("NPmtRequired",m_pmtUsedNumCut = 100);
	m_minuit = ROOT::Math::Factory::CreateMinimizer("Minuit2","Migrad");
	//m_minuit = ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "ConjugateFR");
}

CdWpFHTMuonRecTool::~CdWpFHTMuonRecTool(){
	delete m_minuit;
}

bool
CdWpFHTMuonRecTool::configure(const Params* pars, const PmtTable* ptab){
	LogDebug<< " config reconstructing tool [CdWpFHTMuonRecTool] "
		<<std::endl;

	m_oriptable = ptab;

	if (m_corrmethod == "corr2dtad"){
		m_fhtcorrtool = new Correctiond2ad("corr2ad");
	}
	else if (m_corrmethod == "corr2dtqd"){
		m_fhtcorrtool = new Correctiond2qd("corr2qd");
	}
	else if (m_corrmethod == "corr1dtqdqd") {
		m_fhtcorrtool = new Correctiond1qdqd("corr1qdqd");
	}
	else if (m_corrmethod == "corr1dtdqdq") {
		m_fhtcorrtool = new Correctiond1dqdq("corr1qdqd");
	}
	else if (m_corrmethod == "corr1dtadad") {
		m_fhtcorrtool = new Correctiond1adad("corr1adad");
	}
	else if (m_corrmethod == "corr1dtdada") {
		m_fhtcorrtool = new Correctiond1dada("corr1dada");
	}
	else if (m_corrmethod == "corr1dtq") {
		m_fhtcorrtool = new Correctiond1q("corr1q");
	}
	else if (m_corrmethod == "nocorr") {
	}
	else{
		LogError<<" Please set right fht correction method"<<std::endl;
		return false;
	}


	//m_fhtcorrtool = dynamic_cast<IFhtCorrection*>(DLEFactory::instance()->create(m_corrmethod));
	if (m_fhtcorrtool == NULL){
		LogError<< " Failed to create fht correction tool, make sure the tool name is right ["<<
			m_corrmethod<<"] "<<std::endl;
	}
	if(not m_fhtcorrtool->setCorrectionFileAndNumBinDist(m_corrfilename,m_nBinDist)) return false;

	m_chi2func = new Chi2TimeSimple(m_ptable);
	m_chi2func->setLightSpeed(m_clight);
	m_chi2func->setLSRefraction(m_nLSlight);
	m_chi2func->setMuonSpeed(m_vmuon);
	m_minuit->SetMaxFunctionCalls(1000000);
	m_minuit->SetMaxIterations(100000);
	m_minuit->SetTolerance(0.001);
	ROOT::Math::Functor *fcn = new ROOT::Math::Functor(*m_chi2func,5);
	m_minuit->SetFunction(*fcn);

	return true;

}

#include <fstream>
void outputpmtinfo(const PmtTable& oriptab,const PmtTable& ptab, std::ofstream& f){
	for(unsigned int i=0;i<ptab.size();i++){
		if (ptab[i].used){
			f<<i<<" "<<ptab[i].q<<" "<<oriptab[i].fht<<" "<<ptab[i].fht<<std::endl;
		}
	}
}
bool 
CdWpFHTMuonRecTool::reconstruct(RecTrks* trks){
	LogDebug<< " reconstructing ... "<< std::endl;
	m_evtid++;
	m_ptable.assign(m_oriptable->begin(),m_oriptable->end());

	TVector3 ri, dir;
	double ti,totalpe; 
	int numPmtUsed = pmtCut();
	iniargs(ri,ti,dir,totalpe);

	double thei = ri.Theta();
	double phii = ri.Phi();
	double theTrk = dir.Theta();
	double phiTrk = dir.Phi();

	double deg= TMath::Pi()/180;

	m_minuit->SetVariable(0, "thei",  thei,  1*deg);
	m_minuit->SetVariable(1, "phii",  phii,  1*deg);
	m_minuit->SetVariable(2, "ti",    ti,    5    );
	m_minuit->SetVariable(3, "theTrk",theTrk,1*deg);
	m_minuit->SetVariable(4, "phiTrk",phiTrk,1*deg);

	m_minuit->SetPrintLevel(0);

	if(numPmtUsed<m_pmtUsedNumCut){
		LogWarn << " Number of pmts after cut is less than "<< m_pmtUsedNumCut<<", skip the event."
			<<std::endl;
		return true;
	}
	m_minuit->Minimize();
	const double *x ;
	double chi2byndf ;
	x = m_minuit->X();
	chi2byndf= m_minuit->MinValue();

	TVector3 ri_rec, dir_rec, re_rec;
	double ti_rec,te_rec,tl_rec;
	ri_rec.SetMagThetaPhi(m_LSRadius,x[0],x[1]);
	ti_rec = x[2];
	dir_rec.SetMagThetaPhi(1,x[3],x[4]);
	tl_rec = fabs(ri_rec*dir_rec)*2;
	re_rec = ri_rec + dir_rec*tl_rec;
	te_rec = ti_rec + tl_rec/m_vmuon;


	if (m_corrmethod != "nocorr")
		for (int irec=0; irec<3; irec++){
			corrpmtinfo(ri_rec,dir_rec);
			//std::ofstream outf("pmtinfo.txt",std::ios::app);
			//outputpmtinfo(*m_oriptable,m_ptable,outf);
			m_minuit->Minimize();
			x = m_minuit->X();
			chi2byndf= m_minuit->MinValue();

			ri_rec.SetMagThetaPhi(m_LSRadius,x[0],x[1]);
			ti_rec = x[2];
			dir_rec.SetMagThetaPhi(1,x[3],x[4]);
			tl_rec = fabs(ri_rec*dir_rec)*2;
			re_rec = ri_rec + dir_rec*tl_rec;
			te_rec = ti_rec + tl_rec/m_vmuon;

		}

	TVector3 start(ri_rec[0],ri_rec[1],ri_rec[2]);
	TVector3 end(re_rec[0],re_rec[1],re_rec[2]);
	trks->addTrk(start, end, ti_rec, te_rec, totalpe, chi2byndf, 0);

	// CLHEP::HepLorentzVector start(ri_rec[0],ri_rec[1],ri_rec[2], ti_rec);
	// CLHEP::HepLorentzVector end(re_rec[0],re_rec[1],re_rec[2], te_rec);
	// JM::RecTrack* rectrk = new JM::RecTrack(start,end);
	// rectrk->setQuality(chi2byndf);
	// rectrk->setPESum(totalpe);
	// JM::CDTrackRecEvent* evt = new JM::CDTrackRecEvent();
	// evt->addTrack(rectrk);
	// rh->setCDTrackEvent(evt);

	double halfl,dist ;
	halfl= fabs(dir_rec*ri_rec);
	dist = TMath::Sqrt(m_LSRadius*m_LSRadius-halfl*halfl);
	LogDebug << "-----------------------------------" << std::endl
		<<"Recon result : " << std::endl
		<<"total pe         : "<<totalpe<<std::endl
		<<"chi2/ndf         : "<< chi2byndf << std::endl
		<<"ri_theta(deg)    : "<<x[0]/deg<<std::endl
		<<"ri_phi(deg)      : "<<x[1]/deg<<std::endl
		<<"ti(ns)           : "<<ti_rec<<std::endl
		<<"dir_theta(deg)   : "<<x[3]/deg<<std::endl
		<<"dir_phi(deg)     : "<<x[4]/deg<<std::endl
		<<"trkl(mm)         : "<<2*halfl<<std::endl
		<<"dist(mm)         : "<<dist<<std::endl
		<<"dir              : "<<dir_rec<<std::endl
		<<"start:"<<ri_rec<<std::endl
		<<"end:"<<re_rec<<std::endl
		;

	m_minuit->Clear();

	return true;
}

void 
CdWpFHTMuonRecTool::corrpmtinfo(TVector3& ri,TVector3& dir){
	if(m_corrmethod == "nocorr"){
		return;
	}
	PmtTable& ptab = m_ptable;
	const PmtTable& oriptab = *m_oriptable;
	unsigned int pmtNum = ptab.size();
	for(unsigned int ipmt=0; ipmt<pmtNum; ipmt++){
		if(not ptab[ipmt].used)continue;
		if (ptab[ipmt].loc == 2) continue;
		double corr = m_fhtcorrtool->correction(ptab[ipmt],ri,dir);
		double nouse;
		double error = m_fhtcorrtool->sigma(ptab[ipmt],ri,dir,&nouse);
		ptab[ipmt].fht=oriptab[ipmt].fht-corr;
		ptab[ipmt].res = error;
	}
}
	void 
CdWpFHTMuonRecTool::iniargs(TVector3 & ri, double & ti,
		TVector3 & direction, double& pesum)// output
{
	TVector3 Rchgcen(0, 0, 0); //charge center of all pmt signals
	double tempCharge=0 ;

	double chargetotal=0;
	int numPmtUsed=0;
	PmtTable& ptab = m_ptable;
	ti = ptab[0].fht;
	unsigned int pmtnum = ptab.size();
	double qMax = 0;
	for (unsigned int i = 0; i < pmtnum; ++i)
	{
		if(ptab[i].used == 1){
			if (ptab[i].loc == 1) {
				Rchgcen += ptab[i].pos*ptab[i].q;
				chargetotal += ptab[i].q;
			}
			// if (ptab[i].q > qMax && ptab[i].loc == 2) {
			// 	ti = ptab[i].fht;
			// 	ri = ptab[i].pos;
			// 	qMax = ptab[i].q;
			// }
			if (ptab[i].fht<ti && ptab[i].loc == 1)
			{
				ti = ptab[i].fht;
				ri = ptab[i].pos;
			}
			//find the max. charge PMT
			if(ptab[i].q>tempCharge && ptab[i].loc == 1)  {

				tempCharge = ptab[i].q;
				//ri = ptab[i].pos;
			}
			numPmtUsed++;
			//   if (ptab[i].loc == 2)
		}
	}
	Rchgcen = Rchgcen * (1/chargetotal);
	direction = (Rchgcen-ri).Unit();
	// TVector3 mid = ri + direction * (-ri) * direction;
	// TVector3 tmpRi = mid - direction * TMath::Sqrt(m_LSRadius * m_LSRadius - mid.Mag2());
	// ti += (tmpRi - ri).Mag();
	// ri = tmpRi;
	TVector3 re = 2*Rchgcen - ri;
	pesum = chargetotal;

	// direction = direction.Unit();
	// double shift = (- ri * direction) - (m_LSRadius * m_LSRadius - (TMath::Power(ri.Mag(), 2) - TMath::Power(ri * direction, 2)));
	// ri = shift * direction + ri;
	// ti += shift / m_vmuon;

	LogDebug  << std::endl
		<<"Pmt used count: " << numPmtUsed << std::endl
		<<"Inital argument: " << std::endl
		<< " ti:" << ti << std::endl
		<< " ri:" << ri << std::endl
		<< " re:" << re << std::endl
		<< " the_in:" << ri.Theta()*180/TMath::Pi() << std::endl
		<< " phi_in:" << ri.Phi()*180/TMath::Pi() << std::endl
		<< " theTrk:" << direction.Theta()*180/TMath::Pi() << std::endl
		<< " phiTrk:" << direction.Phi() *180/TMath::Pi()<< std::endl
		<< " total pe : "<<chargetotal<<std::endl
		<< " charge center:"<< Rchgcen <<"  Dist:"<<Rchgcen.Mag()<<std::endl;
}

int 
CdWpFHTMuonRecTool::pmtCut(){
	PmtTable& ptab = m_ptable;
	unsigned int pmtNum = ptab.size();
	double earliest_fht  = ptab[0].fht;
	double wpfirstHit = ptab[43213].fht;
	double qmax = 0;
	for (unsigned int ipmt = 0; ipmt < pmtNum; ++ipmt) {
		if(ptab[ipmt].loc == 1 && ptab[ipmt].fht<earliest_fht)
			earliest_fht = ptab[ipmt].fht;
		if (ptab[ipmt].loc == 2 && ptab[ipmt].fht < wpfirstHit)
			wpfirstHit = ptab[ipmt].fht;
		if (ptab[ipmt].loc == 2 && ptab[ipmt].q > qmax)
			qmax = ptab[ipmt].q;
		if(ptab[ipmt].q<m_qcut)
			ptab[ipmt].used = false;
	}
	LogInfo << "Max charge in the pool is: " << qmax << endl;
	cdfirstHit = earliest_fht;
	int pmtNumUsed = 0;
	for (unsigned int ipmt = 0; ipmt < pmtNum; ++ipmt) {
		if(ptab[ipmt].loc == 1 && ptab[ipmt].fht-earliest_fht>m_relativetimecut)
			ptab[ipmt].used = false;
		if (ptab[ipmt].loc == 2 && (ptab[ipmt].fht - wpfirstHit > 50 || ptab[ipmt].q < 10))
			ptab[ipmt].used = false;
		if(ptab[ipmt].used && ptab[ipmt].loc == 1)
			pmtNumUsed ++;
	}

	LogDebug<<" pmt number used after cut : "<< pmtNumUsed<<std::endl;
	return pmtNumUsed;
}


