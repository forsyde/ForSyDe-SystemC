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

DEFINE_TYPE_NAME(r_t,"r_t");
DEFINE_TYPE_NAME(rc_t,"rc_t");
DEFINE_TYPE_NAME(rav1_t,"rav1_t");
DEFINE_TYPE_NAME(Pfloat,"Pfloat");
DEFINE_TYPE_NAME(pvad_acf0_t,"pvad_acf0_t");

//~ DEFINE_TYPE_NAME(tuple_of_vectors<L_av_t,L_av_t>,"r_t.r_t");
template<>const char* get_type_name<tuple_of_vectors<L_av_t,L_av_t>>(){return "r_t.r_t";}
//~ DEFINE_TYPE_NAME(tuple_of_vectors<rav1_t,short,pvad_acf0_t,short,short>,"rav1_t.short.pvad_acf0_t.short.short");
template<>const char* get_type_name<tuple_of_vectors<rav1_t,short,pvad_acf0_t,short,short>>(){return "rav1_t.short.pvad_acf0_t.short.short";}
//~ DEFINE_TYPE_NAME(tuple_of_vectors<pvad_acf0_t,Pfloat>,"pvad_acf0_t.Pfloat");
template<>const char* get_type_name<tuple_of_vectors<pvad_acf0_t,Pfloat>>(){return "pvad_acf0_t.Pfloat";}
//~ DEFINE_TYPE_NAME(tuple_of_vectors<rvad_t,Pfloat>,"rav1_t.Pfloat");
template<>const char* get_type_name<tuple_of_vectors<rvad_t,Pfloat>>(){return "rav1_t.Pfloat";}
//~ DEFINE_TYPE_NAME(tuple_of_vectors<r_t,r_t,r_t,short,short,rc_t,short>,"r_t.r_t.r_t.short.short.rc_t.short");
template<>const char* get_type_name<tuple_of_vectors<r_t,r_t,r_t,short,short,rc_t,short>>(){return "r_t.r_t.r_t.short.short.rc_t.short";}


#endif
