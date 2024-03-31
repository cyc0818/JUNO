#ifndef MCTruthTrackLikeClassifier_h
#define MCTruthTrackLikeClassifier_h

#include "ClassificationTool/IClassificationTool.h"
#include "SniperKernel/ToolBase.h"

class MCTruthTrackLikeClassifier: public ToolBase,
                                  public IClassificationTool
{
public:
    MCTruthTrackLikeClassifier(const std::string& name);
    ~MCTruthTrackLikeClassifier();

    bool configure();
    ClassificationType classify(JM::NavBuffer* buffer);

private:
};

#endif
