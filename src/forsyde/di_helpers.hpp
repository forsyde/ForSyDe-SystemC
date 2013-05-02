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

//! Helper function to construct an CT2DE domain interface
/*! This function is used to construct a domain interface (SystemC module)
 * from the continuous-time to the discrete-event MoC and connect its
 * input and output signals.
 * It provides a more functional style definition of a ForSyDe DI.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T, template <class> class OIf, class IIf>
inline CT2DE<T>* make_CT2DE(std::string pName,
    OIf<T>& outS,
    IIf& inpS
    )
{
    auto p = new CT2DE<T>(pName.c_str());
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct an DE2CT domain interface
/*! This function is used to construct a domain interface (SystemC module)
 * from the discrete-event MoC to the continuous-time and connect its
 * input and output signals.
 * It provides a more functional style definition of a ForSyDe DI.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T, class OIf, template <class> class IIf>
inline DE2CT<T>* make_DE2CT(std::string pName,
    A2DMode op_mode,    ///< The operation mode
    OIf& outS,
    IIf<T>& inpS
    )
{
    auto p = new DE2CT<T>(pName.c_str(), op_mode);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct an SY2SDF domain interface
/*! This function is used to construct a domain interface (SystemC module)
 * from the synchronous to synchronous dataflow MoC and connect its
 * input and output signals.
 * It provides a more functional style definition of a ForSyDe DI.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T, template <class> class OIf, template <class> class IIf>
inline SY2SDF<T>* make_SY2SDF(std::string pName,
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new SY2SDF<T>(pName.c_str());
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct an SDF2SY domain interface
/*! This function is used to construct a domain interface (SystemC module)
 * from the synchronous dataflow MoC to the synchronous and connect its
 * input and output signals.
 * It provides a more functional style definition of a ForSyDe DI.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T, template <class> class OIf, template <class> class IIf>
inline SDF2SY<T>* make_SDF2SY(std::string pName,
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new SDF2SY<T>(pName.c_str());
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct an SY2DE domain interface
/*! This function is used to construct a domain interface (SystemC module)
 * from the synchronous to discrete-event MoC and connect its
 * input and output signals.
 * It provides a more functional style definition of a ForSyDe DI.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T, template <class> class OIf, template <class> class IIf>
inline SY2DE<T>* make_SY2DE(std::string pName,
    sc_time sample_period,    ///< The unified period length
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new SY2DE<T>(pName.c_str(), sample_period);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct an DE2SY domain interface
/*! This function is used to construct a domain interface (SystemC module)
 * from the discrete-event MoC to the synchronous and connect its
 * input and output signals.
 * It provides a more functional style definition of a ForSyDe DI.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T, template <class> class OIf, template <class> class IIf>
inline DE2SY<T>* make_DE2SY(std::string pName,
    sc_time sample_period,    ///< The unified period length
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new DE2SY<T>(pName.c_str(), sample_period);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

}

#endif
