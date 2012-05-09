/**********************************************************************           
    * symoc.hpp -- The synchronous MoC process                        *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing the primitives for the SY MoC                *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SY_PROCESS_HPP
#define SY_PROCESS_HPP

/*! \file sy_process.h
 * \brief Implements the abstract process in the synchronous Model of Computation
 * 
 *  This files procides definitions for the signals, ports ans the
 * abstract base process used in the synchronous MoC.
 */

#include "abst_ext.hpp"
#include "abssemantics.hpp"

namespace ForSyDe
{

namespace SY
{

using namespace sc_core;

//! The SY2SY signal used to inter-connect SY processes
template <typename T>
class SY2SY: public sc_fifo<abst_ext<T>>
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
    
    std::string moc() const
    {
        return "SY";
    }
#endif
};

//! The SY_in port is used for input ports of SY processes
template <typename T>
class SY_in: public sc_fifo_in<abst_ext<T>>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    //! Record the bounded channels
    void operator()(sc_fifo_in_if<abst_ext<T>>& i)
    {
        sc_fifo_in<abst_ext<T>>::operator()(i);
        static_cast<SY2SY<T>&>(i).iport = this;
    }
    
    //! Record the bounded ports
    void operator()(SY_in<T>& p)
    {
        sc_fifo_in<abst_ext<T>>::operator()(p);
        p.bound_port = this;
    }
#endif
};

//! The SY_out port is used for output ports of SY processes
template <typename T>
class SY_out: public sc_fifo_out<abst_ext<T>>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    void operator()(sc_fifo_out_if<abst_ext<T>>& i)
    {
        sc_fifo_out<abst_ext<T>>::operator()(i);
        // Register the port-to-port binding
        static_cast<SY2SY<T>&>(i).oport = this;
    }
    
    void operator()(SY_out<T>& p)
    {
        sc_fifo_out<abst_ext<T>>::operator()(p);
        // Register the port-to-port binding
        p.bound_port = this;
    }
#endif
};

//! Abstract semantics of a process in the SY MoC
typedef ForSyDe::process sy_process;

}
}

#endif
