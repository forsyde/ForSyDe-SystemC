#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <forsyde.hpp>

typedef std::function <std::vector<int>(const std::vector<int>&)> scenrio_func;

///!< gamma function for the zipU process of the top module
size_t gamma_func_zipa (const size_t &ca)
{   
    return ca;
}

size_t gamma_func_zipb (const size_t &ca)
{   
    return 1;
}

///<! Mealy Detector process 
void gamma_detector_func(unsigned int& tokens, const int& state)
{
    tokens = 1;
}


void next_state_detector_func(int& next_state, const int& cur_state, const std::vector<int>& inp)
{

    if (cur_state == 0)
        next_state = 1;
    else 
        next_state = 0;

}

void output_decode_detector_func(
    std::vector<
        std::tuple<
            size_t, size_t, scenrio_func            
        >
    >& out, const int& cur_state, const std::vector<int>& inp
)
{
    out.resize(1);
    if (cur_state == 0)
    {
        out[0] = std::make_tuple(3, 1, [=] (const std::vector<int>& inp){
            std::vector<int> out(1);
            out[0] =inp[0] + inp[1] + inp[2];
            return out; 
        });
    }
    else 
    {
        out[0] = std::make_tuple(2, 1, [=] (const std::vector<int>& inp){
            std::vector<int> out(1);
            out[0] =inp[1]-inp[0];
            return out; 
        });
    }

}

///<! Kernels Function
void kernel_func(std::vector<int>& out, const std::vector<std::tuple<std::vector<int>,std::vector<std::tuple<size_t, size_t, scenrio_func>>>>& inp)
{
    out = std::get<2>(std::get<1>(inp[0])[0])(std::get<0>(inp[0]));
}


#endif
