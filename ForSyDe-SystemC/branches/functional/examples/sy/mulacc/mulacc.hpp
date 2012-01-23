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
    
    comb2<int,int,int> mul1;
    comb2<int,int,int> add1;
    delay<int> accum;
    
    sc_fifo<int> addi1, addi2, acci;
    
    SC_CTOR(mulacc): mul1("mul1", mul_func), add1("add1", add_func),
                     accum("accum", 0)
    {
        mul1.iport1(a);
        mul1.iport2(b);
        mul1.oport(addi1);
        
        add1.iport1(addi1);
        add1.iport2(addi2);
        add1.oport(acci);
        //~ comb2<int,int,int> add1 = make_comb2<int,int,int>("add1", add_func, addi1, addi2, acci);
        //~ comb2<int,int,int> add1 = make_comb2("add1", add_func, addi1, addi2, acci);
        add1.oport(result);
        
        //~ auto accum = make_delay("accum",0,acci,addi2);
        accum.iport(acci);
        accum.oport(addi2);
    }
};

#endif
