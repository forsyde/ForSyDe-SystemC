/**********************************************************************
    * A3p.hpp -- the amplifier process based on the scan constructor  *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple example in the untimed MoC.  *
    *                                                                 *
    * Usage:   amplifier example                                      *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef A3P_HPP
#define A3P_HPP

#include <forsyde.hpp>
#include <numeric>

using namespace ForSyDe;

void A3p_gamma_func(std::array<size_t,1>& itoks,
                    const std::tuple<int>& csts)
{
    itoks[0] = 5;
}

void A3p_ns_func(std::tuple<int>& nsts,
                const std::tuple<int>& csts,
                const std::tuple<std::vector<int>>& inps)
{
    auto inp = std::get<0>(inps);
    int sum = std::accumulate(inp.begin(), inp.end(), 0);
    if (sum > 500)
        std::get<0>(nsts) = std::get<0>(csts) - 1;
    else if (sum < 400)
        std::get<0>(nsts) = std::get<0>(csts) + 1;
    else
        std::get<0>(nsts) = std::get<0>(csts);
}

void A3p_od_func(std::tuple<std::vector<int>>& outs,
                const std::tuple<int>& csts,
                const std::tuple<std::vector<int>>& inps)
{
    std::get<0>(outs).resize(1);
    auto inp = std::get<0>(inps);
    int sum = std::accumulate(inp.begin(), inp.end(), 0);
    if (sum > 500)
        std::get<0>(outs)[0] = std::get<0>(csts) - 1;
    else if (sum < 400)
        std::get<0>(outs)[0] = std::get<0>(csts) + 1;
    else
        std::get<0>(outs)[0] = std::get<0>(csts);
}
#endif
