/**********************************************************************           
    * sorter.hpp -- the top level module of a sorter                  *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Sorter example                                         *
    *          inspired by material from Doulos SystemC course        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef SORTER_HPP
#define SORTER_HPP

#include <forsyde.hpp>
#include "comparator.hpp"
#include "mux.hpp"
#include "decoder.hpp"

using namespace ForSyDe::SY;

SC_MODULE(sorter)
{
    SY_in<int>  a, b, c;
    SY_out<int> biggest;
    
    SY2SY<int> c11, c12, c21, c22, c31, c32,
                 m1, m2, m3, m4;
    SY2SY<bool> dec1, dec2, dec3;
    
    SC_CTOR(sorter)
    {
        auto foa = make_fanout("foa", c11, a);
        foa->oport1(c32);
        foa->oport1(m2);
        
        auto fob = make_fanout("fob", c12, b);
        fob->oport1(c21);
        fob->oport1(m3);
        
        auto foc = make_fanout("foc", c22, c);
        foc->oport1(c31);
        foc->oport1(m4);
        
        make_comb2("comparator1", comparator_func, dec1, c11, c12);
        
        make_comb2("comparator2", comparator_func, dec2, c21, c22);
        
        make_comb2("comparator3", comparator_func, dec3, c31, c32);
        
        make_comb3("decoder1", decoder_func, m1, dec1, dec2, dec3);
        
        make_comb4("mux1", mux_func, biggest, m1, m2, m3, m4);
    }
};

#endif
