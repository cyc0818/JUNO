#include "NucUtil.h"
#include "NucState.h"
#include "NucDecay.h"
#include "Radiation.h"

#include "SniperKernel/SniperLog.h"
#include "CLHEP/Units/SystemOfUnits.h"

#include <sstream>
#include <more/phys/ens.h>

using namespace GenDecay;
using namespace std;
using namespace more;
using namespace more::phys;

double GenDecay::more_to_clhep_time(double more_time)
{
    return more_time * CLHEP::second / SI::s;
}
double GenDecay::more_to_clhep_energy(double more_energy)
{
    return more_energy * CLHEP::MeV / SI::MeV;
}

template<typename T>
bool operator==(confidence_interval<T> x, confidence_interval<T> y)
{
    const double epsilon = 20*SI::eV;
    return (fabs(x.cent()-y.cent()) < epsilon);
    //&& (fabs(x.min()-y.min()) < epsilon)
    //&& (fabs(x.max()-y.max()) < epsilon);
}
template<typename T>
bool operator!=(confidence_interval<T> x, confidence_interval<T> y)
{
    return !(x==y);
}

typedef map<const phys::nucleus, ens::nucleus> NucNucMap_t;
bool GenDecay::get_nucleus(const more::phys::nucleus& pn, more::phys::ens::nucleus& en)
{
    static NucNucMap_t nucnuc;

    ens::nucleus out = nucnuc[pn];
    if (out.has_dataset()) {
        en = out;
        return true;
    }

    try {
        out = pn;
    }
    catch (std::runtime_error) {
        cerr << "Caught run time error when assigning nucleus: \"" << pn << "\"" << endl;
        cerr << "Is the fetcher used a few lines above the correct one?\n"
             << "Maybe you need to set MORE_PHYS_FETCHER to point to the fetcher script?\n";            
        return false;
    }
    en = out;
    // FIXME if the cache is used, sometimes the python can't exit.
    // nucnuc[pn] = out;
    return true;
}

// NucState factory.
typedef map<phys::nucleus, vector<NucState*> > Nuc2state_t;
Nuc2state_t gsNuc2state;


NucState* GenDecay::get_state(const phys::nucleus& n, ens::confiv_t hl,
                              ens::confiv_t rel, int ref)
{
    Nuc2state_t::iterator it = gsNuc2state.find(n);

    // No such nucleus seen yet
    if (it == gsNuc2state.end()) {
        vector<NucState*> vns;
        NucState* ns = new NucState(n,hl,rel,ref);
        vns.push_back(ns);
        gsNuc2state[n] = vns;
        
        //cerr << "get_state: Made first ever " << *ns << "(@" 
        //     << (void*)ns << ")" << endl;
        return ns;
    }

    // nucleus seen, check for specific NucState
    vector<NucState*>& vns = it->second;
    for (size_t ind=0; ind < vns.size(); ++ind) {
        if (vns[ind]->eref() != ref) {
            //cerr << "get_state: No match: " << vns[ind]->eref() << " != " << ref 
            //     << endl;
            continue;
        }
        if (vns[ind]->erel() != rel) {
            //cerr << "get_state: No match: " << vns[ind]->erel() / SI::keV 
            //     << " != " << rel / SI::keV << endl;
            continue;
        }
        // found it
        //cerr << "get_state: in slot " << ind << " found "
        //     << *vns[ind] << "(@" << (void*)vns[ind] << ")" << endl;
        return vns[ind];
    }

    // Specific state not yet seen, add it.
    NucState* ns = new NucState(n,hl,rel,ref);
    vns.push_back(ns);
    //cerr << "get_state: added new " << *ns << "(@" << (void*)ns << ")" << endl;
    return ns;
}

static
void normalize_branching_fractions(NucState& state)
{
    vector<NucDecay*>& decays = state.decays();

    // special case, stable or
    if (!decays.size()) return;

    // single decay branch
    if (decays.size() == 1) { 
        decays[0]->fraction = 1.0;
        return;
    }

    // Some decays have unknown (0) branching fraction, drop them.
    vector<NucDecay*> tmp;
    for (size_t dind=0; dind < decays.size(); ++dind) {
        if (decays[dind]->fraction > 0.0) {
            tmp.push_back(decays[dind]);
        }
//	else 
//	{
//	    LogDebug << "--------------------NUCUNTIL------------------------"<<"\n";
//	    LogDebug << decays[dind]->mother->nuc() << " level=" << decays[dind]->mother->energy()
//	              << " can decay to "<< decays[dind]->daughter->nuc() << " level=" << decays[dind]->daughter->energy()
//		      << " fraction:" << decays[dind]->fraction << std::endl;
//	    LogDebug << "--------------------NUCUNTIL------------------------"<<"\n";
//	}
    }
    decays = tmp;
    if (!decays.size()) return;

    if (decays.size() == 1) { 
        decays[0]->fraction = 1.0;
        return;
    }

    double mag = 0;
    for (size_t dind=0; dind < decays.size(); ++dind) {
        mag += decays[dind]->fraction;
    }

    if (mag == 0.0) {
        cerr << "Zero total decay fraction for: " << state 
             << " with " << state.ndecays() << " decays"
             << endl;
        return;
    }
    for (size_t dind=0; dind < decays.size(); ++dind) {
        decays[dind]->fraction /= mag;
    }
    //cerr << "Normalized daughters of " << state << " by " << mag << endl;
}

//typedef map<pair<NucState*,NucState*>,vector<NucDecay*>> NucDecayMap_t;
// it is static ....
static NucDecayMap_t gsNucDecays;
const NucDecayMap_t& GenDecay::get_decays() { return gsNucDecays; }

NucDecay* get_decay(NucState* mo, NucState* da, const string& typ, double q=0, double f=0);
NucDecay* get_decay(NucState* mo, NucState* da, const string& typ, double q, double f)
{
    pair<NucState*,NucState*> moda(mo,da);

    vector<NucDecay*> vec;

    NucDecayMap_t::iterator it = gsNucDecays.find(moda);
    if (it != gsNucDecays.end()) {
        vec = it->second;
    }

    for (size_t ind=0; ind<vec.size(); ++ind) {
        NucDecay* nd = vec[ind];
        if (nd->type == typ) return nd;
    }

    NucDecay* nd = new NucDecay(mo,da,typ,q,f);
    vec.push_back(nd);
    gsNucDecays[moda] = vec;
    return nd;
}

template<typename ItType>
int count(ItType beg, ItType end)
{
    int c = 0;
    for(; beg!=end; ++beg) ++c;
    return c;
}

void dump_radiation(const ens::rec_radiation& rad)
{
    cerr << "\t\t\t";

    if (rad.is_alpha()) {
        ens::rec_alpha const* alpha = rad.to_alpha();
        cerr << " alpha:"
             << " energy=" << rad.E_minus_E_ref() / SI::keV
             << " ref=" << rad.i_E_ref()
             << " status=" << rad.status()
             << " I_alpha=" << alpha->I_alpha()
             << " hindrance=" << alpha->hindrance_factor();
    }
    else if (rad.is_gamma()) {
        ens::rec_gamma const* gamma = rad.to_gamma();
        cerr << " gamma:"
             << " energy=" << rad.E_minus_E_ref() / SI::keV
             << " ref=" << rad.i_E_ref()
             << " status=" << rad.status()
             << " r_mix=" << gamma->r_mix()
             << " I_gamma_rel=" << gamma->I_gamma_rel()
             << " alpha_tot()=" << gamma->alpha_tot()
             << " I_trans_rel=" << gamma->I_trans_rel();
    }
    else if (rad.is_beta_mi()) {
        ens::rec_beta_mi const* beta_mi = rad.to_beta_mi();
        cerr << " beta-:"
             << " energy=" << rad.E_minus_E_ref() / SI::keV
             << " ref=" << rad.i_E_ref()
             << " status=" << rad.status()
             << " I_beta_mi_rel=" << beta_mi->I_beta_mi_rel()
             << " log_ft=" << beta_mi->log_ft();
    }
    else if (rad.is_beta_pl()) {
        ens::rec_beta_pl const* beta_pl = rad.to_beta_pl();
        cerr << " beta+:"
             << " energy=" << rad.E_minus_E_ref() / SI::keV
             << " ref=" << rad.i_E_ref()
             << " status=" << rad.status()
             << " I_tot_rel=" << beta_pl->I_tot_rel()
             << " I_beta_pl_rel=" << beta_pl->I_beta_pl_rel()
             << " I_EC_rel=" << beta_pl->I_EC_rel()
             << " log_ft=" << beta_pl->log_ft();
    }
    else {
        cerr << "unknown radiation:";
        rad.dump(cerr);
    }
    cerr << endl;

}

void dump_level(const ens::rec_level& lev)
{
    cerr << "\t\tenergy level = " << lev.E_minus_E_ref() / SI::keV
         << " ref=" << lev.i_E_ref()
         << " hl=" << lev.T_half()
         << " radiations:\n";
    for (ens::rec_level::radiation_iterator it_rad = lev.radiation_begin();
         it_rad != lev.radiation_end(); ++it_rad) {
        dump_radiation(*it_rad);
    }
}

double rad_mass(const ens::rec_radiation& rad)
{
    if (rad.is_gamma()) return 0.0;
    if (rad.is_beta_mi() || rad.is_beta_pl()) return SI::E_e;
    if (rad.is_alpha()) return 2*(SI::E_p + SI::E_n);
    cerr << "Unknown radiation:\n";
    rad.dump(cerr);
    return 0.0;
}
double rad_fraction(const ens::rec_radiation& rad,
                    const ens::rec_norm* norm=0,
                    const string& idstr = "");
double rad_fraction(const ens::rec_radiation& rad,
                    const ens::rec_norm* norm,
                    const string& idstr)
{
    double fraction = 0;

    if (rad.is_alpha()) {
        ens::rec_alpha const* alpha = rad.to_alpha();
        if (alpha->I_alpha().is_known()) {
            fraction = alpha->I_alpha().cent();
        }
        //cerr << " I_alpha = " << fraction<< endl;
    }
    if (rad.is_gamma()) {
        ens::rec_gamma const* gamma = rad.to_gamma();
        if (gamma->I_gamma_rel().is_known()) {
            fraction = gamma->I_gamma_rel().cent();
            //cerr << " I_gamma_rel = " << fraction<< endl;
            if (norm && norm->mult_gamma_rel_branch().is_known()) {
                fraction *= norm->mult_gamma_rel_branch().cent();
                //cerr << "\twith gamma_rel_branch: " << fraction << endl;
            }
        }
        else if (gamma->I_trans_rel().is_known()) {
            fraction = gamma->I_trans_rel().cent();
            //cerr << " I_trans_rel = " << fraction<< endl;
            if (norm && norm->mult_trans_rel_branch().is_known()) {
                fraction *= norm->mult_trans_rel_branch().cent();
                //cerr << "\twith mult_trans_rel_branch: " << fraction << endl;
            }
        }

    }
    if (rad.is_beta_mi()) {
        ens::rec_beta_mi const* beta_mi = rad.to_beta_mi();
        if (beta_mi->I_beta_mi_rel().is_known()) {
            fraction = beta_mi->I_beta_mi_rel().cent();
        }
        if (norm && norm->mult_beta_rel_branch().is_known()) {
            fraction *= norm->mult_beta_rel_branch().cent();
        }
    }
    if (rad.is_beta_pl()) {
        ens::rec_beta_pl const* beta_pl = rad.to_beta_pl();
        if (idstr == "EC DECAY") {
            if (beta_pl->I_EC_rel().is_known()) {
                fraction = beta_pl->I_EC_rel().cent();
            }
        }
        else {
            if (beta_pl->I_beta_pl_rel().is_known()) {
                fraction = beta_pl->I_beta_pl_rel().cent();
            }
        }
        if (norm && norm->mult_beta_rel_branch().is_known()) {
            fraction *= norm->mult_beta_rel_branch().cent();
        }
    }

    if (norm && norm->mult_branch().is_known()) {
        if (norm->mult_branch().is_known()) {
            fraction *= norm->mult_branch().cent();
        }
    }

    return fraction;
}


void GenDecay::dump_dataset(const ens::dataset& ds)
{
    cerr << ds.ident()->nucl() << " with\n"
         << "\t"<< count(ds.decay_info_begin(), ds.decay_info_end()) << " decay info\n"
         << "\t"<< count(ds.level_begin(), ds.level_end()) << " levels\n"
         << endl;    

    for (ens::dataset::decay_info_iterator it_dk = ds.decay_info_begin();
         it_dk != ds.decay_info_end(); ++it_dk) {
        
        if (it_dk->parent()) {
            cerr << "Decay parent:\n";
            it_dk->parent()->dump(cerr);
        }
        else { cerr << "No decay parent\n"; }

        if (it_dk->norm()) {
            cerr << "Decay norm:\n";
            it_dk->norm()->dump(cerr);
        }
        else { cerr << "No decay norm\n"; }

        if (it_dk->pnorm()) {
            cerr << "Decay pnorm:\n";
            it_dk->pnorm()->dump(cerr);
        }
        else { cerr << "No decay pnorm\n"; }
    }

    cerr << "Levels:\n";
    for (ens::dataset::level_iterator it_lev = ds.level_begin();
         it_lev != ds.level_end(); ++it_lev) {
        
        dump_level(*it_lev);
    }

}

ens::rec_norm const* get_norm(const ens::dataset& ds)
{
    for (ens::dataset::decay_info_iterator 
             it_di = ds.decay_info_begin();
         it_di != ds.decay_info_end(); ++it_di) {
        ens::rec_norm const* n = it_di->norm();
        if (n) return n;
    }
    return 0;
}

ens::rec_parent const* from_parent(const ens::dataset& ds, NucState& parent)
{
    //cerr << "Checking parentage in decay_info for: " << parent << endl;
    for (ens::dataset::decay_info_iterator 
             it_di = ds.decay_info_begin();
         it_di != ds.decay_info_end(); ++it_di) {
            
        ens::rec_parent const* candidate = it_di->parent();

        if (!candidate) {
            //cerr << "from_parent: no decay_info parent\n";
            continue;
        }
        if (candidate->nucl() != parent.nuc()) {
            //cerr << "from_parent: decay_info parent = " << candidate->nucl()
            //     << " != parent = " << parent.nuc() << endl;
            continue;
        }
        if (candidate->E_minus_E_ref() != parent.erel()) {
            //cerr << "from_parent: energies differ: " << candidate->E_minus_E_ref() 
            //     << " != " << parent.erel() << endl;
            continue;
        }
        if (candidate->i_E_ref() != parent.eref()) {
            //cerr << "from_parent: eref differ: " << candidate->i_E_ref()
            //     << " != " << parent.eref() << endl;
            continue;
        }
            
        return candidate;
    }
    return 0;
}

const ens::dataset& get_decay_dataset(NucState& parent, ens::nucleus& daughter,
                                      string idstr, 
                                      ens::rec_parent const*& rparent,
                                      ens::rec_norm const*& rnorm)
{
    static ens::dataset bogus;
    rparent = 0;

    //cerr << "Checking for " << parent << " -> " << daughter << " via \"" << idstr << "\"\n";

    for (ens::nucleus::dataset_iterator it_ds
             = daughter.dataset_begin ();
         it_ds != daughter.dataset_end (); ++it_ds) {
        std::string idn = it_ds->ident()->dataset_id_string();

        //cerr << "idn: \"" << idn << "\" idstr: \"" << idstr << "\"\n";
        
        if (idn.find(idstr) == std::string::npos) {
            //cerr << "get_decay_dataset: idstr \"" << idstr << "\" not in \"" << idn << "\"\n";
            continue;
        }
        ens::rec_norm const* n = get_norm(*it_ds);
        if (n) rnorm = n;
        ens::rec_parent const* p = from_parent(*it_ds,parent);
        if (! p) continue;
        rparent = p;
        return *it_ds;
    }
    //cerr << "Failed to get decay dataset for " << parent << " -> " << daughter
    //     << " for \"" << idstr  << "\"\n";
    return bogus;
}

bool dk_dataset_ok(const ens::dataset& ds)
{
    return ds.decay_info_begin() != ds.decay_info_end();
}

const ens::dataset& get_adopted_levels(ens::nucleus& nuc)
{
    static ens::dataset bogus;
    ens::dataset const* ds = adopted_levels_dataset(nuc);
    if (ds) return *ds;
    return bogus;
}

bool al_dataset_ok(const ens::dataset& ds)
{
    return ds.level_begin() != ds.level_end();
}

// Returns the best, closely matching level of the given erel/eref.
// Candidate levels must pass a hard selection of 1keV difference or
// be withing the errors given by erel.  If none is found with the
// given eref and eref is non-zero, a second search will be done with
// eref == 0.
const ens::rec_level* get_level(const ens::dataset& ds, ens::confiv_t erel, int eref)
{
    const double epsilon = 1*SI::keV;
    double min_dE = -1;
    const ens::rec_level* the_level = 0;

    ens::dataset::level_iterator it, done = ds.level_end();
    for (it = ds.level_begin(); it != done; ++it) {

        if (it->i_E_ref() != eref) continue;

        bool candidate = false;

        // level energy must be w/in errors of target
        double x = it->E_minus_E_ref().cent();
        if (erel.min() < x && x < erel.max()) candidate = true;

        // And must be w/in hard epsilon
        double dE = fabs(x-erel.cent());
        if (dE < epsilon) candidate = true;

        if (!candidate) continue;

        // Take closest, not first
        if (min_dE < 0 || dE < min_dE) {
            min_dE = dE;
            the_level = &(*it);
        }
    }

    if (the_level) return the_level;

    // See if daughter level exists with zero reference energy
    if (eref > 0) return get_level(ds,erel,0);
    return 0;
}

const ens::rec_level* get_closest_level(const ens::dataset& ds, ens::confiv_t erel, int eref)
{
    double min_dE = -1;
    const ens::rec_level* the_level = 0;
    ens::dataset::level_iterator it, done = ds.level_end();
    for (it = ds.level_begin(); it != done; ++it) {
        if (it->i_E_ref() != eref) continue;

        double x = it->E_minus_E_ref().cent();
        double dE = fabs(x-erel.cent());
        if (min_dE < 0 || dE < min_dE) {
            min_dE = dE;
            the_level = &*it;
        }
    }

    const ens::rec_level* the_other_level = 0;
    if (eref > 0) {
        the_other_level = get_closest_level(ds,erel,0);
    }

    if (the_level && the_other_level) {
        double dE1 = fabs(the_level->E_minus_E_ref().cent() - erel.cent());
        double dE2 = fabs(the_other_level->E_minus_E_ref().cent() - erel.cent());
        if (dE1 < dE2) return the_level;
        return the_other_level;
    }
    if (!(the_level || the_other_level)) return 0;
    if (the_level) return the_level;
    return the_other_level;
}

bool is_only_gamma(const ens::rec_level& lev)
{
    int non_gamma = 0;

    for (ens::rec_level::radiation_iterator it_rad = lev.radiation_begin();
         it_rad != lev.radiation_end(); ++it_rad) {
        if (! it_rad->is_gamma()) ++non_gamma;
    }
    return non_gamma == 0;
}

const ens::rec_radiation* get_non_gamma(const ens::rec_level& lev)
{
    for (ens::rec_level::radiation_iterator it_rad = lev.radiation_begin();
         it_rad != lev.radiation_end(); ++it_rad) {
        if (! it_rad->is_gamma()) return &*it_rad;
    }
    return 0;    
}

int count_gammas(const ens::rec_level& lev)
{
    int ngammas = 0;
    for (ens::rec_level::radiation_iterator it_rad = lev.radiation_begin();
         it_rad != lev.radiation_end(); ++it_rad) {
        if (it_rad->is_gamma()) {
            ++ngammas;
        }
    }
    return ngammas;
}


// get_daughters for beta+/-, EC and alpha.  beta+ and EC are considered the same
vector<NucState*> get_daughters(NucState* mother, phys::nucleus daughter, string idstr)
{
    vector<NucState*> ret;
    ens::nucleus candidate;
    if (!get_nucleus(daughter,candidate)) {
        cerr << "Failed to get nucleus for " << daughter << endl;
        return ret;
    }

    ens::rec_parent const* parent=0;
    ens::rec_norm const* norm=0;
    const ens::dataset& dk = get_decay_dataset(*mother,candidate,idstr,parent,norm);
    if (!dk_dataset_ok(dk)) {
        // can happen if there is no way to get there from here
        //cerr << "Failed to get okay data set for mother "
        //     << *mother << " and daughter " << candidate << endl;
        return ret;
    }

    const ens::dataset& al = get_adopted_levels(candidate);
    if (!al_dataset_ok(al)) {
        cerr << "Failed to get adopted levels for " << candidate << endl;
        return ret;
    }

    // cerr << "Searching for " << idstr <<" for "<<*mother<<":\n";
    //dump_dataset(dk);

    for (ens::dataset::level_iterator it_lev = dk.level_begin();
         it_lev != dk.level_end(); ++it_lev) {
        const ens::rec_level& dk_lev = *it_lev;

        //dump_level(dk_lev);
        // The decay info data set has levels that are not directly
        // reachable by alpha/beta but are entered through gamma
        // decay, skip them for now.
        const ens::rec_radiation* rad = get_non_gamma(dk_lev);
        if (!rad) {
            //cerr << "No non-gammas for level\n";
            continue;
        }

        // try to find energy
        double energy = 0;
        if (rad->E_minus_E_ref().is_known()) {
            energy = rad->E_minus_E_ref().cent();
        }
        else {
            energy = parent->QP().cent()         // available ground state energy
                + parent->E_minus_E_ref().cent() // any excitation
                - dk_lev.E_minus_E_ref().cent(); // less our energy
            if (idstr == "A DECAY") {
                int A = mother->nuc().n_part();
                double alpha_mod = (A-4.0)/A;
                energy *= alpha_mod;
                cerr << "Warning: alpha decay energy not found, calculate from Qvalue*"
                     << alpha_mod << " = " << energy << endl;
            }
        }
        if (energy <= 0.0) {
            cerr << "Got bogus energy for this decay, skipping:\n";
            cerr << "Radiation:\n";
            rad->dump(cerr);
            cerr << "Level:\n";
            dk_lev.dump(cerr);
            continue;
        }

        // try to find halflife
        ens::confiv_t halflife;
        if (dk_lev.T_half().is_known()) halflife = dk_lev.T_half();
        else {
            const ens::rec_level *al_lev = get_level(al,dk_lev.E_minus_E_ref(),
                                                     dk_lev.i_E_ref());
            if (!al_lev) {
                cerr << "Failed to get al_lev for dk_level = ";
                dump_level(dk_lev);
                cerr << "\tfrom adopted level dataset:\n";
                dump_dataset(al);
            }
            else if (al_lev->T_half().is_known()) 
                halflife = al_lev->T_half();
        }
        
        // ENSDF conflats electron capture and positron emission.
        // Datasets can be found with either "EC DECAY" or "B+ DECAY"
        // labels and in both cases the radiation line has branching
        // fractions for both decays.  This is related to #339.
        vector<string> idstrs;
        if (idstr == "EC DECAY" || idstr == "B+ DECAY") {
            idstrs.push_back("EC DECAY");
            idstrs.push_back("B+ DECAY");
        }
        else {
            idstrs.push_back(idstr);
        }

        for (size_t ind=0; ind < idstrs.size(); ++ind) {
            double fraction = rad_fraction(*rad,norm,idstrs[ind]);
        
            //dump_level(dk_lev);
            NucState* child = get_state(daughter,halflife,
                                        dk_lev.E_minus_E_ref(),
                                        dk_lev.i_E_ref());
            NucDecay* decay = get_decay(mother,child,idstrs[ind],energy,fraction);
            //cerr << "Decay by " << idstrs[ind] << ": "
            //     << *mother << " -> " << *child << " by " << *decay << endl;
            decay = 0;
        
            ret.push_back(child);
        }
    }

    //dump_dataset(dk);
    //dump_dataset(al);

    return ret;
}

vector<NucState*> get_alpha_daughters(NucState* mother)
{
    phys::nucleus daughter(mother->nuc().n_neut()-2,mother->nuc().n_prot()-2);
    return get_daughters(mother, daughter,"A DECAY");
}
vector<NucState*> get_betami_daughters(NucState* mother)
{
    phys::nucleus daughter(mother->nuc().n_neut()-1,mother->nuc().n_prot()+1);
    return get_daughters(mother, daughter,"B- DECAY");
}
vector<NucState*> get_betapl_daughters(NucState* mother)
{
    phys::nucleus daughter(mother->nuc().n_neut()+1,mother->nuc().n_prot()-1);
    return get_daughters(mother, daughter,"B+ DECAY");
}
vector<NucState*> get_ec_daughters(NucState* mother)
{
    phys::nucleus daughter(mother->nuc().n_neut()+1,mother->nuc().n_prot()-1);
    return get_daughters(mother, daughter,"EC DECAY");
}
vector<NucState*> get_it_daughters(NucState* mother)
{
    phys::nucleus daughter(mother->nuc());

    vector<NucState*> ret;
    ens::nucleus candidate;
    
    if (!get_nucleus(daughter,candidate)) return ret;

    string idstr = "IT DECAY";

    ens::rec_parent const* parent=0;
    ens::rec_norm const* norm=0;
    const ens::dataset& dk = get_decay_dataset(*mother,candidate,idstr,parent,norm);
    if (!dk_dataset_ok(dk)) return ret;

    const ens::dataset& al = get_adopted_levels(candidate);
    if (!al_dataset_ok(al)) return ret;


    const ens::rec_level *lev = get_level(dk,mother->erel(),mother->eref());
    if (!lev) {
        cerr << "Failed to get decay level for " << *mother << endl;
        return ret;
    }

    int ngamma = count_gammas(*lev);
    if (ngamma != 1) {
        cerr << "Too many gammas for an IT DECAY level, got: " << ngamma
             << " from decay of " << *mother << " -> " << candidate
             << endl;
        dump_level(*lev);
        return ret;
    }

    //cerr << "Found " << idstr <<" for "<<*mother<<":\n";

    const ens::rec_level *child_lev = get_level(al,mother->erel(),0);
    if (!child_lev) {
        cerr << "Failed to get child of IT DECAY of " << *mother << endl;
        dump_dataset(al);
        return ret;
    }

    const ens::rec_radiation& rad = *(lev->radiation_begin());

    // try to find energy
    double energy = 0;
    if (rad.E_minus_E_ref().is_known()) {
        energy = rad.E_minus_E_ref().cent();
    }

    // try to find halflife
    ens::confiv_t halflife;
    if (lev->T_half().is_known()) halflife = lev->T_half();
    else {
        const ens::rec_level *al_lev = get_level(al,lev->E_minus_E_ref(),
                                                 lev->i_E_ref());
        if (!al_lev) {
            cerr << "Failed to get al_lev for " 
                 << lev->E_minus_E_ref() / SI::keV
                 << " w.r.t. " << lev->i_E_ref() << endl;
        }
        else if (al_lev->T_half().is_known()) 
            halflife = al_lev->T_half();
    }
        
    double fraction = norm->mult_branch().cent();

    NucState* child = get_state(daughter,halflife,
                                child_lev->E_minus_E_ref(),
                                child_lev->i_E_ref());
    NucDecay* decay = get_decay(mother,child,idstr,energy,fraction);
    //cerr << "Decay: " << *mother << " -> " << *child << " by " << *decay << endl;
    decay = 0;                  // quell compiler warning

    ret.push_back(child);
    return ret;
}

vector<NucState*> get_gamma_daughters(NucState* mother)
{
    phys::nucleus daughter(mother->nuc());

    vector<NucState*> ret;
    ens::nucleus candidate;
    if (!get_nucleus(daughter,candidate)) return ret;

    const ens::dataset& al = get_adopted_levels(candidate);
    if (!al_dataset_ok(al)) return ret;

    const ens::rec_level* lev = get_level(al,mother->erel(),mother->eref());
    if (!lev) {
        cerr << "Failed to get level for " << *mother << endl;
        return ret;
    }

    if (0 == count_gammas(*lev) and mother->erel().cent() > 0) {
        // Sometimes there are no gammas given in the dataset, what
        // can I say?

        //cerr << "No gammas in level " << *mother << endl;
        //dump_level(*lev);
        return ret;
    }

    //cerr << "GAMMA decay from " << *mother << endl;

    for (ens::rec_level::radiation_iterator it_rad = lev->radiation_begin();
         it_rad != lev->radiation_end(); ++it_rad) {

        if (! it_rad->is_gamma()) continue;

        ens::confiv_t halflife;

        double energy = it_rad->E_minus_E_ref().cent();
        double fraction = rad_fraction(*it_rad);

        // fixme, this is bogus if i_E_ref is non-zero
        ens::confiv_t erel = mother->erel() - it_rad->E_minus_E_ref();
        if (erel.cent() < 0) erel = ens::confiv_t(0.0,0);

        // First look in same X-excited state
        const ens::rec_level* daughter_lev = get_level(al,erel,mother->eref());
        if (!daughter_lev) {
            daughter_lev = get_closest_level(al,erel,mother->eref());
            double de = daughter_lev->E_minus_E_ref().cent();
            double diff = fabs(de-erel.cent())/erel.cent();

            if (daughter_lev) {
                cerr << "Warning inexact match for daughter of "<<*mother<<" for gamma of "
                     << it_rad->E_minus_E_ref()/SI::keV
                     << "\twanted:"<<erel/SI::keV<<" got:"
                     << de/SI::keV<<" ["<<daughter_lev->i_E_ref()<<"] diff="<<diff*100<<"%\n";
            }
        }
        if (!daughter_lev) {
            cerr << "Failed to get daughter energy ("<<erel/SI::keV<<") level for gamma from "<<*mother<<":\n";
            dump_radiation(*it_rad);
            dump_dataset(al);
            continue;
        }
        halflife = daughter_lev->T_half();

        NucState* child = get_state(mother->nuc(),halflife,
                                    daughter_lev->E_minus_E_ref(),
                                    daughter_lev->i_E_ref());
        NucDecay* decay = get_decay(mother,child,"Gamma",energy,fraction);
        //cerr << "Decay: " << *mother << " -> " << *child << " by " << *decay << endl;
        decay = 0;  // quell compiler warning

        ret.push_back(child);

    }
    return ret;
}

typedef vector<NucState*> (*GetDaughterFunc)(NucState*);
GetDaughterFunc get_xxx_daughters[] = {
    get_alpha_daughters,
    get_betami_daughters,
    get_betapl_daughters,
    get_ec_daughters,
    get_it_daughters,
    get_gamma_daughters,
    0
};
const char* decay_names[] = {
    "A DECAY",
    "B- DECAY",
    "B+ DECAY",
    "EC DECAY",
    "IT DECAY",
    "GAMMA DECAY",
    0
};

static std::map<NucDecay*,int> chainMap;

std::map<NucDecay*,int> GenDecay::getChainMap()
{
    return chainMap;
}
void GenDecay::chain(NucState* mother, int depth,
                     more::phys::nucleus stop_nuc)
{
    // record if a NucState has been chained yet
    typedef map<NucState*,int> NucChained_t;
    static NucChained_t chained;


    if (!mother) {
        cerr << "ERROR: chain given null mother\n";
        return;
    }

    if (!depth) {
        //cerr << "Reached target depth with " << *mother << endl;
        return;
    }
    --depth;

    // cerr << "Mother nuc: " << mother->nuc()
    //      << " stop_nuc: " << stop_nuc << endl;
    if (mother->energy() == 0.0 && mother->eref() == 0 &&
        mother->nuc() == stop_nuc) 
    {
        cerr << "Reached ground state stop_nuc = " << stop_nuc << endl;
        return;
    }

    // Don't chain a state that has already been seen
    if (chained[mother]) return;
    chained[mother] = 1;

    //cerr << "Chaining " << *mother << "(@" << (void*)mother << ") at depth "<<depth<<"\n";

    // load mother
    ens::nucleus pnucl;
    if (!get_nucleus(mother->nuc(),pnucl)) {
        cerr << "Got run time error with " << mother->nuc() << endl;
    }

    vector<NucState*> daughters;

    // look for daughters
    for (int ind=0; get_xxx_daughters[ind]; ++ind) {
        vector<NucState*> ds = get_xxx_daughters[ind](mother);

        //cerr << *mother << " has " << ds.size() 
        //     << " daughters made by " << decay_names[ind] << endl;

        // daughters.insert(daughters.end(),ds.begin(),ds.end());
        
        for (std::vector<NucState*>::iterator it = ds.begin();
             it != ds.end(); ++it) {
            // In order to stop decay:
            // * mother is stop nuclear
            // * daughter is not stop nuclear
            if (mother->nuc()==stop_nuc && (*it)->nuc()!=stop_nuc) {
                // skip it
                LogInfo << "SKIP "
                        << " mother: " << mother->nuc()
                        << " dauther: " << (*it)->nuc()
                        << " stop: " << stop_nuc
                        << std::endl;
            } else {
                daughters.push_back(*it);
            }

        }
    }

    // After normalizing, some decays will be throwed away.
    normalize_branching_fractions(*mother);
    static int num = 1;
    for (int ind = 0; ind < mother->ndecays(); ++ind)
    {
	NucState* daug = mother->decay(ind)->daughter;
        if (mother->nuc()==stop_nuc && daug->nuc()!=stop_nuc) {
            // skip it
            LogInfo << "SKIP "
                    << " mother: " << mother->nuc()
                    << " dauther: " << daug->nuc()
                    << " stop: " << stop_nuc
                    << std::endl;
            continue;
        }
	LogDebug << "decay ID:" << num << std::endl;
	LogDebug << "Decay:" << std::endl;
        LogDebug << *mother << "\t" << "\n";
        LogDebug << *daug << "\n" << "Decay fraction:" << mother->decay(ind) -> fraction << "\t" << *(mother->decay(ind)) << "\n";
	LogDebug << "There are " << mother -> n_origin_decays() << " nucs decay into this mother." <<std::endl;
	num++;
	chainMap[mother->decay(ind)] = 0;
	mother->decay(ind)->daughter->origin_decays().push_back(mother->decay(ind));
    }

    for (int k = 0; k < mother->ndecays(); ++k)
    {
        if (mother->nuc()==stop_nuc && mother->decay(k)->daughter->nuc()!=stop_nuc) {
            // skip it
            LogInfo << "SKIP "
                    << " mother: " << mother->nuc()
                    << " dauther: " << mother->decay(k)->daughter->nuc()
                    << " stop: " << stop_nuc
                    << std::endl;
            continue;
        }
        chain(mother->decay(k)->daughter,depth,stop_nuc);
    }

}

NucState* GenDecay::get_ground(std::string name)
{
    istringstream iss(name);
    phys::nucleus nucl;
    iss >> nucl;
    return get_ground(nucl);
}

NucState* GenDecay::get_ground(phys::nucleus nucl)
{
    ens::nucleus candidate;
    if (!get_nucleus(nucl,candidate)) {
        cerr << "get_ground: failed to get_nucleus for " << nucl << endl;
        return 0;
    }
    const ens::dataset& al = get_adopted_levels(candidate);
    if (!al_dataset_ok(al)) {
        cerr << "get_ground: adopted levels data set is not okay:\n";
        al.dump(cerr);
        return 0;
    }

    ens::confiv_t erel(0.0,0.0);
    const ens::rec_level* lev = get_level(al,erel,0);
    if (!lev) {
        cerr << "get_ground: failed to get ground energy level record for " << nucl << endl;
        return 0;
    }

    ens::confiv_t hl = lev->T_half();
    NucState* head = get_state(nucl,hl);
    return head;
}

const Radiation* GenDecay::decay_radiation(const NucDecay& dk)
{
    static map<const NucDecay*, Radiation*> radCache;

    Radiation* rad = radCache[&dk];
    if (rad) return rad;

    int parentZ = dk.mother->nuc().n_prot();
    int parentA = dk.mother->nuc().n_part();
    if (dk.type == "A DECAY") {
        rad = new AlphaRadiation(dk.clhep_energy(), parentA);
    }
    if (dk.type == "Gamma") {
        rad = new GammaRadiation(dk.clhep_energy());
    }
    if (dk.type == "IT DECAY") {
        rad = new GammaRadiation(dk.clhep_energy());
    }
    if (dk.type == "B- DECAY") {
        rad = new BetaRadiation(dk.clhep_energy(), parentZ);
    }
    if (dk.type == "B+ DECAY") {
        rad = new BetaRadiation(dk.clhep_energy(), -1*parentZ);
    }
    if (dk.type == "EC DECAY") {
        rad = new ElectronCapture(dk.clhep_energy());
    }

    if (!rad) {
        cerr << "decay_radiation: failed to make radiation from \"" << dk << "\"\n";
        return 0;
    }

    radCache[&dk] = rad;    
    return rad;
}
