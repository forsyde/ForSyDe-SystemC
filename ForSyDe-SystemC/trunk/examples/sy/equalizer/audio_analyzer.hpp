/**********************************************************************
    * audio_analyzer.hpp -- analyzes the current bass level and raises*
    *                    a flag when the bass level exceeds a limit.  *
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
#ifndef AUDIO_ANALYZER_HPP
#define AUDIO_ANALYZER_HPP

#include <forsyde.hpp>
#include <globals.hpp>

#include <tuple>
#include <complex>
#include <numeric>

#define grppts 2
#define limit  1.0
#define nLow   3

using namespace ForSyDe::SY;

void to_complex_func(std::complex<double>& out1, const double& inp1)
{
    out1 = std::complex<double>(inp1, 0);
}


void dft_func(abst_ext<std::vector<std::complex<double>>>& out,
                const abst_ext<std::vector<abst_ext<std::complex<double>>>> inp)
{
    if (is_absent(inp))
        out = abst_ext<std::vector<std::complex<double>>>();
    else
    {
        auto list = unsafe_from_abst_ext(inp);
        std::vector<std::complex<double>> temp;
        unsigned int n = list.size();
        for (unsigned int k = 0; k < n; ++k)
        {
            std::complex<double> result;
            for (unsigned int j = 0; j < n; ++j)
            {
                std::complex<double> omega(cos(k * j * 2*M_PI/n), sin(k * j * 2*M_PI/n));
                result += unsafe_from_abst_ext(list[j]) * omega;
            }
            temp.push_back(result);
        }
        out = abst_ext<std::vector<std::complex<double>>>(temp);
    }
}

void take_spectrum_func(abst_ext<std::vector<double>>& out, abst_ext<std::vector<std::complex<double>>> inp)
{
    if (is_absent(inp))
        out = abst_ext<std::vector<double>>();
    else
    {
        std::vector<std::complex<double>> tempvec = unsafe_from_abst_ext(inp);
        std::vector<double> retvec;
        for (int i=0;i<nLow;i++)
            retvec.push_back(log10(std::norm(tempvec[i+1])));
        out = abst_ext<std::vector<double>>(retvec);
    }
}

void check_bass_func(abst_ext<AnalyzerMsg>& out, abst_ext<std::vector<double>> inp)
{
    if (is_absent(inp))
        out = abst_ext<AnalyzerMsg>();
    else
    {
        std::vector<double> tempvec = unsafe_from_abst_ext(inp);
        double ta = std::accumulate(tempvec.begin(), tempvec.end(), 0.0);
        out = abst_ext<AnalyzerMsg>(ta > limit ? Fail : Pass);
    }
}

SC_MODULE(audio_analyzer)
{
    SY::in_port<double> audioIn;

    SY::out_port<AnalyzerMsg> analyzerOut;

    SY::signal<std::complex<double>> cmplxSig;
    SY::signal<std::vector<abst_ext<std::complex<double>>>> grpSig;
    SY::signal<std::vector<std::complex<double>>> dftSig;
    SY::signal<std::vector<double>> spectrumSig;

    SC_CTOR(audio_analyzer)
    {
        make_scomb("to_complex1", to_complex_func, cmplxSig, audioIn);

        make_group("group_samples", grppts, grpSig, cmplxSig);

        make_comb("dft1", dft_func, dftSig, grpSig);

        make_comb("take_spectrum", take_spectrum_func, spectrumSig, dftSig);

        make_comb("check_bass", check_bass_func, analyzerOut, spectrumSig);
    }
};

#endif
