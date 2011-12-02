/**********************************************************************           
    * compAvg.hpp -- A composite process which includes an averager   *
    *          with a delay.                                          *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a variadic program using zip and unzip*
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef COMPAVG_HPP
#define COMPAVG_HPP

#include <forsyde.hpp>
#include "averager.hpp"
#include <vector>
#include <tuple>

std::vector<uint> itoks = {3,2};

using namespace ForSyDe::SDF;
using namespace std;

SC_MODULE(compAvg)
{
    sc_fifo_in<float>  iport;
    sc_fifo_out<float> oport;
    
    zipN<float,float> zip1;
    averager avg1;
    unzipN<float,float> unzip1;
    delayn<float> avginit;
    
    sc_fifo<float> din, dout;
    sc_fifo< tuple<vector<float>,vector<float>> > zi, zo;
    
    SC_CTOR(compAvg): zip1("zip1",itoks), avg1("avg1"), unzip1("unzip1"),
                      avginit("avginit1",0,2)
    {
        get<0>(zip1.iport)(iport);
        get<1>(zip1.iport)(dout);
        zip1.oport(zi);
        
        avg1.iport(zi);
        avg1.oport(zo);
        
        unzip1.iport(zo);
        get<0>(unzip1.oport)(oport);
        get<1>(unzip1.oport)(din);
        
        avginit.iport(din);
        avginit.oport(dout);
    }
};

#endif
