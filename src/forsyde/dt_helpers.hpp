/**********************************************************************           
    * dt_helpers.hpp -- Helper primitives in the DT MoC               *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing helper primitives for modeling in the DT MoC *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DT_HELPERS_HPP
#define DT_HELPERS_HPP

/*! \file dt_helpers.hpp
 * \brief Implements helper primitives for modeling in the DT MoC
 * 
 *  This file includes helper functions which facilliate construction of
 * processes in the discrete-time MoC
 */

#include <functional>
#include <tuple>

#include "abst_ext.hpp"
#include "dt_process_constructors.hpp"

namespace ForSyDe
{

namespace DT
{

using namespace sc_core;

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
    OIf<T>& outS,
    IIf<T>& inpS
    )
{
    auto p = new delay<T>(pName.c_str(), initval);
    
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
inline mealy<IT,ST,OT>* make_mealy(std::string pName,
    typename mealy<IT,ST,OT>::gamma_functype gamma,
    typename mealy<IT,ST,OT>::ns_functype _ns_func,
    typename mealy<IT,ST,OT>::od_functype _od_func,
    ST init_st,
    OIf<OT>& outS,
    IIf<IT>& inpS
    )
{
    auto p = new mealy<IT,ST,OT>(pName.c_str(), gamma, _ns_func, _od_func, init_st);
    
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
    abst_ext<T> initval,
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
    abst_ext<T> initval,
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
inline vsource<T>* make_vsource(std::string pName,
    std::vector<std::tuple<size_t,T>> in_vec,
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
inline sink<T>* make_sink(std::string pName,
    typename sink<T>::functype _func,
    IIf<T>& inS
    )
{
    auto p = new sink<T>(pName.c_str(), _func);
    
    (*p).iport1(inS);
    
    return p;
}

//! Helper function to construct a zips process
/*! This function is used to construct a zips process (SystemC module) and
 * connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class T1, template <class> class I1If,
           class T2, template <class> class I2If,
           template <class> class OIf>
inline zips<T1,T2>* make_zips(std::string pName,
    const size_t& itoks, 
    OIf<std::tuple<std::vector<abst_ext<T1>>,std::vector<abst_ext<T2>>>>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S
    )
{
    auto p = new zips<T1,T2>(pName.c_str(), itoks);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a zip process
/*! This function is used to construct a zip process (SystemC module) and
 * connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class T1, template <class> class I1If,
           class T2, template <class> class I2If,
           class TC, template <class> class ICIf,
           template <class> class OIf>
inline zip<T1,T2,TC>* make_zip(std::string pName,
    typename zip<T1,T2,TC>::gamma_functype gamma, 
    OIf<std::tuple<std::vector<abst_ext<T1>>,std::vector<abst_ext<T2>>>>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S,
    ICIf<TC>& inp3S
    )
{
    auto p = new zip<T1,T2,TC>(pName.c_str(), gamma);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).iport3(inp3S);
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
