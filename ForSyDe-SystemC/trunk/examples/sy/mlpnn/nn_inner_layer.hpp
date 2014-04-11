/**********************************************************************
    * add.hpp -- an adder process                                     *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple sequential process.          *
    *                                                                 *
    * Usage:   MulAcc example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef NN_INNER_LAYER_HPP
#define NN_INNER_LAYER_HPP

#include <forsyde.hpp>

using namespace ForSyDe;
using namespace ForSyDe::SY;

void nn_inner_layer_func(abst_ext<float>& out1,
			const std::array<abst_ext<float>,NN_NUM_INPUTS>& inp)
{
#pragma ForSyDe begin nn_inner_layer_func
	int i;
	int res = 0;
	for (i=0;i<NN_NUM_INPUTS;i++)
		res += from_abst_ext(inp[i],0);
	set_val(out1,res);
#pragma ForSyDe end
}

#endif
