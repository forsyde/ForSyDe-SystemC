/**********************************************************************           
    * dt_process.hpp -- The synchronous MoC process                   *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing the primitives for the DT MoC                *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DT_PROCESS_HPP
#define DT_PROCESS_HPP

/*! \file dt_process.hpp
 * \brief Implements the abstract process in the DT Model of Computation
 * 
 *  This files procides definitions for the signals, ports and the
 * abstract base process used in the discrete-time MoC.
 */

#include "abst_ext.hpp"
#include "abssemantics.hpp"

namespace ForSyDe
{

namespace DT
{

using namespace sc_core;

//! The DT2DT signal used to inter-connect DT processes
template <typename T>
class DT2DT: public sc_fifo<abst_ext<T>>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_channel
#endif
{
public:
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    //! Returns only the size of the actual type (not abst_ext version)
    virtual unsigned token_size() const
    {
        return sizeof(T);
    }
    
    //! Returns the name of the actual type (not abst_ext version)
    virtual const char* token_type() const
    {
        return get_type_name<T>();
    }
    
    std::string moc() const
    {
        return "DT";
    }
#endif
};

//! The DT_in port is used for input ports of DT processes
template <typename T>
class DT_in: public sc_fifo_in<abst_ext<T>>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
    DT_in() : sc_fifo_in<abst_ext<T>>(){}
    DT_in(const char* name) : sc_fifo_in<abst_ext<T>>(name){}
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    //! Record the bounded channels
    void operator()(sc_fifo_in_if<abst_ext<T>>& i)
    {
        sc_fifo_in<abst_ext<T>>::operator()(i);
        static_cast<DT2DT<T>&>(i).iport = this;
    }
    
    //! Record the bounded ports
    void operator()(DT_in<T>& p)
    {
        sc_fifo_in<abst_ext<T>>::operator()(p);
        p.bound_port = this;
    }
    
    //! Returns the name of the actual type (not abst_ext version)
    virtual const char* token_type() const
    {
        return get_type_name<T>();
    }
#endif
};

//! The DT_out port is used for output ports of DT processes
template <typename T>
class DT_out: public sc_fifo_out<abst_ext<T>>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
    DT_out() : sc_fifo_out<abst_ext<T>>(){}
    DT_out(const char* name) : sc_fifo_out<abst_ext<T>>(name){}
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    void operator()(sc_fifo_out_if<abst_ext<T>>& i)
    {
        sc_fifo_out<abst_ext<T>>::operator()(i);
        // Register the port-to-port binding
        static_cast<DT2DT<T>&>(i).oport = this;
    }
    
    void operator()(DT_out<T>& p)
    {
        sc_fifo_out<abst_ext<T>>::operator()(p);
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

//! Abstract semantics of a process in the DT MoC
typedef ForSyDe::process dt_process;

}
}

#endif
