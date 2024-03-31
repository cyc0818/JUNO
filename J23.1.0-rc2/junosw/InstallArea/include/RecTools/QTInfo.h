#ifndef QTINFO_H
#define QTINFO_H

#include <cstdint>
#include <vector>

struct QTInfo{
    bool is_valid = false;

    int pmtid = -1;
    std::vector<double> wf;
    double bsl_sigma = -1;
    bool b_getFHT = false;
    
    // results
    float nPE = -1;
    float firstHitTime = -100;
    std::vector<float> charge;
    std::vector<float> time;

    QTInfo(int id, std::vector<uint16_t>& adc_int){
        if(adc_int.empty() || id>=17612) return;

        const int L = adc_int.size();
        wf .reserve(L);
        for(int i=0;i<L;i++){
            if (adc_int[i] >> 14 == 0) wf.emplace_back( double(adc_int[i]) );
            if (adc_int[i] >> 14 == 1) wf.emplace_back( double(adc_int[i]-(1<<14))  * .55/.08 );
        }

        charge.reserve(32);
        time  .reserve(32);
        pmtid = id;

        is_valid = true;
    }
};
#endif