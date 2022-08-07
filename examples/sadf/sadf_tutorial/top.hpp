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

    SADF::signal<kernel1_scenario_type> from_detector1; 
    SADF::signal<kernel2_scenario_type> from_detector2;
    SADF::signal<int> from_source;
    SADF::signal<int> to_kernel1, from_kernel1, to_kernel2, from_kernel2;
    SADF::signal<tuple<vector<kernel1_scenario_type>,vector<kernel2_scenario_type>>> to_zip;

    SC_CTOR(top)
    {

        SDF::make_source ("sourced", [] (int& out1, const int& inp1) {out1 = inp1 + 1;}, 1, 4, from_source);

        SADF::make_detector("detector1",
                            detector1_cds_func,
                            detector1_kss_func,
                            detector1_table,
                            S1,
                            1,
                            to_zip,
                            from_source
                            );
        SDF::make_unzip("unzip1", to_zip, 1, 1, from_detector1, from_detector2);

        SADF::make_kernel("kernel1",
                            kernel1_func,
                            kernel1_table,
                            from_kernel1,
                            from_detector1,
                            to_kernel1
                        );

        SADF::make_kernel("kernel2",
                            kernel2_func,
                            kernel2_table,
                            from_kernel2,
                            from_detector2,
                            to_kernel2
                        );

        SDF::make_source("source1", [] (int& out1, const int& inp1) {out1 = inp1 + 1;}, 1, 0, to_kernel1);
        
        SDF::make_source("source2", [] (int& out1, const int& inp1) {out1 = inp1 - 1;}, -1, 0, to_kernel2);        
        
        SDF::make_sink ("sink1", [] (const int& out) {std::cout <<"kernel1 = " <<out << std::endl;}, from_kernel1);
        
        SDF::make_sink ("sink2", [] (const int& out) {std::cout <<"kernel2 = " <<out << std::endl;}, from_kernel2);


        
        
        //! < -------------------------------- Without Using Helper--------------------------------> //!
        
        // auto sourced = new SDF::source<int>("sourced", [] (int& out1, const int& inp1) {out1 = inp1 + 1;}, 1, 4);
        // sourced->oport1(from_source);

        // auto detector1 = new SADF::detector<
        //                         tuple<vector<kernel1_scenario_type>,vector<kernel2_scenario_type>>,
        //                         int,
        //                         detector_scenario_type
        //                     >
        //                     (
        //                         "detector1",
        //                         detector1_cds_func,
        //                         detector1_kss_func,
        //                         detector1_table,
        //                         S1,
        //                         1
        //                     );
        // detector1->iport1(from_source);
        // detector1->oport1(to_zip);
        // auto unzip1 = new SDF::unzip<kernel1_scenario_type,kernel2_scenario_type>("unzip1",1,1);
        // unzip1->iport1(to_zip);
        // unzip1->oport1(from_detector1);
        // unzip1->oport2(from_detector2);

        // auto kernel1 = new SADF::kernel <int,kernel1_scenario_type,int>(
        //                     "kernel1",
        //                     kernel1_func,
        //                     kernel1_table
        //                 );
        // kernel1->cport1(from_detector1);
        // kernel1->iport1(to_kernel1);
        // kernel1->oport1(from_kernel1);

                            
        // auto kernel2 = new SADF::kernel <int,kernel2_scenario_type,int>(
        //                     "kernel2",
        //                     kernel2_func,
        //                     kernel2_table
        //                 );
        // kernel2->cport1(from_detector2);
        // kernel2->iport1(to_kernel2);
        // kernel2->oport1(from_kernel2);


        // auto source1 = new SDF::source<int>("source1", [] (int& out1, const int& inp1) {out1 = inp1 + 1;}, 1, 0);
        // source1->oport1(to_kernel1);

        // auto source2 = new SDF::source<int>("source2", [] (int& out1, const int& inp1) {out1 = inp1 - 1;}, -1, 0);
        // source2->oport1(to_kernel2);


        // auto sink1 = new SDF::sink<int>("sink1",[](const int& out) {std::cout <<"kernel1 = " <<out << std::endl;});
        // sink1->iport1(from_kernel1);


        // auto sink2 = new SDF::sink<int>("sink2",[](const int& out) {std::cout <<"kernel2 = " <<out << std::endl;;});
        // sink2-> iport1(from_kernel2);

    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif

};

