#include "SniperKernel/ToolFactory.h"
#include "RecTimeLikeAlg/QcRecTool.h"

DECLARE_TOOL(QcRecTool);

QcRecTool::QcRecTool(const std::string& name):ToolBase(name)
{
	declProp("Pdf_Value", m_isElec=0);
	declProp("LS_R", LS_R=17.7);

}

QcRecTool::~QcRecTool(){
}

bool QcRecTool::configure(){
	return true;
}

void QcRecTool::rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo)
{
	double x_sum = 0;
	double y_sum = 0;
	double z_sum = 0;
	double PE_sum = 0;
	double scaleFac = 1.25;
	if (m_isElec) scaleFac = 1.45;

	for(auto* thePMT: hitpmtvector){
		if (thePMT->totq <= 0) continue;
		//if (thePMT->isdn) continue;
		x_sum = x_sum + thePMT->pos.X() * thePMT->totq;
		y_sum = y_sum + thePMT->pos.Y() * thePMT->totq;
		z_sum = z_sum + thePMT->pos.Z() * thePMT->totq;
		PE_sum = PE_sum + thePMT->totq;
	}
	double ChaCenRec_x = x_sum/PE_sum * scaleFac;
	double ChaCenRec_y = y_sum/PE_sum * scaleFac;
	double ChaCenRec_z = z_sum/PE_sum * scaleFac;

	//LogInfo << "ChaCenRec_x = " << ChaCenRec_x << ", ChaCenRec_y = " << ChaCenRec_y << ", ChaCenRec_z = " << ChaCenRec_z << std::endl;
	TVector3 m_vtx_cc(ChaCenRec_x, ChaCenRec_y, ChaCenRec_z);
	recinfo.vtx = m_vtx_cc;
}
