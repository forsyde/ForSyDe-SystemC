/**********************************************************************
    * vad_types.hpp -- type definitions                               *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          adapted from KisTA: https://github.com/nandohca/kista  *
    *                                                                 *
    * Purpose: Definition of types used in the model                  *
    *                                                                 *
    * Usage:   The VAD example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef VADTYPES_HPP
#define VADTYPES_HPP

#include <forsyde.hpp>
#include "includes/vad.h"
#include <array>
#include <cmath>

template<typename ...T>
using tuple_of_vectors = std::tuple<std::vector<T>...>;

typedef std::array<short,9> r_t;
typedef std::array<short,9> L_av_t;
typedef std::array<short,4> rc_t;
typedef std::tuple<std::array<short,9> , short> rav1_t;
typedef std::tuple<std::array<short,9> , short> rvad_t;
typedef std::tuple<Pfloat,Pfloat> pvad_acf0_t;

std::ostream& operator <<(std::ostream &os,const Pfloat &obj)
{
      os << obj.m * pow(10,obj.e);
      return os;
}

#endif
