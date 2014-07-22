/**********************************************************************
    * test_uwb_pg.hpp -- the test module and testbench for an Ultra   *
    *                    Wide-band Pulse Generator                    *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          Based on a model developed by Kody Lee (Novelda AS)    *
    *                                                                 *
    * Purpose: Demonstration of a single cyber-physical system        *
    *                                                                 *
    * Usage:   IR UWB radar transceiver example                       *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>

#include "uwb_pg.hpp"

using namespace sc_core;
using namespace ForSyDe;

// End time 
sc_time end_t = sc_time(11.0,SC_NS);
// Step size
sc_time t_step = sc_time(1/4.0e9/10,SC_SEC);
// Pulse repetition period
sc_time t_fire = sc_time(10.0,SC_NS);

SC_MODULE(top)
{
    CT::signal from_pg;
    
    SC_CTOR(top)
	{
        auto uwb_pg1 = new uwb_pg("uwb_pg1", end_t, t_c, t_bw, t_fire, duty_cycle);
        uwb_pg1->out(from_pg);
        
        make_traceSig("traceSig1", t_step, from_pg);
    }
};
