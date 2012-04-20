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


void siggen_func(AbstExt<int>& out1, const AbstExt<int>& inp)
{
    int inp1 = inp.fromAbstExt(0);
#pragma ForSyDe begin siggen_func
    out1 = -inp1;
#pragma ForSyDe end
}

void report_func(AbstExt<int> inp1)
{
#pragma ForSyDe begin report_func
    std::cout << "output value: " << inp1 << std::endl;
#pragma ForSyDe end
}

SC_MODULE(Top)
{
    SY2SY<int> srca, srcb, result;
    
    constant<AbstExt<int>> const1;
    mulacc mulacc1;
    
    SC_CTOR(Top): const1("const1",AbstExt<int>(3)),
                  mulacc1("mulacc1")
    {
        const1.oport(srca);
        make_source("siggen1", siggen_func, AbstExt<int>(1), 10, srcb);
        
        mulacc1.a(srca);
        mulacc1.b(srcb);
        mulacc1.result(result);
        
        make_sink("report1", report_func, result);
    }
};
