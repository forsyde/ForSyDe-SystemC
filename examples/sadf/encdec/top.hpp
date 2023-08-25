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
            switch (sc) {
                case Sp:
                    get<0>(out) = get<1>(out) = get<4>(out) = {Sp};
                    break;
                case Sm:
                    get<0>(out) = get<2>(out) = get<4>(out) = {Sm};
                    break;
                case Sc:
                    get<0>(out) = {Sc, Sc};
                    get<3>(out) = get<4>(out) = {Sc};
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
            auto cur_st = get<0>(inp)[0];
            get<0>(out) = {cur_st + 1};
            switch (sc) {
                case Sp:
                    get<1>(out) = {cur_st};
                    break;
                case Sm:
                    get<2>(out) = {cur_st};
                    break;
                case Sc:
                    get<3>(out) = {cur_st};
                    break;
                default:
                    break;
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
            out = {{ {get<0>(inp)[0] + 1} }};
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
            out = {{ {get<0>(inp)[0] - 1} }};
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
            auto a = get<0>(inp)[0];
            auto b = get<0>(inp)[1];
            out = {{ {a + b, a - b} }};
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
            switch (sc) {
                case Sp:
                    out = {{ {get<0>(inp)[0] - 1} }};
                    break;
                case Sm:
                    out = {{ {get<1>(inp)[0] + 1} }};
                    break;
                case Sc:
                {
                    auto n = get<2>(inp)[0];
                    auto m = get<2>(inp)[1];
                    out = {{ {(n + m)/2, (n - m)/2} }};
                }
                    break;
                default:
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

