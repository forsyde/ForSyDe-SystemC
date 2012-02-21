#include "include/MP3Decoder.h"

#include <cstdlib>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void audio_write_raw (FrameHeader* frameHeader, UINT32 *samples, UINT32 nsamples, char* filename);

void mergeChanuleSampleData(ChanuleSamples& source, ChanuleSamples& destination)
{
	int i;
	for (i=0; i<576; i++)
	{
		/*if (source.samples[i] != 0)
			printf("FOUND A NON-ZERO SAMPLE, GOOD!");*/
		destination.samples[i] |= source.samples[i];
	}
}

void mergeChanules(FrameHeader* g_frame_header, ChanuleSamples *channelSample0Left, ChanuleSamples *channelSample0Right, ChanuleSamples *channelSample1Left, ChanuleSamples *channelSample1Right, char *filename)
{
	UINT32 i;

	// Initialize output to zero
	ChanuleSamples out;
	ChanuleSamples out2;
	for (i=0; i<576; i++)
	{
		out.samples[i] = (UINT32)0;
		out2.samples[i] = (UINT32)0;
	}

	// Merge the samples
	mergeChanuleSampleData(*channelSample0Left, out);
	mergeChanuleSampleData(*channelSample0Right, out);
	mergeChanuleSampleData(*channelSample1Left, out2);
	mergeChanuleSampleData(*channelSample1Right, out2);

	// Write out the raw file
	audio_write_raw(g_frame_header, out.samples, 576, filename);
	audio_write_raw(g_frame_header, out2.samples, 576, filename);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void audio_write_raw (FrameHeader* g_frame_header, UINT32 *samples, UINT32 nsamples, char* filename)
{
    static int init = 0, fd;
    char fname[1024];
    UINT32 lo, hi;
    int i, nch;
    unsigned short s[576*2];

    if (init == 0) {
        init = 1;

        sprintf (fname, "%s.raw", filename);

        fd = open (fname, O_WRONLY | O_CREAT, 0666);

        if (fd == -1) {
            perror (fname);
            exit (-1);
        }
    }

    nch = ((*g_frame_header).mode == mpeg1_mode_single_channel ? 1 : 2);

    for (i = 0; i < nsamples; i++) {
        if (nch == 1) {
            lo = samples[i] & 0xffff;
            s[i] = lo;
        } else {
            lo = samples[i] & 0xffff;
            hi = (samples[i] & 0xffff0000) >> 16;
            s[2*i] = hi;
            s[2*i+1] = lo;
        }

    }

    if (write (fd, (char *) s, nsamples * 2 * nch) != nsamples * 2 * nch) {
        fprintf (stderr, "Unable to write raw data\n");
        perror (fname);
        exit (-1);
    }

    return;

} /* audio_write_raw() */
