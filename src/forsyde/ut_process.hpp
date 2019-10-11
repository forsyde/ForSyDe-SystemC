/**********************************************************************           
    * ut_process.hpp -- The UT MoC process                            *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing the primitives for the UT MoC                *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef UT_PROCESS_HPP
#define UT_PROCESS_HPP

/*! \file ut_process.hpp
 * \brief Implements the abstract process in the UT Model of Computation
 * 
 *  This files procides definitions for the signals, ports ans the
 * abstract base process used in the untimed MoC.
 */

#include "abssemantics.hpp"

namespace ForSyDe
{

namespace UT
{

using namespace sc_core;

//! The UT2UT signal used to inter-connect UT processes
template <typename T>
class UT2UT: public ForSyDe::signal<T,T>
{
public:
    UT2UT() : ForSyDe::signal<T,T>() {}
    UT2UT(sc_module_name name, unsigned size) : ForSyDe::signal<T,T>(name, size) {}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "UT";
    }
#endif
};

//! The UT::signal is an alias for UT::UT2UT
template <typename T>
using signal = UT2UT<T>;

//! The UT_in port is used for input ports of UT processes
template <typename T>
class UT_in: public ForSyDe::in_port<T,T,signal<T>>
{
public:
    UT_in() : ForSyDe::in_port<T,T,signal<T>>(){}
    UT_in(const char* name) : ForSyDe::in_port<T,T,signal<T>>(name){}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "UT";
    }
#endif
};

//! The UT::in_port is an alias for UT::UT_in
template <typename T>
using in_port = UT_in<T>;

//! The UT_out port is used for output ports of UT processes
template <typename T>
class UT_out: public ForSyDe::out_port<T,T,signal<T>>
{
public:
    UT_out() : ForSyDe::out_port<T,T,signal<T>>(){}
    UT_out(const char* name) : ForSyDe::out_port<T,T,signal<T>>(name){}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "UT";
    }
#endif
};

//! The UT::out_port is an alias for UT::UT_out
template <typename T>
using out_port = UT_out<T>;

//! Abstract semantics of a process in the SY MoC
typedef ForSyDe::process ut_process;

}
}

#endif
