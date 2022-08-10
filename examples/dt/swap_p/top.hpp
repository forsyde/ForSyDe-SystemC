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
    // {make_tuple(0,1), make_tuple(1,2), make_tuple(2,3),
    //  make_tuple(3,4), make_tuple(4,5), make_tuple(5,6)};
    {make_tuple(0,1), make_tuple(1,2), make_tuple(2,3),
     make_tuple(3,4), make_tuple(4,5), make_tuple(5,5),
     make_tuple(6,5), make_tuple(7,5), make_tuple(8,4),
     make_tuple(9,3), make_tuple(10,2), make_tuple(11,1),
     make_tuple(12,1), make_tuple(13,1), make_tuple(14,2),
     make_tuple(15,2), make_tuple(16,3), make_tuple(17,3),
     make_tuple(18,4), make_tuple(19,4)};

SC_MODULE(top)
{
    DT::signal<int> src, result;
    
    SC_CTOR(top)
    {        
        DT::make_vsource("vsource1", in_vec1, src);
        
        // DT::P::make_mealy("swap1", swap_gamma, swap_ns_func, swap_od_func, 0, result, src);
        auto swap1 = new DT::P::mealyMN<tuple<int>,tuple<int>,int>("swap1", swap_gamma, swap_ns_func, swap_od_func, 0);
        get<0>(swap1->iport)(src);
        get<0>(swap1->oport)(result);
        
        DT::make_sink("report1", report_func, result);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
