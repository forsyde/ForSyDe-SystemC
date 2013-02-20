/**********************************************************************
    * Top.hpp -- the top module and testbench for the mulacc example  *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple sequential processes.        *
    *                                                                 *
    * Usage:   Parallel MulAcc example                                *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "mulacc.hpp"
#include <iostream>

using namespace ForSyDe::SY;

SC_MODULE(top)
{
    SY2SY<int> srca, srcb, result;
    
    SC_CTOR(top)
    {
        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size );
        int partner_rank = (world_rank + 1) % 2;
        
        make_constant("constant1", abst_ext<int>(3), 10, srca);
        
        make_receiver<int>("receiver1", partner_rank, 1, srcb);
        
        auto mulacc1 = new mulacc("mulacc1");
        mulacc1->a(srca);
        mulacc1->b(srcb);
        mulacc1->result(result);
        
        sender<int>* sender1 = new sender<int>("sender1", partner_rank, 0);
        sender1->iport1(result);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("subsim1/gen/");
        dumper.traverse(this);
    }
#endif
};
