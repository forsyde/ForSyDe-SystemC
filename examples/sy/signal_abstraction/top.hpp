#include <forsyde.hpp>

using namespace sc_core;
using namespace ForSyDe;


void report_func(abst_ext<float> inp1)
{
    if (inp1.is_present())
        std::cout << "Input value: " << inp1.unsafe_from_abst_ext() << std::endl;
    else
        std::cout << "Input value: absent" << std::endl;
}

void signalabst_func(abst_ext<float>& out, const unsigned long& take, const std::vector<abst_ext<float>>& inp)
{

 
        float sum = 0;
        for (unsigned long i = 0; i < inp.size(); i++)
        {
            sum += unsafe_from_abst_ext(inp[i]);
        }
        out.set_val(sum/take);
}

SC_MODULE(top)

{   
    SY::signal <float> out_source;
    SY::signal <float> out_signalabst;

    std::vector<abst_ext<float>> s1 = {36.7, 36.8, 36.7, 36.8, 36.9, 36.9, 37.0, 37.0, 37.1, 37.2, 37.3, 37.2, 37.3, 37.3, 37.4, 37.5, 37.6, 36.6};
    std::vector<abst_ext<float>> s2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};


    SC_CTOR(top)
    {   
        std::cout<<s2<<std::endl;
        SY::make_vsource ("source", s2, out_source) ;
        auto abstsig = new SY::signalabst <float,float> ("signalabst", 4, signalabst_func);
        abstsig-> iport1 (out_source);
        abstsig-> oport1(out_signalabst);
        SY::make_sink("report1", report_func, out_signalabst);
    }
};

