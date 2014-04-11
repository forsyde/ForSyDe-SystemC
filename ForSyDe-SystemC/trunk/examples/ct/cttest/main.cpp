/**********************************************************************           
    * main.cpp -- the main file and testbench for the cttutorial exmpl*
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy CT example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "top.hpp"

int sc_main(int argc, char **argv)
{
    top top("top");

    sc_start();
        
    return 0;
}

