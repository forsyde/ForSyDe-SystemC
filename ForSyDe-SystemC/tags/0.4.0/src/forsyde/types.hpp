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

/*! \file types.hpp
 * \brief Provides facilities for basic type introspection
 * 
 *  This file includes a a set of basic facilities for registering names
 * for non-POD C/C++ types to be reflected in the XML output of the
 * interospection stage.
 */

//~ namespace ForSyDe
//~ {

// The general case uses RTTI (if the type is not registered explicitly)
#pragma once
template<typename T> const char* get_type_name() {return typeid(T).name();}

// Specialization for each type
#define DEFINE_TYPE(X) \
    template<>const char* get_type_name<X>(){return #X;}
// Another version where we explicitly provide the type name (for complex types)
#define DEFINE_TYPE_NAME(X,N) \
    template<>const char* get_type_name<X>(){return N;}

// Specialization for base types

DEFINE_TYPE(char);
DEFINE_TYPE(short int);
DEFINE_TYPE(unsigned short int);
DEFINE_TYPE(int);
DEFINE_TYPE(unsigned int);
DEFINE_TYPE(long int);
DEFINE_TYPE(unsigned long int);
DEFINE_TYPE(long long int);
DEFINE_TYPE(unsigned long long int);
DEFINE_TYPE(bool);
DEFINE_TYPE(float);
DEFINE_TYPE(double);
DEFINE_TYPE(long double);
DEFINE_TYPE(wchar_t);


//~ }

#endif
