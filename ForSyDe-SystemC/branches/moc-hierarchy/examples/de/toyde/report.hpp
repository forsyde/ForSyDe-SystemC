/**********************************************************************
    * report.hpp -- the report process                                *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple DE system.                   *
    *                                                                 *
    * Usage:   ToyDE example                                          *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef REPORT_HPP
#define REPORT_HPP

#include <forsyde.hpp>
#include <iostream>

using namespace ForSyDe;

void report_func(tt_event<int> inp1)
{
#pragma ForSyDe begin report_func
    std::cout << "output value: " << inp1 << std::endl;
#pragma ForSyDe end
}

#endif
