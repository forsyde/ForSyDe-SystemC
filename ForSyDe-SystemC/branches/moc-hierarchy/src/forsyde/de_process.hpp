/**********************************************************************           
    * de_process.hpp -- The DE MoC process                            *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing the primitives for the DE MoC                *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DE_PROCESS_HPP
#define DE_PROCESS_HPP

/*! \file de_process.hpp
 * \brief Implements the abstract process in the DE Model of Computation
 * 
 *  This files procides definitions for the signals, ports ans the
 * abstract base process used in the synchronous MoC.
 */

#include "tt_event.hpp"
#include "abssemantics.hpp"

namespace ForSyDe
{

namespace DE
{

using namespace sc_core;

//! The DE2DE signal used to inter-connect DE processes
template <typename T>
class DE2DE: public ForSyDe::signal<T,tt_event<T>>
{
public:
    DE2DE() : ForSyDe::signal<T,tt_event<T>>() {}
    DE2DE(sc_module_name name, unsigned size) : ForSyDe::signal<T,tt_event<T>>(name, size) {}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "DE";
    }
#endif
};

//! The DE::signal is an alias for DE::DE2DE
template <typename T>
using signal = DE2DE<T>;

//! The DE_in port is used for input ports of DE processes
template <typename T>
class DE_in: public ForSyDe::in_port<T,tt_event<T>,signal<T>>
{
public:
    DE_in() : ForSyDe::in_port<T,tt_event<T>,signal<T>>(){}
    DE_in(const char* name) : ForSyDe::in_port<T,tt_event<T>,signal<T>>(name){}
};

//! The DE::in_port is an alias for DE::DE_in
template <typename T>
using in_port = DE_in<T>;

//! The DE_out port is used for output ports of DE processes
template <typename T>
class DE_out: public ForSyDe::out_port<T,tt_event<T>,signal<T>>
{
public:
    DE_out() : ForSyDe::out_port<T,tt_event<T>,signal<T>>(){}
    DE_out(const char* name) : ForSyDe::out_port<T,tt_event<T>,signal<T>>(name){}
};

//! The DE::out_port is an alias for DE::DE_out
template <typename T>
using out_port = DE_out<T>;

//! Abstract semantics of a process in the DE MoC
typedef ForSyDe::process de_process;

}
}

#endif
