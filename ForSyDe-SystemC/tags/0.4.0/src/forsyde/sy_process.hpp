/**********************************************************************           
    * sy_process.hpp -- The synchronous MoC process                   *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing the primitives for the SY MoC                *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SY_PROCESS_HPP
#define SY_PROCESS_HPP

/*! \file sy_process.hpp
 * \brief Implements the abstract process in the synchronous Model of Computation
 * 
 *  This files procides definitions for the signals, ports ans the
 * abstract base process used in the synchronous MoC.
 */

#include "abst_ext.hpp"
#include "abssemantics.hpp"

namespace ForSyDe
{

namespace SY
{

using namespace sc_core;

//! The SY2SY signal used to inter-connect SY processes
template <typename T>
class SY2SY: public ForSyDe::signal<T,abst_ext<T>>
{
public:
    SY2SY() : ForSyDe::signal<T,abst_ext<T>>() {}
    SY2SY(sc_module_name name, unsigned size) : ForSyDe::signal<T,abst_ext<T>>(name, size) {}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "SY";
    }
#endif
};

//! The SY::signal is an alias for SY::SY2SY
template <typename T>
using signal = SY2SY<T>;

//! The SY_in port is used for input ports of SY processes
template <typename T>
class SY_in: public ForSyDe::in_port<T,abst_ext<T>,signal<T>>
{
public:
    SY_in() : ForSyDe::in_port<T,abst_ext<T>,signal<T>>(){}
    SY_in(const char* name) : ForSyDe::in_port<T,abst_ext<T>,signal<T>>(name){}
};

//! The SY::in_port is an alias for SY::SY_in
template <typename T>
using in_port = SY_in<T>;

//! The SY_out port is used for output ports of SY processes
template <typename T>
class SY_out: public ForSyDe::out_port<T,abst_ext<T>,signal<T>>
{
public:
    SY_out() : ForSyDe::out_port<T,abst_ext<T>,signal<T>>(){}
    SY_out(const char* name) : ForSyDe::out_port<T,abst_ext<T>,signal<T>>(name){}
};

//! The SY::out_port is an alias for SY::SY_out
template <typename T>
using out_port = SY_out<T>;

//! Abstract semantics of a process in the SY MoC
typedef ForSyDe::process sy_process;

}
}

#endif
