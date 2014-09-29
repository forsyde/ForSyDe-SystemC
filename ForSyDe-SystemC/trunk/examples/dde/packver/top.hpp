/**********************************************************************
    * top.hpp -- the top module and testbench for the packet verifier *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple DDE system.                  *
    *                                                                 *
    * Usage:   Packet Verifier example                                *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "report.hpp"
#include "splitter.hpp"
#include <iostream>

using namespace ForSyDe;

SC_MODULE(top)
{
    DDE::signal<int> si, s1, s2;
    DDE::signal<char> sp1, sp2, so1, so2, sf;
    
    SC_CTOR(top)
    {
        DDE::make_vsource("inputs", {4, 8, -3}, 
            {sc_time(10,SC_MS), sc_time(40,SC_MS), sc_time(60,SC_MS)}, si
        );
        
        auto splitter1 = new splitter("splitter1");
        splitter1->iport1(sf);
        splitter1->iport2(si);
        splitter1->oport1(s1);
        splitter1->oport2(s2);
        
        DDE::make_comb("pv1", pv_func, sp1, s1);
        
        DDE::make_comb("pv2", pv_func, sp2, s2);
        
        auto merge1 = DDE::make_comb2("merge1", merge_func, so1, sp1, sp2);
        merge1->oport1(so2);
        
        DDE::make_delay("delay1", abst_ext<char>(), sc_time(15,SC_MS),
            sf, so2
        );
        
        DDE::make_sink("report1", report_func, so1);
    }
    
    static void pv_func(abst_ext<char>& out, const int& inp)
    {
        out = abst_ext<char>(inp>=0 ? 'V' : 'F');
    }
    
    static void merge_func(abst_ext<char>& out, const abst_ext<char>& inp1, const abst_ext<char>& inp2)
    {
        out = unsafe_from_abst_ext(inp1)=='F' || unsafe_from_abst_ext(inp2)=='F'?
            abst_ext<char>('F'):
            abst_ext<char>('V');
    }
    
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
