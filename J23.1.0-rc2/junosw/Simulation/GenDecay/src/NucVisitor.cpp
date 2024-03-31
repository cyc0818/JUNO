#include "NucVisitor.h"
#include "NucState.h"
#include "NucDecay.h"
#include "SniperKernel/SniperLog.h"

#include <vector>
using namespace std;
using namespace GenDecay;

NucVisitor::NucVisitor(double min_br)
    : m_minBranchFraction(min_br)
{
}

NucVisitor::~NucVisitor() {}

void NucVisitor::descend(NucState* state) 
{
    this->preDescend();
    this->_descend(state);
    this->addResidual();
    this->postDescend();
}

/*
void NucVisitor::_descend(NucState* state) 
{
    if (m_countMap[state]) return;
    m_countMap[state] += 1;
    vector<NucDecay*> &decays = state->decays();
    for (size_t ind=0; ind < decays.size(); ++ind) {

        if (decays[ind]->fraction < m_minBranchFraction) continue;

        NucDecay* decay = decays[ind];
        NucState* daughter = decay->daughter;
        if (this->visit(state,daughter,decay)) {
	    
            _descend(daughter);
        }
    }
}
*/
void NucVisitor::addResidual()
{
   for (std::map<NucDecay*,int>::iterator it = m_countMap.begin();
					  it != m_countMap.end(); ++ it)
   {
	if (it -> second == 0)
	    _descend(it->first->mother);
	   
	it -> second += 1;
	
   }
}

void NucVisitor::_descend(NucState* state)
{
    vector<NucDecay*> &decays = state->decays();
    for (size_t ind=0; ind < decays.size(); ++ind)
    {
        if (decays[ind]->fraction < m_minBranchFraction) continue;
        NucDecay* decay = decays[ind];
        NucState* daughter = decay->daughter;
	if (!m_countMap[decay]) 
	{
	    m_countMap[decay] +=1;
	    LogDebug << "--------------------------------------------"<<"\n";
	    LogDebug << "Visit Decay down:" << "\n";

	    if (this->visit(decay->mother,decay->daughter,decay))	
	    {
	    
	        _origin(daughter);
	        _descend(daughter);
	    }
	}
	else
	{
	    continue;
//	    _descend(daughter);
	}
    }
    return;
}

void NucVisitor::_origin(NucState* state)
{
//    LogDebug << "------------------------------------------------" << "\n"
//	      << "NucState:" << state->nuc().name() << " level:"<<state->energy()
//	      << "origin size:" << state->origin_decays().size() << "\n"
//            << "------------------------------------------------" << "\n"
//              << std::endl;
    vector<NucDecay*> &origin_decays = state -> origin_decays();
    for (size_t ind=0; ind < origin_decays.size(); ++ind)
    {
	NucDecay* origin_decay = origin_decays[ind];
   	NucState* mother = origin_decay->mother;
	if (m_countMap[origin_decay]) continue;
	m_countMap[origin_decay] +=1;
	
	_origin(mother);
//	this->visit(mother,state,origin_decay);
//	if (mother->nuc().name()=="Pa-234" || state->nuc().name()=="Pa-234")
//	{
	LogDebug << "--------------------------------------------" << "\n";
	LogDebug << "Visit Decay up:" << "\n";
//	}
	this->visit(origin_decay->mother,origin_decay->daughter,origin_decay);
    }
 //   _descend(state);

    return;
}



