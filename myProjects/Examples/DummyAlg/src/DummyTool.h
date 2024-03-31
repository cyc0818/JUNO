#ifndef DUMMY_TOOL_H
#define DUMMY_TOOL_H

#include "SniperKernel/ToolBase.h"

class DummyTool : public ToolBase
{
    public :

	DummyTool(const std::string& name);
	virtual ~DummyTool();

	void doSomeThing();
};

#endif
