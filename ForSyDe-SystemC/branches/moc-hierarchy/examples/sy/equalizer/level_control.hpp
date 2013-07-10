/**********************************************************************
    * level_control.hpp -- The user interface in the equalizer system *
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
#ifndef LEVEL_CONTROL_HPP
#define LEVEL_CONTROL_HPP

#include <forsyde.hpp>
#include <globals.hpp>

#include <tuple>

#define maxLevel    5.0
#define minLevel   -5.0
#define step        0.2
#define cutStep     1.0

using namespace ForSyDe;
using namespace ForSyDe::SY;

Level decreaseLevel(Level level, Level stepval)
{
    Level reducedLevel = level - stepval;
    if (reducedLevel >= minLevel)
        return reducedLevel;
    else 
        return minLevel;
}

Level increaseLevel(Level level, Level stepval)
{
    Level increasedLevel = level + stepval;
    if (increasedLevel <= maxLevel)
        return increasedLevel;
    else
        return maxLevel;
}

void level_control_ns_func(std::tuple<ButState,std::tuple<Bass,Treble>>& out_st,
                           const std::tuple<ButState,std::tuple<Bass,Treble>>& inp_st,
                           const abst_ext<std::tuple<abst_ext<Button>, abst_ext<OverrideMsg>>>& inp1)
{
    ButState state, newState;
    Bass bass, newBass;
    Treble treble, newTreble;
    abst_ext<Button> button;
    abst_ext<OverrideMsg> override;
    
    state = std::get<0>(inp_st);
    std::tie(bass,treble) = std::get<1>(inp_st);
    std::tie(button,override) = unsafe_from_abst_ext(inp1);
    
    if (state==Operating)
        if (is_present(override)&&unsafe_from_abst_ext(override)==Lock)
            newState = Locked;
        else newState = Operating;
    else 
        if (is_present(override)&&unsafe_from_abst_ext(override)==Release)
            newState = Operating;
        else newState = Locked;
    
    if (state==Locked)
        if (is_present(override)&&unsafe_from_abst_ext(override)==CutBass)
            newBass = decreaseLevel(bass,cutStep);
        else 
            if (is_present(button)&&unsafe_from_abst_ext(button)==BassDn)
                newBass = decreaseLevel(bass,step);
            else newBass = bass;
    else 
        if (is_present(button)&&unsafe_from_abst_ext(button)==BassDn)
            newBass = decreaseLevel(bass,step);
        else 
            if (is_present(button)&&unsafe_from_abst_ext(button)==BassUp)
                newBass = increaseLevel(bass,step);
            else newBass = bass;
    
    if (is_present(button)&&unsafe_from_abst_ext(button)==TrebleDn)
        newTreble = decreaseLevel(treble,step);
    else
        if (is_present(button)&&unsafe_from_abst_ext(button)==TrebleUp)
            newTreble = increaseLevel(treble,step);
        else newTreble = treble;
        
    out_st = std::make_tuple(newState,std::make_tuple(newBass,newTreble));
}
    
void level_control_od_func(abst_ext<std::tuple<abst_ext<Bass>,abst_ext<Treble>>>& out1,
              const std::tuple<ButState,std::tuple<Bass,Treble>>& st,
              const abst_ext<std::tuple<abst_ext<Button>, abst_ext<OverrideMsg>>>& inp1)
{
    abst_ext<Button> btn;
    abst_ext<OverrideMsg> om;
    std::tie(btn,om) = unsafe_from_abst_ext(inp1);
    
    if (is_absent(btn) && is_absent(om))
        set_abst(out1);
    else
    {
        auto tmp = std::get<1>(st);
        set_val(out1, std::make_tuple(abst_ext<Bass>(std::get<0>(tmp)),abst_ext<Treble>(std::get<1>(tmp))));
    }
}

#endif
