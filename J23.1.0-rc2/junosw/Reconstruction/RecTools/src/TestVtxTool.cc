#include "TestVtxTool.h"

#include "SniperKernel/ToolFactory.h"

DECLARE_TOOL(TestVtxTool);

TestVtxTool::TestVtxTool(const std::string &name): ToolBase(name){
}

TestVtxTool::~TestVtxTool(){
}

bool TestVtxTool::configure(){
    return true;
}

void TestVtxTool::rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo){
    recinfo.energy = 2.2;
}
