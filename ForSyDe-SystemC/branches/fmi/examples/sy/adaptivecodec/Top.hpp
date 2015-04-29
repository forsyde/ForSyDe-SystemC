/**********************************************************************
    * Top.hpp -- the top module and testbench for the mulacc example  *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple sequential processes.        *
    *                                                                 *
    * Usage:   MulAcc example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "codec.hpp"

#include "forsyde.hpp"

#include <iostream>

using namespace ForSyDe::SY;
using namespace std;

void siggen_func(abst_ext<int>& out1, const abst_ext<int>& inp)
{
    int inp1 = from_abst_ext(inp,0);
#pragma ForSyDe begin siggen_func
    out1 = inp1 + 1;
#pragma ForSyDe end
}

void codegen_func(abst_ext<int>& out1, const abst_ext<int>& inp)
{
    int inp1 = from_abst_ext(inp,0);
#pragma ForSyDe begin siggen_func
    out1 = inp1 + 1;
#pragma ForSyDe end
}

void report_func(abst_ext<int> inp1)
{
#pragma ForSyDe begin report_func
    std::cout << "output value: " << inp1 << std::endl;
#pragma ForSyDe end
}

SC_MODULE(Top)
{
    SY2SY<int> srcval, srccode, result;
    
    SC_CTOR(Top)
    {
        make_source("siggen1", siggen_func, abst_ext<int>(1), 10, srcval);
        
        make_source("codegen1", codegen_func, abst_ext<int>(1), 10, srccode);
        
        auto codec1 = new codec("codec1");
        codec1->iport(srcval);
        codec1->code(srccode);
        codec1->oport(result);
        
        make_sink("report1", report_func, result);
    }
};

