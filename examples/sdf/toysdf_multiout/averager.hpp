/**********************************************************************           
    * averager.hpp -- an averager with feedback from outside          *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef AVERAGER_HPP
#define AVERAGER_HPP

#include <forsyde.hpp>

void averager_func(std::vector<std::tuple<std::vector<float>,std::vector<float>>>& outs,
                   const std::vector<float>& inp1, const std::vector<float>& inp2)
{
    std::vector<float> out1(2);
    std::vector<float> out2(2);
#pragma ForSyDe begin averager_func
    out1[0] = (inp1[0]+inp1[1]+inp2[0])/3;
    out1[1] = (inp1[1]+inp1[2]+inp2[1])/3;
    out2[0] = (inp1[0]+inp1[1]+inp2[0])/3;
    out2[1] = (inp1[1]+inp1[2]+inp2[1])/3;
#pragma ForSyDe end
    outs[0] = std::make_pair(out1,out2);
}

#endif
