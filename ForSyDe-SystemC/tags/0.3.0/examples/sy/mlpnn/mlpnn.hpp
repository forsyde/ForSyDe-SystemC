/**********************************************************************
    * mlpnn.hpp -- A multi-layer perceptron artifivial neural network *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of the vector operations.                *
    *                                                                 *
    * Usage:   mlpnn example                                          *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/
#ifndef MLPNN_HPP
#define MLPNN_HPP

#include "forsyde.hpp"

#include "conf.hpp"
#include "nn_inner_layer.hpp"
#include "nn_output_layer.hpp"

using namespace std;
using namespace ForSyDe::SY;

SC_MODULE(mlpnn)
{
	SY_in<float> iport[NN_NUM_INPUTS];
	SY_out<float> oport[NN_NUM_OUTPUTS];

    SY2SY<float> nn_inner_connections[NN_NUM_INPUTS][NN_NUM_INNERS];
    SY2SY<float> nn_output_connections[NN_NUM_INNERS][NN_NUM_OUTPUTS];
    
    SC_CTOR(mlpnn)
    {
    	fanout<float>* nn_input_layer[NN_NUM_INPUTS];
    	combX<float,float,NN_NUM_INPUTS>* nn_inner_layer[NN_NUM_INNERS];
    	combX<float,float,NN_NUM_INNERS>* nn_output_layer[NN_NUM_OUTPUTS];

    	for (int i=0;i<NN_NUM_INPUTS;i++)
    	{
    		nn_input_layer[i] = make_fanout("nn_input_layer"+std::to_string(i), nn_inner_connections[i][0], iport[i]);
    		for (int j=1;j<NN_NUM_INNERS;j++)
    			nn_input_layer[i]->oport1(nn_inner_connections[i][j]);
    	}

    	for (int i=0;i<NN_NUM_INNERS;i++)
    	{
    		nn_inner_layer[i] = new combX<float,float,NN_NUM_INPUTS>(("nn_inner_layer"+std::to_string(i)).c_str(),nn_inner_layer_func);
    		for (int j=0;j<NN_NUM_INPUTS;j++)
    			nn_inner_layer[i]->iport[j](nn_inner_connections[j][i]);
    		for (int j=0;j<NN_NUM_OUTPUTS;j++)
    			nn_inner_layer[i]->oport1(nn_output_connections[i][j]);
    	}

    	for (int i=0;i<NN_NUM_OUTPUTS;i++)
    	{
    		nn_output_layer[i] = new combX<float,float,NN_NUM_INNERS>(("nn_output_layer"+std::to_string(i)).c_str(),nn_output_layer_func);
    		for (int j=0;j<NN_NUM_INNERS;j++)
    			nn_output_layer[i]->iport[j](nn_output_connections[j][i]);
    		nn_output_layer[i]->oport1(oport[i]);
    	}

    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};

#endif
