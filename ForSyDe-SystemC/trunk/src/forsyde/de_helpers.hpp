/**********************************************************************           
    * de_helpers.hpp -- Helper primitives in the DE MoC               *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing helper primitives for modeling in the DE MoC *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DE_HELPERS_HPP
#define DE_HELPERS_HPP

/*! \file de_helpers.hpp
 * \brief Implements helper primitives for modeling in the DE MoC
 * 
 *  This file includes helper functions which facilliate construction of
 * processes in the DE MoC
 */

#include <functional>
#include <tuple>

#include "tt_event.hpp"
#include "de_process_constructors.hpp"

namespace ForSyDe
{

namespace DE
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
inline comb<T0,T1>* make_comb(std::string pName,
    typename comb<T0,T1>::functype _func,
    OIf<T0>& outS,
    I1If<T1>& inp1S
    )
{
    auto p = new comb<T0,T1>(pName.c_str(), _func);
    
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
//~ template <class T0, template <class> class OIf,
          //~ class T1, template <class> class I1If,
          //~ class T2, template <class> class I2If,
          //~ class T3, template <class> class I3If>
//~ inline comb3<T0,T1,T2,T3>* make_comb3(std::string pName,
    //~ typename comb3<T0,T1,T2,T3>::functype _func,
    //~ OIf<T0>& outS,
    //~ I1If<T1>& inp1S,
    //~ I2If<T2>& inp2S,
    //~ I3If<T3>& inp3S
    //~ )
//~ {
    //~ auto p = new comb3<T0,T1,T2,T3>(pName.c_str(), _func);
    //~ 
    //~ (*p).iport1(inp1S);
    //~ (*p).iport2(inp2S);
    //~ (*p).iport3(inp3S);
    //~ (*p).oport1(outS);
    //~ 
    //~ return p;
//~ }
//~ 
//~ //! Helper function to construct a comb4 process
//~ /*! This function is used to construct a process (SystemC module) and
 //~ * connect its output and output signals.
 //~ * It provides a more functional style definition of a ForSyDe process.
 //~ * It also removes bilerplate code by using type-inference feature of
 //~ * C++ and automatic binding to the input and output FIFOs.
 //~ */
//~ template <class T0, template <class> class OIf,
          //~ class T1, template <class> class I1If,
          //~ class T2, template <class> class I2If,
          //~ class T3, template <class> class I3If,
          //~ class T4, template <class> class I4If>
//~ inline comb4<T0,T1,T2,T3,T4>* make_comb4(std::string pName,
    //~ typename comb4<T0,T1,T2,T3,T4>::functype _func,
    //~ OIf<T0>& outS,
    //~ I1If<T1>& inp1S,
    //~ I2If<T2>& inp2S,
    //~ I3If<T3>& inp3S,
    //~ I4If<T4>& inp4S
    //~ )
//~ {
    //~ auto p = new comb4<T0,T1,T2,T3,T4>(pName.c_str(), _func);
    //~ 
    //~ (*p).iport1(inp1S);
    //~ (*p).iport2(inp2S);
    //~ (*p).iport3(inp3S);
    //~ (*p).iport4(inp4S);
    //~ (*p).oport1(outS);
    //~ 
    //~ return p;
//~ }

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
    sc_time delay_time,
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new delay<T>(pName.c_str(), initval, delay_time);
    
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
    sc_time interval,
    OIf<T>& outS
    )
{
    auto p = new constant<T>(pName.c_str(), initval, interval);
    
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a single source process
/*! This function is used to construct a single event process (SystemC
 * module) and connect its output signal.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the output FIFOs.
 */
template <class T, template <class> class OIf>
inline single<T>* make_single(std::string pName,
    T val,
    sc_time instant,
    OIf<T>& outS
    )
{
    auto p = new single<T>(pName.c_str(), val, instant);
    
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
    abst_ext<T> initval,
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
    OIf<std::tuple<abst_ext<T1>,abst_ext<T2>>>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S
    )
{
    auto p = new zip<T1,T2>(pName.c_str());
    
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
    IIf<std::tuple<abst_ext<T1>,abst_ext<T2>>>& inpS,
    O1If<T1>& out1S,
    O2If<T2>& out2S
    )
{
    auto p = new unzip<T1,T2>(pName.c_str());
    
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
