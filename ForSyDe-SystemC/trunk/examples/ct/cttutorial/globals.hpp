/**********************************************************************
    * globals.hpp -- the globals definitions                          *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple process.                     *
    *                                                                 *
    * Usage:   CT tutorial example                                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <forsyde.hpp>
using namespace sc_core;
using namespace ForSyDe;

// Period of the cosine wave
sc_time CosPeriod = sc_time(200,SC_MS);
// The end time of the cosine wave signal
sc_time endT = sc_time(1.0,SC_SEC);

double cutoffFreq = 2.0/(CosPeriod.to_seconds());
// Sampling period of the solver for filter
sc_time samplingPeriod = sc_time(100,SC_US);

std::vector<CTTYPE> nums = {1.0};
std::vector<CTTYPE> dens = {1.0/(M_PI*cutoffFreq), 1.0};

#endif
