/**********************************************************************           
    * sy_helpers.hpp -- Helper primitives in the SY MoC               *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing helper primitives for modeling in the SY MoC *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SY_HELPERS_HPP
#define SY_HELPERS_HPP

/*! \file sy_helpers.hpp
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

//! Helper function to construct a comb process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class I1If>
inline comb<T0,T1>* make_comb(const std::string& pName,
    const typename comb<T0,T1>::functype& _func,
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
inline comb2<T0,T1,T2>* make_comb2(const std::string& pName,
    const typename comb2<T0,T1,T2>::functype& _func,
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
template <class T0, template <class> class OIf,
          class T1, template <class> class I1If,
          class T2, template <class> class I2If,
          class T3, template <class> class I3If>
inline comb3<T0,T1,T2,T3>* make_comb3(const std::string& pName,
    const typename comb3<T0,T1,T2,T3>::functype& _func,
    OIf<T0>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S,
    I3If<T3>& inp3S
    )
{
    auto p = new comb3<T0,T1,T2,T3>(pName.c_str(), _func);
    
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
inline comb4<T0,T1,T2,T3,T4>* make_comb4(const std::string& pName,
    const typename comb4<T0,T1,T2,T3,T4>::functype& _func,
    OIf<T0>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S,
    I3If<T3>& inp3S,
    I4If<T4>& inp4S
    )
{
    auto p = new comb4<T0,T1,T2,T3,T4>(pName.c_str(), _func);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).iport3(inp3S);
    (*p).iport4(inp4S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a combX process
/*! This function is used to construct a process (SystemC module) and
 * connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class IIf,
          std::size_t N>
inline combX<T0,T1,N>* make_combX(const std::string& pName,
    const typename combX<T0,T1,N>::functype& _func,
    OIf<T0>& outS,
    std::array<IIf<T1>,N>& inpS
    )
{
    auto p = new combX<T0,T1,N>(pName.c_str(), _func);

    for (int i=0;i<N;i++)
    	(*p).iport[i](inpS[i]);
    (*p).oport1(outS);

    return p;
}

// //! Helper function to construct a combN process
// /*! This function is used to construct a process (SystemC module) and
//  * connect its input and output signals.
//  * It provides a more functional style definition of a ForSyDe process.
//  * It also removes bilerplate code by using type-inference feature of
//  * C++ and automatic binding to the input and output FIFOs.
//  */
// template <class T0, template <class> class OIf,
//           class... Ts, template <class> class... IsIf>
// inline combN<T0,Ts...>* make_combN(const std::string& pName,
//     const typename combN<T0,Ts...>::functype& _func,
//     OIf<T0>& outS,
//     std::tuple<IsIf<Ts>...>& inpsS)
// {
//     auto p = new combN<T0,Ts...>(pName.c_str(), _func);
    
//     for(int i=0;i<sizeof...(Ts);i++)
//     	std::get<i>((*p).iport)(std::get<i>(inpsS));
//     (*p).oport1(outS);
    
//     return p;
// }

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
    const abst_ext<T>& initval,
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
    const abst_ext<T>& initval,
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
    const typename moore<IT,ST,OT>::ns_functype& _ns_func,
    const typename moore<IT,ST,OT>::od_functype& _od_func,
    const ST& init_st,
    OIf<OT>& outS,
    IIf<IT>& inpS
    )
{
    auto p = new moore<IT,ST,OT>(pName.c_str(), _ns_func, _od_func, init_st);
    
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
    const typename mealy<IT,ST,OT>::ns_functype& _ns_func,
    const typename mealy<IT,ST,OT>::od_functype& _od_func,
    const ST& init_st,
    OIf<OT>& outS,
    IIf<IT>& inpS
    )
{
    auto p = new mealy<IT,ST,OT>(pName.c_str(), _ns_func, _od_func, init_st);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a fill process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename T, template <class> class IIf,
                        template <class> class OIf>
inline fill<T>* make_fill(const std::string& pName,
    const T& def_val,
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new fill<T>(pName.c_str(), def_val);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a hold process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename T, template <class> class IIf,
                        template <class> class OIf>
inline hold<T>* make_hold(const std::string& pName,
    const T& def_val,
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new hold<T>(pName.c_str(), def_val);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a group process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename T, template <class> class IIf,
                        template <class> class OIf>
inline group<T>* make_group(const std::string& pName,
    const unsigned long& samples,
    OIf<std::vector<abst_ext<T>>>& outS,
    IIf<T>& inpS
    )
{
    auto p = new group<T>(pName.c_str(), samples);
    
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
    const abst_ext<T>& initval,
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
    const abst_ext<T>& initval,
    const unsigned long long& take,
    OIf<T>& outS
    )
{
    auto p = new source<T>(pName.c_str(), _func, initval, take);
    
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a file_source process
/*! This function is used to construct a file_source (SystemC module) and
 * connect its output signal.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the output FIFOs.
 */
template <class T, template <class> class OIf>
inline file_source<T>* make_file_source(std::string pName,
    typename file_source<T>::functype _func,
    std::string file_name,
    OIf<T>& outS
    )
{
    auto p = new file_source<T>(pName.c_str(), _func, file_name);
    
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
    const std::vector<abst_ext<T>>& in_vec,
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

//! Helper function to construct a file_sink process
/*! This function is used to construct a file_sink (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class T, template <class> class IIf>
inline file_sink<T>* make_file_sink(std::string pName,
    typename file_sink<T>::functype _func,
    std::string file_name,
    IIf<T>& inS
    )
{
    auto p = new file_sink<T>(pName.c_str(), _func, file_name);
    
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
inline zip<T1,T2>* make_zip(const std::string& pName,
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

//! Helper function to construct a zipX process
/*! This function is used to construct a zip process (SystemC module) and
 * connect its output signals.
 * The user binds the inputs manually.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class T1, std::size_t N,
           template <class> class OIf>
inline zipX<T1,N>* make_zipX(const std::string& pName,
    OIf<std::array<abst_ext<T1>,N>>& outS
    )
{
    auto p = new zipX<T1,N>(pName.c_str());
    
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

//! Helper function to construct an unzipX process
/*! This function is used to construct an unzipX process (SystemC module) and
 * connect its input signal.
 * The user binds the outputs manually.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <template <class> class IIf,
           class T1, std::size_t N>
inline unzipX<T1,N>* make_unzipX(const std::string& pName,
    IIf<std::array<abst_ext<T1>,N>>& inpS
    )
{
    auto p = new unzipX<T1,N>(pName.c_str());
    
    (*p).iport1(inpS);
    
    return p;
}

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


}
}

#endif
