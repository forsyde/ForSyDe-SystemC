/**********************************************************************
    * detectors.hpp -- Implementation of Kernels                    *
    *                                                                 *
    * Author:  Hosein Attarzadeh (h_attarzadeh@sbu.ac.ir)             *
    *                                                                 *
    * Purpose: Demonstration of an example in the SADF MoC.           *
    *                                                                 *
    * Usage:   MPEG4-SP example                                       *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef KERNELS_HPP
#define KERNELS_HPP

#include <forsyde.hpp>
#include "globals.hpp"

using namespace sc_core;
using namespace ForSyDe;
using namespace std;

// void vld_func(tuple<vector<posblock>,vector<tuple<position,motionvec>>>& out, 
//                     const frame_type& _scenarios_state, 
//                     const tuple<vector<macroblock>>& inp)
auto vld_func = [](auto&& out, const auto& _scenarios_state, const auto& inp)
{
    const auto& [mb] = inp;
    auto&& [vld2idct, vld2mc] = out;
    
    if (_scenarios_state == I)
    {
        for (size_t i=0; i<mb.size(); i++)
            if (holds_alternative<FullB<bs>>(mb[i]))
            {
                const auto& fb = get<FullB<bs>>(mb[i]);
                vld2idct[i] = PosB<bs>{fb.pos, fb.block};
            }
            else
            {
                const auto& pb = get<PosB<bs>>(mb[i]);
                vld2idct[i] = PosB<bs>{pb.pos, pb.block};
            }
    }
    else // _scenarios_state > P0
    {
        for (size_t i=0; i<mb.size(); i++)
            if (holds_alternative<FullB<bs>>(mb[i]))
            {
                const auto& fb = get<FullB<bs>>(mb[i]);
                vld2idct[i] = PosB<bs>{fb.pos, fb.block};
                vld2mc[i] = MotionVec{fb.pos, fb.motionV};
            }
            else
            {
                const auto& pb = get<PosB<bs>>(mb[i]);
                vld2idct[i] = PosB<bs>{pb.pos, pb.block};
            }
    }
};


// void idct_func(tuple<vector<posblock>>& out, 
//                     const frame_type& _scenarios_state, 
//                     const tuple<vector<posblock>>& inp)
auto idct_func = [](auto&& out, const auto& _scenarios_state, const auto& inp)
{
    const auto& [vld2idct] = inp;
    auto&& [idct2rc] = out;

    const auto& pb = get<PosB<bs>>(vld2idct[0]);
    idct2rc[0] = PosB<bs>{pb.pos, inverseDCT(pb.block)};
};

// void mc_func(tuple<vector<frame>>& out, 
//                     const frame_type& _scenarios_state, 
//                     const tuple<vector<tuple<position,motionvec>>,vector<frame>>& inp)
auto mc_func = [](auto&& out, const auto& _scenarios_state, const auto& inp)
{
    const auto& [vld2mc, rc2mc] = inp;
    auto&& [mc2rc] = out;

    if (_scenarios_state == I)
        mc2rc[0] = Frame<fsr,fsc>();
    else // _scenarios_state > P0
        mc2rc[0] = motionComp<bs>(vld2mc, rc2mc[0]);
};

// void rc_func(tuple<vector<frame>,vector<bool>>& out, 
//                     const frame_type& _scenarios_state, 
//                     const tuple<vector<posblock>,vector<frame>>& inp)
auto rc_func = [](auto&& out, const auto& _scenarios_state, const auto& inp)
{
    const auto& [idct2rc, mc2rc] = inp;
    auto&& [rc2mc, rc2fd] = out;
    
    rc2mc[0] = frameRC(idct2rc,mc2rc[0]);
    rc2fd[0] = true;
};


#endif
