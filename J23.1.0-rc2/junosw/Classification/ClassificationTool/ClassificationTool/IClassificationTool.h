#ifndef IClassificationTool_h
#define IClassificationTool_h

/*
 * Description:
 *   Used for event classification in JUNO experiment.
 *
 *   An instance of ClassificationAlg is responsible to classify an event 
 *   with the pre-configured instances of ClassificationTools.
 *
 *   For physics performance study, users could create two instances of 
 *   ClassificationAlgs and produce the corresponding results. 
 *
 *   The ClassificationTool could be also reused by some other cases, 
 *   such as the reconstruction steering, it is possible to create
 *   a high level steering algorithm to invoke the instances of ClassificationTool
 *   and then invoke the corresponding reconstruction algorithms. 
 *
 *   The workflow of ClassificationAlg and ClassificationTool:
 *
 *                                           +--------------------+
 *     +-------------------+               +--------------------+ |
 *     | ClassificationAlg |               | ClassificationTool | +
 *     +-------------------+               +--------------------+
 *       -----> |          classify(nav_buffer)       |
 *      |       | ----------------------------------> |
 *      |       |                                     |
 *      |       | <---------------------------------- |
 *      `<----- |      an type will be returned
 *              |
 *
 *  The reason to use NavBuffer is to make it possible to classify the 
 *  time correlated events. 
 *
 * Author:
 *   Tao Lin <lintao@ihep.ac.cn> 
 *   May 30, 2022
 * 
 */

#include "ClassificationTool/ClassificationType.h"
#include "EvtNavigator/NavBuffer.h"

struct IClassificationTool {

    // configure the classification tool in initialization of ClassificationAlg
    virtual bool configure() = 0;

    // classify the current event
    virtual ClassificationType classify(JM::NavBuffer* buffer) = 0;

    // destructor
    virtual ~IClassificationTool();

};

#endif
