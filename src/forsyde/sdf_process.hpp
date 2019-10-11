/**********************************************************************           
    * sdf_process.hpp -- The SDF MoC process                          *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing the primitives for the SDF MoC               *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SDF_PROCESS_HPP
#define SDF_PROCESS_HPP

/*! \file sdf_process.hpp
 * \brief Implements the abstract process in the SDF Model of Computation
 * 
 *  This files procides definitions for the signals, ports ans the
 * abstract base process used in the SDF MoC.
 */

#include "abssemantics.hpp"

namespace ForSyDe
{

namespace SDF
{

using namespace sc_core;

//! The SDF2SDF signal used to inter-connect SDF processes
template <typename T>
class SDF2SDF: public UT::UT2UT<T>
{
public:
    SDF2SDF() : UT::UT2UT<T>() {}
    SDF2SDF(sc_module_name name, unsigned size) : UT::UT2UT<T>(name, size) {}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "SDF";
    }
#endif
};

//! The SDF::signal is an alias for SDF::SDF2SDF
template <typename T>
using signal = SDF2SDF<T>;

//! The SY_in port is used for input ports of SY processes
template <typename T>
class SDF_in: public ForSyDe::UT::UT_in<T>
{
public:
    SDF_in() : ForSyDe::UT::UT_in<T>(){}
    SDF_in(const char* name) : ForSyDe::UT::UT_in<T>(name){}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "SDF";
    }
#endif
};

//! The SDF::in_port is an alias for SDF::SDF_in
template <typename T>
using in_port = SDF_in<T>;

//! The SY_out port is used for output ports of SY processes
template <typename T>
class SDF_out: public ForSyDe::UT::UT_out<T>
{
public:
    SDF_out() : ForSyDe::UT::UT_out<T>(){}
    SDF_out(const char* name) : ForSyDe::UT::UT_out<T>(name){}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "SDF";
    }
#endif
};

//! The SDF::out_port is an alias for SDF::SDF_out
template <typename T>
using out_port = SDF_out<T>;

//! Abstract semantics of a process in the SY MoC
typedef ForSyDe::process sdf_process;

}
}

#endif
