/**********************************************************************
    * ToneDetection.hpp                                               *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          adapted from KisTA: https://github.com/nandohca/kista  *
    *                                                                 *
    * Purpose: The Tone Detection task                                *
    *                                                                 *
    * Usage:   The VAD example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef TONEDETECTION_HPP
#define TONEDETECTION_HPP

#include <forsyde.hpp>
#include "includes/vad.h"

using namespace ForSyDe::SDF;

void ToneDetection_func(std::vector<short>& out, std::vector<rc_t> inp)
{
    // Resize all the vectors to contain 1 element
    out.resize(1);
    
    short* inp1 = inp[0].data();
    short out1;
#pragma ForSyDe begin ToneDetection_func
    tone_detection(inp1, &out1);
#pragma ForSyDe end
    out[0] = out1;
}


#endif
