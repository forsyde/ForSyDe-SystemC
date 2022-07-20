#include <forsyde.hpp>
#include "report.hpp"
#include "ramp.hpp"
#include "controller.hpp"

using namespace sc_core;
using namespace ForSyDe;


SC_MODULE(top)
{
    UT::signal<int> from_source, from_constant, from_kernels;
    UT::signal<unsigned int> zip_control;
    UT::signal<std::tuple<std::vector<int>,std::vector<std::tuple<unsigned int, unsigned int, scenrio_func>>>> from_zip;
    UT::signal<std::tuple<unsigned int, unsigned int, scenrio_func>> from_detector, from_detector2; 

    SC_CTOR(top)
    {        
        
        UT::make_source("ramp1", ramp_func, 1, 20, from_source);

        UT::make_constant("constant1", 1, 0, from_constant);
        auto detector = UT::make_mealy("detector", 
                        gamma_detector_func,
                        next_state_detector_func,
                        output_decode_detector_func,
                        0,
                        from_detector,
                        from_constant
                        );

        detector->oport1(from_detector2);
                        
        UT::make_comb("cextract",[](std::vector<unsigned int>& out,const std::vector<std::tuple<unsigned int, unsigned int, scenrio_func>>& inp){
            out.resize(1);
            out[0] = std::get<0>(inp[0]);
        }, 1, zip_control, from_detector2);
        
        // auto zipU1 = new UT::zipU <int, std::tuple<unsigned int, unsigned int, scenrio_func>, unsigned int>("zipU1", gamma_func_zipa, gamma_func_zipb);
        // zipU1->iport1 (from_source);   
        // zipU1->iport2 (from_detector);
        // zipU1->controlport (zip_control);
        // zipU1->oport1 (from_zip);
        UT::make_zipU("zipU1", gamma_func_zipa, gamma_func_zipb, from_zip, from_source, from_detector, zip_control);
        UT::make_comb ("kernels",
                        kernel_func,
                        1,
                        from_kernels,
                        from_zip
                        );

        UT::make_sink("sink", report_func, from_kernels);
    
    }

};

