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

    SC_CTOR(top)
    {
        
        //! < -------------------------------- Without Using Helper--------------------------------> //!
        
        auto sourced = new SDF::source<int>("sourced", [] (int& out1, const int& inp1) {out1 = inp1 + 1;}, 1, 4);
        sourced->oport1(from_source);

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
        auto detector1 = new SADF::detectorMN<
                                tuple<kernel1_scenario_type,kernel2_scenario_type>,
                                tuple<int>,
                                detector_scenario_type
                            >
                            (
                                "detector1",
                                detector1_cds_func,
                                detector1_kss_func,
                                detector1_table,
                                S1,
                                {1}
                            );
        get<0>(detector1->iport)(from_source);
        get<0>(detector1->oport)(from_detector1);
        get<1>(detector1->oport)(from_detector2);

        auto kernel1 = new SADF::kernelMN<tuple<int>,kernel1_scenario_type,tuple<int>>(
                            "kernel1",
                            kernel1_func,
                            kernel1_table
                        );
        kernel1->cport1(from_detector1);
        get<0>(kernel1->iport)(to_kernel1);
        get<0>(kernel1->oport)(from_kernel1);
                            
        auto kernel2 = new SADF::kernelMN<tuple<int>,kernel2_scenario_type,tuple<int>>(
                            "kernel2",
                            kernel2_func,
                            kernel2_table
                        );
        kernel2->cport1(from_detector2);
        get<0>(kernel2->iport)(to_kernel2);
        get<0>(kernel2->oport)(from_kernel2);


        auto source1 = new SDF::source<int>("source1", [] (int& out1, const int& inp1) {out1 = inp1 + 1;}, 1, 0);
        source1->oport1(to_kernel1);

        auto source2 = new SDF::source<int>("source2", [] (int& out1, const int& inp1) {out1 = inp1 - 1;}, -1, 0);
        source2->oport1(to_kernel2);


        auto sink1 = new SDF::sink<int>("sink1",[](const int& out) {cout <<"kernel1 = " <<out << endl;});
        sink1->iport1(from_kernel1);


        auto sink2 = new SDF::sink<int>("sink2",[](const int& out) {cout <<"kernel2 = " <<out << endl;;});
        sink2-> iport1(from_kernel2);

    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif

};

