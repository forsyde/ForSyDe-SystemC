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


AbstExt<int> siggen_func(AbstExt<int> inp)
{
    AbstExt<int> temp(-inp.fromAbstExt(0));
    return temp;
}

void report_func(AbstExt<int> inp)
{
    std::cout << "output value: " << inp << std::endl;
}

SC_MODULE(Top)
{
    SY2SY<int> srca, srcb, result;
    
    constant<AbstExt<int>> const1;
    source<AbstExt<int>> siggen1;
    mulacc mulacc1;
    sink<AbstExt<int>> report1;
    
    SC_CTOR(Top): const1("const1",AbstExt<int>(3)),
                  siggen1("siggen1", siggen_func, AbstExt<int>(1), 10),
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
