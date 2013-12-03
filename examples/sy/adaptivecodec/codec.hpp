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
    SY_in<int>  iport;
    SY_in<int>  code;
    SY_out<int> oport;
    
    SY2SY<int> coded;
    SY2SY<tuple<abst_ext<functype>,abst_ext<functype>>> keys;
    SY2SY<functype> key1, key2;
    
    SC_CTOR(codec)
    {
        make_apply("encoder1", coded, iport, key1);
        
        make_apply("decoder1", oport, coded, key2);
        
        make_comb("keygen1", keygen_func, keys, code);
        
        make_unzip("unzip1", keys, key1, key2);
    }
};

#endif
