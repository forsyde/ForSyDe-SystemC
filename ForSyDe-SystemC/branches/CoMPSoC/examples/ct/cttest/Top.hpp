/**********************************************************************           
    * Top.hpp -- the Top process and testbench for a toy CT system    *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy CT example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/
#ifndef PI
#define PI 3.14159265358979323846
#endif

#include <forsyde.hpp>
#include "pwr.hpp"

using namespace sc_core;
using namespace ForSyDe::CT;

std::vector<SubSignal> iv = {
    SubSignal(sc_time(0,SC_SEC), sc_time(1,SC_SEC),[](sc_time t){
        return sin(2*PI*t.to_seconds());
        }),
    SubSignal(sc_time(1,SC_SEC), sc_time(2,SC_SEC),[](sc_time t){
        return -sin(2*PI*t.to_seconds());
        }),
    SubSignal(sc_time(2,SC_SEC), sc_time(3,SC_SEC),[](sc_time t){
        return sin(2*PI*t.to_seconds());
        })
};

SC_MODULE(Top)
{
    sc_fifo<SubSignal> src1, src2, src3, src4, des1, des2;
    
    pwr pwr1;
    add add1;
    //source stimuli1
    vsource stimuli1;
    printSig report1;
    traceSigs report2;
    
    SC_CTOR(Top): pwr1("pwr1"), add1("add1"),
                  /*stimuli1("stimuli1", sc_time(0,SC_SEC), sc_time(1,SC_SEC),
                  [](sc_time t)->CTTYPE{
                      return sin(2*PI*t.to_seconds());
                  }),*/ 
                  stimuli1("stimuli1",iv),
                  report1("report1", sc_time(10,SC_MS)),
                  report2("report2", sc_time(10,SC_MS))
    {
        
        stimuli1.oport(src1);
        stimuli1.oport(src2);
        stimuli1.oport(src3);
        stimuli1.oport(src4);
        
        add1.iport1(src2);
        add1.iport2(src3);
        add1.oport(des1);
        
        pwr1.iport(des1);
        pwr1.oport(des2);
        
        report1.iport(src1);
        
        report2.iport(src4);
        report2.iport(des2);
    }
   
};
