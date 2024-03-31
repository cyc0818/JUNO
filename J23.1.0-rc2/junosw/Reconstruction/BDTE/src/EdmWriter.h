#ifndef BDTE_EDM_WRITER_H
#define BDTE_EDM_WRITER_H

#include <EvtNavigator/NavBuffer.h>
#include <Geometry/RecGeomSvc.h>
#include <RootWriter/RootWriter.h>
#include <SniperKernel/AlgBase.h>
#include <SniperPython/PyDataStore.h>

#include <boost/python/numpy.hpp>

namespace BDTE {
/**
 * @brief Algorithm that read reconstucted vertices from the python datastore
 * and write them in the EDM
 */
class EdmWriter : public AlgBase {
 public:
  /**
   * @brief Create a EdmWriter
   *
   * @param name The name or identifier of the EdmReader
   */
  EdmWriter(const std::string& name);

  /**
   * @brief Initialize the writer. Should not be manually called
   *
   * @return True if everything went correctly. False if there was an error
   */
  bool initialize();

  /**
   * @brief Execute the writer. Should not be manually called
   *
   * @return True if everything went correctly. False if there was an error
   */
  bool execute();

  /**
   * @brief Finalize the writer. Should not be manually called
   *
   * @return True if everything went correctly. False if there was an error
   */
  bool finalize();

 private:
  SniperDataPtr<JM::NavBuffer>* _nav_buffer = nullptr;
  SniperDataPtr<PyDataStore>* _pystore = nullptr;
};
}  // namespace BDTE

#endif
