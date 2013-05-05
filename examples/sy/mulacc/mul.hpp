/**********************************************************************
    * mul.hpp -- a multilpier process                                 *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple sequential processes.        *
    *                                                                 *
    * Usage:   MulAcc example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef MUL_HPP
#define MUL_HPP

#include <forsyde.hpp>

using namespace ForSyDe::SY;

class mul : public comb2<int,int,int>
{
public:
    mul(sc_module_name _name) : comb2<int,int,int>(_name){}
protected:
    int _func(int a, int b)
    {
        return a*b;
    }
};

#endif
