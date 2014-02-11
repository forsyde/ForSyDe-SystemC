/**********************************************************************
    * report.hpp -- a report process                                  *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a data parallel model.                *
    *                                                                 *
    * Usage:   datapar example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef REPORT_HPP
#define REPORT_HPP

#include <forsyde.hpp>
#include <array>
#include <iostream>

using namespace ForSyDe;

void report_func(std::array<int,10> inp1)
{
#pragma ForSyDe begin report_func
    std::cout << "output value: " << inp1 << std::endl;
#pragma ForSyDe end
}

#endif
