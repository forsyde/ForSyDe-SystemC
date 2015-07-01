/**********************************************************************
    * SpectralComparison.hpp                                          *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          adapted from KisTA: https://github.com/nandohca/kista  *
    *                                                                 *
    * Purpose: The Spectral Comparison task                           *
    *                                                                 *
    * Usage:   The VAD example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SPECTRALCOMPARISON_HPP
#define SPECTRALCOMPARISON_HPP

#include <forsyde.hpp>
#include <includes/vad.h>

using namespace ForSyDe::SDF;

void SpectralComparison_func(std::vector<short>& out,
                            std::vector<L_av_t> inp1,
                            std::vector<rav1_t> inp2)
{
    // Resize all the vectors to contain 1 element
    // TODO: Generalize
    out.resize(1);
    
    short* in_rav_buff = std::get<0>(inp2[0]).data();
    short in_rav_scal = std::get<1>(inp2[0]);
    
    short out_stat;
    
#pragma ForSyDe begin SpectralComparison_func
    
    int in_L_av[9];
    for (auto i=0;i<9;i++) in_L_av[i] = inp1[0][i];
    
    // Returns flag to indicate spectral stationarity
    out_stat = spectral_comparison(
        in_rav_buff,    // ACF obtained from L_av1
        in_rav_scal,    // rav1[] scaling factor
        in_L_av);       // ACF averaged over last four frames
    
#pragma ForSyDe end
    
    out[0] = out_stat;
}


#endif
