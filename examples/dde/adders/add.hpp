/**********************************************************************
    * add.hpp -- an adder process                                     *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple DDE system.                   *
    *                                                                 *
    * Usage:   ToyDDE example                                          *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef ADD_HPP
#define ADD_HPP

#include <forsyde.hpp>

using namespace ForSyDe;

void add_func(abst_ext<int>& out1, const abst_ext<int>& inp1, const abst_ext<int>& inp2)
{
#pragma ForSyDe begin add_func
    int a = from_abst_ext(inp1,0);
    int b = from_abst_ext(inp2,0);
    out1 = abst_ext<int>(a + b);
#pragma ForSyDe end
}

#endif
