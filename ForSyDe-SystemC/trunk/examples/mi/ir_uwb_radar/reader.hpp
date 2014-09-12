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

namespace ForSyDe {
namespace SY {
//! Process constructor for a file_sink_last process
/*! This class is used to build a file_sink_last process which only has
 * an input.
 * Its main purpose is to be used in test-benches. The process passes
 * its last input to a given function to generate a string and write the
 * string to a new line of an output file.
 */
template <class T>
class file_sink_last : public sy_process
{
public:
    SY_in<T> iport1;         ///< port for the input channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(std::string&, const abst_ext<T>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    file_sink_last(sc_module_name _name, ///< process name
         functype _func,            ///< function to be passed
         std::string file_name      ///< the file name
        ) : sy_process(_name), iport1("iport1"), file_name(file_name),
            _func(_func)
            
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
        arg_vec.push_back(std::make_tuple("file_name", file_name));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::file_sink_last";}
    
private:
    std::string file_name;
    
    std::string ostr;        // The current string to be written to the output
    std::ofstream ofs;
    abst_ext<T>* cur_val;         // The current state of the process

    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        cur_val = new abst_ext<T>;
    }
    
    void prep()
    {
        *cur_val = iport1.read();
    }
    
    void exec() {}
    
    void prod() {}
    
    void clean()
    {
        ofs.open(file_name);
        if (!ofs.is_open())
        {
            SC_REPORT_ERROR(name(),"cannot open the file.");
        }
        _func(ostr, *cur_val);
        ofs << ostr << std::endl;
        ofs.close();
        delete cur_val;
    }
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);    // only one output port
        boundInChans[0].port = &iport1;
    }
#endif
};

//! Helper function to construct a file_sink_last process
/*! This function is used to construct a file_sink_last (SystemC module)
 * and connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input FIFOs.
 */
template <class T, template <class> class IIf>
inline file_sink_last<T>* make_file_sink_last(std::string pName,
    typename file_sink_last<T>::functype _func,
    std::string file_name,
    IIf<T>& inS
    )
{
    auto p = new file_sink_last<T>(pName.c_str(), _func, file_name);
    
    (*p).iport1(inS);
    
    return p;
}

}
}

SC_MODULE(reader)
{
    std::vector<SY::in_port<int>>   iports;

    //~ // Vector of debug reporters
    //~ std::vector<SY::ssink<int>*>       m_rpt_vec;

    reader(sc_module_name,int N) : iports(N)
    {
        auto zipped_inp = new SY::signal<std::array<int,NTAPS>>;
        
        auto zipx1 = SY::make_szipX("zipx1", *zipped_inp);
        for(int i=0;i<N;i++) zipx1->iport[i](iports[i]);
        
        SY::make_file_sink_last("report", report_func, "results.txt", *zipped_inp);
        
      //~ // Create reporter modules
      //~ for(int i=0;i<N;i++){
        //~ std::stringstream name;
        //~ name << "report" << i;
        //~ m_rpt_vec.push_back(
            //~ SY::make_ssink( name.str().c_str(), report_func, iports[i])
        //~ );
      //~ }
    }
    
    //~ static void report_func(const int& inp)
    //~ {
        //~ std::cout << inp << std::endl;
    //~ }
    
    static void report_func(std::string& out, const abst_ext<std::array<int,NTAPS>>& inp)
    {
        auto sinp = unsafe_from_abst_ext(inp);
        std::stringstream str;
        str << "plot '-' with lines" << std::endl;
        for (auto it=sinp.rbegin();it!=sinp.rend();++it) str << *it << std::endl;
        out = str.str();
    }
    
};

