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

// include different MoCs
#include "forsyde/symoc.hpp"
#include "forsyde/sylib.hpp"
#include "forsyde/sdfmoc.hpp"
#include "forsyde/democ.hpp"
#include "forsyde/ctmoc.hpp"
#include "forsyde/ctlib.hpp"

#include "forsyde/dis.hpp"

// include various back-ends
#include "forsyde/xml.hpp"

#endif
