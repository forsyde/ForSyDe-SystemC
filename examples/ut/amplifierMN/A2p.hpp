/**********************************************************************
    * A2p.hpp -- the merge process based on the comb constructor      *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
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

void A2p_gamma_func(std::array<size_t,2>& itoks,
                    const std::tuple<bool>& csts)
{
    itoks[0] = 1;
    itoks[1] = 5;
}

void A2p_ns_func(std::tuple<bool>& nsts,
                const std::tuple<bool>& csts,
                const std::tuple<std::vector<int>,std::vector<int>>& inps)
{
}

void A2p_od_func(std::tuple<std::vector<int>>& outs,
                const std::tuple<bool>& csts,
                const std::tuple<std::vector<int>,std::vector<int>>& inps)
{
    auto inp1 = std::get<0>(inps);
    auto inp2 = std::get<1>(inps);
    auto out1 = std::get<0>(outs);
    
    out1.resize(5);
    for (int i=0;i<5;i++)
        out1[i] = inp1[0] * inp2[i];
    std::get<0>(outs) = out1;

}

#endif
