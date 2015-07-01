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
#include "EnergyComputation.hpp"
#include "ACFAveraging.hpp"
#include "PredictorValues.hpp"
#include "SpectralComparison.hpp"

#include "VADdecision.hpp"

using namespace ForSyDe;

SC_MODULE(top)
{
    SDF::signal<short> e5, e9, e11, e19, e15, e16, e18;
    SDF::signal<L_av_t> e1, e2;
    SDF::signal<rav1_t> e3, e4;
    SDF::signal<pvad_acf0_t> e6;
    SDF::signal<rvad_t> e7, e7d;
    SDF::signal<Pfloat> e8, e10;
    SDF::signal<r_t> e12, e13, e14;
    SDF::signal<rc_t> e17;
    SDF::signal<tuple_of_vectors<L_av_t,L_av_t>> e1_2;
    SDF::signal<tuple_of_vectors<pvad_acf0_t,Pfloat>> e6_8;
    SDF::signal<tuple_of_vectors<rvad_t,Pfloat>> e7_10;
    SDF::signal<tuple_of_vectors<r_t,r_t,r_t,short,short,rc_t,short>> e12_13_14_15_16_17_18;
    
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
        
        SDF::make_comb3("EnergyComputation1", EnergyComputation_func, 1, 1, 1, 1, e6_8, e7d, e13, e16);
        
        SDF::make_unzip("EnergyComputation1_unzip", e6_8, 1, 1, e6, e8);
        
        SDF::make_comb3("ACFAveraging1", ACFAveraging_func, 1, 1, 1, 1, e1_2, e12, e14, e15);
        
        SDF::make_unzip("ACFAveraging1_unzip", e1_2, 1, 1, e1, e2);
        
        auto PredictorValues1 = SDF::make_comb("PredictorValues1", PredictorValues_func, 1, 1, e3, e2);
        PredictorValues1->oport1(e4);
        
        SDF::make_comb2("SpectralComparison1", SpectralComparison_func, 1, 1, 1, e5, e1, e3);
        
        //~ SDF::make_comb5("ThresholdAdaptation1", ThresholdAdaptation_func, , , , e7_10, e4, e5, e6, e9, e18);
        //~ 
        //~ SDF::make_unzip("ThresholdAdaptation1_unzip", e7_10, , , e7, e10);
        //~ 
        std::array<short,9> rvad_init = {{0x6000,0,0,0,0,0,0,0,0}}; short scal_init = 7;
        SDF::make_delay("e7_init", std::make_tuple(rvad_init,scal_init), e7d, e7);
        
        SDF::make_comb2("VADdecision1", VADdecision_func, 1, 1, 1, e11, e8, e10);
        
        //~ SDF::make_comb("VAD_hangover1", VADhangover_func, , , e19, e11);
        //~ 
        //~ SDF::make_file_sink("VADFilesink", VADFilesink_func, e19);
        
        // FIXME: REMOVE! only for test:
        SDF::make_sink("test_sink", [](short val){std::cout << val << std::endl;}, e11);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
