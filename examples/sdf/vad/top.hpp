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
#include "ThresholdAdaptation.hpp"
#include "VADdecision.hpp"
#include "VADhangover.hpp"
#include "VADFilesink.hpp"

using namespace ForSyDe;

SC_MODULE(top)
{
    SDF::signal<short> e5, e9, e11, e15, e16, e18, e19;
    SDF::signal<L_av_t> e1, e2;
    SDF::signal<rav1_t> e3, e4;
    SDF::signal<pvad_acf0_t> e6;
    SDF::signal<rvad_t> e7, e7d;
    SDF::signal<Pfloat> e8, e10;
    SDF::signal<r_t> e12, e13, e14;
    SDF::signal<rc_t> e17;
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
        
        auto EnergyComputation1 = new SDF::combMN<std::tuple<pvad_acf0_t,Pfloat>,std::tuple<rvad_t,r_t,short>>(
            "EnergyComputation1",
            EnergyComputation_func,
            {1,1},
            {1,1,1}
        );
        std::get<0>(EnergyComputation1->iport)(e7d);
        std::get<1>(EnergyComputation1->iport)(e13);
        std::get<2>(EnergyComputation1->iport)(e16);
        std::get<0>(EnergyComputation1->oport)(e6);
        std::get<1>(EnergyComputation1->oport)(e8);
        
        auto ACFAveraging1 = new SDF::combMN<std::tuple<L_av_t,L_av_t>,std::tuple<r_t,r_t,short>>(
            "ACFAveraging1",
            ACFAveraging_func,
            {1,1},
            {1,1,1}
        );
        std::get<0>(ACFAveraging1->iport)(e12);
        std::get<1>(ACFAveraging1->iport)(e14);
        std::get<2>(ACFAveraging1->iport)(e15);
        std::get<0>(ACFAveraging1->oport)(e1);
        std::get<1>(ACFAveraging1->oport)(e2);
        
        auto PredictorValues1 = SDF::make_comb("PredictorValues1", PredictorValues_func, 1, 1, e3, e2);
        PredictorValues1->oport1(e4);
        
        SDF::make_comb2("SpectralComparison1", SpectralComparison_func, 1, 1, 1, e5, e1, e3);
        
        auto ThresholdAdaptation1 = new SDF::combMN<std::tuple<rvad_t,Pfloat>,std::tuple<rav1_t,short,pvad_acf0_t,short,short>>(
            "ThresholdAdaptation1",
            ThresholdAdaptation_func,
            {1,1},
            {1,1,1,1,1}
        );
        std::get<0>(ThresholdAdaptation1->iport)(e4);
        std::get<1>(ThresholdAdaptation1->iport)(e5);
        std::get<2>(ThresholdAdaptation1->iport)(e6);
        std::get<3>(ThresholdAdaptation1->iport)(e9);
        std::get<4>(ThresholdAdaptation1->iport)(e18);
        std::get<0>(ThresholdAdaptation1->oport)(e7);
        std::get<1>(ThresholdAdaptation1->oport)(e10);
        
        std::array<short,9> rvad_init = {{0x6000,0,0,0,0,0,0,0,0}}; short scal_init = 7;
        SDF::make_delay("e7_init", std::make_tuple(rvad_init,scal_init), e7d, e7);
        
        SDF::make_comb2("VADdecision1", VADdecision_func, 1, 1, 1, e11, e8, e10);
        
        SDF::make_comb("VADhangover1", VADhangover_func, 1, 1, e19, e11);
        
        SDF::make_file_sink("VADFilesink1", VADFilesink_func, "sink_data.txt", e19);
        
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
