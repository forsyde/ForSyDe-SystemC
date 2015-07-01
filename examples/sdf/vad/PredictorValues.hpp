/**********************************************************************
    * PredictorValues.hpp                                             *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          adapted from KisTA: https://github.com/nandohca/kista  *
    *                                                                 *
    * Purpose: The Predictor Values task                              *
    *                                                                 *
    * Usage:   The VAD example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef PREDICTORVALUES_HPP
#define PREDICTORVALUES_HPP

#include <forsyde.hpp>
#include <includes/vad.h>

using namespace ForSyDe::SDF;

void PredictorValues_func(std::vector<rav1_t>& out,
                        std::vector<L_av_t> inp1)
{
    // Resize all the vectors to contain 1 element
    out.resize(1);
    
    short* out_rav_buff = std::get<0>(out[0]).data();
    short* out_rav_scal = &std::get<1>(out[0]);
#pragma ForSyDe begin PredictorValues_func
    
    int in_L_av[9];
    for (auto i=0;i<9;i++) in_L_av[i] = inp1[0][i];
    
    predictor_values(
        in_L_av,        // ACF averaged over previous four frames
        out_rav_buff,   // ACF obtained from in_buff
        out_rav_scal    // out scaling factor
    );
    
#pragma ForSyDe end
}


#endif
