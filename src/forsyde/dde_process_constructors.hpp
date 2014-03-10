/**********************************************************************           
    * dde_process_constructors.hpp -- Process constructors in the DDE *
    *                                MOC                              *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing basic process constructors for modeling      *
    *          distributed discrete-event systems in ForSyDe-SystemC  *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DDE_PROCESS_CONSTRUCTORS_HPP
#define DDE_PROCESS_CONSTRUCTORS_HPP

/*! \file dde_process_constructors.hpp
 * \brief Implements the basic process constructors in the DDE MoC
 * 
 *  This file includes the basic process constructors used for modeling
 * in the distributed discrete-event model of computation.
 */

#include <functional>
#include <algorithm>
#include <tuple>
#include <deque>
#include <boost/numeric/ublas/matrix.hpp>

#include "tt_event.hpp"
#include "dde_process.hpp"

namespace ForSyDe
{

namespace DDE
{

using namespace sc_core;
using namespace boost::numeric::ublas;

//! Process constructor for a combinational process with one input and one output
/*! This class is used to build combinational processes with one input
 * and one output. The class is parameterized for input and output
 * data-types.
 */
template <typename T0, typename T1>
class comb : public dde_process
{
public:
    DDE_in<T1>  iport1;       ///< port for the input channel
    DDE_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(T0&, const T1&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    comb(sc_module_name _name,      ///< process name
         functype _func             ///< function to be passed
         ) : dde_process(_name), iport1("iport1"), oport1("oport1"),
             _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::comb";}

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
        ival1 = new tt_event<T1>();
    }
    
    void prep()
    {
        *ival1 = iport1.read();
    }
    
    void exec()
    {
        if (get_time(*ival1)!=sc_max_time())
            _func(*oval, get_value(*ival1));
    }
    
    void prod()
    {
        auto oev = tt_event<T0>(*oval, get_time(*ival1));
        WRITE_MULTIPORT(oport1, oev)
        if (get_time(*ival1)!=sc_max_time())
            wait(sc_time_stamp() - get_time(oev));
        else
            wait();
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

//! Process constructor for a combinational process with two inputs and one output
/*! similar to comb with two inputs
 */
template <typename T0, typename T1, typename T2>
class comb2 : public dde_process
{
public:
    DDE_in<T1> iport1;        ///< port for the input channel 1
    DDE_in<T2> iport2;        ///< port for the input channel 2
    DDE_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(T0&, const T1&, const T2&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb2(sc_module_name _name,      ///< process name
          functype _func             ///< function to be passed
          ) : dde_process(_name), iport1("iport1"), iport2("iport2"), oport1("oport1"),
              _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::comb2";}
private:
    // Inputs and output variables
    T0* oval;
    tt_event<T1> *next_itok1, *cur_itok1;
    tt_event<T2> *next_itok2, *cur_itok2;
    
    // the current time (local time)
    sc_time tc;
    
    // clocks of the input ports (channel times)
    sc_time in1T, in2T;
    
    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        oval = new T0;
        next_itok1 = new tt_event<T1>;
        next_itok2 = new tt_event<T2>;
        cur_itok1 = new tt_event<T1>();
        cur_itok2 = new tt_event<T2>();
        in1T = in2T = tc = SC_ZERO_TIME;
    }
    
    void prep()
    {
        if (in1T<in2T)
        {
            *next_itok1 = iport1.read();  // read from input 1
            in1T = get_time(*next_itok1);
        }
        else if (in1T>in2T)
        {
            *next_itok2 = iport2.read();  // read from input 1
            in2T = get_time(*next_itok2);
        }
        else
        {
            *next_itok1 = iport1.read();  // read from input 1
            *next_itok2 = iport2.read();  // read from input 2
            in1T = get_time(*next_itok1);
            in2T = get_time(*next_itok2);
        }
        
        // updated channel clocks and the local clock
        if (get_time(*next_itok1) < get_time(*next_itok2))
        {
            *cur_itok1 = *next_itok1;
            tc = get_time(*cur_itok1);
        }
        else if (get_time(*next_itok1) > get_time(*next_itok2))
        {
            *cur_itok2 = *next_itok2;
            tc = get_time(*cur_itok2);
        }
        else
        {
            *cur_itok1 = *next_itok1;
            *cur_itok2 = *next_itok2;
            tc = get_time(*cur_itok1);
        }
    }
    
    void exec()
    {        
        _func(*oval, get_value(*cur_itok1), get_value(*cur_itok2));
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, tt_event<T0>(*oval,tc))
        wait(tc - sc_time_stamp());
    }
    
    void clean()
    {
        delete oval;
        delete next_itok1;
        delete next_itok2;
        delete cur_itok1;
        delete cur_itok2;
    }
    
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
class delay : public dde_process
{
public:
    DDE_in<T>  iport1;       ///< port for the input channel
    DDE_out<T> oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial element, reads
     * data from its input port, and writes the results using the output
     * port.
     */
    delay(sc_module_name _name,     ///< process name
          T init_val,                ///< initial value
          sc_time delay_time         ///< delay time
          ) : dde_process(_name), iport1("iport1"), oport1("oport1"),
              init_val(init_val), delay_time(delay_time)
    {
#ifdef FORSYDE_INTROSPECTION
        arg_vec.push_back(std::make_tuple("init_val", std::to_string(init_val)));
        arg_vec.push_back(std::make_tuple("delay_time", std::to_string(delay_time.to_double())));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::delay";}
    
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
        auto oev = tt_event<T>(init_val, SC_ZERO_TIME);
        WRITE_MULTIPORT(oport1, oev)
        wait(SC_ZERO_TIME);
    }
    
    void prep()
    {
        *val = iport1.read();
    }
    
    void exec()
    {
        set_time(*val, get_time(*val)+delay_time);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *val)
        wait(get_time(*val) - sc_time_stamp());
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

//! Process constructor for a Mealy machine
/*! This class is used to build a finite state machine of type Mealy.
 * Given an initial state, a next-state function, and an output decoding
 * function it creates a Mealy process.
 */
template <typename IT, typename ST, typename OT>
class mealy : public dde_process
{
public:
    DDE_in<IT>  iport1;        ///< port for the input channel
    DDE_out<OT> oport1;        ///< port for the output channel
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(ST&, const ST&, const IT&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(OT&, const ST&, const IT&)> od_functype;
    
    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    mealy(const sc_module_name& _name,      ///< process name
           const ns_functype& _ns_func, ///< The next_state function
           const od_functype& _od_func, ///< The output-decoding function
           const ST& init_st  ///< Initial state
          ) : dde_process(_name), _ns_func(_ns_func), _od_func(_od_func),
              init_st(init_st)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_ns_func",func_name+std::string("_ns_func")));
        arg_vec.push_back(std::make_tuple("_od_func",func_name+std::string("_od_func")));
        std::stringstream ss;
        ss << init_st;
        arg_vec.push_back(std::make_tuple("init_st",ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "DDE::mealy";}
    
private:
    //! The functions passed to the process constructor
    ns_functype _ns_func;
    od_functype _od_func;
    // Initial value
    ST init_st;
    
    // Input, output, current state, and next state variables
    tt_event<IT>* itok;
    ST* stval;
    ST* nsval;
    OT* oval;

    //Implementing the abstract semantics
    void init()
    {
        itok = new tt_event<IT>;
        stval = new ST;
        *stval = init_st;
        nsval = new ST;
        oval = new OT;
    }
    
    void prep()
    {
        *itok = iport1.read();
    }
    
    void exec()
    {
        _ns_func(*nsval, *stval, get_value(*itok));
        _od_func(*oval, *stval, get_value(*itok));
        *stval = *nsval;
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, tt_event<OT>(*oval,get_time(*itok)))
    }
    
    void clean()
    {
        delete itok;
        delete stval;
        delete nsval;
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

//! Process constructor for a Mealy machine with two inputs
/*! This class is used to build a finite state machine of type Mealy
 * with two inputs. Given an initial state, a next-state function, and
 * an output decoding function it creates a Mealy process.
 */
template <typename IT1, typename IT2, typename ST, typename OT>
class mealy2 : public dde_process
{
public:
    DDE_in<IT1>  iport1;        ///< port for the input channel
    DDE_in<IT2>  iport2;        ///< port for the input channel
    DDE_out<OT> oport1;        ///< port for the output channel
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(ST&, const ST&, const IT1&, const IT2&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(OT&, const ST&, const IT1&, const IT2&)> od_functype;
    
    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    mealy2(const sc_module_name& _name,  ///< process name
           const ns_functype& _ns_func, ///< The next_state function
           const od_functype& _od_func, ///< The output-decoding function
           const ST& init_st  ///< Initial state
          ) : dde_process(_name), _ns_func(_ns_func), _od_func(_od_func),
              init_st(init_st)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_ns_func",func_name+std::string("_ns_func")));
        arg_vec.push_back(std::make_tuple("_od_func",func_name+std::string("_od_func")));
        std::stringstream ss;
        ss << init_st;
        arg_vec.push_back(std::make_tuple("init_st",ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "DDE::mealy2";}
    
private:
    //! The functions passed to the process constructor
    ns_functype _ns_func;
    od_functype _od_func;
    // Initial value
    ST init_st;
    
    // Input, output, current state, and next state variables
    tt_event<IT1> *next_itok1, *cur_itok1;
    tt_event<IT2> *next_itok2, *cur_itok2;
    ST* stval;
    ST* nsval;
    OT* oval;
    
    // the current time (local time)
    sc_time tc;

    // clocks of the input ports (channel times)
    sc_time in1T, in2T;
    
    //Implementing the abstract semantics
    void init()
    {
        next_itok1 = new tt_event<IT1>;
        next_itok2 = new tt_event<IT2>;
        cur_itok1 = new tt_event<IT1>();
        cur_itok2 = new tt_event<IT2>();
        stval = new ST;
        *stval = init_st;
        nsval = new ST;
        oval = new OT;
        in1T = in2T = tc = SC_ZERO_TIME;
    }
    
    void prep()
    {
        if (in1T<in2T)
        {
            *next_itok1 = iport1.read();  // read from input 1
            in1T = get_time(*next_itok1);
        }
        else if (in1T>in2T)
        {
            *next_itok2 = iport2.read();  // read from input 1
            in2T = get_time(*next_itok2);
        }
        else
        {
            *next_itok1 = iport1.read();  // read from input 1
            *next_itok2 = iport2.read();  // read from input 2
            in1T = get_time(*next_itok1);
            in2T = get_time(*next_itok2);
        }
        
        // updated channel clocks and the local clock
        if (get_time(*next_itok1) < get_time(*next_itok2))
        {
            *cur_itok1 = *next_itok1;
            tc = get_time(*cur_itok1);
        }
        else if (get_time(*next_itok1) > get_time(*next_itok2))
        {
            *cur_itok2 = *next_itok2;
            tc = get_time(*cur_itok2);
        }
        else
        {
            *cur_itok1 = *next_itok1;
            *cur_itok2 = *next_itok2;
            tc = get_time(*cur_itok1);
        }
    }
    
    void exec()
    {        
        _ns_func(*nsval, *stval, get_value(*cur_itok1), get_value(*cur_itok2));
        _od_func(*oval, *stval, get_value(*cur_itok1), get_value(*cur_itok2));
        *stval = *nsval;
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, tt_event<OT>(*oval,tc))
        wait(tc - sc_time_stamp());
    }
    
    void clean()
    {
        delete next_itok1;
        delete next_itok2;
        delete cur_itok1;
        delete cur_itok2;
        delete stval;
        delete nsval;
        delete oval;
    }
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

//! Process constructor for implementing a linear filter
/*! This class is used to build a process which implements a linear filter
 * based on the numerator and denominator constants.
 */
template <class T>
class filter : public dde_process
{
public:
    DDE_in<T>  iport1;           ///< port for the input channel
    DDE_out<T> oport1;           ///< port for the output channel
    DDE_out<unsigned int> oport2;///< port for the sampling signal

    typedef matrix<T> MatrixDouble;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial element, reads
     * data from its input port, and writes the results using the output
     * port.
     */
    filter(sc_module_name _name,             ///< process name
            std::vector<T> numerators,       ///< Numerator constants
            std::vector<T> denominators,     ///< Denominator constants
            sc_time max_step,                ///< Maximum time step
            sc_time min_step=sc_time(0.05,SC_NS),///< Minimum time step
            T tol_error=1e-5                 ///< Tolerated error
          ) : dde_process(_name), iport1("iport1"), oport1("oport1"),
              numerators(numerators), denominators(denominators),
              max_step(max_step), min_step(min_step), tol_error(tol_error)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << numerators;
        arg_vec.push_back(std::make_tuple("numerators", ss.str()));
        ss.str("");
        ss << denominators;
        arg_vec.push_back(std::make_tuple("denominators", ss.str()));
        ss.str("");
        ss << max_step;
        arg_vec.push_back(std::make_tuple("max_step", ss.str()));
        ss.str("");
        ss << min_step;
        arg_vec.push_back(std::make_tuple("min_step", ss.str()));
        ss.str("");
        ss << tol_error;
        arg_vec.push_back(std::make_tuple("tol_error", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::filter";}
    
private:
    // Constructor parameters
    std::vector<T> numerators, denominators;
    sc_time max_step, min_step;
    T tol_error;
    
    // Internal variables
    sc_time step;
    sc_time samplingTimeTag; 
    MatrixDouble a, b, c, d;
    // states
    MatrixDouble x, x0, x1, x2;
    // current and previous input/time.
    MatrixDouble u, u0, u1, u_1;
    sc_time t, t_1, t2, h;
    // output
    MatrixDouble y0, y1, y2;
    // Some helper matrices used in RK solver
    MatrixDouble k1,k2,k3,k4;
    // to prevent rounding error
    double roundingFactor;
    
    // Output event
    tt_event<T>* out_ev;
    
    //Implementing the abstract semantics
    void init()
    {
        out_ev = new tt_event<T>;
        
        step = max_step;
        int /*nn = nums.size(),*/ nd = denominators.size();
        a = MatrixDouble(nd-1,nd-1);
        b = MatrixDouble(nd-1,1);
        c = MatrixDouble(1,nd-1);
        d = MatrixDouble(1,1);
    
        tf2ss(numerators,denominators,a,b,c,d);
        
        // State number
        int numState = a.size1();
        assert(a.size1() == a.size2());
        samplingTimeTag = sc_time(0, SC_NS);;
        x = zero_matrix<double>(numState,1);;
        u = MatrixDouble(1,1), u_1 = MatrixDouble(1,1);
        u0 = u1 = MatrixDouble(1,1);
        y1 = MatrixDouble(1,1);
        k1 = MatrixDouble(numState,1);
        k2 = MatrixDouble(numState,1);
        k3 = MatrixDouble(numState,1);
        k4 = MatrixDouble(numState,1);
        
        // initial sampling time tag
        WRITE_MULTIPORT(oport2,tt_event<unsigned int>(0, samplingTimeTag))
        // read initial input
        auto in_ev = iport1.read();
        u(0,0) = get_value(in_ev);
        t = get_time(in_ev);
        // calculate and write initial output
        y1 = boost::numeric::ublas::prod(c,x) + boost::numeric::ublas::prod(d,u);
        *out_ev = tt_event<T>(y1(0,0), t);
        WRITE_MULTIPORT(oport1, *out_ev)
        // step signal
        WRITE_MULTIPORT(oport2, tt_event<unsigned int>(0, samplingTimeTag+step/2))
        WRITE_MULTIPORT(oport2, tt_event<unsigned int>(0, samplingTimeTag+step))
        u_1(0,0) = u(0,0); 
        t_1 = t;
        roundingFactor = 1.0001;
    }
    
    void prep()
    {
        auto in_ev = iport1.read();
        u1(0,0) = get_value(in_ev);
        t = get_time(in_ev);
        
        in_ev = iport1.read();
        u0(0,0) = get_value(in_ev);
        t2 = get_time(in_ev);
    }
    
    void exec()
    {
        // 1st step error estimation
        h = t - t_1;
        rkSolver(a, b, c, d, u1, u_1, x, h.to_seconds(), x1, y1);
        
        // regular RK 
        h = t2 - t_1;
        rkSolver(a, b, c, d, u0, u_1, x, h.to_seconds(), x0, y0);
        
        // 2nd step error estimation
        rkSolver(a, b, c, d, u0, u1, x1, (h/2).to_seconds(), x2, y2);
        
        // error estimation
        double err_est = (double) abs(y2(0,0)-y0(0,0))/(h.to_seconds());
        if( (err_est < tol_error) || (h<=roundingFactor*min_step)) {
          x = x0;
          samplingTimeTag = t;
          // TODO: move the following line to the prod stage
          WRITE_MULTIPORT(oport2, tt_event<unsigned int>(1, samplingTimeTag)) // commitment
          *out_ev = tt_event<T>(y0(0,0), t);
          WRITE_MULTIPORT(oport1, *out_ev)
          u(0,0) = u0(0,0);
          u_1(0,0) = u(0,0); 
          t_1 = t;
          if(h==min_step)
            std::cout << "Step accepted due to minimum step size. "
             << "However, err_tol is not met." << std::endl;
        }
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport2, tt_event<unsigned int>(0, samplingTimeTag+step/2))
        WRITE_MULTIPORT(oport2, tt_event<unsigned int>(0, samplingTimeTag+step))
    }
    
    void clean()
    {
        delete out_ev;
    }
    
    // To obtain state space matrices from transfer function.
    // We assume there are non-zero leading coefficients in num and denom.
    int tf2ss(std::vector<double> & num_, std::vector<double> & den_, MatrixDouble & a, 
          MatrixDouble & b, MatrixDouble & c, MatrixDouble & d)
    {
        std::vector<double> num, den;
        // sizes checking
        int nn = num_.size(), nd = den_.size();
        if(nn >= nd)
        {
            std::cerr << "ERROR: " << "degree(num) = " << nn
            << " >= degree(denom) = " << nd << std::endl;
            abort();
        }
        double dCoef1 = den_.at(0);
        if(nd==1)
        {
            //~ a = NULL, b = NULL, c = NULL;
            d = MatrixDouble(1,1);
            d(0,0) = num_.at(0)/dCoef1;
        }
        else
        {
            if ((nd - nn) > 0)
            {
                // Pad num so that degree(num) == degree(denom)
                for(int i=0; i<nd; i++)
                {
                    if(i<(nd-nn))
                        num.push_back(0.0);
                    else
                        num.push_back(num_.at(i-nd+nn));
                }
            }
        
            // Normalizing w.r.t the leading coefficient of denominator
            for(unsigned int i=0; i<num.size(); i++)
                num.at(i) /= dCoef1;
            for(unsigned int i=0; i<den_.size(); i++)
                den_.at(i) /= dCoef1;
            for(unsigned int i=0; i<(den_.size()-1); i++)
                den.push_back(den_.at(i+1));
        
            // Form A (nd-1)*(nd-1)
            a = zero_matrix<double> (a.size1(), a.size2());
            if(nd > 2)
            {
                // The eyes (up-right corner) are set to '1'
                for(int i=0; i<(nd-2); i++)
                    for(int j=0; j<(nd-1); j++)
                        if((j-i)==1) a(i,j) = 1.0;
                // The lower row(s)
                for(int j=0; j<(nd-1); j++)
                    a(nd-2,j) = 0-den.at(nd-2-j);
            }
            else
                a(0,0) = 0-den.at(0);
            //
            // Form B (nd-1)*1
            b = zero_matrix<double> (b.size1(), b.size2());
            b(nd-2,0) = 1.0;
            //
            // Form C 1*(nd-1)
            for(int j=0; j< nd-1; j++)
                c(0,nd-2-j) = num.at(j+1) - num.at(0)*den.at(j);
            //
            // Form D 1*1
            d(0,0) = num.at(0);
        }
        
        return 0;
    }
    
    void rkSolver(MatrixDouble a, MatrixDouble b, MatrixDouble c,
                  MatrixDouble d, MatrixDouble u_k, MatrixDouble u_k_1,
                  MatrixDouble x, double h, MatrixDouble &x_, MatrixDouble &y)
    {
        // Some helper matrices used in RK solver
        MatrixDouble k1,k2,k3,k4;
        k1 = boost::numeric::ublas::prod(a,x) + boost::numeric::ublas::prod(b,u_k_1);
        k2 = boost::numeric::ublas::prod(a,(x+k1*(h/2.0))) + boost::numeric::ublas::prod(b,(u_k_1 + u_k)) * 0.5;
        k3 = boost::numeric::ublas::prod(a,(x+k2*(h/2.0))) + boost::numeric::ublas::prod(b,(u_k_1 + u_k)) * 0.5;
        k4 = boost::numeric::ublas::prod(a,(x+k3*h)) + boost::numeric::ublas::prod(b,u_k);
        x_ = x + (k1 + 2.0*k2 + 2.0*k3 + k4) * (h/6.0);
        y = boost::numeric::ublas::prod(c,x_) + boost::numeric::ublas::prod(d,u_k);
    }
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport1;
        boundOutChans.resize(2);    // two output ports
        boundOutChans[0].port = &oport1;
        boundOutChans[1].port = &oport2;
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
class constant : public dde_process
{
public:
    DDE_out<T> oport1;            ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    constant(sc_module_name _name,      ///< The module name
              T init_val,                ///< The constant output value
              sc_time interval           ///< The interval for which the output exist (the time of the second and last event)
             ) : dde_process(_name), oport1("oport1"),
                 init_val(init_val), interval(interval)
                 
    {
#ifdef FORSYDE_INTROSPECTION
        arg_vec.push_back(std::make_tuple("init_val", std::to_string(init_val)));
        arg_vec.push_back(std::make_tuple("interval", std::to_string(interval.to_double())));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::constant";}
    
private:
    T init_val;
    sc_time interval;
    
    unsigned int tok_cnt;
    
    //Implementing the abstract semantics
    void init()
    {
        tok_cnt = 0;
    }
    
    void prep() {}
    
    void exec() {}
    
    void prod()
    {
        if (tok_cnt++ <= 1)
        {
            WRITE_MULTIPORT(oport1, tt_event<T>(init_val, sc_time_stamp()))
            wait(interval - sc_time_stamp());
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

//! Process constructor for a single source process
/*! This class is used to build a source process with a single event in
 * its output.
 * Its main purpose is to be used in test-benches.
 */
template <class T>
class single : public dde_process
{
public:
    DDE_out<T> oport1;            ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    single(sc_module_name _name,        ///< The module name
              T val,                    ///< The constant output value
              sc_time instant           ///< The instant at which the event happens
             ) : dde_process(_name), oport1("oport1"),
                 val(val), instant(instant)
                 
    {
#ifdef FORSYDE_INTROSPECTION
        arg_vec.push_back(std::make_tuple("val", std::to_string(val)));
        arg_vec.push_back(std::make_tuple("instant", std::to_string(instant.to_double())));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::single";}
    
private:
    T val;
    sc_time instant;
        
    //Implementing the abstract semantics
    void init() {}
    
    void prep() {}
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, tt_event<T>(val, instant))
        wait(instant - sc_time_stamp());
        // Promise no more values
        WRITE_MULTIPORT(oport1, tt_event<T>(T(), sc_max_time()))
        wait();
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
class source : public dde_process
{
public:
    DDE_out<T> oport1;        ///< port for the output channel
    
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
          ) : dde_process(_name), oport1("oport1"),
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
    std::string forsyde_kind() const {return "DDE::source";}
    
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
        wait(get_time(*cur_st) - sc_time_stamp());
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
        {
            WRITE_MULTIPORT(oport1, *cur_st)
            wait(get_time(*cur_st) - sc_time_stamp());
        }
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
class sink : public dde_process
{
public:
    DDE_in<T> iport1;         ///< port for the input channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(const tt_event<T>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    sink(sc_module_name _name,      ///< process name
         functype _func             ///< function to be passed
        ) : dde_process(_name), iport1("iport1"), _func(_func)
            
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::sink";}
    
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
class zip : public dde_process
{
public:
    DDE_in<T1> iport1;        ///< port for the input channel 1
    DDE_in<T2> iport2;        ///< port for the input channel 2
    DDE_out<std::tuple<T1,T2>> oport1;///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    zip(sc_module_name _name)
         :dde_process(_name), iport1("iport1"), iport2("iport2"), oport1("oport1")
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::zip";}
    
private:    
    // inputs and output variables
    tt_event<T1> *next_itok1, *cur_itok1;
    tt_event<T2> *next_itok2, *cur_itok2;
        
    // the current time (local time)
    sc_time tc;
    
    // clocks of the input ports (channel times)
    sc_time in1T, in2T;
    
    void init()
    {
        next_itok1 = new tt_event<T1>;
        next_itok2 = new tt_event<T2>;
        cur_itok1 = new tt_event<T1>();
        cur_itok2 = new tt_event<T2>();
        in1T = in2T = tc = SC_ZERO_TIME;
    }
    
    void prep()
    {
        if (in1T<in2T)
        {
            *next_itok1 = iport1.read();  // read from input 1
            in1T = get_time(*next_itok1);
        }
        else if (in1T>in2T)
        {
            *next_itok2 = iport2.read();  // read from input 1
            in2T = get_time(*next_itok2);
        }
        else
        {
            *next_itok1 = iport1.read();  // read from input 1
            *next_itok2 = iport2.read();  // read from input 2
            in1T = get_time(*next_itok1);
            in2T = get_time(*next_itok2);
        }
        
        // updated channel clocks and the local clock
        if (get_time(*next_itok1) < get_time(*next_itok2))
        {
            *cur_itok1 = *next_itok1;
            tc = get_time(*cur_itok1);
        }
        else if (get_time(*next_itok1) > get_time(*next_itok2))
        {
            *cur_itok2 = *next_itok2;
            tc = get_time(*cur_itok2);
        }
        else
        {
            *cur_itok1 = *next_itok1;
            *cur_itok2 = *next_itok2;
            tc = get_time(*cur_itok1);
        }
    }
    
    void exec()
    {
        
    }
    
    void prod()
    {
        auto zipped_vals = std::make_tuple(get_value(*cur_itok1),get_value(*cur_itok2));
        auto temp_event = tt_event<std::tuple<T1,T2>>(zipped_vals,tc);
        WRITE_MULTIPORT(oport1,temp_event)
        wait(tc - sc_time_stamp());
    }
    
    void clean()
    {
        delete next_itok1;
        delete next_itok2;
        delete cur_itok1;
        delete cur_itok2;
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

//! The unzip process with one input and two outputs
/*! This process "unzips" a signal of tuples into two separate signals
 */
template <class T1, class T2>
class unzip : public dde_process
{
public:
    DDE_in<std::tuple<T1,T2>> iport1;///< port for the input channel
    DDE_out<T1> oport1;        ///< port for the output channel 1
    DDE_out<T2> oport2;        ///< port for the output channel 2

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * unzips them and writes the results using the output ports
     */
    unzip(sc_module_name _name)
         :dde_process(_name), iport1("iport1"), oport1("oport1"), oport2("oport2")
    {}
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::unzip";}
private:
    // intermediate values
    tt_event<std::tuple<T1,T2>>* in_val;
    
    void init()
    {
        in_val = new tt_event<std::tuple<T1,T2>>;
    }
    
    void prep()
    {
        *in_val = iport1.read();
    }
    
    void exec() {}
    
    void prod()
    {
        sc_time te(get_time(*in_val));
        WRITE_MULTIPORT(oport1,tt_event<T1>(std::get<0>(get_value(*in_val)),te))  // write to the output 1
        WRITE_MULTIPORT(oport2,tt_event<T2>(std::get<1>(get_value(*in_val)),te))  // write to the output 2
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
class fanout : public dde_process
{
public:
    DDE_in<T> iport1;        ///< port for the input channel
    DDE_out<T> oport1;       ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies and writes the results using the output port
     */
    fanout(sc_module_name _name)  // module name
         : dde_process(_name) { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::fanout";}
    
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
