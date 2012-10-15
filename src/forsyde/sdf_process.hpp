/**********************************************************************           
    * sdf_process.hpp -- The SDF MoC process                          *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing the primitives for the SDF MoC               *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SDF_PROCESS_HPP
#define SDF_PROCESS_HPP

/*! \file sdf_process.h
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
class SDF2SDF: public sc_fifo<T>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_channel
#endif
{
public:
    SDF2SDF() : sc_fifo<T>() {}
    SDF2SDF(sc_module_name name, unsigned size) : sc_fifo<T>(name, size) {}
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    //! Returns only the size of the token type
    virtual unsigned token_size() const
    {
        return sizeof(T);
    }
    
    //! Returns the name of the token type
    virtual const char* token_type() const
    {
        return get_type_name<T>();
    }
    
    std::string moc() const
    {
        return "SDF";
    }
#endif
};

//! The SY_in port is used for input ports of SY processes
template <typename T>
class SDF_in: public sc_fifo_in<T>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
    SDF_in() : sc_fifo_in<T>(){}
    SDF_in(const char* name) : sc_fifo_in<T>(name){}
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    //! Record the bounded channels
    void operator()(sc_fifo_in_if<T>& i)
    {
        sc_fifo_in<T>::operator()(i);
        static_cast<SDF2SDF<T>&>(i).iport = this;
    }
    
    //! Record the bounded ports
    void operator()(SDF_in<T>& p)
    {
        sc_fifo_in<T>::operator()(p);
        p.bound_port = this;
    }
    
    //! Returns the name of the token type
    virtual const char* token_type() const
    {
        return get_type_name<T>();
    }
#endif
};

//! The SY_out port is used for output ports of SY processes
template <typename T>
class SDF_out: public sc_fifo_out<T>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
    SDF_out() : sc_fifo_out<T>(){}
    SDF_out(const char* name) : sc_fifo_out<T>(name){}
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    void operator()(sc_fifo_out_if<T>& i)
    {
        sc_fifo_out<T>::operator()(i);
        // Register the port-to-port binding
        static_cast<SDF2SDF<T>&>(i).oport = this;
    }
    
    void operator()(SDF_out<T>& p)
    {
        sc_fifo_out<T>::operator()(p);
        // Register the port-to-port binding
        p.bound_port = this;
    }
    
    //! Returns the name of the actual type (not abst_ext version)
    virtual const char* token_type() const
    {
        return get_type_name<T>();
    }
#endif
};

//! Abstract semantics of a process in the SY MoC
typedef ForSyDe::process sdf_process;

}
}

#endif
