/**********************************************************************           
    * parallel_sim_helpers.hpp -- Helper primitives for parallel simulation*
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing helper primitives for parallel simulations   *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef PARALLEL_SIM_HPP
#define PARALLEL_SIM_HPP

/*! \file parallel_sim_helpers.hpp
 * \brief Implements helper primitives for parallel simulations
 * 
 *  This file includes helper functions which facilliate construction of
 * parallel sub-simulations.
 */

#include <mpi.h>

#include "parallel_sim.hpp"


namespace ForSyDe
{

namespace SY
{

using namespace sc_core;

//! Helper function to construct a sender process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class I0If>
inline sender<T0>* make_sender(const std::string& pName,
    int destination,          ///< MPI rank of the destination process
    int tag,                  ///< MPI tag of the message
    I0If<T0>& inp1S
    )
{
    auto p = new sender<T0>(pName.c_str(), destination, tag);
    
    (*p).iport1(inp1S);
    
    return p;
}

//! Helper function to construct a sender process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf>
inline receiver<T0>* make_receiver(const std::string& pName,
    int source,               ///< MPI rank of the source process
    int tag,                  ///< MPI tag of the message
    OIf<T0>& outS
    )
{
    auto p = new receiver<T0>(pName.c_str(), source, tag);
    
    (*p).oport1(outS);
    
    return p;
}


}
}

#endif
