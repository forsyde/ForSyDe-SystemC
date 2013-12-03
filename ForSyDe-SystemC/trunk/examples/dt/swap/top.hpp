/**********************************************************************
    * top.hpp -- the top module and testbench for the swap example    *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
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

vector<tuple<unsigned int,int>> in_vec1 = 
    {make_tuple(0,1), make_tuple(1,2), make_tuple(2,3),
     make_tuple(3,4), make_tuple(4,5), make_tuple(5,6)};

SC_MODULE(top)
{
    DT::signal<int> src, result;
    
    SC_CTOR(top)
    {        
        DT::make_vsource("vsource1", in_vec1, src);
        
        DT::make_mealyT("swap1", swap_gamma, swap_ns_func, swap_od_func, 0, result, src);
        
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
