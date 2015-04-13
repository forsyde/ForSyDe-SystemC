/**********************************************************************
    * controller.hpp -- The controller of the geenrator               *
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

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <forsyde.hpp>

using namespace ForSyDe;

SC_MODULE(controller)
{
	DDE::in_port<double> voltage;
    DDE::out_port<double> drive;
	
	DDE::signal<double> voltage2, trigger, desired_v, err;
    CT::signal err_ct, drive_ct;
	
    SC_CTOR(controller)
	{
        auto fanout1 = DDE::make_fanout("fanout1", trigger, voltage);
        fanout1->oport1(voltage2);
        
        DDE::make_comb("desired_v1", 
            [](abst_ext<double>& desv, const double& trig) {desv=abst_ext<double>(110.0);},
            desired_v,
            trigger
        );
        
        DDE::make_comb2("sub1",
            [](abst_ext<double>& res, const abst_ext<double>& inp1, const abst_ext<double>& inp2)
            {
                res = abst_ext<double>(unsafe_from_abst_ext(inp1)-unsafe_from_abst_ext(inp2));
            },
            err,
            desired_v,
            voltage2
        );
        
        make_DDE2CT("de2ct1", HOLD, err_ct, err);
        
        CT::make_pif("pi1", 1.1, 1.0, sc_time(100,SC_MS), drive_ct, err_ct);
        
        make_CT2DDEf("ct2de1", sc_time(100, SC_MS), drive, drive_ct);
        
	}
};

#endif
