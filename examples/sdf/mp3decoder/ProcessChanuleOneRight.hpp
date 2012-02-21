
#ifndef ProcessChanuleOneRight_HPP
#define ProcessChanuleOneRight_HPP

#include <forsyde.hpp>
#include "include/MP3Decoder.h"

using namespace std;

void ProcessChanuleOneRight_func(
    vector<ChanuleSamples>&           out1, // headerChanuleLeft
    const vector<FrameHeader>&        inp1, // headerGranule
    const vector<FrameSideInfo>&      inp2, // sideInfoGranule
    const vector<ChanuleData>&        inp3  // chanuleData
)
{
#pragma ForSyDe begin ProcessChanule_func

    /* User-defined local variables */
    GranuleData processedMainData;
    
    /* Main actor code */
    processChanule(1, 1, &out1[0], &inp1[0], &inp2[0], &inp3[0]);
            
#pragma ForSyDe end
}

#endif
