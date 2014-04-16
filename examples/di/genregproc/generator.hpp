/**********************************************************************
    * generator.hpp -- simplified model of a gas-powered generator    *
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

#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <forsyde.hpp>

using namespace ForSyDe;

SC_MODULE(generator)
{
	CT::in_port drive;
	CT::in_port load_impedance;
    CT::out_port voltage;
	
	CT::signal limiter2sub, sub2scale, scale2int, int2sub, int2sub2, int2expr;
	
    generator(sc_module_name name_, double time_constant,
        double output_impedance, float drive_limit): sc_module(name_)
	{
        CT::make_comb("limiter1", [drive_limit](CTTYPE& vout, const CTTYPE& vin)
            {
                if (vin>drive_limit) vout = drive_limit;
                else if (vin<0) vout = 0;
                else vout = vin;
            }, limiter2sub, drive);
        
        CT::make_sub("sub1", sub2scale, limiter2sub, int2sub);
        
        CT::make_scale("scale1", 1.0/time_constant, scale2int, sub2scale);
        
        auto int1 = CT::make_integratorf("integrator1", sc_time(1 ,SC_MS), int2sub2, scale2int);
        int1->oport1(int2expr);
        
        CT::make_delay("delay1", sc_time(1.1,SC_MS), int2sub, int2sub2);
                
        CT::make_comb2("expression1", [=](CTTYPE& vout, const CTTYPE& vin, const CTTYPE& imp)
            {
                vout = (imp == INFINITY) ? vin : vin*imp/(output_impedance+imp);
            }, voltage, int2expr, load_impedance);
	}
};

#endif
