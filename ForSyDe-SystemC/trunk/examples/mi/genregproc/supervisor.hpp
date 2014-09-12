/**********************************************************************
    * supervisor.hpp -- the supervisor for a power grid system        *
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

#ifndef SUPERVISOR_HPP
#define SUPERISOR_HPP

#include <forsyde.hpp>

#define A 10.0

using namespace ForSyDe;

enum state_type {OFF, ON, FAULT};

void supervisor_fsm_ns(state_type& ns,
    const state_type& cs,
    const ttn_event<bool>& on_off,
    const ttn_event<bool>& fault)
{
    switch(cs)
    {
        case OFF:if (from_abst_ext(get_value(on_off),false))
                    ns = ON;
                else
                    ns = OFF;
                break;
        
        case ON:if (from_abst_ext(get_value(fault),false))
                    ns = OFF;
                else
                    if (from_abst_ext(get_value(on_off),true))
                        ns = ON;
                    else
                        ns = OFF;
                break;
        default: ns = cs;
    }
}

void supervisor_fsm_od(abst_ext<double>& la,
    const state_type& cs,
    const ttn_event<bool>& on_off,
    const ttn_event<bool>& fault)
{
    switch(cs)
    {
        case OFF:if (from_abst_ext(get_value(on_off),false))
                    la = A;
                else
                    la = INFINITY;
                break;
        
        case ON:if (from_abst_ext(get_value(fault),false))
                    la = INFINITY;
                else
                    if (from_abst_ext(get_value(on_off),true))
                        la = A;
                    else
                        la = INFINITY;
                break;
        default: la = INFINITY;
    }
}

SC_MODULE(supervisor)
{
    DDE::in_port<bool> on_off;
    DDE::in_port<bool> fault;
    DDE::out_port<double> load_impedance;
    
    SC_CTOR(supervisor)
    {
        DDE::make_mealy2("supervisor_fsm1",
            supervisor_fsm_ns,
            supervisor_fsm_od,
            OFF,
            SC_ZERO_TIME,
            load_impedance,
            on_off,
            fault
        );
    }
};

#endif
