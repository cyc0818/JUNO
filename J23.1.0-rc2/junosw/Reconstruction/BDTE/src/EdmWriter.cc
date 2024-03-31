#include "EdmWriter.h"

#include <Event/CdLpmtCalibHeader.h>
#include <Event/CdSpmtCalibHeader.h>
#include <Event/CdVertexRecEvt.h>
#include <Event/CdVertexRecHeader.h>
#include <Event/HeaderObject.h>
#include <Event/RecVertex.h>
#include <Event/SimEvt.h>
#include <Event/SimHeader.h>
#include <Event/SimTrack.h>
#include <EvtNavigator/EvtNavHelper.h>
#include <Identifier/CdID.h>
#include <Identifier/Identifier.h>
#include <SniperKernel/AlgFactory.h>
#include <TH1F.h>

#include "Utils.h"

namespace BDTE {
DECLARE_ALGORITHM(EdmWriter);

namespace p = boost::python;
namespace np = boost::python::numpy;

EdmWriter::EdmWriter(const std::string& name) : AlgBase(name) { LogDebug << "Building" << std::endl; }

bool EdmWriter::initialize() {
  LogDebug << "Initializing" << std::endl;

  _nav_buffer = new SniperDataPtr<JM::NavBuffer>(getParent(), "/Event");
  _pystore = new SniperDataPtr<PyDataStore>(getParent(), "DataStore");

  if (_nav_buffer == nullptr || _nav_buffer->invalid()) {
    LogError << "Failed to recover a NavBuffer instance ! - " << _nav_buffer << std::endl;
    return false;
  }

  if (_pystore == nullptr || _pystore->invalid()) {
    LogError << "Failed to recover a PyDataStore instance ! - " << _pystore << std::endl;
    return false;
  }

  return true;
}

bool EdmWriter::execute() {
  LogDebug << "Executing" << std::endl;

  JM::EvtNavigator* evt_nav = (*_nav_buffer)->curEvt();
  PyDataStore* store = _pystore->data();

  LogDebug << "Processing event " << evt_nav->EventID() << std::endl;

  // Fetch data from calibration
  auto calibLpmtHeader = JM::getHeaderObject<JM::CdLpmtCalibHeader>(evt_nav);
  auto calibSpmtHeader = JM::getHeaderObject<JM::CdSpmtCalibHeader>(evt_nav);
  auto simHeader = JM::getHeaderObject<JM::SimHeader>(evt_nav);

  if (calibLpmtHeader == nullptr || calibSpmtHeader == nullptr || simHeader == nullptr) {
    LogError << "Cannot retrieve the headers : (LPMT) " << calibLpmtHeader << " | (SPMT) " << calibSpmtHeader
             << " | (Sim) " << simHeader << std::endl;
    return false;
  }

  float e_rec = 0;

  store->get("e_rec", e_rec);

  LogDebug << "Event retrieved" << std::endl;

  LogDebug << "Storing" << std::endl;

  JM::CdVertexRecHeader* rec_header =
      nullptr;                              //< Uninitialized pointer. Be carefull, do not use before `##EVT_HEADER##`
  JM::CdVertexRecEvt* rec_event = nullptr;  //< Uninitialized pointer. Be carefull, do not use before `##EVT_HEADER##`
  JM::RecVertex* rec_vertex = new JM::RecVertex();

  // Get result storage location
  rec_header = JM::getHeaderObject<JM::CdVertexRecHeader>(evt_nav);
  LogDebug << "rec_header : " << rec_header << std::endl;
  if (rec_header == nullptr) {
    // If no rec header, create one
    rec_header = new JM::CdVertexRecHeader();
    // JM::addHeaderObject(evt_nav, rec_header);
    evt_nav->addHeader(rec_header);
  }

  if (!rec_header->hasEvent()) {
    // If no event, create one
    rec_event = new JM::CdVertexRecEvt();
    rec_header->setEvent(rec_event);
  } else {
    // Else get event
    rec_event = rec_header->event();
  }

  // The JM::CdVertexRecEvt::getVertex is flag as const.
  // As I interpret it, it's mean that the correct usage is to
  // add a new reconstructed vertex and that you should not modify
  // the same one each time
  //
  // Therefore, always create and add a new vertex to the event
  rec_event->addVertex(rec_vertex);

  // ##EVT_HEADER##

  rec_vertex->setEnergy(e_rec);

  LogDebug << "End of event " << evt_nav->EventID() << std::endl;
  return true;
}

bool EdmWriter::finalize() {
  LogDebug << "Finalizing" << std::endl;

  delete _nav_buffer;
  delete _pystore;

  return true;
}
}  // namespace BDTE
