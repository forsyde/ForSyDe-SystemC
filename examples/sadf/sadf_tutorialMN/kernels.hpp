/**********************************************************************
    * kernels.hpp -- Implementation of Kernels                        *
    *                                                                 *
    * Author:  Mohammad Vazirpanah (mohammad.vazirpanah@yahoo.com)    *
    *                                                                 *
    * Purpose: Demonstration of a simple process.                     *
    *                                                                 *
    * Usage:   SADF Tutorial                                          *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef KERNELS_HPP
#define KERNELS_HPP

#include <forsyde.hpp>
#include "globals.hpp"

using namespace sc_core;
using namespace ForSyDe;
using namespace std;

void kernel1_func(tuple<vector<int>>& out, 
                    const kernel1_scenario_type& _scenarios_state, 
                    const tuple<vector<int>>& inp)
{
    auto inp1 = get<0>(inp);
    auto& out1 = get<0>(out);
    if (_scenarios_state == ADD)
        out1[0] = inp1[0] + inp1[1] + inp1[2];
    else if (_scenarios_state == MINUS)
        out1[0] = inp1[1]- inp1[0];
}


void kernel2_func(tuple<vector<int>>& out, 
                    const kernel2_scenario_type& _scenarios_state, 
                    const tuple<vector<int>>& inp)
{
    auto inp1 = get<0>(inp);
    auto& out1 = get<0>(out);
    if (_scenarios_state == MUL)
        out1[0] = inp1[0] * inp1[1];
    else if (_scenarios_state == DIV)
        out1[0] = inp1[0] / inp1[1];
}


#endif
