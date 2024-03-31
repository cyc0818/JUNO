#ifndef IWaveFitTool_h
#define IWaveFitTool_h

class IWaveFitTool {
public:
    virtual ~IWaveFitTool() {}

    virtual bool fit(const float* waveform, int spantime) = 0;

    virtual float get_first_time() = 0;
    virtual float get_total_charge() = 0;

};

#endif
