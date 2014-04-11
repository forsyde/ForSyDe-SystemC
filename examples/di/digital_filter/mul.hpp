/**********************************************************************
    * mul.hpp -- a multilpier process                                 *
    *                                                                 *
    * Authors: Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a heterogeneous system.               *
    *                                                                 *
    * Usage:   The digital filter example                             *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef MUL_HPP
#define MUL_HPP

#include <forsyde.hpp>

using namespace ForSyDe;
using namespace ForSyDe::SY;

void mul_func(abst_ext<double>& out1,
              const abst_ext<double>& a, const abst_ext<double>& b)
{
    double inp1 = a.from_abst_ext(0);
    double inp2 = b.from_abst_ext(0);

#pragma ForSyDe begin mul_func  
    out1 = inp1 * inp2;
#pragma ForSyDe end
}

#endif
