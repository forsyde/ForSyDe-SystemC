/**********************************************************************           
    * top.hpp -- the Top process and testbench for the toy sdf example*
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
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
#include "report.hpp"
#include "stimuli.hpp"
#include <iostream>

using namespace ForSyDe;

SC_MODULE(top)
{
    SDF::signal<double> src, src2, upsrc, res, downres;
    SDF::signal<std::tuple<std::vector<double>,std::vector<double>>> zipped_res;
    
    SC_CTOR(top)
    {
        auto stimuli1 = SDF::make_file_source("stimuli1", stimuli_func, 
            "input.txt", src
        );
        stimuli1->oport1(src2);
      
        SDF::make_comb("upSampler1", upSampler_func, 2, 1, upsrc, src);

        auto compAvg1 = new compAvg("compAvg1");
        compAvg1->iport1(upsrc);
        compAvg1->oport1(res);

        SDF::make_comb("downSampler1", downSampler_func, 2, 3, downres, res);
        
        SDF::make_zip("zip1", 2, 1, zipped_res, src2, downres);
        
        SDF::make_file_sink("report1", report_func, "output.txt", zipped_res);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
