/**********************************************************************
    * discrete_generator.hpp -- a generator wrapped in a DDE interface*
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

#ifndef DISCRETEGENERATOR_HPP
#define DISCRETEGENERATOR_HPP

#include <forsyde.hpp>
#include <cmath>
#include "generator.hpp"

using namespace ForSyDe;

SC_MODULE(discrete_generator)
{
	DDE::in_port<double> drive;
	DDE::in_port<double> load_impedance;
    DDE::out_port<double> voltage;
	
	CT::signal ct_drive, ct_load_impedance, ct_voltage;
	
    discrete_generator(sc_module_name name_, double time_constant,
        double output_impedance, sc_time sampling_period): sc_module(name_)
	{
        make_DDE2CT("zero_order_hold1", HOLD, ct_drive, drive);
        
        make_DDE2CT("zero_order_hold2", HOLD, ct_load_impedance, load_impedance);
		
		auto generator1 = new generator("generator1", time_constant, output_impedance, INFINITY);
        generator1->drive(ct_drive);
        generator1->load_impedance(ct_load_impedance);
        generator1->voltage(ct_voltage);
        
        make_CT2DDEf("periodic_sampler1", sampling_period, voltage, ct_voltage);
	}
};

#endif
