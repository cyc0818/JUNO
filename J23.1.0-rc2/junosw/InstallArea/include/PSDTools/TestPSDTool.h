#ifndef TESTPSDTOOL_H
#define TESTPSDTOOL_H
//#pragma once
/*
 * Author: Xin Huang, Xiaojie Luo
 * TestPSDTool is an example for PSDTools, developers can develop their own tool base on this file
 */

#include "IPSDTool.h"
#include "IPSDInputSvc.h"
#include "SniperKernel/ToolBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "SniperKernel/SniperPtr.h"
#include <vector>

#include "TTree.h"

class TestPSDTool: public ToolBase, public IPSDTool{
    public:
        explicit TestPSDTool(const std::string &name);
        ~TestPSDTool() override;

        bool initialize() override;
        bool finalize() override;
        virtual bool preProcess(JM::EvtNavigator *);
        double CalPSDVal(bool) override;

    private:
        TTree *m_userTree;
        double d_psdVar;
        TTree *m_psdTree;

        std::vector<double> Time;
        std::vector<double> Charge;
        std::vector<int>    isHam;
        std::vector<int>    PMTID;

        IPSDInputSvc* m_psdInput;
};
#endif
