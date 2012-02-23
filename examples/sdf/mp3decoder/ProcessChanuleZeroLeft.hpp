
#ifndef ProcessChanuleZeroLeft_HPP
#define ProcessChanuleZeroLeft_HPP

#include <forsyde.hpp>
#include "include/MP3Decoder.h"

using namespace std;

void ProcessChanuleZeroLeft_func(
    vector<ChanuleSamples>&           out1, // headerChanuleLeft
    const vector<FrameHeader>&        inp1, // headerGranule
    const vector<FrameSideInfo>&      inp2, // sideInfoGranule
    const vector<ChanuleData>&        inp3  // chanuleData
)
{
#pragma ForSyDe begin ProcessChanuleZeroLeft_func

    /* User-defined local variables */
    GranuleData processedMainData;
    
    /* Main actor code */
    processChanule(0, 0, &out1[0], &inp1[0], &inp2[0], &inp3[0]);
            
#pragma ForSyDe end
}

#endif
