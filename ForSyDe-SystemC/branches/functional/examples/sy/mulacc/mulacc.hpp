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
    sc_fifo_in<int>  a, b;
    sc_fifo_out<int> result;
    
    SY2SY<int> addi1, addi2, acci;
    
    SC_CTOR(mulacc)
    {
        make_comb2("mul1", mul_func, a, b, addi1);

        auto add1 = make_comb2("add1", add_func, addi1, addi2, acci);
        (*add1).oport(result);
        
        make_delay("accum",0,acci,addi2);
    }
};

#endif
