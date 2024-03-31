#ifndef ClassificationAlg_h
#define ClassificationAlg_h

/*
 * Description:
 *   This is a simple classification algorithm to invoke the 
 *   classifiers in sequence. 
 *
 * Author:
 *   Tao Lin <lintao@ihep.ac.cn>
 *   May 30, 2022
 *
 */

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "ClassificationTool/IClassificationTool.h"

class ClassificationAlg: public AlgBase {
public:
    ClassificationAlg(const std::string& name);
    ~ClassificationAlg();

    bool initialize();
    bool execute();
    bool finalize();

private:
    std::vector<std::string> m_classifierNames;
    std::vector<IClassificationTool*> m_classifiers;

private:
    // hold the buffer
    JM::NavBuffer *m_buffer;

};

#endif

