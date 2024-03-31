#ifndef MCTruthPointLikeClassifier_h
#define MCTruthPointLikeClassifier_h

#include "ClassificationTool/IClassificationTool.h"
#include "SniperKernel/ToolBase.h"

class MCTruthPointLikeClassifier: public ToolBase,
                                  public IClassificationTool
{
public:
    MCTruthPointLikeClassifier(const std::string& name);
    ~MCTruthPointLikeClassifier();

    bool configure();
    ClassificationType classify(JM::NavBuffer* buffer);

private:
};

#endif
