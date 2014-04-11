/**********************************************************************           
    * top.hpp -- the Top process and testbench for the digital filter *
    *                                                                 *
    * Authors: Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a heterogeneous system.               *
    *                                                                 *
    * Usage:   The digital filter example                             *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>

#include "globals.hpp"
#include "ctadd.hpp"
#include "fir.hpp"

using namespace sc_core;
using namespace ForSyDe;
using namespace ForSyDe::CT;

SC_MODULE(top)
{
    CT2CT cosSrc, NoiseSrc1, NoiseSrc2, filtInp, filtOut;
    SY::SY2SY<double> dig_in, dig_out;
    
    SC_CTOR(top)    
    {
        
        make_cosine("cosine1", endT, CosPeriod, 1.0, cosSrc);
        
        make_gaussian("gaussian1", 0.01, 0, sc_time(1, SC_MS), NoiseSrc1);
        
        auto ctadd1 = make_comb2("ctadd1", ctadd_func, filtInp, cosSrc, NoiseSrc1);
        ctadd1->oport1(NoiseSrc2);
        
        make_CT2SY("a2d", samplingPeriod, dig_in, filtInp);
        
        auto fir1 = new fir("fir1");
        fir1->iport1(dig_in);
        fir1->oport1(dig_out);
        
        make_SY2CT("d2a", samplingPeriod, LINEAR, filtOut, dig_out);
                
        make_traceSig("report1", sc_time(100,SC_US), filtOut);
        
        make_traceSig("report2", sc_time(100,SC_US), NoiseSrc2);
    }
   
};
