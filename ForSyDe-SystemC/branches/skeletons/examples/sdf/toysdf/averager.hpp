/**********************************************************************           
    * averager.hpp -- an averager with feedback from outside          *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
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

void averager_func(std::vector<double>& out1,
                   const std::vector<double>& inp1,
                   const std::vector<double>& inp2)
{
#pragma ForSyDe begin averager_func
    out1[0] = (inp1[0]+inp1[1]+inp2[0])/3;
    out1[1] = (inp1[1]+inp1[2]+inp2[1])/3;
#pragma ForSyDe end
}

#endif
