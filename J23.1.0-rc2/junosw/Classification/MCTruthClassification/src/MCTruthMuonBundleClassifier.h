#ifndef MCTruthMuonBundleClassifier_h
#define MCTruthMuonBundleClassifier_h

#include "ClassificationTool/IClassificationTool.h"
#include "SniperKernel/ToolBase.h"

class MCTruthMuonBundleClassifier: public ToolBase,
                                  public IClassificationTool
{
public:
    MCTruthMuonBundleClassifier(const std::string& name);
    ~MCTruthMuonBundleClassifier();

    bool configure();
    ClassificationType classify(JM::NavBuffer* buffer);

private:
};

#endif
