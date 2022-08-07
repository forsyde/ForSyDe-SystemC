/**********************************************************************           
    * sadf_process.hpp -- The SADF MoC process                        *
    *                                                                 *
    * Author:  Mohammad Vazirpanah (mohammad.vazirpanah@yahoo.com)    *
    *                                                                 *
    * Purpose: Providing the primitives for the SADF MoC              *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SADF_PROCESS_HPP
#define SADF_PROCESS_HPP

/*! \file sadf_process.hpp
 * \brief Implements the abstract process in the SADF Model of Computation
 * 
 *  This files procides definitions for the signals, ports ans the
 * abstract base process used in the SADF MoC.
 */

#include "abssemantics.hpp"

namespace ForSyDe
{

namespace SADF
{

using namespace sc_core;

//! The SADF2SADF signal used to inter-connect SADF processes
template <typename T>
class SADF2SADF: public UT::UT2UT<T>
{
public:
    SADF2SADF() : UT::UT2UT<T>() {}
    SADF2SADF(sc_module_name name, unsigned size) : UT::UT2UT<T>(name, size) {}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "SADF";
    }
#endif
};

//! The SADF::signal is an alias for SADF::SADF2SADF
template <typename T>
using signal = SADF2SADF<T>;

//! The SY_in port is used for input ports of SY processes
template <typename T>
class SADF_in: public ForSyDe::UT::UT_in<T>
{
public:
    SADF_in() : ForSyDe::UT::UT_in<T>(){}
    SADF_in(const char* name) : ForSyDe::UT::UT_in<T>(name){}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "SADF";
    }
#endif
};

//! The SADF::in_port is an alias for SADF::SADF_in
template <typename T>
using in_port = SADF_in<T>;

//! The SY_out port is used for output ports of SY processes
template <typename T>
class SADF_out: public ForSyDe::UT::UT_out<T>
{
public:
    SADF_out() : ForSyDe::UT::UT_out<T>(){}
    SADF_out(const char* name) : ForSyDe::UT::UT_out<T>(name){}
#ifdef FORSYDE_INTROSPECTION
    
    virtual std::string moc() const
    {
        return "SADF";
    }
#endif
};

//! The SADF::out_port is an alias for SADF::SADF_out
template <typename T>
using out_port = SADF_out<T>;

//! Abstract semantics of a process in the SY MoC
typedef ForSyDe::process SADF_process;

}
}

#endif
