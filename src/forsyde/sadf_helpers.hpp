/************************************************************************           
    * sadf_helpers.hpp -- Helper primitives in the SADF MoC             *
    *                                                                   *
    * Author:  Mohammad Vazirpanah (mohammad.vazirpanah@yahoo.com)      *
    *                                                                   *
    * Purpose: Providing helper primitives for modeling in the SADF MoC *
    *                                                                   *
    * Usage:   This file is included automatically                      *
    *                                                                   *
    * License:                                                          *
    *********************************************************************/

#ifndef SADF_HELPERS_HPP
#define SADF_HELPERS_HPP

/*! \file sadf_helpers.hpp
 * \brief Implements helper primitives for modeling in the SADF MoC
 * 
 *  This file includes helper functions which facilliate construction of
 * processes in the SADF MoC
 */

#include <functional>
#include <tuple>

#include "sadf_process_constructors.hpp"

namespace ForSyDe
{

namespace SADF
{

using namespace sc_core;

//! Helper function to construct a kernel process
/*! This function is used to construct a kernel (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <typename T0, typename TC, typename T1,
           template <class> class CIf,
           template <class> class I1If,
           template <class> class OIf>
inline kernel<T0,TC,T1>* make_kernel(const std::string& pName,
    const typename kernel<T0,TC,T1>::functype& _func,
    const typename kernel<T0,TC,T1>::scenario_table_type& _scenario_table,
    OIf<T0>& outS1,
    CIf<TC>& cS1,
    I1If<T1>& inpS1
    )
    
{
    auto p = new kernel<T0,TC,T1>(pName.c_str(), _func, _scenario_table);
    
    (*p).cport1(cS1);
    (*p).iport1(inpS1);
    (*p).oport1(outS1);
    
    return p;
}

//! Helper function to construct a detector12 process
/*! This function is used to construct a detector12 (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <typename T0, typename T1, typename TS,
           template <class> class OIf,
           template <class> class I1If>
inline detector<T0,T1,TS>* make_detector(const std::string& pName,
    const typename detector<T0,T1,TS>::cds_functype& _cds_func,
    const typename detector<T0,T1,TS>::kss_functype& _kss_func,
    const typename detector<T0,T1,TS>::scenario_table_type& scenario_table,
    const TS& init_sc,
    const size_t& i1toks,
    OIf<T0>& outS,
    I1If<T1>& inpS1
    )
{
    auto p = new detector<T0,T1,TS>(pName.c_str(), _cds_func, _kss_func, scenario_table, init_sc, i1toks);
    
    (*p).iport1(inpS1);
    (*p).oport1(outS);
    
    return p;
}

}
}

#endif
