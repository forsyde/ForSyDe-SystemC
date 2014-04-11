/**********************************************************************
    * inc.hpp -- an adder process                                     *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple DDE system.                   *
    *                                                                 *
    * Usage:   ToyDDE example                                          *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef INC_HPP
#define INC_HPP

#include <forsyde.hpp>

using namespace ForSyDe;

void inc_func(abst_ext<int>& out1, const int& inp1)
{
#pragma ForSyDe begin inc_func 
    out1 = abst_ext<int>(inp1 + 1);
#pragma ForSyDe end
}

#endif
