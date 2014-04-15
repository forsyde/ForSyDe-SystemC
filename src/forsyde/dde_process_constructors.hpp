/**********************************************************************           
    * dde_process_constructors.hpp -- Process constructors in the DDE *
    *                                MOC                              *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
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
    typedef std::function<void(abst_ext<T0>&, const T1&)> functype;

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
    abst_ext<T0>* oval;
    ttn_event<T1>* iev1;
    
    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        oval = new abst_ext<T0>;
        iev1 = new ttn_event<T1>;
    }
    
    void prep()
    {
        *iev1 = iport1.read();
    }
    
    void exec()
    {
        if (is_present(get_value(*iev1)))
            _func(*oval, unsafe_from_abst_ext(get_value(*iev1)));
        else
            *oval = abst_ext<T0>();
    }
    
    void prod()
    {
        auto oev = ttn_event<T0>(*oval, get_time(*iev1));
        WRITE_MULTIPORT(oport1, oev)
        // synchronization with kernel time
        wait(get_time(oev) - sc_time_stamp());
    }
    
    void clean()
    {
        delete iev1;
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
    typedef std::function<void(abst_ext<T0>&, const abst_ext<T1>&, const abst_ext<T2>&)> functype;

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
    abst_ext<T0>* oval;
    ttn_event<T1> *next_iev1;
    ttn_event<T2> *next_iev2;
    abst_ext<T1> *cur_ival1;
    abst_ext<T2> *cur_ival2;
    
    // the current time (local time)
    sc_time tl;
    
    // clocks of the input ports (channel times)
    sc_time in1T, in2T;
    
    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        oval = new abst_ext<T0>;
        next_iev1 = new ttn_event<T1>;
        next_iev2 = new ttn_event<T2>;
        cur_ival1 = new abst_ext<T1>;
        cur_ival2 = new abst_ext<T2>;
        in1T = in2T = tl = SC_ZERO_TIME;
    }
    
    void prep()
    {
        if (in1T == tl)
        {
            *next_iev1 = iport1.read();
            in1T = get_time(*next_iev1);
        }
        if (in2T == tl)
        {
            *next_iev2 = iport2.read();
            in2T = get_time(*next_iev2);
        }
        
        // update channel clocks and the local clock
        tl = std::min(in1T, in2T);
        
        // update current values
        if (get_time(*next_iev1) == tl)
            *cur_ival1 = get_value(*next_iev1);
        else
            *cur_ival1 = abst_ext<T1>();
        if (get_time(*next_iev2) == tl)
            *cur_ival2 = get_value(*next_iev2);
        else
            *cur_ival2 = abst_ext<T1>();
    }
    
    void exec()
    {
        if (is_absent(*cur_ival1) && is_absent(*cur_ival2))
            *oval = abst_ext<T0>();
        else
            _func(*oval, *cur_ival1, *cur_ival2);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, ttn_event<T0>(*oval,tl))
        wait(tl - sc_time_stamp());
    }
    
    void clean()
    {
        delete oval;
        delete next_iev1;
        delete next_iev2;
        delete cur_ival1;
        delete cur_ival2;
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
/*! This class is used to build a delay element. Given an initial null-
 * extended value and a delay time, it inserts this value as the first
 * event in time zero in the output and delays the rest of the events by
 * the delay time.
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
          abst_ext<T> init_val,     ///< initial value
          sc_time delay_time        ///< delay time
          ) : dde_process(_name), iport1("iport1"), oport1("oport1"),
              init_val(init_val), delay_time(delay_time)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
        ss.str("");
        ss << delay_time;
        arg_vec.push_back(std::make_tuple("delay_time", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::delay";}
    
private:
    // Initial value and the delay time
    abst_ext<T> init_val;
    sc_time delay_time;
    
    // Inputs and output variables
    ttn_event<T>* ev;
    
    //Implementing the abstract semantics
    void init()
    {
        ev = new ttn_event<T>;
        auto oev = ttn_event<T>(init_val, SC_ZERO_TIME);
        WRITE_MULTIPORT(oport1, oev)
        wait(SC_ZERO_TIME);
    }
    
    void prep()
    {
        *ev = iport1.read();
    }
    
    void exec()
    {
        set_time(*ev, get_time(*ev)+delay_time);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *ev)
        wait(get_time(*ev) - sc_time_stamp());
    }
    
    void clean()
    {
        delete ev;
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
    typedef std::function<void(ST&, const ST&, const ttn_event<IT>&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(abst_ext<OT>&, const ST&, const ttn_event<IT>&)> od_functype;
    
    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    mealy(const sc_module_name& _name,  ///< process name
           const ns_functype& _ns_func, ///< The next_state function
           const od_functype& _od_func, ///< The output-decoding function
           const ST& init_st,           ///< Initial state
           const sc_time& delay_time    ///< The constant delay for output
          ) : dde_process(_name), _ns_func(_ns_func), _od_func(_od_func),
              init_st(init_st), delay_time(delay_time)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_ns_func",func_name+std::string("_ns_func")));
        arg_vec.push_back(std::make_tuple("_od_func",func_name+std::string("_od_func")));
        std::stringstream ss;
        ss << init_st;
        arg_vec.push_back(std::make_tuple("init_st",ss.str()));
        ss.str("");
        ss << delay_time;
        arg_vec.push_back(std::make_tuple("delay_time",ss.str()));
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
    
    sc_time delay_time;
    
    // Input, output, current state, and next state variables
    ttn_event<IT>* itok;
    ST* stval;
    ST* nsval;
    abst_ext<OT>* oval;

    //Implementing the abstract semantics
    void init()
    {
        itok = new ttn_event<IT>;
        stval = new ST;
        *stval = init_st;
        nsval = new ST;
        oval = new abst_ext<OT>;
    }
    
    void prep()
    {
        *itok = iport1.read();
    }
    
    void exec()
    {
        _ns_func(*nsval, *stval, *itok);
        _od_func(*oval, *stval, *itok);
        *stval = *nsval;
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, ttn_event<OT>(*oval,get_time(*itok)+delay_time))
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
    typedef std::function<void(ST&, const ST&, const ttn_event<IT1>&, const ttn_event<IT2>&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(abst_ext<OT>&, const ST&, const ttn_event<IT1>&, const ttn_event<IT2>&)> od_functype;
    
    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    mealy2(const sc_module_name& _name, ///< process name
           const ns_functype& _ns_func, ///< The next_state function
           const od_functype& _od_func, ///< The output-decoding function
           const ST& init_st,           ///< Initial state
           const sc_time& delay_time    ///< The constant delay for output
          ) : dde_process(_name), _ns_func(_ns_func), _od_func(_od_func),
              init_st(init_st), delay_time(delay_time)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_ns_func",func_name+std::string("_ns_func")));
        arg_vec.push_back(std::make_tuple("_od_func",func_name+std::string("_od_func")));
        std::stringstream ss;
        ss << init_st;
        arg_vec.push_back(std::make_tuple("init_st",ss.str()));
        ss.str("");
        ss << delay_time;
        arg_vec.push_back(std::make_tuple("delay_time",ss.str()));
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
    
    sc_time delay_time;
    
    // Input, output, current state, and next state variables
    ttn_event<IT1> *next_iev1;
    ttn_event<IT2> *next_iev2;
    abst_ext<IT1> *cur_ival1;
    abst_ext<IT2> *cur_ival2;
    ST* stval;
    ST* nsval;
    abst_ext<OT>* oval;
    
    // the current time (local time)
    sc_time tl;

    // clocks of the input ports (channel times)
    sc_time in1T, in2T;
    
    //Implementing the abstract semantics
    void init()
    {
        next_iev1 = new ttn_event<IT1>;
        next_iev2 = new ttn_event<IT2>;
        cur_ival1 = new abst_ext<IT1>;
        cur_ival2 = new abst_ext<IT2>;
        stval = new ST;
        *stval = init_st;
        nsval = new ST;
        oval = new abst_ext<OT>;
        in1T = in2T = tl = SC_ZERO_TIME;
    }
    
    void prep()
    {
        if (in1T == tl)
        {
            *next_iev1 = iport1.read();
            in1T = get_time(*next_iev1);
        }
        if (in2T == tl)
        {
            *next_iev2 = iport2.read();
            in2T = get_time(*next_iev2);
        }
        
        // update channel clocks and the local clock
        tl = std::min(in1T, in2T);
        
        // update current values
        if (get_time(*next_iev1) == tl)
            *cur_ival1 = get_value(*next_iev1);
        else
            *cur_ival1 = abst_ext<IT1>();
        if (get_time(*next_iev2) == tl)
            *cur_ival2 = get_value(*next_iev2);
        else
            *cur_ival2 = abst_ext<IT1>();
    }
    
    void exec()
    {
        if (is_absent(*cur_ival1) && is_absent(*cur_ival2))
            *oval = abst_ext<OT>();
        else
        {
            _ns_func(*nsval, *stval, ttn_event<IT1>(*cur_ival1,tl), ttn_event<IT2>(*cur_ival2,tl));
            _od_func(*oval, *stval, ttn_event<IT1>(*cur_ival1,tl), ttn_event<IT2>(*cur_ival2,tl));
            *stval = *nsval;
        }
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, ttn_event<OT>(*oval,tl+delay_time))
        wait(tl - sc_time_stamp());
    }
    
    void clean()
    {
        delete next_iev1;
        delete next_iev2;
        delete cur_ival1;
        delete cur_ival2;
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
    ttn_event<T>* out_ev;
    
    //Implementing the abstract semantics
    void init()
    {
        out_ev = new ttn_event<T>;
        
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
        samplingTimeTag = SC_ZERO_TIME;
        x = zero_matrix<T>(numState,1);
        u = MatrixDouble(1,1), u_1 = MatrixDouble(1,1);
        u0 = u1 = MatrixDouble(1,1);
        y1 = MatrixDouble(1,1);
        k1 = MatrixDouble(numState,1);
        k2 = MatrixDouble(numState,1);
        k3 = MatrixDouble(numState,1);
        k4 = MatrixDouble(numState,1);
        
        // initial sampling time tag
        WRITE_MULTIPORT(oport2,ttn_event<unsigned int>(0, samplingTimeTag))
        // read initial input
        auto in_ev = iport1.read();
        u(0,0) = unsafe_from_abst_ext(get_value(in_ev)); // FIXME: assumes non-null inputs
        t = get_time(in_ev);
        // calculate and write initial output
        y1 = boost::numeric::ublas::prod(c,x) + boost::numeric::ublas::prod(d,u);
        *out_ev = ttn_event<T>(y1(0,0), t);
        WRITE_MULTIPORT(oport1, *out_ev)
        // step signal
        WRITE_MULTIPORT(oport2, ttn_event<unsigned int>(0, samplingTimeTag+step/2))
        WRITE_MULTIPORT(oport2, ttn_event<unsigned int>(0, samplingTimeTag+step))
        u_1(0,0) = u(0,0); 
        t_1 = t;
        roundingFactor = 1.0001;
    }
    
    void prep()
    {
        auto in_ev = iport1.read();
        u1(0,0) = unsafe_from_abst_ext(get_value(in_ev)); // FIXME: assumes non-null inputs
        t = get_time(in_ev);
        
        in_ev = iport1.read();
        u0(0,0) = unsafe_from_abst_ext(get_value(in_ev)); // FIXME: assumes non-null inputs
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
          WRITE_MULTIPORT(oport2, ttn_event<unsigned int>(1, samplingTimeTag)) // commitment
          *out_ev = ttn_event<T>(y0(0,0), t);
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
        WRITE_MULTIPORT(oport2, ttn_event<unsigned int>(0, samplingTimeTag+step/2))
        WRITE_MULTIPORT(oport2, ttn_event<unsigned int>(0, samplingTimeTag+step))
    }
    
    void clean()
    {
        delete out_ev;
    }
    
    // To obtain state space matrices from transfer function.
    // We assume there are non-zero leading coefficients in num and denom.
    int tf2ss(std::vector<T> & num_, std::vector<T> & den_, MatrixDouble & a, 
          MatrixDouble & b, MatrixDouble & c, MatrixDouble & d)
    {
        std::vector<T> num, den;
        // sizes checking
        int nn = num_.size(), nd = den_.size();
        if(nn >= nd)
        {
            std::cerr << "ERROR: " << "degree(num) = " << nn
            << " >= degree(denom) = " << nd << std::endl;
            abort();
        }
        T dCoef1 = den_.at(0);
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
            a = zero_matrix<T> (a.size1(), a.size2());
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
            b = zero_matrix<T> (b.size1(), b.size2());
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
                  MatrixDouble x, T h, MatrixDouble &x_, MatrixDouble &y)
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

//! Process constructor for implementing a linear filter with fixed step size
/*! This class is used to build a process which implements a linear filter
 * with fixed step size based on the numerator and denominator constants.
 */
template <class T>
class filterf : public dde_process
{
public:
    DDE_in<T>  iport1;           ///< port for the input channel
    DDE_out<T> oport1;           ///< port for the output channel

    typedef matrix<T> MatrixDouble;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial element, reads
     * data from its input port, and writes the results using the output
     * port.
     */
    filterf(sc_module_name _name,           ///< process name
            std::vector<T> numerators,      ///< Numerator constants
            std::vector<T> denominators,    ///< Denominator constants
            sc_time step_size               ///< Step size
          ) : dde_process(_name), iport1("iport1"), oport1("oport1"),
              numerators(numerators), denominators(denominators),
              step_size(step_size)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << numerators;
        arg_vec.push_back(std::make_tuple("numerators", ss.str()));
        ss.str("");
        ss << denominators;
        arg_vec.push_back(std::make_tuple("denominators", ss.str()));
        ss.str("");
        ss << step_size;
        arg_vec.push_back(std::make_tuple("step_size", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::filterf";}
    
private:
    // Constructor parameters
    std::vector<T> numerators, denominators;
    sc_time step_size;
    
    // Internal variables
    sc_time step;
    sc_time samplingTimeTag; 
    MatrixDouble a, b, c, d;
    // states
    MatrixDouble x, x_1;
    // current and previous input/time.
    MatrixDouble u, u_1;
    sc_time t, t_1, h;
    // output
    MatrixDouble y;
    // Some helper matrices used in RK solver
    MatrixDouble k1,k2,k3,k4;
    // to prevent rounding error
    double roundingFactor;
    
    // Output event
    ttn_event<T>* out_ev;
    
    //Implementing the abstract semantics
    void init()
    {
        out_ev = new ttn_event<T>;
        
        int /*nn = nums.size(),*/ nd = denominators.size();
        a = MatrixDouble(nd-1,nd-1);
        b = MatrixDouble(nd-1,1);
        c = MatrixDouble(1,nd-1);
        d = MatrixDouble(1,1);
    
        tf2ss(numerators,denominators,a,b,c,d);
        
        // State number
        int numState = a.size1();
        assert(a.size1() == a.size2());
        x = zero_matrix<T>(numState,1), x_1 = zero_matrix<T>(numState,1);
        u = MatrixDouble(1,1), u_1 = MatrixDouble(1,1);
        y = MatrixDouble(1,1);
        k1 = MatrixDouble(numState,1);
        k2 = MatrixDouble(numState,1);
        k3 = MatrixDouble(numState,1);
        k4 = MatrixDouble(numState,1);
        
        // read initial input
        auto in_ev = iport1.read();
        u(0,0) = unsafe_from_abst_ext(get_value(in_ev)); // FIXME: assumes non-absent inputs
        t = get_time(in_ev);
        // calculate and write initial output
        y = boost::numeric::ublas::prod(c,x) + boost::numeric::ublas::prod(d,u);
        *out_ev = ttn_event<T>(y(0,0), t);
        WRITE_MULTIPORT(oport1, *out_ev)
        wait(t - sc_time_stamp());
        u_1(0,0) = u(0,0); 
        t_1 = t;
        roundingFactor = 1.0001;
    }
    
    void prep()
    {
        auto in_ev = iport1.read();
        u(0,0) = unsafe_from_abst_ext(get_value(in_ev)); // FIXME: assumes non-absent inputs
        t = get_time(in_ev);
    }
    
    void exec()
    {
        // 1st step error estimation
        h = t - t_1;
        rkSolver(a, b, c, d, u, u_1, x_1, h.to_seconds(), x, y);
        *out_ev = ttn_event<T>(y(0,0), t);
        

    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *out_ev)
        wait(t - sc_time_stamp());
        x_1 = x;
        u_1(0,0) = u(0,0); 
        t_1 = t;
    }
    
    void clean()
    {
        delete out_ev;
    }
    
    // To obtain state space matrices from transfer function.
    // We assume there are non-zero leading coefficients in num and denom.
    int tf2ss(std::vector<T> & num_, std::vector<T> & den_, MatrixDouble & a, 
          MatrixDouble & b, MatrixDouble & c, MatrixDouble & d)
    {
        std::vector<T> num, den;
        // sizes checking
        int nn = num_.size(), nd = den_.size();
        if(nn >= nd)
        {
            std::cerr << "ERROR: " << "degree(num) = " << nn
            << " >= degree(denom) = " << nd << std::endl;
            abort();
        }
        T dCoef1 = den_.at(0);
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
            a = zero_matrix<T> (a.size1(), a.size2());
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
            b = zero_matrix<T> (b.size1(), b.size2());
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
                  MatrixDouble x, T h, MatrixDouble &x_, MatrixDouble &y)
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
        boundOutChans.resize(1);    // two output ports
        boundOutChans[0].port = &oport1;
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
    typedef std::function<void(ttn_event<T>&, const ttn_event<T>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    source(sc_module_name _name,   ///< The module name
           functype _func,         ///< function to be passed
           ttn_event<T> init_st,   ///< Initial state
           unsigned long long take=0 ///< number of tokens produced (0 for infinite)
          ) : dde_process(_name), oport1("oport1"),
              init_st(init_st), take(take), _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
        std::stringstream ss;
        ss << init_st;
        arg_vec.push_back(std::make_tuple("init_st", ss.str()));
        ss.str("");
        ss << take;
        arg_vec.push_back(std::make_tuple("take", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::source";}
    
private:
    ttn_event<T> init_st;        // The current state
    unsigned long long take;    // Number of tokens produced
    
    ttn_event<T>* cur_st;        // The current state of the process
    unsigned long long tok_cnt;
    bool infinite;
    
    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        cur_st = new ttn_event<T>;
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
template <class T>
class vsource : public dde_process
{
public:
    DDE_out<T> oport1;     ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which writes the result using the output
     * port.
     */
    vsource(sc_module_name _name,                   ///< the module name
            const std::vector<T>& values,           ///< event values
            const std::vector<sc_time>& offsets     ///< event offsets
            ) : dde_process(_name), oport1("oport1"),
                values(values), offsets(offsets)
    {
        if (values.size()<offsets.size())
            SC_REPORT_ERROR(name(),"Error matching values and offsets vectors!");
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << values;
        arg_vec.push_back(std::make_tuple("values", ss.str()));
        ss.str("");
        ss << offsets;
        arg_vec.push_back(std::make_tuple("offsets", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::vsource";}
private:
    std::vector<T> values;
    std::vector<sc_time> offsets;
    size_t iter;

    //Implementing the abstract semantics
    void init()
    {
        iter = 0;
    }
    
    void prep() {}
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, ttn_event<T>(abst_ext<T>(values[iter]), offsets[iter]))
        wait(offsets[iter] - sc_time_stamp());
        iter++;
        if (iter == values.size())
        {
            // Promise no more values
            WRITE_MULTIPORT(oport1, ttn_event<T>(abst_ext<T>(), sc_max_time()))
            wait();
        }
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
    typedef std::function<void(const ttn_event<T>&)> functype;

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
    ttn_event<T>* val;         // The current state of the process

    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new ttn_event<T>;
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

//! The zip process with two inputs and one output
/*! This process "zips" two incoming signals into one signal of tuples.
 */
template <class T1, class T2>
class zip : public dde_process
{
public:
    DDE_in<T1> iport1;        ///< port for the input channel 1
    DDE_in<T2> iport2;        ///< port for the input channel 2
    DDE_out< std::tuple<abst_ext<T1>,abst_ext<T2>> > oport1;///< port for the output channel

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
    ttn_event<T1> *next_iev1;
    ttn_event<T2> *next_iev2;
    abst_ext<T1> *cur_ival1;
    abst_ext<T2> *cur_ival2;
    abst_ext< std::tuple<abst_ext<T1>,abst_ext<T2>> >* oval;
        
    // the current time (local time)
    sc_time tl;
    
    // clocks of the input ports (channel times)
    sc_time in1T, in2T;
    
    void init()
    {
        next_iev1 = new ttn_event<T1>;
        next_iev2 = new ttn_event<T2>;
        cur_ival1 = new abst_ext<T1>;
        cur_ival2 = new abst_ext<T2>;
        in1T = in2T = tl = SC_ZERO_TIME;
        oval = new abst_ext< std::tuple<abst_ext<T1>,abst_ext<T2>> >();
    }
    
    void prep()
    {
        if (in1T == tl)
        {
            *next_iev1 = iport1.read();
            in1T = get_time(*next_iev1);
        }
        if (in2T == tl)
        {
            *next_iev2 = iport2.read();
            in2T = get_time(*next_iev2);
        }
        
        // update channel clocks and the local clock
        tl = std::min(in1T, in2T);
        
        // update current values
        if (get_time(*next_iev1) == tl)
            *cur_ival1 = get_value(*next_iev1);
        else
            *cur_ival1 = abst_ext<T1>();
        if (get_time(*next_iev2) == tl)
            *cur_ival2 = get_value(*next_iev2);
        else
            *cur_ival2 = abst_ext<T1>();
    }
    
    void exec()
    {
        if (is_absent(*cur_ival1) && is_absent(*cur_ival2))
            oval->set_abst();
        else
            *oval = abst_ext< std::tuple<abst_ext<T1>,abst_ext<T2>> >(
                std::make_tuple(*cur_ival1,*cur_ival2)
            );
    }
    
    void prod()
    {
        auto temp_event = ttn_event<std::tuple<abst_ext<T1>,abst_ext<T2>>>(*oval,tl);
        WRITE_MULTIPORT(oport1,temp_event)
        wait(tl - sc_time_stamp());
    }
    
    void clean()
    {
        delete next_iev1;
        delete next_iev2;
        delete cur_ival1;
        delete cur_ival2;
        delete oval;
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
        boundOutChans[0].portType = typeid(std::tuple<T1,T2>).name();  // FIXME: shouldn't be abst_ext<Tx> instead of Tx?
    }
#endif
};

//! The zipX process with a vector of inputs and one output
/*! This process "zips" a vector of incoming signals into one signal of
 * vector type.
 */
template <class T1, std::size_t N>
class zipX : public dde_process
{
public:
    std::array<DDE_in<T1>,N> iport;    ///< port array for the input channels
    DDE_out<std::array<abst_ext<T1>,N>> oport1;  ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    zipX(sc_module_name _name)
         :dde_process(_name), oport1("oport1")
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::zipX";}
    
private:    
    // inputs and output variables
    std::array<ttn_event<T1>,N> next_ievs;
    std::array<abst_ext<T1>,N> cur_ivals;
    abst_ext< std::array<abst_ext<T1>,N> >* oval;
        
    // the current time (local time)
    sc_time tl;
    
    // clocks of the input ports (channel times)
    std::array<sc_time,N> insT;
    
    void init()
    {
        insT.fill(SC_ZERO_TIME);
        tl = SC_ZERO_TIME;
        oval = new abst_ext< std::array<abst_ext<T1>,N> >();
    }
    
    void prep()
    {
        for (size_t i=0;i<N;i++)
            if (insT[i] == tl)
            {
                next_ievs[i] = iport[i].read();
                insT[i] = get_time(next_ievs[i]);
            }
        
        // update channel clocks and the local clock
        tl = *std::min_element(insT.begin(), insT.end());
        
        // update current values
        for (size_t i=0;i<N;i++)
            if (get_time(next_ievs[i]) == tl)
                cur_ivals[i] = get_value(next_ievs[i]);
            else
                cur_ivals[i] = abst_ext<T1>();
    }
    
    void exec()
    {
        if (std::all_of(cur_ivals.begin(), cur_ivals.end(), [](abst_ext<T1> el){
            return is_absent(el);
        }))
            oval->set_abst();
        else
            *oval = abst_ext< std::array<abst_ext<T1>,N> >(cur_ivals);
    }
    
    void prod()
    {
        auto temp_event = ttn_event<std::array<abst_ext<T1>,N>>(*oval,tl);
        WRITE_MULTIPORT(oport1,temp_event)
        wait(tl - sc_time_stamp());
    }
    
    void clean()
    {
        delete oval;
    }
    
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

//! The unzip process with one input and two outputs
/*! This process "unzips" a signal of tuples into two separate signals
 */
template <class T1, class T2>
class unzip : public dde_process
{
public:
    DDE_in<std::tuple<abst_ext<T1>,abst_ext<T2>>> iport1;///< port for the input channel
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
    ttn_event< std::tuple<abst_ext<T1>,abst_ext<T2>> >* in_ev;
    abst_ext<T1>* out_val1;
    abst_ext<T2>* out_val2;
    
    void init()
    {
        in_ev = new ttn_event< std::tuple<abst_ext<T1>,abst_ext<T2>> >;
        out_val1 = new abst_ext<T1>;
        out_val2 = new abst_ext<T2>;
    }
    
    void prep()
    {
        *in_ev = iport1.read();
    }
    
    void exec()
    {
        if (is_absent(get_value(*in_ev)))
        {
            *out_val1 = abst_ext<T1>();
            *out_val2 = abst_ext<T2>();
        }
        else
        {
            *out_val1 = std::get<0>(get_value(*in_ev));
            *out_val2 = std::get<1>(get_value(*in_ev));
        }
    }
    
    void prod()
    {
        sc_time te(get_time(*in_ev));
        WRITE_MULTIPORT(oport1,ttn_event<T1>(out_val1,te))  // write to the output 1
        WRITE_MULTIPORT(oport2,ttn_event<T2>(out_val2,te))  // write to the output 2
    }
    
    void clean()
    {
        delete in_ev;
        delete out_val1;
        delete out_val2;
    }
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport1;
        boundInChans[0].portType = typeid(std::tuple<T1,T2>).name(); // FIXME: shouldn't be abst_ext<Tx> instead of Tx?
        boundOutChans.resize(2);    // two output ports
        boundOutChans[0].port = &oport1;
        boundOutChans[0].portType = typeid(T1).name();
        boundOutChans[1].port = &oport2;
        boundOutChans[1].portType = typeid(T2).name();
    }
#endif
};

//! The unzipX process with a vector of outputs and one input
/*! This process "unzips" a signal of vector type into a vector of
 * output signals.
 */
template <class T1, std::size_t N>
class unzipX : public dde_process
{
public:
    DDE_in<std::array<abst_ext<T1>,N>> iport1;  ///< port for the input channel
    std::array<DDE_out<T1>,N> oport;    ///< port array for the output channels

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    unzipX(sc_module_name _name)
         :dde_process(_name), iport1("iport1")
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "DDE::unzipX";}
    
private:
    // intermediate values
    ttn_event<std::array<abst_ext<T1>,N>>* in_ev;
    
    // output events
    std::array<ttn_event<T1>,N> oevs;
    
    sc_time tl;
    
    void init()
    {
        in_ev = new ttn_event<std::array<abst_ext<T1>,N>>;
        tl = SC_ZERO_TIME;
    }
    
    void prep()
    {
        *in_ev = iport1.read();
    }
    
    void exec()
    {
        if (is_absent(*in_ev))
        {
            for (size_t i=0; i<N; i++)
                oevs[i] = ttn_event<T1>(abst_ext<T1>(),tl);
        }
        else
        {
            for (size_t i=0; i<N; i++)
                oevs[i] = ttn_event<T1>(unsafe_from_abst_ext(*in_ev)[i]);
        }
        tl = get_time(*in_ev);
    }
    
    void prod()
    {
        for (size_t i=0; i<N; i++)
            WRITE_MULTIPORT(oport[i],oevs[i])  // write to the output i
        wait(tl - sc_time_stamp());
    }
    
    void clean()
    {
        delete in_ev;
    }
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport1;
        boundOutChans.resize(N);    // output ports
        for (size_t i=0;i<N;i++)
            boundOutChans[i].port = &oport[i];
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
    ttn_event<T>* val;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new ttn_event<T>;
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
