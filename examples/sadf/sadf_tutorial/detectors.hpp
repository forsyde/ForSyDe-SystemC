/**********************************************************************
    * detectors.hpp -- Implementation of Detectors                    *
    *                                                                 *
    * Author:  Mohammad Vazirpanah (mohammad.vazirpanah@yahoo.com)    *
    *                                                                 *
    * Purpose: Demonstration of a simple process.                     *
    *                                                                 *
    * Usage:   SADF Tutorial                                          *
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

void detector1_cds_func(detector_scenario_type& new_scenario, 
                        const detector_scenario_type& previous_scenario, 
                        const std::vector<int>& inp)
{
    switch (previous_scenario)
    {
        case S1:
            new_scenario = S2;
            break;
        case S2:
            new_scenario = S3;
            break;
        case S3:
            new_scenario = S4;
            break;
        case S4:
            new_scenario = S1;
            break;
    }
}


void detector1_kss_func(vector<tuple<vector<kernel1_scenario_type>,vector<kernel2_scenario_type>>>& out,
                        const detector_scenario_type& current_scenario,
                        const vector<int>& inp)
{
    auto& [k1s,k2s] = out[0];
    k1s.resize(1);
    k2s.resize(1);
    switch (current_scenario)
    {
        case S1:
            k1s = {ADD};
            k2s = {MUL};
            break;
        case S2:
            k1s = {MINUS};
            k2s = {DIV};
            break;
        case S3:
            k1s = {ADD};
            k2s = {MUL};
            break;
        case S4:
            k1s = {MINUS};
            k2s = {DIV};
            break;
    }
}

#endif
