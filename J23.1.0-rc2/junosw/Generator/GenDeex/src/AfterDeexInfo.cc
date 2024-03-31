#include "AfterDeexInfo.h"

AfterDeexInfo::AfterDeexInfo(){
    Energy.clear();
    Pdg.clear();
    ResidualZ = 0;
    ResidualN = 0;
    DeexChannelID = 0;
    NPars = 0 ;   
    ResidualPdg = 0;
    }


std::ostream &operator<<(std::ostream & os , const AfterDeexInfo & adi){
	
	os << " ResidualZ = "<< adi.ResidualZ
	   << " ResidualN = "<< adi.ResidualN
	   << " DeexChannelID = " << adi.DeexChannelID
	   << " ResidualPdg = " << adi.ResidualPdg
	   << " NPars = " << adi.NPars
	   <<std::endl;
	
	
	for( int i = 0 ; i < adi.Energy.size(); i++){
		os << " idx = " << i
		   << " energy = "<< adi.Energy[i]
		   << " pdg_code = "<< adi.Pdg[i]
		   << std::endl;
	}

	return os;		
}

 
