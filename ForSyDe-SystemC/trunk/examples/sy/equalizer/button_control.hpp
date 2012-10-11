/**********************************************************************
    * button_control.hpp -- The user interface in the equalizer system*
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
#ifndef BUTTON_CONTROL_HPP
#define BUTTON_CONTROL_HPP

#include <forsyde.hpp>
#include <globals.hpp>
#include "button_interface.hpp"
#include "level_control.hpp"
#include <tuple>

#define initState   Operating
#define initLevel   std::make_tuple(0.0, 0.0)
#define maxLevel    5.0
#define minLevel   -5.0
#define step        0.2
#define cutStep     1.0

using namespace ForSyDe::SY;

SC_MODULE(button_control)
{
    SY_in<OverrideMsg> overrides;
    SY_in<Sensor> bassDn;
    SY_in<Sensor> bassUp;
    SY_in<Sensor> trebleDn;
    SY_in<Sensor> trebleUp;
    
    SY_out<Bass> bass;
    SY_out<Treble> treble;
    
    SY2SY<Button> button;
    SY2SY<std::tuple<abst_ext<Bass>,abst_ext<Treble>>> levelCntrl;
    SY2SY<std::tuple<abst_ext<Bass>,abst_ext<Treble>>> levels;
    SY2SY<std::tuple<abst_ext<Button>,abst_ext<OverrideMsg>>> tup_btn_ovr;
    
    SC_CTOR(button_control)
    {
        make_comb4("button_interface1", button_interface_func, 
                    button, bassUp, bassDn, trebleUp, trebleDn);
        
        make_zip("zip1", tup_btn_ovr, button, overrides);
        
        make_mealy("level_control1", level_control_ns_func, level_control_od_func,
                    std::make_tuple(initState,initLevel),
                    levelCntrl, tup_btn_ovr);
        
        make_hold("hold1",
                std::make_tuple(abst_ext<Bass>(0),abst_ext<Treble>(0)),
                levels, levelCntrl);
        
        make_unzip("unzip1", levels, bass, treble);
    }
};

#endif
