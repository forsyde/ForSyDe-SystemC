/******************************************************************************
*
* Filename: MPG_Decode_L3.c
* Author: Krister Lagerström (krister@kmlager.com)
* Description: This file contains the Level 3 decoding function.
*
******************************************************************************/

#include "include/MP3Decoder.h"

/* Local functions and variables (defined here, used here) */

#define POW34_TABLE
/*  #define POW34_ITERATE   */
#if !defined(POW34_TABLE) && !defined(POW34_ITERATE)
#err "Must define one of POW34_TABLE and POW34_ITERATE!"
#endif

static void MPG_L3_Requantize (UINT32 gr, UINT32 ch, FrameHeader* g_frame_header, FrameSideInfo* g_side_info, GranuleData* granuleData);
static void MPG_Requantize_Process_Long (UINT32 gr, UINT32 ch, UINT32 is_pos, UINT32 sfb, FrameSideInfo* g_side_info, GranuleData* granuleData);
static void MPG_Requantize_Process_Short (UINT32 gr, UINT32 ch, UINT32 is_pos, UINT32 sfb, UINT32 win, FrameSideInfo* g_side_info, GranuleData* granuleData);
static void MPG_L3_Reorder (UINT32 gr, UINT32 ch, FrameHeader* g_frame_header, FrameSideInfo* g_side_info, GranuleData* granuleData);
static void MPG_L3_Stereo (UINT32 gr, FrameHeader* g_frame_header, FrameSideInfo* g_side_info, GranuleData* granuleData);
static void MPG_Stereo_Process_Intensity_Long (UINT32 gr, UINT32 sfb, FrameHeader* g_frame_header, GranuleData* granuleData);
static void MPG_Stereo_Process_Intensity_Short (UINT32 gr, UINT32 sfb, FrameHeader* g_frame_header, GranuleData* granuleData);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/* Main actor function */
void processGranule(const UINT32 gr, FrameHeader *frameHeader, FrameSideInfo *frameSideInfo, GranuleData *frameInMainData, GranuleData *frameOutMainData)
{
  UINT32 ch, nch;
   /* Sampling frequencies in hertz (valid for all layers) */
  /* UINT32 g_sampling_frequency[3] = {
     44100 * Hz,
     48000 * Hz,
     32000 * Hz
   };*/

    *frameOutMainData = *frameInMainData;

    /* Number of channels (1 for mono and 2 for stereo) */
    nch = ((*frameHeader).mode == mpeg1_mode_single_channel ? 1 : 2);


    for (ch = 0; ch<nch; ch++) {
        /* Requantize samples */
        MPG_L3_Requantize (gr, ch, frameHeader, frameSideInfo, frameOutMainData);

        /* Reorder short blocks */
        MPG_L3_Reorder (gr, ch, frameHeader, frameSideInfo, frameOutMainData);
    }
    /* Stereo processing */
    MPG_L3_Stereo (gr, frameHeader, frameSideInfo, frameOutMainData);

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void
MPG_L3_Requantize (UINT32 gr, UINT32 ch, FrameHeader* g_frame_header, FrameSideInfo* g_side_info, GranuleData* granuleData)
{
    UINT32 sfb /* scalefac band index */;
    UINT32 next_sfb /* frequency of next sfb */;
    UINT32 sfreq, i, j, win, win_len;


    /* Setup sampling frequency index */
    sfreq = (*g_frame_header).sampling_frequency;

    /* Determine type of block to process */
    if (((*g_side_info).win_switch_flag[gr][ch] == 1) &&
            ((*g_side_info).block_type[gr][ch] == 2)) { /* Short blocks */

        /* Check if the first two subbands
         * (=2*18 samples = 8 long or 3 short sfb's) uses long blocks */
        if ((*g_side_info).mixed_block_flag[gr][ch] != 0) { /* 2 longbl. sb  first */

            /*
             * First process the 2 long block subbands at the start
             */
            sfb = 0;
            next_sfb = g_sf_band_indices[sfreq].l[sfb+1];
            for (i = 0; i < 36; i++) {
                if (i == next_sfb) {
                    sfb++;
                    next_sfb = g_sf_band_indices[sfreq].l[sfb+1];
                } /* end if */
                MPG_Requantize_Process_Long (gr, ch, i, sfb, g_side_info, granuleData);
            }

            /*
             * And next the remaining, non-zero, bands which uses short blocks
             */
            sfb = 3;
            next_sfb = g_sf_band_indices[sfreq].s[sfb+1] * 3;
            win_len = g_sf_band_indices[sfreq].s[sfb+1] -
                      g_sf_band_indices[sfreq].s[sfb];

            for (i = 36; i < (*g_side_info).count1[gr][ch]; /* i++ done below! */) {

                /* Check if we're into the next scalefac band */
                if (i == next_sfb) {  /* Yes */
                    sfb++;
                    next_sfb = g_sf_band_indices[sfreq].s[sfb+1] * 3;
                    win_len = g_sf_band_indices[sfreq].s[sfb+1] -
                              g_sf_band_indices[sfreq].s[sfb];
                } /* end if (next_sfb) */

                for (win = 0; win < 3; win++) {
                    for (j = 0; j < win_len; j++) {
                        MPG_Requantize_Process_Short (gr, ch, i, sfb, win, g_side_info, granuleData);
                        i++;
                    } /* end for (win... */
                } /* end for (j... */

            } /* end for (i... */

        } else {      /* Only short blocks */

            sfb = 0;
            next_sfb = g_sf_band_indices[sfreq].s[sfb+1] * 3;
            win_len = g_sf_band_indices[sfreq].s[sfb+1] -
                      g_sf_band_indices[sfreq].s[sfb];

            for (i = 0; i < (*g_side_info).count1[gr][ch]; /* i++ done below! */) {

                /* Check if we're into the next scalefac band */
                if (i == next_sfb) {  /* Yes */
                    sfb++;
                    next_sfb = g_sf_band_indices[sfreq].s[sfb+1] * 3;
                    win_len = g_sf_band_indices[sfreq].s[sfb+1] -
                              g_sf_band_indices[sfreq].s[sfb];
                } /* end if (next_sfb) */

                for (win = 0; win < 3; win++) {
                    for (j = 0; j < win_len; j++) {
                        MPG_Requantize_Process_Short (gr, ch, i, sfb, win, g_side_info, granuleData);
                        i++;
                    } /* end for (win... */
                } /* end for (j... */

            } /* end for (i... */

        } /* end else (only short blocks) */

    } else {      /* Only long blocks */

        sfb = 0;
        next_sfb = g_sf_band_indices[sfreq].l[sfb+1];
        for (i = 0; i < (*g_side_info).count1[gr][ch]; i++) {
            if (i == next_sfb) {
                sfb++;
                next_sfb = g_sf_band_indices[sfreq].l[sfb+1];
            } /* end if */
            MPG_Requantize_Process_Long (gr, ch, i, sfb, g_side_info, granuleData);
        }

    } /* end else (only long blocks) */

    /* Done */
    return;

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static FLOAT32
MPG_Requantize_Pow_43 (UINT32 is_pos)
{

#ifdef POW34_TABLE
    static FLOAT32 powtab34[8207];
    static UINT32 init = 0;
    UINT32 i;


    /* First time initialization */
    if (init == 0) {
        for (i = 0; i < 8207; i++) {
            powtab34[i] = pow ((FLOAT32) i, 4.0 / 3.0);
        }
        init = 1;
    }

#ifdef DEBUG_CHECK
    if (is_pos > 8206) {
        ERR ("is_pos = %d larger than 8206!", is_pos);
        is_pos = 8206;
    }
#endif /* DEBUG_CHECK */

    /* Done */
    return (powtab34[is_pos]);

#endif /* POW34_TABLE */


#ifdef POW34_ITERATE
    FLOAT32 a4, a2;
    FLOAT32 x, x2, x3, x_next, is_f1, is_f2, is_f3;
    UINT32 i;
    static FLOAT32 powtab34[32];
    static UINT32 init = 0;
    static FLOAT32 coeff[3] = {
        -1.030797119e+02,
        6.319399834e+00,
        2.395095071e-03,
    };


    /* First time initialization */
    if (init == 0) {
        for (i = 0; i < 32; i++) {
            powtab34[i] = pow ((FLOAT32) i, 4.0 / 3.0);
        }
        init = 1;
    }

    /* We use a table for 0<is_pos<32 since they are so common */
    if (is_pos < 32) return (powtab34[is_pos]);

    a2 = is_pos * is_pos;
    a4 = a2 * a2;

    is_f1 = (float) is_pos;
    is_f2 = is_f1 * is_f1;
    is_f3 = is_f1 * is_f2;

    /*  x = coeff[0] + coeff[1]*is_f1 + coeff[2]*is_f2 + coeff[3]*is_f3; */
    x = coeff[0] + coeff[1]*is_f1 + coeff[2]*is_f2;

    for (i = 0; i < 3; i++) {

        x2 = x*x;
        x3 = x*x2;

        x_next = (2*x3 + a4) / (3*x2);

        x = x_next;
    }

    return (x);

#endif /* POW34_ITERATE */

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void MPG_Requantize_Process_Long (UINT32 gr, UINT32 ch, UINT32 is_pos, UINT32 sfb, FrameSideInfo* g_side_info, GranuleData* granuleData)
{
    FLOAT32 res, tmp1, tmp2, tmp3, sf_mult, pf_x_pt;
    static FLOAT32 pretab[21] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  1, 1, 1, 1, 2, 2, 3, 3, 3, 2
                                };


    sf_mult = (*g_side_info).scalefac_scale[gr][ch] ? 1.0 : 0.5;
    pf_x_pt = (*g_side_info).preflag[gr][ch] * pretab[sfb];

    tmp1 =
        pow (2.0, -(sf_mult * ((*granuleData).scalefac_l[ch][sfb] + pf_x_pt)));

    tmp2 = pow (2.0, 0.25 * ((INT32) (*g_side_info).global_gain[gr][ch] - 210));

    if ((*granuleData).is[ch][is_pos] < 0.0) {
        tmp3 = -MPG_Requantize_Pow_43 (-((*granuleData).is[ch][is_pos]));
    } else {
        tmp3 = MPG_Requantize_Pow_43 ((*granuleData).is[ch][is_pos]);
    }

    res = (*granuleData).is[ch][is_pos] = tmp1 * tmp2 * tmp3;

    /* Done */
    return;

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void MPG_Requantize_Process_Short (UINT32 gr, UINT32 ch, UINT32 is_pos,
                              UINT32 sfb, UINT32 win, FrameSideInfo* g_side_info, GranuleData* granuleData)
{
    FLOAT32 res, tmp1, tmp2, tmp3, sf_mult;


    sf_mult = (*g_side_info).scalefac_scale[gr][ch] ? 1.0 : 0.5;

    tmp1 =
        pow (2.0, -(sf_mult * (*granuleData).scalefac_s[ch][sfb][win]));

    tmp2 =
        pow (2.0, 0.25 * ((FLOAT32) (*g_side_info).global_gain[gr][ch] - 210.0 -
                          8.0 * (FLOAT32) (*g_side_info).subblock_gain[gr][ch][win]));

    if ((*granuleData).is[ch][is_pos] < 0.0) {
        tmp3 = -MPG_Requantize_Pow_43 (-((*granuleData).is[ch][is_pos]));
    } else {
        tmp3 = MPG_Requantize_Pow_43 ((*granuleData).is[ch][is_pos]);
    }

    res = (*granuleData).is[ch][is_pos] = tmp1 * tmp2 * tmp3;

    /* Done */
    return;

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void
MPG_L3_Reorder (UINT32 gr, UINT32 ch, FrameHeader* g_frame_header, FrameSideInfo* g_side_info, GranuleData* granuleData)
{
    UINT32 sfreq, i, j, next_sfb, sfb, win_len, win;
    FLOAT32 re[576];


    /* Setup sampling frequency index */
    sfreq = (*g_frame_header).sampling_frequency;

    /* Only reorder short blocks */
    if (((*g_side_info).win_switch_flag[gr][ch] == 1) &&
            ((*g_side_info).block_type[gr][ch] == 2)) { /* Short blocks */

        /* Check if the first two subbands
         * (=2*18 samples = 8 long or 3 short sfb's) uses long blocks */
        if ((*g_side_info).mixed_block_flag[gr][ch] != 0) { /* 2 longbl. sb  first */

            /* Don't touch the first 36 samples */

            /*
             * Reorder the remaining, non-zero, bands which uses short blocks
             */
            sfb = 3;
            next_sfb = g_sf_band_indices[sfreq].s[sfb+1] * 3;
            win_len = g_sf_band_indices[sfreq].s[sfb+1] -
                      g_sf_band_indices[sfreq].s[sfb];

        } else { /* Only short blocks */

            sfb = 0;
            next_sfb = g_sf_band_indices[sfreq].s[sfb+1] * 3;
            win_len = g_sf_band_indices[sfreq].s[sfb+1] -
                      g_sf_band_indices[sfreq].s[sfb];

        } /* end else (only short blocks) */


        for (i = ((sfb == 0) ? 0 : 36); i < 576; /* i++ done below! */) {

            /* Check if we're into the next scalefac band */
            if (i == next_sfb) {  /* Yes */

                /* Copy reordered data back to the original vector */
                for (j = 0; j < 3*win_len; j++) {
                    (*granuleData).is[ch][3*g_sf_band_indices[sfreq].s[sfb] + j] =
                        re[j];
                }

                /* Check if this band is above the rzero region, if so we're done */
                if (i >= (*g_side_info).count1[gr][ch]) {
                    /* Done */
                    return;
                }

                sfb++;
                next_sfb = g_sf_band_indices[sfreq].s[sfb+1] * 3;
                win_len = g_sf_band_indices[sfreq].s[sfb+1] -
                          g_sf_band_indices[sfreq].s[sfb];

            } /* end if (next_sfb) */

            /* Do the actual reordering */
            for (win = 0; win < 3; win++) {
                for (j = 0; j < win_len; j++) {
                    re[j*3 + win] = (*granuleData).is[ch][i];
                    i++;
                } /* end for (j... */
            } /* end for (win... */

        } /* end for (i... */

        /* Copy reordered data of the last band back to the original vector */
        for (j = 0; j < 3*win_len; j++) {
            (*granuleData).is[ch][3 * g_sf_band_indices[sfreq].s[12] + j] = re[j];
        }

    } else {      /* Only long blocks */
        /* No reorder necessary, do nothing! */
        return;

    } /* end else (only long blocks) */

    /* Done */
    return;

}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void
MPG_L3_Stereo (UINT32 gr, FrameHeader* g_frame_header, FrameSideInfo* g_side_info, GranuleData* granuleData)
{
    UINT32 max_pos, i;
    FLOAT32 left, right;
    UINT32 sfb /* scalefac band index */;
    UINT32 sfreq;



    /* Do nothing if joint stereo is not enabled */
    if (((*g_frame_header).mode != 1) || ((*g_frame_header).mode_extension == 0)) {
        /* Done */
        return;
    }

    /* Do Middle/Side ("normal") stereo processing */
    if ((*g_frame_header).mode_extension & 0x2) {

        /* Determine how many frequency lines to transform */
        if ((*g_side_info).count1[gr][0] > (*g_side_info).count1[gr][1]) {
            max_pos = (*g_side_info).count1[gr][0];
        } else {
            max_pos = (*g_side_info).count1[gr][1];
        }

        /* Do the actual processing */
        for (i = 0; i < max_pos; i++) {
            left = ((*granuleData).is[0][i] + (*granuleData).is[1][i])
                   * (C_INV_SQRT_2);
            right = ((*granuleData).is[0][i] - (*granuleData).is[1][i])
                    * (C_INV_SQRT_2);
            (*granuleData).is[0][i] = left;
            (*granuleData).is[1][i] = right;
        } /* end for (i... */

    } /* end if (ms_stereo... */

    /* Do intensity stereo processing */
    if ((*g_frame_header).mode_extension & 0x1) {

        /* Setup sampling frequency index */
        sfreq = (*g_frame_header).sampling_frequency;

        /* The first band that is intensity stereo encoded is the first band
         * scale factor band on or above the count1 frequency line.
         * N.B.: Intensity stereo coding is only done for the higher subbands,
         * but the logic is still included to process lower subbands.
         */

        /* Determine type of block to process */
        if (((*g_side_info).win_switch_flag[gr][0] == 1) &&
                ((*g_side_info).block_type[gr][0] == 2)) { /* Short blocks */

            /* Check if the first two subbands
             * (=2*18 samples = 8 long or 3 short sfb's) uses long blocks */
            if ((*g_side_info).mixed_block_flag[gr][0] != 0) { /* 2 longbl. sb  first */

                /*
                 * First process the 8 sfb's at the start
                 */
                for (sfb = 0; sfb < 8; sfb++) {

                    /* Is this scale factor band above count1 for the right channel? */
                    if (g_sf_band_indices[sfreq].l[sfb] >= (*g_side_info).count1[gr][1]) {
                        MPG_Stereo_Process_Intensity_Long (gr, sfb, g_frame_header, granuleData);
                    }

                } /* end if (sfb... */

                /*
                 * And next the remaining bands which uses short blocks
                 */
                for (sfb = 3; sfb < 12; sfb++) {

                    /* Is this scale factor band above count1 for the right channel? */
                    if (g_sf_band_indices[sfreq].s[sfb]*3 >= (*g_side_info).count1[gr][1]) {

                        /* Perform the intensity stereo processing */
                        MPG_Stereo_Process_Intensity_Short (gr, sfb, g_frame_header, granuleData);
                    }
                }

            } else {      /* Only short blocks */

                for (sfb = 0; sfb < 12; sfb++) {

                    /* Is this scale factor band above count1 for the right channel? */
                    if (g_sf_band_indices[sfreq].s[sfb]*3 >= (*g_side_info).count1[gr][1]) {

                        /* Perform the intensity stereo processing */
                        MPG_Stereo_Process_Intensity_Short (gr, sfb, g_frame_header, granuleData);
                    }
                }

            } /* end else (only short blocks) */

        } else {      /* Only long blocks */

            for (sfb = 0; sfb < 21; sfb++) {

                /* Is this scale factor band above count1 for the right channel? */
                if (g_sf_band_indices[sfreq].l[sfb] >= (*g_side_info).count1[gr][1]) {

                    /* Perform the intensity stereo processing */
                    MPG_Stereo_Process_Intensity_Long (gr, sfb, g_frame_header, granuleData);
                }
            }

        } /* end else (only long blocks) */

    } /* end if (intensity_stereo processing) */

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void
MPG_Stereo_Process_Intensity_Long (UINT32 gr, UINT32 sfb, FrameHeader* g_frame_header, GranuleData* granuleData)
{
    static int init = 0;
    static FLOAT32 is_ratios[6];
    UINT32 i;
    UINT32 sfreq;
    UINT32 sfb_start, sfb_stop;
    UINT32 is_pos;
    FLOAT32 is_ratio_l, is_ratio_r;
    FLOAT32 left, right;


    /* First-time init */
    if (init == 0) {
        init = 1;
        for (i = 0; i < 6; i++) {
            is_ratios[i] = tan ((i * C_PI) / 12.0);
        }

    }

    /* Check that ((is_pos[sfb]=scalefac) != 7) => no intensity stereo */
    if ((is_pos = (*granuleData).scalefac_l[0][sfb]) != 7) {

        /* Setup sampling frequency index */
        sfreq = (*g_frame_header).sampling_frequency;

        sfb_start = g_sf_band_indices[sfreq].l[sfb];
        sfb_stop = g_sf_band_indices[sfreq].l[sfb+1];

        /* tan((6*PI)/12 = PI/2) needs special treatment! */
        if (is_pos == 6) {
            is_ratio_l = 1.0;
            is_ratio_r = 0.0;
        } else {
            is_ratio_l = is_ratios[is_pos] / (1.0 + is_ratios[is_pos]);
            is_ratio_r = 1.0 / (1.0 + is_ratios[is_pos]);
        }

        /* Now decode all samples in this scale factor band */
        for (i = sfb_start; i < sfb_stop; i++) {
            left = is_ratio_l * ((*granuleData).is[0][i]);
            right = is_ratio_r * ((*granuleData).is[0][i]);
            (*granuleData).is[0][i] = left;
            (*granuleData).is[1][i] = right;
        }
    }

    /* Done */
    return;

} /* end MPG_Stereo_Process_Intensity_Long() */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void
MPG_Stereo_Process_Intensity_Short (UINT32 gr, UINT32 sfb, FrameHeader* g_frame_header, GranuleData* granuleData)
{
    UINT32 i;
    FLOAT32 left, right;
    UINT32 sfb_start, sfb_stop;
    UINT32 is_pos, is_ratio_l, is_ratio_r, is_ratios[6];
    UINT32 sfreq, win, win_len;


    /* Setup sampling frequency index */
    sfreq = (*g_frame_header).sampling_frequency;

    /* The window length */
    win_len = g_sf_band_indices[sfreq].s[sfb+1] -
              g_sf_band_indices[sfreq].s[sfb];

    /* The three windows within the band has different scalefactors */
    for (win = 0; win < 3; win++) {

        /* Check that ((is_pos[sfb]=scalefac) != 7) => no intensity stereo */
        if ((is_pos = (*granuleData).scalefac_s[0][sfb][win]) != 7) {

            sfb_start = g_sf_band_indices[sfreq].s[sfb]*3 + win_len*win;
            sfb_stop = sfb_start + win_len;

            /* tan((6*PI)/12 = PI/2) needs special treatment! */
            if (is_pos == 6) {
                is_ratio_l = 1.0;
                is_ratio_r = 0.0;
            } else {
                is_ratio_l = is_ratios[is_pos] / (1.0 + is_ratios[is_pos]);
                is_ratio_r = 1.0 / (1.0 + is_ratios[is_pos]);
            }

            /* Now decode all samples in this scale factor band */
            for (i = sfb_start; i < sfb_stop; i++) {
                left = is_ratio_l = (*granuleData).is[0][i];
                right = is_ratio_r = (*granuleData).is[0][i];
                (*granuleData).is[0][i] = left;
                (*granuleData).is[1][i] = right;
            }
        } /* end if (not illegal is_pos) */
    } /* end for (win... */

    /* Done */
    return;

} /* end MPG_Stereo_Process_Intensity_Short() */
