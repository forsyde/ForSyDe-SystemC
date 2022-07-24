/**********************************************************************
    * mul.hpp -- a multilpier process                                 *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
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

using namespace ForSyDe;

void mul_func(int& out1, const std::tuple<int,int>& inp)
{

#pragma ForSyDe begin mul_func
    out1 = std::get<0>(inp) * std::get<1>(inp);
#pragma ForSyDe end
}

#endif
