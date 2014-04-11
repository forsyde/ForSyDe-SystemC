/**********************************************************************           
    * fir.hpp -- a digital FIR filter                                 *
    *                                                                 *
    * Authors: Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a heterogeneous system.               *
    *                                                                 *
    * Usage:   The digital filter example                             *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>

#include "globals.hpp"
#include "add.hpp"
#include "mul.hpp"

using namespace sc_core;
using namespace ForSyDe;

#define TAPS 5

SC_MODULE(fir)
{
    SY_in<double> iport1;
    SY_out<double> oport1;
    
    std::vector<SY2SY<double>> del_line;
    std::vector<SY2SY<double>> coef_line;
    std::vector<SY2SY<double>> coef_src_line;
    std::vector<SY2SY<double>> mac_line;
    std::vector<SY2SY<double>> res_line;
    
    SC_CTOR(fir): del_line(TAPS-1),
                  coef_line(TAPS),
                  coef_src_line(TAPS),
                  mac_line(TAPS-1),
                  res_line(TAPS-1)
    
    {
        auto fo = make_fanout("fo", del_line[0], iport1);
        fo->oport1(coef_line[0]);
        
        SY::make_constant("coef0", abst_ext<double>(coefs[0]), 0, coef_src_line[0]);
        
        SY::make_comb2("mul0", mul_func, res_line[0], coef_line[0], coef_src_line[0]);
        
        for (int i=0; i<TAPS-1; i++)
        {
            auto del = SY::make_delay("del_line"+std::to_string(i), abst_ext<double>(0), coef_line[i+1], del_line[i]);
            if (i<TAPS-1-1) del->oport1(del_line[i+1]);
            
            SY::make_constant("coef"+std::to_string(i+1), abst_ext<double>(coefs[i+1]), 0, coef_src_line[i+1]);
            
            SY::make_comb2("mul"+std::to_string(i+1), mul_func, mac_line[i], coef_line[i+1], coef_src_line[i+1]);
            
            if (i<TAPS-1-1)
                SY::make_comb2("add"+std::to_string(i), add_func, res_line[i+1], mac_line[i], res_line[i]);
            else
                SY::make_comb2("add"+std::to_string(i), add_func, oport1, mac_line[i], res_line[i]);
        }
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
