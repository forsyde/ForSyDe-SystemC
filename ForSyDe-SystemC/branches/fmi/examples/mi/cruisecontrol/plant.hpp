/**********************************************************************
        * plant.hpp -- the model of a car to be controlled                *
        *          example                                                *
        *                                                                 *
        * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
        *                                                                 *
        * Purpose: Demonstration of a heterogeneous system                *
        *                                                                 *
        * Usage:   Cruise control example                                 *
        *                                                                 *
        * License: BSD3                                                   *
        *******************************************************************/

#ifndef PLANT_HPP
#define PLANT_HPP

#include <forsyde.hpp>

using namespace sc_core;
using namespace ForSyDe;

#define m 1000
#define b 50
#define DEAT_TIME .1  // transport delay of the plant in seconds

SC_MODULE(plant)
{
        CT::in_port u;
    CT::out_port v;

        CT::signal v_t;

    SC_CTOR(plant)
        {
            // CT::make_filter("car",
            //      {1.0},
            //      {{m, b}},
            //      sc_time(20,SC_MS),
            //      v,
            //      u
            // );
            #ifndef FORSYDE_COSIMULATION_WRAPPERS
            auto car = new CT::filter("car",
                    {1.0},
                    {{m, b}},
                    sc_time(20,SC_MS),
                    sc_time(0.05,SC_NS),
                    1
            );
            car->iport1(u);
            car->oport1(v_t);
            #else
            CT::make_fmi2cswrap("car", "fmi2/car.fmu", 0, 1, sc_time(20,SC_MS), v_t, u);
            #endif

            CT::make_shift("dead_time", sc_time(DEAT_TIME, SC_SEC), v, v_t);

    }
};

#endif
