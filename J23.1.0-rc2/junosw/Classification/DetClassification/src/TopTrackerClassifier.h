/*
 * Description:
 *   classify the event in top tracker
 *
 * Author:
 *   Tao Lin <lintao AT ihep.ac.cn>
 */

#ifndef TopTrackerClassifier_h
#define TopTrackerClassifier_h

#include "ClassificationTool/IClassificationTool.h"
#include "SniperKernel/ToolBase.h"

class TopTrackerClassifier: public ToolBase,
                            public IClassificationTool 
{
public:
    TopTrackerClassifier(const std::string& name);
    ~TopTrackerClassifier();

    bool configure();
    ClassificationType classify(JM::NavBuffer* buffer);

private:
};

#endif
