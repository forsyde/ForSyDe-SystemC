
#ifndef ReadBitstreamAndExtractFrames_HPP
#define ReadBitstreamAndExtractFrames_HPP

#include <forsyde.hpp>
#include "include/MP3Decoder.h"

using namespace std;

typedef tuple<
        vector<float>,                // dummyCounter
        vector<bool>,               // lastFrame
        vector<FrameHeader>,        // headerMerge
        vector<FrameHeader>,        // headerGranule0
        vector<FrameSideInfo>,      // sideInfoGranule0
        vector<GranuleData>,        // granuleData0
        vector<FrameHeader>,        // headerGranule1
        vector<FrameSideInfo>,      // sideInfoGranule1
        vector<GranuleData>         // granuleData1
    > InputType;

void ReadBitstreamAndExtractFrames_func(
    vector<InputType>& outs,
    const vector<float>& inp1)
{
    vector<float>               out1(1);    // dummyCounter
    vector<bool>                out2(1);    // lastFrame
    vector<FrameHeader>         out3(1);    // headerMerge
    vector<FrameHeader>         out4(1);    // headerGranule0
    vector<FrameSideInfo>       out5(1);    // sideInfoGranule0
    vector<GranuleData>         out6(1);    // granuleData0
    vector<FrameHeader>         out7(1);    // headerGranule1
    vector<FrameSideInfo>       out8(1);    // sideInfoGranule1
    vector<GranuleData>         out9(1);    // granuleData1
    
#pragma ForSyDe begin ReadBitstreamAndExtractFrames_func
    /* User-defined local variables */
    char* file_name = "test.mp3";
    static FrameHeader header;
    static FrameSideInfo sideInfo = {0};
    static FrameMainData frameMainData;
    bool moreFrames = true;
    
    /* Main actor code */

    #ifdef FORSYDE_HPP
    moreFrames = readBitstreamAndExtractFrames(file_name, &header, &sideInfo, &frameMainData);
    #endif

    out1[0] = 1;
    out2[0] = !moreFrames;
    out3[0] = header;
    out4[0] = header;
    out5[0] = sideInfo;
    copyGranuleData(frameMainData, 0, out6[0]);
    out7[0] = header;
    out8[0] = sideInfo;
    copyGranuleData(frameMainData, 1, out9[0]);

#pragma ForSyDe end
    outs[0] = make_tuple(out1,out2,out3,out4,out5,out6,out7,out8,out9);
}

#endif
