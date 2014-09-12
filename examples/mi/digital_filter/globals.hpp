/**********************************************************************
    * globals.hpp -- the globals definitions for the digital filter   *
    *                                                                 *
    * Authors: Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a heterogeneous system.               *
    *                                                                 *
    * Usage:   The digital filter example                             *
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
sc_time samplingPeriod = sc_time(1000,SC_US);

const std::vector<double> coefs = {
    0.09638467386774,  0.09877932526544,    0.100594742773,    0.101814330123,
     0.1024269279708,   0.1024269279708,    0.101814330123,    0.100594742773,
    0.09877932526544,  0.09638467386774
};

#endif
