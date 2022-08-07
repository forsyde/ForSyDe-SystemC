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


void kernel1_func(std::vector<int>& out, 
                    const kernel1_scenario_type& _scenarios_state, 
                    const std::vector<int>& inp)
{
    if (_scenarios_state == ADD)
        out[0] = inp[0] + inp[1] + inp[2];
    else if (_scenarios_state == MINUS)
        out[0] = inp[1]- inp[0];
}


void kernel2_func(std::vector<int>& out, 
                    const kernel2_scenario_type& _scenarios_state, 
                    const std::vector<int>& inp)
{
    if (_scenarios_state == MUL)
        out[0] = inp[0] * inp[1];
    else if (_scenarios_state == DIV)
        out[0] = inp[0] / inp[1];
}


#endif
