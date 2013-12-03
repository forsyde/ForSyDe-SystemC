/**********************************************************************
    * A2p.hpp -- the merge process based on the comb constructor      *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple example in the untimed MoC.  *
    *                                                                 *
    * Usage:   amplifier example                                      *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef A2P_HPP
#define A2P_HPP

#include <forsyde.hpp>
#include <algorithm>

using namespace ForSyDe;

void A2p_func(std::vector<int>& out1,
              const std::vector<std::tuple<std::vector<int>,std::vector<int>>>& inps)
{
    auto inp1 = std::get<0>(inps[0]);
    auto inp2 = std::get<1>(inps[0]);
    
    out1 = inp2;
    for (int i=0;i<5;i++)
        out1[i] = inp1[0] * inp2[i];
}

#endif
