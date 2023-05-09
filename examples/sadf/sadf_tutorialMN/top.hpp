/**********************************************************************
    * main.cpp -- the main file and testbench for the SADF tutorial   *
    *                                                                 *
    * Author:  Mohammad Vazirpanah (mohammad.vazirpanah@yahoo.com)    *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   SADF Tutorial                                          *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>
#include "kernels.hpp"
#include "detectors.hpp"
#include "globals.hpp"

using namespace sc_core;
using namespace ForSyDe;
using namespace std;

SC_MODULE(top)
{
    SADF::signal<int> from_source;
    SADF::signal<int> to_kernel1, from_kernel1, to_kernel2, from_kernel2;
#ifdef FORSYDE_SELF_REPORTING
    // Communication pipes
    FILE* report_pipe;      // Report pipe
    int report_pipe_fd;     // Report pipe file descriptor
#endif

    SC_CTOR(top)
    {

        auto from_detector1 = new SADF::signal<kernel1_scenario_type>("from_detector1",1);
        auto from_detector2 = new SADF::signal<kernel2_scenario_type>("from_detector2",1);

        //! < -------------------------------- Using Helper--------------------------------> //!

        SADF::make_detectorMN(
                                "detector1",
                                detector1_cds_func,
                                detector1_kss_func,
                                detector1_table,
                                S1,
                                {1},
                                tie(*from_detector1,*from_detector2),
                                tie(from_source)
                            );
        
        SADF::make_kernelMN("kernel1",
                            kernel1_func,
                            kernel1_table,
                            tie(from_kernel1),
                            *from_detector1,
                            tie(to_kernel1)
        );
        
        SADF::make_kernelMN("kernel2",
                            kernel2_func,
                            kernel2_table,
                            tie(from_kernel2),
                            *from_detector2,
                            tie(to_kernel2)
        );

        SDF::make_source("source1", [] (int& out1, const int& inp1) {out1 = inp1 + 1;}, 1, 0, to_kernel1);
        
        SDF::make_source("source2", [] (int& out1, const int& inp1) {out1 = inp1 - 1;}, -1, 0, to_kernel2);        
        
        SDF::make_sink ("sink1", [] (const int& out) {std::cout <<"kernel1 = " <<out << std::endl;}, from_kernel1);
        
        SDF::make_sink ("sink2", [] (const int& out) {std::cout <<"kernel2 = " <<out << std::endl;}, from_kernel2);
        
        //! < -------------------------------- Without Using Helper--------------------------------> //!
        
        auto sourced = new SDF::source<int>("sourced", [] (int& out1, const int& inp1) {out1 = inp1 + 1;}, 1, 4);
        sourced->oport1(from_source);

        // auto detector1 = new SADF::detectorMN<
        //                         tuple<kernel1_scenario_type,kernel2_scenario_type>,
        //                         tuple<int>,
        //                         detector_scenario_type
        //                     >
        //                     (
        //                         "detector1",
        //                         detector1_cds_func,
        //                         detector1_kss_func,
        //                         detector1_table,
        //                         S1,
        //                         {1}
        //                         #ifdef FORSYDE_SELF_REPORTING
        //                         ,&report_pipe
        //                         #endif
        //                     );
        // get<0>(detector1->iport)(from_source);
        // get<0>(detector1->oport)(*from_detector1);
        // get<1>(detector1->oport)(*from_detector2);

        // auto kernel1 = new SADF::kernelMN<tuple<int>,kernel1_scenario_type,tuple<int>>(
        //                     "kernel1",
        //                     kernel1_func,
        //                     kernel1_table
        //                     #ifdef FORSYDE_SELF_REPORTING
        //                     ,&report_pipe
        //                     #endif
        //                 );
        // kernel1->cport1(*from_detector1);
        // get<0>(kernel1->iport)(to_kernel1);
        // get<0>(kernel1->oport)(from_kernel1);

                        
        // auto kernel2 = new SADF::kernelMN<tuple<int>,kernel2_scenario_type,tuple<int>>(
        //                     "kernel2",
        //                     kernel2_func,
        //                     kernel2_table
        //                     #ifdef FORSYDE_SELF_REPORTING
        //                     ,&report_pipe
        //                     #endif
        //                 );
        // kernel2->cport1(*from_detector2);
        // get<0>(kernel2->iport)(to_kernel2);
        // get<0>(kernel2->oport)(from_kernel2);


        // auto source1 = new SDF::source<int>("source1", [] (int& out1, const int& inp1) {out1 = inp1 + 1;}, 1, 0);
        // source1->oport1(to_kernel1);

        // auto source2 = new SDF::source<int>("source2", [] (int& out1, const int& inp1) {out1 = inp1 - 1;}, -1, 0);
        // source2->oport1(to_kernel2);


        // auto sink1 = new SDF::sink<int>("sink1",[](const int& out) {cout <<"kernel1 = " <<out << endl;});
        // sink1->iport1(from_kernel1);


        // auto sink2 = new SDF::sink<int>("sink2",[](const int& out) {cout <<"kernel2 = " <<out << endl;;});
        // sink2-> iport1(from_kernel2);

    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
#ifdef FORSYDE_SELF_REPORTING
        while (report_pipe_fd<=0) // pipe is not open
        {
            report_pipe_fd = open("gen/self_report", O_WRONLY|O_NONBLOCK);
            if (report_pipe_fd > 0)
                report_pipe = fdopen(report_pipe_fd, "w");
        }
#endif
    }
#endif
#ifdef FORSYDE_SELF_REPORTING
    void end_of_simulation()
    {
        fclose(report_pipe);
    }
#endif

};

