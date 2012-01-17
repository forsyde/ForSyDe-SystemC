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
    
    comb2<float,float,float> averager1;
    delayn<float> avginit;
    
    SDF2SDF<float> din, dout;
    
    SC_CTOR(compAvg): averager1("averager1", averager_func, 3,2,2),
                      avginit("avginit1",0,2)
    {
        averager1.iport1(iport);
        averager1.iport2(dout);
        averager1.oport(oport);
        averager1.oport(din);
        
        avginit.iport(din);
        avginit.oport(dout);
    }
};

#endif
