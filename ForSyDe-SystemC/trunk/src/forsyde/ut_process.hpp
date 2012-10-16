/**********************************************************************           
    * ut_process.hpp -- The UT MoC process                            *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing the primitives for the UT MoC                *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef UT_PROCESS_HPP
#define UT_PROCESS_HPP

/*! \file ut_process.h
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
class UT2UT: public sc_fifo<T>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_channel
#endif
{
public:
    UT2UT() : sc_fifo<T>() {}
    UT2UT(sc_module_name name, unsigned size) : sc_fifo<T>(name, size) {}
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
        return "UT";
    }
#endif
};

//! The SY_in port is used for input ports of SY processes
template <typename T>
class UT_in: public sc_fifo_in<T>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
    UT_in() : sc_fifo_in<T>(){}
    UT_in(const char* name) : sc_fifo_in<T>(name){}
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    //! Record the bounded channels
    void operator()(sc_fifo_in_if<T>& i)
    {
        sc_fifo_in<T>::operator()(i);
        static_cast<UT2UT<T>&>(i).iport = this;
    }
    
    //! Record the bounded ports
    void operator()(UT_in<T>& p)
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
class UT_out: public sc_fifo_out<T>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
    UT_out() : sc_fifo_out<T>(){}
    UT_out(const char* name) : sc_fifo_out<T>(name){}
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    void operator()(sc_fifo_out_if<T>& i)
    {
        sc_fifo_out<T>::operator()(i);
        // Register the port-to-port binding
        static_cast<UT2UT<T>&>(i).oport = this;
    }
    
    void operator()(UT_out<T>& p)
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
typedef ForSyDe::process ut_process;

}
}

#endif
