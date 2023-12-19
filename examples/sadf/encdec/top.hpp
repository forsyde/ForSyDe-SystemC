/**********************************************************************
    * main.cpp -- the main file and testbench for the SADF Encoder/   *
    *             Decoder example                                     *
    *                                                                 *
    * Author:  Hosein Attarzadeh (h_attarzadeh@sbu.ac.ir)             *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   SADF Encoder/Decoder                                   *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>

using namespace sc_core;
using namespace ForSyDe;
using namespace std;

// Define an enumerated tupe for the graph scenarios with values Sp, Sm, Sc
enum scen {Sp, Sm, Sc};

SC_MODULE(top)
{
    SADF::signal<int> ttot, ttotd, ttoep, ttoem, ttoec, eptod, emtod, ectod, dtor;
    SADF::signal<scen> ktot, ktoep, ktoem, ktoec, ktod;
#ifdef FORSYDE_SELF_REPORTING
    // Communication pipes
    FILE* report_pipe;      // Report pipe
    int report_pipe_fd = 0;     // Report pipe file descriptor
#endif

    SC_CTOR(top)
    {
        // The detector K        
        auto k_cds_func = [](auto&& new_scen, const auto& prev_scen, const auto& inp) {
            new_scen = (scen)((prev_scen+1) % 3);
        };

        auto k_kss_func = [](auto&& out, const auto& sc, const auto& inp) {
            auto&& [outT,outEp,outEm,outEc,outD] = out;

            switch (sc) {
                case Sp:
                    outT[0] = outEp[0] = outD [0]= Sp;
                    break;
                case Sm:
                    outT[0] = outEm[0]= outD[0] = Sm;
                    break;
                case Sc:
                    outT[0] = outT[1] = outEc[0] = outD[0] = Sc;
                    break;
                default:
                    break;
            }
        };

        SADF::make_detectorMN(
            "k",
            k_cds_func,
            k_kss_func,
            {
                {Sp,{1,1,0,0,1}},
                {Sm,{1,0,1,0,1}},
                {Sc,{2,0,0,1,1}}
            }, // k_table
            Sc,
            {},
            #ifdef FORSYDE_SELF_REPORTING
            &report_pipe,
            #endif
            tie(ktot, ktoep, ktoem, ktoec, ktod),
            tie()
        );

        // The kernel T        
        auto t_func = [&](auto&& out, const auto& sc, const auto& inp) {
            const auto& [inp1] = inp;
            auto&& [outT,outEp,outEm,outEc] = out;
            auto& cur_st = inp1[0];

            outT[0] = cur_st + 1;
            switch (sc) {
                case Sp:    outEp[0] = cur_st;  break;
                case Sm:    outEm[0] = cur_st;  break;
                case Sc:    outEc[0] = cur_st;  break;
            }
            if (cur_st > 20) wait();
        };
        
        SADF::make_kernelMN(
            "t",
            t_func,
            {
                {Sp,{{1},{1,1,0,0}}},
                {Sm,{{1},{1,0,1,0}}},
                {Sc,{{1},{1,0,0,1}}}
            }, // t_table
            #ifdef FORSYDE_SELF_REPORTING
            &report_pipe,
            #endif
            tie(ttot, ttoep, ttoem, ttoec),
            ktot,
            tie(ttotd)
        );

        SADF::make_delayn("totd", 0, 1, ttotd, ttot);

        // The kernel E+
        
        auto ep_func = [](auto&& out, const auto& sc, const auto& inp) {
            const auto& [inpT] = inp;
            auto&& [outD] = out;

            outD[0] = inpT[0] + 1;
        };
        
        SADF::make_kernelMN(
            "ep",
            ep_func,
            {
                {Sp,{{1},{1}}},
                {Sm,{{0},{0}}},
                {Sc,{{0},{0}}}
            }, // e_table
            #ifdef FORSYDE_SELF_REPORTING
            &report_pipe,
            #endif
            tie(eptod),
            ktoep,
            tie(ttoep)
        );

        // The kernel E-
        
        auto em_func = [](auto&& out, const auto& sc, const auto& inp) {
            const auto& [inpT] = inp;
            auto&& [outD] = out;

            outD[0] = {inpT[0] - 1};
        };

        SADF::make_kernelMN(
            "em",
            em_func,
            {
                {Sp,{{0},{0}}},
                {Sm,{{1},{1}}},
                {Sc,{{0},{0}}}
            }, // e_table
            #ifdef FORSYDE_SELF_REPORTING
            &report_pipe,
            #endif
            tie(emtod),
            ktoem,
            tie(ttoem)
        );

        // The kernel Ec

        auto ec_func = [](auto&& out, const auto& sc, const auto& inp) {
            const auto& [inpT] = inp;
            auto&& [outD] = out;

            outD[0] = inpT[0]+inpT[1];
            outD[1] = inpT[0]-inpT[1];
        };
        
        SADF::make_kernelMN(
            "ec",
            ec_func,
            {
                {Sp,{{0},{0}}},
                {Sm,{{0},{0}}},
                {Sc,{{2},{2}}}
            }, // ec_table
            #ifdef FORSYDE_SELF_REPORTING
            &report_pipe,
            #endif
            tie(ectod),
            ktoec,
            tie(ttoec)
        );

        // The kernel D
        
        auto d_func = [](auto&& out, const auto& sc, const auto& inp) {
            const auto& [inpEp, inpEm, inpEc] = inp;
            auto&& [outR] = out;

            switch (sc) {
                case Sp:
                    outR[0] = inpEp[0] - 1;
                    break;
                case Sm:
                    outR[0] = inpEm[0] + 1;
                    break;
                case Sc:
                    outR[0] = (inpEc[0] + inpEc[1]) / 2;
                    outR[1] = (inpEc[0] - inpEc[1]) / 2;
                    break;
            }
        };

        SADF::make_kernelMN(
            "d",
            d_func,
            {
                {Sp,{{1,0,0},{1}}},
                {Sm,{{0,1,0},{1}}},
                {Sc,{{0,0,2},{2}}}
            }, // d_table
            #ifdef FORSYDE_SELF_REPORTING
            &report_pipe,
            #endif
            tie(dtor),
            ktod,
            tie(eptod, emtod, ectod)
        );

        // The SDF sink actor r
        
        SDF::make_sink(
            "r", 
            [](const int& out) {
                std::cout <<"out = " <<out << std::endl;
            },
            dtor
        );
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
#ifdef FORSYDE_SELF_REPORTING
        while (report_pipe_fd<=0) // pipe is not open
        {
            report_pipe_fd = open("gen/self_report", O_WRONLY|O_NONBLOCK);
            if (report_pipe_fd > 0)
                report_pipe = fdopen(report_pipe_fd, "w");
        }
#endif
    }
#endif
#ifdef FORSYDE_SELF_REPORTING
    void end_of_simulation()
    {
        fclose(report_pipe);
    }
#endif

};

