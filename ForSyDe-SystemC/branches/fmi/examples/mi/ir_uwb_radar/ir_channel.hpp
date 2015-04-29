/**********************************************************************
    * ir_channel.hpp -- the IR UWB channel                            *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          Based on a model developed by Kody Lee (Novelda AS)    *
    *                                                                 *
    * Purpose: Demonstration of a single cyber-physical system        *
    *                                                                 *
    * Usage:   IR UWB radar transceiver example                       *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

// This module generates backscattering signals with shift and attenuation.

#include <forsyde.hpp>

using namespace sc_core;
using namespace ForSyDe;

// Define the delay and attentuation paremeters of the channel	
int no_of_bs = 5;
std::vector<sc_time> ch_shift = {sc_time(500.0e-12,SC_SEC), 
                                 sc_time(600.0e-12,SC_SEC),
                                 sc_time(700.0e-12,SC_SEC),
                                 sc_time(800.0e-12,SC_SEC),
                                 sc_time(900.0e-12,SC_SEC)};
std::vector<CTTYPE> ch_attn = {0.5, -0.3, 0.18, -0.2, 0.1};

SC_MODULE(ir_channel)
{
    //----Input(s) and output(s)----
    CT::in_port	    in;
    CT::out_port    out;
    //------------------------------

    //----Degine the modules----
    std::vector<CT::shift*>	shift_vec;
    std::vector<CT::scale*>	scale_vec;
    std::vector<CT::add*>	add_vec;
    //---------------------------------------

    //----Defining nodes----
    std::vector<CT::signal> shift_out_vec;
    std::vector<CT::signal> scale_out_vec;
    std::vector<CT::signal> add_out_vec;
    std::vector<CT::signal> from_input_vec;
    //---------------------------------------

    ir_channel(sc_module_name _name, 
            std::vector<sc_time> ch_shift,
            std::vector<double> ch_attn,
            int no_of_bs	
        ) : sc_module(_name),
            shift_out_vec(no_of_bs),
            scale_out_vec(no_of_bs),
            add_out_vec(no_of_bs-2),
            from_input_vec(no_of_bs)
    {

        //----Generating modules dynamically----

		for(int i=0;i<(no_of_bs);i++){
			std::stringstream name;
			name << "shift" << i;
			shift_vec.push_back(
                new CT::shift(name.str().c_str(), ch_shift[i])
            );
		}
		
		for(int i=0;i<(no_of_bs);i++){
			std::stringstream name;
			name << "scale" << i;
			scale_vec.push_back(
                new CT::scale(name.str().c_str(), ch_attn[i])
            );
		}
		
        for(int i=0;i<(no_of_bs-1);i++){
			std::stringstream name;
			name << "add" << i;
			add_vec.push_back(
                new CT::add(name.str().c_str())
            );
		}
        
		auto scale123 = new CT::scale("scale123", 1.0);
        //---------------------------------------

        //----Interconnections----

		scale123->iport1(in);
	
		for(int i=0;i<no_of_bs;i++){
			scale123->oport1(from_input_vec[i]);
			shift_vec[i]->iport1(from_input_vec[i]);
			shift_vec[i]->oport1(shift_out_vec[i]);
			scale_vec[i]->iport1(shift_out_vec[i]);
			scale_vec[i]->oport1(scale_out_vec[i]);
		}

		add_vec[0]->iport1(scale_out_vec[0]);
		add_vec[0]->iport2(scale_out_vec[1]);
		for(int i=1;i<(no_of_bs-1);i++){
			add_vec[i-1]->oport1(add_out_vec[i-1]);
			add_vec[i]->iport1(scale_out_vec[i+1]);
			add_vec[i]->iport2(add_out_vec[i-1]);
		}
		add_vec[no_of_bs-2]->oport1(out);
	}
    //---------------------------------------

	~ir_channel()
	{
		for(unsigned int i=0;i<shift_vec.size();i++){
			delete shift_vec[i];
		}
		for(unsigned int i=0;i<scale_vec.size();i++){
	        	delete scale_vec[i];
		}
		for(unsigned int i=0;i<add_vec.size();i++){
			delete add_vec[i];
		}
	}
};
