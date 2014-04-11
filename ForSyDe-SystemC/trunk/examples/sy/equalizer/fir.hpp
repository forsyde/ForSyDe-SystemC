/**********************************************************************
    * fir.hpp -- A FIR filter                                         *
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
#ifndef FIR_HPP
#define FIR_HPP

#include <forsyde.hpp>

#include <vector>
#include <cmath>

using namespace ForSyDe::SY;

void fir_ns_func(std::vector<double>& out_st,
                const std::vector<double>& inp_st,
                const abst_ext<double>& inp1)
{
    out_st = inp_st;
    out_st.insert(out_st.begin(), unsafe_from_abst_ext(inp1));
    out_st.pop_back();
}

template <std::vector<double>& coefs>
void fir_od_func(abst_ext<double>& out,
                const std::vector<double>& st,
                const abst_ext<double>& inp1)
{
    double out1 = unsafe_from_abst_ext(inp1) * coefs.front();
    for (unsigned i=0; i<st.size(); i++)
        out1 += st[i] * coefs[i+1];
    out = abst_ext<double>(out1);
}

#endif
