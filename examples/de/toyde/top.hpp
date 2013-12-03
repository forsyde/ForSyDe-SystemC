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
#include "add.hpp"
#include <iostream>

using namespace ForSyDe;

SC_MODULE(top)
{
    DE::signal<int> srca, feedback, addi1, addi2, result;
    
    SC_CTOR(top)
    {
        DE::make_delay("delay1", 0, sc_time(10, SC_NS), srca, feedback);
        
        auto inc1 = DE::make_comb("inc1", inc_func, addi1, srca);
        inc1->oport1(feedback);
        
        DE::make_constant("const1", -1, sc_time(100,SC_NS), addi2);
        
        DE::make_comb2("add1", add_func, result, addi1, addi2);
        
        DE::make_sink("report1", report_func, result);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
