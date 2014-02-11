/**********************************************************************
    * Top.hpp -- the top module and testbench for the datapar example *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a data parallel model.                *
    *                                                                 *
    * Usage:   datapar example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "inc.hpp"
#include "report.hpp"
//#include "siggen.hpp"

#include <array>

using namespace ForSyDe;
using namespace std;

std::array<int,10> inpval = {{0,1,2,3,4,5,6,7,8,9}};

SC_MODULE(top)
{
    SY::signal<std::array<int,10>> srca, srcb, result;
    
    SC_CTOR(top)
    {
        SY::make_sconstant("constant1", inpval, 10, srca);
        
        SY::make_sdpcomb("parinc1", inc_func, result, srca);
                
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
