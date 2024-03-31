#ifndef BDTE_EDM_READER_H
#define BDTE_EDM_READER_H

#include <EvtNavigator/NavBuffer.h>
#include <Geometry/RecGeomSvc.h>
#include <RootWriter/RootWriter.h>
#include <SniperKernel/AlgBase.h>
#include <SniperPython/PyDataStore.h>

#include <boost/python/numpy.hpp>

namespace BDTE {

const double EPSILON = 50; //< Epsilon value used internaly when dividing by value that could be 0

/**
 * @brief Algorithm that extract aggregated features from the calib EDM
 * and make them available to python
 */
class EdmReader : public AlgBase {
 public:
  /**
   * @brief Create a XDGEdmReader
   *
   * @param name The name or identifier of the XDGEdmReader
   */
  EdmReader(const std::string& name);

  /**
   * @brief Initialize the reader. Should not be manually called
   *
   * @return True if everything went correctly. False if there was an error
   */
  bool initialize();

  /**
   * @brief Execute the reader. Should not be manually called
   *
   * @return True if everything went correctly. False if there was an error
   */
  bool execute();

  /**
   * @brief Finalize the reader. Should not be manually called
   *
   * @return True if everything went correctly. False if there was an error
   */
  bool finalize();

 private:
  SniperPtr<RecGeomSvc>* _rec_geo_svc = nullptr;
  SniperDataPtr<JM::NavBuffer>* _nav_buffer = nullptr;
  SniperDataPtr<PyDataStore>* _pystore = nullptr;
};

}  // namespace BDTE

#endif
