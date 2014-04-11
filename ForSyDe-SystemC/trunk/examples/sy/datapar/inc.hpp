/**********************************************************************
    * inc.hpp -- a data parallel increment process                    *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a data parallel model.                *
    *                                                                 *
    * Usage:   datapar example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef INC_HPP
#define INC_HPP

#include <forsyde.hpp>

using namespace ForSyDe;

void inc_func(int& out1, const int& inp1)
{

#pragma ForSyDe begin add_func 
    out1 = inp1 + 1;
#pragma ForSyDe end
}

#endif
