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
class DE2DE: public sc_fifo<tt_event<T>>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_channel
#endif
{
public:
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    //! Returns only the size of the actual type (not tt_event version)
    virtual unsigned token_size() const
    {
        return sizeof(T);
    }
    
    //! Returns the name of the actual type (not tt_event version)
    virtual const char* token_type() const
    {
        return get_type_name<T>();
    }
    
    std::string moc() const
    {
        return "DE";
    }
#endif
};

//! The DE_in port is used for input ports of DE processes
template <typename T>
class DE_in: public sc_fifo_in<tt_event<T>>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
    DE_in() : sc_fifo_in<tt_event<T>>(){}
    DE_in(const char* name) : sc_fifo_in<tt_event<T>>(name){}
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    //! Record the bounded channels
    void operator()(sc_fifo_in_if<tt_event<T>>& i)
    {
        sc_fifo_in<tt_event<T>>::operator()(i);
        static_cast<DE2DE<T>&>(i).iport = this;
    }
    
    //! Record the bounded ports
    void operator()(DE_in<T>& p)
    {
        sc_fifo_in<tt_event<T>>::operator()(p);
        p.bound_port = this;
    }
    
    //! Returns the name of the actual type (not tt_event version)
    virtual const char* token_type() const
    {
        return get_type_name<T>();
    }
#endif
};

//! The DE_out port is used for output ports of DE processes
template <typename T>
class DE_out: public sc_fifo_out<tt_event<T>>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
    DE_out() : sc_fifo_out<tt_event<T>>(){}
    DE_out(const char* name) : sc_fifo_out<tt_event<T>>(name){}
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    void operator()(sc_fifo_out_if<tt_event<T>>& i)
    {
        sc_fifo_out<tt_event<T>>::operator()(i);
        // Register the port-to-port binding
        static_cast<DE2DE<T>&>(i).oport = this;
    }
    
    void operator()(DE_out<T>& p)
    {
        sc_fifo_out<tt_event<T>>::operator()(p);
        // Register the port-to-port binding
        p.bound_port = this;
    }
    
    //! Returns the name of the actual type (not tt_event version)
    virtual const char* token_type() const
    {
        return get_type_name<T>();
    }
#endif
};

//! Abstract semantics of a process in the DE MoC
typedef ForSyDe::process de_process;

}
}

#endif
