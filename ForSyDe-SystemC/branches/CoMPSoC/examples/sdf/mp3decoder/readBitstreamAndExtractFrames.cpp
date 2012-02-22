/*
 *
 *  Filename: readBitstreamAndExtractFrames
 *  Created on: 03 May 2010
 *  Author: Sprao
 *  Description: This file contains the implementation of reading a bitstream and extracting the logical frame.
 */

#include <stdlib.h>
#include <stdio.h>
#include "include/MP3Decoder.h"
#include "string.h"
#include "include/MP3_Huffman_Table.h"
#include "include/MP3_Huffman_Table.c"

#ifdef FORSYDE

#define SRC FILE

#else

#include "include/input.h"
#define SRC unsigned char

#endif

/* Function Declaration*/
char file_name[256];
STATUS MPG_Read_Frame (FrameHeader* g_frame_header, FrameSideInfo* g_side_info, FrameMainData* g_main_data, SRC*& fp);
UINT32 MPG_Get_Filepos (SRC*& fp);
STATUS MPG_Read_Header (FrameHeader* g_frame_header, FrameSideInfo* g_side_info, FrameMainData* g_main_data, SRC*& fp);
UINT32 MPG_Get_Byte (SRC*& fp);
void MPG_Decode_L3_Init_Song ();
STATUS MPG_Read_CRC (SRC*& fp);
STATUS MPG_Read_Audio_L3 (FrameHeader* g_frame_header, FrameSideInfo* g_side_info, FrameMainData* g_main_data, SRC*& fp);
static void MPG_Get_Sideinfo (UINT32 sideinfo_size, SRC*& fp);
STATUS MPG_Get_Bytes (UINT32 no_of_bytes, UINT32 data_vec[], SRC*& fp);
UINT32 MPG_Get_Side_Bits (UINT32 number_of_bits);
STATUS MPG_Read_Main_L3 (FrameHeader* g_frame_header, FrameSideInfo* g_side_info, FrameMainData* g_main_data, SRC*& fp);
UINT32 MPG_Get_Main_Bits (UINT32 number_of_bits);
static STATUS MPG_Get_Main_Data (UINT32 main_data_size, UINT32 main_data_begin, SRC*& fp);
UINT32 MPG_Get_Main_Pos ();
static STATUS MPG_Huffman_Decode (UINT32 table_num, INT32 *x, INT32 *y, INT32 *v, INT32 *w);
void MPG_Read_Huffman (UINT32 part_2_start, UINT32 gr, UINT32 ch, FrameHeader* g_frame_header, FrameSideInfo* g_side_info, FrameMainData* g_main_data);
UINT32 MPG_Get_Main_Bit ();
STATUS MPG_Set_Main_Pos (UINT32 bit_pos);

/* Macro from MP3_Bitstream.h*/
#ifndef _MPG_BITSTREAM_H_
#define C_MPG_EOF           0xffffffff
#endif

#define DBG(str, args...) { printf (str, ## args); printf ("\n"); }
#define ERR(str, args...) { fprintf (stderr, str, ## args) ; fprintf (stderr, "\n"); }
#define EXIT(str, args...) { printf (str, ## args);  printf ("\n"); exit (0); }

/* Local Variables */
UINT32  g_main_data_vec[2*1024]; /* Large static data */
UINT32 *g_main_data_ptr;	/* Pointer into the reservoir */
UINT32  g_main_data_idx;	/* Index into the current byte (0-7) */
UINT32  g_main_data_top = 0;/* Number of bytes in reservoir (0-1024) */

static SRC *fp = (SRC *) NULL;

static UINT32 hsynth_init = 1;
static UINT32 synth_init = 1;

UINT32 g_mpeg1_bitrates[3 /* layer 1-3 */][15 /* header bitrate_index */] = {
  {   /* Layer 1 */
         0,  32000,  64000,  96000, 128000, 160000, 192000, 224000,
    256000, 288000, 320000, 352000, 384000, 416000, 448000
  },

  {  /* Layer 2 */
         0,  32000,  48000,  56000,  64000,  80000,  96000, 112000,
    128000, 160000, 192000, 224000, 256000, 320000, 384000
  },

  {   /* Layer 3 */
         0,  32000,  40000,  48000,  56000,  64000,  80000,  96000,
    112000, 128000, 160000, 192000, 224000, 256000, 320000
  }
};

UINT32 g_sampling_frequency[3] = {
  44100 * Hz,
  48000 * Hz,
  32000 * Hz
};

static UINT32 mpeg1_scalefac_sizes[16][2 /* slen1, slen2 */] = {
  { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 },
  { 3, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 },
  { 2, 1 }, { 2, 2 }, { 2, 3 }, { 3, 1 },
  { 3, 2 }, { 3, 3 }, { 4, 2 }, { 4, 3 }
};


/* Bit reservoir for side info */
static UINT32  side_info_vec[32+4];
static UINT32 *side_info_ptr;	/* Pointer into the reservoir */
static UINT32  side_info_idx;	/* Index into the current byte (0-7) */

SRC *pfile;


bool readBitstreamAndExtractFrames(char *file_name, FrameHeader *frameHeader, FrameSideInfo *frameSideInfo, FrameMainData *frameMainData){

  /* File open? */
  if (fp == (SRC *) NULL) {
  #ifdef FORSYDE
    if ((fp = fopen (file_name, "r")) == (SRC *) NULL) {
      printf ("Cannot open mp3 file \"%s\"\n", file_name);
      exit (0);
    }
  #else
    fp = instream;
  #endif
  }

    if (MPG_Get_Filepos (fp) != C_MPG_EOF) {
     MPG_Read_Frame(frameHeader, frameSideInfo, frameMainData, fp);
    }
    else{
      //fclose(fw);
      //exit (0);
      return false;
    }


    return true;
}

STATUS MPG_Read_Frame (FrameHeader* g_frame_header, FrameSideInfo* g_side_info, FrameMainData* g_main_data, SRC*& fp)
{
    UINT32 first = 0;


    if (MPG_Get_Filepos (fp) == 0) {
        first = 1;
        g_main_data_top = 0;
    }

    /* Try to find the next frame in the bitstream and decode it */
    if (MPG_Read_Header (g_frame_header, g_side_info, g_main_data, fp) != OK) {
        return (ERROR);
    }

    if (first) {
	    g_main_data_top = 0;		/* Clear bit reservoir */
        MPG_Decode_L3_Init_Song ();
    }

    /* Get CRC word if present */
    if ((*g_frame_header).protection_bit == 0) { /* CRC present */
        if (MPG_Read_CRC (fp) != OK) {
            return (ERROR);
        }
    }

    /* Get audio data */
    if ((*g_frame_header).layer == 3) {

        /* Get side info */
        MPG_Read_Audio_L3 (g_frame_header, g_side_info, g_main_data, fp);


        /* If there's not enough main data in the bit reservoir, signal to calling function so that decoding isn't done!*/

        /* Get main data (scalefactors and Huffman coded frequency data) */
        if (MPG_Read_Main_L3 (g_frame_header, g_side_info, g_main_data, fp) != OK) {
            return (ERROR);
        }

    } else {
        ERR ("Only layer 3 (!= %d) is supported!\n", (*g_frame_header).layer);
        return (ERROR);
    }

   return (OK);

}

UINT32 MPG_Get_Filepos (SRC*& fp)
{

    /* File open? */
    if (fp == (SRC *) NULL) {
        return (0);
    }
#ifdef FORSYDE
    if (feof (fp)) {
        return (C_MPG_EOF);
    } else {
        return ((UINT32) ftell (fp));
    }
#else
    if (fp-instream >= instream_size) {
        return (C_MPG_EOF);
    } else {
        return ((UINT32) (fp-instream));
    }
#endif
}

STATUS MPG_Read_Header (FrameHeader* g_frame_header, FrameSideInfo* g_side_info, FrameMainData* g_main_data, SRC*& fp)
{
    UINT32 b1, b2, b3, b4, header;


    /* Get the next four bytes from the bitstream */
    b1 = MPG_Get_Byte (fp);
    b2 = MPG_Get_Byte (fp);
    b3 = MPG_Get_Byte (fp);
    b4 = MPG_Get_Byte (fp);

    /* If we got an End Of File condition we're done */
    if ((b1 == C_MPG_EOF) || (b2 == C_MPG_EOF) || (b3 == C_MPG_EOF) || (b4 == C_MPG_EOF)) {
        return (ERROR);
    }

    header = (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4 << 0);

    /* Are the high 12 bits the syncword (0xfff)? */
    if ((header & 0xfff00000) != C_MPG_SYNC) {

        /* No, so scan the bitstream one byte at a time until we find it or EOF */
        while (1) {

            /* Shift the values one byte to the left */
            b1 = b2;
            b2 = b3;
            b3 = b4;

            /* Get one new byte from the bitstream */
            b4 = MPG_Get_Byte (fp);

            /* If we got an End Of File condition we're done */
            if (b4 == C_MPG_EOF) {
                return (ERROR);
            }

            /* Make up the new header */
            header = (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4 << 0);

            /* If it's the syncword (0xfff00000) we're done */
            if ((header & 0xfff00000) == C_MPG_SYNC) {
                break;			/* Exit while(1) loop */
            }

        } /* while (1) */

    } /* if... */

    /* If we get here we've found the sync word, and can decode the header which is in the low 20 bits of the 32-bit sync+header word.*/

    /* Decode the header */
    (*g_frame_header).id                = (header & 0x00080000) >> 19;
    (*g_frame_header).layer              = (t_mpeg1_layer)((header & 0x00060000) >> 17);
    (*g_frame_header).protection_bit     = (header & 0x00010000) >> 16;

    (*g_frame_header).bitrate_index      = (header & 0x0000f000) >> 12;

    (*g_frame_header).sampling_frequency = (header & 0x00000c00) >> 10;
    (*g_frame_header).padding_bit        = (header & 0x00000200) >> 9;
    (*g_frame_header).private_bit        = (header & 0x00000100) >> 8;

    (*g_frame_header).mode               = (t_mpeg1_mode)((header & 0x000000c0) >> 6);
    (*g_frame_header).mode_extension     = (header & 0x00000030) >> 4;

    (*g_frame_header).copyright          = (header & 0x00000008) >> 3;
    (*g_frame_header).original_or_copy   = (header & 0x00000004) >> 2;
    (*g_frame_header).emphasis           = (header & 0x00000003) >> 0;

    /* Check for invalid values and impossible combinations */
    if ((*g_frame_header).id != 1) {
        ERR ("ID must be 1\n");
        ERR ("Header word is 0x%08x at file pos %d\n", header,MPG_Get_Filepos (fp));
        return (ERROR);
    }

    if ((*g_frame_header).bitrate_index == 0) {
        ERR ("Free bitrate format NIY!\n");
        ERR ("Header word is 0x%08x at file pos %d\n", header,MPG_Get_Filepos (fp));
        exit (1);
    }

    if ((*g_frame_header).bitrate_index == 15) {
        ERR ("bitrate_index = 15 is invalid!\n");
        ERR ("Header word is 0x%08x at file pos %d\n", header,MPG_Get_Filepos (fp));
        return (ERROR);
    }

    if ((*g_frame_header).sampling_frequency == 3) {
        ERR ("sampling_frequency = 3 is invalid!\n");
        ERR ("Header word is 0x%08x at file pos %d\n", header,MPG_Get_Filepos (fp));
        return (ERROR);
    }

    if ((*g_frame_header).layer == 0) {
        ERR ("layer = 0 is invalid!\n");
        ERR ("Header word is 0x%08x at file pos %d\n", header,MPG_Get_Filepos (fp));
        return (ERROR);
    }
    (*g_frame_header).layer = (t_mpeg1_layer)(4 - (*g_frame_header).layer);

//    DBG ("Header         =   0x%08x\n", header);

    /* Done */

  	return (OK);

}

void MPG_Decode_L3_Init_Song ()
{
    hsynth_init = 1;
    synth_init = 1;
/*    g_main_data_top = 0;*/		/* Clear bit reservoir */
}


STATUS MPG_Read_CRC (SRC*& fp)
{
    UINT32 b1, b2;

    /* Get the next two bytes from the bitstream */
    b1 = MPG_Get_Byte (fp);
    b2 = MPG_Get_Byte (fp);

    /* If we got an End Of File condition we're done */
    if ((b1 == C_MPG_EOF) || (b2 == C_MPG_EOF)) {
        return (FALSE);
    }

    /* Done */
    return (OK);

}


UINT32 MPG_Get_Byte (SRC*& fp)
{
	 UINT32 val;

#ifdef FORSYDE

	    /* Get byte */
	    val = fgetc (fp) & 0xff;

	    /* EOF? */
	    if (feof (fp)) {
	        val = C_MPG_EOF;
	    }
#else
        /* Get byte */
	    val = (*(fp++)) & 0xff;

	    /* EOF? */
	    if (fp-instream >= instream_size) {
	        val = C_MPG_EOF;
	    }
#endif
	    /* Done */
	    return (val);

}

STATUS MPG_Read_Audio_L3 (FrameHeader* g_frame_header, FrameSideInfo* g_side_info, FrameMainData* g_main_data, SRC*& fp)
{
	UINT32 framesize, sideinfo_size, main_data_size;
    UINT32 nch, ch, gr, scfsi_band, region, window;


    /* Number of channels (1 for mono and 2 for stereo) */
    nch = ((*g_frame_header).mode == mpeg1_mode_single_channel ? 1 : 2);

    /* Calculate header audio data size */
    framesize = (144 *g_mpeg1_bitrates[(*g_frame_header).layer-1][(*g_frame_header).bitrate_index]) / g_sampling_frequency[(*g_frame_header).sampling_frequency] +
        (*g_frame_header).padding_bit;


    if (framesize > 2000) {
        ERR ("framesize = %d\n", framesize);
        return (ERROR);
    }

    /* Sideinfo is 17 bytes for one channel and 32 bytes for two */
    sideinfo_size = (nch == 1 ? 17 : 32);

    /* Main data size is the rest of the frame, including ancillary data */
    main_data_size = framesize - sideinfo_size - 4 /* sync+header */;

    /* CRC is 2 bytes */
    if ((*g_frame_header).protection_bit == 0) main_data_size -= 2;

//     DBG ("framesize      =   %d\n", framesize);
//     DBG ("sideinfo_size  =   %d\n", sideinfo_size);
//     DBG ("main_data_size =   %d\n", main_data_size);

    /* Read the sideinfo from the bitstream into a local buffer used by the  MPG_Get_Side_Bits function.*/
    MPG_Get_Sideinfo (sideinfo_size, fp);

    if (MPG_Get_Filepos (fp) == C_MPG_EOF) return (ERROR);

    /* Parse audio data */

    /* Pointer to where we should start reading main data */
    (*g_side_info).main_data_begin = MPG_Get_Side_Bits (9);

    /* Get private bits. Not used for anything. */
    if ((*g_frame_header).mode == mpeg1_mode_single_channel)
        (*g_side_info).private_bits = MPG_Get_Side_Bits (5);
    else
        (*g_side_info).private_bits = MPG_Get_Side_Bits (3);

    /* Get scale factor selection information */
    for (ch = 0; ch < nch; ch++) {
        for (scfsi_band = 0; scfsi_band < 4; scfsi_band++) {
            (*g_side_info).scfsi[ch][scfsi_band] = MPG_Get_Side_Bits (1);
        }
    }

    /* Get the rest of the side information */
    for (gr = 0; gr < 2; gr++) {
        for (ch = 0; ch < nch; ch++) {
            (*g_side_info).part2_3_length[gr][ch]    = MPG_Get_Side_Bits (12);
            (*g_side_info).big_values[gr][ch]        = MPG_Get_Side_Bits (9);
            (*g_side_info).global_gain[gr][ch]       = MPG_Get_Side_Bits (8);
            (*g_side_info).scalefac_compress[gr][ch] = MPG_Get_Side_Bits (4);

            (*g_side_info).win_switch_flag[gr][ch]   = MPG_Get_Side_Bits (1);

            if ((*g_side_info).win_switch_flag[gr][ch] == 1) {
                (*g_side_info).block_type[gr][ch]       = MPG_Get_Side_Bits (2);
                (*g_side_info).mixed_block_flag[gr][ch] = MPG_Get_Side_Bits (1);
                for (region = 0; region < 2; region++) {
                    (*g_side_info).table_select[gr][ch][region] = MPG_Get_Side_Bits (5);
                }
                for (window = 0; window < 3; window++) {
                    (*g_side_info).subblock_gain[gr][ch][window] = MPG_Get_Side_Bits (3);
                }
                if (((*g_side_info).block_type[gr][ch] == 2) &&
                        ((*g_side_info).mixed_block_flag[gr][ch] == 0)) {
                    (*g_side_info).region0_count[gr][ch] = 8; /* Implicit */
                } else {
                    (*g_side_info).region0_count[gr][ch] = 7; /* Implicit */
                }
                /* The standard is wrong on this!!! */
                (*g_side_info).region1_count[gr][ch] =
                    20 - (*g_side_info).region0_count[gr][ch];	/* Implicit */
            } else {
                for (region = 0; region < 3; region++) {
                    (*g_side_info).table_select[gr][ch][region] = MPG_Get_Side_Bits (5);
                }
                (*g_side_info).region0_count[gr][ch] = MPG_Get_Side_Bits (4);
                (*g_side_info).region1_count[gr][ch] = MPG_Get_Side_Bits (3);
                (*g_side_info).block_type[gr][ch] = 0;	/* Implicit */
            }	/* end if ... */

            (*g_side_info).preflag[gr][ch]            = MPG_Get_Side_Bits (1);
            (*g_side_info).scalefac_scale[gr][ch]     = MPG_Get_Side_Bits (1);
            (*g_side_info).count1table_select[gr][ch] = MPG_Get_Side_Bits (1);

        } /* end for (channel... */
    } /* end for (granule... */

    /* Done */
    return (OK);

}

UINT32 MPG_Get_Side_Bits (UINT32 number_of_bits)
{
    UINT32 tmp;


    /* Form a word of the next four bytes */
    tmp = (side_info_ptr[0] << 24) | (side_info_ptr[1] << 16) |
          (side_info_ptr[2] <<  8) | (side_info_ptr[3] <<  0);

    /* Remove bits already used */
    tmp = tmp << side_info_idx;

    /* Remove bits after the desired bits */
    tmp = tmp >> (32 - number_of_bits);

    /* Update pointers */
    side_info_ptr += (side_info_idx + number_of_bits) >> 3;
    side_info_idx = (side_info_idx + number_of_bits) & 0x07;

    return (tmp);
}

static void MPG_Get_Sideinfo (UINT32 sideinfo_size,SRC*& fp)
{
    if (MPG_Get_Bytes (sideinfo_size, side_info_vec, fp) != OK) {
        ERR ("\nCouldn't read sideinfo %d bytes at pos %d\n", sideinfo_size, MPG_Get_Filepos (fp));
        return;
    }

    side_info_ptr = &(side_info_vec[0]);
    side_info_idx = 0;

}

STATUS MPG_Get_Bytes (UINT32 no_of_bytes, UINT32 data_vec[], SRC*& fp)
{
    int i;
    UINT32 val;


    for (i = 0; i < no_of_bytes; i++) {
        val = MPG_Get_Byte (fp);

        if (val == C_MPG_EOF) {
            return (C_MPG_EOF);
        } else {
            data_vec[i] = val;
        }
    }

    return (OK);

}

STATUS MPG_Read_Main_L3 (FrameHeader* g_frame_header, FrameSideInfo* g_side_info, FrameMainData* g_main_data, SRC*& fp)
{

    UINT32 framesize, sideinfo_size, main_data_size;
    UINT32 gr, ch, nch, sfb, win, slen1, slen2, nbits, part_2_start;


    /* Number of channels (1 for mono and 2 for stereo) */
    nch = ((*g_frame_header).mode == mpeg1_mode_single_channel ? 1 : 2);

    /* Calculate header audio data size */
    framesize =
        (144 *
         g_mpeg1_bitrates[(*g_frame_header).layer-1][(*g_frame_header).bitrate_index]) / g_sampling_frequency[(*g_frame_header).sampling_frequency] + (*g_frame_header).padding_bit;

    if (framesize > 2000) {
        ERR ("framesize = %d\n", framesize);
        return (ERROR);
    }

    /* Sideinfo is 17 bytes for one channel and 32 bytes for two */
    sideinfo_size = (nch == 1 ? 17 : 32);

    /* Main data size is the rest of the frame, including ancillary data */
    main_data_size = framesize - sideinfo_size - 4 /* sync+header */;

    /* CRC is 2 bytes */
    if ((*g_frame_header).protection_bit == 0) main_data_size -= 2;

    /* Assemble main data buffer with data from this frame and the previous
     * two frames. main_data_begin indicates how many bytes from previous
     * frames that should be used. This buffer is later accessed by the
     * MPG_Get_Main_Bits function in the same way as the side info is.
     */
    if (MPG_Get_Main_Data (main_data_size, (*g_side_info).main_data_begin, fp) != OK) {
        return (ERROR); /* This could be due to not enough data in reservoir */
    }

    for (gr = 0; gr < 2; gr++) {
        for (ch = 0; ch < nch; ch++) {

            part_2_start = MPG_Get_Main_Pos ();

            /* Number of bits in the bitstream for the bands */
            slen1 = mpeg1_scalefac_sizes[(*g_side_info).scalefac_compress[gr][ch]][0];
            slen2 = mpeg1_scalefac_sizes[(*g_side_info).scalefac_compress[gr][ch]][1];

            if (((*g_side_info).win_switch_flag[gr][ch] != 0) &&
                    ((*g_side_info).block_type[gr][ch] == 2)) {
                if ((*g_side_info).mixed_block_flag[gr][ch] != 0) {
                    for (sfb = 0; sfb < 8; sfb++) {
                        (*g_main_data).scalefac_l[gr][ch][sfb] = MPG_Get_Main_Bits (slen1);
                    }
                    for (sfb = 3; sfb < 12; sfb++) {
                        if (sfb < 6) {	/* slen1 is for bands 3-5, slen2 for 6-11 */
                            nbits = slen1;
                        } else {
                            nbits = slen2;
                        }

                        for (win = 0; win < 3; win++) {
                            (*g_main_data).scalefac_s[gr][ch][sfb][win] =
                                MPG_Get_Main_Bits (nbits);
                        }
                    }
                } else {
                    for (sfb = 0; sfb < 12; sfb++) {
                        if (sfb < 6) {	/* slen1 is for bands 3-5, slen2 for 6-11 */
                            nbits = slen1;
                        } else {
                            nbits = slen2;
                        }

                        for (win = 0; win < 3; win++) {
                            (*g_main_data).scalefac_s[gr][ch][sfb][win] =
                                MPG_Get_Main_Bits (nbits);
                        }
                    }
                }
            } else { /* block_type == 0 if winswitch == 0 */

                /* Scale factor bands 0-5 */
                if (((*g_side_info).scfsi[ch][0] == 0) || (gr == 0)) {
                    for (sfb = 0; sfb < 6; sfb++) {
                        (*g_main_data).scalefac_l[gr][ch][sfb] = MPG_Get_Main_Bits (slen1);
                    }
                } else if (((*g_side_info).scfsi[ch][0] == 1) && (gr == 1)) {
                    /* Copy scalefactors from granule 0 to granule 1 */
                    for (sfb = 0; sfb < 6; sfb++) {
                        (*g_main_data).scalefac_l[1][ch][sfb] =
                            (*g_main_data).scalefac_l[0][ch][sfb];
                    }
                }

                /* Scale factor bands 6-10 */
                if (((*g_side_info).scfsi[ch][1] == 0) || (gr == 0)) {
                    for (sfb = 6; sfb < 11; sfb++) {
                        (*g_main_data).scalefac_l[gr][ch][sfb] = MPG_Get_Main_Bits (slen1);
                    }
                } else if (((*g_side_info).scfsi[ch][1] == 1) && (gr == 1)) {
                    /* Copy scalefactors from granule 0 to granule 1 */
                    for (sfb = 6; sfb < 11; sfb++) {
                        (*g_main_data).scalefac_l[1][ch][sfb] =
                            (*g_main_data).scalefac_l[0][ch][sfb];
                    }
                }

                /* Scale factor bands 11-15 */
                if (((*g_side_info).scfsi[ch][2] == 0) || (gr == 0)) {
                    for (sfb = 11; sfb < 16; sfb++) {
                        (*g_main_data).scalefac_l[gr][ch][sfb] = MPG_Get_Main_Bits (slen2);
                    }
                } else if (((*g_side_info).scfsi[ch][2] == 1) && (gr == 1)) {
                    /* Copy scalefactors from granule 0 to granule 1 */
                    for (sfb = 11; sfb < 16; sfb++) {
                        (*g_main_data).scalefac_l[1][ch][sfb] = (*g_main_data).scalefac_l[0][ch][sfb];
                    }
                }

                /* Scale factor bands 16-20 */
                if (((*g_side_info).scfsi[ch][3] == 0) || (gr == 0)) {
                    for (sfb = 16; sfb < 21; sfb++) {
                        (*g_main_data).scalefac_l[gr][ch][sfb] = MPG_Get_Main_Bits (slen2);
                    }
                } else if (((*g_side_info).scfsi[ch][3] == 1) && (gr == 1)) {
                    /* Copy scalefactors from granule 0 to granule 1 */
                    for (sfb = 16; sfb < 21; sfb++) {
                        (*g_main_data).scalefac_l[1][ch][sfb] =
                            (*g_main_data).scalefac_l[0][ch][sfb];
                    }
                }
            }

            /* Read Huffman coded data. Skip stuffing bits. */
            MPG_Read_Huffman (part_2_start, gr, ch, g_frame_header, g_side_info, g_main_data);

        } /* end for (gr... */
    } /* end for (ch... */

    /* The ancillary data is stored here, but we ignore it. */

    return (OK);

}

static STATUS MPG_Get_Main_Data (UINT32 main_data_size, UINT32 main_data_begin, SRC*& fp)
{
    int i, start_pos;


    if (main_data_size > 1500)
        ERR ("main_data_size = %d\n", main_data_size);

    /* Check that there's data available from previous frames if needed */
    if (main_data_begin > g_main_data_top) {

        /* No, there is not, so we skip decoding this frame, but we have to read the main_data bits from the bitstream in case they are needed for decoding the next frame.*/
        (void) MPG_Get_Bytes (main_data_size, &(g_main_data_vec[g_main_data_top]), fp);

        /* Set up pointers */
        g_main_data_ptr = &(g_main_data_vec[0]);
        g_main_data_idx = 0;
        g_main_data_top += main_data_size;

        return (ERROR);		/* This frame cannot be decoded! */

    }

    /* Copy data from previous frames */
    for (i = 0; i < main_data_begin; i++) {
        g_main_data_vec[i] = g_main_data_vec[g_main_data_top - main_data_begin + i];
    }

    start_pos = MPG_Get_Filepos (fp);

    /* Read the main_data from file */
    (void) MPG_Get_Bytes (main_data_size, &(g_main_data_vec[main_data_begin]), fp);

    /* Set up pointers */
    g_main_data_ptr = &(g_main_data_vec[0]);
    g_main_data_idx = 0;
    g_main_data_top = main_data_begin + main_data_size;

    return (OK);

}


UINT32 MPG_Get_Main_Bits (UINT32 number_of_bits)
{
    UINT32 tmp;


    if (number_of_bits == 0) return (0);

    /* Form a word of the next four bytes */
    tmp = (g_main_data_ptr[0] << 24) | (g_main_data_ptr[1] << 16) |
          (g_main_data_ptr[2] <<  8) | (g_main_data_ptr[3] <<  0);

    /* Remove bits already used */
    tmp = tmp << g_main_data_idx;

    /* Remove bits after the desired bits */
    tmp = tmp >> (32 - number_of_bits);

    /* Update pointers */
    g_main_data_ptr += (g_main_data_idx + number_of_bits) >> 3;
    g_main_data_idx = (g_main_data_idx + number_of_bits) & 0x07;

    return (tmp);

}

UINT32 MPG_Get_Main_Pos ()
{
    UINT32 pos;


    pos = ((UINT32) g_main_data_ptr) - ((UINT32) &(g_main_data_vec[0]));

    pos /= 4; /* Divide by four to get number of bytes */
    pos = pos << 3;		/* Multiply by 8 to get number of bits */
    pos = pos + g_main_data_idx;	/* Add current bit index */

    return (pos);

}

void MPG_Read_Huffman (UINT32 part_2_start, UINT32 gr, UINT32 ch, FrameHeader* g_frame_header, FrameSideInfo* g_side_info, FrameMainData* g_main_data)
{
    INT32 x, y, v, w;
    UINT32 table_num, is_pos, bit_pos_end, sfreq;
    UINT32 region_1_start, region_2_start; /* region_0_start = 0 */


    /* Check that there is any data to decode. If not, zero the array. */
    if ((*g_side_info).part2_3_length[gr][ch] == 0) {

        for (is_pos = 0; is_pos < 576; is_pos++) {
            (*g_main_data).is[gr][ch][is_pos] = 0.0;
        }

        return;
    }

    /* Calculate bit_pos_end which is the index of the last bit for this part. */
    bit_pos_end = part_2_start + (*g_side_info).part2_3_length[gr][ch] - 1;

    /* Determine region boundaries */
    if (((*g_side_info).win_switch_flag[gr][ch] == 1) &&
            ((*g_side_info).block_type[gr][ch] == 2)) {

        region_1_start = 36;  /* sfb[9/3]*3=36 */
        region_2_start = 576; /* No Region2 for short block case. */

    } else {
        sfreq = (*g_frame_header).sampling_frequency;
        region_1_start = g_sf_band_indices[sfreq].l[(*g_side_info).region0_count[gr][ch] + 1];
        region_2_start = g_sf_band_indices[sfreq].l[(*g_side_info).region0_count[gr][ch] + (*g_side_info).region1_count[gr][ch] + 2];
    }

    /* Read big_values using tables according to region_x_start */
    for (is_pos = 0; is_pos < (*g_side_info).big_values[gr][ch] * 2; is_pos++) {

        if (is_pos < region_1_start) {
            table_num = (*g_side_info).table_select[gr][ch][0];
        } else if (is_pos < region_2_start) {
            table_num = (*g_side_info).table_select[gr][ch][1];
        } else {
            table_num = (*g_side_info).table_select[gr][ch][2];
        }

        /* Get next Huffman coded words */
        (void) MPG_Huffman_Decode (table_num, &x, &y, &v, &w);

        /* In the big_values area there are two freq lines per Huffman word */
        (*g_main_data).is[gr][ch][is_pos++] = x;
        (*g_main_data).is[gr][ch][is_pos] = y;

    }

    /* Read small values until is_pos = 576 or we run out of huffman data */
    table_num = (*g_side_info).count1table_select[gr][ch] + 32;
    for (is_pos = (*g_side_info).big_values[gr][ch] * 2;
            (is_pos <= 572) && (MPG_Get_Main_Pos () <= bit_pos_end); is_pos++) {

        /* Get next Huffman coded words */
        (void) MPG_Huffman_Decode (table_num, &x, &y, &v, &w);

        (*g_main_data).is[gr][ch][is_pos++] = v;
        if (is_pos >= 576) break;

        (*g_main_data).is[gr][ch][is_pos++] = w;
        if (is_pos >= 576) break;

        (*g_main_data).is[gr][ch][is_pos++] = x;
        if (is_pos >= 576) break;

        (*g_main_data).is[gr][ch][is_pos] = y;
    }

    /* Check that we didn't read past the end of this section */
    if (MPG_Get_Main_Pos () > (bit_pos_end+1)) {
        /* Remove last words read */
        is_pos -= 4;
    }

    /* Setup count1 which is the index of the first sample in the rzero reg. */
    (*g_side_info).count1[gr][ch] = is_pos;

    /* Zero out the last part if necessary */
    for (/* is_pos comes from last for-loop */; is_pos < 576; is_pos++) {
        (*g_main_data).is[gr][ch][is_pos] = 0.0;
    }

    /* Set the bitpos to point to the next part to read */
    (void) MPG_Set_Main_Pos (bit_pos_end+1);

    /* Done */
    return;

}

//~ void MPG_Set_Filepos (UINT32 position, SRC* fp)
//~ {
//~ 
    //~ /* File open? */
    //~ if (fp == (SRC *) NULL) {
        //~ return;
    //~ }
//~ 
    //~ fseek (fp, (long) position, SEEK_SET);
//~ 
//~ }

static STATUS MPG_Huffman_Decode (UINT32 table_num, INT32 *x, INT32 *y, INT32 *v, INT32 *w)
{
    UINT32 point, error, bitsleft, treelen, linbits;
    UINT32 *htptr;


    point = 0;
    error = 1;
    bitsleft = 32;

    /* Check for empty tables */
    if (g_huffman_main[table_num][1 /* treelen */] == 0) {
        *x = *y = *v = *w = 0;
        return (OK);
    }

    treelen = g_huffman_main[table_num][1 /* treelen */];
    linbits = g_huffman_main[table_num][2 /* linbits */];
    htptr = (UINT32 *) g_huffman_main[table_num][0];


    /* Start reading the Huffman code word, bit by bit */
    do {
        /* Check if we've matched a code word */
        if ((htptr[point] & 0xffff0000) == 0x00000000) {
            error = 0;
            *x = (htptr[point] >> 4) & 0xf;
            *y = htptr[point] & 0xf;
            break;
        }

        if (MPG_Get_Main_Bit ()) { /* Go right in tree */
            while ((htptr[point] & 0xff) >= 250) {
                point += htptr[point] & 0xff;
            }
            point += htptr[point] & 0xff;
        } else { /* Go left in tree */
            while ((htptr[point] >> 16) >= 250) {
                point += htptr[point] >> 16;
            }
            point += htptr[point] >> 16;
        }

    } while ((--bitsleft > 0) && (point < treelen));

    /* Check for error. */
    if (error) {
        ERR ("Illegal Huff code in data. bleft = %d, point = %d. tab = %d.",bitsleft, point, table_num);
        *x = *y = 0;
    }

    /* Process sign encodings for quadruples tables. */
    if (table_num > 31) {
        *v = (*y >> 3) & 1;
        *w = (*y >> 2) & 1;
        *x = (*y >> 1) & 1;
        *y = *y & 1;

        if (*v > 0)
            if (MPG_Get_Main_Bit () == 1) *v = -*v;
        if (*w > 0)
            if (MPG_Get_Main_Bit () == 1) *w = -*w;
        if (*x > 0)
            if (MPG_Get_Main_Bit () == 1) *x = -*x;
        if (*y > 0)
            if (MPG_Get_Main_Bit () == 1) *y = -*y;

    } else {
        /* Get linbits */
        if ((linbits > 0) && (*x == 15)) {
            *x += MPG_Get_Main_Bits (linbits);
        }

        /* Get sign bit */
        if (*x > 0) {
            if (MPG_Get_Main_Bit () == 1) *x = -*x;
        }

        /* Get linbits */
        if ((linbits > 0) && (*y == 15)) {
            *y += MPG_Get_Main_Bits (linbits);
        }

        /* Get sign bit */
        if (*y > 0) {
            if (MPG_Get_Main_Bit () == 1) *y = -*y;
        }

    }

    /* Done */
    return (error ? ERROR : OK);

}

UINT32 MPG_Get_Main_Bit ()
{
    UINT32 tmp;


    tmp = g_main_data_ptr[0] >> (7 - g_main_data_idx);
    tmp &= 0x01;

    g_main_data_ptr += (g_main_data_idx + 1) >> 3;
    g_main_data_idx = (g_main_data_idx + 1) & 0x07;

    /* Done */
    return (tmp);

}

STATUS MPG_Set_Main_Pos (UINT32 bit_pos)
{

    g_main_data_ptr = &(g_main_data_vec[bit_pos >> 3]);
    g_main_data_idx = bit_pos & 0x7;

    return (OK);

}


