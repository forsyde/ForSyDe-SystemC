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

using namespace ForSyDe;
using namespace ForSyDe::SY;
using namespace std;

typedef std::function<abst_ext<int>(const abst_ext<int>&)> functype;

void keygen_func(abst_ext<tuple<abst_ext<functype>,abst_ext<functype>>>& out1,
    const abst_ext<int>& inp)
{
    int inp1 = from_abst_ext(inp,0);
#pragma ForSyDe begin siggen_func
    auto key1 = abst_ext<functype>([=](abst_ext<int> x){
            return abst_ext<int>(unsafe_from_abst_ext(x)+inp1);
        });
    auto key2 = abst_ext<functype>([=](abst_ext<int> x){
            return abst_ext<int>(unsafe_from_abst_ext(x)-inp1);
        });
    out1 = abst_ext<tuple<abst_ext<functype>,abst_ext<functype>>>(make_tuple(key1,key2));
#pragma ForSyDe end
}

#endif
