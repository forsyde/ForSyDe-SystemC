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
#pragma ForSyDe begin stimuli_func
    float _func(float inp)
    {
      return inp+1;
    }
#pragma ForSyDe end
};

class report : public sink<float>
{
public:
    report(sc_module_name _name) : sink<float>(_name){}
protected:
#pragma ForSyDe begin report_func
    void _func(float inp)
    {
        std::cout << "output value: " << inp << std::endl;
    }
#pragma ForSyDe end
};

SC_MODULE(Top)
{
    SDF2SDF<float> src, upsrc, res, downres;
    
    stimuli stimuli1;
    comb<float,float> upSampler1;
    comb<float,float> downSampler1;
    compAvg compAvg1;
    report report1;
    
    SC_CTOR(Top): stimuli1("stimuli1"),
                  upSampler1("upSampler1", upSampler_func,1,2),
                  downSampler1("downSampler1", downSampler_func,3,2), 
                  compAvg1("compAvg1"), report1("report1")
    {
        stimuli1.oport(src);
      
        upSampler1.iport(src);
        upSampler1.oport(upsrc);

        compAvg1.iport(upsrc);
        compAvg1.oport(res);

        downSampler1.iport(res);
        downSampler1.oport(downres);
        
        report1.iport(downres);
    }
public:
    void start_of_simulation()
    {
        ForSyDe::CoMPSoCExport dumper;
        dumper.traverse(this);
        dumper.printXML("gen/toysdf.xml");
        dumper.printSrc("./","gen/");
    }
};
