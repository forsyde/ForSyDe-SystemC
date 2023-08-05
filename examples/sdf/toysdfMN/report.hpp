/**********************************************************************
    * report.hpp -- a report process                                  *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef REPORT_HPP
#define REPORT_HPP

#include <forsyde.hpp>
#include <iostream>

using namespace ForSyDe::SDF;

void report_func(tuple<>& out,
                const tuple<vector<double>>& inp)
{
    auto inp1 = get<0>(inp);
#pragma ForSyDe begin report_func
    std::cout << "output value: " << inp1[0] << std::endl;
#pragma ForSyDe end
}

#endif
