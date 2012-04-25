/**********************************************************************           
    * sy_helpers.hpp -- Helper primitives in the SY MoC               *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing helper primitives for modeling in the SY MoC *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SY_HELPERS_HPP
#define SY_HELPERS_HPP

/*! \file sy_helpers.h
 * \brief Implements helper primitives for modeling in the SY MoC
 * 
 *  This file includes helper functions which facilliate construction of
 * processes in the SY MoC
 */

#include <functional>
#include <tuple>

#include "abst_ext.hpp"
#include "sy_process_constructors.hpp"

namespace ForSyDe
{

namespace SY
{

using namespace sc_core;

//! Helper function to construct a comb2 process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class I1If,
          class T2, template <class> class I2If>
inline comb2<T0,T1,T2>* make_comb2(std::string pName,
    typename comb2<T0,T1,T2>::functype _func,
    OIf<T0>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S
    )
{
    auto p = new comb2<T0,T1,T2>(pName.c_str(), _func);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).oport(outS);
    
    return p;
}

//! Helper function to construct a delay process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename T>
inline delay<T>* make_delay(std::string pName,
    abst_ext<T> initval,
    sc_fifo_out_if<abst_ext<T>>& outS,
    sc_fifo_in_if<abst_ext<T>>& inpS
    )
{
    auto p = new delay<T>(pName.c_str(), initval);
    
    (*p).iport1(inpS);
    (*p).oport(outS);
    
    return p;
}

//! Helper function to construct a source process
/*! This function is used to construct a source (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the output FIFOs.
 */
template <class T, template <class> class OIf>
inline source<T>* make_source(std::string pName,
    typename source<T>::functype _func,
    abst_ext<T> initval,
    unsigned long long take,
    OIf<T>& outS
    )
{
    auto p = new source<T>(pName.c_str(), _func, initval, take);
    
    (*p).oport(outS);
    
    return p;
}

//! Helper function to construct a sink process
/*! This function is used to construct a sink (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class T, template <class> class IIf>
inline sink<T>* make_sink(std::string pName,
    typename sink<T>::functype _func,
    IIf<T>& inS
    )
{
    auto p = new sink<T>(pName.c_str(), _func);
    
    (*p).iport1(inS);
    
    return p;
}

}
}

#endif
