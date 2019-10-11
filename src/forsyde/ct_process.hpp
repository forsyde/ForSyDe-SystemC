/**********************************************************************           
    * ct_process.hpp -- The CT MoC process                            *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing the primitives for the CT MoC                *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef CT_PROCESS_HPP
#define CT_PROCESS_HPP

/*! \file ct_process.hpp
 * \brief Implements the abstract process in the CT Model of Computation
 * 
 *  This files procides definitions for the signals, ports ans the
 * abstract base process used in the continuous-time MoC.
 */

#include "sub_signal.hpp"
#include "abssemantics.hpp"

namespace ForSyDe
{

namespace CT
{

using namespace sc_core;

//! The CT2CT signal used to inter-connect CT processes
class CT2CT: public ForSyDe::signal<double,sub_signal>
{
public:
    CT2CT() : ForSyDe::signal<double,sub_signal>() {}
    CT2CT(sc_module_name name, unsigned size) : ForSyDe::signal<double,sub_signal>(name, size) {}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "CT";
    }
#endif
};

//! The CT::signal is an alias for CT::CT2CT
using signal = CT2CT;

//! The CT_in port is used for input ports of CT processes
class CT_in: public ForSyDe::in_port<double,sub_signal,signal>
{
public:
    CT_in() : ForSyDe::in_port<double,sub_signal,signal>(){}
    CT_in(const char* name) : ForSyDe::in_port<double,sub_signal,signal>(name){}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "CT";
    }
#endif
};

//! The CT::in_port is an alias for CT::CT_in
using in_port = CT_in;

//! The CT_out port is used for output ports of CT processes
class CT_out: public ForSyDe::out_port<double,sub_signal,signal>
{
public:
    CT_out() : ForSyDe::out_port<double,sub_signal,signal>(){}
    CT_out(const char* name) : ForSyDe::out_port<double,sub_signal,signal>(name){}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "CT";
    }
#endif
};

//! The CT::out_port is an alias for CT::CT_out
using out_port = CT_out;

//! Abstract semantics of a process in the CT MoC
typedef ForSyDe::process ct_process;

}
}

#endif
