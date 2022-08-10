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

void swap_gamma(size_t& out1, const int& inp1)
{
#pragma ForSyDe begin swap_p_func 
    out1 = 2;
#pragma ForSyDe end
}

// void swap_ns_func(int& out1, 
//                   const int& inp1, const vector<int>& inp2)
// {
// #pragma ForSyDe begin swap_ns_func 
//     out1 = 0;
// #pragma ForSyDe end
// }
void swap_ns_func(int& out1, 
                  const int& inp1, const tuple<vector<int>>& inp2)
{
#pragma ForSyDe begin swap_ns_func 
    out1 = 0;
#pragma ForSyDe end
}

// void swap_od_func(vector<abst_ext<int>>& out1,
//                   const tuple<int>& inp1, const vector<abst_ext<int>>& inp2)
// {
// #pragma ForSyDe begin swap_od_func
//     out1.resize(2);
//     out1[0] = inp2[1];
//     out1[1] = inp2[0];
// #pragma ForSyDe end
// }
void swap_od_func(tuple<vector<int>>& outs1,
                  const tuple<int>& st, const tuple<vector<int>>& inps1)
{
    auto inp1 = get<0>(inps1);
    auto& out1 = get<0>(outs1);
#pragma ForSyDe begin swap_od_func
    if (inp1[1]>inp1[0])
    {
        out1.resize(2);
        out1[0] = inp1[1];
        out1[1] = inp1[0];
    }
    else if (inp1[1]==inp1[0])
    {
        out1.resize(1);
        out1[0] = inp1[0];
    }
    else
    {
        out1.resize(3);
        out1[0] = inp1[1];
        out1[1] = inp1[0];
        out1[2] = inp1[1]-inp1[0];
    }
#pragma ForSyDe end
}

#endif
