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

SC_MODULE(plant)
{
		CT::in_port u;
    CT::out_port v;

		// CT::signal inter;

    SC_CTOR(plant)
		{
			// CT::make_filter("car",
			// 		{1.0},
			// 		{{m, b}},
			// 		sc_time(20,SC_MS),
			// 		v,
			// 		u
			// );
			auto car = new CT::filter("car", {1.0}, {{m, b}}, sc_time(20,SC_MS), sc_time(0.05,SC_NS), 1);
			car->iport1(u);
			car->oport1(v);
  	}
};

#endif
