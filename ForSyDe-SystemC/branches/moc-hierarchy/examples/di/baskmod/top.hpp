/**********************************************************************
    * top.hpp -- the top module and testbench for the bask_mod example*
    *                                                                 *
    * Author:  Gilmar Beserra (gilmar@kth.se)                         *
    *          Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: A BASK modulator system.                               *
    *                                                                 *
    * Usage:   bask_mod example                                       *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>
#include "bask_mod.hpp"
#include "bit_src.hpp"

using namespace sc_core;
using namespace ForSyDe;
using namespace ForSyDe::SY;
using namespace ForSyDe::CT;

SC_MODULE(top)
{
	SY2SY<double> from_bit_src;
	
	CT2CT in_bits, wave, to_report1;
		
	SC_CTOR(top)
	{
		make_source("bit_src1", bit_src_func, abst_ext<double>(0), 0, from_bit_src);
        
		auto sy2ct1 = make_SY2CT("sy2ct1", sc_time(100,SC_NS), HOLD, in_bits, from_bit_src);
		sy2ct1->oport1(to_report1);
		
		auto bask_mod1 = new bask_mod("bm");
        bask_mod1->iport1(in_bits);
		bask_mod1->oport1(wave);
		
		make_traceSig("report1", sc_time(5,SC_NS), to_report1);
        
        make_traceSig("report2", sc_time(5,SC_NS), wave);
	}
};
