/**********************************************************************           
    * main.cpp -- the main file and testbench for the cttutorial exmpl*
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Tutorial CT example                                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "top.hpp"

int sc_main(int argc, char **argv)
{
    Top top("top");

    sc_start();
        
    return 0;
}

