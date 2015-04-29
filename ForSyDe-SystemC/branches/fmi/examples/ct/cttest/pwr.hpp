/**********************************************************************           
    * pwr.hpp -- a process that computes square of a signal           *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy CT example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef PWR_HPP
#define PWR_HPP

#include <forsyde.hpp>

using namespace ForSyDe;
using namespace ForSyDe::CT;

void pwr_func(CTTYPE& out1, const CTTYPE& inp1)
{
#pragma ForSyDe begin pwr_func
    out1 = inp1 * inp1;
#pragma ForSyDe end
}

#endif

