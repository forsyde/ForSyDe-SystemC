/**********************************************************************
    * top.hpp -- the top module and testbench for the toyde example   *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple DDE system.                  *
    *                                                                 *
    * Usage:   ToyDDE example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "report.hpp"
#include "inc.hpp"
#include "add.hpp"
#include "buf_add.hpp"
#include <iostream>

using namespace ForSyDe;

SC_MODULE(top)
{
    DDE::signal<int> srca, feedback, addi1, addi2, result, addi1p, addi2p, buf_result;
    DDE::signal<std::tuple<abst_ext<int>,abst_ext<int>>> zip_result;
    
    SC_CTOR(top)
    {
        DDE::make_delay("delay1", abst_ext<int>(0), sc_time(10, SC_NS), srca, feedback);
        
        auto inc1 = DDE::make_comb("inc1", inc_func, feedback, srca);
        inc1->oport1(addi1);
        inc1->oport1(addi1p);
        
        auto const1 = DDE::make_vsource("const1",
                std::vector<int>(1,7),
                std::vector<sc_time>(1,sc_time(50,SC_NS)),
                addi2
        );
        const1->oport1(addi2p);
        
        DDE::make_comb2("add1", add_func, result, addi1, addi2);
        
        DDE::make_mealy2("buf_add1", buf_add_ns_func, buf_add_od_func,
            std::make_tuple((int)0,(int)0),
            sc_time(0,SC_NS),
            buf_result, addi1p, addi2p);
        
        DDE::make_zip("zip1", zip_result, result, buf_result);
        
        DDE::make_sink("report1", report_func, zip_result);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
