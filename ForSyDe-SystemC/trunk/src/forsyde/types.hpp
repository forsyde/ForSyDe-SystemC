/**********************************************************************           
    * types.hpp -- provides a simple type reflection mechanism.       *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se) based on:             *
    * http://stackoverflow.com/questions/1055452/c-get-name-of-type-in-template *
    *                                                                 *
    * Purpose: Provide facilities to store the type names, used in    *
    *          introspection.                                         *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License:                                                        *
    *******************************************************************/

#ifndef TYPES_HPP
#define TYPES_HPP

namespace ForSyDe
{

// The general case uses RTTI (if the type is not registered explicitly)
#pragma once
template<typename T> const char* get_type_name() {return typeid(T).name();}

// Specialization for each type
#define DEFINE_TYPE_NAME(X) \
    template<>const char* get_type_name<X>(){return #X;}

// Specialization for base types

DEFINE_TYPE_NAME(char);
DEFINE_TYPE_NAME(short int);
DEFINE_TYPE_NAME(unsigned short int);
DEFINE_TYPE_NAME(int);
DEFINE_TYPE_NAME(unsigned int);
DEFINE_TYPE_NAME(long int);
DEFINE_TYPE_NAME(unsigned long int);
DEFINE_TYPE_NAME(long long int);
DEFINE_TYPE_NAME(unsigned long long int);
DEFINE_TYPE_NAME(bool);
DEFINE_TYPE_NAME(float);
DEFINE_TYPE_NAME(double);
DEFINE_TYPE_NAME(long double);
DEFINE_TYPE_NAME(wchar_t);


}

#endif
