/**********************************************************************
    * codec.hpp -- an adaptive encoder/decoder                        *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple adaptive system.             *
    *                                                                 *
    * Usage:   Adaptive Codec example                                 *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef CODEC_HPP
#define CODEC_HPP

#include <forsyde.hpp>
#include <tuple>

#include "keygen.hpp"

using namespace ForSyDe::SY;
using namespace std;

SC_MODULE(codec)
{
    sc_fifo_in<int>  iport;
    sc_fifo_in<int>  code;
    sc_fifo_out<int> oport;
    
    apply<int,int> encoder;
    apply<int,int> decoder;
    keygen keygen1;
    unzip<functype,functype> unzip1;
    
    sc_fifo<int> coded;
    sc_fifo<tuple<functype,functype>> keys;
    sc_fifo<functype> key1, key2;
    
    SC_CTOR(codec): encoder("encoder"), decoder("decoder"),
                    keygen1("keygen1"), unzip1("unzip1")
    {
        encoder.iport(iport);
        encoder.fport(key1);
        encoder.oport(coded);
        
        decoder.iport(coded);
        decoder.fport(key2);
        decoder.oport(oport);
        
        keygen1.iport(code);
        keygen1.oport(keys);
        
        unzip1.iport(keys);
        unzip1.oport1(key1);
        unzip1.oport2(key2);
    }
};

#endif
