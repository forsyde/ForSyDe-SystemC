/**********************************************************************           
    * sy_helpers_strict.hpp -- strict helper primitives in the SY MoC *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
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
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class IIf,
          unsigned int N>
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

}
}

#endif
