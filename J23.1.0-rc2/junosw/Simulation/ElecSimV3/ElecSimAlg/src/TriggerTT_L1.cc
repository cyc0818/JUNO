#include "TriggerTT_L1.h"

#include "Identifier/TtID.h"

#include <cmath>

TriggerTT_L1::TriggerTT_L1(){
	TM = kXY;
	SW = 16; // ns
	TW = 100; // ns
}

TriggerTT_L1::~TriggerTT_L1(){
}

void TriggerTT_L1::SetSteppingWindow(int _SW){
	SW = _SW;
}

void TriggerTT_L1::SetTimeWindow(int _TW){
	TW = _TW;
}

void TriggerTT_L1::SetTriggerMode(enum TT_L1_Trigger_mode _TM){
	TM = _TM;
}

void TriggerTT_L1::Clear(){
	PMT_list.clear();
	time_in_step.clear();
}

bool TriggerTT_L1::AddHit(int PMTnumber, int time){
	int this_time_in_step = floor(time/SW)*SW;
	if(time_in_step.size() > 0 && time_in_step[0] - this_time_in_step > TW) return false;
	PMT_list.push_back(PMTnumber);
	time_in_step.push_back(this_time_in_step);
	return true;
}

void TriggerTT_L1::RemoveFirst(){
	PMT_list.pop_front();
	time_in_step.pop_front();
}

bool TriggerTT_L1::HasTrigger(int * trigger_time){
	int n_hits = PMT_list.size();
	// Check minimum number of hits for trigger
	if(n_hits < 2) return false;
	else if(n_hits < 3 && TM == kXYT) return false;
	else if(n_hits < 4 && TM == kXYX) return false;

	int first_pmt = PMT_list[0];
	bool first_valid = false;
	int  first_time = time_in_step[0];
	if(TM == kXYT || TM == kXYX){
		for(int iPMT=1; iPMT < n_hits; iPMT++){
			if(PMT_list[iPMT] == TtID::pmt_other_end(first_pmt)){
				first_valid = true;
				first_time = time_in_step[iPMT];
			}
		}
	}
	if(TM == kXYX && !first_valid) return false;

	const int n_pmts_perp = 8;
	const int * pmts_perp = TtID::pmts_perp(first_pmt);
	bool has_perp = false;
	int perp_time = time_in_step[0] + 2*TW;
	for(int iPMT=1; iPMT < n_hits; iPMT++){
		bool is_perp = false;
		for(int i_perp = 0 ; i_perp < n_pmts_perp; i_perp++){
			if(PMT_list[iPMT] == pmts_perp[i_perp]){
				is_perp = true;
				break;
			}
		}
		if(!is_perp) continue;
		if(TM == kXY){
			has_perp = true;
			perp_time = time_in_step[iPMT];
			break;
		}
		else if(TM == kXYX){
			for(int jPMT=iPMT+1; jPMT < n_hits; jPMT++){
				if(PMT_list[jPMT] != TtID::pmt_other_end(PMT_list[iPMT])) continue;
				has_perp = true;
				if(perp_time > time_in_step[jPMT])
					perp_time = time_in_step[jPMT];
			}
		}
		else if(TM == kXYT){
			// This is the more complex case as the validation can be done by the first
			// or any other hit. First found should be used for timing.
			bool perp_valid = false;
			int jPMT;
			for(jPMT=iPMT+1; jPMT < n_hits; jPMT++){
				if(PMT_list[jPMT] != TtID::pmt_other_end(PMT_list[iPMT])) continue;
				perp_valid = true;
			}
			if(first_valid || perp_valid) has_perp = true;
			if(!first_valid && perp_valid){
				if(perp_time > time_in_step[jPMT])
					perp_time = time_in_step[jPMT];
			}
			else if(first_valid && !perp_valid){
				if(perp_time > time_in_step[iPMT])
					perp_time = time_in_step[iPMT];
			}
			else if(first_valid && perp_valid){
				if(first_time > time_in_step[jPMT]) first_time = time_in_step[jPMT];
				if(perp_time > time_in_step[iPMT]){
					perp_time = time_in_step[iPMT];
				}
			}
		}
	}

	if(has_perp && trigger_time != NULL){
		*trigger_time = std::max(perp_time,first_time) + SW;
	}

	return has_perp;
}
