/**********************************************************************
    * add.hpp -- an adder process                                     *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple sequential process.          *
    *                                                                 *
    * Usage:   MulAcc example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef ADD_HPP
#define ADD_HPP

#include <forsyde.hpp>

using namespace ForSyDe::SY;

class add : public comb2<int,int,int>
{
public:
    add(sc_module_name _name) : comb2<int,int,int>(_name){}
protected:
    int _func(int a, int b)
    {
        return a+b;
    }
};

#endif
