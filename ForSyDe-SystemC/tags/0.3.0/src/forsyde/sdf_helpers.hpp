/**********************************************************************           
    * sdf_helpers.hpp -- Helper primitives in the SDF MoC             *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing helper primitives for modeling in the SDF MoC*
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SDF_HELPERS_HPP
#define SDF_HELPERS_HPP

/*! \file sdf_helpers.hpp
 * \brief Implements helper primitives for modeling in the SDF MoC
 * 
 *  This file includes helper functions which facilliate construction of
 * processes in the SY MoC
 */

#include <functional>
#include <tuple>

#include "sdf_process_constructors.hpp"

namespace ForSyDe
{

namespace SDF
{

using namespace sc_core;

//! Helper function to construct a comb process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class I1If>
inline comb<T0,T1>* make_comb(std::string pName,    ///< process name
    typename comb<T0,T1>::functype _func,          ///< function to be passed
    unsigned int o1toks,                            ///< consumption rate for the first output
    unsigned int i1toks,                            ///< consumption rate for the first input
    OIf<T0>& outS,                                   ///< the first output signal
    I1If<T1>& inp1S                                  ///< the first input signal
    )
{
    auto p = new comb<T0,T1>(pName.c_str(), _func, o1toks, i1toks);
    
    (*p).iport1(inp1S);
    (*p).oport1(outS);
    
    return p;
}

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
inline comb2<T0,T1,T2>* make_comb2(std::string pName,///< process name
    typename comb2<T0,T1,T2>::functype _func,      ///< function to be passed
    unsigned int o1toks,                            ///< consumption rate for the first output
    unsigned int i1toks,                            ///< consumption rate for the first input
    unsigned int i2toks,                            ///< consumption rate for the second input
    OIf<T0>& outS,                                   ///< the first output signal
    I1If<T1>& inp1S,                                 ///< the first input signal
    I2If<T2>& inp2S                                  ///< the second input signal
    )
{
    auto p = new comb2<T0,T1,T2>(pName.c_str(), _func, o1toks, 
                                  i1toks, i2toks);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a comb3 process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class I1If,
          class T2, template <class> class I2If,
          class T3, template <class> class I3If>
inline comb3<T0,T1,T2,T3>* make_comb3(std::string pName,///< process name
    typename comb3<T0,T1,T2,T3>::functype _func,   ///< function to be passed
    unsigned int o1toks,                            ///< consumption rate for the first output
    unsigned int i1toks,                            ///< consumption rate for the first input
    unsigned int i2toks,                            ///< consumption rate for the second input
    unsigned int i3toks,                            ///< consumption rate for the third input
    OIf<T0>& outS,                                   ///< the first output signal
    I1If<T1>& inp1S,                                 ///< the first input signal
    I2If<T2>& inp2S,                                 ///< the second input signal
    I3If<T3>& inp3S                                  ///< the third input signal
    )
{
    auto p = new comb3<T0,T1,T2,T3>(pName.c_str(), _func, o1toks,
                                     i1toks, i2toks, i3toks);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).iport3(inp3S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a comb4 process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class I1If,
          class T2, template <class> class I2If,
          class T3, template <class> class I3If,
          class T4, template <class> class I4If>
inline comb4<T0,T1,T2,T3,T4>* make_comb4(std::string pName,///< process name
    typename comb4<T0,T1,T2,T3,T4>::functype _func,///< function to be passed
    unsigned int o1toks,                            ///< consumption rate for the first output
    unsigned int i1toks,                            ///< consumption rate for the first input
    unsigned int i2toks,                            ///< consumption rate for the second input
    unsigned int i3toks,                            ///< consumption rate for the third input
    unsigned int i4toks,                            ///< consumption rate for the fourth input
    OIf<T0>& outS,                                   ///< the first output signal
    I1If<T1>& inp1S,                                 ///< the first input signal
    I2If<T2>& inp2S,                                 ///< the second input signal
    I3If<T3>& inp3S,                                 ///< the third input signal
    I4If<T4>& inp4S                                  ///< the fourth input signal
    )
{
    auto p = new comb4<T0,T1,T2,T3,T4>(pName.c_str(), _func, o1toks,
                                     i1toks, i2toks, i3toks, i4toks);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).iport3(inp3S);
    (*p).iport4(inp4S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a delay process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename T, template <class> class IIf,
                        template <class> class OIf>
inline delay<T>* make_delay(std::string pName,
    T initval,
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new delay<T>(pName.c_str(), initval);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a delayn process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename T, template <class> class IIf,
                        template <class> class OIf>
inline delayn<T>* make_delayn(std::string pName,
    T initval,
    unsigned int n,
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new delayn<T>(pName.c_str(), initval, n);
    
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
template <class T, template <class> class OIf>
inline constant<T>* make_constant(std::string pName,
    T initval,
    unsigned long long take,
    OIf<T>& outS
    )
{
    auto p = new constant<T>(pName.c_str(), initval, take);
    
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
template <class T, template <class> class OIf>
inline source<T>* make_source(std::string pName,
    typename source<T>::functype _func,
    T initval,
    unsigned long long take,
    OIf<T>& outS
    )
{
    auto p = new source<T>(pName.c_str(), _func, initval, take);
    
    (*p).oport1(outS);
    
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

//! Helper function to construct a zip process
/*! This function is used to construct a zip process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class T1, template <class> class I1If,
           class T2, template <class> class I2If,
           template <class> class OIf>
inline zip<T1,T2>* make_zip(std::string pName,
    unsigned int i1toks,
    unsigned int i2toks,
    OIf<std::tuple<std::vector<T1>,std::vector<T2>>>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S
    )
{
    auto p = new zip<T1,T2>(pName.c_str(), i1toks, i2toks);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct an unzip process
/*! This function is used to construct an unzip process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <template <class> class IIf,
           class T1, template <class> class O1If,
           class T2, template <class> class O2If>
inline unzip<T1,T2>* make_unzip(std::string pName,
    IIf<std::tuple<std::vector<T1>,std::vector<T2>>>& inpS,
    unsigned int o1toks,
    unsigned int o2toks,
    O1If<T1>& out1S,
    O2If<T2>& out2S
    )
{
    auto p = new unzip<T1,T2>(pName.c_str(), o1toks, o2toks);
    
    (*p).iport1(inpS);
    (*p).oport1(out1S);
    (*p).oport2(out2S);
    
    return p;
}

//! Helper function to construct an unzipN process
/*! This function is used to construct an unzipN process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
//~ template <template <class> class IIf,
           //~ typename T1, template <typename> typename OIf,
           //~ typename... Ts>
//~ inline unzipN<T...>* make_unzipN(std::string pName,
    //~ IIf<std::tuple<abst_ext<Ts>...>>& inpS,
    //~ OIf<T1>& outS,
    //~ OIfs<Ts>&... outsS
    //~ )
//~ {
    //~ make_unzipN(pName, inpS, outsS...);
    //~ std:get???????????(*p).iport1(inpS);
    //~ 
    //~ return p;
//~ }
    //~ auto p = new unzip<T1,T2>(pName.c_str());
    //~ 
    //~ (*p).iport1(inpS);
    //~ (*p).oport1(out1S);
    //~ (*p).oport2(out2S);
    //~ 
    //~ return p;
//~ }

//! Helper function to construct a fanout process
/*! This function is used to construct a fanout process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename T, template <class> class IIf,
                        template <class> class OIf>
inline fanout<T>* make_fanout(std::string pName,
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new fanout<T>(pName.c_str());
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}


}
}

#endif
