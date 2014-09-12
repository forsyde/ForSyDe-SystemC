/**********************************************************************
    * uwb_pg.hpp -- an Ultra Wide-band Pulse Generator                *
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

// This UWB PG contains two sine wave generators and one square wave generator.
// First the output of sine wave generators multiply to each other, 
// The resultant waveform multiplies to the output of square wave generator to get the impulse signal.

#include <forsyde.hpp>

using namespace sc_core;
using namespace ForSyDe;

// Define carrier frequency and period
#define fc 4.0e9
sc_time t_c = sc_time(1/4.0e9,SC_SEC);
// Define the (time) width of the envelope
#define f_bw 1.0e9
sc_time t_bw = sc_time(1/1.0e9,SC_SEC);
// Duty cycle = Firing period/Pulse pepetition period
double duty_cycle = 0.05;

SC_MODULE(uwb_pg)
{
    // Define the output
    CT::out_port out;

    // Define the nodes
    CT::signal from_sin1, from_sin2, from_mult1, from_sq;
    
    uwb_pg(sc_module_name _name,
            sc_time end_t, 			
            sc_time t_c, 			// carrier frequency
            sc_time t_bw, 			// envelope frequency
            sc_time t_fire, 	 		// frequency of the square wave
            double duty_cycle		// duty cycle of the square wave
        ) : sc_module(_name)
	{
        CT::make_sine("sin1", end_t, t_c, 1.0, from_sin1);
        
        CT::make_sine("sin2", end_t, t_bw, 1.0, from_sin2);
        
        CT::make_mul("mult1", from_mult1, from_sin1, from_sin2);
        
        CT::make_square("square1", end_t, t_fire, 1.0, 0.0, duty_cycle, from_sq);
        
        CT::make_mul("mult2", out, from_mult1, from_sq);
   	 }   
};

