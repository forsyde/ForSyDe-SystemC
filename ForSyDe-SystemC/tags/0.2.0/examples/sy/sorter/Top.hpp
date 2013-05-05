/**********************************************************************           
    * Top.hpp -- the Top process and testbench for the sorter example *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Sorter example                                         *
    *          inspired by material from Doulos SystemC course        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "sorter.hpp"
#include <iostream>

using namespace ForSyDe::SY;

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
    sc_fifo<int> srca, srcb, srcc, biggest;
    
    constant<int> const1, const2, const3;
    sorter sorter1;
    report report1;
    
    SC_CTOR(Top): const1("const1",5), const2("const2",7), const3("const3",3),
                  sorter1("sorter1"), report1("report1")
    {
        const1.oport(srca);
        const2.oport(srcb);
        const3.oport(srcc);
        
        sorter1.a(srca);
        sorter1.b(srcb);
        sorter1.c(srcc);
        sorter1.biggest(biggest);
        
        report1.iport(biggest);
    }
};
