/**********************************************************************
    * add.hpp -- an adder process                                     *
    *                                                                 *
    * Authors: Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a heterogeneous system.               *
    *                                                                 *
    * Usage:   The digital filter example                             *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef ADD_HPP
#define ADD_HPP

#include <forsyde.hpp>

using namespace ForSyDe;
using namespace ForSyDe::SY;

void add_func(abst_ext<double>& out1,
              const abst_ext<double>& a, const abst_ext<double>& b)
{
    double inp1 = a.from_abst_ext(0);
    double inp2 = b.from_abst_ext(0);

#pragma ForSyDe begin add_func 
    out1 = inp1 + inp2;
#pragma ForSyDe end
}

#endif
