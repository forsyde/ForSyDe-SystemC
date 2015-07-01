/**********************************************************************
    * ACFAveraging.hpp                                                *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          adapted from KisTA: https://github.com/nandohca/kista  *
    *                                                                 *
    * Purpose: The ACF Avegaring task                                 *
    *                                                                 *
    * Usage:   The VAD example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef ACFAVERAGING_HPP
#define ACFAVERAGING_HPP

#include <forsyde.hpp>
#include "includes/vad.h"

using namespace ForSyDe::SDF;

void ACFAveraging_func(std::vector<tuple_of_vectors<L_av_t,L_av_t>>& out,
                    std::vector<r_t> inp1,
                    std::vector<r_t> inp2,
                    std::vector<short> inp3)
{
    // Resize all the vectors to contain 1 element
    // TODO: Generalize
    out.resize(1);
    std::get<0>(out[0]).resize(1);
    std::get<1>(out[0]).resize(1);
    
    short* in_r_h = inp1[0].data();
    short* in_r_l = inp2[0].data();
    short in_scal_acf = inp3[0];
    short* tout_L_av0 = (std::get<0>(out[0]))[0].data();
    short* tout_L_av1 = (std::get<1>(out[0]))[0].data();
#pragma ForSyDe begin ACFAveraging_func
    int out_L_av0[9];
    int out_L_av1[9];
    
    acf_averaging(
        in_r_h,      // autocorrelation of input signal frame (msb)
        in_r_l,      // autocorrelation of input signal frame (lsb)
        in_scal_acf, // scaling factor for the autocorrelations
        out_L_av0,   // ACF averaged over last four frames
        out_L_av1);  // ACF averaged over previous four frames
    
    for (auto i=0;i<9;i++)
    {
        tout_L_av0[i] = out_L_av0[i];
        tout_L_av1[i] = out_L_av1[i];
    }
#pragma ForSyDe end
}


#endif
