/**********************************************************************           
    * compAvg.hpp -- A composite process which includes an averager   *
    *          with a delay.                                          *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a variadic program using zip and unzip*
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef COMPAVG_HPP
#define COMPAVG_HPP

#include <forsyde.hpp>
#include "averager.hpp"
#include <vector>
#include <tuple>

std::vector<uint> itoks = {3,2};
std::vector<uint> otoks = {2,2};

using namespace ForSyDe;
using namespace std;

SC_MODULE(compAvg)
{
    SDF::in_port<float>  iport;
    SDF::out_port<float> oport;
        
    SDF::signal<float> din, dout;
    SDF::signal< tuple<vector<float>,vector<float>> > zi, zo;
    
    SC_CTOR(compAvg)
    {
        auto zip1 = new SDF::zipN<float,float>("zip1",itoks);
        get<0>(zip1->iport)(iport);
        get<1>(zip1->iport)(dout);
        zip1->oport1(zi);
        
        SDF::make_comb("averager1", averager_func, 1, 1, zo, zi);
        
        auto unzip1 = new SDF::unzipN<float,float>("unzip1",otoks);
        unzip1->iport1(zo);
        get<0>(unzip1->oport)(oport);
        get<1>(unzip1->oport)(din);
        
        SDF::make_delayn("avginit1", (float)0, 2, dout, din);
    }
};

#endif
