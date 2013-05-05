/**********************************************************************           
    * averager.hpp -- an averager with feedback from outside          *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef AVERAGER_HPP
#define AVERAGER_HPP

#include <forsyde.hpp>

using namespace ForSyDe::SDF;

class averager : public comb2<float,float,float>
{
public:
    averager(sc_module_name _name) : comb2<float,float,float>(_name,3,2,2){}
protected:
    std::vector<float> _func(std::vector<float> a, std::vector<float> b)
    {
      std::vector<float> ret(2);
      ret[0] = (a[0]+a[1]+b[0])/3;
      ret[1] = (a[1]+a[2]+b[1])/3;
      return ret;
    }
};

#endif
