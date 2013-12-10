/**********************************************************************           
    * Top.hpp -- the Top process and testbench for the toy sdf example*
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a variadic program using zip and unzip*
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>
#include "compAvg.hpp"
#include "upSampler.hpp"
#include "downSampler.hpp"
#include <iostream>

using namespace ForSyDe;

void stimuli_func(float& out1, const float& inp1)
{
  out1 = inp1+1;
}

void report_func(float inp1)
{
    std::cout << "output value: " << inp1 << std::endl;
}

SC_MODULE(Top)
{
    SDF::signal<float> src, upsrc, res, downres;
        
    SC_CTOR(Top)
    {
        SDF::make_source("stimuli1", stimuli_func, (float)0, 100, src);
      
        SDF::make_comb("upSampler1", upSampler_func, 2, 1, upsrc, src);

        auto compAvg1 = new compAvg("compAvg1");
        compAvg1->iport(upsrc);
        compAvg1->oport(res);

        SDF::make_comb("downSampler1", downSampler_func, 2, 3, downres, res);
        
        SDF::make_sink("report1", report_func, downres);
    }
};
