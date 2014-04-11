/**********************************************************************
    * conf.hpp -- the configuration parameters and inputs             *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of the vector operations.                *
    *                                                                 *
    * Usage:   mlpnn example                                          *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef CONF_HPP
#define CONF_HPP

#include "forsyde.hpp"

using namespace std;
using namespace ForSyDe;

#define NN_NUM_INPUTS 2
#define NN_NUM_INNERS 4
#define NN_NUM_OUTPUTS 2

#define V(val) abst_ext<float>(val)

float nn_inner_weights[NN_NUM_INPUTS][NN_NUM_INNERS] = {
	0,0,0,0,
	0,0,0,0
};

float nn_out_weights[NN_NUM_INNERS][NN_NUM_OUTPUTS] = {
	0,0,
	0,0,
	0,0,
	0,0
};

vector<vector<abst_ext<float>>> nn_inputs = {
		{V(0),V(0)},
		{V(0),V(0)}
};

#endif
