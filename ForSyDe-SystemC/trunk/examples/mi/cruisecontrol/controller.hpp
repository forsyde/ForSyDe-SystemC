/**********************************************************************
    * controller.hpp -- digital cruise controller                     *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a heterogeneous system                *
    *                                                                 *
    * Usage:   Cruise control example                                 *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <forsyde.hpp>

using namespace sc_core;
using namespace ForSyDe;


void controller_ns_func(std::tuple<double,double>& ns,
    const std::tuple<double,double>& cs,
    const double& inp)
{
    double pinp, pout;
    std::tie(pinp, pout) = cs;
    auto out = .9998 * pout + 2445 * .2 * (inp - .999*pinp);
    ns = std::make_tuple(inp, out);
}

void controller_od_func(double& out,
    const std::tuple<double,double>& cs,
    const double& inp)
{
    double pinp, pout;
    std::tie(pinp, pout) = cs;
    out = .9998 * pout + 2445 * .2 * (inp - .999*pinp); // K * C

    //out = 451 * inp;
}

#endif
