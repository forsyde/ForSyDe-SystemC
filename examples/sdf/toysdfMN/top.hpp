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
    SDF::signal<double> src, upsrc, res, downres;
    SDF::signal<int> cnt, cnt_delay;
    
    SC_CTOR(top)
    {
        SDF::make_combMN("stimuli1",
                            stimuli_func,
                            {1,1},
                            {1},
                            tie(src, cnt),
                            tie(cnt_delay)
        );
        SDF::make_delay("src_delay1", 0, cnt_delay, cnt);
        // SDF::make_source("stimuli1", stimuli_func, 0.0, 20, src);
      
        SDF::make_combMN("upSampler1",
                        upSampler_func,
                        {2},
                        {1},
                        tie(upsrc),
                        tie(src)
        );
        // auto upSampler1 = new SDF::combMN<std::tuple<double>,std::tuple<double>>("upSampler1", upSampler_func, {2}, {1});
        // std::get<0>(upSampler1->iport)(src);
        // std::get<0>(upSampler1->oport)(upsrc);

        auto compAvg1 = new compAvg("compAvg1");
        compAvg1->iport1(upsrc);
        compAvg1->oport1(res);

        SDF::make_combMN("downSampler1",
                            downSampler_func,
                            {2},
                            {3},
                            tie(downres),
                            tie(res)
        );
        // auto downSampler1 = new SDF::combMN<std::tuple<double>,std::tuple<double>>("downSampler1", downSampler_func, {2}, {3});
        // std::get<0>(downSampler1->iport)(res);
        // std::get<0>(downSampler1->oport)(downres);
        
        SDF::make_combMN("report1",
                        report_func,
                        {},
                        {1},
                        tie(),
                        tie(downres)
        );
        // SDF::make_sink("report1", report_func, downres);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
