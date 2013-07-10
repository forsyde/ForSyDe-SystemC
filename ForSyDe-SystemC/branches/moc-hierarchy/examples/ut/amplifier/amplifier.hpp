/**********************************************************************
    * amplifier.hpp -- a an adaptive amplifier process                *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *          taken from the book by Axel Jantsch (p. 114-122)       *
    *                                                                 *
    * Purpose: Demonstration of a simple example in the untimed MoC.  *
    *                                                                 *
    * Usage:   amplifier example                                      *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef AMPLIFIER_HPP
#define AMPLIFIER_HPP

#include <forsyde.hpp>
#include "A2p.hpp"
#include "A3p.hpp"

using namespace ForSyDe::UT;

SC_MODULE(amplifier)
{
    UT_in<int>  iport1;
    UT_out<int> oport1;
    
    UT2UT<std::tuple<std::vector<int>,std::vector<int>>> s1;
    UT2UT<int> s2, s3, s4;
    
    SC_CTOR(amplifier)
    {
        make_zip("A1p", 1, 5, s1, s3, iport1);
        
        auto A2p1 = make_comb("A2p1", A2p_func, 1, s4, s1);
        A2p1->oport1(oport1);

        make_scan("A3p1", A3p_gamma_func, A3p_ns_func, 10, s2, s4);
        
        make_delay("A4p", 10, s3, s2);
    }
};

#endif
