/**********************************************************************           
    * de_process_constructors.hpp -- Process constructors in the DE   *
    *                                MOC                              *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing basic process constructors for modeling      *
    *          discrete-event systems in ForSyDe-SystemC              *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DE_PROCESS_CONSTRUCTORS_HPP
#define DE_PROCESS_CONSTRUCTORS_HPP

/*! \file de_process_constructors.hpp
 * \brief Implements the basic process constructors in the DE MoC
 * 
 *  This file includes the basic process constructors used for modeling
 * in the discrete-event model of computation.
 */

#include <functional>
#include <tuple>

#include "tt_event.hpp"
#include "de_process.hpp"

namespace ForSyDe
{

namespace DE
{

using namespace sc_core;

// Auxilliary Macro definitions
#define WRITE_MULTIPORT(PORT,VAL) \
    for (int port_index=0;port_index<PORT.size();port_index++) \
        PORT[port_index]->write(VAL);

//! Process constructor for a combinational process with one input and one output
/*! This class is used to build combinational processes with one input
 * and one output. The class is parameterized for input and output
 * data-types.
 */
template <typename T0, typename T1>
class comb : public de_process
{
public:
    DE_in<T1>  iport1;       ///< port for the input channel
    DE_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(T0&, const T1&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    comb(sc_module_name _name,      ///< process name
         functype _func             ///< function to be passed
         ) : de_process(_name), iport1("iport1"), oport1("oport1"),
             _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DE::comb";}

private:
    // Inputs and output variables
    T0* oval;
    tt_event<T1>* ival1;
    
    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        oval = new T0;
        ival1 = new tt_event<T1>;
    }
    
    void prep()
    {
        *ival1 = iport1.read();
    }
    
    void exec()
    {
        _func(*oval, get_value(*ival1));
    }
    
    void prod()
    {
        auto oev = tt_event<T0>(*oval, get_time(*ival1));
        wait(sc_time_stamp() - get_time(oev));
        WRITE_MULTIPORT(oport1, oev)
    }
    
    void clean()
    {
        delete ival1;
        delete oval;
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

//~ //! Process constructor for a combinational process with two inputs and one output
//~ /*! similar to comb with two inputs
 //~ */
//~ template <typename T0, typename T1, typename T2>
//~ class comb2 : public de_process
//~ {
//~ public:
    //~ DE_in<T1> iport1;        ///< port for the input channel 1
    //~ DE_in<T2> iport2;        ///< port for the input channel 2
    //~ DE_out<T0> oport1;        ///< port for the output channel
    //~ 
    //~ //! Type of the function to be passed to the process constructor
    //~ typedef std::function<void(T0&, const T1&, const T2&)> functype;
//~ 
    //~ //! The constructor requires the module name
    //~ /*! It creates an SC_THREAD which reads data from its input ports,
     //~ * applies the user-imlpemented function to them and writes the
     //~ * results using the output port
     //~ */
    //~ comb2(sc_module_name _name,      ///< process name
          //~ functype _func             ///< function to be passed
          //~ ) : de_process(_name), iport1("iport1"), iport2("iport2"), oport1("oport1"),
              //~ _func(_func)
    //~ {
//~ #ifdef FORSYDE_INTROSPECTION
        //~ std::string func_name = std::string(basename());
        //~ func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        //~ arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
//~ #endif
    //~ }
    //~ 
    //~ //! Specifying from which process constructor is the module built
    //~ std::string forsyde_kind() const {return "DE::comb2";}
//~ private:
    //~ // Inputs and output variables
    //~ T0* oval;
    //~ tt_event<T1>* ival1;
    //~ tt_event<T2>* ival2;
    //~ 
    //~ //! The function passed to the process constructor
    //~ functype _func;
//~ 
    //~ //Implementing the abstract semantics
    //~ void init()
    //~ {
        //~ oval = new T0;
        //~ ival1 = new tt_event<T1>;
        //~ ival2 = new tt_event<T2>;
    //~ }
    //~ 
    //~ void prep()
    //~ {
        //~ *ival1 = iport1.read();
        //~ *ival2 = iport2.read();
    //~ }
    //~ 
    //~ void exec()
    //~ {
        //~ _func(*oval, get_value(*ival1), get_value(*ival2));
    //~ }
    //~ 
    //~ void prod()
    //~ {
        //~ auto oev = tt_event<T0>(*oval, get_time(*ival1));
        //~ WRITE_MULTIPORT(oport1, *oval)
    //~ }
    //~ 
    //~ void clean()
    //~ {
        //~ delete ival2;
        //~ delete ival1;
        //~ delete oval;
    //~ }
    //~ 
//~ #ifdef FORSYDE_INTROSPECTION
    //~ void bindInfo()
    //~ {
        //~ boundInChans.resize(2);     // only one input port
        //~ boundInChans[0].port = &iport1;
        //~ boundInChans[0].portType = typeid(T1).name();
        //~ boundInChans[1].port = &iport2;
        //~ boundInChans[1].portType = typeid(T2).name();
        //~ boundOutChans.resize(1);    // only one output port
        //~ boundOutChans[0].port = &oport1;
        //~ boundOutChans[0].portType = typeid(T0).name();
    //~ }
//~ #endif
//~ };
//~ 
//~ //! Process constructor for a combinational process with three inputs and one output
//~ /*! similar to comb with three inputs
 //~ */
//~ template <typename T0, typename T1, typename T2, typename T3>
//~ class comb3 : public de_process
//~ {
//~ public:
    //~ DE_in<T1> iport1;        ///< port for the input channel 1
    //~ DE_in<T2> iport2;        ///< port for the input channel 2
    //~ DE_in<T3> iport3;        ///< port for the input channel 3
    //~ DE_out<T0> oport1;        ///< port for the output channel
    //~ 
    //~ //! Type of the function to be passed to the process constructor
    //~ typedef std::function<void(tt_event<T0>&, const tt_event<T1>&,
                                              //~ const tt_event<T2>&,
                                              //~ const tt_event<T3>&)> functype;
//~ 
    //~ //! The constructor requires the module name
    //~ /*! It creates an SC_THREAD which reads data from its input ports,
     //~ * applies the user-imlpemented function to them and writes the
     //~ * results using the output port
     //~ */
    //~ comb3(sc_module_name _name,      ///< process name
          //~ functype _func             ///< function to be passed
          //~ ) : de_process(_name), iport1("iport1"), iport2("iport2"), iport3("iport3"), 
              //~ oport1("oport1"), _func(_func)
    //~ {
//~ #ifdef FORSYDE_INTROSPECTION
        //~ std::string func_name = std::string(basename());
        //~ func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        //~ arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
//~ #endif
    //~ }
    //~ 
    //~ //! Specifying from which process constructor is the module built
    //~ std::string forsyde_kind() const {return "DE::comb3";}
    //~ 
//~ private:
    //~ // Inputs and output variables
    //~ tt_event<T0>* oval;
    //~ tt_event<T1>* ival1;
    //~ tt_event<T2>* ival2;
    //~ tt_event<T3>* ival3;
//~ 
    //~ //! The function passed to the process constructor
    //~ functype _func;
    //~ 
    //~ //Implementing the abstract semantics
    //~ void init()
    //~ {
        //~ oval = new tt_event<T0>;
        //~ ival1 = new tt_event<T1>;
        //~ ival2 = new tt_event<T2>;
        //~ ival3 = new tt_event<T3>;
    //~ }
    //~ 
    //~ void prep()
    //~ {
        //~ *ival1 = iport1.read();
        //~ *ival2 = iport2.read();
        //~ *ival3 = iport3.read();
    //~ }
    //~ 
    //~ void exec()
    //~ {
        //~ _func(*oval, *ival1, *ival2, *ival3);
    //~ }
    //~ 
    //~ void prod()
    //~ {
        //~ WRITE_MULTIPORT(oport1, *oval)
    //~ }
    //~ 
    //~ void clean()
    //~ {
        //~ delete ival3;
        //~ delete ival2;
        //~ delete ival1;
        //~ delete oval;
    //~ }
    //~ 
//~ #ifdef FORSYDE_INTROSPECTION
    //~ void bindInfo()
    //~ {
        //~ boundInChans.resize(3);     // only one input port
        //~ boundInChans[0].port = &iport1;
        //~ boundInChans[0].portType = typeid(T1).name();
        //~ boundInChans[1].port = &iport2;
        //~ boundInChans[1].portType = typeid(T2).name();
        //~ boundInChans[2].port = &iport3;
        //~ boundInChans[2].portType = typeid(T3).name();
        //~ boundOutChans.resize(1);    // only one output port
        //~ boundOutChans[0].port = &oport1;
        //~ boundOutChans[0].portType = typeid(T0).name();
    //~ }
//~ #endif
//~ };
//~ 
//~ //! Process constructor for a combinational process with four inputs and one output
//~ /*! similar to comb with four inputs
 //~ */
//~ template <typename T0, typename T1, typename T2, typename T3, typename T4>
//~ class comb4 : public de_process
//~ {
//~ public:
    //~ DE_in<T1> iport1;       ///< port for the input channel 1
    //~ DE_in<T2> iport2;       ///< port for the input channel 2
    //~ DE_in<T3> iport3;       ///< port for the input channel 3
    //~ DE_in<T4> iport4;       ///< port for the input channel 4
    //~ DE_out<T0> oport1;        ///< port for the output channel
    //~ 
    //~ //! Type of the function to be passed to the process constructor
    //~ typedef std::function<void(tt_event<T0>&, const tt_event<T1>&,
                                             //~ const tt_event<T2>&,
                                             //~ const tt_event<T3>&,
                                             //~ const tt_event<T4>&)> functype;
//~ 
    //~ //! The constructor requires the module name
    //~ /*! It creates an SC_THREAD which reads data from its input ports,
     //~ * applies the user-imlpemented function to them and writes the
     //~ * results using the output port
     //~ */
    //~ comb4(sc_module_name _name,      ///< process name
          //~ functype _func             ///< function to be passed
          //~ ) : de_process(_name), iport1("iport1"), iport2("iport2"), 
              //~ iport3("iport3"), iport4("iport4"), _func(_func)
    //~ {
//~ #ifdef FORSYDE_INTROSPECTION
        //~ std::string func_name = std::string(basename());
        //~ func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        //~ arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
//~ #endif
    //~ }
    //~ 
    //~ //! Specifying from which process constructor is the module built
    //~ std::string forsyde_kind() const{return "DE::comb4";}
    //~ 
//~ private:
    //~ // Inputs and output variables
    //~ tt_event<T0>* oval;
    //~ tt_event<T1>* ival1;
    //~ tt_event<T2>* ival2;
    //~ tt_event<T3>* ival3;
    //~ tt_event<T4>* ival4;
    //~ 
    //~ //! The function passed to the process constructor
    //~ functype _func;
//~ 
    //~ //Implementing the abstract semantics
    //~ void init()
    //~ {
        //~ oval = new tt_event<T0>;
        //~ ival1 = new tt_event<T1>;
        //~ ival2 = new tt_event<T2>;
        //~ ival3 = new tt_event<T3>;
        //~ ival4 = new tt_event<T4>;
    //~ }
    //~ 
    //~ void prep()
    //~ {
        //~ *ival1 = iport1.read();
        //~ *ival2 = iport2.read();
        //~ *ival3 = iport3.read();
        //~ *ival4 = iport4.read();
    //~ }
    //~ 
    //~ void exec()
    //~ {
        //~ _func(*oval, *ival1, *ival2, *ival3, *ival4);
    //~ }
    //~ 
    //~ void prod()
    //~ {
        //~ WRITE_MULTIPORT(oport1, *oval)
    //~ }
    //~ 
    //~ void clean()
    //~ {
        //~ delete ival4;
        //~ delete ival3;
        //~ delete ival2;
        //~ delete ival1;
        //~ delete oval;
    //~ }
    //~ 
//~ #ifdef FORSYDE_INTROSPECTION
    //~ void bindInfo()
    //~ {
        //~ boundInChans.resize(4);     // only one input port
        //~ boundInChans[0].port = &iport1;
        //~ boundInChans[0].portType = typeid(T1).name();
        //~ boundInChans[1].port = &iport2;
        //~ boundInChans[1].portType = typeid(T2).name();
        //~ boundInChans[2].port = &iport3;
        //~ boundInChans[2].portType = typeid(T3).name();
        //~ boundInChans[3].port = &iport4;
        //~ boundInChans[3].portType = typeid(T4).name();
        //~ boundOutChans.resize(1);    // only one output port
        //~ boundOutChans[0].port = &oport1;
        //~ boundOutChans[0].portType = typeid(T0).name();
    //~ }
//~ #endif
//~ };

//! Process constructor for a delay element
/*! This class is used to build a delay element. Given an initial value
 * and a delay time, it inserts this value as the first event in time
 * zero in the output and delays the rest of the events by the delay
 * time.
 * 
 * It is mandatory to include at least one delay element in all feedback
 * loops since combinational loops are forbidden in ForSyDe.
 */
template <class T>
class delay : public de_process
{
public:
    DE_in<T>  iport1;       ///< port for the input channel
    DE_out<T> oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial element, reads
     * data from its input port, and writes the results using the output
     * port.
     */
    delay(sc_module_name _name,     ///< process name
          T init_val,                ///< initial value
          sc_time delay_time         ///< delay time
          ) : de_process(_name), iport1("iport1"), oport1("oport1"),
              init_val(init_val), delay_time(delay_time)
    {
#ifdef FORSYDE_INTROSPECTION
        arg_vec.push_back(std::make_tuple("init_val", std::to_string(init_val)));
        arg_vec.push_back(std::make_tuple("delay_time", std::to_string(delay_time.to_double())));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DE::delay";}
    
private:
    // Initial value and the delay time
    T init_val;
    sc_time delay_time;
    
    // Inputs and output variables
    tt_event<T>* val;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new tt_event<T>;
        auto oev = tt_event<T>(init_val, sc_time(0,SC_SEC));
        WRITE_MULTIPORT(oport1, oev)
    }
    
    void prep()
    {
        *val = iport1.read();
    }
    
    void exec()
    {
        set_time(*val, get_time(*val)+delay_time);
        wait(get_time(*val) - sc_time_stamp());
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *val)
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

//! Process constructor for a constant source process
/*! This class is used to build a source process with a constant output
 * and a given period.
 * Its main purpose is to be used in test-benches.
 * 
 * This class can directly be instantiated to build a process.
 */
template <class T>
class constant : public de_process
{
public:
    DE_out<T> oport1;            ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    constant(sc_module_name _name,      ///< The module name
              T init_val,                ///< The constant output value
              sc_time interval,          ///< The interval between the events
              unsigned long long take=0///< number of tokens produced (0 for infinite)
             ) : de_process(_name), oport1("oport1"),
                 init_val(init_val), interval(interval), take(take)
                 
    {
#ifdef FORSYDE_INTROSPECTION
        arg_vec.push_back(std::make_tuple("init_val", std::to_string(init_val)));
        arg_vec.push_back(std::make_tuple("interval", std::to_string(interval.to_double())));
        arg_vec.push_back(std::make_tuple("take", std::to_string(take)));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DE::constant";}
    
private:
    T init_val;
    sc_time interval, next_time;
    unsigned long long take;    // Number of tokens produced
    
    unsigned long long tok_cnt;
    bool infinite;
    
    //Implementing the abstract semantics
    void init()
    {
        if (take==0) infinite = true;
        tok_cnt = 0;
        next_time = sc_time(0, SC_NS);
    }
    
    void prep() {}
    
    void exec()
    {
        if (tok_cnt != 0)
            wait(next_time - sc_time_stamp());
        next_time += interval;
    }
    
    void prod()
    {
        if (tok_cnt++ < take || infinite)
        {
            WRITE_MULTIPORT(oport1, tt_event<T>(init_val, sc_time_stamp()))
        }
        else wait();
    }
    
    void clean() {}

#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
        boundOutChans[0].portType = typeid(T).name();
    }
#endif
};

//! Process constructor for a source process
/*! This class is used to build a souce process which only has an output.
 * Given an initial state and a function, the process repeatedly applies
 * the function to the current state to produce next state, which is
 * also the process output. It can be used in test-benches.
 */
template <class T>
class source : public de_process
{
public:
    DE_out<T> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(tt_event<T>&, const tt_event<T>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    source(sc_module_name _name,   ///< The module name
           functype _func,         ///< function to be passed
           tt_event<T> init_val,    ///< Initial state
           unsigned long long take=0 ///< number of tokens produced (0 for infinite)
          ) : de_process(_name), oport1("oport1"),
              init_st(init_val), take(take), _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
        ss.str("");
        ss << take;
        arg_vec.push_back(std::make_tuple("take", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DE::source";}
    
private:
    tt_event<T> init_st;        // The current state
    unsigned long long take;    // Number of tokens produced
    
    tt_event<T>* cur_st;        // The current state of the process
    unsigned long long tok_cnt;
    bool infinite;
    
    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        cur_st = new tt_event<T>;
        *cur_st = init_st;
        WRITE_MULTIPORT(oport1, *cur_st)
        if (take==0) infinite = true;
        tok_cnt = 1;
    }
    
    void prep() {}
    
    void exec()
    {
        _func(*cur_st, *cur_st);
    }
    
    void prod()
    {
        if (tok_cnt++ < take || infinite)
            WRITE_MULTIPORT(oport1, *cur_st)
        else wait();
    }
    
    void clean()
    {
        delete cur_st;
    }
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
        boundOutChans[0].portType = typeid(T).name();
    }
#endif
};

//! Process constructor for a source process with vector input
/*! This class is used to build a souce process which only has an output.
 * Given the test bench vector, the process iterates over the emenets
 * of the vector and outputs one value on each evaluation cycle.
 */
template <class OTYP>
class vsource : public sc_module
{
public:
    sc_fifo_out<OTYP> oport1;     ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which writes the result using the output
     * port.
     */
    vsource(sc_module_name _name,           ///< The module name
            const std::vector<OTYP>& invec  ///< Initial vector
            )
         :sc_module(_name), in_vec(invec)
    {
        SC_THREAD(worker);
    }
private:
    std::vector<OTYP> in_vec;
    SC_HAS_PROCESS(vsource);

    //! The main and only execution thread of the module
    void worker()
    {
        typename std::vector<OTYP>::iterator itr;
        for (itr=in_vec.begin();itr!=in_vec.end();itr++)
        {
            OTYP out_val = *itr;
            WRITE_MULTIPORT(oport1,out_val)    // write to the output
        }
    }
};

//! Process constructor for a sink process
/*! This class is used to build a sink process which only has an input.
 * Its main purpose is to be used in test-benches. The process repeatedly
 * applies a given function to the current input.
 */
template <class T>
class sink : public de_process
{
public:
    DE_in<T> iport1;         ///< port for the input channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(const tt_event<T>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    sink(sc_module_name _name,      ///< process name
         functype _func             ///< function to be passed
        ) : de_process(_name), iport1("iport1"), _func(_func)
            
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DE::sink";}
    
private:
    tt_event<T>* val;         // The current state of the process

    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new tt_event<T>;
    }
    
    void prep()
    {
        *val = iport1.read();
    }
    
    void exec()
    {
        _func(*val);
    }
    
    void prod() {}
    
    void clean()
    {
        delete val;
    }
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);    // only one output port
        boundInChans[0].port = &iport1;
    }
#endif
};

//! Process constructor for a multi-input print process
/*! This class is used to build a sink process which has a multi-port input.
 * Its main purpose is to be used in test-benches.
 * 
 * The resulting process prints the sampled data as a trace in the
 * standard output.
 */
template <class ITYP>
class printSigs : public sc_module
{
public:
    sc_fifo_in<ITYP> iport;         ///< multi-port for the input channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    printSigs(sc_module_name _name     ///< Process name
              ):sc_module(_name)
    {
        SC_THREAD(worker);
    }

private:
    SC_HAS_PROCESS(printSigs);

    //! The main and only execution thread of the module
    void worker()
    {
        // write the header
        for (int i=0;i<iport.size();i++)
            std::cout << " " << name() << "(" << i << ")";
        std::cout << std::endl;
        // start reading from the ports
        ITYP in_val[iport.size()];
        while (1)
        {
            for (int i=0;i<iport.size();i++)
                in_val[i] = iport[i]->read();
            // print one line
            for (int i=0;i<iport.size();i++)
                std::cout << " " << in_val[i];
            std::cout << std::endl;
        }
    }
};

//! The zip process with two inputs and one output
/*! This process "zips" two incoming signals into one signal of tuples.
 */
template <class T1, class T2>
class zip : public de_process
{
public:
    DE_in<T1> iport1;        ///< port for the input channel 1
    DE_in<T2> iport2;        ///< port for the input channel 2
    DE_out<std::tuple<tt_event<T1>,tt_event<T2>>> oport1;///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    zip(sc_module_name _name)
         :de_process(_name), iport1("iport1"), iport2("iport2"), oport1("oport1")
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DE::zip";}
    
private:
    // intermediate values
    tt_event<T1>* ival1;
    tt_event<T1>* ival2;
    
    void init()
    {
        ival1 = new tt_event<T1>;
        ival2 = new tt_event<T2>;
    }
    
    void prep()
    {
        *ival1 = iport1.read();
        *ival2 = iport2.read();
    }
    
    void exec() {}
    
    void prod()
    {
        if (ival1->is_absent() && ival2->is_absent())
        {
            typedef std::tuple<tt_event<T1>,tt_event<T2>> TT;
            WRITE_MULTIPORT(oport1,tt_event<TT>())  // write to the output 1
        }
        else
        {
            WRITE_MULTIPORT(oport1,std::make_tuple(ival1,ival2))  // write to the output
        }
    }
    
    void clean()
    {
        delete ival1;
        delete ival2;
    }
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(2);     // two input ports
        boundInChans[0].port = &iport1;
        boundInChans[0].portType = typeid(T1).name();
        boundInChans[1].port = &iport2;
        boundInChans[1].portType = typeid(T2).name();
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
        boundOutChans[0].portType = typeid(std::tuple<T1,T2>).name();
    }
#endif
};

//! The zip process with variable number of inputs and one output
/*! This process "zips" the incoming signals into one signal of tuples.
 */
template <class... ITYPs>
class zipN : public sc_module
{
public:
    std::tuple <sc_fifo_in<ITYPs>...> iport;///< tuple of ports for the input channels
    sc_fifo_out<std::tuple<ITYPs...> > oport1;///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    zipN(sc_module_name _name)
         :sc_module(_name)
    {
        SC_THREAD(worker);
    }
private:
    SC_HAS_PROCESS(zipN);

    //! The main and only execution thread of the module
    void worker()
    {
        std::tuple<ITYPs...> in_vals;
        while (1)
        {
            in_vals = sc_fifo_tuple_read<ITYPs...>(iport);
            WRITE_MULTIPORT(oport1,in_vals)    // write to the output
        }
    }
    
    template<size_t N,class R,  class T>
    struct fifo_read_helper
    {
        static void read(R& ret, T& t)
        {
            fifo_read_helper<N-1,R,T>::read(ret,t);
            std::get<N>(ret) = std::get<N>(t).read();
        }
    };

    template<class R, class T>
    struct fifo_read_helper<0,R,T>
    {
        static void read(R& ret, T& t)
        {
            std::get<0>(ret) = std::get<0>(t).read();
        }
    };

    template<class... T>
    std::tuple<T...> sc_fifo_tuple_read(std::tuple<sc_fifo_in<T>...>& ports)
    {
        std::tuple<T...> ret;
        fifo_read_helper<sizeof...(T)-1,
                         std::tuple<T...>,
                         std::tuple<sc_fifo_in<T>...>>::read(ret,ports);
        return ret;
    }

};

//! The unzip process with one input and two outputs
/*! This process "unzips" a signal of tuples into two separate signals
 */
template <class T1, class T2>
class unzip : public de_process
{
public:
    DE_in<std::tuple<tt_event<T1>,tt_event<T2>>> iport1;///< port for the input channel
    DE_out<T1> oport1;        ///< port for the output channel 1
    DE_out<T2> oport2;        ///< port for the output channel 2

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * unzips them and writes the results using the output ports
     */
    unzip(sc_module_name _name)
         :de_process(_name), iport1("iport1"), oport1("oport1"), oport2("oport2")
    {}
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DE::unzip";}
private:
    // intermediate values
    tt_event<std::tuple<tt_event<T1>,tt_event<T2>>>* in_val;
    
    void init()
    {
        in_val = new tt_event<std::tuple<tt_event<T1>,tt_event<T2>>>;
    }
    
    void prep()
    {
        *in_val = iport1.read();
    }
    
    void exec() {}
    
    void prod()
    {
        if (in_val->is_absent())
        {
            WRITE_MULTIPORT(oport1,tt_event<T1>())  // write to the output 1
            WRITE_MULTIPORT(oport2,tt_event<T2>())  // write to the output 2
        }
        else
        {
            WRITE_MULTIPORT(oport1,std::get<0>(in_val->unsafe_from_tt_event()))  // write to the output 1
            WRITE_MULTIPORT(oport2,std::get<1>(in_val->unsafe_from_tt_event()))  // write to the output 2
        }
    }
    
    void clean()
    {
        delete in_val;
    }
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport1;
        boundInChans[0].portType = typeid(std::tuple<T1,T2>).name();
        boundOutChans.resize(2);    // two output ports
        boundOutChans[0].port = &oport1;
        boundOutChans[0].portType = typeid(T1).name();
        boundOutChans[1].port = &oport2;
        boundOutChans[1].portType = typeid(T2).name();
    }
#endif
};

//! The unzip process with one input and variable number of outputs
/*! This process "unzips" the incoming signal into a tuple of signals.
 */
template <class... Ts>
class unzipN : public de_process
{
public:
    DE_in<std::tuple<tt_event<Ts>...>> iport1;///< port for the input channel
    std::tuple<DE_out<Ts>...> oport;///< tuple of ports for the output channels

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * unzips it and writes the results using the output ports
     */
    unzipN(sc_module_name _name)
          :de_process(_name), iport1("iport1")
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DE::unzipN";}
private:
    // intermediate values
    tt_event<std::tuple<tt_event<Ts>...>>* in_val;
    
    void init()
    {
        in_val = new tt_event<std::tuple<tt_event<Ts>...>>;
    }
    
    void prep()
    {
        *in_val = iport1.read();
    }
    
    void exec() {}
    
    void prod()
    {
        if (in_val->is_absent())
        {
            std::tuple<tt_event<Ts>...> all_abs;
            fifo_tuple_write<Ts...>(all_abs, oport);
        }
        else
        {
            fifo_tuple_write<Ts...>(in_val->unsafe_from_tt_event(), oport);
        }
    }
    
    void clean()
    {
        delete in_val;
    }
    
    template<size_t N,class R,  class T>
    struct fifo_write_helper
    {
        static void write(const R& vals, T& t)
        {
            fifo_write_helper<N-1,R,T>::write(vals,t);
            std::get<N>(t).write(std::get<N>(vals));
        }
    };

    template<class R, class T>
    struct fifo_write_helper<0,R,T>
    {
        static void write(const R& vals, T& t)
        {
            std::get<0>(t).write(std::get<0>(vals));
        }
    };

    template<class... T>
    void fifo_tuple_write(const std::tuple<tt_event<T>...>& vals,
                             std::tuple<DE_out<T>...>& ports)
    {
        fifo_write_helper<sizeof...(T)-1,
                          std::tuple<tt_event<T>...>,
                          std::tuple<DE_out<T>...>>::write(vals,ports);
    }

#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport1;
        boundInChans[0].portType = typeid(int).name();
        boundOutChans.resize(sizeof...(Ts));    // two output ports
        register_ports(boundOutChans, oport);
    }
    
    template<size_t N, class T>
    struct register_ports_helper
    {
        static void reg_port(std::vector<PortInfo>& boundChans, T& t)
        {
            register_ports_helper<N-1,T>::reg_port(boundChans,t);
            boundChans[N].port = &std::get<N>(t);
        }
    };

    template<class T>
    struct register_ports_helper<0,T>
    {
        static void reg_port(std::vector<PortInfo>& boundChans, T& t)
        {
            boundChans[0].port = &std::get<0>(t);
        }
    };

    template<class... T>
    void register_ports(std::vector<PortInfo>& boundChans,
                             std::tuple<DE_out<T>...>& ports)
    {
        register_ports_helper<sizeof...(T)-1,
                              std::tuple<DE_out<T>...>&>::reg_port(boundChans,ports);
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
template <class T>
class fanout : public de_process
{
public:
    DE_in<T> iport1;        ///< port for the input channel
    DE_out<T> oport1;       ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies and writes the results using the output port
     */
    fanout(sc_module_name _name)  // module name
         : de_process(_name) { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DE::fanout";}
    
private:
    // Inputs and output variables
    tt_event<T>* val;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new tt_event<T>;
    }
    
    void prep()
    {
        *val = iport1.read();
    }
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *val)
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
        boundInChans[0].portType = typeid(T).name();
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
        boundOutChans[0].portType = typeid(T).name();
    }
#endif
};

}
}

#endif
