/**********************************************************************
    * detectors.hpp -- Implementation of Detectors                    *
    *                                                                 *
    * Author:  Hosein Attarzadeh (h_attarzadeh@sbu.ac.ir)             *
    *                                                                 *
    * Purpose: Demonstration of an example in the SADF MoC.           *
    *                                                                 *
    * Usage:   MPEG4-SP example                                       *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DETECTORS_HPP
#define DETECTORS_HPP

#include <forsyde.hpp>
#include "globals.hpp"

using namespace sc_core;
using namespace ForSyDe;
using namespace std;

// void fd_cds_func(frame_type& new_scenario, 
//                  const frame_type& previous_scenario, 
//                  const tuple<vector<frame_type>,vector<bool>>& inp)
auto fd_cds_func = [](auto&& new_scenario, 
                const auto& previous_scenario, 
                const auto& inp)
{
    const auto& [inpFT, inp2] = inp;
    
    new_scenario = inpFT[0];
};

// void fd_kss_func(tuple<vector<frame_type>,vector<frame_type>,vector<frame_type>,vector<frame_type>>& out,
//                  const frame_type& current_scenario,
//                  const tuple<vector<frame_type>,vector<bool>>& inp)
auto fd_kss_func = [](auto&& out,
                 const auto& current_scenario,
                 const auto& inp)
{
    auto&& [out1, out2, out3, out4] = out;

    out1[0] = out2[0] = out3[0] = out4[0] = current_scenario;
    // Replicate the first and second outputs according to the scenario
    for (size_t i=1; i<out1.size(); i++)
        out1[i] = out1[0];
    for (size_t i=1; i<out2.size(); i++)
        out2[i] = out2[0];
};

#endif
