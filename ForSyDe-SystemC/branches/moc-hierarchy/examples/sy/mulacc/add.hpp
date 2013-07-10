/**********************************************************************
    * add.hpp -- an adder process                                     *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple sequential process.          *
    *                                                                 *
    * Usage:   MulAcc example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef ADD_HPP
#define ADD_HPP

#include <forsyde.hpp>

using namespace ForSyDe;
using namespace ForSyDe::SY;

void add_func(abst_ext<int>& out1,
              const abst_ext<int>& a, const abst_ext<int>& b)
{
    int inp1 = a.from_abst_ext(0);
    int inp2 = b.from_abst_ext(0);

#pragma ForSyDe begin add_func 
    out1 = inp1 + inp2;
#pragma ForSyDe end
}

#endif
