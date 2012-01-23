/**********************************************************************           
    * mux.hpp -- a three input multiplexer                            *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Sorter example                                         *
    *          inspired by material from Doulos SystemC course        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef MUX_HPP
#define MUX_HPP

#include <forsyde.hpp>

using namespace ForSyDe::SY;

int mux_func(int sel, int a, int b, int c)
{
    switch (sel)
    {
        case 0: return a;
        case 1: return b;
        case 2: return c;
        default: return a;
    }
}

#endif
