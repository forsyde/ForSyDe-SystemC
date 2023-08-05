/**********************************************************************           
    * compAvg.hpp -- A composite process which includes an averager   *
    *          with a delay.                                          *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef COMPAVG_HPP
#define COMPAVG_HPP

#include <forsyde.hpp>
#include "averager.hpp"

using namespace ForSyDe;

SC_MODULE(compAvg)
{
    SDF::in_port<double>  iport1;
    SDF::out_port<double> oport1;
    
    SDF::signal<double> din, dout;
    
    SC_CTOR(compAvg)
    {
        auto averager1 = SDF::make_combMN("averager1",
                                            averager_func,
                                            {2},
                                            {3,2},
                                            tie(oport1), 
                                            tie(iport1, dout)
        );
        get<0>(averager1->oport)(din);
        // auto averager1 = new SDF::combMN<std::tuple<double>,std::tuple<double,double>>("double", averager_func, {2}, {3, 2});
        // std::get<0>(averager1->iport)(iport1);
        // std::get<1>(averager1->iport)(dout);
        // std::get<0>(averager1->oport)(oport1);
        // std::get<0>(averager1->oport)(din);
        
        SDF::make_delayn("avginit1",0.0,2, dout, din);
    }
};

#endif
