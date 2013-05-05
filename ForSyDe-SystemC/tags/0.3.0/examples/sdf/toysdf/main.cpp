/**********************************************************************           
    * main.cpp -- the main file and testbench for the tyo SDF example *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   To SDF example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "top.hpp"

int sc_main(int argc, char **argv)
{
    top top1("top1");

    sc_start();
        
    return 0;
}

