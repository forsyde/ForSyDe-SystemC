/**********************************************************************
    * bit_src.hpp -- a random bit generator                           *
    *                                                                 *
    * Author:  Gilmar Beserra (gilmar@kth.se)                         *
    *          Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: A BASK modulator system.                               *
    *                                                                 *
    * Usage:   bask_mod example                                       *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef BITSRC_HPP
#define BITSRC_HPP

#include <forsyde.hpp>

using namespace ForSyDe;
using namespace ForSyDe::SY;

void bit_src_func(abst_ext<double>& outp, const abst_ext<double>& inp)
{
    int out1;
#pragma ForSyDe begin bit_src_func 
    out1 = (double)(std::rand()%2);
#pragma ForSyDe end
    outp = abst_ext<double>(out1);
}

#endif
