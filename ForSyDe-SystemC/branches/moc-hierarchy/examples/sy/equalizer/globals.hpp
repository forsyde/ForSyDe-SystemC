/**********************************************************************
    * types.hpp -- the global definitions used in the equalizer example*
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *          the Haskell version from Ingo Sander (ingo@kth.se)     *
    *                                                                 *
    * Purpose: Demonstrating how co-simulation with legacy codes is   *
    *          performed.                                             *
    *                                                                 *
    * Usage:   equalizer example                                      *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/
#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <forsyde.hpp>
#include <vector>
#include <tuple>

using namespace ForSyDe;

enum AnalyzerMsg {Pass, Fail};
DEFINE_TYPE(AnalyzerMsg);

enum OverrideMsg {Lock, CutBass, Release};
DEFINE_TYPE(OverrideMsg);

enum ButState {Operating, Locked};
DEFINE_TYPE(ButState);

enum DistState {Passed, Failed, DLocked};
DEFINE_TYPE(DistState);

enum Sensor {Active};
DEFINE_TYPE(Sensor);

enum Button {BassDn, BassUp, TrebleDn, TrebleUp};
DEFINE_TYPE(Button);

typedef double Level;

typedef Level Bass;

typedef Level Treble;

DEFINE_TYPE_NAME(std::complex<double>,"complex<double>");
DEFINE_TYPE_NAME(std::vector<std::complex<double>>,"vector<complex<double>>");
DEFINE_TYPE_NAME(std::vector<abst_ext<std::complex<double>>>,"vector<abst_ext<complex<double>>>");
DEFINE_TYPE_NAME(std::vector<double>,"vector<double>");

typedef std::tuple<abst_ext<double>,abst_ext<double>> tup_abs_bass_abs_treble;
DEFINE_TYPE_NAME(tup_abs_bass_abs_treble,"tuple<abst_ext<double>,abst_ext<double>>");

typedef std::tuple<abst_ext<Button>,abst_ext<OverrideMsg>> tup_abs_btn_abs_ovrmsg;
DEFINE_TYPE_NAME(tup_abs_btn_abs_ovrmsg,"tuple<abst_ext<Button>,abst_ext<OverrideMsg>>");

// Filter Coefficients
std::vector<double> lpCoeff =
     { 0.01392741661548, 0.01396895728902,
       0.01399870011280, 0.01401657422649,
       0.01402253700635, 0.01401657422649,
       0.01399870011280, 0.01396895728902,
       0.01392741661548 };

std::vector<double> bpCoeff =
     { 0.06318761339784, 0.08131651217682,
       0.09562326700432, 0.10478344432968,
       0.10793629404886, 0.10478344432968,
       0.09562326700432, 0.08131651217682,
       0.06318761339784 };

std::vector<double> hpCoeff =
     { -0.07883878579454, -0.09820015927379,
       -0.11354603917221, -0.12339860164118,
        0.87320570334018, -0.12339860164118,
       -0.11354603917221, -0.09820015927379,
       -0.07883878579454 };

#endif
