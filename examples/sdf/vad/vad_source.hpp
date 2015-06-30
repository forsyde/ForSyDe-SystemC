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

void VADFilesource_func(std::tuple<r_t,r_t,r_t,short,short,rc_t,short>& out,
                        const std::string& line)
{
#pragma ForSyDe begin report_func

    std::stringstream ss(line);
    
    //~ ss >> std::get<0>(out) >> "," >> std::get<1>(out) >> "," >>
        //~ std::get<2>(out) >> "," >> std::get<3>(out) >> "," >>
        //~ std::get<4>(out) >> "," >> std::get<5>(out) >> "," >>
        //~ std::get<6>(out);
        
    // read r_h_1
    for (short& val: std::get<0>(out))
        ss >> val;
    ss.ignore(1);   //ignore comma
    
    // read r_h_2
    for (short& val: std::get<1>(out))
        ss >> val;
    ss.ignore(1);   //ignore comma
    
    // read r_l
    for (short& val: std::get<2>(out))
        ss >> val;
    ss.ignore(1);   //ignore comma
    
    // read scal_acf_1
    ss >> std::get<3>(out);
    ss.ignore(1);   //ignore comma
    
    // read scal_acf_1
    ss >> std::get<4>(out);
    ss.ignore(1);   //ignore comma
    
    // read rc
    for (short& val: std::get<5>(out))
        ss >> val;
    ss.ignore(1);   //ignore comma
    
    // read pitch
    ss >> std::get<6>(out);
    
#pragma ForSyDe end
}


#endif
