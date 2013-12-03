/**********************************************************************
    * Top.hpp -- the top module and testbench for the mulacc example  *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple sequential processes.        *
    *                                                                 *
    * Usage:   MulAcc example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "mulacc.hpp"
#include "report.hpp"
#include "siggen.hpp"
#include <iostream>

using namespace ForSyDe;

SC_MODULE(top)
{
    SY::signal<int> srca, srcb, result;
    
    SC_CTOR(top)
    {
        SY::make_constant("constant1", abst_ext<int>(3), 10, srca);
        
        SY::make_source("siggen1", siggen_func, abst_ext<int>(1), 10, srcb);
        
        auto mulacc1 = new mulacc("mulacc1");
        mulacc1->a(srca);
        mulacc1->b(srcb);
        mulacc1->result(result);
        
        SY::make_sink("report1", report_func, result);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
