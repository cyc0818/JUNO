/*=============================================================================
#
# Author: ZHANG Kun - zhangkun@ihep.ac.cn, Diru Wu - wudr@ihep.ac.cn
# Last modified:	2022-1-17
# Filename:		RecMuonAlg.h
# Description: 
#
=============================================================================*/
#ifndef RecMuonAlg_h
#define RecMuonAlg_h 1

#include "SniperKernel/AlgBase.h"

#include "EvtNavigator/NavBuffer.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdSpmtCalibHeader.h"
#include "Event/WpCalibHeader.h"
#include "Event/TtCalibHeader.h"
#include "RecTools/PmtProp.h"
#include "RecTools/Params.h"
#include <string>

class RecGeomSvc;
class CdGeom;
class WpGeom;
class TtGeom;
class IRecMuonTool;

class RecMuonAlg : public AlgBase
{
	public:  

		RecMuonAlg(const std::string& name);
		virtual ~RecMuonAlg();

		virtual bool execute();
		virtual bool initialize();
		virtual bool finalize();


	private:

		bool iniBufSvc(); 
		bool iniGeomSvc(); 
		bool iniPmtPos(); 
		bool iniRecTool(); 

		bool freshPmtData(); 
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
		JM::NavBuffer* m_buf; 


		PmtTable m_pmtTable; 
		Params m_params; //set of parameters' key/value

		std::string m_recToolName; 
		IRecMuonTool* m_recTool; 

		bool m_flagOpPmtpos;
};

#endif // RecMuonAlg_h
