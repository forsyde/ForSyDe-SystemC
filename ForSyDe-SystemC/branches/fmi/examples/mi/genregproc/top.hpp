/**********************************************************************
    * top.hpp -- the top module and testbench for the Generator/      *
    *          Regulator/Protector example                            *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          Based on the example from chapter 1 of:                *
    *          System Design, Modeling, and Simulation using PtolemyII*
    *                                                                 *
    * Purpose: Demonstration of a single cyber-physical system        *
    *                                                                 *
    * Usage:   Generator/Regulator/Protector example                  *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>
#include "supervisor.hpp"
#include "discrete_generator.hpp"
#include "controller.hpp"
#include "report.hpp"

//~ #define A 5.0
#define OVT 115.0

using namespace sc_core;
using namespace ForSyDe;

SC_MODULE(top)
{
    DDE::signal<bool> on_off, fault;
	DDE::signal<double> load_impedance, load_impedance_d,
        voltage_controller, voltage_expression, voltage_plot,
        drive_discgen, drive_discgen_d;
		
	SC_CTOR(top)
	{
		DDE::make_vsource("single_event1",
            std::vector<bool>({false,true}),
            std::vector<sc_time>({sc_time(0, SC_SEC),sc_time(15, SC_SEC)}),
            on_off
        );
		
		auto supervisor1 = new supervisor("supervisor1");
        supervisor1->on_off(on_off);
        supervisor1->fault(fault);
		supervisor1->load_impedance(load_impedance);
        
        auto impedance_delay = DDE::make_delay(
            "impedance_delay", abst_ext<double>(),sc_time(200, SC_MS),
            load_impedance_d, load_impedance
        );
        
        auto discrete_generator1 = new discrete_generator(
            "discrete_generator1", 5.0, 1.0, sc_time(100, SC_MS)
        );
        discrete_generator1->drive(drive_discgen_d);
		discrete_generator1->load_impedance(load_impedance_d);
        discrete_generator1->voltage(voltage_controller);
        discrete_generator1->voltage(voltage_expression);
        discrete_generator1->voltage(voltage_plot);
        
        auto controller1 = new controller("controller1");
        controller1->voltage(voltage_controller);
        controller1->drive(drive_discgen);
        
        auto drive_delay = DDE::make_delay(
            "drive_delay", abst_ext<double>(), sc_time(400, SC_MS),
            drive_discgen_d, drive_discgen
        );
        
        DDE::make_comb("expression1",
            [](abst_ext<bool>& fault, const double& voltage) {
                    fault=abst_ext<bool>(voltage >= OVT);
            },
            fault,
            voltage_expression
        );
		
		DDE::make_sink("report1", report_func, voltage_plot);
	}
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
