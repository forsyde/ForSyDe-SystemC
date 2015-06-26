/**********************************************************************
    * equalizer.hpp -- The main task of the equalizer system is to    *
    *                adjust the audio signal according to the         *
    *                Button Control, that works as a user interface.  *
    *                In addition, the bass level must not exceed a    *
    *                predefined threshold to avoid damage to the      *
    *                speakers.                                        *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          the Haskell version from Ingo Sander (ingo@kth.se)     *
    *                                                                 *
    * Purpose: Demonstrating how co-simulation with legacy codes is   *
    *          performed.                                             *
    *                                                                 *
    * Usage:   equalizer example                                      *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>

#include "button_control.hpp"
#include "audio_analyzer.hpp"
#include "distortion_control.hpp"
#include "audio_filter.hpp"

using namespace ForSyDe::SY;

SC_MODULE(equalizer)
{
    SY::in_port<Sensor> bassDn;
    SY::in_port<Sensor> bassUp;
    SY::in_port<Sensor> trebleDn;
    SY::in_port<Sensor> trebleUp;
    SY::in_port<double> input;

    SY::out_port<double> output;

    SY::signal<double> filteredInp;
    SY::signal<AnalyzerMsg> distFlag_sig;
    SY::signal<AnalyzerMsg> deldistFlag_sig;
    SY::signal<OverrideMsg> override_sig;
    SY::signal<Bass> bass;
    SY::signal<Treble> treble;

    equalizer(sc_module_name name)
    {
        auto audio_filter1 = new audio_filter("audio_filter1");
        audio_filter1->bass(bass);
        audio_filter1->treble(treble);
        audio_filter1->audioIn(input);
        audio_filter1->audioOut(filteredInp);
        audio_filter1->audioOut(output);

        auto audio_analyzer1 = new audio_analyzer("audio_analyzer1");
        audio_analyzer1->audioIn(filteredInp);
        audio_analyzer1->analyzerOut(distFlag_sig);

        make_delay("del", abst_ext<AnalyzerMsg>(), deldistFlag_sig, distFlag_sig);

        make_mealy("distortion_control1",
                    distortion_control_ns_func,
                    distortion_control_od_func,
                    std::make_tuple(Passed,0),
                    override_sig,
                    deldistFlag_sig);

        auto button_control1 = new button_control("button_control1");
        button_control1->overrides(override_sig);
        button_control1->bassDn(bassDn);
        button_control1->bassUp(bassUp);
        button_control1->trebleDn(trebleDn);
        button_control1->trebleUp(trebleUp);
        button_control1->bass(bass);
        button_control1->treble(treble);
    }
};
