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
class SY2SY: public sc_fifo<abst_ext<T>>, public ForSyDe::channel_size
{
public:
    typedef T type;
    
    //! Returns only the size of the actual type (not abst_ext version)
    virtual unsigned token_size()
    {
        return sizeof(T);
    }
};

//! The SY_in port is used for input ports of SY processes
template <typename T>
class SY_in: public sc_fifo_in<abst_ext<T>>
{
public:
    typedef T type;
};

//! The SY_out port is used for output ports of SY processes
template <typename T>
class SY_out: public sc_fifo_out<abst_ext<T>>
{
public:
    typedef T type;
};

//! Information of port types in the SY MoC
struct SY_PortInfo
{
    sc_object* port;
    std::vector<sc_object*> boundChans;
    std::string portType;
};

//! Abstract semantics of a process in the SY MoC
typedef ForSyDe::process<SY_PortInfo> SY_process;

}
}

#endif
