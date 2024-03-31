/*=============================================================================
#
# Author: ZHANG Kun - zhangkun@ihep.ac.cn, Diru Wu - wudr@ihep.ac.cn
# Last modified:	2022-1-17
# Filename:		nClusterMuonClassificationTool.h
# Description: 
#
=============================================================================*/
#ifndef nClusterMuonClassificationTool_h
#define nClusterMuonClassificationTool_h 1

#include "SniperKernel/ToolBase.h"
#include "SniperKernel/AlgBase.h"
#include "ClassificationTool/IClassificationTool.h"

#include "EvtNavigator/NavBuffer.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdSpmtCalibHeader.h"
#include "Event/WpCalibHeader.h"
#include "Event/TtCalibHeader.h"
#include "RecTools/PmtProp.h"
#include "RecTools/Params.h"
#include <string>

#include "TSpectrum2.h"
#include "TH2F.h"

class RecGeomSvc;
class CdGeom;
class WpGeom;
class TtGeom;
class IRecMuonTool;

class nClusterMuonClassificationTool : public ToolBase, public IClassificationTool
{
	public:

		nClusterMuonClassificationTool(const std::string& name);
		virtual ~nClusterMuonClassificationTool();
		ClassificationType classify(JM::NavBuffer* buffer);
		bool configure();

		double PMTThreshold_pe;
		int PMTThreshold_n;
		int maxpositions = 7;
		int AroundPePlus_DisCut = 5000;

	private:

		bool iniGeomSvc(); 
		bool iniPmtPos(); 

		bool freshPmtData(JM::NavBuffer* buffer); 
		bool freshPmtDataHelper(const std::list<JM::CalibPmtChannel*>& chlist);
		bool freshPmtDataHelper(const std::list<JM::CalibTtChannel*>& ttlist);

	private:

		int m_iEvt; 

		unsigned int m_totPmtNum; 

		double m_sigmaPmt3inch;
		double m_sigmaPmt20inch;
		double m_sigmaPmtTT;

		bool m_flagUse3inch;
		bool m_flagUse20inch;

		bool CdUsed;
		bool WpUsed;
		bool TtUsed;

		int m_chosenDetectors;

		CdGeom*  m_cdGeom; 
		WpGeom* m_wpGeom;
		TtGeom* m_ttGeom;
		std::vector<unsigned int> m_offsets;

		std::vector<double> PmtPe;//the charge of picked pmt
		std::vector<double> OptPmtPe;//the charge of picked pmt
		std::vector<double> vpe_around;
		std::vector<TVector3> PmtPos;
		std::vector<TVector3> OptPmtPos;
		std::vector<TVector3> vfitp;
		std::vector<TVector3> rectracks;
		std::vector<TVector3> qwpoints;//charge weight center
		std::vector<TVector3> reqwpoints;//charge weight center
		std::vector<TVector3> vrec_in;//charge weight center
		std::vector<TVector3> vrec_out;//charge weight center


		PmtTable m_pmtTable; 
		Params m_params; //set of parameters' key/value

		std::string m_recToolName; 

		bool m_flagOpPmtpos;
};

#endif // nClusterMuonClassificationTool_h
