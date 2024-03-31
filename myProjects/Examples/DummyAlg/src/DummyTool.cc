#include "DummyTool.h"
#include "SniperKernel/ToolFactory.h"

DECLARE_TOOL(DummyTool);

DummyTool::DummyTool(const std::string& name)
    : ToolBase(name)
{
}

DummyTool::~DummyTool()
{
}

void DummyTool::doSomeThing()
{
    LogInfo << "DummyTool is running :)" << std::endl;
}
