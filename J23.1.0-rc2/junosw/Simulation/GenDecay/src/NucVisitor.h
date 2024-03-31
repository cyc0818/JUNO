/**
 * \class NucVisitor
 *
 * \brief Visit a nuclear decay chain.
 *
 * Starting from a given NucState, descend the chain, visiting each
 * daughter.  Subclasses should implement visit() to gain access to
 * each daughter.  Return false to not descend into visiting the given
 * daughter.
 *
 * Setting a non default minium branch fraction lets one avoid small
 * branches.  Default is all branches are visitted.
 *
 * bv@bnl.gov Wed May 13 17:18:58 2009
 *
 */
#ifndef NUCVISITOR_H
#define NUCVISITOR_H

#include <map>

namespace GenDecay {

class NucState;
class NucDecay;

class NucVisitor 
{
    double m_minBranchFraction;
    std::map<NucDecay*,int> m_countMap;

public:
    NucVisitor(double min_br = 0.0);
    virtual ~NucVisitor();

    /// Start the visitation with the given state
    virtual void descend(NucState* state);

    /// Subclass must override.
    virtual bool visit(NucState* /*mother*/, NucState* /*daughter*/, NucDecay* /*decay*/) = 0;

    /// Hooks called before and after actual descent through the
    /// hiearchy.  Subclass may optionally implement.
    virtual void preDescend() {}
    virtual void postDescend() {}
    void setMap(std::map<NucDecay*,int> & map) { m_countMap = map;}
private:

    // Actual recusive call
    void _descend(NucState* state);
    void _origin(NucState* state);
    void addResidual();
};

}
#endif  // NUCVISITOR_H
