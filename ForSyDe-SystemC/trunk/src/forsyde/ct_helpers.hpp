/**********************************************************************           
    * ct_helpers.hpp -- Helper primitives in the CT MoC               *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing helper primitives for modeling in the CT MoC *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef CT_HELPERS_HPP
#define CT_HELPERS_HPP

/*! \file ct_helpers.hpp
 * \brief Implements helper primitives for modeling in the CT MoC
 * 
 *  This file includes helper functions which facilliate construction of
 * processes in the continuous-time MoC
 */

#include <functional>

#include "sub_signal.hpp"
#include "ct_process_constructors.hpp"

namespace ForSyDe
{

namespace CT
{

using namespace sc_core;

//! Helper function to construct a comb process
/*! This function is used to construct a process (SystemC module) and
 * connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class I1If>
inline comb* make_comb(std::string pName,
    typename comb::functype _func,
    OIf& outS,
    I1If& inp1S
    )
{
    auto p = new comb(pName.c_str(), _func);
    
    (*p).iport1(inp1S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a comb2 process
/*! This function is used to construct a process (SystemC module) and
 * connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class I1If, class I2If>
inline comb2* make_comb2(std::string pName,
    typename comb2::functype _func,
    OIf& outS,
    I1If& inp1S,
    I2If& inp2S
    )
{
    auto p = new comb2(pName.c_str(), _func);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a delay process
/*! This function is used to construct a process (SystemC module) and
 * connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class IIf, class OIf>
inline delay* make_delay(std::string pName,
    sc_time delay_time,
    OIf& outS,
    IIf& inpS
    )
{
    auto p = new delay(pName.c_str(), delay_time);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a shift process
/*! This function is used to construct a process (SystemC module) and
 * connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class IIf, class OIf>
inline shift* make_shift(std::string pName,
    sc_time delay_time,
    OIf& outS,
    IIf& inpS
    )
{
    auto p = new shift(pName.c_str(), delay_time);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a constant source process
/*! This function is used to construct a constant (SystemC module) and
 * connect its output signal.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the output FIFOs.
 */
template <class OIf>
inline constant* make_constant(std::string pName,
    CTTYPE init_val,
    sc_time end_time,
    OIf& outS
    )
{
    auto p = new constant(pName.c_str(), init_val, end_time);
    
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a source process
/*! This function is used to construct a source (SystemC module) and
 * connect its output signal.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the output FIFOs.
 */
template <class OIf>
inline source* make_source(std::string pName,
    typename source::functype _func,
    const sc_time& end_time,
    OIf& outS
    )
{
    auto p = new source(pName.c_str(), _func, end_time);
    
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a sink process
/*! This function is used to construct a sink (SystemC module) and
 * connect its input signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class IIf>
inline sink* make_sink(std::string pName,
    typename sink::functype _func,
    sc_time sampling_period,
    IIf& inS
    )
{
    auto p = new sink(pName.c_str(), _func, sampling_period);
    
    (*p).iport1(inS);
    
    return p;
}

//! Helper function to construct a traceSig process
/*! This function is used to construct a traceSig (SystemC module) and
 * connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class IIf>
inline traceSig* make_traceSig(std::string pName,
    sc_time sampling_period,
    IIf& inpS
    )
{
    auto p = new traceSig(pName.c_str(), sampling_period);
    
    (*p).iport1(inpS);
    
    return p;
}

//! Helper function to construct a fanout process
/*! This function is used to construct a fanout process (SystemC module) and
 * connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class IIf, class OIf>
inline fanout* make_fanout(std::string pName,
    OIf& outS,
    IIf& inpS
    )
{
    auto p = new fanout(pName.c_str());
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}


}
}

#endif
