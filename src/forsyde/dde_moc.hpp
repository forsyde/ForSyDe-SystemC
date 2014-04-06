/**********************************************************************           
    * dde_moc.hpp -- The distributed discrete-event (DDE) model of    *
    *                   computation                                   *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing promitive element required for modeling      *
    *          dicrete-event systems in ForSyDe-SystemC               *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DE_MOC_HPP
#define DE_MOC_HPP

/*! \file de_moc.hpp
 * \brief Implements the Distributed Discrete-Event (DDE) Model of Computation
 * 
 *  This file includes the basic process constructors and other
 * facilities used for modeling in the distributed discrete-event model
 * of computation.
 */

#include <functional>
#include <tuple>

#include "tt_event.hpp"
#include "dde_process.hpp"
#include "dde_process_constructors.hpp"
#include "dde_helpers.hpp"

namespace ForSyDe
{

//! The namespace for DDE MoC
/*! This namespace includes constructs used for building models in the
 * DDE MoC.
 */
namespace DDE
{


}
}

#endif
