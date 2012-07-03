/**********************************************************************           
    * upSampler.hpp -- a 1:2 up sampler                                 *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
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

void upSampler_func(std::vector<double>& out1,
                    const std::vector<double>& inp1)
{
#pragma ForSyDe begin upSampler_func
    out1[0] = inp1[0];
    out1[1] = inp1[0];
#pragma ForSyDe end
}

#endif
