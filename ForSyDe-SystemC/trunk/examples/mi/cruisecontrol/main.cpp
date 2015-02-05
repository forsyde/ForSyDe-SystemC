/**********************************************************************
    * main.hpp -- the top module and testbench for the cruise control *
    *          example                                                *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a heterogeneous system                *
    *                                                                 *
    * Usage:   Cruise control example                                 *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "top.hpp"

int sc_main(int argc, char **argv)
{
    top top("top");

    sc_start(12,SC_SEC);

    sc_stop();
    return 0;
}
