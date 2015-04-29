/**********************************************************************           
    * decoder.hpp -- decodes its three inputs                         *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Sorter example                                         *
    *          inspired by material from Doulos SystemC course        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef DECODER_HPP
#define DECODER_HPP

#include <forsyde.hpp>

using namespace ForSyDe;
using namespace ForSyDe::SY;

void decoder_func(abst_ext<int>& out1,
              const abst_ext<bool>& in0, const abst_ext<bool>& in1,
              const abst_ext<bool>& in2)
{
    int inp1 = in0.from_abst_ext(0);
    int inp2 = in1.from_abst_ext(0);
    int inp3 = in2.from_abst_ext(0);

#pragma ForSyDe begin decoder_func  
    out1 = 0;
    if (inp1 && !inp3) out1 = 0;
    else if (!inp1 && inp2) out1 = 1;
        else if (!inp2 && inp3) out1 = 2;
#pragma ForSyDe end
}

#endif
