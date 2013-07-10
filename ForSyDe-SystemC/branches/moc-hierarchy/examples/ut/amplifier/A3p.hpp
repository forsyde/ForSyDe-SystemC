/**********************************************************************
    * A3p.hpp -- the amplifier process based on the scan constructor  *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
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

using namespace ForSyDe::UT;

void A3p_gamma_func(unsigned int& tokens, const int& state)
{
    tokens = 5;
}

void A3p_ns_func(int& next_state, const int& cur_state, const std::vector<int>& inp)
{
    int sum = std::accumulate(inp.begin(), inp.end(), 0);
    if (sum > 500)
        next_state = cur_state - 1;
    else if (sum < 400)
        next_state = cur_state + 1;
    else
        next_state = cur_state;
}

#endif
