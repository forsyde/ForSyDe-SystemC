/**********************************************************************
    * top.hpp -- the top module and testbench for an Impulse Radio    *
    *            Ultra Wide-band (IR UWB) radar transceiver           *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          Based on a model developed by Nikolaj Andersen and     *
    *          Kody Lee (Novelda AS)                                  *
    *                                                                 *
    * Purpose: Demonstration of a single cyber-physical system        *
    *                                                                 *
    * Usage:   IR UWB radar transceiver example                       *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#define NTAPS 200			// numbers of samplers
#define END_T 11000.0e-9    // Simulation end time, must be long enough

#ifndef PI
#define PI 3.14159265358979323846
#endif

#include <forsyde.hpp>
#include "reader.hpp"
#include "radar_simplified.hpp"
#include "uwb_pg.hpp"
#include "ir_channel.hpp"

// End time of the simulation
sc_time end_t = sc_time(END_T,SC_SEC);

// Start time of the sampler 
sc_time st_t  = sc_time(0, SC_SEC);

// Initial delay
sc_time delay_int  = sc_time(0, SC_NS);

// Pulse repetition frequency
sc_time t_fire = sc_time(10.0,SC_NS);

// Step size of the simulation
sc_time t_step = sc_time(25e-12,SC_SEC);

// Filter parameters
std::vector<CTTYPE> nums = {1.039e39, 0.0, 0.0, 0.0, 0.0};
std::vector<CTTYPE> dens = {1.0, 2.6912e9, 2.5302e21, 5.0585e30, 2.2986e42, 2.9955e51, 8.8729e62, 5.5885e71, 1.2297e83};

using namespace sc_core;
using namespace ForSyDe;

SC_MODULE(top)
{
    CT::signal  from_pg, from_anttx, from_channel, from_antrx, src2;
    std::vector<SY::signal<int>>    from_sampler;
    
    SC_CTOR(top) : from_sampler(NTAPS)
	{
        auto uwb_pg1 = new uwb_pg("uwb_pg1", end_t, t_c, t_bw, t_fire, duty_cycle);
        uwb_pg1->out(from_pg);
        
        CT::make_filterf("tx_antenna", nums, dens, t_step, from_anttx, from_pg);
        
        auto ir_channel1 = new ir_channel("ir_channel1", ch_shift, ch_attn, no_of_bs);
        ir_channel1->in(from_anttx);
        ir_channel1->out(from_channel);
        
        CT::make_filterf("rx_antenna", nums, dens, t_step, from_antrx, from_channel);
        
        CT::make_shift("chan_delay", delay_int, src2, from_antrx);
        
        auto radar1 = new radar_simplified("radar1", NTAPS, 0.16);
        radar1->sig(src2);
        for(int i=0;i<NTAPS;i++) radar1->oports[i](from_sampler[i]);
        
        auto reader1 = new reader("reader1", NTAPS);
        for(int i=0;i<NTAPS;i++) reader1->iports[i](from_sampler[i]);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif    
};
