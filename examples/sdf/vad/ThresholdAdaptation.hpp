/**********************************************************************
    * ThresholdAdaptation.hpp                                         *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          adapted from KisTA: https://github.com/nandohca/kista  *
    *                                                                 *
    * Purpose: The Threshold Adaptation.hpp task                      *
    *                                                                 *
    * Usage:   The VAD example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef THRESHOLDADAPTATION_HPP
#define THRESHOLDADAPTATION_HPP

#include <forsyde.hpp>
#include "includes/vad.h"

using namespace ForSyDe::SDF;

void ThresholdAdaptation_func(tuple_of_vectors<rvad_t,Pfloat>& outs,
                            const tuple_of_vectors<rav1_t,short,pvad_acf0_t,short,short>& inps)
{
    auto inp1 = std::get<0>(inps);
    auto inp2 = std::get<1>(inps);
    auto inp3 = std::get<2>(inps);
    auto inp4 = std::get<3>(inps);
    auto inp5 = std::get<4>(inps);   
    short* in_rav_buff = std::get<0>(inp1[0]).data();
    short in_rav_scal = std::get<1>(inp1[0]);
    short in_stat = inp2[0];
    Pfloat in_pvad_pvad = std::get<0>(inp3[0]);
    Pfloat in_pvad_acf0 = std::get<1>(inp3[0]);
    short in_tone = inp4[0];
    short in_ptch = inp5[0];
    
    short* out_rvad_buff = std::get<0>((std::get<0>(outs))[0]).data();
    short* out_rvad_scal = &std::get<1>((std::get<0>(outs))[0]);
    Pfloat* out_thvad = &(std::get<1>(outs))[0];
    
#pragma ForSyDe begin ThresholdAdaptation_func

    threshold_adaptation(
        in_stat,        // flag to indicate spectral stationarity
        in_ptch,        // flag to indicate a periodic signal component
        in_tone,        // flag to indicate a tone signal component
        in_rav_buff,    // ACF obtained from L_av1
        in_rav_scal,    // rav1[] scaling factor
        in_pvad_pvad,   // filtered signal energy (mantissa+exponent)
        in_pvad_acf0,   // signal frame energy (mantissa+exponent)
        out_rvad_buff,  // autocorrelated adaptive filter coefficients
        out_rvad_scal,  // rvad[] scaling factor
        out_thvad       // decision threshold (mantissa+exponent)
    );
    
#pragma ForSyDe end
}


#endif
