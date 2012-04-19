/**********************************************************************           
    * forsyde.hpp -- the main header file used to import the SystemC  *
    *          map of the ForSyDe library                             *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
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
 * Originally, 4 different MoCs are suported in ForSyDe-SystemC:
 *
 *     - Synchronous MoC in ForSyDe::SY
 *     - Untimed MoC (only the Synchronous Dataflow variant) in ForSyDe::SDF
 *     - Discrete-Event MoC in ForSyDe::DE
 *     - Continuous-Time MoC in ForSyDe::CT
 */

/*! \file forsyde.h
 * \brief Exports the library definitions for the ForSyDe-SystemC
 * 
 *  The user only includes this header file. Definitions in the other
 * files of the library are re-exported from here.
 */

// include utility libraries
#include "forsyde/prettyprint.hpp"

// include the main SystemC library
#include <systemc>

// include the abstract semantics
#include "forsyde/abssemantics.hpp"

// include different MoCs
#include "forsyde/symoc.hpp"
//~ #include "forsyde/sylib.hpp"
#include "forsyde/sdfmoc.hpp"
#include "forsyde/democ.hpp"
#include "forsyde/ctmoc.hpp"
#include "forsyde/ctlib.hpp"

#include "forsyde/dis.hpp"

#include "forsyde/adaptivity.hpp"

#endif
