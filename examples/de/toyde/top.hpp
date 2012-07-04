/**********************************************************************
    * top.hpp -- the top module and testbench for the toyde example   *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple DE system.                   *
    *                                                                 *
    * Usage:   ToyDE example                                          *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "report.hpp"
#include "inc.hpp"
#include <iostream>

using namespace ForSyDe::DE;

SC_MODULE(top)
{
    DE2DE<int> srca, feedback, result;
    
    SC_CTOR(top)
    {
        make_delay("delay1", 0, sc_time(10, SC_NS), srca, feedback);
        
        auto inc1 = make_comb("inc1", inc_func, result, srca);
        inc1->oport1(feedback);
        
        make_sink("report1", report_func, result);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
