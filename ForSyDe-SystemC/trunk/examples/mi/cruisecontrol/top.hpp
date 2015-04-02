/**********************************************************************
    * top.hpp -- the top module and testbench for the cruise control  *
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

#include <forsyde.hpp>
#include "plant.hpp"
#include "controller.hpp"

using namespace sc_core;
using namespace ForSyDe;

void sub_func(double& out, const double& inp1, const double& inp2)
{
    out = inp1 - inp2;
}

SC_MODULE(top)
{
  CT::signal u, v, vout;
	SY::signal<double> r, e, du, dv;

	SC_CTOR(top)
	{
		SY::make_sconstant("step", 1.0, 0, r);

    SY::make_scomb2("sub1", sub_func, e, r, dv);

    #ifndef FORSYDE_COSIMULATION_WRAPPERS
    SY::make_smealy("controller1",
              controller_ns_func,
              controller_od_func,
              std::make_tuple(0.0, 0.0),
              du,
              e
        );
    #else
    SY::make_gdbwrap("controller1",
              "software/controller",
              du,
              e
        );
    #endif

    make_SY2CT("d2a", sc_time(20,SC_MS), HOLD, u, du);

		auto plant1 = new plant("plant1");
    plant1->u(u);
    plant1->v(v);
    plant1->v(vout);

    make_CT2SY("a2d", sc_time(20,SC_MS), dv, v);

    CT::make_traceSig("output", sc_time(20,SC_MS), vout);
	}
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};
