/**********************************************************************
    * add.hpp -- an adder process                                     *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple DE system.                   *
    *                                                                 *
    * Usage:   ToyDE example                                          *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef ADD_HPP
#define ADD_HPP

#include <forsyde.hpp>

using namespace ForSyDe;

void add_func(int& out1, const int& inp1, const int& inp2)
{
#pragma ForSyDe begin add_func 
    out1 = inp1 + inp2;
#pragma ForSyDe end
}

#endif
