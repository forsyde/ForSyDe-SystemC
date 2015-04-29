/**********************************************************************           
    * dde_process.hpp -- The DDE MoC process                          *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing the primitives for the DDE MoC                *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DDE_PROCESS_HPP
#define DDE_PROCESS_HPP

/*! \file dde_process.hpp
 * \brief Implements the abstract process in the DDE Model of Computation
 * 
 *  This files procides definitions for the signals, ports ans the
 * abstract base process used in the distributed discrete-event MoC.
 */

#include "tt_event.hpp"
#include "abssemantics.hpp"

namespace ForSyDe
{

namespace DDE
{

using namespace sc_core;

//! The DDE2DDE signal used to inter-connect DDE processes
template <typename T>
class DDE2DDE: public ForSyDe::signal<T,ttn_event<T>>
{
public:
    DDE2DDE() : ForSyDe::signal<T,ttn_event<T>>() {}
    DDE2DDE(sc_module_name name, unsigned size) : ForSyDe::signal<T,ttn_event<T>>(name, size) {}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "DDE";
    }
#endif
};

//! The DDE::signal is an alias for DDE::DDE2DDE
template <typename T>
using signal = DDE2DDE<T>;

//! The DDE_in port is used for input ports of DDE processes
template <typename T>
class DDE_in: public ForSyDe::in_port<T,ttn_event<T>,signal<T>>
{
public:
    DDE_in() : ForSyDe::in_port<T,ttn_event<T>,signal<T>>(){}
    DDE_in(const char* name) : ForSyDe::in_port<T,ttn_event<T>,signal<T>>(name){}
};

//! The DDE::in_port is an alias for DDE::DDE_in
template <typename T>
using in_port = DDE_in<T>;

//! The DDE_out port is used for output ports of DDE processes
template <typename T>
class DDE_out: public ForSyDe::out_port<T,ttn_event<T>,signal<T>>
{
public:
    DDE_out() : ForSyDe::out_port<T,ttn_event<T>,signal<T>>(){}
    DDE_out(const char* name) : ForSyDe::out_port<T,ttn_event<T>,signal<T>>(name){}
};

//! The DDE::out_port is an alias for DDE::DDE_out
template <typename T>
using out_port = DDE_out<T>;

//! Abstract semantics of a process in the DDE MoC
typedef ForSyDe::process dde_process;

}
}

#endif
