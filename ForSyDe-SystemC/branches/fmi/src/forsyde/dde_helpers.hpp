/**********************************************************************           
    * dde_helpers.hpp -- Helper primitives in the DDE MoC             *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing helper primitives for modeling in the DDE MoC*
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DDE_HELPERS_HPP
#define DDE_HELPERS_HPP

/*! \file dde_helpers.hpp
 * \brief Implements helper primitives for modeling in the DDE MoC
 * 
 *  This file includes helper functions which facilliate construction of
 * processes in the DDE MoC
 */

#include <functional>
#include <tuple>

#include "tt_event.hpp"
#include "dde_process_constructors.hpp"

namespace ForSyDe
{

namespace DDE
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
    abst_ext<T> initval,
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
    const sc_time& delay_time,
    OIf<OT>& outS,
    IIf<IT>& inpS
    )
{
    auto p = new mealy<IT,ST,OT>(pName.c_str(), _ns_func, _od_func, init_st, delay_time);
    
    (*p).iport1(inpS);
    (*p).oport1(outS);
    
    return p;
}

/*! This function is used to construct a mealy2 process (SystemC module)
 * with two inputs and connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <typename IT1, typename IT2, typename ST, typename OT,
           template <class> class IIf1, template <class> class IIf2,
           template <class> class OIf>
inline mealy2<IT1,IT2,ST,OT>* make_mealy2(const std::string& pName,
    const typename mealy2<IT1,IT2,ST,OT>::ns_functype& _ns_func,
    const typename mealy2<IT1,IT2,ST,OT>::od_functype& _od_func,
    const ST& init_st,
    const sc_time& delay_time,
    OIf<OT>& outS,
    IIf1<IT1>& inpS1,
    IIf2<IT2>& inpS2
    )
{
    auto p = new mealy2<IT1,IT2,ST,OT>(pName.c_str(), _ns_func, _od_func, init_st, delay_time);
    
    (*p).iport1(inpS1);
    (*p).iport2(inpS2);
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
    ttn_event<T> initval,
    unsigned long long take,
    OIf<T>& outS
    )
{
    auto p = new source<T>(pName.c_str(), _func, initval, take);
    
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a vsource process
/*! This function is used to construct a source (SystemC module) and
 * connect its output signal.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the output FIFOs.
 */
template <class T, template <class> class OIf>
inline vsource<T>* make_vsource(std::string pName,  ///< the module name
    const std::vector<T>& values,                   ///< event values
    const std::vector<sc_time>& offsets,            ///< event offsets
    OIf<T>& outS
    )
{
    auto p = new vsource<T>(pName.c_str(), values, offsets);
    
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
    OIf< std::tuple<abst_ext<T1>,abst_ext<T2>> >& outS,
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
 * connect its output signal.
 * The user binds the inputs manually.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class T1, std::size_t N,
            template <class> class OIf>
inline zipX<T1,N>* make_zipX(std::string pName,
    OIf< std::array<abst_ext<T1>,N> >& outS
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
inline unzip<T1,T2>* make_unzip(std::string pName,
    IIf< std::tuple<abst_ext<T1>,abst_ext<T2>> >& inpS,
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
