/**********************************************************************           
    * averager.h -- an averager with feedback from outside            *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a variadic program using zip and unzip*
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef AVERAGER_HPP
#define AVERAGER_HPP

#include <forsyde.hpp>
#include <vector>
#include <tuple>

using namespace ForSyDe::SDF;
using namespace std;

typedef tuple<vector<float>,vector<float>> floattup;

class averager : public comb<floattup,floattup>
{
public:
    averager(sc_module_name _name) : comb<floattup,floattup>(_name,1,1){}
protected:
    vector<floattup> _func(vector<floattup> itup)
    {
      vector<float> a = get<0>(itup[0]);
      vector<float> b = get<1>(itup[0]);
      vector<float> ret(2);
      ret[0] = (a[0]+a[1]+b[0])/3;
      ret[1] = (a[1]+a[2]+b[1])/3;
      vector<floattup> retres = vector<floattup>(1,make_tuple(ret,ret));
      return retres;
    }
};

#endif
