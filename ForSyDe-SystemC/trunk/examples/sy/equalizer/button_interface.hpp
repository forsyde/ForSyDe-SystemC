/**********************************************************************
    * button_interface.hpp -- The user interface in the equalizer system*
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *          the Haskell version from Ingo Sander (ingo@kth.se)     *
    *                                                                 *
    * Purpose: Demonstrating how co-simulation with legacy codes is   *
    *          performed.                                             *
    *                                                                 *
    * Usage:   Equalizer example                                      *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/
#ifndef BUTTON_INTERFACE_HPP
#define BUTTON_INTERFACE_HPP

#include <forsyde.hpp>
#include <globals.hpp>

#include <tuple>

using namespace ForSyDe;
using namespace ForSyDe::SY;

void button_interface_func(abst_ext<Button>& out1,
                            const abst_ext<Sensor>& bassUp,
                            const abst_ext<Sensor>& bassDn,
                            const abst_ext<Sensor>& trebleUp,
                            const abst_ext<Sensor>& trebleDn)
{
    if (is_present(bassUp)) out1 = abst_ext<Button>(BassUp);
    else if (is_present(bassDn)) out1 = abst_ext<Button>(BassDn);
    else if (is_present(trebleUp)) out1 = abst_ext<Button>(TrebleUp);
    else if (is_present(trebleDn)) out1 = abst_ext<Button>(TrebleDn);
    else out1 = abst_ext<Button>();
}

#endif
