#ifndef HEPEVT_DATA_HH
#define HEPEVT_DATA_HH
#include <vector>
#include <array>

struct hepevt_data{

	hepevt_data();
	std::vector<int> m_pdgs;
	std::vector<std::array<double, 3>>  m_moms; // record momentum
	std::vector<double> m_masss;
	std::vector<int> m_statuss;
	
	int get_par_num() const;
	void add_one_par(int pdg, double px, double py, double pz, double mass);
	void clear();
	void reset_iso_status(int status);
	

};

inline hepevt_data::hepevt_data(){
	m_pdgs.clear();
	m_moms.clear();
	m_masss.clear();
	m_statuss.clear();
}


inline int hepevt_data::get_par_num() const {
	assert( m_pdgs.size() == m_moms.size());
	assert( m_masss.size() == m_moms.size());
	assert( m_statuss.size() == m_moms.size());
	return m_pdgs.size();

}
inline void hepevt_data::reset_iso_status(int status){
	for(int i = 0; i < m_pdgs.size(); i++){
		int pdg = m_pdgs[i];
		//check if it is isotope.
		if( pdg > 1000000000){
			m_statuss[i] = status;
		}
	}
}

inline void hepevt_data::add_one_par(int pdg, double px, double py, double pz, double mass){
	std::array<double, 3> mom = { px, py, pz };
	m_pdgs.push_back(pdg);
	m_moms.push_back(mom);
	m_masss.push_back(mass);
	m_statuss.push_back(1); //default is 1.
}

inline void hepevt_data::clear(){
	m_pdgs.clear();
	m_moms.clear();
	m_masss.clear();
	m_statuss.clear();
}

#endif
