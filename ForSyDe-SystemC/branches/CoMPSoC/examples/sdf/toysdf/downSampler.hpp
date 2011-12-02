/**********************************************************************           
    * downSampler.hpp -- a 3:2 down sampler                           *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DOWNSAMPLER_HPP
#define DOWNSAMPLER_HPP

#include <forsyde.hpp>

using namespace ForSyDe::SDF;

class downSampler : public comb<float,float>
{
public:
    downSampler(sc_module_name _name) : comb<float,float>(_name,3,2){}
protected:
#pragma ForSyDe begin downSampler_func
    std::vector<float> _func(std::vector<float> a)
    {
      std::vector<float> ret(2);
      ret[0] = a[0];
      ret[1] = a[1];
      return ret;
    }
#pragma ForSyDe end
};

#endif
