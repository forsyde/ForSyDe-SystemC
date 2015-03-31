/**********************************************************************           
    * dt_moc.hpp -- The discrete-time model of computation            *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing promitive element required for modeling      *
    *          discrete-time systems in ForSyDe-SystemC               *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DT_MOC_HPP
#define DT_MOC_HPP

/*! \file dt_moc.hpp
 * \brief Implements the DT Model of Computation
 * 
 *  This file includes the basic process constructors and other
 * facilities used for modeling in the discrete-time model of computation.
 */

#include <functional>
#include <tuple>

#include "abst_ext.hpp"
#include "dt_process.hpp"
#include "dt_process_constructors.hpp"
#include "dt_helpers.hpp"

namespace ForSyDe
{

//! The namespace for synchronous MoC
/*! This namespace includes constructs used for building models in the
 * discrete-time MoC.
 */
namespace DT
{


}
}

#endif
