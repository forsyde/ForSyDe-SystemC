/**********************************************************************
    * swap.hpp -- a timed swapper process                             *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a the usage of mealyT.                *
    *                                                                 *
    * Usage:   swap example                                           *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef SWAP_HPP
#define SWAP_HPP

#include <forsyde.hpp>

using namespace std;

using namespace ForSyDe;

void swap_gamma(size_t& out1, const tuple<int>& inp1)
{
#pragma ForSyDe begin swap_p_func 
    out1 = 2;
#pragma ForSyDe end
}

void swap_ns_func(tuple<int>& out1, 
                  const tuple<int>& inp1,
                  const tuple<vector<abst_ext<int>>,vector<abst_ext<int>>>& inp2)
{
#pragma ForSyDe begin swap_ns_func 
    std::get<0>(out1) = 0;
#pragma ForSyDe end
}

void swap_od_func(tuple<vector<abst_ext<int>>,vector<abst_ext<int>>>& out1,
                  const tuple<int>& inp1,
                  const tuple<vector<abst_ext<int>>,vector<abst_ext<int>>>& inp2)
{
#pragma ForSyDe begin swap_od_func
    get<0>(out1)[0] = get<1>(inp2)[1];
    get<0>(out1)[1] = get<1>(inp2)[0];

    get<1>(out1)[0] = get<0>(inp2)[1];
    get<1>(out1)[1] = get<0>(inp2)[0];
#pragma ForSyDe end
}

#endif
