/**********************************************************************
    * main.cpp -- the top module and testbench for an Impulse Radio    *
    *            Ultra Wide-band (IR UWB) radar transceiver           *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          Based on a model developed by Novelda AS               *
    *                                                                 *
    * Purpose: Demonstration of a single cyber-physical system        *
    *                                                                 *
    * Usage:   IR UWB radar transceiver example                       *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

//~ #include "top.hpp"
//~ #include "test_uwb_pg.hpp"
//~ #include "test_ir_channel.hpp"
#include "test_radar.hpp"


int sc_main(int argc, char **argv)
{
    top top1("top1");

    sc_start(11,SC_NS);
        
    return 0;
}
