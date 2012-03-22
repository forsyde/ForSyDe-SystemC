/******************************************************************************
*
* Filename: MP3Decoder.h
* Date: 03-05-2010
* Author: mbozkaya 
* Description: This file contains the definitions used by most parts of the 
*              MP3 decoder.
*
******************************************************************************/

#ifndef MP3DECODER_H
#define MP3DECODER_H

/* Include files */
#include <stdio.h>
#include <math.h>

/* Global definitions */
#define SIM_UNIX

#define OK         0
#define ERROR     -1
#define TRUE       1
#define FALSE      0

#define C_MPG_SYNC             0xfff00000

#define C_PI                   3.14159265358979323846
#define C_INV_SQRT_2           0.70710678118654752440

#define Hz                           1
#define kHz                    1000*Hz
#define bit_s                        1
#define kbit_s                 1000*bit_s


/* MP3 decoder data types */

typedef unsigned int  UINT32;
typedef int           INT32;
typedef short int     INT16;
typedef unsigned short int UINT16;
typedef float         FLOAT32;
typedef double        FLOAT64;
typedef int           BOOL;
typedef int           STATUS;
typedef char *        STRING;

/* Types used in the frame header */

/* Layer number */
typedef enum {
  mpeg1_layer_reserved = 0,
  mpeg1_layer_3        = 1,
  mpeg1_layer_2        = 2,
  mpeg1_layer_1        = 3
} t_mpeg1_layer;

/* Modes */
typedef enum {
  mpeg1_mode_stereo = 0,
  mpeg1_mode_joint_stereo,
  mpeg1_mode_dual_channel,
  mpeg1_mode_single_channel
} t_mpeg1_mode;

/* MPEG1 Layer 1-3 frame header */
typedef struct {
  UINT32 id;				      /* 1 bit */
  t_mpeg1_layer layer;			      /* 2 bits */
  UINT32 protection_bit;		      /* 1 bit */
  UINT32 bitrate_index;			      /* 4 bits */
  UINT32 sampling_frequency;		      /* 2 bits */
  UINT32 padding_bit;			      /* 1 bit */
  UINT32 private_bit;			      /* 1 bit */
  t_mpeg1_mode mode;			      /* 2 bits */
  UINT32 mode_extension;		      /* 2 bits */
  UINT32 copyright;			      /* 1 bit */
  UINT32 original_or_copy;		      /* 1 bit */
  UINT32 emphasis;			      /* 2 bits */
} FrameHeader;

/* MPEG1 Layer 3 Side Information */
/* [2][2] means [gr][ch] */
typedef struct {
  UINT32 main_data_begin;                     /* 9 bits */
  UINT32 private_bits;			      /* 3 bits in mono, 5 in stereo */
  UINT32 scfsi[2][4];			      /* 1 bit */
  UINT32 part2_3_length[2][2];		      /* 12 bits */
  UINT32 big_values[2][2];		      /* 9 bits */
  UINT32 global_gain[2][2];		      /* 8 bits */
  UINT32 scalefac_compress[2][2];	      /* 4 bits */
  UINT32 win_switch_flag[2][2];		      /* 1 bit */
  /* if (win_switch_flag[][]) */
  UINT32 block_type[2][2];		      /* 2 bits */
  UINT32 mixed_block_flag[2][2];	      /* 1 bit */
  UINT32 table_select[2][2][3];		      /* 5 bits */
  UINT32 subblock_gain[2][2][3];	      /* 3 bits */
  /* else */
  /* table_select[][][] */
  UINT32 region0_count[2][2];		      /* 4 bits */
  UINT32 region1_count[2][2];		      /* 3 bits */
  /* end */
  UINT32 preflag[2][2];			      /* 1 bit */
  UINT32 scalefac_scale[2][2];		      /* 1 bit */
  UINT32 count1table_select[2][2];	      /* 1 bit */
  UINT32 count1[2][2];		      /* Not in file, calc. by huff.dec.! */
} FrameSideInfo;

/* MPEG1 Layer 3 Main Data */
typedef struct {
  UINT32  scalefac_l[2][2][21];	              /* 0-4 bits */
  UINT32  scalefac_s[2][2][12][3];	      /* 0-4 bits */
  FLOAT32 is[2][2][576];		      /* Huffman coded freq. lines */
} FrameMainData;

typedef struct {
  UINT32  scalefac_l[2][21];	              /* 0-4 bits */
  UINT32  scalefac_s[2][12][3];				  /* 0-4 bits */
  FLOAT32 is[2][576];					      /* Huffman coded freq. lines */
} GranuleData;


typedef struct {
  FLOAT32 is[576];						  /* Huffman coded freq. lines */
} ChanuleData;

inline void copyGranuleData(FrameMainData& frameMainData, int granuleId, GranuleData& outputGranule)
{
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 576; j++)
			outputGranule.is[i][j] = frameMainData.is[granuleId][i][j];

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 21; j++)
			outputGranule.scalefac_l[i][j] = frameMainData.scalefac_l[granuleId][i][j];

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 12; j++)
			for (int k = 0; k < 3; k++)
				outputGranule.scalefac_s[i][j][k] = frameMainData.scalefac_s[granuleId][i][j][k];
}

inline void copyChanuleData(GranuleData& granule, int channelId, ChanuleData& outputChanule)
{
	for (int i = 0; i < 576; i++)
		outputChanule.is[i] = granule.is[channelId][i];
}

inline void copyChanuleData(FrameMainData& frameMainData, int granuleId, int channelId, ChanuleData& outputChanule)
{
	for (int i = 0; i < 576; i++)
		outputChanule.is[i] = frameMainData.is[granuleId][channelId][i];
}

/* Scale factor band indices, for long and short windows */
typedef struct  {
  UINT32 l[23];
  UINT32 s[14];
} BandIndices;

/* Channel sample*/
typedef struct  {
  UINT32 samples[576];
} ChanuleSamples;

//testing added by sprao and ykhambia
/* Scale factor band indices, for long and short windows */

typedef struct  {
  UINT32 l[23];
  UINT32 s[14];
} t_sf_band_indices;

static const t_sf_band_indices g_sf_band_indices[3 /* Sampling freq. */] =
  {
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

typedef struct {
    FLOAT32 v_vec[1024];
} VecType;

static VecType zeroVec = {{0}};

/* MP3 Decoder function definitions */


/// ReadBitstreamAndExtractFrames.c
bool readBitstreamAndExtractFrames(char *file_name, FrameHeader *frameHeader, FrameSideInfo *frameSideInfo, FrameMainData *frameMainData);

///ProcessGranule.c
void processGranule(const UINT32 gr, FrameHeader *frameHeader, FrameSideInfo *frameSideInfo, GranuleData *frameInMainData, GranuleData *frameOutMainData);

///ProcessChanule.c
void processChanule(const INT32 granuleId, const INT32 channelId, ChanuleSamples *channelSample, FrameHeader *frameHeader, FrameSideInfo *frameSideInfo, ChanuleData *frameMainData, FLOAT32 *v_vec);

///Merge.c
void mergeChanules(FrameHeader *frameHeader, ChanuleSamples *channelSample0Left, ChanuleSamples *channelSample0Right, ChanuleSamples *channelSample1Left, ChanuleSamples *channelSample1Right, char *filename);

#endif
