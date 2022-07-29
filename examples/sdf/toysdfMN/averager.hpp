/**********************************************************************           
    * averager.hpp -- an averager with feedback from outside          *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef AVERAGER_HPP
#define AVERAGER_HPP

#include <forsyde.hpp>

using namespace std;

void averager_func(tuple<vector<double>>& outs,
                    const tuple<vector<double>,vector<double>>& inps)
{
#pragma ForSyDe begin averager_func
    get<0>(outs)[0] = (get<0>(inps)[0]+get<0>(inps)[1]+get<1>(inps)[0])/3;
    get<0>(outs)[1] = (get<0>(inps)[1]+get<0>(inps)[2]+get<1>(inps)[1])/3;
#pragma ForSyDe end
}

#endif
