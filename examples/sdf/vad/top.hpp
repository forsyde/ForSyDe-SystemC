/**********************************************************************           
    * top.hpp -- the Top process and testbench for the VAD example    *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          adapted from KisTA: https://github.com/nandohca/kista  *
    *                                                                 *
    * Purpose:                                                        *
    *                                                                 *
    * Usage:   The Voice Activity Detection (VAD) example             *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>

#include "includes/vad_types.hpp"
#include "vad_source.hpp"
#include "ToneDetection.hpp"

#include "ACFAveraging.hpp"
#include "PredictorValues.hpp"
#include "SpectralComparison.hpp"

using namespace ForSyDe;

SC_MODULE(top)
{
    SDF::signal<r_t> e12, e13, e14;
    SDF::signal<short> e15, e16;
    SDF::signal<rc_t> e17;
    SDF::signal<short> e18;
    
    SDF::signal<tuple_of_vectors<r_t,r_t,r_t,short,short,rc_t,short>> e12_13_14_15_16_17_18;
    
    SDF::signal<short> e9;
    
    SDF::signal<L_av_t> e1, e2;
    
    SDF::signal<rav1_t> e3, e4;
    
    SDF::signal<short> e5, e6, e7, e8, e10, e11;
    
    SDF::signal<short> e19;
    
    SDF::signal<tuple_of_vectors<L_av_t,L_av_t>> e1_2;
    
    SDF::signal<short> e7_10;
    
    
    SC_CTOR(top)
    {
        SDF::make_file_source("VADFilesource1", VADFilesource_func, "source_data.txt", e12_13_14_15_16_17_18);
      
        //~ SDF::make_unzip("VADFilesource1_unzip", e12_13_14_15_16_17_18, {1,1,1,1,1,1,1}, e12, e13, e14, e15, e16, e17, e18);
        auto VADFilesource1_unzip = new SDF::unzipN<r_t,r_t,r_t,short,short,rc_t,short>("VADFilesource1_unzip", {1,1,1,1,1,1,1});
        VADFilesource1_unzip->iport1(e12_13_14_15_16_17_18);
        std::get<0>(VADFilesource1_unzip->oport)(e12);
        std::get<1>(VADFilesource1_unzip->oport)(e13);
        std::get<2>(VADFilesource1_unzip->oport)(e14);
        std::get<3>(VADFilesource1_unzip->oport)(e15);
        std::get<4>(VADFilesource1_unzip->oport)(e16);
        std::get<5>(VADFilesource1_unzip->oport)(e17);
        std::get<6>(VADFilesource1_unzip->oport)(e18);        
        
        SDF::make_comb("ToneDetection1", ToneDetection_func, 1, 1, e9, e17);
        
        //~ SDF::make_comb3("EnergyComputation1", EnergyComputation_func, 1, 1, 1, e6, e7, e13, e16);
        
        SDF::make_comb3("ACFAveraging1", ACFAveraging_func, 1, 1, 1, 1, e1_2, e12, e14, e15);
        
        SDF::make_unzip("ACFAveraging1_unzip", e1_2, 1, 1, e1, e2);
        
        auto PredictorValues1 = SDF::make_comb("PredictorValues1", PredictorValues_func, 1, 1, e3, e2);
        PredictorValues1->oport1(e4);
        
        SDF::make_comb2("SpectralComparison1", SpectralComparison_func, 1, 1, 1, e5, e1, e3);
        
        //~ SDF::make_comb5("ThresholdAdaptation1", ThresholdAdaptation_func, , , , e7_10, e4, e5, e6, e9, e18);
        //~ 
        //~ SDF::make_unzip("ThresholdAdaptation1_unzip", e7_10, , , e7, e10);
        //~ 
        //~ SDF::make_comb2("VADdecision1", VADdecision_func, , , , e11, e8, e10);
        //~ 
        //~ SDF::make_comb("VAD_hangover1", VADhangover_func, , , e19, e11);
        //~ 
        //~ SDF::make_file_sink("VADFilesink", VADFilesink_func, e19);
        
        // FIXME: REMOVE! only for test:
        SDF::make_sink("test_sink", [](short val){std::cout << val << std::endl;}, e5);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
