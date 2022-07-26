/**********************************************************************
    * vad_source.hpp                                                  *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          adapted from KisTA: https://github.com/nandohca/kista  *
    *                                                                 *
    * Purpose: Provide input stimuli                                  *
    *                                                                 *
    * Usage:   The VAD example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef VADSOURCE_HPP
#define VADSOURCE_HPP

#include <forsyde.hpp>
#include <iostream>

using namespace ForSyDe::SDF;

void VADFilesource_func(tuple_of_vectors<r_t,r_t,r_t,short,short,rc_t,short>& out,
                        const std::string& line)
{
    // Resize all the vectors to contain 1 element
    // TODO: Generalize
    std::get<0>(out).resize(1);
    std::get<1>(out).resize(1);
    std::get<2>(out).resize(1);
    std::get<3>(out).resize(1);
    std::get<4>(out).resize(1);
    std::get<5>(out).resize(1);
    std::get<6>(out).resize(1);
#pragma ForSyDe begin VADFilesource_func

    std::stringstream ss(line);
    
    // read r_h_1
    for (short& val: (std::get<0>(out))[0])
        ss >> val;
    ss.ignore(1);   //ignore comma
    
    // read r_h_2
    (std::get<1>(out))[0] = (std::get<0>(out))[0];
    //~ for (short& val: (std::get<1>(out))[0])
        //~ ss >> val;
    //~ ss.ignore(1);   //ignore comma
    
    // read r_l
    for (short& val: (std::get<2>(out))[0])
        ss >> val;
    ss.ignore(1);   //ignore comma
    
    // read scal_acf_1
    ss >> (std::get<3>(out))[0];
    ss.ignore(1);   //ignore comma
    
    // read scal_acf_1
    (std::get<4>(out))[0] = (std::get<3>(out))[0];
    //~ ss >> (std::get<4>(out))[0];
    //~ ss.ignore(1);   //ignore comma
    
    // read rc
    for (short& val: (std::get<5>(out))[0])
        ss >> val;
    ss.ignore(1);   //ignore comma
    
    // read pitch
    ss >> (std::get<6>(out))[0];
    
#pragma ForSyDe end
}


#endif
