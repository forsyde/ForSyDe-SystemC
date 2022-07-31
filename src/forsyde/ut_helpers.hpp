/**********************************************************************           
    * ut_helpers.hpp -- Helper primitives in the UT MoC               *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing helper primitives for modeling in the UT MoC *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef UT_HELPERS_HPP
#define UT_HELPERS_HPP

/*! \file ut_helpers.hpp
 * \brief Implements helper primitives for modeling in the UT MoC
 * 
 *  This file includes helper functions which facilliate construction of
 * processes in the untimed MoC
 */

#include <functional>
#include <tuple>

#include "ut_process_constructors.hpp"

namespace ForSyDe
{

namespace UT
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
inline comb<T0,T1>* make_comb(const std::string& pName,    ///< process name
    const typename comb<T0,T1>::functype& _func,          ///< function to be passed
    const unsigned int& i1toks,                            ///< consumption rate for the first input
    OIf<T0>& outS,                                   ///< the first output signal
    I1If<T1>& inp1S                                  ///< the first input signal
    )
{
    auto p = new comb<T0,T1>(pName.c_str(), _func, i1toks);
    
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
inline comb2<T0,T1,T2>* make_comb2(const std::string pName,///< process name
    const typename comb2<T0,T1,T2>::functype& _func,      ///< function to be passed
    const unsigned int& i1toks,                            ///< consumption rate for the first input
    const unsigned int& i2toks,                            ///< consumption rate for the second input
    OIf<T0>& outS,                                   ///< the first output signal
    I1If<T1>& inp1S,                                 ///< the first input signal
    I2If<T2>& inp2S                                  ///< the second input signal
    )
{
    auto p = new comb2<T0,T1,T2>(pName.c_str(), _func, 
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
inline comb3<T0,T1,T2,T3>* make_comb3(const std::string& pName,///< process name
    const typename comb3<T0,T1,T2,T3>::functype& _func,   ///< function to be passed
    const unsigned int& i1toks,                            ///< consumption rate for the first input
    const unsigned int& i2toks,                            ///< consumption rate for the second input
    const unsigned int& i3toks,                            ///< consumption rate for the third input
    OIf<T0>& outS,                                   ///< the first output signal
    I1If<T1>& inp1S,                                 ///< the first input signal
    I2If<T2>& inp2S,                                 ///< the second input signal
    I3If<T3>& inp3S                                  ///< the third input signal
    )
{
    auto p = new comb3<T0,T1,T2,T3>(pName.c_str(), _func,
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
inline comb4<T0,T1,T2,T3,T4>* make_comb4(const std::string& pName,///< process name
    const typename comb4<T0,T1,T2,T3,T4>::functype& _func,///< function to be passed
    const unsigned int& i1toks,                            ///< consumption rate for the first input
    const unsigned int& i2toks,                            ///< consumption rate for the second input
    const unsigned int& i3toks,                            ///< consumption rate for the third input
    const unsigned int& i4toks,                            ///< consumption rate for the fourth input
    OIf<T0>& outS,                                   ///< the first output signal
    I1If<T1>& inp1S,                                 ///< the first input signal
    I2If<T2>& inp2S,                                 ///< the second input signal
    I3If<T3>& inp3S,                                 ///< the third input signal
    I4If<T4>& inp4S                                  ///< the fourth input signal
    )
{
    auto p = new comb4<T0,T1,T2,T3,T4>(pName.c_str(), _func,
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
inline delay<T>* make_delay(const std::string& pName,
    const T& initval,
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
inline delayn<T>* make_delayn(const std::string& pName,
    const T& initval,
    const unsigned int& n,
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new delayn<T>(pName.c_str(), initval, n);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a scan-based process
/*! This function is used to construct a scan-based process (SystemC 
 * module) and connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename IT, typename ST,
           template <class> class IIf,
           template <class> class OIf>
inline scan<IT,ST>* make_scan(const std::string& pName,
    const typename scan<IT,ST>::gamma_functype& _gamma_func,
    const typename scan<IT,ST>::ns_functype& _ns_func,
    const ST& init_st,
    OIf<ST>& outS,
    IIf<IT>& inpS
    )
{
    auto p = new scan<IT,ST>(pName.c_str(), _gamma_func, _ns_func, init_st);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a scand-based process
/*! This function is used to construct a scand-based process (SystemC 
 * module) and connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename IT, typename ST,
           template <class> class IIf,
           template <class> class OIf>
inline scand<IT,ST>* make_scand(const std::string& pName,
    const typename scan<IT,ST>::gamma_functype& _gamma_func,
    const typename scan<IT,ST>::ns_functype& _ns_func,
    const ST& init_st,
    OIf<ST>& outS,
    IIf<IT>& inpS
    )
{
    auto p = new scand<IT,ST>(pName.c_str(), _gamma_func, _ns_func, init_st);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a moore process
/*! This function is used to construct a moore process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename IT, typename ST, typename OT,
           template <class> class IIf,
           template <class> class OIf>
inline moore<IT,ST,OT>* make_moore(const std::string& pName,
    const typename moore<IT,ST,OT>::gamma_functype& _gamma_func,
    const typename moore<IT,ST,OT>::ns_functype& _ns_func,
    const typename moore<IT,ST,OT>::od_functype& _od_func,
    const ST& init_st,
    OIf<OT>& outS,
    IIf<IT>& inpS
    )
{
    auto p = new moore<IT,ST,OT>(pName.c_str(), _gamma_func, _ns_func, _od_func, init_st);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a mealy process
/*! This function is used to construct a mealy process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename IT, typename ST, typename OT,
           template <class> class IIf,
           template <class> class OIf>
inline mealy<IT,ST,OT>* make_mealy(const std::string& pName,
    const typename moore<IT,ST,OT>::gamma_functype& _gamma_func,
    const typename mealy<IT,ST,OT>::ns_functype& _ns_func,
    const typename mealy<IT,ST,OT>::od_functype& _od_func,
    const ST& init_st,
    OIf<OT>& outS,
    IIf<IT>& inpS
    )
{
    auto p = new mealy<IT,ST,OT>(pName.c_str(), _gamma_func, _ns_func, _od_func, init_st);
    
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
inline constant<T>* make_constant(const std::string& pName,
    const T& initval,
    const unsigned long long& take,
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
inline source<T>* make_source(const std::string& pName,
    const typename source<T>::functype& _func,
    const T& initval,
    const unsigned long long& take,
    OIf<T>& outS
    )
{
    auto p = new source<T>(pName.c_str(), _func, initval, take);
    
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a vector source process
/*! This function is used to construct a vector source (SystemC module) and
 * connect its output signal.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the output FIFOs.
 */
template <class T, template <class> class OIf>
inline vsource<T>* make_vsource(const std::string& pName,
    const std::vector<T>& in_vec,
    OIf<T>& outS
    )
{
    auto p = new vsource<T>(pName.c_str(), in_vec);
    
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
inline sink<T>* make_sink(const std::string& pName,
    const typename sink<T>::functype& _func,
    IIf<T>& inS
    )
{
    auto p = new sink<T>(pName.c_str(), _func);
    
    (*p).iport1(inS);
    
    return p;
}

//! Helper function to construct a zips process
/*! This function is used to construct a zips process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class T1, template <class> class I1If,
           class T2, template <class> class I2If,
           template <class> class OIf>
inline zips<T1,T2>* make_zips(const std::string& pName,
    const unsigned int& i1toks,
    const unsigned int& i2toks,
    OIf<std::tuple<std::vector<T1>,std::vector<T2>>>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S
    )
{
    auto p = new zips<T1,T2>(pName.c_str(), i1toks, i2toks);
    
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
inline unzip<T1,T2>* make_unzip(const std::string& pName,
    IIf<std::tuple<std::vector<T1>,std::vector<T2>>>& inpS,
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
//~ inline unzipN<T...>* make_unzipN(const std::string& pName,
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
inline fanout<T>* make_fanout(const std::string& pName,
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new fanout<T>(pName.c_str());
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a zipU process
/*! This function is used to construct a zipU process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */

template <class T1, template <class> class I1If,
           class T2, template <class> class I2If,
           class TCS, template <class> class I3If,
           template <class> class OIf>
inline zipU<T1,T2,TCS>* make_zipU(const std::string& pName,
    const typename zipU<T1,T2,TCS>::gamma_functype& _gamma_func_a,
    const typename zipU<T1,T2,TCS>::gamma_functype& _gamma_func_b,
    OIf<std::tuple<std::vector<T1>,std::vector<T2>>>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S,
    I3If<TCS>& inp3S
    )
{
    auto p = new zipU<T1,T2,TCS>(pName.c_str(), _gamma_func_a, _gamma_func_b);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).controlport(inp3S);
    (*p).oport1(outS);
    
    return p;
}

}
}

#endif
