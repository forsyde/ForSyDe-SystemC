/**********************************************************************
    * keygen.hpp -- a key generator fr adaptive encoding              *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple adaptive process.            *
    *                                                                 *
    * Usage:   Adaptive Codec example                                 *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef KEYGEN_HPP
#define KEYGEN_HPP

#include <forsyde.hpp>
#include <tuple>

using namespace ForSyDe::SY;
using namespace std;

typedef std::function<int(const int&)> functype;

class keygen : public comb<int,tuple<functype,functype>>
{
public:
    keygen(sc_module_name _name) : comb<int,tuple<functype,functype>>(_name){}
protected:
    tuple<functype,functype> _func(int a)
    {
        auto key1 = [=](int x){return x+a;};
        auto key2 = [=](int x){return x-a;};
        
        return make_tuple(key1,key2);
    }
};

#endif
