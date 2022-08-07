/**********************************************************************
    * globals.hpp -- The globals Definitions for the SADF tutorial    *
    *                                                                 *
    * Author: Mohammad Vazirpanah (mohammad.vazirpanah@yahoo.com)     *
    *                                                                 *
    * Purpose: Demonstration of a simple process.                     *
    *                                                                 *
    * Usage:   SADF Tutorial                                          *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <forsyde.hpp>

using namespace ForSyDe;

enum detector_scenario_type {S1 ,S2, S3, S4};
enum kernel1_scenario_type {ADD, MINUS};
enum kernel2_scenario_type {MUL, DIV};

typedef std::map<
            detector_scenario_type,
            size_t
        > detector_scenario_table_type;

typedef std::map<
            kernel1_scenario_type,
            std::tuple<size_t,size_t>
        > kernel1_scenario_table_type;
typedef std::map<
            kernel2_scenario_type,
            std::tuple<size_t,size_t>
        > kernel2_scenario_table_type;

detector_scenario_table_type detector1_table = {
    {S1, 1},
    {S2, 1},
    {S3, 1},
    {S4, 1}
};

kernel1_scenario_table_type kernel1_table = 
{  
    {ADD, std::make_tuple(3,1)},
    {MINUS, std::make_tuple(2,1)}
};

kernel2_scenario_table_type kernel2_table =
{
    {MUL, std::make_tuple(2,1)},
    {DIV, std::make_tuple(2,1)}
};


#endif
