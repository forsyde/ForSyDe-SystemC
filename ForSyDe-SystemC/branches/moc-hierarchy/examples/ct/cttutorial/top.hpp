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

#include "globals.hpp"
#include "add.hpp"

using namespace sc_core;
using namespace ForSyDe;

SC_MODULE(Top)
{
    CT::signal cosSrc, NoiseSrc1, NoiseSrc2, filtInp, filtOut;
    
    SC_CTOR(Top)
    {
        
        CT::make_cosine("cosine1", endT, CosPeriod, 1.0, cosSrc);
        
        //~ make_cosine("cosine2", endT, CosPeriod/10, 0.1, NoiseSrc1);
        CT::make_gaussian("gaussian1", 0.01, 0, sc_time(1, SC_MS), NoiseSrc1);
        
        auto add1 = CT::make_comb2("add1", add_func, filtInp, cosSrc, NoiseSrc1);
        add1->oport1(NoiseSrc2);
        
        CT::make_filter("filter1", nums, dens, samplingPeriod, filtOut, filtInp);
        
        CT::make_traceSig("report2", sc_time(100,SC_US), NoiseSrc2);
        
        CT::make_traceSig("report1", sc_time(100,SC_US), filtOut);
    }
   
};
