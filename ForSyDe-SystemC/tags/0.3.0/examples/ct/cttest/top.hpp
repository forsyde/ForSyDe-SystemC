/**********************************************************************           
    * top.hpp -- the Top process and testbench for a toy CT system    *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy CT example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>
#include "add.hpp"
#include "pwr.hpp"

using namespace sc_core;
using namespace ForSyDe::CT;

void abssin_func(CTTYPE& out1, const sc_time& inp1)
{
#pragma ForSyDe begin abssin_func
    out1 = fabs(sin(2*M_PI*inp1.to_seconds()));
#pragma ForSyDe end
}

SC_MODULE(top)
{
    CT2CT src1, src2, src3, des1, des2;
    
    SC_CTOR(top)
    {
        auto stimuli1 = make_source("stimuli1", abssin_func, sc_time(3,SC_SEC), src1);
        stimuli1->oport1(src2);
        stimuli1->oport1(src3);
        
        make_comb2("add1", add_func, des1, src2, src3);
        
        make_comb("pwr1", pwr_func, des2, des1);
        
        make_traceSig("report1", sc_time(10,SC_MS), src1);
        
        make_traceSig("report2", sc_time(10,SC_MS), des2);
    }
   
};
