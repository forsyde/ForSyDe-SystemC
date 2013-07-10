/**********************************************************************           
    * mux.hpp -- a three input multiplexer                            *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Sorter example                                         *
    *          inspired by material from Doulos SystemC course        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef MUX_HPP
#define MUX_HPP

#include <forsyde.hpp>

using namespace ForSyDe;
using namespace ForSyDe::SY;

void mux_func(abst_ext<int>& out1,
              const abst_ext<int>& sel, const abst_ext<int>& a,
              const abst_ext<int>& b, const abst_ext<int>& c)
{
    int inp1 = sel.from_abst_ext(0);
    int inp2 = a.from_abst_ext(0);
    int inp3 = b.from_abst_ext(0);
    int inp4 = c.from_abst_ext(0);

#pragma ForSyDe begin mux_func
    switch (inp1)
    {
        case 0: out1 = inp2; break;
        case 1: out1 = inp3; break;
        case 2: out1 = inp4; break;
        default: out1 = inp2;
    }
#pragma ForSyDe end
}

#endif
