/**********************************************************************
    * ctadd.hpp -- an adder process for the CT MoC                    *
    *                                                                 *
    * Authors: Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a heterogeneous system.               *
    *                                                                 *
    * Usage:   The digital filter example                             *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef CTADD_HPP
#define CTADD_HPP

#include <forsyde.hpp>

using namespace ForSyDe;
using namespace ForSyDe::CT;

void ctadd_func(CTTYPE& out1, const CTTYPE& inp1, const CTTYPE& inp2)
{
#pragma ForSyDe begin ctadd_func
    out1 = inp1 + inp2;
#pragma ForSyDe end
}

#endif
