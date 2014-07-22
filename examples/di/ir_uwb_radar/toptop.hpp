/**********************************************************************
    * top.hpp -- the top module and testbench for an Impulse Radio    *
    *            Ultra Wide-band (IR UWB) radar transceiver           *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          Based on a model developed by Novelda AS               *
    *                                                                 *
    * Purpose: Demonstration of a single cyber-physical system        *
    *                                                                 *
    * Usage:   IR UWB radar transceiver example                       *
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
	DDE::signal<double> load_impedance, voltage_controller, voltage_pd3,voltage_pd4,
        voltage_expression, voltage_plot, drive_discgen, drive_plot;
		
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
                
        auto discrete_generator1 = new discrete_generator(
            "discrete_generator1", 5.0, 1.0, sc_time(100, SC_MS)
        );
        discrete_generator1->drive(drive_discgen);
		discrete_generator1->load_impedance(load_impedance);
        discrete_generator1->voltage(voltage_pd3);
        
        // the following two delays should be there and the second one needs to be more than 3*sample_time
        auto voltage_delay3 = DDE::make_delay(
            "voltage_delay3", abst_ext<double>(),sc_time(100, SC_MS),
            voltage_pd4, voltage_pd3
        );
        auto voltage_delay4 = DDE::make_delay(
            "voltage_delay4", abst_ext<double>(), sc_time(400, SC_MS),
            voltage_controller, voltage_pd4
        );
        voltage_delay4->oport1(voltage_expression);
        voltage_delay4->oport1(voltage_plot);
        
        auto controller1 = new controller("controller1");
        controller1->voltage(voltage_controller);
        controller1->drive(drive_discgen);
        
        DDE::make_comb("expression1",
            [](abst_ext<bool>& fault, const double& voltage) {
                    fault=abst_ext<bool>(voltage >= OVT);
            },
            fault,
            voltage_expression
        );
		
		DDE::make_sink("report1", report_func, voltage_plot);
	}
};
