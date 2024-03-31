
//   **************************************************************************
//   *                                                                        *
//   *                      ! ! ! A T T E N T I O N ! ! !                     *
//   *                                                                        *
//   *  This file was created automatically by XmlObjDesc, please do not      *
//   *  delete it or edit it by hand.                                         *
//   *                                                                        *
//   *  If you want to change this file, first change the corresponding       *
//   *  xml-file and rerun the tools from XmlObjDesc (or run make if you      *
//   *  are using it from inside a Sniper-package).                           *
//   *                                                                        *
//   **************************************************************************

#ifndef SNIPER_RECEVENT_CDVERTEXRECEVT_H
#define SNIPER_RECEVENT_CDVERTEXRECEVT_H

// Include files
#include "Event/EventObject.h"
#include "Event/RecVertex.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdVertexRecEvt CdVertexRecEvt.h
   *
   * Reconstruced vertex 
   *
   * @author ZHANG Kun
   * created Sun Sep 24 17:52:48 2023
   * 
   */

  class CdVertexRecEvt: public EventObject
  {
  private:

    std::vector<JM::RecVertex*> m_vertices; // Get the Cd reconstructed vertices
  

  protected:


  public:

    /// Default Constructor
    CdVertexRecEvt() : m_vertices() {}
  
  /// destructor
  ~CdVertexRecEvt();
  
    /// Get a Cd vertex via index
    const JM::RecVertex * getVertex(int index) const;
  
    /// Add a Cd vertex to this event
    void addVertex(JM::RecVertex* vertex);
  
    /// Get number of vertices
    int nVertices() const;
  
    /// Retrieve const  
    /// Get the Cd reconstructed vertices
    const std::vector<JM::RecVertex*>& vertices() const;
  
    ClassDef(CdVertexRecEvt,1)
  

  }; // class CdVertexRecEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdVertexRecEvt::~CdVertexRecEvt() 
{

  std::vector<RecVertex*>::const_iterator it = m_vertices.begin();
  std::vector<RecVertex*>::const_iterator end = m_vertices.end();
  while(it!=end){
    if(*it) delete  (*it);
    it++;
  }
  m_vertices.clear();
                
}

inline const std::vector<JM::RecVertex*>& JM::CdVertexRecEvt::vertices() const 
{
  return m_vertices;
}

inline const JM::RecVertex * JM::CdVertexRecEvt::getVertex(int index) const 
{

  return m_vertices.at(index);
                
}

inline void JM::CdVertexRecEvt::addVertex(JM::RecVertex* vertex) 
{

  m_vertices.push_back(vertex);
                
}

inline int JM::CdVertexRecEvt::nVertices() const 
{

  return m_vertices.size();;
                
}


#endif ///SNIPER_RECEVENT_CDVERTEXRECEVT_H
