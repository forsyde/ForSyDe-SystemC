/**********************************************************************           
    * de_moc.hpp -- The discrete-event (DE) model of computation      *
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
 * \brief Implements the Discrete-Event (DE) Model of Computation
 * 
 *  This file includes the basic process constructors and other
 * facilities used for modeling in the synchronous model of computation.
 */

#include <functional>
#include <tuple>

#include "tt_event.hpp"
#include "de_process.hpp"
#include "de_process_constructors.hpp"
#include "de_helpers.hpp"

namespace ForSyDe
{

//! The namespace for DE MoC
/*! This namespace includes constructs used for building models in the
 * DE MoC.
 */
namespace DE
{


}
}

#endif
