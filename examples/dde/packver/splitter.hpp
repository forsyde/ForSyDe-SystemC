/**********************************************************************
    * splitter.hpp -- a splitter composite process                    *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple DDE system.                  *
    *                                                                 *
    * Usage:   Packet Verifier example                                *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef SPLITTER_HPP
#define SPLITTER_HPP

#include <forsyde.hpp>

using namespace ForSyDe;

SC_MODULE(splitter)
{
    DDE::in_port<char> iport1;
    DDE::in_port<char> iport2;
    DDE::out_port<char> oport1;
    DDE::out_port<char> oport2;
    
    DDE::signal<std::tuple<abst_ext<char>,abst_ext<char>>> zout;
    
    SC_CTOR(splitter)
    {        
        DDE::make_mealy2("split", split_ns_func, split_od_func, 'V', SC_ZERO_TIME, zout, iport1, iport2);
        
        DDE::make_unzip("unzip1", zout, oport1, oport2);
    }
    
    static void split_ns_func(char& nst, const char& st, 
        const ttn_event<char>& inp1, const ttn_event<char>& inp2)
    {
        nst = st == 'F' || unsafe_from_abst_ext(get_value(inp1)) == 'F'?
            'F':
            'V';
    }
    
    static void split_od_func(abst_ext<std::tuple<abst_ext<char>,abst_ext<char>>>& out, const char& st, 
        const ttn_event<char>& inp1, const ttn_event<char>& inp2)
    {
        if (st == 'F')
            out = std::tuple<abst_ext<char>,abst_ext<char>>();
        else
            if (unsafe_from_abst_ext(get_value(inp2)) == 'A')
                out = std::make_tuple(abst_ext<char>('A'),abst_ext<char>());
            else if (unsafe_from_abst_ext(get_value(inp2)) == 'B')
                out = std::make_tuple(abst_ext<char>(),abst_ext<char>('B'));
            else
                out = std::tuple<abst_ext<char>,abst_ext<char>>();
    }

};


#endif
