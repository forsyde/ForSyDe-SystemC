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

using namespace ForSyDe::SY;

void add_func(AbstExt<int>& out1,
              const AbstExt<int>& a, const AbstExt<int>& b)
{
    int inp1 = a.fromAbstExt(0);
    int inp2 = b.fromAbstExt(0);

#pragma ForSyDe begin add_func 
    out1 = inp1 + inp2;
#pragma ForSyDe end
}

#endif
