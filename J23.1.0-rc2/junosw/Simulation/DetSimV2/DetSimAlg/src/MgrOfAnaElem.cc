#include "DetSimAlg/MgrOfAnaElem.h"
#include "DetSimAlg/IAnalysisElement.h"

#include <boost/foreach.hpp>

MgrOfAnaElem&
MgrOfAnaElem::instance() {
    static MgrOfAnaElem s_mgr;
    return s_mgr;
}

bool
MgrOfAnaElem::reg(const std::string& key, IAnalysisElement* mgr) {
    m_anamgrs[key] = mgr;
    m_order.push_back(mgr);
    return true;
}

bool
MgrOfAnaElem::get(const std::string& key, IAnalysisElement*& mgr) {
    K2AM::iterator it = m_anamgrs.find(key);

    if (it != m_anamgrs.end()) {
        mgr = m_anamgrs[key];
        return true;
    }
    return false;
}

void MgrOfAnaElem::BeginOfRunAction(const G4Run* run) {
    BOOST_FOREACH(EXEORD::value_type const& val, m_order)
    {
          val->BeginOfRunAction(run);
    }
}
void MgrOfAnaElem::EndOfRunAction(const G4Run* run){
    BOOST_FOREACH(EXEORD::value_type const& val, m_order)
    {
          val->EndOfRunAction(run);
    }

}
void MgrOfAnaElem::BeginOfEventAction(const G4Event* evt){
    BOOST_FOREACH(EXEORD::value_type const& val, m_order)
    {
          val->BeginOfEventAction(evt);
    }

}
void MgrOfAnaElem::EndOfEventAction(const G4Event* evt){
    BOOST_FOREACH(EXEORD::value_type const& val, m_order)
    {
          val->EndOfEventAction(evt);
    }

}
void MgrOfAnaElem::PreUserTrackingAction(const G4Track* trk){
    BOOST_FOREACH(EXEORD::value_type const& val, m_order)
    {
          val->PreUserTrackingAction(trk);
    }

}
void MgrOfAnaElem::PostUserTrackingAction(const G4Track* trk){
    BOOST_FOREACH(EXEORD::value_type const& val, m_order)
    {
          val->PostUserTrackingAction(trk);
    }

}
void MgrOfAnaElem::UserSteppingAction(const G4Step* step){
    BOOST_FOREACH(EXEORD::value_type const& val, m_order)
    {
          val->UserSteppingAction(step);
    }

}
