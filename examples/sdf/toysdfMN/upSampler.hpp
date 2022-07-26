/**********************************************************************           
    * upSampler.hpp -- a 1:2 up sampler                                 *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef UPSAMPLER_HPP
#define UPSAMPLER_HPP

#include <forsyde.hpp>

using namespace std;

void upSampler_func(tuple<vector<double>>& outs,
                    const tuple<vector<double>>& inps)
{
#pragma ForSyDe begin upSampler_func
    std::get<0>(outs)[0] = std::get<0>(inps)[0];
    std::get<0>(outs)[1] = std::get<0>(inps)[0];
#pragma ForSyDe end
}

#endif
