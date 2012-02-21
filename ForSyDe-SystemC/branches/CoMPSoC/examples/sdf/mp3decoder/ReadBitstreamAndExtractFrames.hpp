
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
        vector<t_sf_band_indices*>,  // bandIndicesGranule0
        vector<FrameHeader>,        // headerGranule1
        vector<FrameSideInfo>,      // sideInfoGranule1
        vector<GranuleData>,        // granuleData1
        vector<t_sf_band_indices*>   // bandIndicesGranule1
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
    vector<t_sf_band_indices*>  out7(1);    // bandIndicesGranule0
    vector<FrameHeader>         out8(1);    // headerGranule1
    vector<FrameSideInfo>       out9(1);    // sideInfoGranule1
    vector<GranuleData>         out10(1);   // granuleData1
    vector<t_sf_band_indices*>  out11(1);   // bandIndicesGranule1
    
#pragma ForSyDe begin ReadBitstreamAndExtractFrames_func
    /* User-defined local variables */
    char* file_name = "test.mp3";
    static FrameHeader header;
    static FrameSideInfo sideInfo = {0};
    static FrameMainData frameMainData;
    t_sf_band_indices bandIndices[3] =   {
        {
            { 0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 52, 62, 74, 90, 110, 134, 162,
                196, 238, 288, 342, 418, 576 },
            { 0, 4, 8, 12, 16, 22, 30, 40, 52, 66, 84, 106, 136, 192 }
        },
        {
            { 0, 4, 8, 12, 16, 20, 24, 30, 36, 42, 50, 60, 72, 88, 106, 128, 156,
                190, 230, 276, 330, 384, 576 },
            { 0, 4, 8, 12, 16, 22, 28, 38, 50, 64, 80, 100, 126, 192 }
        },
        {
            { 0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 54, 66, 82, 102, 126, 156, 194,
                240, 296, 364, 448, 550, 576 },
            { 0, 4, 8, 12, 16, 22, 30, 42, 58, 78, 104, 138, 180, 192 }
        }
    };
    bool moreFrames = true;
    
    /* Main actor code */

    #ifdef FORSYDE_HPP
    moreFrames = readBitstreamAndExtractFrames(file_name, &header, &sideInfo, &frameMainData, bandIndices);
    #endif

    out1[0] = 1;
    out2[0] = !moreFrames;
    out3[0] = header;
    out4[0] = header;
    out5[0] = sideInfo;
    copyGranuleData(frameMainData, 0, out6[0]);
    out7[0] = bandIndices;
    out8[0] = header;
    out9[0] = sideInfo;
    copyGranuleData(frameMainData, 1, out10[0]);
    out11[0] = bandIndices;

#pragma ForSyDe end
    outs[0] = make_tuple(out1,out2,out3,out4,out5,
                         out6,out7,out8,out9,out10,out11);
}

#endif
