/**********************************************************************           
    * Top.hpp -- the Top process and testbench for the toy sdf example*
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "compAvg.hpp"
#include "upSampler.hpp"
#include "downSampler.hpp"
#include <iostream>

using namespace ForSyDe::SDF;

class stimuli : public source<float>
{
public:
    stimuli(sc_module_name _name) : source<float>(_name, 0){}
protected:
    float _func(float inp)
    {
      return inp+1;
    }
};

class report : public sink<float>
{
public:
    report(sc_module_name _name) : sink<float>(_name){}
protected:
    void _func(float inp)
    {
        std::cout << "output value: " << inp << std::endl;
    }
    
};

SC_MODULE(Top)
{
    sc_fifo<float> src, upsrc, res, downres;
    
    stimuli stim1;
    upSampler us1;
    downSampler ds1;
    compAvg ca1;
    report report1;
    
    SC_CTOR(Top): stim1("stim1"), us1("us1"), ds1("ds1"),
                  ca1("ca1"), report1("report1")
    {
        stim1.oport(src);
      
        us1.iport(src);
        us1.oport(upsrc);

        ca1.iport(upsrc);
        ca1.oport(res);

        ds1.iport(res);
        ds1.oport(downres);
        
        report1.iport(downres);
    }
};
