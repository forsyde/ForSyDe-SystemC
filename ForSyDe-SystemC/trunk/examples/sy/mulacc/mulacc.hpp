/**********************************************************************
    * mulacc.hpp -- a multiply-accumulate process                     *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple sequential processes.        *
    *                                                                 *
    * Usage:   MulAcc example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef MULACC_HPP
#define MULACC_HPP

#include <forsyde.hpp>
#include "mul.hpp"
#include "add.hpp"

using namespace ForSyDe::SY;

SC_MODULE(mulacc)
{
    SY_in<int>  a, b;
    SY_out<int> result;
    
    SY2SY<int> addi1, addi2, acci;
    
    SC_CTOR(mulacc)
    {
        make_comb2("mul1", mul_func, addi1, a, b);

        auto add1 = make_comb2("add1", add_func, acci, addi1, addi2);
        add1->oport(result);
        
        make_delay("accum", AbstExt<int>(0), addi2, acci);
    }
};

#endif
