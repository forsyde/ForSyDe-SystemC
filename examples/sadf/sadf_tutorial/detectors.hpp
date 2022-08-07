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


void detector1_kss_func(std::tuple<vector<kernel1_scenario_type>,vector<kernel2_scenario_type>>& out,
                        const detector_scenario_type& current_scenario,
                        const std::vector<int>& inp)
{
    get<0>(out).resize(1);
    get<1>(out).resize(1);
    switch (current_scenario)
    {
        case S1:
            get<0>(out) = {ADD};
            get<1>(out) = {MUL};
            break;
        case S2:
            get<0>(out) = {MINUS};
            get<1>(out) = {DIV};
            break;
        case S3:
            get<0>(out) = {ADD};
            get<1>(out) = {MUL};
            break;
        case S4:
            get<0>(out) = {MINUS};
            get<1>(out) = {DIV};
            break;
    }
}

#endif
