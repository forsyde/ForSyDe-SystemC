/**********************************************************************           
    * decoder.hpp -- decodes its three inputs                         *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Sorter example                                         *
    *          inspired by material from Doulos SystemC course        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef DECODER_HPP
#define DECODER_HPP

#include <forsyde.hpp>

using namespace ForSyDe::SY;

class decoder : public comb3<bool,bool,bool,int>
{
public:
    decoder(sc_module_name _name) : comb3<bool,bool,bool,int>(_name){}
protected:
    int _func(bool in0, bool in1, bool in2)
    {
        int res = 0;
        if (in0 && !in2) res = 0;
        else if (!in0 && in1) res = 1;
             else if (!in1 && in2) res = 2;
        return res;
    }
};

#endif
