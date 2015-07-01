/**********************************************************************
    * VADdecision.hpp                                                 *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          adapted from KisTA: https://github.com/nandohca/kista  *
    *                                                                 *
    * Purpose: The VAD Decision task                                  *
    *                                                                 *
    * Usage:   The VAD example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef VADDECISION_HPP
#define VADDECISION_HPP

#include <forsyde.hpp>
#include "includes/vad.h"

using namespace ForSyDe::SDF;

void VADdecision_func(std::vector<short>& out,
                    std::vector<Pfloat> inp1,
                    std::vector<Pfloat> inp2)
{
    // Resize all the vectors to contain 1 element
    out.resize(1);
    
    Pfloat* in_pvad = &inp1[0];
    Pfloat* in_thvad = &inp2[0];
    short out_vvad;
#pragma ForSyDe begin VADdecision_func
    
    out_vvad = vad_decision(
        *in_pvad,  // filtered signal energy (mantissa+exponent)
        *in_thvad  // decision threshold (mantissa+exponent)
    );
    
#pragma ForSyDe end
    out[0] = out_vvad;
}


#endif
