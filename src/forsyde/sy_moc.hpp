/**********************************************************************           
    * sy_moc.hpp -- The synchronous model of computation              *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing promitive element required for modeling      *
    *          synchronous systems in ForSyDe-SystemC                 *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SY_MOC_HPP
#define SY_MOC_HPP

/*! \file sy_moc.hpp
 * \brief Implements the synchronous Model of Computation
 * 
 *  This file includes the basic process constructors and other
 * facilities used for modeling in the synchronous model of computation.
 */

#include <functional>
#include <tuple>

#include "abst_ext.hpp"
#include "sy_process.hpp"
#include "sy_process_constructors.hpp"
#include "sy_helpers.hpp"
#include "sy_process_constructors_strict.hpp"
#include "sy_helpers_strict.hpp"

namespace ForSyDe
{

//! The namespace for synchronous MoC
/*! This namespace includes constructs used for building models in the
 * synchronous MoC.
 */
namespace SY
{


}
}

#endif
