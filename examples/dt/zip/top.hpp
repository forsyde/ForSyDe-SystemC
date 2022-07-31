/**********************************************************************
    * top.hpp -- the top module and testbench for the swap example    *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a the usage of mealyT.                *
    *                                                                 *
    * Usage:   swap example                                           *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "swap.hpp"
#include "report.hpp"
#include <iostream>

using namespace std;
using namespace ForSyDe;

vector<tuple<size_t,int>> in_vec1 = 
    {make_tuple(0,1), make_tuple(1,2), make_tuple(2,3),
     make_tuple(3,4), make_tuple(4,5), make_tuple(5,6)};
vector<tuple<size_t,int>> in_vec2 = 
    {make_tuple(0,6), make_tuple(1,5), make_tuple(2,4),
     make_tuple(3,3), make_tuple(4,2), make_tuple(5,1)};
vector<tuple<size_t,int>> in_vecc = 
    {make_tuple(0,1), make_tuple(1,2), make_tuple(3,0),
     make_tuple(5,0)};

SC_MODULE(top)
{
    DT::signal<int> src1, src2, src1p, src2p, srcc, src1pp, src2pp, report3, report4;
    DT::signal<tuple<vector<abst_ext<int>>,vector<abst_ext<int>>>> zipped1, zipped2;
    
    SC_CTOR(top)
    {        
        auto vsource1 = DT::make_vsource("vsource1", in_vec1, src1);
        vsource1->oport1(src1p);
        vsource1->oport1(src1pp);
        auto vsource2 = DT::make_vsource("vsource2", in_vec2, src2);
        vsource2->oport1(src2p);
        vsource2->oport1(src2pp);
        DT::make_vsource("vsourcec", in_vecc, srcc);
        
        DT::make_zips("zips1", 3, zipped1, src1, src2);
        auto zip1_gamma = [] (auto& itoks, const auto& k) {itoks = k+1;};
        DT::make_zip("zip1", zip1_gamma, zipped2, src1p, src2p, srcc);

        auto swap1 = new DT::mealyMN<tuple<int,int>,tuple<int,int>,tuple<int>>("swap1", swap_gamma, swap_ns_func, swap_od_func, make_tuple(0));
        get<0>(swap1->iport)(src1pp);
        get<1>(swap1->iport)(src2pp);
        get<0>(swap1->oport)(report3);
        get<1>(swap1->oport)(report4);
        
        DT::make_sink("report1", report1_func, zipped1);
        DT::make_sink("report2", report2_func, zipped2);
        DT::make_sink("report3", report3_func, report3);
        DT::make_sink("report4", report4_func, report4);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
