/**********************************************************************           
    * compAvg.hpp -- A composite process which includes an averager   *
    *          with a delay.                                          *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef COMPAVG_HPP
#define COMPAVG_HPP

#include <forsyde.hpp>
#include "averager.hpp"

using namespace ForSyDe::SDF;

SC_MODULE(compAvg)
{
    sc_fifo_in<float>  iport;
    sc_fifo_out<float> oport;
    
    averager avg1;
    delayn<float> avginit;
    
    sc_fifo<float> din, dout;
    
    SC_CTOR(compAvg): avg1("avg1"), avginit("avginit1",0,2)
    {
        avg1.iport1(iport);
        avg1.iport2(dout);
        avg1.oport(oport);
        avg1.oport(din);
        
        avginit.iport(din);
        avginit.oport(dout);
    }
};

#endif
