/**********************************************************************           
    * ct_process.hpp -- The CT MoC process                            *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing the primitives for the CT MoC                *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef CT_PROCESS_HPP
#define CT_PROCESS_HPP

/*! \file ct_process.h
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
class CT2CT: public sc_fifo<sub_signal>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_channel
#endif
{
public:
#ifdef FORSYDE_INTROSPECTION
    //! Returns only the size of the actual type
    virtual unsigned token_size() const
    {
        return sizeof(CTTYPE);
    }
    
    //! Returns the name of the actual type
    virtual const char* token_type() const
    {
        return get_type_name<CTTYPE>();
    }
    
    std::string moc() const
    {
        return "CT";
    }
#endif
};

//! The CT_in port is used for input ports of CT processes
class CT_in: public sc_fifo_in<sub_signal>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
    CT_in() : sc_fifo_in<sub_signal>(){}
    CT_in(const char* name) : sc_fifo_in<sub_signal>(name){}
#ifdef FORSYDE_INTROSPECTION
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    //! Record the bounded channels
    void operator()(sc_fifo_in_if<sub_signal>& i)
    {
        sc_fifo_in<sub_signal>::operator()(i);
        static_cast<CT2CT&>(i).iport = this;
    }
    
    //! Record the bounded ports
    void operator()(CT_in& p)
    {
        sc_fifo_in<sub_signal>::operator()(p);
        p.bound_port = this;
    }
    
    //! Returns the name of the actual type (not abst_ext version)
    virtual const char* token_type() const
    {
        return get_type_name<CTTYPE>();
    }
#endif
};

//! The CT_out port is used for output ports of CT processes
class CT_out: public sc_fifo_out<sub_signal>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
    CT_out() : sc_fifo_out<sub_signal>(){}
    CT_out(const char* name) : sc_fifo_out<sub_signal>(name){}
#ifdef FORSYDE_INTROSPECTION
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    void operator()(sc_fifo_out_if<sub_signal>& i)
    {
        sc_fifo_out<sub_signal>::operator()(i);
        // Register the port-to-port binding
        static_cast<CT2CT&>(i).oport = this;
    }
    
    void operator()(CT_out& p)
    {
        sc_fifo_out<sub_signal>::operator()(p);
        // Register the port-to-port binding
        p.bound_port = this;
    }
    
    //! Returns the name of the actual type (not abst_ext version)
    virtual const char* token_type() const
    {
        return get_type_name<CTTYPE>();
    }
#endif
};

//! Abstract semantics of a process in the CT MoC
typedef ForSyDe::process ct_process;

}
}

#endif
