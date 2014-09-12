/**********************************************************************
    * sweep_ctrl.hpp -- A sweep controller for DAC                    *
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
// Sweep DAC value from dacmin to dacmax with step step

#include <forsyde.hpp>

using namespace sc_core;
using namespace ForSyDe;

SC_MODULE(sweep_ctrl)
{
    SY::in_port<int>        clk;      // "Clock"
    SY::out_port<int>       smpl_en;  // Sample enable signal
    SY::out_port<double>    th;       // Threshold

    SY::signal<int>         sc_in, ac_in, ac_out, a2_in;//, a2_out, sc_out;
    
    /*
     * Constructor for the sweep controller top module
     */
    SC_CTOR(sweep_ctrl)
    {
        
        auto fo1 = SY::make_fanout("fo1", sc_in, clk);
        fo1->oport1(ac_in);
        fo1->oport1(a2_in);
        
        SY::make_smoore("avg_ctrl1",
            avg_ctrl_ns_func,
            avg_ctrl_od_func,
            0,
            ac_out,
            ac_in
        );
        
        auto and1 = SY::make_scomb2("and1", and_func, smpl_en, ac_out, a2_in);
        //~ and1->oport1(a2_out);
        
        //~ SY::make_sink("report1", report_func, a2_out);
        
        SY::make_smoore("sc",
            sweep_ctrl_ns_func,
            sweep_ctrl_od_func,
            std::make_tuple(static_cast<double>(DACMIN),0),
            th,
            sc_in
        );
    }
    
    static void sweep_ctrl_ns_func(std::tuple<double,int>& out,
            const std::tuple<double,int>& st,
            const int& inp
    )
    {
        double val = std::get<0>(st);
        int x = std::get<1>(st);
        
        if(inp)
        {
            x++;
            if(x ==AVGFACTOR){
                x=0;
                if(val==DACMAX)
                    val = val;
                else
                    val = val + DACSTEP;
            }
        }
        else
            val = val;
        out = std::make_tuple(val, x);
    }

    static void sweep_ctrl_od_func(double& out, const std::tuple<double,int>& st)
    {
        out = std::get<0>(st);
      /*
      if(st < _N)
        return 1;
      else return 0;
      */
    }
    
    static void avg_ctrl_ns_func(int& out, const int& st, const int& inp)
    {
        if(inp)
            out = st+1;
        else
            out = st;
    }

    static void avg_ctrl_od_func(int& out, const int& st)
    {
        if(st<TOTAVG)
            out = 1;
        else 
            out = 0;
    }
    
    static void and_func(int& out, const int& inp1, const int& inp2)
    {
        out = (inp1 && inp2) ? 1 : 0;
    }
    
    static void report_func(const int& inp)
    {
        std::cout << " output value: " << inp << std::endl;
    }
};
