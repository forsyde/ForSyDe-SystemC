/**********************************************************************           
    * sy_helpers_strict.hpp -- strict helper primitives in the SY MoC *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing helper primitives for modeling in the SY MoC *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SY_HELPERS_STRICT_HPP
#define SY_HELPERS_STRICT_HPP

/*! \file sy_helpers_strict.hpp
 * \brief Implements strict helper primitives for modeling in the SY MoC
 * 
 *  This file includes helper functions which facilliate construction of
 * strict processes in the SY MoC
 */

#include <functional>
#include <tuple>

#include "abst_ext.hpp"
#include "sy_process_constructors_strict.hpp"

namespace ForSyDe
{

namespace SY
{

using namespace sc_core;

//! Helper function to construct a strict comb process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class I1If>
inline scomb<T0,T1>* make_scomb(const std::string& pName,
    const typename scomb<T0,T1>::functype& _func,
    OIf<T0>& outS,
    I1If<T1>& inp1S
    )
{
    auto p = new scomb<T0,T1>(pName.c_str(), _func);
    
    (*p).iport1(inp1S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a strict comb2 process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class I1If,
          class T2, template <class> class I2If>
inline scomb2<T0,T1,T2>* make_scomb2(const std::string& pName,
    const typename scomb2<T0,T1,T2>::functype& _func,
    OIf<T0>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S
    )
{
    auto p = new scomb2<T0,T1,T2>(pName.c_str(), _func);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a strict comb3 process
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
inline scomb3<T0,T1,T2,T3>* make_scomb3(const std::string& pName,
    const typename scomb3<T0,T1,T2,T3>::functype& _func,
    OIf<T0>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S,
    I3If<T3>& inp3S
    )
{
    auto p = new scomb3<T0,T1,T2,T3>(pName.c_str(), _func);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).iport3(inp3S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a strict comb4 process
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
inline scomb4<T0,T1,T2,T3,T4>* make_scomb4(const std::string& pName,
    const typename scomb4<T0,T1,T2,T3,T4>::functype& _func,
    OIf<T0>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S,
    I3If<T3>& inp3S,
    I4If<T4>& inp4S
    )
{
    auto p = new scomb4<T0,T1,T2,T3,T4>(pName.c_str(), _func);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).iport3(inp3S);
    (*p).iport4(inp4S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a strict combX process
/*! This function is used to construct a process (SystemC module) and
 * connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class IIf,
          std::size_t N>
inline scombX<T0,T1,N>* make_scombX(const std::string& pName,
    const typename scombX<T0,T1,N>::functype& _func,
    OIf<T0>& outS,
    std::array<IIf<T1>,N>& inpS
    )
{
    auto p = new scombX<T0,T1,N>(pName.c_str(), _func);

    for (int i=0;i<N;i++)
    	(*p).iport[i](inpS[i]);
    (*p).oport1(outS);

    return p;
}

// //! Helper function to construct a strict combN process
// /*! This function is used to construct a process (SystemC module) and
//  * connect its input and output signals.
//  * It provides a more functional style definition of a ForSyDe process.
//  * It also removes bilerplate code by using type-inference feature of
//  * C++ and automatic binding to the input and output FIFOs.
//  */
// template <class T0, template <class> class OIf,
//           class... Ts, template <class> class... IsIf>
// inline scombN<T0,Ts...>* make_scombN(const std::string& pName,
//     const typename scombN<T0,Ts...>::functype& _func,
//     OIf<T0>& outS,
//     std::tuple<IsIf<Ts>...>& inpsS)
// {
//     auto p = new scombN<T0,Ts...>(pName.c_str(), _func);
    
//     for(int i=0;i<sizeof...(Ts);i++)
//     	std::get<i>((*p).iport)(std::get<i>(inpsS));
//     (*p).oport1(outS);
    
//     return p;
// }

//! Helper function to construct a strict data parallel comb process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class IIf,
          std::size_t N>
inline sdpmap<T0,T1,N>* make_sdpmap(const std::string& pName,
    const typename sdpmap<T0,T1,N>::functype& _func,
    OIf<std::array<T0,N>>& outS,
    IIf<std::array<T1,N>>& inpS
    )
{
    auto p = new sdpmap<T0,T1,N>(pName.c_str(), _func);

    (*p).iport1(inpS);
    (*p).oport1(outS);

    return p;
}

//! Helper function to construct a strict reduce process
/*! This function is used to construct a sreduce process (SystemC module)
 * and connect connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          template <class> class IIf,
          std::size_t N>
inline sdpreduce<T0,N>* make_sdpreduce(const std::string& pName,
    const typename sdpreduce<T0,N>::functype& _func,
    OIf<T0>& outS,
    IIf<std::array<T0,N>>& inpS
    )
{
    auto p = new sdpreduce<T0,N>(pName.c_str(), _func);

    (*p).iport1(inpS);
    (*p).oport1(outS);

    return p;
}

//! Helper function to construct a strict data parallel scan process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class IIf,
          std::size_t N>
inline sdpscan<T0,T1,N>* make_sdpscan(const std::string& pName,
    const typename sdpscan<T0,T1,N>::functype& _func,
    const T0& init_res,
    OIf<std::array<T0,N>>& outS,
    IIf<std::array<T1,N>>& inpS
    )
{
    auto p = new sdpscan<T0,T1,N>(pName.c_str(), _func, init_res);

    (*p).iport1(inpS);
    (*p).oport1(outS);

    return p;
}

//! Helper function to construct a strict delay process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename T, template <class> class IIf,
                        template <class> class OIf>
inline sdelay<T>* make_sdelay(const std::string& pName,
    const T& initval,
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new sdelay<T>(pName.c_str(), initval);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a strict delayn process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename T, template <class> class IIf,
                        template <class> class OIf>
inline sdelayn<T>* make_sdelayn(const std::string& pName,
    const T& initval,
    const unsigned int& n,
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new sdelayn<T>(pName.c_str(), initval, n);
    
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
inline smoore<IT,ST,OT>* make_smoore(const std::string& pName,
    const typename smoore<IT,ST,OT>::ns_functype& _ns_func,
    const typename smoore<IT,ST,OT>::od_functype& _od_func,
    const ST& init_st,
    OIf<OT>& outS,
    IIf<IT>& inpS
    )
{
    auto p = new smoore<IT,ST,OT>(pName.c_str(), _ns_func, _od_func, init_st);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a strict mealy process
/*! This function is used to construct a mealy process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename IT, typename ST, typename OT,
           template <class> class IIf,
           template <class> class OIf>
inline smealy<IT,ST,OT>* make_smealy(const std::string& pName,
    const typename smealy<IT,ST,OT>::ns_functype& _ns_func,
    const typename smealy<IT,ST,OT>::od_functype& _od_func,
    const ST& init_st,
    OIf<OT>& outS,
    IIf<IT>& inpS
    )
{
    auto p = new smealy<IT,ST,OT>(pName.c_str(), _ns_func, _od_func, init_st);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a strict group process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename T, template <class> class IIf,
                        template <class> class OIf>
inline sgroup<T>* make_sgroup(const std::string& pName,
    const unsigned long& samples,
    OIf<std::vector<T>>& outS,
    IIf<T>& inpS
    )
{
    auto p = new sgroup<T>(pName.c_str(), samples);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a strict constant source process
/*! This function is used to construct a constant (SystemC module) and
 * connect its output signal.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the output FIFOs.
 */
template <class T, template <class> class OIf>
inline sconstant<T>* make_sconstant(const std::string& pName,
    const T& initval,
    const unsigned long long& take,
    OIf<T>& outS
    )
{
    auto p = new sconstant<T>(pName.c_str(), initval, take);
    
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a strict source process
/*! This function is used to construct a source (SystemC module) and
 * connect its output signal.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the output FIFOs.
 */
template <class T, template <class> class OIf>
inline ssource<T>* make_ssource(const std::string& pName,
    const typename ssource<T>::functype& _func,
    const T& initval,
    const unsigned long long& take,
    OIf<T>& outS
    )
{
    auto p = new ssource<T>(pName.c_str(), _func, initval, take);
    
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a strict vector source process
/*! This function is used to construct a vector source (SystemC module) and
 * connect its output signal.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the output FIFOs.
 */
template <class T, template <class> class OIf>
inline svsource<T>* make_svsource(const std::string& pName,
    const std::vector<T>& in_vec,
    OIf<T>& outS
    )
{
    auto p = new svsource<T>(pName.c_str(), in_vec);
    
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a strict sink process
/*! This function is used to construct a sink (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class T, template <class> class IIf>
inline ssink<T>* make_ssink(const std::string& pName,
    const typename ssink<T>::functype& _func,
    IIf<T>& inS
    )
{
    auto p = new ssink<T>(pName.c_str(), _func);
    
    (*p).iport1(inS);
    
    return p;
}

//! Helper function to construct a szip process
/*! This function is used to construct a strict zip process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class T1, template <class> class I1If,
           class T2, template <class> class I2If,
           template <class> class OIf>
inline szip<T1,T2>* make_szip(const std::string& pName,
    OIf<std::tuple<T1,T2>>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S
    )
{
    auto p = new szip<T1,T2>(pName.c_str());
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a szipX process
/*! This function is used to construct a szip process (SystemC module) and
 * connect its output signal.
 * The user binds the inputs manually.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class T1, std::size_t N,
           template <class> class OIf>
inline szipX<T1,N>* make_szipX(const std::string& pName,
    OIf<std::array<T1,N>>& outS
    )
{
    auto p = new szipX<T1,N>(pName.c_str());
    
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a sunzip process
/*! This function is used to construct a strict unzip process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <template <class> class IIf,
           class T1, template <class> class O1If,
           class T2, template <class> class O2If>
inline sunzip<T1,T2>* make_sunzip(const std::string& pName,
    IIf<std::tuple<T1,T2>>& inpS,
    O1If<T1>& out1S,
    O2If<T2>& out2S
    )
{
    auto p = new sunzip<T1,T2>(pName.c_str());
    
    (*p).iport1(inpS);
    (*p).oport1(out1S);
    (*p).oport2(out2S);
    
    return p;
}

//! Helper function to construct a strict unzipX process
/*! This function is used to construct a strict unzipX process (SystemC 
 * module) and connect its input signal.
 * The user binds the outputs manually.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <template <class> class IIf,
           class T1, std::size_t N>
inline unzipX<T1,N>* make_unzipX(const std::string& pName,
    IIf<std::array<T1,N>>& inpS
    )
{
    auto p = new unzipX<T1,N>(pName.c_str());
    
    (*p).iport1(inpS);
    
    return p;
}

}
}

#endif
