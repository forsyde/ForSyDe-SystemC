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

class siggen : public source<int>
{
public:
    siggen(sc_module_name _name) : source<int>(_name, 1, 10){}
protected:
    int _func(int inp)
    {
        return -inp;
    }
    
};

class report : public sink<int>
{
public:
    report(sc_module_name _name) : sink<int>(_name){}
protected:
    void _func(int inp)
    {
        std::cout << "output value: " << inp << std::endl;
    }
    
};

SC_MODULE(Top)
{
    sc_fifo<int> srca, srcb, result;
    
    constant<int> const1;
    siggen siggen1;
    mulacc mulacc1;
    report report1;
    
    SC_CTOR(Top): const1("const1",3), siggen1("siggen1"),
                  mulacc1("mulacc1"), report1("report1")
    {
        const1.oport(srca);
        siggen1.oport(srcb);
        
        mulacc1.a(srca);
        mulacc1.b(srcb);
        mulacc1.result(result);
        
        report1.iport(result);
    }
};

