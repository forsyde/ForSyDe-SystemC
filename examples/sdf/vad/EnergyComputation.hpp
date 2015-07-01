/**********************************************************************
    * EnergyComputation.hpp                                           *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          adapted from KisTA: https://github.com/nandohca/kista  *
    *                                                                 *
    * Purpose: The Energy Computation task                            *
    *                                                                 *
    * Usage:   The VAD example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef ENERGYCOMPUTATION_HPP
#define ENERGYCOMPUTATION_HPP

#include <forsyde.hpp>
#include "includes/vad.h"

using namespace ForSyDe::SDF;

void EnergyComputation_func(std::vector<tuple_of_vectors<pvad_acf0_t,Pfloat>>& out,
                            std::vector<rvad_t> inp1,
                            std::vector<r_t> inp2,
                            std::vector<short> inp3)
{
    // Resize all the vectors to contain 1 element
    // TODO: Generalize
    out.resize(1);
    std::get<0>(out[0]).resize(1);
    std::get<1>(out[0]).resize(1);
    
    short* in_rvad_buff = std::get<0>(inp1[0]).data();
    short in_rvad_scal = std::get<1>(inp1[0]);
    short* in_r_h = inp2[0].data();
    short in_scal_acf = inp3[0];
    
    Pfloat* val_pvad = &std::get<0>((std::get<0>(out[0]))[0]);
    Pfloat* val_acf0 = &std::get<1>((std::get<0>(out[0]))[0]);
    
#pragma ForSyDe begin ACFAveraging_func
    
    energy_computation(
        in_r_h,         // autocorrelation of input signal frame (msb)
        in_scal_acf,    // scaling factor for the autocorrelations       
        in_rvad_buff,   // autocorrelated adaptive filter coefficients
        in_rvad_scal,   // scaling factor for rvad[]
        val_acf0,       // signal frame energy (mantissa+exponent)
        val_pvad        // filtered signal energy (mantissa+exponent)
    );
    
#pragma ForSyDe end
    (std::get<1>(out[0]))[0] = *val_pvad;
}


#endif
