/**********************************************************************           
    * compAvg.hpp -- A composite process which includes an averager   *
    *          with a delay.                                          *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
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

using namespace ForSyDe;

SC_MODULE(compAvg)
{
    SDF::in_port<double>  iport1;
    SDF::out_port<double> oport1;
    
    SDF::signal<double> din, dout;
    
    SC_CTOR(compAvg)
    {
        auto averager1 = SDF::make_comb2("averager1", averager_func, 2,3,2, oport1, iport1, dout);
        averager1->oport1(din);
        
        SDF::make_delayn("avginit1",0.0,2, dout, din);
    }
};

#endif
