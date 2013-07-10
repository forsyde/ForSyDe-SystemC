/**********************************************************************
    * ramp.hpp -- function used to create a ramp input                *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple example in the untimed MoC.  *
    *                                                                 *
    * Usage:   amplifier example                                      *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef RAMP_HPP
#define RAMP_HPP

#include <forsyde.hpp>

using namespace ForSyDe::UT;


void ramp_func(int& out1, const int& inp1)
{
    out1 = inp1 + 1;
}

#endif
