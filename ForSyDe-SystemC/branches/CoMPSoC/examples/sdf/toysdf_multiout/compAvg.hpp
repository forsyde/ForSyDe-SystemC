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
    
    comb2<float,float,std::tuple<std::vector<float>,std::vector<float>>> averager1;
    delayn<float> avginit;
    unzip<float,float> unzip1;
    
    SDF2SDF<float> din, dout;
    SDF2SDF<std::tuple<std::vector<float>,std::vector<float>>> zippedout;
    
    SC_CTOR(compAvg): averager1("averager1", averager_func, 3,2,1),
                      avginit("avginit1",0,2), unzip1("unzip1",2,2)
    {
        averager1.iport1(iport);
        averager1.iport2(dout);
        averager1.oport(zippedout);
        
        unzip1.iport(zippedout);
        unzip1.oport1(oport);
        unzip1.oport2(din);
        
        avginit.iport(din);
        avginit.oport(dout);
    }
};

#endif
