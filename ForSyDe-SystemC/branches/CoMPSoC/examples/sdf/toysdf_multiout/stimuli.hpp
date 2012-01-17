/**********************************************************************           
    * stimuli.hpp -- the stimuli generator                            *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
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

void stimuli_func(std::vector<std::tuple<std::vector<float>,std::vector<float>>>& outs, const std::vector<float>& inp1)
{
    std::vector<float> out1(1);
    std::vector<float> out2(1);
#pragma ForSyDe begin stimuli_func
    out1[0] = inp1[0]+1;
    out2[0] = inp1[0]+1;
#pragma ForSyDe end
    outs[0] = std::make_pair(out1,out2);
}

#endif
