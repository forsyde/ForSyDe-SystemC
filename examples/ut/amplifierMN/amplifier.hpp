/**********************************************************************
    * amplifier.hpp -- a an adaptive amplifier process                *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
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

using namespace ForSyDe;

SC_MODULE(amplifier)
{
    UT::in_port<int>  iport1;
    UT::out_port<int> oport1;
    
    UT::signal<int> s2, s3, s4;
    
    SC_CTOR(amplifier)
    {
        auto A2P1 = new UT::mealyMN<std::tuple<int>,std::tuple<int,int>,std::tuple<>>(
                        "A2P1", A2p_gamma_func, A2p_ns_func, A2p_od_func, std::make_tuple());
        std::get<0>(A2P1->iport)(s3);
        std::get<1>(A2P1->iport)(iport1);
        std::get<0>(A2P1->oport)(s4);
        std::get<0>(A2P1->oport)(oport1);

        auto A3P1 = new UT::mealyMN<std::tuple<int>,std::tuple<int>,std::tuple<int>>(
                        "A3P1", A3p_gamma_func, A3p_ns_func, A3p_od_func, std::make_tuple(10));
        std::get<0>(A3P1->iport)(s4);
        std::get<0>(A3P1->oport)(s2);

        UT::make_delay("A4p", 10, s3, s2);
    }
};

#endif
