/**********************************************************************
    * report.hpp -- prints the output to console                      *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple example in the untimed MoC.  *
    *                                                                 *
    * Usage:   amplifier example                                      *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef REPORT_HPP
#define REPORT_HPP

#include <forsyde.hpp>
#include <iostream>

using namespace ForSyDe;

void report_func(const int& inp1)
{
    std::cout << "output value: " << inp1 << std::endl;
}

#endif
