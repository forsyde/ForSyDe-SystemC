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

using namespace ForSyDe;

SC_MODULE(top)
{
    SDF::signal<r_t> e12, e13, e14;
    SDF::signal<short> e15, e16;
    SDF::signal<rc_t> e17;
    SDF::signal<short> e18;
    
    SDF::signal<std::tuple<r_t,r_t,r_t,short,short,rc_t,short>> e12_13_14_15_16_17_18;
    
    SDF::signal<short> e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11;
    
    SDF::signal<short> e1_2, e3_4, e7_10;
    
    SDF::signal<short> e19;
    
    SC_CTOR(top)
    {
        SDF::make_file_source("VADFilesource1", VADFilesource_func, "source_data.txt", e12_13_14_15_16_17_18);
      
        //~ SDF::make_unzip("VADFilesource1_unzip", e12_13_14_15_16_17_18, , , , , , , , e12, e13, e14, e15, e16, e17, e18);
        
        //~ SDF::make_comb("ToneDetection1", ToneDetection_func, , , e9, e17);
        //~ 
        //~ SDF::make_comb3("EnergyComputation1", EnergyComputation_func, , , e6, e7, e13, e16);
        //~ 
        //~ SDF::make_comb3("ACFAveraging1", ACFAveraging_fun, , , ez2, e12, e14, e15);
        //~ 
        //~ SDF::make_unzip("ACFAveraging1_unzip", e1_2, , , e1, e2);
        //~ 
        //~ SDF::make_comb("PredictorValues1", PredictorValues_func, , , e3_4, e2);
        //~ 
        //~ SDF::make_unzip("PredictorValues1_unzip", e3_4, , , e3, e4);
        //~ 
        //~ SDF::make_comb2("SpectralComparison1", SpectralComparison_func, , , , e5, e1, e3);
        //~ 
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
        SDF::make_sink("test_sink", [](std::tuple<r_t,r_t,r_t,short,short,rc_t,short> val){std::cout << val << std::endl;}, e12_13_14_15_16_17_18);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
