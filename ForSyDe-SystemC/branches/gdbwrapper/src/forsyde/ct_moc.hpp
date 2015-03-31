/**********************************************************************           
    * ct_moc.hpp -- The continuous-time model of computation          *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          Based on the HetMoC library by Jun Zhu (junz@kth.se)   *
    *                                                                 *
    * Purpose: Providing promitive element required for modeling      *
    *          continuous-time systems in ForSyDe-SystemC             *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef CT_MOC_HPP
#define CT_MOC_HPP

/*! \file ct_moc.hpp
 * \brief Implements the continuous-time Model of Computation
 * 
 *  This file includes the basic process constructors and other
 * facilities used for modeling in the continuous-time model of
 * computation.
 */

#include <functional>
#include <tuple>

#include "sub_signal.hpp"
#include "ct_process.hpp"
#include "ct_process_constructors.hpp"
#include "ct_helpers.hpp"

namespace ForSyDe
{

//! The namespace for CT MoC
/*! This namespace includes constructs used for building models in the
 * continuous-time MoC.
 */
namespace CT
{

}
}

#endif
