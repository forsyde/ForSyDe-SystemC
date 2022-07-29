/**********************************************************************           
    * ct_process_constructors.hpp -- Process constructors in the CT   *
    *                                MOC                              *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing basic process constructors for modeling      *
    *          continuous-time systems in ForSyDe-SystemC             *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef CT_PROCESS_CONSTRUCTORS_HPP
#define CT_PROCESS_CONSTRUCTORS_HPP

/*! \file ct_process_constructors.hpp
 * \brief Implements the basic process constructors in the CT MoC
 * 
 *  This file includes the basic process constructors used for modeling
 * in the synchronous model of computation.
 */

#include <functional>
#include <fstream>
#include <string>

#include "sub_signal.hpp"
#include "ct_process.hpp"

namespace ForSyDe
{

namespace CT
{

using namespace sc_core;

//! Process constructor for a combinational process with one input and one output
/*! This class is used to build combinational processes with one input
 * and one output. The class is parameterized for input and output
 * data-types.
 */
class comb : public ct_process
{
public:
    CT_in  iport1;       ///< port for the input channel
    CT_out oport1;       ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(CTTYPE&,const CTTYPE&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    comb(sc_module_name _name,      ///< process name
         const functype& _func      ///< function to be passed
         ) : ct_process(_name), iport1("iport1"), oport1("oport1"),
             _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::comb";}

private:
    // Inputs and output variables
    sub_signal oval;
    sub_signal ival1;
    
    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init() {}
    
    void prep()
    {
        ival1 = iport1.read();
    }
    
    void exec()
    {
        sub_signal iv1 = ival1;
        oval = sub_signal(get_start_time(ival1), get_end_time(ival1),
                    [this,iv1](const sc_time& t)
                    {
                        CTTYPE res;
                        _func(res, iv1(t));
                        return res;
                    }
               );
    }
    
    void prod()
    {
        write_multiport(oport1, oval);
        wait(get_end_time(oval) - sc_time_stamp());
    }
    
    void clean(){}
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport1;
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

//! Process constructor for a combinational process with two inputs and one output
/*! similar to comb with two inputs
 */
class comb2 : public ct_process
{
public:
    CT_in iport1;        ///< port for the input channel 1
    CT_in iport2;        ///< port for the input channel 2
    CT_out oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(CTTYPE&, const CTTYPE&,
                                              const CTTYPE&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb2(sc_module_name _name,      ///< process name
          const functype& _func      ///< function to be passed
          ) : ct_process(_name), iport1("iport1"), iport2("iport2"), oport1("oport1"),
              _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::comb2";}
private:    
    // Inputs and output sub-signals
    sub_signal oss;
    sub_signal iss1;
    sub_signal iss2;
    
    // the current time (local time) and the next time
    sc_time tl, tn;
    
    // clocks of the input ports (channel times)
    sc_time in1T, in2T;
    
    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        in1T = in2T = tl = tn = SC_ZERO_TIME;
        set_range(iss1, SC_ZERO_TIME, SC_ZERO_TIME);
        set_range(iss2, SC_ZERO_TIME, SC_ZERO_TIME);
    }
    
    void prep()
    {
        if (in1T == tl)
        {
            iss1 = iport1.read();
            in1T = get_end_time(iss1);
        }
        if (in2T == tl)
        {
            iss2 = iport2.read();
            in2T = get_end_time(iss2);
        }
        
        // update the next local clock
        tn = std::min(in1T, in2T);
    }
    
    void exec()
    {
        sub_signal iv1 = iss1;
        sub_signal iv2 = iss2;
        oss = sub_signal(tl, tn, 
                             [iv1,iv2,this](const sc_time& t)
                             {
                                 CTTYPE res;
                                 _func(res, iv1(t), iv2(t));
                                 return res;
                             }
               );
        tl = tn;
    }
    
    void prod()
    {
        write_multiport(oport1, oss);
        wait(tl - sc_time_stamp());
    }
    
    void clean(){}
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(2);     // only one input port
        boundInChans[0].port = &iport1;
        boundInChans[1].port = &iport2;
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

//! Process constructor for a combinational process with an array of inputs and one output
/*! similar to comb but with an array of inputs
 */
template <std::size_t N>
class combX : public ct_process
{
public:
    std::array<CT_in,N> iport;  ///< port for the input channel array
    CT_out oport1;              ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(CTTYPE&, const std::array<CTTYPE,N>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    combX(sc_module_name _name,      ///< process name
          const functype& _func      ///< function to be passed
          ) : ct_process(_name), oport1("oport1"), _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::combX";}
private:    
    // Inputs and output sub-signals
    sub_signal oss;
    std::array<sub_signal,N> isss;
    
    // the current time (local time) and the next time
    sc_time tl, tn;
    
    // clocks of the input ports (channel times)
    std::array<sc_time,N> insT;
    
    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        tl = tn = SC_ZERO_TIME;
        std::fill(insT.begin(), insT.end(), SC_ZERO_TIME);
        for(sub_signal &iss: isss)
            set_range(iss, SC_ZERO_TIME, SC_ZERO_TIME);
    }
    
    void prep()
    {
        for (size_t i=0;i<N;i++)
            if (insT[i] == tl)
            {
                isss[i] = iport[i].read();
                insT[i] = get_end_time(isss[i]);
            }
        
        // update the next local clock
        tn = *std::min_element(insT.begin(), insT.end());
    }
    
    void exec()
    {
        std::array<sub_signal,N> temp_isss = isss;
        oss = sub_signal(tl, tn, 
                             [temp_isss,this](const sc_time& t)
                             {
                                 CTTYPE res;
                                 std::array<CTTYPE,N> ivs;
                                 for (size_t i=0;i<N;i++)
                                    ivs[i] = temp_isss[i](t);
                                 _func(res, ivs);
                                 return res;
                             }
               );
        tl = tn;
    }
    
    void prod()
    {
        write_multiport(oport1, oss);
        wait(tl - sc_time_stamp());
    }
    
    void clean(){}
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(N);     // N input ports
        for (size_t i=0;i<N;i++)
            boundInChans[i].port = &iport[i];
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

//! Process constructor for a delay element
/*! This class is used to build a process which delays the input CT signal.
 * It operates by adding the specified delay value to the start and end
 * times for ranges of every sub-signal element.
 * 
 * The resulting process does not buffer anything from the signal.
 */
class delay : public ct_process
{
public:
    CT_in  iport1;        ///< port for the input channel
    CT_out oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial element, reads
     * data from its input port, and writes the results using the output
     * port.
     */
    delay(sc_module_name _name,     ///< process name
          sc_time delay_time         ///< delay time
          ) : ct_process(_name), iport1("iport1"), oport1("oport1"),
              delay_time(delay_time)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << delay_time;
        arg_vec.push_back(std::make_tuple("delay_time", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::delay";}
    
private:
    // Initial value
    sc_time delay_time;
    
    // Inputs and output variables
    sub_signal val;
    
    //Implementing the abstract semantics
    void init()
    {
        if (delay_time > SC_ZERO_TIME)
        {
            write_multiport(oport1, 
                sub_signal(SC_ZERO_TIME, delay_time, 
                    [](const sc_time& t){return 0;}
                )
            );
            wait(delay_time);
        }
    }
    
    void prep()
    {
        val = iport1.read();
    }
    
    void exec()
    {
        set_range(val, get_start_time(val)+delay_time,
                       get_end_time(val)+delay_time);
    }
    
    void prod()
    {
        write_multiport(oport1, val);
        wait(get_end_time(val) - sc_time_stamp());
    }
    
    void clean(){}
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport1;
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

//! Process constructor for a shift element
/*! This class is used to build a process which shifts the shape of the
 * input signal by a given value to the right.
 */
class shift : public ct_process
{
public:
    CT_in  iport1;       ///< port for the input channel
    CT_out oport1;       ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial element, reads
     * data from its input port, and writes the results using the output
     * port.
     */
    shift(sc_module_name _name,     ///< process name
          sc_time delay_time         ///< delay time
          ) : ct_process(_name), iport1("iport1"), oport1("oport1"),
              delay_time(delay_time)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << delay_time;
        arg_vec.push_back(std::make_tuple("delay_time", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::shift";}
    
private:
    // Initial value
    sc_time delay_time;
    
    // Inputs and output variables
    sub_signal val;
    
    //Implementing the abstract semantics
    void init()
    {
        if (delay_time > SC_ZERO_TIME)
        {
            write_multiport(oport1, 
                sub_signal(SC_ZERO_TIME, delay_time, 
                    [](const sc_time& t){return 0;}
                )
            );
            wait(delay_time);
        }
    }
    
    void prep()
    {
        val = iport1.read();
    }
    
    void exec()
    {
        set_range(val, get_start_time(val)+delay_time,
                       get_end_time(val)+delay_time);
        sub_signal v = val;
        set_function(val, [v,this](const sc_time& t){
                return get_function(v)(t-delay_time);
            }
        );
    }
    
    void prod()
    {
        write_multiport(oport1, val);
        wait(get_end_time(val) - sc_time_stamp());
    }
    
    void clean() {}
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport1;
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

//! Process constructor for a constant source process
/*! This class is used to build a souce process with constant output.
 * Its main purpose is to be used in test-benches.
 * 
 * This class can directly be instantiated to build a process.
 */
class constant : public ct_process
{
public:
    CT_out oport1;            ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    constant(sc_module_name _name,      ///< The module name
              CTTYPE init_val,          ///< The constant output value
              sc_time end_time           ///< The end time of the signal
             ) : ct_process(_name), oport1("oport1"),
                 init_val(init_val)
                 
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
        ss.str("");
        ss << end_time;
        arg_vec.push_back(std::make_tuple("end_time", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::constant";}
    
private:
    CTTYPE init_val;
    sc_time end_time;
    
    //Implementing the abstract semantics
    void init()
    {
        auto ss = sub_signal(sc_time(0,SC_NS), end_time, 
                        [this](const sc_time& t){return init_val;}
                  );
        write_multiport(oport1, ss);
        wait(get_end_time(ss) - sc_time_stamp());
    }
    
    void prep() {}
    
    void exec()
    {
        wait();
    }
    
    void prod() {}
    
    void clean() {}

#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

//! Process constructor for a source process
/*! This class is used to build a souce process which only has an output.
 * Given a function, the process generates a source process which
 * generates a continuous signal made out of a single SubSignal. The
 * start and end times of the signals should aso be mentioned.
 * 
 */
class source : public ct_process
{
public:
    CT_out oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(CTTYPE&, const sc_time&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    source(sc_module_name _name,   ///< The module name
           functype _func,         ///< function to be passed
           const sc_time& end_time ///< End time
          ) : ct_process(_name), oport1("oport1"),
              _func(_func), end_time(end_time)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
        std::stringstream ss;
        ss << end_time;
        arg_vec.push_back(std::make_tuple("end_time", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::source";}
    
private:
    //! The function passed to the process constructor
    functype _func;
    
    sc_time end_time;        // The end time    
    
    //Implementing the abstract semantics
    void init()
    {
        auto ss = sub_signal(sc_time(0,SC_NS), end_time,
                                [this](const sc_time& t)
                                {
                                    CTTYPE res=0;
                                    _func(res, t);
                                    return res;
                                }
                            );
        write_multiport(oport1, ss);
        wait(get_end_time(ss) - sc_time_stamp());
    }
    
    void prep() {}
    
    void exec()
    {
        wait();
    }
    
    void prod() {}
    
    void clean(){}
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

//! Process constructor for a sink process
/*! This class is used to build a sink process which only has an input.
 * Its main purpose is to be used in test-benches. The process repeatedly
 * applies a given function to the current input using a sampling time.
 */
class sink : public ct_process
{
public:
    CT_in iport1;         ///< port for the input channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(const CTTYPE&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    sink(sc_module_name _name,      ///< process name
         functype _func,           ///< function to be passed
         sc_time sampling_period    ///< the output sampling period
        ) : ct_process(_name), iport1("iport1"), _func(_func),
            sampling_period(sampling_period)
            
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::sink";}
    
private:
    sub_signal val;         // The current read sub_signal
    sc_time cur_time;        // The current time used for sampling

    //! The function passed to the process constructor
    functype _func;
    
    sc_time sampling_period;
    
    //Implementing the abstract semantics
    void init()
    {
        cur_time = sc_time(0, SC_NS);
    }
    
    void prep()
    {
        while(get_end_time(val) <= cur_time)
            val = iport1.read();
    }
    
    void exec()
    {
        _func(val(cur_time));
        cur_time+=sampling_period;
    }
    
    void prod() {}
    
    void clean(){}
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);    // only one output port
        boundInChans[0].port = &iport1;
    }
#endif
};

//! Process constructor for a trace process
/*! This class is used to build a sink process which only has an input.
 * Its main purpose is to be used in test-benches.
 * 
 * The resulting process prints the sampled data as a trace in an output
 * ".dat" file which can be plotted using gaw or gwave.
 */
class traceSig : public ct_process
{
public:
    CT_in  iport1;       ///< port for the input channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    traceSig(sc_module_name _name,          ///< Process name
             const sc_time& sample_period   ///< Sampling time
             ) : ct_process(_name), iport1("iport1"),
                 sample_period(sample_period)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << sample_period;
        arg_vec.push_back(std::make_tuple("sample_period", ss.str()));
#endif        
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::traceSig";}

private:
    sc_time sample_period;
    
    // The internal variables
    std::ofstream outFile;
    sub_signal in_val;
    sc_time curTime;
    
    //Implementing the abstract semantics
    void init()
    {
        outFile.open(name()+std::string(".dat"));
        if (!outFile.is_open())
            SC_REPORT_ERROR(name(),"file could not be opened");
        outFile << "#time " << name() << std::endl;
        in_val = iport1.read();
        curTime = get_start_time(in_val);
    }
    
    void prep()
    {
        while (curTime >= get_end_time(in_val)) in_val = iport1.read();
    }
    
    void exec() {}
    
    void prod()
    {
        outFile << curTime.to_seconds() << " " << in_val(curTime)
                << std::endl;
        curTime += sample_period;
    }
    
    void clean()
    {
        if (outFile.is_open()) outFile.close();
    }
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport1;
    }
#endif
};

//! Process constructor for a fan-out process with one input and one output
/*! This class is used to build a fanout processes with one input
 * and one output. The class is parameterized for input and output
 * data-types.
 * 
 * This class exist because it is impossible to connect channels
 * directly to ports in SystemC (which may be needed in hierarchical
 * designs). It will be used when it is needed to connect an input
 * port of a module to the input channels of multiple processes (modules).
 */
class fanout : public ct_process
{
public:
    CT_in iport1;        ///< port for the input channel
    CT_out oport1;       ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies and writes the results using the output port
     */
    fanout(sc_module_name _name)  // module name
         : ct_process(_name) { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::fanout";}
    
private:
    // Inputs and output variables
    sub_signal* val;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new sub_signal;
    }
    
    void prep()
    {
        *val = iport1.read();
    }
    
    void exec() {}
    
    void prod()
    {
        write_multiport(oport1, *val);
        wait(get_end_time(*val) - sc_time_stamp());
    }
    
    void clean()
    {
        delete val;
    }
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport1;
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

}
}

#endif
