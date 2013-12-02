/**********************************************************************
    * report.hpp -- the report process                                *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a the usage of mealyT.                *
    *                                                                 *
    * Usage:   swap example                                           *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef REPORT_HPP
#define REPORT_HPP

#include <forsyde.hpp>
#include <iostream>

using namespace ForSyDe;

void report_func(abst_ext<int> inp1)
{
    static unsigned int local_time = 0;
#pragma ForSyDe begin report_func
    //~ if (is_present(inp1))
        std::cout << "at: " << local_time++
                  << ", value: " << inp1
                  << std::endl;
#pragma ForSyDe end
}

#endif
