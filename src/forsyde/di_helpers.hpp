/**********************************************************************           
    * di_helpers.hpp -- Helper primitives for domain interfaces       *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing helper primitives for modeling DIs           *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DI_HELPERS_HPP
#define DI_HELPERS_HPP

/*! \file di_helpers.hpp
 * \brief Implements helper primitives for modeling with DIs
 * 
 *  This file includes helper functions which facilliate construction of
 * domain interfaces
 */

#include <functional>

#include "dis.hpp"

namespace ForSyDe
{

using namespace sc_core;

//! Helper function to construct an SY2CT domain interface
/*! This function is used to construct a domain interface (SystemC module)
 * from the synchronous to the continuous-time MoC and connect its input
 * and output signals.
 * It provides a more functional style definition of a ForSyDe DI.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class IIf>
inline SY2CT* make_SY2CT(std::string pName,
    sc_time sample_period,     ///< The sampling period
    A2DMode op_mode,          ///< The operation mode
    OIf& outS,
    IIf& inpS
    )
{
    auto p = new SY2CT(pName.c_str(), sample_period, op_mode);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct an CT2SY domain interface
/*! This function is used to construct a domain interface (SystemC module)
 * from the continuous-time to the synchronous MoC and connect its input
 * and output signals.
 * It provides a more functional style definition of a ForSyDe DI.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class IIf>
inline CT2SY* make_CT2SY(std::string pName,
    sc_time sample_period,     ///< The sampling period
    OIf& outS,
    IIf& inpS
    )
{
    auto p = new CT2SY(pName.c_str(), sample_period);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}
}

#endif
