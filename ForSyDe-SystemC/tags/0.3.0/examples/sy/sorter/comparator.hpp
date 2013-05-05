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

using namespace ForSyDe;
using namespace ForSyDe::SY;

void comparator_func(abst_ext<bool>& out1,
              const abst_ext<int>& a, const abst_ext<int>& b)
{
    int inp1 = a.from_abst_ext(0);
    int inp2 = b.from_abst_ext(0);

#pragma ForSyDe begin comparator_func  
    out1 = inp1>inp2 ? true : false;
#pragma ForSyDe end
}

#endif
