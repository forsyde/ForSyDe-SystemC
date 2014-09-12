/**********************************************************************
    * test_ir_channel.hpp -- the test module of the IR UWB channel    *
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

// Testbench of IR UWB communication channel.

#include <forsyde.hpp>

#include "uwb_pg.hpp"
#include "ir_channel.hpp"

//~ #include <cstdlib> // for std::rand
//~ #include <cmath> // for std::sqrt and std::log

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
    CT::signal from_pg, from_channel;
    
    SC_CTOR(top)
	{
        auto uwb_pg1 = new uwb_pg("uwb_pg1", end_t, t_c, t_bw, t_fire, duty_cycle);
        uwb_pg1->out(from_pg);
        
        auto ir_channel1 = new ir_channel("ir_channel1", ch_shift, ch_attn, no_of_bs);
        ir_channel1->in(from_pg);
        ir_channel1->out(from_channel);
        
        make_traceSig("traceSig1", t_step, from_channel);
    }
};
