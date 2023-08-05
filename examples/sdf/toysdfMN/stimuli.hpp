/**********************************************************************
    * stimuli.hpp -- a stimuli generator                              *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef STIMULI_HPP
#define STIMULI_HPP

#include <forsyde.hpp>

using namespace ForSyDe::SDF;

void stimuli_func(tuple<vector<double>,vector<int>>& out, 
                    const tuple<vector<int>>& inp)
{
    auto inp1 = get<0>(inp);
    auto& out1 = get<0>(out);
    auto& out2 = get<1>(out);
#pragma ForSyDe begin stimuli_func
    out1[0] = (double)inp1[0];
    out2[0] = inp1[0] + 1;
#pragma ForSyDe end
    if (out2[0] == 20)
        wait();
}

#endif
