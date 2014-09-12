

#ifndef REPORT_HPP
#define REPORT_HPP

#include <forsyde.hpp>
#include <iostream>

using namespace ForSyDe;

void report_func(ttn_event<double> inp1)
{
#pragma ForSyDe begin report_func
    //  Switch between twe following lines and redirect the output to a
    // data file and plot using gnuplot with a command like:
    // gnuplot -p -e 'plot "file.dat" with impulse'
    //~ std::cout << "output value: " << inp1 << std::endl;
    //~ /*
    std::cout << get_time(inp1).to_seconds() << " " 
              << from_abst_ext(get_value(inp1),0) << std::endl;
    //~ */
#pragma ForSyDe end
}

#endif
