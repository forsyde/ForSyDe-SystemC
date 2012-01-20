/**********************************************************************           
    * Top.hpp -- the Top process and testbench for a tutorial CT exmpl*
    *                                                                 *
    * Authors:  Hosien Attarzadeh (shan2@kth.se)                      *
    *           Jun Zhu (junz@kth.se)                                 *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Tutorial CT example                                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>

using namespace sc_core;
using namespace ForSyDe::CT;

// Period of the cosine wave
sc_time CosPeriod = sc_time(200,SC_MS);
// The end time of the cosine wave signal
sc_time endT = sc_time(1.0,SC_SEC);

double cutoffFreq = 2.0/(CosPeriod.to_seconds());
// Sampling period of the solver for filter
sc_time samplingPeriod = sc_time(100,SC_US);

std::vector<CTTYPE> nums = {1.0};
std::vector<CTTYPE> dens = {1.0/(M_PI*cutoffFreq), 1.0};


SC_MODULE(Top)
{
    sc_fifo<SubSignal> cosSrc, NoiseSrc1, NoiseSrc2, filtInp, filtOut;
    
    cosine      cos1;
    sine        noise1;
    add         add1;
    filter      filter1;
    traceSigs   report1;
    
    SC_CTOR(Top): cos1("cos1",endT,CosPeriod,1.0), 
                  noise1("noise1",endT,CosPeriod/10,0.1), 
                  add1("add1"),
                  filter1("filter1", nums, dens, samplingPeriod),
                  report1("report1", sc_time(100,SC_US))
    {
        
        cos1.oport(cosSrc);
        
        noise1.oport(NoiseSrc1);
        
        add1.iport1(cosSrc);
        add1.iport2(NoiseSrc1);
        add1.oport(filtInp);
        add1.oport(NoiseSrc2);
        
        filter1.iport(filtInp);
        filter1.oport(filtOut);
        
        report1.iport(filtOut);
        report1.iport(NoiseSrc2);
    }
   
};
