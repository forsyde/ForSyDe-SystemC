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

#include "stimuli.hpp"
#include "compAvg.hpp"
#include "upSampler.hpp"
#include "downSampler.hpp"
#include <iostream>

using namespace ForSyDe::SDF;

class report : public sink<float>
{
public:
    report(sc_module_name _name) : sink<float>(_name){}
protected:
    void _func(float inp)
    {
#pragma ForSyDe begin report_func
        std::cout << "output value: " << inp << std::endl;
        // mon_debug_info(inp1[0]);
#pragma ForSyDe end
    }
};

SC_MODULE(Top)
{
    SDF2SDF<float> src_self1, src_self2, src, upsrc, res, downres;
    SDF2SDF<std::tuple<std::vector<float>,std::vector<float>>> zippedsrc;
    
    comb<float,std::tuple<std::vector<float>,std::vector<float>>> stimuli1;
    unzip<float,float> src_unzip1;
    delayn<float> src_self_del;
    comb<float,float> upSampler1;
    comb<float,float> downSampler1;
    compAvg compAvg1;
    report report1;
    
    SC_CTOR(Top): stimuli1("stimuli1", stimuli_func,1,1),
                  src_unzip1("src_unzip1", 1, 1),
                  src_self_del("src_self_del1", 0, 1),
                  upSampler1("upSampler1", upSampler_func,1,2),
                  downSampler1("downSampler1", downSampler_func,3,2), 
                  compAvg1("compAvg1"), report1("report1")
    {
        stimuli1.iport(src_self2);
        stimuli1.oport(zippedsrc);
        
        src_unzip1.iport(zippedsrc);
        src_unzip1.oport1(src);
        src_unzip1.oport2(src_self1);
        
        src_self_del.iport(src_self1);
        src_self_del.oport(src_self2);
      
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
