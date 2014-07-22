/**********************************************************************
    * sampler_counter.hpp -- A sample counter process                 *
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
// Contains an array of N counters. If the iports input and the clk
// input is high, the counter will increase its value

#include <forsyde.hpp>

using namespace sc_core;
using namespace ForSyDe;

SC_MODULE(sampler_counter)
{
    std::vector<SY::in_port<int>>   iports;
    SY::in_port<int>                clk;
    std::vector<SY::out_port<int>>  oports;

    // Vector of samplers
    std::vector<SY::smoore<int,int,int>*>   m_cnt_vec;
    std::vector<SY::scomb2<int,int,int>*>   and_vec;

    std::vector<SY::signal<int>>    sel_vec;
    std::vector<SY::signal<int>>    sig_vec;


    /*
     * Constructor for samplercounter mdule
     */
    sampler_counter(
            sc_core::sc_module_name,
            int N
        ) : iports(N), oports(N), sel_vec(N), sig_vec(N)
    {
        // Connect the fanout module input to the module input
        auto fo1 = SY::make_fanout("fo1", sel_vec[0], clk);
        for(int i=1;i<N;i++) fo1->oport1(sel_vec[i]);
        
        // Create sampler modules
        for(int i=0;i<N;i++){
            std::stringstream name;
            name << "smpl" << i;
            m_cnt_vec.push_back(
                SY::make_smoore(
                    name.str().c_str(),
                    counter_ns_func,
                    counter_od_func,
                    0,
                    oports[i],
                    sig_vec[i]
                )
            );

            name << "_and";
            and_vec.push_back(
                SY::make_scomb2(
                    name.str().c_str(),
                    and_func,
                    sig_vec[i],
                    iports[i],
                    sel_vec[i]
                )
            );
        }
    }
    
    static void counter_ns_func(int out, int st, int inp)
    {
        if(inp>0)
            out = st+1;
        else
            out = st;
    }

    static void counter_od_func(int out, int st)
    {
        out = st;
    }

    static void and_func(int out, int inp1, int inp2)
    {
        out = (inp1 && inp2) ? 1 : 0;
    }
};
