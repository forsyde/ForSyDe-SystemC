/**********************************************************************
    * add.hpp -- an adder process                                     *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
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

using namespace ForSyDe;

void add_func(std::tuple<int>& out, const std::tuple<int,int>& inp)
{

#pragma ForSyDe begin add_func
    std::get<0>(out) = std::get<0>(inp) + std::get<1>(inp);
#pragma ForSyDe end
}

#endif
