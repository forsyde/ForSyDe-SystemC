/**********************************************************************
    * Top.hpp -- the top module and testbench for the mulacc example  *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple sequential processes.        *
    *                                                                 *
    * Usage:   Parallel MulAcc example                                *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "report.hpp"
#include "siggen.hpp"
#include <iostream>

using namespace ForSyDe::SY;

SC_MODULE(top)
{
    SY2SY<int> srcb, result;
    
    SC_CTOR(top)
    {
        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size );
        int partner_rank = (world_rank + 1) % 2;
        
        make_source("siggen1", siggen_func, abst_ext<int>(1), 10, srcb);
        
        make_sender<int>("sender1", partner_rank, 1, srcb);
        
        auto receiver1 = new receiver<int>("receiver1", partner_rank, 0);
        receiver1->oport1(result);
        
        make_sink("report1", report_func, result);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("subsim2/gen/");
        dumper.traverse(this);
    }
#endif
};
