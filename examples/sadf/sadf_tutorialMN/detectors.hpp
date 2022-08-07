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
                        const tuple<vector<int>>& inp)
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


void detector1_kss_func(tuple<kernel1_scenario_type,kernel2_scenario_type>& out,
                        const detector_scenario_type& current_scenario,
                        const tuple<vector<int>>& inp)
{
    switch (current_scenario)
    {
        case S1:
            out = make_tuple(ADD,MUL);
            break;
        case S2:
            out = make_tuple(MINUS,DIV);
            break;
        case S3:
            out = make_tuple(ADD,MUL);
            break;
        case S4:
            out = make_tuple(MINUS,DIV);
            break;
    }
}

#endif
