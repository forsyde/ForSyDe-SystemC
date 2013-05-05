/**********************************************************************
    * add.hpp -- an adder process                                     *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple sequential process.          *
    *                                                                 *
    * Usage:   Parallel MulAcc example                                *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef SIGGEN_HPP
#define SIGGEN_HPP

#include <forsyde.hpp>

using namespace ForSyDe::SY;


void siggen_func(abst_ext<int>& out1, const abst_ext<int>& inp)
{
    int inp1 = inp.from_abst_ext(0);
#pragma ForSyDe begin siggen_func
    out1 = -inp1;
#pragma ForSyDe end
}

#endif
