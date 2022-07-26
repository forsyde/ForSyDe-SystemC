/**********************************************************************           
    * forsyde.hpp -- the main header file used to import the SystemC  *
    *          map of the ForSyDe library                             *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Exporting library definitions of the ForSyDe-SystemC   *
    *                                                                 *
    * Usage:   The user only includes this header file to access the  *
    *          library definitions                                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef FORSYDE_HPP
#define FORSYDE_HPP

/*! \mainpage ForSyDe-SystemC API Documentation
 *
 * \section intro_sec Introduction
 *
 * This documentation is supplied as the API-level guideline for using the
 * ForSyDe-SystemC library.
 * It is generated automatically from the library source code and is updated
 * with the library itself.
 *
 * \subsection scope_purpose Scope and Purpose
 *
 * This document provides the designer with information about the constructs
 * provided by the library and how they can be used practically.
 * It does NOT describe the general modeling concepts and formalisms behind
 * ForSyDe.
 *
 * It is suggested to consult the ForSyDe webpage and Wiki page before starting
 * modeling with this library.
 * More information and tutorials are available there.
 *
 * \section using_doc Using the Documentation
 *
 * ForSyDe-SystemC library mainly includes constructs to build processes in
 * different Models of Computation (MoCs) and connect them using domain
 * interfaces.
 *
 * \subsection namespaces_mocs Namespaces and MoCs
 *
 * Everything provided by the ForSyDe-SystemC library is a member of the
 * ForSyDe namespace.
 * In addition, there is a separate sub-namespace dedicated to each MoC which
 * includes process constructors and other constructs related to that specific
 * MoC.
 * There are different MoCs suported in ForSyDe-SystemC:
 *
 *     - Synchronous MoC in ForSyDe::SY
 *     - Untimed MoC in ForSyDe::UT and its Synchronous Dataflow variant in ForSyDe::SDF
 *     - Two timed MoCs Distributed Discrete-Event in ForSyDe::DDE and Discrete-Time in ForSyDe::DT
 *     - Continuous-Time MoC in ForSyDe::CT
 */

/*! \file forsyde.hpp
 * \brief Exports the library definitions for the ForSyDe-SystemC
 * 
 *  The user only includes this header file. Definitions in the other
 * files of the library are re-exported from here.
 */

// include utility libraries
#include "forsyde/prettyprint.hpp"

// include the main SystemC library
#include <systemc>

// include type libraries
#include "forsyde/type_helpers.hpp"
#ifdef FORSYDE_INTROSPECTION
#include "forsyde/type_introspect.hpp"
#endif

// include the abstract semantics
#include "forsyde/abssemantics.hpp"

// include different MoCs
#include "forsyde/ut_moc.hpp"

#include "forsyde/sy_moc.hpp"
#include "forsyde/sy_lib.hpp"

#include "forsyde/sdf_moc.hpp"

#include "forsyde/dde_moc.hpp"

#include "forsyde/dt_moc.hpp"

#include "forsyde/ct_moc.hpp"
#include "forsyde/ct_lib.hpp"

// include MoC interfaces
#include "forsyde/mis.hpp"
#include "forsyde/mi_helpers.hpp"

#include "forsyde/adaptivity.hpp"

#ifdef FORSYDE_INTROSPECTION
#include "forsyde/xml.hpp"
#endif

#ifdef FORSYDE_PARALLEL_SIM
#include "forsyde/parallel_sim_helpers.hpp"
#endif

#ifdef FORSYDE_COSIMULATION_WRAPPERS
#include "forsyde/sy_wrappers.hpp"
#include "forsyde/ct_wrappers.hpp"
#endif


#endif
