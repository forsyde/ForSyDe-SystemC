/**********************************************************************
    * AudioFilter.h -- Amplify different frequencies of the audio     *
    *                  signal                                         *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *          the Haskell version from Ingo Sander (ingo@kth.se)     *
    *                                                                 *
    * Purpose: Demonstrating how co-simulation with legacy codes is   *
    *          performed.                                             *
    *                                                                 *
    * Usage:   Equalizer example                                      *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/
#ifndef AUDIOFILTER_HPP
#define AUDIOFILTER_HPP

#include <forsyde.hpp>

#include <vector>
#include <cmath>

#include "fir.hpp"

#define base 1.1

using namespace ForSyDe::SY;

void amplify_func(abst_ext<double>& out,
            const abst_ext<double>& x, const abst_ext<double>& y)
{
    double inp1 = unsafe_from_abst_ext(x);
    double inp2 = unsafe_from_abst_ext(y);
    double out1;
#pragma ForSyDe begin amplify_func    
    out1 = inp2 * pow(base, inp1);
#pragma ForSyDe end
    out = abst_ext<double>(out1);
}


void tri_adder_func(abst_ext<double>& output1,
                    const abst_ext<double>& input1,
                    const abst_ext<double>& input2,
                    const abst_ext<double>& input3)
{
    double inp1 = unsafe_from_abst_ext(input1);
    double inp2 = unsafe_from_abst_ext(input2);
    double inp3 = unsafe_from_abst_ext(input3);
    double out1;
    
#pragma ForSyDe begin tri_adder_func    
    out1 = inp1 + inp2 + inp3;
#pragma ForSyDe end
    
    output1 = abst_ext<double>(out1);
}


SC_MODULE(audio_filter)
{
    SY_in<double> bass;
    SY_in<double> treble;
    SY_in<double> audioIn;
    
    SY_out<double> audioOut;
    
    SY2SY<double> ampedLow, ampedHigh, filteredLow, filteredBand, filteredHigh;
    SY2SY<double> fAudioIn1, fAudioIn2, fAudioIn3;
    
    audio_filter(sc_module_name name)
    {
        make_comb3("tri_adder1", tri_adder_func, audioOut, ampedLow, filteredBand, ampedHigh);
        
        // Amplify Bass
        make_comb2("amplitude1", amplify_func, ampedLow, bass, filteredLow);
        
        // Amplify Treble
        make_comb2("amplitude2", amplify_func, ampedHigh, treble, filteredHigh);
        
        // Low pass filter
        make_mealy("fir1", fir_ns_func, fir_od_func<lpCoeff>, std::vector<double>(lpCoeff.size()-1, 0), filteredLow, fAudioIn1);
        
        // Band pass filter
        make_mealy("fir2", fir_ns_func, fir_od_func<bpCoeff>, std::vector<double>(bpCoeff.size()-1, 0), filteredBand, fAudioIn2);
        
        // High pass filter
        make_mealy("fir3", fir_ns_func, fir_od_func<hpCoeff>, std::vector<double>(hpCoeff.size()-1, 0), filteredHigh, fAudioIn3);
        
        auto fanout1 = make_fanout("fanout1", fAudioIn1, audioIn);
        fanout1->oport1(fAudioIn2);
        fanout1->oport1(fAudioIn3);
    }
};

#endif
