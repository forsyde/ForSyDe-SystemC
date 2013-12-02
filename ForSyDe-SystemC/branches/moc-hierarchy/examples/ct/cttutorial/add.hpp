/**********************************************************************
    * add.hpp -- an adder process                                     *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple process.                     *
    *                                                                 *
    * Usage:   CT tutorial example                                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef ADD_HPP
#define ADD_HPP

#include <forsyde.hpp>

using namespace ForSyDe;

void add_func(CTTYPE& out1, const CTTYPE& inp1, const CTTYPE& inp2)
{
#pragma ForSyDe begin add_func
    out1 = inp1 + inp2;
#pragma ForSyDe end
}

#endif
