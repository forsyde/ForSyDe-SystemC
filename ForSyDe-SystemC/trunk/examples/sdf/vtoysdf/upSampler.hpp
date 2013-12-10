/**********************************************************************           
    * upSampler.hpp -- a 1:2 up sampler                               *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a variadic program.                   *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef UPSAMPLER_HPP
#define UPSAMPLER_HPP

#include <vector>

using namespace std;

void upSampler_func(vector<float>& out1, const vector<float>& inp1)
{
  out1[0] = inp1[0];
  out1[1] = inp1[0];
}

#endif
