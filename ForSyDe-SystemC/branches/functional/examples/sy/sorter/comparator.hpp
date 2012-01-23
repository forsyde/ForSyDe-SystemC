/**********************************************************************           
    * comparator.hpp -- a simple two input comparator                 *
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

#ifndef COMPARATOR_HPP
#define COMPARATOR_HPP

#include <forsyde.hpp>

using namespace ForSyDe::SY;


bool comparator_func(int a, int b)
{
    return a>b ? true : false;
}

#endif
