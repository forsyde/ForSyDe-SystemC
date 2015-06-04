/**********************************************************************
    * AudioFilter.h -- Amplify different frequencies of the audio     *
    *                  signal                                         *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
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

void amplify_func(double& out1,
            const double& inp1, const double& inp2)
{
#pragma ForSyDe begin amplify_func
    out1 = inp2 * pow(base, inp1);
#pragma ForSyDe end
}


void tri_adder_func(double& out1,
                    const double& inp1,
                    const double& inp2,
                    const double& inp3)
{

#pragma ForSyDe begin tri_adder_func
    out1 = inp1 + inp2 + inp3;
#pragma ForSyDe end

}


SC_MODULE(audio_filter)
{
    SY::in_port<double> bass;
    SY::in_port<double> treble;
    SY::in_port<double> audioIn;

    SY::out_port<double> audioOut;

    SY::signal<double> ampedLow, ampedHigh, filteredLow, filteredBand, filteredHigh;
    SY::signal<double> fAudioIn1, fAudioIn2, fAudioIn3;

    audio_filter(sc_module_name name)
    {
        make_scomb3("tri_adder1", tri_adder_func, audioOut, ampedLow, filteredBand, ampedHigh);

        // Amplify Bass
        make_scomb2("amplitude1", amplify_func, ampedLow, bass, filteredLow);

        // Amplify Treble
        make_scomb2("amplitude2", amplify_func, ampedHigh, treble, filteredHigh);

        // Low pass filter
        make_smealy("fir1", fir_ns_func, fir_od_func<lpCoeff>, std::vector<double>(lpCoeff.size()-1, 0), filteredLow, fAudioIn1);

        // Band pass filter
        make_smealy("fir2", fir_ns_func, fir_od_func<bpCoeff>, std::vector<double>(bpCoeff.size()-1, 0), filteredBand, fAudioIn2);

        // High pass filter
        make_smealy("fir3", fir_ns_func, fir_od_func<hpCoeff>, std::vector<double>(hpCoeff.size()-1, 0), filteredHigh, fAudioIn3);

        auto fanout1 = make_fanout("fanout1", fAudioIn1, audioIn);
        fanout1->oport1(fAudioIn2);
        fanout1->oport1(fAudioIn3);
    }
};

#endif
