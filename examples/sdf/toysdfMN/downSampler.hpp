/**********************************************************************           
    * downSampler.hpp -- a 3:2 down sampler                           *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DOWNSAMPLER_HPP
#define DOWNSAMPLER_HPP

#include <forsyde.hpp>

using namespace std;

void downSampler_func(tuple<vector<double>>& outs,
                      const tuple<vector<double>>& inps)
{
#pragma ForSyDe begin downSampler_func
    std::get<0>(outs)[0] = std::get<0>(inps)[0];
    std::get<0>(outs)[1] = std::get<0>(inps)[1];
#pragma ForSyDe end
}

#endif
