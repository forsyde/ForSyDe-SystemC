/**********************************************************************           
    * downSampler.hpp -- a 3:2 down sampler                           *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
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

void downSampler_func(std::vector<float>& out1, const std::vector<float>& inp1)
{
#pragma ForSyDe begin downSampler_func
    out1[0] = inp1[0];
    out1[1] = inp1[1];
#pragma ForSyDe end
}

#endif
