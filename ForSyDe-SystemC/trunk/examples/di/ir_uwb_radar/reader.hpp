/**********************************************************************
    * reader.hpp -- Reads the radar output                            *
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

//~ // Create a global debug object
//~ nvadebug dbg;
//~ 
//~ // Debug reporter. This class might break with ForSyDe rules?
//~ class debug_report : public sink<int>
//~ {
  //~ int chan;
//~ public:
  //~ debug_report(sc_module_name _name) : sink<int>(_name)
  //~ {
    //~ chan = dbg.add_item(this->name());
  //~ }
//~ protected:
  //~ void _func(int inp)
  //~ {
    //~ dbg.channels[chan].Add(inp);
  //~ }
    //~ 
//~ };


SC_MODULE(reader)
{
    std::vector<SY::in_port<int>>   iports;

    // Vector of debug reporters
    std::vector<SY::ssink<int>*>       m_rpt_vec;

    reader(sc_core::sc_module_name,int N) : iports(N)
    {
      // Create reporter modules
      for(int i=0;i<N;i++){
        std::stringstream name;
        name << "report" << i;
        m_rpt_vec.push_back(
            SY::make_ssink( name.str().c_str(), report_func, iports[i])
        );
      }
    }
    
    static void report_func(const int& inp)
    {
        std::cout << inp << std::endl;
    }
    
};

