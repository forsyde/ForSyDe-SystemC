/**********************************************************************
    * buf_add.hpp -- an adder process with buffered inputs            *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple DDE system.                  *
    *                                                                 *
    * Usage:   ToyDDE example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef BUF_ADD_HPP
#define BUF_ADD_HPP

#include <forsyde.hpp>

using namespace ForSyDe;

void buf_add_ns_func(std::tuple<int,int>& next_vals, 
    const std::tuple<int,int>& prev_vals,
    const ttn_event<int> inp1,
    const ttn_event<int> inp2
)
{
    std::get<0>(next_vals) = from_abst_ext(get_value(inp1), std::get<0>(prev_vals));
    std::get<1>(next_vals) = from_abst_ext(get_value(inp2), std::get<1>(prev_vals));
}

void buf_add_od_func(abst_ext<int>& out1, 
    const std::tuple<int,int>& prev_vals,
    const ttn_event<int> inp1,
    const ttn_event<int> inp2
)
{
    out1 = abst_ext<int>(
        from_abst_ext(get_value(inp1), std::get<0>(prev_vals)) +
        from_abst_ext(get_value(inp2), std::get<1>(prev_vals))
    );
}

#endif
