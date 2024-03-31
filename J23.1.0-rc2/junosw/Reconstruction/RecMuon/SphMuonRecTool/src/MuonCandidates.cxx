/*=============================================================================
#
# Author: Artem Chukanov - chukanov@jinr.ru
# Created: 26.09.2019
# Filename: MuonCandidates.cxx
# Description:
=============================================================================*/

#include "MuonCandidates.h"
#include <TMath.h>
#include <TF2.h>
#include <TH3F.h>
#include <TCanvas.h>

#define WPCHECK_SEARCHRADIUS2 25.   // Search local maximums in circle area with this radius
#define WPCHECK_BACKGROUNDDIVIDER 6. // WP PMTs with less nPE can't be maximums
#define WPCHECK_CONDITION 3.00       // If distance between found track and WP dot less then condition – track is proved
#define CDCHECK_BACKGROUNDDIVIDER 6. // WP PMTs with less nPE can't be maximums
#define CDCHECK_SEARCHRADIUS2 36.   // Search local maximums in circle area with this radius
using namespace std;

MuonCandidates::MuonCandidates(double radius, int pmtType, bool muonCalibration)
{
  fRadius = radius;
  pi = TMath::Pi();
  fDegToRad = TMath::DegToRad();
  fRadToDeg = TMath::RadToDeg();
  fMaxSlope = 4.*pi/9;
  fPMTtype = pmtType;
  fProductionMode = false;

  if (!muonCalibration)
    fOutputFileEnergy.open("results/energy_point.txt");
}


MuonCandidates::~MuonCandidates()
{
   if (fOutputFileEnergy.is_open()) fOutputFileEnergy.close();
}


//----------------------------------------------------------------------------
void MuonCandidates::DefineMuonCandidates(TF2 *func, std::vector<std::vector<track_t>> &muonCandidates, TH2F *hEventLegendre)
{
  muonCandidates.clear();

  vector <TVector3> CDioPoints;
  vector <TVector3> WPioPoints;
  
// #ifdef _OPENMP 
// #pragma omp parallel
//   {
// #pragma omp sections
//     {
// #pragma omp section
// #endif
      // get input and output points for central detector

      GetIOpointsCD(CDioPoints, func);

// #ifdef _OPENMP
// #pragma omp section
// #endif 

      // get input and output points for water pool

      GetIOpointsWP(WPioPoints);
// #ifdef _OPENMP
//     }
//   }
// #endif

 // get tracks in CD confirmed by WP

  vector <TVector3> unusedWPioPoints;
  vector <track_t> muon_candidate;
  GetConfirmedMuonTracks(muon_candidate,CDioPoints,WPioPoints,unusedWPioPoints);

  // adding unused WP points

  for (auto& point : unusedWPioPoints) {CDioPoints.push_back(point);}

  // making list of confirmed tracks

  SearchTrackCombinations(muon_candidate, CDioPoints, muonCandidates);
  
  if (fProductionMode) return;

  fHistoCD = hEventLegendre;
  
  // set polymarkers for found maximums

  int nPoints = 0;
  double x[20], y[20];

  TPolyMarker *pm = (TPolyMarker*)fHistoCD->GetListOfFunctions()->FindObject("TPolyMarker");
  if (pm) {
    fHistoCD->GetListOfFunctions()->Remove(pm);
    delete pm;
  }

  // preliminary selected points
  
  // for (auto& evt : muonCandidates) {
  //   for (auto& track : evt) {
  //     x[nPoints] = track.first.Phi()*fRadToDeg;
  //     y[nPoints++] = 90-track.first.Theta()*fRadToDeg;
  //     x[nPoints] = track.second.Phi()*fRadToDeg;
  //     y[nPoints++] = 90-track.second.Theta()*fRadToDeg;
  //   }
  // }
  
  // pm = new TPolyMarker(nPoints, x, y);
  // pm->SetMarkerStyle(26);
  // pm->SetMarkerSize(3);
  // pm->SetMarkerColor(2);
  // fHistoCD->GetListOfFunctions()->Add(pm);

  // set polymarkers for found maximums in CD
  
  nPoints = 0;
  
  for (auto& io : CDioPoints) {
    x[nPoints] = io.Phi()*fRadToDeg;
    y[nPoints] = 90-io.Theta()*fRadToDeg;
    ++nPoints;
  }

  pm = new TPolyMarker(nPoints, x, y);
  pm->SetMarkerStyle(3);
  pm->SetMarkerSize(3);
  pm->SetMarkerColor(1);
  fHistoCD->GetListOfFunctions()->Add(pm);

  // set polymarkers for found maximums in WP

  nPoints = 0;

  for (auto& io : WPioPoints) {
    x[nPoints] = io.Phi()*fRadToDeg;
    y[nPoints++] = 90-io.Theta()*fRadToDeg;
  }

  pm = new TPolyMarker(nPoints, x, y);
  pm->SetMarkerStyle(4);
  pm->SetMarkerSize(3);
  pm->SetMarkerColor(3);
  fHistoCD->GetListOfFunctions()->Add(pm);
}


//----------------------------------------------------------------------------
void MuonCandidates::GetIOpointsCD(std::vector<TVector3> &CDioPoints, TF2 *func)
{
  // Get PMTs with local maximum values

  // for found peaks we will use effective radius

  static const double radius_eff = 16.5;
    
  CDioPoints.clear();
  fCDsignal.clear();
  
  double angleStep = 4;
  double x[2];
  double par1[1] = {0};
  double signalThreshold = 0;
  
  PmtProp signal;
  
  // fill signal vector and get threshold signal

  for (int theta = angleStep; theta < 180; theta += angleStep) {
    x[0] = fDegToRad*theta;
    double step = angleStep*fDegToRad/sin(x[0]);
    for (double phi = -pi; phi < pi; phi += step) {
      x[1] = phi;
      double cont = 0;
      for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {
	par1[0] = pmtType; cont += func->EvalPar(x, par1);
      }
      // signal.pos.SetMagThetaPhi(fRadius, x[0], x[1]);
      signal.pos.SetMagThetaPhi(radius_eff, x[0], x[1]);
      signal.q = cont;
      if (cont > signalThreshold) signalThreshold = cont;
      fCDsignal.push_back(signal);
    }
  }

  signalThreshold /= CDCHECK_BACKGROUNDDIVIDER;

  // get list of zones above threshold

  PmtTable maxCandidates;

  for (auto &itPmt : fCDsignal)
    if (itPmt.q > signalThreshold) maxCandidates.push_back(itPmt);

  for (auto &itPmt : fCDsignal) {
    for (auto ioCandidate = std::begin(maxCandidates); ioCandidate != std::end(maxCandidates); ) {
      if ((ioCandidate->pos-itPmt.pos).Mag2() <= CDCHECK_SEARCHRADIUS2 &&
	  (ioCandidate->q < itPmt.q)) {
	ioCandidate = maxCandidates.erase(ioCandidate);
	continue;
      }
      ++ioCandidate;
    }
  }

  for (auto &ioCandidate : maxCandidates) CDioPoints.push_back(ioCandidate.pos);

  if (fProductionMode) return;
  
  TH2F *histo = new TH2F("histo", "histo", 90, -180, 180, 90, -90, 90);

  for (int binx = 1; binx <= histo->GetNbinsX(); ++binx) {
    x[1] = histo->GetXaxis()->GetBinCenter(binx)*fDegToRad;
    for (int biny = 1; biny <= histo->GetNbinsY(); ++biny){
      double theta = histo->GetYaxis()->GetBinCenter(biny);
      x[0] = (90-theta)*fDegToRad;
      double cont = 0;
      for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {
	par1[0] = pmtType; cont += func->EvalPar(x, par1);
      }
      histo->SetBinContent(binx, biny, cont);
    }
  }

  int nPoints = 0;
  double x1[20], y1[20];

  for (auto& io : maxCandidates) {
    x1[nPoints] = io.pos.Phi()*fRadToDeg;
    y1[nPoints] = 90-io.pos.Theta()*fRadToDeg;
    ++nPoints;
  }

  TPolyMarker *pm = new TPolyMarker(nPoints, x1, y1);
  pm->SetMarkerStyle(3);
  pm->SetMarkerSize(3);
  pm->SetMarkerColor(1);
  
  TCanvas *canvas = new TCanvas("canvas 5", "canvas 5", 1600, 1200);
  canvas->cd(); histo->Draw("COLZ"); pm->Draw();
  canvas->Print(Form("pictures/init_points_cd_%d.png", fEventNumber));
  delete canvas;
}


//----------------------------------------------------------------------------
void MuonCandidates::GetIOpointsWP(std::vector<TVector3> &WPioPoints)
{
  // Get PMTs with local maximum values

  WPioPoints.clear();

  // get threshold signal

  int signalThreshold = 0;
  
  for (auto &itPmt : fWPsignal)
    if (itPmt.q > signalThreshold) signalThreshold = itPmt.q;

  signalThreshold /= WPCHECK_BACKGROUNDDIVIDER;

  // get list of PMT id's above threshold

  PmtTable maxCandidates;

  for (auto &itPmt : fWPsignal)
    if (itPmt.q > signalThreshold) maxCandidates.push_back(itPmt);

  for (auto &itPmt : fWPsignal) {
    for (auto ioCandidate = std::begin(maxCandidates); ioCandidate != std::end(maxCandidates); ) {
      if ((ioCandidate->pos-itPmt.pos).Mag2() <= WPCHECK_SEARCHRADIUS2 &&
	  (ioCandidate->q < itPmt.q)) {
	ioCandidate = maxCandidates.erase(ioCandidate);
	continue;
      }
      ++ioCandidate;
    }
  }

  for (auto &ioCandidate : maxCandidates) WPioPoints.push_back(ioCandidate.pos);

  if (fProductionMode) return;
  
  TH2F histo("wp signal", "wp signal", 90, -180, 180, 90, -90, 90);

  for (auto &itPmt : fWPsignal) {
    histo.Fill(itPmt.pos.Phi()*fRadToDeg,90-itPmt.pos.Theta()*fRadToDeg,itPmt.q);
  }

  int nPoints = 0;
  double x1[20], y1[20];

  for (auto& io : maxCandidates) {
    x1[nPoints] = io.pos.Phi()*fRadToDeg;
    y1[nPoints] = 90-io.pos.Theta()*fRadToDeg;
    ++nPoints;
  }

  TPolyMarker *pm = new TPolyMarker(nPoints, x1, y1);
  pm->SetMarkerStyle(3);
  pm->SetMarkerSize(3);
  pm->SetMarkerColor(1);

  TCanvas *canvas = new TCanvas("canvas 6", "canvas 6", 1600, 1200);
  canvas->cd(); histo.Draw("COL"); pm->Draw();
  canvas->Print(Form("pictures/wpsignal_%d.png", fEventNumber));
  delete canvas;
}


//----------------------------------------------------------------------------
void MuonCandidates::GetConfirmedMuonTracks(std::vector<track_t> &muonTrack, std::vector<TVector3> &CDioPoints, std::vector<TVector3> &WPioPoints, std::vector<TVector3> &unusedWPioPoints)
{
  static const double radius_acr = 16.5;
  
  // find muon tracks in CD confirmed by WP

  muonTrack.clear();

  if (WPioPoints.empty()) return;

  vector <bool> wpUsage(WPioPoints.size());

  for (auto iPoint1 = std::begin(CDioPoints); iPoint1 != std::end(CDioPoints);
       ++iPoint1)
    for (auto iPoint2 = std::next(iPoint1); iPoint2 != std::end(CDioPoints);
	 ++iPoint2) {

      // skip tracks with high angle

      double theta = ((*iPoint1)-(*iPoint2)).Theta();
      if (theta > fMaxSlope && pi - theta > fMaxSlope) continue;

      // energy deposition is related to the accrylic sphere
      // not for the PMT surface. We are doing corresponding modifications

      TVector3 v1(*iPoint1), v2(*iPoint2);

      v1.SetMag(radius_acr); v2.SetMag(radius_acr);
      
      int n_wp = 0;
      bool confirmedTrack = false;

      for (auto &wpPoint : WPioPoints) {

	// Distance between WP point and CD track

	double dist = ((wpPoint-v1).Cross(v2-v1)).Mag()/(v2-v1).Mag();

	if (dist < WPCHECK_CONDITION) {
	  wpUsage[n_wp] = true;
	  confirmedTrack = true;
	}

	++n_wp;
      }

      if (confirmedTrack) {
	if (iPoint1->z() > iPoint2->z())
	  muonTrack.push_back(std::make_pair(*iPoint1,*iPoint2));
	else
	  muonTrack.push_back(std::make_pair(*iPoint2,*iPoint1));
      }
    }

  // fill unused WP points

  for (size_t i = 0; i < wpUsage.size(); ++i)
    if (!wpUsage[i]) unusedWPioPoints.push_back(WPioPoints[i]);
}


//----------------------------------------------------------------------------
void MuonCandidates::SearchTrackCombinations(std::vector<track_t> &muonTrack, std::vector<TVector3> &CDioPoints, std::vector<std::vector<track_t>> &muonCandidates)
{
  // searching tracks with confirmed

  vector<track_t> muon_candidate;
  muonCandidates.clear();

  // if we do not have confirmed tracks then we will include all configurations

  if (muonTrack.empty()) {

    muon_candidate.clear();

    for (auto iPoint1 = std::begin(CDioPoints); iPoint1 != std::end(CDioPoints);
	 ++iPoint1)
      for (auto iPoint2 = std::next(iPoint1); iPoint2 != std::end(CDioPoints);
	   ++iPoint2)
	DefineParallelMuons(iPoint1, iPoint2, CDioPoints, muon_candidate);

    muonCandidates.push_back(muon_candidate);
  }
  else {
    for (auto& track : muonTrack) {

      muon_candidate.clear();
      muon_candidate.push_back(track);

      for (auto iPoint1=std::begin(CDioPoints); iPoint1 != std::end(CDioPoints);
	   ++iPoint1)
	for (auto iPoint2 = std::next(iPoint1); iPoint2 != std::end(CDioPoints);
	     ++iPoint2)
	  DefineParallelMuons(iPoint1, iPoint2, CDioPoints, muon_candidate);

      muonCandidates.push_back(muon_candidate);
    }

    // remove dublicated group

    size_t listSize = muonCandidates.size();
    vector<bool> trackToRemove(listSize);

    for (size_t evt1 = 0; evt1 < listSize; ++evt1) {
      for (size_t evt2 = evt1+1; evt2 < listSize; ++evt2) {

	size_t size1 = muonCandidates[evt1].size();
	size_t size2 = muonCandidates[evt2].size();

	if (size1 != size2) continue;

	size_t n_same = 0;

	for (size_t track1 = 0; track1 < size1; ++track1) {
	  TVector3 v1 = muonCandidates[evt1][track1].first - muonCandidates[evt1][track1].second;
	  double length1 = v1.Mag2();

	  for (size_t track2 = 0; track2 < size2; ++track2) {
	    TVector3 v2 = muonCandidates[evt2][track2].first - muonCandidates[evt2][track2].second;
	    double length2 = v2.Mag2();

	    if (abs(length1-length2) < 0.000001) ++n_same;
	  }
	}

	if (n_same == size1) trackToRemove[evt1] = true;
      }
    }

    for (int i = trackToRemove.size() - 1; i >= 0; --i) {
      if (trackToRemove[i]) muonCandidates.erase(muonCandidates.begin()+i);
    }
  }

  // for (auto& evt : muonCandidates) {
  //   cout << "muon track" << endl;
  //   for (auto& track : evt) {
  //     track.first.Print();
  //     track.second.Print();
      
  //     track.first.SetMag(fRadius);
  //     track.second.SetMag(fRadius);
  //   }
  // }

  // making modification to extrapolate preliminary selected muon tracks
  // from accrylic and water pool PMT spheres to PMT sphere

  for (auto& evt : muonCandidates) {
    for (auto& track : evt) {

      TVector3 initMuon = track.second - track.first;
      TVector3 m1H = track.first-initMuon*(track.first.Dot(initMuon)/initMuon.Mag2());
      double dist2 = m1H.Mag2();

      TVector3 v1 = track.first - m1H;
      TVector3 v2 = track.second - m1H;

      // check the condition that the m1H vector is between init and output
      // points

      if ((v1.z() > 0 && v2.z() < 0) || (v1.z() < 0 && v2.z() > 0)) { 
     
	double k1 = (fRadius*fRadius-dist2)/(track.first.Mag2() - dist2);
	double k2 = (fRadius*fRadius-dist2)/(track.second.Mag2() - dist2);

	k1 = (k1 > 0) ? sqrt(k1) : 1;
	k2 = (k2 > 0) ? sqrt(k2) : 1;

	// back to the PMT sphere radius with modified theta and phi

	track.first  = v1*k1 + m1H;
	track.second = v2*k2 + m1H;
      }
      else {track.first.SetMag(fRadius); track.second.SetMag(fRadius);}
    }
  }

  for (auto& evt : muonCandidates) {
    for (auto& track : evt) {
      track.first.SetMag(fRadius);
      track.second.SetMag(fRadius);
    }
  }
  
  // sort muon candidate vector

  std::sort(muonCandidates.begin(), muonCandidates.end(),
	    IncreasingTracksNumber());
}


//----------------------------------------------------------------------------
void MuonCandidates::DefineParallelMuons(vector<TVector3>::iterator &point1,
					 vector<TVector3>::iterator &point2,
					 vector<TVector3> &points,
					 vector<track_t> &muon_candidate)
{
  // condition for two parallel tracks
  static const double parallel_angle = 0.25; // rad
  // we need to define max angle slope for muons, choose 80 degree
  static const double max_slope = 4*pi/9; // rad

  // check if we have the same point

  for (auto &track : muon_candidate)
    if ((track.first-(*point1)).Mag2() < 0.00001 ||
	(track.first-(*point2)).Mag2() < 0.00001 ||
	(track.second-(*point1)).Mag2() < 0.00001 ||
	(track.second-(*point2)).Mag2() < 0.00001) return;

  // check if the slope of muon track is too high

  TVector3 vtrack = (*point1)-(*point2);
  double theta = vtrack.Theta();

  if (theta > max_slope && pi - theta > max_slope) return;

  // check the angle between all vectors and add the new and keep only
  // parallel tracks

  for (auto &track : muon_candidate) {
    double angle = vtrack.Angle(track.first - track.second);
    if (angle > parallel_angle && pi - angle > parallel_angle) return;
  }

  if (point1->z() > point2->z())
    muon_candidate.push_back(make_pair(*point1, *point2));
  else
    muon_candidate.push_back(make_pair(*point2, *point1));

  for (auto iPoint3=std::next(point1); iPoint3 != std::end(points); ++iPoint3)
    for (auto iPoint4=std::next(iPoint3); iPoint4 != std::end(points); ++iPoint4)
      DefineParallelMuons(iPoint3, iPoint4, points, muon_candidate);
}


//----------------------------------------------------------------------------
int MuonCandidates::DefineNumberOfMuons(double par[], int n_tracks)
{
  static const double R2 = fRadius*fRadius;

  vector <track_t> muonTrack;
  muonTrack.clear();

  // define input and output points from found muon track parameters

  for (int j = 0; j < n_tracks; ++j) {

    // get incoming and outgoing points

    double z = sqrt(R2 - par[j*5]*par[j*5]);
    TVector3 inPoint(par[j*5], 0, z);
    TVector3 outPoint(par[j*5], 0, -z);

    inPoint.RotateZ(-par[j*5+2]); // around Oz
    inPoint.RotateX(-par[j*5+3]); // around Ox'
    inPoint.RotateZ(-par[j*5+1]); // around Oz'

    outPoint.RotateZ(-par[j*5+2]); // around Oz
    outPoint.RotateX(-par[j*5+3]); // around Ox'
    outPoint.RotateZ(-par[j*5+1]); // around Oz'

    muonTrack.push_back(std::make_pair(inPoint, outPoint));
  }

  int trackToRefit = EnergyDepositionPoints(muonTrack);

  return trackToRefit;
}


//----------------------------------------------------------------------------
int MuonCandidates::EnergyDepositionPoints(vector<track_t> &muonTrack)
{
  static const double radius2_eff = 16.5*16.5;
  // static const double radius2_eff = 17.7*17.7;

  // function for dependencies of energy deposition form track distance

  int track_number = -1;
  vector <TVector3> points;
  vector <double> distance2;
  points.clear();

  for (auto &track : muonTrack) {

    // energy deposition is related to the accrylic sphere
    // not for the PMT surface. We are doing corresponding modifications
    // for the theta and phi

    TVector3 initMuon = track.second - track.first;
    TVector3 m1H = track.first-initMuon*(track.first.Dot(initMuon)/initMuon.Mag2());
    double dist2 = m1H.Mag2();

    double k1 = (radius2_eff-dist2)/(track.first.Mag2() - dist2);
    double k2 = (radius2_eff-dist2)/(track.second.Mag2() - dist2);

    if (k1 > 0 && k2 > 0) {k1 = sqrt(k1); k2 = sqrt(k2);}
    else k1 = k2 = 1;

    // back to the PMT sphere radius with modified theta and phi

    points.push_back((track.first - m1H)*k1 + m1H);
    points.back().SetMag(fRadius); 
    points.push_back((track.second - m1H)*k2 + m1H);
    points.back().SetMag(fRadius);

    distance2.push_back(dist2);
  }

  // get sum of signal for each point

  vector <double> sum(points.size());

  for (size_t i = 0; i < points.size(); ++i) {

    int nPoints = 0;
    sum[i] = 0;

    for (auto &itPmt : fCDsignal) {

      if ((points[i]-itPmt.pos).Mag2() < CDCHECK_SEARCHRADIUS2) {
	sum[i] += itPmt.q;
	++nPoints;
      }
    }
    
    sum[i] /= 1.*nPoints;
  }

  // check sum of energy deposition for each track and get first candidate
  // for possible two neighbouring tracks

  double npoints[2];

  for (size_t i = 0; i < points.size(); i += 2) {

    npoints[0] = npoints[1] = 1;

    double sum_track = sum[i] + sum[i+1];
    double energyTwoMuons = 6500;

    if (sum_track > energyTwoMuons) {

      // look if we have a reconstructed point around found

      for (size_t j = 0; j < points.size(); ++j) {

	if (j == i || j == i+1) continue;

	if ((points[i] - points[j]).Mag2() < CDCHECK_SEARCHRADIUS2) ++npoints[0];
	else if ((points[i+1] - points[j]).Mag2() < CDCHECK_SEARCHRADIUS2) ++npoints[1];
      }

      sum_track = sum[i]/npoints[0] + sum[i+1]/npoints[1];

      if (sum_track > energyTwoMuons) {track_number = i/2; break;}
    }
  }

  return track_number;
}


//----------------------------------------------------------------------------
void MuonCandidates::EnergyDepositionMuonTrue(vector <trackSim_t> muonSim)
{
  static const double radius2_eff = 16.5*16.5;
  // static const double radius2_eff = 17.7*17.7;

  // function for dependencies of energy deposition form track distance

  vector <TVector3> points;
  vector <double> distance2;
  points.clear();

  for (auto &iMuon : muonSim) {
    
    // energy deposition is related to the accrylic sphere
    // not for the PMT surface. We are doing corresponding modifications
    // for the theta and phi
    
    TVector3 initMuon = iMuon.initMuonPosition - iMuon.endMuonPosition;
    TVector3 m1H = iMuon.initMuonPosition-initMuon*(iMuon.initMuonPosition.Dot(initMuon)/initMuon.Mag2());
    double dist2 = m1H.Mag2();

    double k1 = (radius2_eff-dist2)/(iMuon.initMuonPosition.Mag2() - dist2);
    double k2 = (radius2_eff-dist2)/(iMuon.endMuonPosition.Mag2() - dist2);

    if (k1 > 0 && k2 > 0) {k1 = sqrt(k1); k2 = sqrt(k2);}
    else k1 = k2 = 1;

    // back to the PMT sphere radius with modified theta and phi

    points.push_back((iMuon.initMuonPosition - m1H)*k1 + m1H);
    points.back().SetMag(fRadius); 
    points.push_back((iMuon.endMuonPosition - m1H)*k2 + m1H);
    points.back().SetMag(fRadius);

    distance2.push_back(dist2);
  }

  // get sum of signal for each point

  vector <double> sum(points.size());

  for (size_t i = 0; i < points.size(); ++i) {

    int nPoints = 0;
    sum[i] = 0;

    for (auto &itPmt : fCDsignal) {

      if ((points[i]-itPmt.pos).Mag2() < CDCHECK_SEARCHRADIUS2) {
	sum[i] += itPmt.q;
	++nPoints;
      }
    }
    
    sum[i] /= 1.*nPoints;
  }

  // write energy deposition to file

  for (size_t i = 0; i < points.size(); i += 2) {

    double sum_track = sum[i] + sum[i+1];

    if (fOutputFileEnergy.is_open()) {

      // get minimum distance for the neighbour input/output point

      double dist_min = 1000000;
      
      for (size_t j = 0; j < points.size(); ++j) {
	if (j == i || j == i+1) continue;
	dist_min = TMath::Min(dist_min, (points[i]-points[j]).Mag2());
	dist_min = TMath::Min(dist_min, (points[i+1]-points[j]).Mag2());
      }
      
      fOutputFileEnergy << distance2[i/2] << " " << sum_track << " "
			<< sqrt(dist_min) << endl;
    }
  }
}


//----------------------------------------------------------------------------
void MuonCandidates::SetWPsignal(PmtTable &pmtTable)
{
  fWPsignal.clear();

  for (auto &itPmt : pmtTable) {
    if (itPmt.used && itPmt.loc == 2) {
      fWPsignal.push_back(itPmt);
      fWPsignal.back().pos *= 0.001;
    }
  }
}
