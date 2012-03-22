
#ifndef ProcessChanuleZeroLeft_HPP
#define ProcessChanuleZeroLeft_HPP

#include <forsyde.hpp>
#include "include/MP3Decoder.h"

using namespace std;

typedef tuple<
        vector<ChanuleSamples>,
        vector<float>
    > ChanuleType;

void ProcessChanuleZeroLeft_func(
    vector<ChanuleType>&              outs, // headerChanuleLeft
    const vector<FrameHeader>&        inp1, // headerGranule
    const vector<FrameSideInfo>&      inp2, // sideInfoGranule
    const vector<ChanuleData>&        inp3, // chanuleData
    const vector<float>&              inp4  // sync
)
{
    vector<ChanuleSamples>         out1(1);
    vector<float>                  out2(1);
#pragma ForSyDe begin ProcessChanuleZeroLeft_func

    /* User-defined local variables */
    GranuleData processedMainData;
    
    /* Main actor code */
    processChanule(0, 0, &out1[0], &inp1[0], &inp2[0], &inp3[0]);
            
#pragma ForSyDe end
}

#endif
