/*
 * Description:
 *   classify the event in central detector
 *
 * Author:
 *   Tao Lin <lintao AT ihep.ac.cn>
 */

#ifndef CentralDetectorClassifier_h
#define CentralDetectorClassifier_h

#include "ClassificationTool/IClassificationTool.h"
#include "SniperKernel/ToolBase.h"

class CentralDetectorClassifier: public ToolBase,
                                 public IClassificationTool 
{
public:
    CentralDetectorClassifier(const std::string& name);
    ~CentralDetectorClassifier();

    bool configure();
    ClassificationType classify(JM::NavBuffer* buffer);

private:
};

#endif
