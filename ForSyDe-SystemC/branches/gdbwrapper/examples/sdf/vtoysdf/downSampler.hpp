/**********************************************************************           
    * downSampler.hpp -- a 3:2 down sampler                             *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a variadic program.                   *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DOWNSAMPLER_HPP
#define DOWNSAMPLER_HPP

#include <vector>

using namespace std;

void downSampler_func(vector<float>& out1, const vector<float>& inp1)
{
  out1[0] = inp1[0];
  out1[1] = inp1[1];
}

#endif
