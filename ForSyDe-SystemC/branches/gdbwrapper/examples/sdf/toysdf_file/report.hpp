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
using namespace std;

void report_func(std::string& line,
    const tuple<vector<double>,vector<double>>& inp
)
{
    std::stringstream ss;
    ss << inp;
    line = ss.str();
}

#endif
