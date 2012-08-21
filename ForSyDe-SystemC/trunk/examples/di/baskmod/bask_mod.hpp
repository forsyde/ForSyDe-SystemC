/**********************************************************************
    * bask_mod.hpp -- the BASK mudulator module.                      *
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

#ifndef BASKMODE_HPP
#define BASKMODE_HPP

#include <forsyde.hpp>

using namespace sc_core;
using namespace ForSyDe::CT;

SC_MODULE(bask_mod)
{
	CT_in iport1;
	CT_out oport1;
	
	CT2CT carrier;
	
	SC_CTOR(bask_mod)
	{
        make_sine("sine1", sc_time(3,SC_US), sc_time(100, SC_NS), 1.0, carrier);
		
		make_mul("mixer", oport1, iport1, carrier);
	}
};

#endif
