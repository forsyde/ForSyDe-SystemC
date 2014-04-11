/**********************************************************************
    * Top.hpp -- the top module and testbench for the datapar example *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a data parallel model.                *
    *                                                                 *
    * Usage:   datapar example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "inc.hpp"
#include "add.hpp"
#include "report.hpp"

#include <array>

using namespace ForSyDe;
using namespace std;

//std::array<int,10> inpval = {{0,1,2,3,4,5,6,7,8,9}};
std::array<int,10000> inpval;

SC_MODULE(top)
{
    SY::signal<std::array<int,10000>> srca, srcb;
    SY::signal<int> result;
    
    SC_CTOR(top)
    {
        inpval.fill(1);
        SY::make_sconstant("constant1", inpval, 10, srca);
        
        SY::make_sdpmap("inc1", inc_func, srcb, srca);
        
        SY::make_sdpreduce("add1", add_func, result, srcb);
        
        SY::make_ssink("report1", report_func, result);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
