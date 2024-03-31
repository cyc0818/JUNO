#ifndef IWFRECTOOL_H
#define IWFRECTOOL_H

#include <map>
#include <vector>
#include "RecTools/QTInfo.h"
#include "Event/CalibPmtChannel.h"
#include "PMTCalibSvc/PMTCalibSvc.h"
#include "Geometry/PMTParamSvc.h"

class IWFRecToolManager;

class IWFRecTool{
    public:
        IWFRecTool();
        virtual ~IWFRecTool();

        virtual bool initialize() = 0;
        virtual void rec(QTInfo &) = 0;
        virtual JM::CalibPmtChannel * recChannel(int, std::vector<uint16_t>&) = 0;
};

#endif