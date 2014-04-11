/**********************************************************************           
    * Top.hpp -- the Top process and testbench for the sorter example *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Sorter example                                         *
    *          inspired by material from Doulos SystemC course        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "sorter.hpp"
#include "report.hpp"
#include <iostream>

using namespace ForSyDe::SY;

SC_MODULE(Top)
{
    SY2SY<int> srca, srcb, srcc, biggest;
    
    SC_CTOR(Top)
    {
        make_constant("constant1", abst_ext<int>(5), 1, srca);
        
        make_constant("constant2", abst_ext<int>(7), 1, srcb);
        
        make_constant("constant3", abst_ext<int>(3), 1, srcc);
        
        auto sorter1 = new sorter("sorter1");
        sorter1->a(srca);
        sorter1->b(srcb);
        sorter1->c(srcc);
        sorter1->biggest(biggest);
        
        make_sink("sink1", report_func, biggest);
    }
};
