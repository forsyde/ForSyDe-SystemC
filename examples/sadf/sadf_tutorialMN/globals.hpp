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
using namespace std;

enum detector_scenario_type {S1 ,S2, S3, S4};
enum kernel1_scenario_type {ADD, MINUS};
enum kernel2_scenario_type {MUL, DIV};

typedef map<
            detector_scenario_type,
            array<size_t,2>
        > detector_scenario_table_type;

typedef map<
            kernel1_scenario_type,
            tuple<array<size_t,1>,array<size_t,1>>
        > kernel1_scenario_table_type;
typedef map<
            kernel2_scenario_type,
            tuple<array<size_t,1>,array<size_t,1>>
        > kernel2_scenario_table_type;

detector_scenario_table_type detector1_table = {
    {S1, {1,1}},
    {S2, {1,1}},
    {S3, {1,1}},
    {S4, {1,1}}
};

kernel1_scenario_table_type kernel1_table = 
{  
    {ADD,   make_tuple(array<size_t,1>{3},array<size_t,1>{1})},
    {MINUS, make_tuple(array<size_t,1>{2},array<size_t,1>{1})}
};

kernel2_scenario_table_type kernel2_table =
{
    {MUL, make_tuple(array<size_t,1>{2},array<size_t,1>{1})},
    {DIV, make_tuple(array<size_t,1>{2},array<size_t,1>{1})}
};


#endif
