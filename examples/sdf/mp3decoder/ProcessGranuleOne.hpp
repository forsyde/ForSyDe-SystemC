
#ifndef ProcessGranuleOne_HPP
#define ProcessGranuleOne_HPP

#include <forsyde.hpp>
#include "include/MP3Decoder.h"

using namespace std;

typedef tuple<
        vector<FrameHeader>,            // headerChanuleLeft
        vector<FrameSideInfo>,          // sideInfoChanuleLeft
        vector<ChanuleData>,            // chanuleDataLeft
        vector<FrameHeader>,            // headerChanuleRight
        vector<FrameSideInfo>,          // sideInfoChanuleRight
        vector<ChanuleData>             // chanuleDataRight
    > GranuleType;

void ProcessGranuleOne_func(
    vector<GranuleType>&              outs,
    const vector<FrameHeader>&        inp1, // headerGranule
    const vector<FrameSideInfo>&      inp2, // sideInfoGranule
    const vector<GranuleData>&        inp3  // granuleData
)
{
    vector<FrameHeader>         out1(1);    // headerChanuleLeft
    vector<FrameSideInfo>       out2(1);    // sideInfoChanuleLeft
    vector<ChanuleData>         out3(1);    // chanuleDataLeft
    vector<FrameHeader>         out4(1);    // headerChanuleRight
    vector<FrameSideInfo>       out5(1);    // sideInfoChanuleRight
    vector<ChanuleData>         out6(1);    // chanuleDataRight
    
#pragma ForSyDe begin ProcessGranuleOne_func

    /* User-defined local variables */
    GranuleData processedMainData;
    
    /* Main actor code */
    processGranule(1, &inp1[0], &inp2[0], &inp3[0], &processedMainData);

    out1[0] = inp1[0];
    out2[0] = inp2[0];
    copyChanuleData(processedMainData, 0, out3[0]);

    out4[0] = inp1[0];
    out5[0] = inp2[0];
    copyChanuleData(processedMainData, 1, out6[0]);
            
#pragma ForSyDe end
    outs[0] = make_tuple(out1,out2,out3,out4,out5,out6);
}

#endif
