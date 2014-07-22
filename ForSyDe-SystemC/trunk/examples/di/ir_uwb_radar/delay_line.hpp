/**********************************************************************
    * delay_line.hpp -- Configurable SY MoC tapped delayline          *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          Based on a model developed by (Novelda AS)             *
    *                                                                 *
    * Purpose: Demonstration of a single cyber-physical system        *
    *                                                                 *
    * Usage:   IR UWB radar transceiver example                       *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/
// Takes a single SY signal as input and creates N outputs

#include <forsyde.hpp>

using namespace sc_core;
using namespace ForSyDe;


SC_MODULE(delay_line)   
{
    SY::in_port<int>                iport;

    std::vector<SY::out_port<int>>  oports;
    
    // Vector of forks
    std::vector<SY::fanout<int>*>       fork_vec;
    // Vector of delay elements
    std::vector<SY::sdelay<int>*>        delay_vec;

    // Array of signals for hooking up the forks and delays
    std::vector<SY::signal<int>>    signal_vec;


    /*
     * Constructor for samplercounter mdule
     * NB!! REQUIRES N>1
     */
    delay_line(
            sc_core::sc_module_name,
            int N
        ) : oports(N), signal_vec(N-1)
    {

        //////////////////////////////
        // Submodule instatiation
        //////////////////////////////

        // Create delay modules
        for(int i=0;i<N;i++){
            std::stringstream name;
            name << "delay" << i;
            delay_vec.push_back(new SY::sdelay<int>( name.str().c_str(), 0));
        }

        //////////////////////////////
        // Submodule connections
        //////////////////////////////

        // Connect the first delay to the input
        delay_vec[0]->iport1(iport);
        delay_vec[0]->oport1(signal_vec[0]);

        // Hook up the rest of the inputs
        for(int i=1;i<(N-1);i++){
            delay_vec[i]->iport1(signal_vec[i-1]);
            delay_vec[i]->oport1(signal_vec[i]);
        }

        delay_vec[N-1]->iport1(signal_vec[N-2]);
        for(int i=0;i<N;i++){
            delay_vec[i]->oport1(oports[i]);
        }

    }

    ~delay_line()
    {
        for(unsigned int i=0;i<delay_vec.size();i++)
        {
            delete delay_vec[i];
        }

    }
};
