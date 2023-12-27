/**********************************************************************
    * topexp.hpp -- the top module testbench for the MPEG-4 decoder   *
    *               used for comparison experiments                   *
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
#include <iomanip>

// The input frame type file name
#define FT_FILE "ft.inp"
// The input macroblocks file name
#define MB_FILE "mbInputs.inp"

using namespace ForSyDe;
size_t mbCount = 0;
SC_MODULE(top)
{
    SADF::signal<MacroBlock<bs>> mbsrc;
    SADF::signal<Frame<fsr,fsc>> result;
    SADF::signal<frame_type> ftsrc;
    
    SC_CTOR(top)
    {        
        // Read the list of Macroblocks from the input file
        auto mbFile = ifstream(MB_FILE);
        vector<MacroBlock<bs>> mbVec;
        char ch;
        
        mbFile >> ch;
        if (ch != '[')
            throw runtime_error("Wrong input format");
        while (true)
        {
            MacroBlock<bs> mb;
            mbFile >> mb;
            mbVec.push_back(mb);
            mbFile >> ch;
            mbCount++;
            if (ch == ']')
                break;
            else if (ch != ',')
                throw runtime_error("Wrong input format");
        }
        cout << "Number of input Macroblocks: " << mbCount << endl;
        mbCount = 0;

        // Read the list of frames types from the input file
        auto ftFile = ifstream(FT_FILE);
        vector<frame_type> ftVec;
        ftFile >> ch;
        if (ch != '[')
            throw runtime_error("Wrong input format");
        while (true)
        {
            string s;
            ftFile >> quoted(s);
            ftVec.push_back(str2frame[s]);
            ftFile >> ch;
            if (ch == ']')
                break;
            else if (ch != ',')
                throw runtime_error("Wrong input format");
        }

        // Instantiate the processes
        SDF::make_vsource("mcsrc", mbVec, mbsrc);
        SDF::make_vsource("ftsrc", ftVec, ftsrc);

        auto mp4dec1 = new mp4dec("mp4dec1");
        mp4dec1->mb(mbsrc);
        mp4dec1->ft(ftsrc);
        mp4dec1->out(result);

        SDF::make_sink("report1", [](const auto& inp) {mbCount++;}, result);

    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif

};
