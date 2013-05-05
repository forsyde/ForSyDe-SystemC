/**********************************************************************
    * top.hpp -- the top module and testbench for the equalizer example*
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *          the Haskell version from Ingo Sander (ingo@kth.se)     *
    *                                                                 *
    * Purpose: Demonstrating how co-simulation with legacy codes is   *
    *          performed.                                             *
    *                                                                 *
    * Usage:   equalizer example                                      *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>

#include <iostream>

#include "equalizer.hpp"
#include "report.hpp"

// Some syntactic sugar
#define AbstS abst_ext<Sensor>()
#define PrstS abst_ext<Sensor>(Active)
#define AbstF abst_ext<double>()
#define PrstF(Value) abst_ext<double>(Value)

using namespace ForSyDe::SY;

// Testbench inputs
std::vector<abst_ext<Sensor>> bassUp_vec =
        {PrstS, PrstS, AbstS, AbstS,
		 PrstS, PrstS, AbstS, AbstS,
		 PrstS, PrstS, AbstS, AbstS,
		 PrstS, PrstS, AbstS, AbstS};
std::vector<abst_ext<Sensor>> bassDn_vec =
        {AbstS, AbstS, PrstS, AbstS,
		 AbstS, AbstS, PrstS, AbstS,
		 AbstS, AbstS, PrstS, AbstS,
		 AbstS, AbstS, PrstS, AbstS};
std::vector<abst_ext<Sensor>> trebleUp_vec =
        {AbstS, AbstS, AbstS, AbstS,
		 AbstS, AbstS, AbstS, PrstS,
		 AbstS, AbstS, AbstS, AbstS,
		 AbstS, AbstS, AbstS, PrstS};
std::vector<abst_ext<Sensor>> trebleDn_vec =
        {AbstS, AbstS, AbstS, PrstS,
		 AbstS, AbstS, AbstS, PrstS,
		 AbstS, AbstS, AbstS, PrstS,
		 AbstS, AbstS, AbstS, PrstS};
std::vector<abst_ext<double>> input_vec =
        {PrstF(0.0), PrstF(0.1), PrstF(0.2), PrstF(0.3),
         PrstF(0.5), PrstF(0.6), PrstF(0.7), PrstF(0.8),
         PrstF(0.0), PrstF(0.1), PrstF(0.2), PrstF(0.3),
         PrstF(0.5), PrstF(0.6), PrstF(0.7), PrstF(0.8)};

SC_MODULE(top)
{
    SY2SY<Sensor> bassDn_sig, bassUp_sig;
    SY2SY<Sensor> trebleDn_sig, trebleUp_sig;
    SY2SY<double> input_sig, output_sig;
    
    SC_CTOR(top)
    {
        make_vsource("bassDn_src", bassDn_vec, bassDn_sig);

        make_vsource("bassUp_src", bassUp_vec, bassUp_sig);
        
        make_vsource("trebleDn_src", trebleDn_vec, trebleDn_sig);

        make_vsource("trebleUp_src", trebleUp_vec, trebleUp_sig);
        
        make_vsource("binput_src", input_vec, input_sig);
        
        auto equalizer1 = new equalizer("equalizer");
        equalizer1->bassUp(bassUp_sig);
        equalizer1->bassDn(bassDn_sig);
        equalizer1->trebleUp(trebleUp_sig);
        equalizer1->trebleDn(trebleDn_sig);
        equalizer1->input(input_sig);
        equalizer1->output(output_sig);
        
        make_sink("report1", report_func, output_sig);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};

