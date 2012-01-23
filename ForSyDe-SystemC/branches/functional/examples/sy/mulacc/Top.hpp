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
#include <iostream>

using namespace ForSyDe::SY;


int siggen_func(int inp)
{
    return -inp;
}

void report_func(int inp)
{
    std::cout << "output value: " << inp << std::endl;
}

SC_MODULE(Top)
{
    sc_fifo<int> srca, srcb, result;
    
    constant<int> const1;
    source<int> siggen1;
    mulacc mulacc1;
    sink<int> report1;
    
    SC_CTOR(Top): const1("const1",3), siggen1("siggen1", siggen_func, 1, 10),
                  mulacc1("mulacc1"), report1("report1", report_func)
    {
        const1.oport(srca);
        siggen1.oport(srcb);
        
        mulacc1.a(srca);
        mulacc1.b(srcb);
        mulacc1.result(result);
        
        report1.iport(result);
    }
};

