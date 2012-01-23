/**********************************************************************           
    * sorter.hpp -- the top level module of a sorter                  *
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


#ifndef SORTER_HPP
#define SORTER_HPP

#include <forsyde.hpp>
#include "comparator.hpp"
#include "mux.hpp"
#include "decoder.hpp"

using namespace ForSyDe::SY;

SC_MODULE(sorter)
{
    sc_fifo_in<int>  a, b, c;
    sc_fifo_out<int> biggest;
    
    comb2<int,int,bool> comp1, comp2, comp3;
    comb3<bool,bool,bool,int> decoder1;
    comb4<int,int,int,int,int> mux1;
    fanout<int> foa, fob, foc;
    
    sc_fifo<int> c11, c12, c21, c22, c31, c32,
                 m1, m2, m3, m4;
    sc_fifo<bool> dec1, dec2, dec3;
    
    SC_CTOR(sorter): comp1("comp1", comparator_func),
                     comp2("comp2", comparator_func),
                     comp3("comp3", comparator_func),
                     decoder1("decoder1",decoder_func),
                     mux1("mux1", mux_func),
                     foa("foa"), fob("fob"), foc("foc")
    {
        foa.iport(a);
        foa.oport(c11);
        foa.oport(c32);
        foa.oport(m2);
        
        fob.iport(b);
        fob.oport(c12);
        fob.oport(c21);
        fob.oport(m3);
        
        foc.iport(c);
        foc.oport(c22);
        foc.oport(c31);
        foc.oport(m4);
        
        comp1.iport1(c11);
        comp1.iport2(c12);
        comp1.oport(dec1);
        
        comp2.iport1(c21);
        comp2.iport2(c22);
        comp2.oport(dec2);
        
        comp3.iport1(c31);
        comp3.iport2(c32);
        comp3.oport(dec3);
        
        decoder1.iport1(dec1);
        decoder1.iport2(dec2);
        decoder1.iport3(dec3);
        decoder1.oport(m1);
        
        mux1.iport1(m1);
        mux1.iport2(m2);
        mux1.iport3(m3);
        mux1.iport4(m4);
        mux1.oport(biggest);
    }
};

#endif
