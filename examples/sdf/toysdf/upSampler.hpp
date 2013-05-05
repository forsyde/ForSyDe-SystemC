/**********************************************************************           
    * upSampler.hpp -- a 1:2 up sampler                                 *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef UPSAMPLER_HPP
#define UPSAMPLER_HPP

#include <forsyde.hpp>

using namespace ForSyDe::SDF;

class upSampler : public comb<float,float>
{
public:
    upSampler(sc_module_name _name) : comb<float,float>(_name,1,2){}
protected:
    std::vector<float> _func(std::vector<float> a)
    {
      std::vector<float> ret(2);
      ret[0] = a[0];
      ret[1] = a[0];
      return ret;
    }
};

#endif
