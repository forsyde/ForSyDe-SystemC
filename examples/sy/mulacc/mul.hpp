/**********************************************************************
    * mul.hpp -- a multilpier process                                 *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple sequential processes.        *
    *                                                                 *
    * Usage:   MulAcc example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef MUL_HPP
#define MUL_HPP

#include <forsyde.hpp>

using namespace ForSyDe::SY;

void mul_func(AbstExt<int>& out1,
              const AbstExt<int>& a, const AbstExt<int>& b)
{
    int inp1 = a.fromAbstExt(0);
    int inp2 = b.fromAbstExt(0);

#pragma ForSyDe begin mul_func  
    out1 = inp1 * inp2;
#pragma ForSyDe end
}

#endif
