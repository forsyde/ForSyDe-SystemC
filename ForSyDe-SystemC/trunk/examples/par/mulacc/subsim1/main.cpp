/**********************************************************************
    * main.cpp -- the main file and testbench for the mulacc example  *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple sequential processes.        *
    *                                                                 *
    * Usage:   Parallel MulAcc example                                *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "top.hpp"

int sc_main(int argc, char **argv)
{
    MPI_Init (&argc, &argv);
    
    top top1("top1");

    sc_start();
    
    MPI_Finalize();
        
    return 0;
}
