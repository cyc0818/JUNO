/*
 * Description:
 *   classify the event in water pool
 *
 * Author:
 *   Tao Lin <lintao AT ihep.ac.cn>
 */

#ifndef WaterPoolClassifier_h
#define WaterPoolClassifier_h

#include "ClassificationTool/IClassificationTool.h"
#include "SniperKernel/ToolBase.h"

class WaterPoolClassifier: public ToolBase,
                           public IClassificationTool 
{
public:
    WaterPoolClassifier(const std::string& name);
    ~WaterPoolClassifier();

    bool configure();
    ClassificationType classify(JM::NavBuffer* buffer);

private:
};

#endif
