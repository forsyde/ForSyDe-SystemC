/**********************************************************************
    * stimuli.hpp -- a stimuli generator                              *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef STIMULI_HPP
#define STIMULI_HPP

#include <forsyde.hpp>

using namespace ForSyDe::SDF;

void stimuli_func(double& out1, const double& inp1)
{
#pragma ForSyDe begin stimuli_func
    out1 = inp1 + 1;
#pragma ForSyDe end
}

#endif
