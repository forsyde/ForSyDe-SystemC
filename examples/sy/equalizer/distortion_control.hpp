/**********************************************************************
    * distortion_control.hpp --                                       *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          the Haskell version from Ingo Sander (ingo@kth.se)     *
    *                                                                 *
    * Purpose: Demonstrating how co-simulation with legacy codes is   *
    *          performed.                                             *
    *                                                                 *
    * Usage:   Equalizer example                                      *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/
#ifndef DISTORTION_CONTROL_HPP
#define DISTORTION_CONTROL_HPP

#include <forsyde.hpp>
#include <globals.hpp>

#include <tuple>

#define lim 3

using namespace ForSyDe::SY;

void distortion_control_ns_func(std::tuple<DistState,int>& out_st,
                                const std::tuple<DistState,int>& inp_st,
                                const abst_ext<AnalyzerMsg>& inp1)
{
    DistState inp_st1, out_st1;
    int inp_st2, out_st2;
    std::tie(inp_st1,inp_st2) = inp_st;   // retrieve the state elements
#pragma ForSyDe begin distortion_control_ns_func 
    if (is_absent(inp1))
        {out_st1 = inp_st1; out_st2 = inp_st2;}
    else
        switch (inp_st1)
        {
            case Passed:
                if (unsafe_from_abst_ext(inp1) == Pass)
                    {out_st1 = Passed; out_st2 = inp_st2;}
                else
                    {out_st1 = Failed; out_st2 = lim;}
                break;
            case Failed:
                if (unsafe_from_abst_ext(inp1) == Pass)
                    {out_st1 = DLocked; out_st2 = inp_st2;}
                else
                    {out_st1 = Failed; out_st2 = inp_st2;}
                break;
            case DLocked:
                if (unsafe_from_abst_ext(inp1) == Fail)
                    {out_st1 = Failed; out_st2 = lim;}
                else
                    {out_st1 = (inp_st2==1)? Passed : DLocked; out_st2 = inp_st2-1;}
        }
#pragma ForSyDe end
    
    out_st = std::make_tuple(out_st1, out_st2);
}
    
void distortion_control_od_func(abst_ext<OverrideMsg>& out1,
                                const std::tuple<DistState,int>& st,
                                const abst_ext<AnalyzerMsg>& inp1)
{
    DistState st1;
    int st2;
    std::tie(st1,st2) = st;   // retrieve the state elements
#pragma ForSyDe begin distortion_control_od_func
    if (is_absent(inp1))
        set_abst(out1);
    else
        switch (st1)
        {
            case Passed:
                if (unsafe_from_abst_ext(inp1) == Pass)
                    set_abst(out1);
                else
                    set_val(out1, Lock);
                break;
            case Failed:
                if (unsafe_from_abst_ext(inp1) == Pass)
                    set_abst(out1);
                else
                    set_val(out1, CutBass);
                break;
            case DLocked:
                if (unsafe_from_abst_ext(inp1) == Fail)
                    set_abst(out1);
                else
                    (st2==1) ? set_val(out1, Release) : set_abst(out1);
        }
#pragma ForSyDe end
}

#endif
