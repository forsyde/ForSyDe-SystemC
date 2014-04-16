

#ifndef REPORT_HPP
#define REPORT_HPP

#include <forsyde.hpp>
#include <iostream>

using namespace ForSyDe;

void report_func(ttn_event<double> inp1)
{
#pragma ForSyDe begin report_func
    std::cout << "output value: " << inp1 << std::endl;
#pragma ForSyDe end
}

#endif
