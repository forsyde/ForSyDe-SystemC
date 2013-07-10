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

#include "codec.hpp"
#include <iostream>

using namespace ForSyDe::SY;
using namespace std;

class siggen : public source<int>
{
public:
    siggen(sc_module_name _name) : source<int>(_name, 1, 10){}
protected:
    int _func(int inp)
    {
        return inp+1;
    }
    
};

class codegen : public source<int>
{
public:
    codegen(sc_module_name _name) : source<int>(_name, 1, 10){}
protected:
    int _func(int inp)
    {
        return inp+1;
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
    sc_fifo<int> srcval, srccode, result;
    
    siggen siggen1;
    codegen codegen1;
    codec codec1;
    report report1;
    
    SC_CTOR(Top): siggen1("siggen1"), codegen1("codegen1"),
                  codec1("codec1"), report1("report1")
    {
        siggen1.oport(srcval);
        codegen1.oport(srccode);
        
        codec1.iport(srcval);
        codec1.code(srccode);
        codec1.oport(result);
        
        report1.iport(result);
    }
};

