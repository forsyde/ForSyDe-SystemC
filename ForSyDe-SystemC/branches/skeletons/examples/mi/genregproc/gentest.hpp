/**********************************************************************
    * gentest.hpp -- test the generator                               *
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
#include "generator.hpp"

using namespace sc_core;
using namespace ForSyDe;

SC_MODULE(top)
{
    CT::signal drive_gen, drive_plot, load, load_plot, output_voltage;
		
	SC_CTOR(top)
	{
		auto dm1 = CT::make_source("drive_model", [](CTTYPE& ret, const sc_time& t)
            {
                ret = t<sc_time(10,SC_SEC) ? 110:
                    t<sc_time(20,SC_SEC) ? 220:
                    110;
            },
            sc_time(30,SC_SEC),
            drive_gen
        );
        dm1->oport1(drive_plot);
		
		auto lm1 = CT::make_source("load_model", [](CTTYPE& ret, const sc_time& t)
            {
                ret = t<sc_time(15,SC_SEC) ? INFINITY: 10;
            },
            sc_time(30,SC_SEC),
            load
        );
        lm1->oport1(load_plot);
        
        auto generator1 = new generator("generator1", 1.0, 1.0, INFINITY);
        generator1->drive(drive_gen);
        generator1->load_impedance(load);
		generator1->voltage(output_voltage);
		
		CT::make_traceSig("report1", sc_time(100,SC_MS), output_voltage);
        CT::make_traceSig("report2", sc_time(100,SC_MS), drive_plot);
        CT::make_traceSig("report3", sc_time(100,SC_MS), load_plot);
	}
};
