/**********************************************************************
    * test_radar.hpp -- the test module of the IR UWB simplified radar*
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *      Based on a model developed by Nikolaj Andersen (Novelda AS)*
    *                                                                 *
    * Purpose: Demonstration of a single cyber-physical system        *
    *                                                                 *
    * Usage:   IR UWB radar transceiver example                       *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/
// Connecting a 5GHz sine wave to the radar module to check its functionality

#include <forsyde.hpp>
#include "reader.hpp"
#include "radar_simplified.hpp"

#define N 400			        // numbers of samplers
#define END_T 20000.0e-9  // Simulation end time, must be long enough	
#define F_IN 5.0e9     		// frequency of the input sine wave

#ifndef PI
#define PI 3.14159265358979323846
#endif

sc_time sin_period = sc_time(1.0/F_IN,SC_SEC);
sc_time end_t = sc_time(END_T,SC_SEC);
sc_time st_t  = sc_time(0, SC_SEC);

using namespace sc_core;
using namespace ForSyDe;

SC_MODULE(top)
{
    CT::signal      src1, src2;
    
    SY::signal<int> from_sampler[N];
    
    SC_CTOR(top)
	{
        auto sin1 = CT::make_sine("sin1", end_t, sin_period, 1.0, src1);
        sin1->oport1(src2);
        
        auto radar1 = new radar_simplified("radar1", N, 10);
        radar1->sig(src1);
        for(int i=0;i<N;i++) radar1->oports[i](from_sampler[i]);
        
        auto reader1 = new reader("reader1", N);
        for(int i=0;i<N;i++) reader1->iports[i](from_sampler[i]);
        
        make_traceSig("trace_sig1", sc_time(1,SC_NS), src2);
    }
};

