/**********************************************************************
    * top.hpp -- the top module and testbench for the amplifier example*
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple example in the untimed MoC.  *
    *                                                                 *
    * Usage:   amplifier example                                      *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "amplifier.hpp"
#include "ramp.hpp"
#include "report.hpp"
#include <iostream>

using namespace ForSyDe;

SC_MODULE(top)
{
    UT::signal<int> src, result;
    
    SC_CTOR(top)
    {        
        UT::make_source("ramp1", ramp_func, 1, 20, src);
        
        auto amplifier1 = new amplifier("amplifier1");
        amplifier1->iport1(src);
        amplifier1->oport1(result);
        
        UT::make_sink("report1", report_func, result);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif

};
