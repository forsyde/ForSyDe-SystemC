/**********************************************************************
    * radar_simplified.hpp -- the IR UWB simplified radar model       *
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

#include <forsyde.hpp>

using namespace sc_core;
using namespace ForSyDe;

//--- Radar configuration
#define PRF 100e6           // Pulse Repetition Frequency, 100 MHz
#define ADC_PERIOD 0.05e-9 // Front-end ADC 50 ps (20 GS/s)
#define DACSTEP 0.01
#define DACMIN -1
#define DACMAX 1
#define AVGFACTOR 5

//--- Some handy variables 
#define PRP (1/PRF)           // PRP, 1/PRF
#define RATIO PRP/ADC_PERIOD  // The downsampling ratio of the backend=SPERIOD/ADC_PERIOD
#define TOTAVG ((DACMAX-DACMIN)/DACSTEP)*AVGFACTOR

// Simulation time required to finish the measurement:
// TOTAVG*SPERIOD*2

#include <delay_line.hpp>
#include <sweep_ctrl.hpp>
#include <sampler_counter.hpp>

/*
 * Toplevel radar module
 */
SC_MODULE(radar_simplified)
{
    CT::in_port                 sig;         // Signal input
    std::vector<SY::out_port<int>>  oports;  // Sampler output
    
    //--- Interconnect signals
    std::vector<SY::signal<int>>    to_sampler; // Array of signals for connecting the delay line and the sampler
    SY::signal<std::tuple<int,int>> from_clk_st;// "Clock" and state signal
    SY::signal<int>             from_clk;    // "Clock" signal
    SY::signal<double>          threshold;   // Threshold from Sweep Controller
    SY::signal<int>             smpl_en;     // Sample enable signal from Sweep Controller
    SY::signal<double>          to_th;       // Output of ADC
    SY::signal<int>             from_th;     // Output of thresholder
    SY::signal<double>          n;
    SY::signal<double>          s;

    /*
    * Constructor for the radar module
    */
    radar_simplified(
            sc_module_name,
            int N,
            double noise_var
        ) : oports(N), to_sampler(N)
    {
        // Front-end: ADC + thresholder
        
        auto noise = SY::make_gaussian("noise", noise_var, 0, n);
        
        make_CT2SY("adc", sc_time(ADC_PERIOD,SC_SEC), s, sig);

        make_scomb2("add1", add_func, to_th, n, s);

        // SY thresholder
        make_scomb2("threshold1", threshold_func, from_th, to_th, threshold);

        // Back-end/downsampler
        
        // Synchronous delay line
        auto delay_line1 = new delay_line("delay_line1",N);
        delay_line1->iport(from_th);
        for(int i=0;i<N;i++) delay_line1->oports[i](to_sampler[i]);
        
        // "Clock", generates a stream of ones and zeros
        SY::make_ssource("clk_gen1", clk_gen_func, std::make_tuple(0,0), 0, from_clk_st);
        
        SY::make_scomb("strip1", strip_func, from_clk, from_clk_st);
        
        // Controls "DAC" sweep and sampling/averaging
        auto sweep_ctrl1 = new sweep_ctrl("sweep_ctrl1");
        sweep_ctrl1->clk(from_clk);
        sweep_ctrl1->th(threshold);
        sweep_ctrl1->smpl_en(smpl_en);

        // Pure synchronous sampler
        auto sampler_counter1 = new sampler_counter("sampler_counter1", N);
        sampler_counter1->clk(smpl_en);
        for(int i=0;i<N;i++)
        {
          sampler_counter1->iports[i](to_sampler[i]);
          sampler_counter1->oports[i](oports[i]);
        }
    }
    
    static void add_func(double out, double inp1, double inp2)
    {
        out = inp1 + inp2;
    }

    static void threshold_func(int out, double sig, double threshold)
    {
        if(sig>threshold)
            out = 1;
        else
            out = 0;
    }

    static void clk_gen_func(std::tuple<int,int>& out, const std::tuple<int,int>& st)
    {
        int out_val = std::get<0>(st);
        int state = std::get<1>(st);
        
        state++;
        if(state == RATIO)
        {
            state = 0;
            out_val = 1;
        } 
        else
            out_val = 0;
        out = std::make_tuple(out_val,state);
    }
    
    static void strip_func(int& out, const std::tuple<int,int>& inp)
    {
        out = std::get<0>(inp);
    }
};
