/**********************************************************************
    * siggen.hpp -- a ramp process                                    *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple source process.              *
    *                                                                 *
    * Usage:   MulAcc example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef SIGGEN_HPP
#define SIGGEN_HPP

#include <forsyde.hpp>

using namespace ForSyDe::SY;


void siggen_func(abst_ext<int>& out1, const abst_ext<int>& inp)
{
    int inp1 = from_abst_ext(inp,0);
#pragma ForSyDe begin siggen_func
    out1 = inp1 + 1;
#pragma ForSyDe end
}

#endif
