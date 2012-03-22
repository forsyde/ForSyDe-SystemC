/******************************************************************************
 *
 * Filename: ProcessChanule.cc
 * Date: 04-05-2010
 * Author: mbozkaya
 * Description: This file contains the implementation for
 *		ProcessChanule actor &
 *		processChanule() function
 * This actor code is created from (sequential) MP3_Decoder_opt.c file
 ******************************************************************************/

#include "include/MP3Decoder.h"

#include <stdio.h>
#include <math.h>

/* This macro is used by both the short and long IMDCT's */
#define POST_TWIDDLE(i,N) (1.0 / (2.0 * cos ((2*i+1) * (C_PI / (2*N)))))

/* Local function declarations*/
static void MPG_L3_Antialias (UINT32 gr, UINT32 ch, FrameSideInfo* g_side_info, ChanuleData* chanuleData);
static void MPG_L3_Hybrid_Synthesis (UINT32 gr, UINT32 ch, FrameSideInfo* g_side_info, ChanuleData* chanuleData);
static void MPG_IMDCT_Win (FLOAT32 in[18], FLOAT32 out[36], UINT32 block_type);
static void MPG_IMDCT_Short (FLOAT32 invec[6], FLOAT32 outvec[12]);
static void MPG_IMDCT_Long (FLOAT32 invec[18], FLOAT32 outvec[36]);
static void MPG_IMDCT_3pt (FLOAT32 in[3], FLOAT32 out[3]);
static void MPG_IMDCT_4pt (FLOAT32 in[4], FLOAT32 out[4]);
static void MPG_IMDCT_5pt (FLOAT32 in[5], FLOAT32 out[5]);
static void MPG_IMDCT_9pt (FLOAT32 invec[9], FLOAT32 outvec[9]);
static void MPG_L3_Frequency_Inversion (UINT32 gr, UINT32 ch, ChanuleData* chanuleData);
static void MPG_L3_Subband_Synthesis (UINT32 gr, UINT32 ch, ChanuleSamples *outdata, FrameHeader* g_frame_header, ChanuleData* chanuleData, FLOAT32 *v_vec);
static void MPG_Polyphase_Matrixing (FLOAT32 invec[32], FLOAT32 outvec[64]);
static void MPG_DCT (FLOAT32 in[], FLOAT32 out[], int N);
static void MPG_DCT_2pt (FLOAT32 in[2], FLOAT32 out[2]);

static UINT32 hsynth_init = 1;
static UINT32 synth_init = 1;

/* Main actor function*/
void processChanule(const INT32 granuleId, const INT32 channelId, ChanuleSamples *channelSample, FrameHeader *frameHeader, FrameSideInfo *frameSideInfo, ChanuleData *frameMainData, FLOAT32 *v_vec) {
    /* Antialias */
    MPG_L3_Antialias(granuleId, channelId, frameSideInfo, frameMainData);

    /* Hybrid synthesis (IMDCT, windowing, overlapp add) */
    MPG_L3_Hybrid_Synthesis(granuleId, channelId, frameSideInfo, frameMainData);

    /* Frequency inversion */
    MPG_L3_Frequency_Inversion(granuleId, channelId, frameMainData);

    /* Polyphase subband synthesis */
    MPG_L3_Subband_Synthesis(granuleId, channelId,  channelSample, frameHeader, frameMainData, v_vec);

}

static void
MPG_L3_Antialias (UINT32 gr, UINT32 ch, FrameSideInfo* g_side_info, ChanuleData* chanuleData)
{
    UINT32 sb /* subband of 18 samples */, i, sblim, ui, li;
    FLOAT32 ub, lb;
    static FLOAT32 cs[8], ca[8];
    static FLOAT32 ci[8] = { -0.6,   -0.535, -0.33,   -0.185,
                             -0.095, -0.041, -0.0142, -0.0037
                           };
    static UINT32 init = 1;


    if (init) {
        for (i = 0; i < 8; i++) {
            cs[i] = 1.0 / sqrt (1.0 + ci[i]*ci[i]);
            ca[i] = ci[i] / sqrt (1.0 + ci[i]*ci[i]);
        }

        init = 0;
    }

    /* No antialiasing is done for short blocks */
    if (((*g_side_info).win_switch_flag[gr][ch] == 1) &&
            ((*g_side_info).block_type[gr][ch] == 2) &&
            ((*g_side_info).mixed_block_flag[gr][ch]) == 0) {
        /* Done */
        return;
    }

    /* Setup the limit for how many subbands to transform */
    if (((*g_side_info).win_switch_flag[gr][ch] == 1) &&
            ((*g_side_info).block_type[gr][ch] == 2) &&
            ((*g_side_info).mixed_block_flag[gr][ch]) == 1) {
        sblim = 2;
    } else {
        sblim = 32;
    }

    /* Do the actual antialiasing */
    for (sb = 1; sb < sblim; sb++) {
        for (i = 0; i < 8; i++) {
            li = 18*sb-1-i;
            ui = 18*sb+i;
            lb = ((*chanuleData).is[li]*cs[i]) - ((*chanuleData).is[ui]*ca[i]);
            ub = ((*chanuleData).is[ui]*cs[i]) + ((*chanuleData).is[li]*ca[i]);
            (*chanuleData).is[li] = lb;
            (*chanuleData).is[ui] = ub;
        }
    }

    /* Done */
    return;

}


static void
MPG_L3_Hybrid_Synthesis (UINT32 gr, UINT32 ch, FrameSideInfo* g_side_info, ChanuleData* chanuleData)
{
    UINT32 sb, i, j, bt;
    FLOAT32 rawout[36];
    static FLOAT32 store[2][32][18];


    if (hsynth_init) {
        /* Clear stored samples vector */
        for (j = 0; j < 2; j++) {
            for (sb = 0; sb < 32; sb++) {
                for (i = 0; i < 18; i++) {
                    store[j][sb][i] = 0.0;
                }
            }
        }
        hsynth_init = 0;
    } /* end if (init) */

    /* Loop through all 32 subbands */
    for (sb = 0; sb < 32; sb++) {

        /* Determine blocktype for this subband */
        if (((*g_side_info).win_switch_flag[gr][ch] == 1) &&
                ((*g_side_info).mixed_block_flag[gr][ch] == 1) && (sb < 2)) {
            bt = 0;     /* Long blocks in first 2 subbands */
        } else {
            bt = (*g_side_info).block_type[gr][ch];
        }

        /* Do the inverse modified DCT and windowing */
        MPG_IMDCT_Win (&((*chanuleData).is[sb*18]), rawout, bt);

        /* Overlapp add with stored vector into main_data vector */
        for (i = 0; i < 18; i++) {

            (*chanuleData).is[sb*18 + i] = rawout[i] + store[ch][sb][i];
            store[ch][sb][i] = rawout[i + 18];

        } /* end for (i... */

    } /* end for (sb... */

    /* Done */
    return;

}

static void
MPG_IMDCT_Win (FLOAT32 in[18], FLOAT32 out[36], UINT32 block_type)
{
    static FLOAT32 g_imdct_win[4][36];
    UINT32 i, m, p;
    FLOAT32 tmp[12];
    FLOAT32 tin[18];
    static UINT32 init = 1;


    /* Setup the four (one for each block type) window vectors */
    if (init) {

        /* Blocktype 0 */
        for (i = 0; i < 36; i++)
            g_imdct_win[0][i] = sin (C_PI/36 * (i + 0.5));

        /* Blocktype 1 */
        for (i = 0; i < 18; i++)
            g_imdct_win[1][i] = sin (C_PI/36 * (i + 0.5));
        for (i = 18; i < 24; i++)
            g_imdct_win[1][i] = 1.0;
        for (i = 24; i < 30; i++)
            g_imdct_win[1][i] = sin (C_PI/12 * (i + 0.5 - 18.0));
        for (i = 30; i < 36; i++)
            g_imdct_win[1][i] = 0.0;

        /* Blocktype 2 */
        for (i = 0; i < 12; i++)
            g_imdct_win[2][i] = sin (C_PI/12 * (i + 0.5));
        for (i = 12; i < 36; i++)
            g_imdct_win[2][i] = 0.0;

        /* Blocktype 3 */
        for (i = 0; i < 6; i++)
            g_imdct_win[3][i] = 0.0;
        for (i = 6; i < 12; i++)
            g_imdct_win[3][i] = sin (C_PI/12 * (i + 0.5 - 6.0));
        for (i = 12; i < 18; i++)
            g_imdct_win[3][i] = 1.0;
        for (i = 18; i < 36; i++)
            g_imdct_win[3][i] = sin (C_PI/36 * (i + 0.5));

        init = 0;

    } /* end of init */

    if (block_type == 2) {  /* 3 short blocks */

        for (i = 0; i < 36; i++) {
            out[i] = 0.0;
        }

        /* The short blocks input vector has to be re-arranged */
        for (i = 0; i < 3; i++) {
            for (m = 0; m < 6; m++) {
                tin[i*6+m] = in[i+3*m];
            }
        }

        for (i = 0; i < 3; i++) {

            MPG_IMDCT_Short (&tin[6*i], tmp);

            /* The three short blocks must be windowed and overlapped added
             * with each other */
            for (p = 0; p < 12; p++) {
                out[6*i+p+6] += tmp[p] * g_imdct_win[block_type][p];
            }

        } /* end for (i... */

    } else { /* block_type != 2 */

        MPG_IMDCT_Long (in, out);

        /* Perform the windowing. This could be inlined in the MPG_IMDCT_Long()
         * function. */
        for (i = 0; i < 36; i++) {
            out[i] *= g_imdct_win[block_type][i];
        }

    }

}


static void
MPG_IMDCT_Short (FLOAT32 invec[6], FLOAT32 outvec[12])
{
    int i;
    FLOAT32 H[6], h[6], even[3], odd[3], even_idct[3], odd_idct[3];


    /* Preprocess the input to the two 3-point IDCT's */
    H[0] = invec[0];
    for (i = 1; i < 6; i++) {
        H[i] = invec[i-1] + invec[i]; /* 5 flop */
    }

    even[0] = H[0];
    even[1] = H[2];
    even[2] = H[4];

    MPG_IMDCT_3pt (even, even_idct);   /* 6 flop */

    odd[0] = H[1];
    odd[1] = H[1] + H[3];
    odd[2] = H[3] + H[5];         /* Total 2 flop */

    MPG_IMDCT_3pt (odd, odd_idct);  /* 6 flop */

    /* Post-Twiddle */
    odd_idct[0] *= POST_TWIDDLE (0, 6);
    odd_idct[1] *= POST_TWIDDLE (1, 6);
    odd_idct[2] *= POST_TWIDDLE (2, 6);

    h[0] = even_idct[0] + odd_idct[0];
    h[1] = even_idct[1] + odd_idct[1];
    h[2] = even_idct[2] + odd_idct[2];

    h[3] = even_idct[2] - odd_idct[2];
    h[4] = even_idct[1] - odd_idct[1];
    h[5] = even_idct[0] - odd_idct[0]; /* Total: 9 flop */

    for (i = 0; i < 6; i++) {
        h[i] *= POST_TWIDDLE (i, 12.0); /* 6 flop */
    }

    /* Total: 5+6+2+6+9+6=34 flop */

    /* Rearrange the 6 values from the IDCT to the output vector */
    outvec[0]  =  h[3];
    outvec[1]  =  h[4];
    outvec[2]  =  h[5];
    outvec[3]  = -h[5];
    outvec[4]  = -h[4];
    outvec[5]  = -h[3];
    outvec[6]  = -h[2];
    outvec[7]  = -h[1];
    outvec[8]  = -h[0];
    outvec[9]  = -h[0];
    outvec[10] = -h[1];
    outvec[11] = -h[2];

    return;

}

void
MPG_IMDCT_Long (FLOAT32 invec[18], FLOAT32 outvec[36])
{
    int i;
    FLOAT32 H[18], h[18], even[9], odd[9], even_idct[9], odd_idct[9];


    H[0] = invec[0];
    for (i = 1; i < 18; i++) {
        H[i] = invec[i-1] + invec[i]; /* 17 flop */
    }

    for (i = 0; i < 9; i++) {
        even[i] = H[i*2];
    }

    MPG_IMDCT_9pt (even, even_idct);   /* 63 flop */

    odd[0] = H[1];
    for (i = 1; i < 9; i++) {
        odd[i] = H[i*2-1] + H[i*2+1];   /* Total 8 flop */
    }

    MPG_IMDCT_9pt (odd, odd_idct);  /* 63 flop */

    /* Post-Twiddle */
    for (i = 0; i < 9; i++) {
        odd_idct[i] *= POST_TWIDDLE (i, 18.0); /* Total 9 flop */
    }

    for (i = 0; i < 9; i++) {
        h[i] = even_idct[i] + odd_idct[i]; /* Total 9 flop */
    }

    for (i = 9; i < 18; i++) {
        h[i] = even_idct[17-i] - odd_idct[17-i]; /* Total 9 flop */
    }

    for (i = 0; i < 18; i++) {
        h[i] *= POST_TWIDDLE (i, 36.0); /* 18 flop */
    }

    /* Total: 17+63+8+63+9+9+9+18 = 196 flop */

    /* Rearrange the 18 values from the IDCT to the output vector */
    outvec[0]  =  h[9];
    outvec[1]  =  h[10];
    outvec[2]  =  h[11];
    outvec[3]  =  h[12];
    outvec[4]  =  h[13];
    outvec[5]  =  h[14];
    outvec[6]  =  h[15];
    outvec[7]  =  h[16];
    outvec[8]  =  h[17];

    outvec[9]  = -h[17];
    outvec[10] = -h[16];
    outvec[11] = -h[15];
    outvec[12] = -h[14];
    outvec[13] = -h[13];
    outvec[14] = -h[12];
    outvec[15] = -h[11];
    outvec[16] = -h[10];
    outvec[17] = -h[9];

    outvec[18] = -h[8];
    outvec[19] = -h[7];
    outvec[20] = -h[6];
    outvec[21] = -h[5];
    outvec[22] = -h[4];
    outvec[23] = -h[3];
    outvec[24] = -h[2];
    outvec[25] = -h[1];
    outvec[26] = -h[0];

    outvec[27] = -h[0];
    outvec[28] = -h[1];
    outvec[29] = -h[2];
    outvec[30] = -h[3];
    outvec[31] = -h[4];
    outvec[32] = -h[5];
    outvec[33] = -h[6];
    outvec[34] = -h[7];
    outvec[35] = -h[8];

    return;

}

static void
MPG_IMDCT_3pt (FLOAT32 in[3], FLOAT32 out[3])
{
    FLOAT32 t0, t1;


    t0 = in[2]/2.0 + in[0];       /* 2 flop */
    t1 = in[1] * (sqrt (3) / 2.0); /* 1 flop */

    out[0] = t0 + t1;             /* 1 flop */
    out[1] = in[0] - in[2];       /* 1 flop */
    out[2] = t0 - t1;             /* 1 flop */

    /* Total of 6 flop */
}

static void
MPG_IMDCT_4pt (FLOAT32 in[4], FLOAT32 out[4])
{
    FLOAT32 t0, t1;


    t0 = in[3]/2.0 + in[0];       /* 2 flop */
    t1 = in[1] - in[2];           /* 1 flop */

    out[0] = t0 + in[1] * cos(C_PI/9.0) + in[2] * cos((2.0*C_PI)/9.0); /* 4 flop */
    out[1] = t1/2.0 + in[0] - in[3]; /* 3 flop */
    out[2] = t0 - in[1] * cos((4.0*C_PI)/9.0) - in[2] * cos(C_PI/9.0); /* 4 flop */
    out[3] = t0 - in[1] * cos((2.0*C_PI)/9.0) + in[2] * cos((4.0*C_PI)/9.0); /* 4 flop */


    /* Total of 18 flop */
}


static void
MPG_IMDCT_5pt (FLOAT32 in[5], FLOAT32 out[5])
{
    FLOAT32 t0, t1, t2;


    t0 = in[3]/2.0 + in[0];       /* 2 flop */
    t1 = in[0] - in[3];           /* 1 flop */
    t2 = in[1] - in[2] - in[4];   /* 2 flop */

    out[0] = t0 + in[1] * cos(C_PI/9.0) + in[2] * cos((2.0*C_PI)/9.0) +
             in[4] * cos((4.0*C_PI)/9.0); /* 6 flop */

    out[1] = t2/2.0 + t1; /* 2 flop */

    out[2] = t0 - in[1] * cos((4.0*C_PI)/9.0) - in[2] * cos(C_PI/9.0) +
             in[4] * cos((2.0*C_PI)/9.0); /* 6 flop */

    out[3] = t0 - in[1] * cos((2.0*C_PI)/9.0) + in[2] * cos((4.0*C_PI)/9.0) -
             in[4] * cos((1.0*C_PI)/9.0); /* 6 flop */

    out[4] = t1 - t2;             /* 1 flop */

    /* Total of 26 flop */
}

static void
MPG_IMDCT_9pt (FLOAT32 invec[9], FLOAT32 outvec[9])
{
    int i;
    FLOAT32 H[9], h[9], even[5], odd[4], even_idct[5], odd_idct[4];


    for (i = 0; i < 9; i++) {
        H[i] = invec[i];
    }

    for (i = 0; i < 5; i++) {
        even[i] = H[2*i];
    }

    MPG_IMDCT_5pt (even, even_idct);   /* 26 flop */

    odd[0] = H[1];
    for (i = 1; i < 4; i++) {
        odd[i] = H[2*i-1] + H[2*i+1]; /* 3 flop */
    }

    MPG_IMDCT_4pt (odd, odd_idct);  /* 18 flop */

    /* Adjust for non power of 2 IDCT */
    odd_idct[0] +=  invec[7] * sin ((2*0+1)*(C_PI/18.0));
    odd_idct[1] += -invec[7] * sin ((2*1+1)*(C_PI/18.0));
    odd_idct[2] +=  invec[7] * sin ((2*2+1)*(C_PI/18.0));
    odd_idct[3] += -invec[7] * sin ((2*3+1)*(C_PI/18.0)); /* Total 4 flop */

    /* Post-Twiddle */
    for (i = 0; i < 4; i++) {
        odd_idct[i] *= POST_TWIDDLE (i, 9.0); /* Total 4 flop */
    }

    for (i = 0; i < 4; i++) {
        h[i] = even_idct[i] + odd_idct[i]; /* Total 4 flop */
    }

    h[4] = even_idct[4];

    for (i = 5; i < 9; i++) {
        h[i] = even_idct[8-i] - odd_idct[8-i]; /* Total 4 flop */
    }

    for (i = 0; i < 9; i++) {
        outvec[i] = h[i];
    }

    /* Total: 63 flop */

    return;

}

static void
MPG_L3_Frequency_Inversion (UINT32 gr, UINT32 ch, ChanuleData* chanuleData)
{
    UINT32 sb, i;


    for (sb = 1; sb < 32; sb += 2) {
        for (i = 1; i < 18; i += 2) {
            (*chanuleData).is[sb*18 + i] = -((*chanuleData).is[sb*18 + i]);
        }
    }

    /* Done */
    return;

}

static void
MPG_L3_Subband_Synthesis (UINT32 gr, UINT32 ch, ChanuleSamples *outdata, FrameHeader* g_frame_header, ChanuleData* chanuleData, FLOAT32 *v_vec)
{
    FLOAT32 g_synth_dtbl[512] = {
        0.000000000, -0.000015259, -0.000015259, -0.000015259,
        -0.000015259, -0.000015259, -0.000015259, -0.000030518,
        -0.000030518, -0.000030518, -0.000030518, -0.000045776,
        -0.000045776, -0.000061035, -0.000061035, -0.000076294,
        -0.000076294, -0.000091553, -0.000106812, -0.000106812,
        -0.000122070, -0.000137329, -0.000152588, -0.000167847,
        -0.000198364, -0.000213623, -0.000244141, -0.000259399,
        -0.000289917, -0.000320435, -0.000366211, -0.000396729,
        -0.000442505, -0.000473022, -0.000534058, -0.000579834,
        -0.000625610, -0.000686646, -0.000747681, -0.000808716,
        -0.000885010, -0.000961304, -0.001037598, -0.001113892,
        -0.001205444, -0.001296997, -0.001388550, -0.001480103,
        -0.001586914, -0.001693726, -0.001785278, -0.001907349,
        -0.002014160, -0.002120972, -0.002243042, -0.002349854,
        -0.002456665, -0.002578735, -0.002685547, -0.002792358,
        -0.002899170, -0.002990723, -0.003082275, -0.003173828,
        0.003250122,  0.003326416,  0.003387451,  0.003433228,
        0.003463745,  0.003479004,  0.003479004,  0.003463745,
        0.003417969,  0.003372192,  0.003280640,  0.003173828,
        0.003051758,  0.002883911,  0.002700806,  0.002487183,
        0.002227783,  0.001937866,  0.001617432,  0.001266479,
        0.000869751,  0.000442505, -0.000030518, -0.000549316,
        -0.001098633, -0.001693726, -0.002334595, -0.003005981,
        -0.003723145, -0.004486084, -0.005294800, -0.006118774,
        -0.007003784, -0.007919312, -0.008865356, -0.009841919,
        -0.010848999, -0.011886597, -0.012939453, -0.014022827,
        -0.015121460, -0.016235352, -0.017349243, -0.018463135,
        -0.019577026, -0.020690918, -0.021789551, -0.022857666,
        -0.023910522, -0.024932861, -0.025909424, -0.026840210,
        -0.027725220, -0.028533936, -0.029281616, -0.029937744,
        -0.030532837, -0.031005859, -0.031387329, -0.031661987,
        -0.031814575, -0.031845093, -0.031738281, -0.031478882,
        0.031082153,  0.030517578,  0.029785156,  0.028884888,
        0.027801514,  0.026535034,  0.025085449,  0.023422241,
        0.021575928,  0.019531250,  0.017257690,  0.014801025,
        0.012115479,  0.009231567,  0.006134033,  0.002822876,
        -0.000686646, -0.004394531, -0.008316040, -0.012420654,
        -0.016708374, -0.021179199, -0.025817871, -0.030609131,
        -0.035552979, -0.040634155, -0.045837402, -0.051132202,
        -0.056533813, -0.061996460, -0.067520142, -0.073059082,
        -0.078628540, -0.084182739, -0.089706421, -0.095169067,
        -0.100540161, -0.105819702, -0.110946655, -0.115921021,
        -0.120697021, -0.125259399, -0.129562378, -0.133590698,
        -0.137298584, -0.140670776, -0.143676758, -0.146255493,
        -0.148422241, -0.150115967, -0.151306152, -0.151962280,
        -0.152069092, -0.151596069, -0.150497437, -0.148773193,
        -0.146362305, -0.143264771, -0.139450073, -0.134887695,
        -0.129577637, -0.123474121, -0.116577148, -0.108856201,
        0.100311279,  0.090927124,  0.080688477,  0.069595337,
        0.057617188,  0.044784546,  0.031082153,  0.016510010,
        0.001068115, -0.015228271, -0.032379150, -0.050354004,
        -0.069168091, -0.088775635, -0.109161377, -0.130310059,
        -0.152206421, -0.174789429, -0.198059082, -0.221984863,
        -0.246505737, -0.271591187, -0.297210693, -0.323318481,
        -0.349868774, -0.376800537, -0.404083252, -0.431655884,
        -0.459472656, -0.487472534, -0.515609741, -0.543823242,
        -0.572036743, -0.600219727, -0.628295898, -0.656219482,
        -0.683914185, -0.711318970, -0.738372803, -0.765029907,
        -0.791213989, -0.816864014, -0.841949463, -0.866363525,
        -0.890090942, -0.913055420, -0.935195923, -0.956481934,
        -0.976852417, -0.996246338, -1.014617920, -1.031936646,
        -1.048156738, -1.063217163, -1.077117920, -1.089782715,
        -1.101211548, -1.111373901, -1.120223999, -1.127746582,
        -1.133926392, -1.138763428, -1.142211914, -1.144287109,
        1.144989014,  1.144287109,  1.142211914,  1.138763428,
        1.133926392,  1.127746582,  1.120223999,  1.111373901,
        1.101211548,  1.089782715,  1.077117920,  1.063217163,
        1.048156738,  1.031936646,  1.014617920,  0.996246338,
        0.976852417,  0.956481934,  0.935195923,  0.913055420,
        0.890090942,  0.866363525,  0.841949463,  0.816864014,
        0.791213989,  0.765029907,  0.738372803,  0.711318970,
        0.683914185,  0.656219482,  0.628295898,  0.600219727,
        0.572036743,  0.543823242,  0.515609741,  0.487472534,
        0.459472656,  0.431655884,  0.404083252,  0.376800537,
        0.349868774,  0.323318481,  0.297210693,  0.271591187,
        0.246505737,  0.221984863,  0.198059082,  0.174789429,
        0.152206421,  0.130310059,  0.109161377,  0.088775635,
        0.069168091,  0.050354004,  0.032379150,  0.015228271,
        -0.001068115, -0.016510010, -0.031082153, -0.044784546,
        -0.057617188, -0.069595337, -0.080688477, -0.090927124,
        0.100311279,  0.108856201,  0.116577148,  0.123474121,
        0.129577637,  0.134887695,  0.139450073,  0.143264771,
        0.146362305,  0.148773193,  0.150497437,  0.151596069,
        0.152069092,  0.151962280,  0.151306152,  0.150115967,
        0.148422241,  0.146255493,  0.143676758,  0.140670776,
        0.137298584,  0.133590698,  0.129562378,  0.125259399,
        0.120697021,  0.115921021,  0.110946655,  0.105819702,
        0.100540161,  0.095169067,  0.089706421,  0.084182739,
        0.078628540,  0.073059082,  0.067520142,  0.061996460,
        0.056533813,  0.051132202,  0.045837402,  0.040634155,
        0.035552979,  0.030609131,  0.025817871,  0.021179199,
        0.016708374,  0.012420654,  0.008316040,  0.004394531,
        0.000686646, -0.002822876, -0.006134033, -0.009231567,
        -0.012115479, -0.014801025, -0.017257690, -0.019531250,
        -0.021575928, -0.023422241, -0.025085449, -0.026535034,
        -0.027801514, -0.028884888, -0.029785156, -0.030517578,
        0.031082153,  0.031478882,  0.031738281,  0.031845093,
        0.031814575,  0.031661987,  0.031387329,  0.031005859,
        0.030532837,  0.029937744,  0.029281616,  0.028533936,
        0.027725220,  0.026840210,  0.025909424,  0.024932861,
        0.023910522,  0.022857666,  0.021789551,  0.020690918,
        0.019577026,  0.018463135,  0.017349243,  0.016235352,
        0.015121460,  0.014022827,  0.012939453,  0.011886597,
        0.010848999,  0.009841919,  0.008865356,  0.007919312,
        0.007003784,  0.006118774,  0.005294800,  0.004486084,
        0.003723145,  0.003005981,  0.002334595,  0.001693726,
        0.001098633,  0.000549316,  0.000030518, -0.000442505,
        -0.000869751, -0.001266479, -0.001617432, -0.001937866,
        -0.002227783, -0.002487183, -0.002700806, -0.002883911,
        -0.003051758, -0.003173828, -0.003280640, -0.003372192,
        -0.003417969, -0.003463745, -0.003479004, -0.003479004,
        -0.003463745, -0.003433228, -0.003387451, -0.003326416,
        0.003250122,  0.003173828,  0.003082275,  0.002990723,
        0.002899170,  0.002792358,  0.002685547,  0.002578735,
        0.002456665,  0.002349854,  0.002243042,  0.002120972,
        0.002014160,  0.001907349,  0.001785278,  0.001693726,
        0.001586914,  0.001480103,  0.001388550,  0.001296997,
        0.001205444,  0.001113892,  0.001037598,  0.000961304,
        0.000885010,  0.000808716,  0.000747681,  0.000686646,
        0.000625610,  0.000579834,  0.000534058,  0.000473022,
        0.000442505,  0.000396729,  0.000366211,  0.000320435,
        0.000289917,  0.000259399,  0.000244141,  0.000213623,
        0.000198364,  0.000167847,  0.000152588,  0.000137329,
        0.000122070,  0.000106812,  0.000106812,  0.000091553,
        0.000076294,  0.000076294,  0.000061035,  0.000061035,
        0.000045776,  0.000045776,  0.000030518,  0.000030518,
        0.000030518,  0.000030518,  0.000015259,  0.000015259,
        0.000015259,  0.000015259,  0.000015259,  0.000015259,
    };
    FLOAT32 u_vec[512];
    FLOAT32 s_vec[32], sum;
    INT32 samp;
    UINT32 ss;
    static UINT32 init = 1;
    UINT32 i, j;
    UINT32 nch;
    //~ static FLOAT32 v_vec[2 /* ch */][1024];


    /* Number of channels (1 for mono and 2 for stereo) */
    nch = ((*g_frame_header).mode == mpeg1_mode_single_channel ? 1 : 2);


    //~ /* Setup the n_win windowing vector and the v_vec intermediate vector */
    //~ if (init) {
//~ 
        //~ synth_init = 1;
//~ 
        //~ init = 0;
    //~ } /* end if (init) */
//~ 
    //~ if (synth_init) {
//~ 
        //~ /* Setup the v_vec intermediate vector */
        //~ for (i = 0; i < 2; i++) {
            //~ for (j = 0; j < 1024; j++) {
                //~ v_vec[i][j] = 0.0;
            //~ }
        //~ }
//~ 
        //~ synth_init = 0;
    //~ } /* end if (synth_init) */

    /* Loop through the 18 samples in each of the 32 subbands */
    for (ss = 0; ss < 18; ss++) {

        /* Shift up the V vector */
        for (i = 1023; i > 63; i--) {
            v_vec[i] = v_vec[i-64];
        }

        /* Copy the next 32 time samples to a temp vector */
        for (i = 0; i < 32; i++) {
            s_vec[i] = ((FLOAT32) (*chanuleData).is[i*18 + ss]);
        }

        /* Perform the matrixing operation on the input vector */
        MPG_Polyphase_Matrixing (s_vec, v_vec);

        /* Build the U vector */
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 32; j++) {
                u_vec[i*64 + j]      = v_vec[i*128 + j];
                u_vec[i*64 + j + 32] = v_vec[i*128 + j + 96];
            }
        } /* end for (i... */

        /* Window by u_vec[i] with g_synth_dtbl[i] */
        for (i = 0; i < 512; i++) {
            u_vec[i] = u_vec[i] * g_synth_dtbl[i];
        }

        /* Calculate 32 samples and store them in the outdata vector */
        for (i = 0; i < 32; i++) {
            sum = 0.0;
            for (j = 0; j < 16; j++) {
                sum += u_vec[j*32 + i];
            }

            /* sum now contains time sample 32*ss+i. Convert to 16-bit signed int */
            samp = (INT32) (sum * 32767.0);
            if (samp > 32767) {
                samp = 32767;
            } else if (samp < -32767) {
                samp = -32767;
            }

            samp &= 0xffff;

            /* This function must be called for channel 0 first */
#if 0
            if (ch == 0) {
                /* We always run the audio system in stereo mode, and duplicate
                 * the channels here for mono */
                if (nch == 1) {

                    outdata->samples[32*ss + i] = (samp << 16) | (samp);

                } else {
                    outdata->samples[32*ss + i] = samp << 16;
                }
            } else {
                outdata->samples[32*ss + i] |= samp;
            }
#endif

            /* KPN takes care of the merge (binary or) */
            if (ch == 0) {
                if (nch == 1)
                {
                    outdata->samples[32 * ss + i] = (samp << 16) | (samp);
                }
                else
                {
                    outdata->samples[32 * ss + i] = samp << 16;
                }
            }
            else {
                outdata->samples[32 * ss + i] = samp;
            }


        } /* end for (i... */

    } /* end for (ss... */

    /* Done */
    return;

}

static void
MPG_Polyphase_Matrixing (FLOAT32 invec[32], FLOAT32 outvec[64])
{
    int i;
    FLOAT32 tmp[32];


    MPG_DCT (invec, tmp, 32);

    for (i = 0; i < 16; i++) {
        outvec[i] = tmp[i+16];
    }

    outvec[16] = 0.0;

    for (i = 17; i < 32; i++) {
        outvec[i] = -tmp[48-i];
    }

    for (i = 32; i < 48; i++) {
        outvec[i] = -tmp[48-i];
    }

    for (i = 48; i < 64; i++) {
        outvec[i] = -tmp[i-48];
    }

    return;

}

static void
MPG_DCT (FLOAT32 in[], FLOAT32 out[], int N)
{
    int i;
    FLOAT32 even_in[N/2], even_out[N/2], odd_in[N/2], odd_out[N/2];


    /* We use recursion here to make the function easier to understand.
     * It should be unrolled manually in order to make it more effective for
     * a real implementation.
     */
    if (N == 2) {
        MPG_DCT_2pt (in, out);
        return;
    }

    for (i = 0; i < N/2; i++) {
        even_in[i] = in[i] + in[N-1-i]; /* N/2 flop */
    }

    MPG_DCT (even_in, even_out, N/2); /* DCT(N/2) flop */

    for (i = 0; i < N/2; i++) {
        odd_in[i] = (in[i] - in[N-1-i]) * POST_TWIDDLE (i, N); /* N flop */
    }

    MPG_DCT (odd_in, odd_out, N/2); /* DCT(N/2) flop */

    for (i = 0; i < N/2; i++) {
        out[2*i] = even_out[i];
    }

    for (i = 0; i < N/2-1; i++) {
        out[2*i+1] = odd_out[i] + odd_out[i+1]; /* N/2 - 1 flop */
    }
    out[N-1] = odd_out[N/2-1];

    /* Total of 2*N - 1 + 2*DCT(N/2) flop */

}

static void
MPG_DCT_2pt (FLOAT32 in[2], FLOAT32 out[2])
{
    int i, j;


    for (i = 0; i < 2; i++) {
        out[i] = 0.0;
        for (j = 0; j < 2; j++) {
            out[i] += in[j] * cos((2*j+1)*i*(C_PI/4.0));
        }
    }

}

