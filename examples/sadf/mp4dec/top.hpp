/**********************************************************************
    * top.hpp -- the top module and testbench for the MPEG-4 decoder  *
    *                                                                 *
    * Author:  Hosein Attarzadeh (h_attarzadeh@sbu.ac.ir)             *
    *                                                                 *
    * Purpose: Demonstration of an example in the SADF MoC.           *
    *                                                                 *
    * Usage:   MPEG4-SP example                                       *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include "globals.hpp"
#include "mp4dec.hpp"
#include <iostream>

using namespace ForSyDe;

SC_MODULE(top)
{
    SADF::signal<MacroBlock<bs>> src;
    SADF::signal<Frame<fsr,fsc>> result;
    SADF::signal<frame_type> src2;
    
    SC_CTOR(top)
    {
        auto frame1   = Frame<fsr,fsc>();
        auto frame2   = Frame<fsr,fsc>();
        auto frame3   = Frame<fsr,fsc>();
        for (size_t i=0; i<frame1.size(); i++)
            for (size_t j=0; j<frame1[i].size(); j++)
            {
                frame1[i][j] = 2*i + j;
                frame2[i][j] = i==j ? 1 : 0;
                frame3[i][j] = 1;
            }
        
        auto block1   = Frame<bs,bs>();
        for (size_t i=0; i<block1.size(); i++)
            for (size_t j=0; j<block1[i].size(); j++)
                block1[i][j] = j - 1;
        
        auto iblocks  = frame2mblocks<fsr,fsc,bs>(frame2);
        auto p1block  = FullB<bs>{make_tuple(3, 3), make_tuple(0, -1), block1};
        auto p2block1 = FullB<bs>{make_tuple(3, 1), make_tuple(0, 1), block1};
        auto p2block2 = FullB<bs>{make_tuple(3, 3), make_tuple(1, 1), block1};
        
        // auto mvec = MotionVec{make_tuple(1,1), make_tuple(-1,0)};
        // auto mvecs = {mvec, mvec, mvec};

        auto testinp = iblocks;
        testinp.push_back(p1block);
        testinp.push_back(p2block1);
        testinp.push_back(p2block2);
        // SDF::make_constant("mcsrc", macroblock(), 10, src);
        SDF::make_vsource("mcsrc", testinp, src);
        
        // SDF::make_constant("ftsrc", I, 10, src2);
        SDF::make_vsource("ftsrc", {I,P30,P40,P50}, src2);
        
        auto mp4dec1 = new mp4dec("mp4dec1");
        mp4dec1->mb(src);
        mp4dec1->ft(src2);
        mp4dec1->out(result);
        
        SDF::make_sink("report1", [](const auto& inp) {cout<<inp<<endl;}, result);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif

};
