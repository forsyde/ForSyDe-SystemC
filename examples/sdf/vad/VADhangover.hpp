/**********************************************************************
    * VADhangover.hpp                                                 *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          adapted from KisTA: https://github.com/nandohca/kista  *
    *                                                                 *
    * Purpose: The VAD Hangover task                                  *
    *                                                                 *
    * Usage:   The VAD example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef VADHANGOVER_HPP
#define VADHANGOVER_HPP

#include <forsyde.hpp>
#include "includes/vad.h"

using namespace ForSyDe::SDF;

void VADhangover_func(std::vector<short>& out, std::vector<short> inp1)
{
    // Resize all the vectors to contain 1 element
    out.resize(1);
    
    short out_vad;
#pragma ForSyDe begin VADhangover_func
    
    out_vad = vad_hangover(
        inp1[0]  // vad decision before hangover is added
    );
    
#pragma ForSyDe end
    out[0] = out_vad;
}


#endif
