/**********************************************************************           
    * ut_process_constructors.hpp -- Process constructors in the      *
    *                                untimed MOC.                     *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing basic process constructors for modeling      *
    *          UT systems in ForSyDe-SystemC                          *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef UT_PROCESS_CONSTRUCTORS_HPP
#define UT_PROCESS_CONSTRUCTORS_HPP

/*! \file ut_process_constructors.hpp
 * \brief Implements the basic process constructors in the UT MoC
 * 
 *  This file includes the basic process constructors used for modeling
 * in the untimed model of computation.
 */

#include <functional>
#include <tuple>
#include <vector>

#include "ut_process.hpp"

namespace ForSyDe
{

namespace UT
{

using namespace sc_core;

//! Process constructor for a combinational process (actor) with one input and one output
/*! This class is used to build combinational processes with one input
 * and one output. The class is parameterized for input and output
 * data-types.
 */
template <typename T0, typename T1>
class comb : public ut_process
{
public:
    UT_in<T1>  iport1;       ///< port for the input channel
    UT_out<T0> oport1;       ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(std::vector<T0>&,
                                const std::vector<T1>&
                                )> functype;

    //! The constructor requires the module name ad the number of tokens to be produced
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    comb(const sc_module_name& _name,      ///< process name
         const functype& _func,           ///< function to be passed
         const unsigned int& i1toks       ///< consumption rate for the first input
         ) : ut_process(_name), iport1("iport1"), oport1("oport1"),
             i1toks(i1toks), _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
        arg_vec.push_back(std::make_tuple("i1toks",std::to_string(i1toks)));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::comb";}

private:
    // consumption rates
    unsigned int i1toks;
    
    // Inputs and output variables
    std::vector<T0> o1vals;
    std::vector<T1> i1vals;
    
    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        i1vals.resize(i1toks);
    }
    
    void prep()
    {
        for (auto it=i1vals.begin();it!=i1vals.end();it++)
            *it = iport1.read();
    }
    
    void exec()
    {
        _func(o1vals, i1vals);
    }
    
    void prod()
    {
        write_vec_multiport(oport1, o1vals);
        o1vals.clear();
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

//! Process constructor for a combinational process with two inputs and one output
/*! similar to comb with two inputs
 */
template <typename T0, typename T1, typename T2>
class comb2 : public ut_process
{
public:
    UT_in<T1> iport1;        ///< port for the input channel 1
    UT_in<T2> iport2;        ///< port for the input channel 2
    UT_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(std::vector<T0>&,
                                const std::vector<T1>&,
                                const std::vector<T2>&
                                )> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb2(const sc_module_name& _name,      ///< process name
          const functype& _func,            ///< function to be passed
          const unsigned int& i1toks,      ///< consumption rate for the first input
          const unsigned int& i2toks       ///< consumption rate for the second input
          ) : ut_process(_name), iport1("iport1"), iport2("iport2"), oport1("oport1"),
              i1toks(i1toks), i2toks(i2toks), _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
        arg_vec.push_back(std::make_tuple("i1toks",std::to_string(i1toks)));
        arg_vec.push_back(std::make_tuple("i2toks",std::to_string(i2toks)));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::comb2";}
private:
    // consumption rates
    unsigned int i1toks, i2toks;
    
    // Inputs and output variables
    std::vector<T0> o1vals;
    std::vector<T1> i1vals;
    std::vector<T2> i2vals;
    
    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        i1vals.resize(i1toks);
        i2vals.resize(i2toks);
    }
    
    void prep()
    {
        for (auto it=i1vals.begin();it!=i1vals.end();it++)
            *it = iport1.read();
        for (auto it=i2vals.begin();it!=i2vals.end();it++)
            *it = iport2.read();
    }
    
    void exec()
    {
        _func(o1vals, i1vals, i2vals);
    }
    
    void prod()
    {
        write_vec_multiport(oport1, o1vals);
        o1vals.clear();
    }
    
    void clean() {}
    
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

//! Process constructor for a combinational process with two inputs and one output
/*! similar to comb with two inputs
 */
template <typename T0, typename T1, typename T2, typename T3>
class comb3 : public ut_process
{
public:
    UT_in<T1> iport1;        ///< port for the input channel 1
    UT_in<T2> iport2;        ///< port for the input channel 2
    UT_in<T3> iport3;        ///< port for the input channel 3
    UT_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(std::vector<T0>&,
                                const std::vector<T1>&,
                                const std::vector<T2>&,
                                const std::vector<T3>&
                                )> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb3(const sc_module_name& _name,      ///< process name
          const functype& _func,            ///< function to be passed
          const unsigned int& i1toks,      ///< consumption rate for the first input
          const unsigned int& i2toks,      ///< consumption rate for the second input
          const unsigned int& i3toks       ///< consumption rate for the third input
          ) : ut_process(_name), iport1("iport1"), iport2("iport2"), iport3("iport3"),
              oport1("oport1"),
              i1toks(i1toks), i2toks(i2toks), i3toks(i3toks), _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
        arg_vec.push_back(std::make_tuple("i1toks",std::to_string(i1toks)));
        arg_vec.push_back(std::make_tuple("i2toks",std::to_string(i2toks)));
        arg_vec.push_back(std::make_tuple("i3toks",std::to_string(i3toks)));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::comb3";}
private:
    // consumption rates
    unsigned int i1toks, i2toks, i3toks;
    
    // Inputs and output variables
    std::vector<T0> o1vals;
    std::vector<T1> i1vals;
    std::vector<T2> i2vals;
    std::vector<T3> i3vals;
    
    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        i1vals.resize(i1toks);
        i2vals.resize(i2toks);
        i3vals.resize(i3toks);
    }
    
    void prep()
    {
        for (auto it=i1vals.begin();it!=i1vals.end();it++)
            *it = iport1.read();
        for (auto it=i2vals.begin();it!=i2vals.end();it++)
            *it = iport2.read();
        for (auto it=i3vals.begin();it!=i3vals.end();it++)
            *it = iport3.read();
    }
    
    void exec()
    {
        _func(o1vals, i1vals, i2vals, i3vals);
    }
    
    void prod()
    {
        write_vec_multiport(oport1, o1vals);
        o1vals.clear();
    }
    
    void clean() {}
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(3);     // only one input port
        boundInChans[0].port = &iport1;
        boundInChans[1].port = &iport2;
        boundInChans[2].port = &iport3;
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

//! Process constructor for a combinational process with two inputs and one output
/*! similar to comb with two inputs
 */
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class comb4 : public ut_process
{
public:
    UT_in<T1> iport1;        ///< port for the input channel 1
    UT_in<T2> iport2;        ///< port for the input channel 2
    UT_in<T3> iport3;        ///< port for the input channel 3
    UT_in<T4> iport4;        ///< port for the input channel 4
    UT_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(std::vector<T0>&,
                                const std::vector<T1>&,
                                const std::vector<T2>&,
                                const std::vector<T3>&,
                                const std::vector<T4>&
                                )> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb4(const sc_module_name& _name,      ///< process name
          const functype& _func,            ///< function to be passed
          const unsigned int& i1toks,      ///< consumption rate for the first input
          const unsigned int& i2toks,      ///< consumption rate for the second input
          const unsigned int& i3toks,      ///< consumption rate for the third input
          const unsigned int& i4toks       ///< consumption rate for the forth input
          ) : ut_process(_name), iport1("iport1"), iport2("iport2"), iport3("iport3"),
              iport4("iport4"), oport1("oport1"),
              i1toks(i1toks), i2toks(i2toks), i3toks(i3toks),
              i4toks(i4toks), _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
        arg_vec.push_back(std::make_tuple("i1toks",std::to_string(i1toks)));
        arg_vec.push_back(std::make_tuple("i2toks",std::to_string(i2toks)));
        arg_vec.push_back(std::make_tuple("i3toks",std::to_string(i3toks)));
        arg_vec.push_back(std::make_tuple("i4toks",std::to_string(i4toks)));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::comb4";}
private:
    // consumption rates
    unsigned int i1toks, i2toks, i3toks, i4toks;
    
    // Inputs and output variables
    std::vector<T0> o1vals;
    std::vector<T1> i1vals;
    std::vector<T2> i2vals;
    std::vector<T3> i3vals;
    std::vector<T4> i4vals;
    
    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        i1vals.resize(i1toks);
        i2vals.resize(i2toks);
        i3vals.resize(i3toks);
        i4vals.resize(i4toks);
    }
    
    void prep()
    {
        for (auto it=i1vals.begin();it!=i1vals.end();it++)
            *it = iport1.read();
        for (auto it=i2vals.begin();it!=i2vals.end();it++)
            *it = iport2.read();
        for (auto it=i3vals.begin();it!=i3vals.end();it++)
            *it = iport3.read();
        for (auto it=i4vals.begin();it!=i4vals.end();it++)
            *it = iport4.read();
    }
    
    void exec()
    {
        _func(o1vals, i1vals, i2vals, i3vals, i4vals);
    }
    
    void prod()
    {
        write_vec_multiport(oport1, o1vals);
        o1vals.clear();
    }
    
    void clean() {}
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(4);     // only one input port
        boundInChans[0].port = &iport1;
        boundInChans[1].port = &iport2;
        boundInChans[2].port = &iport3;
        boundInChans[3].port = &iport4;
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

//! Process constructor for a delay element
/*! This class is used to build the most basic sequential process which
 * is a delay element. Given an initial value, it inserts this value at
 * the beginning of output stream and passes the rest of the inputs to
 * its output untouched. The class is parameterized for its input/output
 * data-type.
 * 
 * It is mandatory to include at least one delay element in all feedback
 * loops since combinational loops are forbidden in ForSyDe.
 */
template <class T>
class delay : public ut_process
{
public:
    UT_in<T>  iport1;       ///< port for the input channel
    UT_out<T> oport1;       ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial element, reads
     * data from its input port, and writes the results using the output
     * port.
     */
    delay(const sc_module_name& _name,     ///< process name
           const T& init_val                 ///< initial value
          ) : ut_process(_name), iport1("iport1"), oport1("oport1"),
              init_val(init_val)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::delay";}
    
private:
    // Initial value
    T init_val;
    
    // Inputs and output variables
    T* val;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new T;
        write_multiport(oport1, init_val);
    }
    
    void prep()
    {
        *val = iport1.read();
    }
    
    void exec() {}
    
    void prod()
    {
        write_multiport(oport1, *val);
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

//! Process constructor for a n-delay element
/*! This class is used to build a sequential process similar to dalay
 * but with an extra initial variable which sets the number of delay
 * elements (initial tokens). Given an initial value, it inserts the
 * initial value n times at the the beginning of output stream and
 * passes the rest of the inputs to its output untouched. The class is
 * parameterized for its input/output data-type.
 */
template <class T>
class delayn : public ut_process
{
public:
    UT_in<T>  iport1;       ///< port for the input channel
    UT_out<T> oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial elements,
     * reads data from its input port, and writes the results using the
     * output port.
     */
    delayn(const sc_module_name& _name,    ///< process name
            const T& init_val,               ///< initial value
            const unsigned int& n            ///< number of delay elements
          ) : ut_process(_name), iport1("iport1"), oport1("oport1"),
              init_val(init_val), ns(n)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
        arg_vec.push_back(std::make_tuple("n", std::to_string(n)));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::delayn";}
    
private:
    // Initial value
    T init_val;
    unsigned int ns;
    
    // Inputs and output variables
    T* val;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new T;
        for (unsigned int i=0; i<ns; i++)
            write_multiport(oport1, init_val);
    }
    
    void prep()
    {
        *val = iport1.read();
    }
    
    void exec() {}
    
    void prod()
    {
        write_multiport(oport1, *val);
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


//! Process constructor for a scan process
/*! This class is used to build a state machine which has its internal
 * state directly visible at the output.
 * Given an initial state, a next-state function, is applied iteratively
 * to compute the next state.
 */
template <class IT, class ST>
class scan : public ut_process
{
public:
    UT_in<IT>  iport1;        ///< port for the input channel
    UT_out<ST> oport1;        ///< port for the output channel
    
    //! Type of the partitioning function to be passed to the process constructor
    typedef std::function<void(unsigned int&, const ST&)> gamma_functype;
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(ST&, const ST&, const std::vector<IT>&)> ns_functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    scan(const sc_module_name& _name,   ///< The module name
         const gamma_functype& _gamma_func,///< The partitioning function
         const ns_functype& _ns_func, ///< The next_state function
         const ST& init_st  ///< Initial state
         ) : ut_process(_name), _gamma_func(_gamma_func), _ns_func(_ns_func),
             init_st(init_st)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_gamma_func",func_name+std::string("_gamma_func")));
        arg_vec.push_back(std::make_tuple("_ns_func",func_name+std::string("_ns_func")));
        std::stringstream ss;
        ss << init_st;
        arg_vec.push_back(std::make_tuple("init_st",ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "UT::scan";}
    
private:
    //! The functions passed to the process constructor
    gamma_functype _gamma_func;
    ns_functype _ns_func;
    // Initial state
    ST init_st;
        
    // Input, output, current state, and next state variables
    std::vector<IT> ivals;
    ST* stval;
    ST* nsval;

    //Implementing the abstract semantics
    void init()
    {
        stval = new ST;
        *stval = init_st;
        nsval = new ST;
    }
    
    void prep()
    {
        unsigned int itoks;
        _gamma_func(itoks, *stval);    // determine how many tokens to read
        ivals.resize(itoks);
        for (auto it=ivals.begin();it!=ivals.end();it++)
            *it = iport1.read();
    }
    
    void exec()
    {
        _ns_func(*nsval, *stval, ivals);
        *stval = *nsval;
    }
    
    void prod()
    {
        write_multiport(oport1, *stval);
    }
    
    void clean()
    {
        delete stval;
        delete nsval;
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

//! Process constructor for a scand process
/*! This class is used to build a state machine which has its internal
 * state directly visible at the output with a delay.
 * Given an initial state, a next-state function, is applied iteratively
 * to compute the next state.
 */
template <class IT, class ST>
class scand : public ut_process
{
public:
    UT_in<IT>  iport1;        ///< port for the input channel
    UT_out<ST> oport1;        ///< port for the output channel
    
    //! Type of the partitioning function to be passed to the process constructor
    typedef std::function<void(unsigned int&, const ST&)> gamma_functype;
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(ST&, const ST&, const std::vector<IT>&)> ns_functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    scand(const sc_module_name& _name,   ///< The module name
           const gamma_functype& _gamma_func,///< The partitioning function
           const ns_functype& _ns_func, ///< The next_state function
           const ST& init_st  ///< Initial state
           ) : ut_process(_name), _gamma_func(_gamma_func), _ns_func(_ns_func),
             init_st(init_st)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_gamma_func",func_name+std::string("_gamma_func")));
        arg_vec.push_back(std::make_tuple("_ns_func",func_name+std::string("_ns_func")));
        std::stringstream ss;
        ss << init_st;
        arg_vec.push_back(std::make_tuple("init_st",ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "UT::scan";}
    
private:
    //! The functions passed to the process constructor
    gamma_functype _gamma_func;
    ns_functype _ns_func;
    // Initial state
    ST init_st;
    
    bool first_run;
    
    // Input, output, current state, and next state variables
    std::vector<IT> ivals;
    ST* stval;
    ST* nsval;

    //Implementing the abstract semantics
    void init()
    {
        stval = new ST;
        *stval = init_st;
        nsval = new ST;
        // First evaluation cycle
        first_run = true;
    }
    
    void prep()
    {
        // We do not read anything in the first cycle since we can produce the output.
        if (!first_run)
        {
            unsigned int itoks;
            _gamma_func(itoks, *stval);    // determine how many tokens to read
            ivals.resize(itoks);
            for (auto it=ivals.begin();it!=ivals.end();it++)
                *it = iport1.read();
        }
    }
    
    void exec()
    {
        // Compute only the output in the first iteration.
        if (!first_run)
        {
            _ns_func(*nsval, *stval, ivals);
            *stval = *nsval;
        }
        else
        {
            first_run = false;
        }
    }
    
    void prod()
    {
        write_multiport(oport1, *stval);
    }
    
    void clean()
    {
        delete stval;
        delete nsval;
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

//! Process constructor for a Moore machine
/*! This class is used to build a finite state machine of type Moore.
 * Given an initial state, a next-state function, and an output decoding
 * function it creates a Moore process.
 */
template <class IT, class ST, class OT>
class moore : public ut_process
{
public:
    UT_in<IT>  iport1;        ///< port for the input channel
    UT_out<OT> oport1;        ///< port for the output channel
    
    //! Type of the partitioning function to be passed to the process constructor
    typedef std::function<void(unsigned int&, const ST&)> gamma_functype;
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(ST&, const ST&, const std::vector<IT>&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(std::vector<OT>&, const ST&)> od_functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    moore(const sc_module_name& _name,   ///< The module name
           const gamma_functype& _gamma_func,///< The partitioning function
           const ns_functype& _ns_func, ///< The next_state function
           const od_functype& _od_func, ///< The output-decoding function
           const ST& init_st  ///< Initial state
          ) : ut_process(_name), _gamma_func(_gamma_func), _ns_func(_ns_func),
              _od_func(_od_func), init_st(init_st)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_gamma_func",func_name+std::string("_gamma_func")));
        arg_vec.push_back(std::make_tuple("_ns_func",func_name+std::string("_ns_func")));
        arg_vec.push_back(std::make_tuple("_od_func",func_name+std::string("_od_func")));
        std::stringstream ss;
        ss << init_st;
        arg_vec.push_back(std::make_tuple("init_st",ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "UT::moore";}
    
private:
    //! The functions passed to the process constructor
    gamma_functype _gamma_func;
    ns_functype _ns_func;
    od_functype _od_func;
    // Initial state
    ST init_st;
    
    bool first_run;
    
    // Input, output, current state, and next state variables
    std::vector<IT> ivals;
    ST* stval;
    ST* nsval;
    std::vector<OT> ovals;

    //Implementing the abstract semantics
    void init()
    {
        stval = new ST;
        *stval = init_st;
        nsval = new ST;
        // First evaluation cycle
        first_run = true;
    }
    
    void prep()
    {
        // We do not read anything in the first cycle since we can produce the output.
        if (!first_run)
        {
            unsigned int itoks;
            _gamma_func(itoks, *stval);    // determine how many tokens to read
            ivals.resize(itoks);
            for (auto it=ivals.begin();it!=ivals.end();it++)
                *it = iport1.read();
        }
    }
    
    void exec()
    {
        // Compute only the output in the first iteration.
        if (!first_run)
        {
            _ns_func(*nsval, *stval, ivals);
            _od_func(ovals, *stval);
            *stval = *nsval;
        }
        else
        {
            first_run = false;
            _od_func(ovals, *stval);
        }
    }
    
    void prod()
    {
        write_vec_multiport(oport1, ovals);
        ovals.clear();
    }
    
    void clean()
    {
        delete stval;
        delete nsval;
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

//! Process constructor for a Moore machine
/*! This class is used to build a finite state machine of type Moore.
 * Given an initial state, a next-state function, and an output decoding
 * function it creates a Mealy process.
 */
template<typename TO_tuple, typename TI_tuple, typename TS_tuple> class mooreMN;

template <typename... TOs, typename... TIs, typename... TSs>
class mooreMN<std::tuple<TOs...>,std::tuple<TIs...>,std::tuple<TSs...>>: public ut_process
{
public:
    std::tuple<UT_in<TIs>...>  iport;///< tuple of ports for the input channels
    std::tuple<UT_out<TOs>...> oport;///< tuple of ports for the output channels
    
    //! Type of the partitioning function to be passed to the process constructor
    typedef std::function<void(std::array<size_t, sizeof...(TIs)>&,
                                const std::tuple<TSs...>&)> gamma_functype;
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(std::tuple<TSs...>&,
                                const std::tuple<TSs...>&,
                                const std::tuple<std::vector<TIs>...>&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(std::tuple<std::vector<TOs>...>&,
                                const std::tuple<TSs...>&)> od_functype;
    
    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    mooreMN(const sc_module_name& _name,        ///< The module name
            const gamma_functype& _gamma_func,  ///< The partitioning function
            const ns_functype& _ns_func,        ///< The next_state function
            const od_functype& _od_func,        ///< The output-decoding function
            const std::tuple<TSs...>& init_st   ///< Initial state
            ) : ut_process(_name), _gamma_func(_gamma_func), _ns_func(_ns_func),
              _od_func(_od_func), init_st(init_st)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_gamma_func",func_name+std::string("_gamma_func")));
        arg_vec.push_back(std::make_tuple("_ns_func",func_name+std::string("_ns_func")));
        arg_vec.push_back(std::make_tuple("_od_func",func_name+std::string("_od_func")));
        std::stringstream ss;
        ss << init_st;
        arg_vec.push_back(std::make_tuple("init_st",ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "UT::mooreMN";}
    
private:
    //! The functions passed to the process constructor
    gamma_functype _gamma_func;
    ns_functype _ns_func;
    od_functype _od_func;
    // Initial value
    std::tuple<TSs...> init_st;
    // consumption rates
    std::array<size_t, sizeof...(TOs)> otoks;
    std::array<size_t, sizeof...(TIs)> itoks;

    bool first_run;
    
    // Input, output, current state, and next state variables
    std::tuple<std::vector<TOs>...>* ovals;
    std::tuple<TSs...>* stvals;
    std::tuple<TSs...>* nsvals;
    std::tuple<std::vector<TIs>...>* ivals;

    //Implementing the abstract semantics
    void init()
    {
        ovals = new std::tuple<std::vector<TOs>...>;
        stvals = new std::tuple<TSs...>;
        *stvals = init_st;
        nsvals = new std::tuple<TSs...>;
        ivals = new std::tuple<std::vector<TIs>...>;
        // First evaluation cycle
        first_run = true;
    }
    
    void prep()
    {
        if (!first_run)
        {
            _gamma_func(itoks, *stvals);    // determine how many tokens to read
            // Size the input buffers
            std::apply([&](auto&... ival) {
                std::apply([&](auto&... itok) {
                    (ival.resize(itok), ...);
                }, itoks);
            }, *ivals);
            // Read the input tokens
            std::apply([&](auto&... inport) {
                std::apply([&](auto&... ival) {
                    (
                        [&ival,&inport](){
                            for (auto it=ival.begin();it!=ival.end();it++)
                                *it = inport.read();
                        }()
                    , ...);
                }, *ivals);
            }, iport);
        }
    }
    
    void exec()
    {
        if (!first_run)
        {
            _ns_func(*nsvals, *stvals, *ivals);
            _od_func(*ovals, *stvals);
            *stvals = *nsvals;
        }
        else
        {
            first_run = false;
            _od_func(*ovals, *stvals);
        }
    }
    
    void prod()
    {
        std::apply([&](auto&&... port){
            std::apply([&](auto&&... val){
                (write_vec_multiport(port, val), ...);
                (val.clear(), ...);
            }, *ovals);
        }, oport);
    }
    
    void clean()
    {
        delete ivals;
        delete ovals;
        delete stvals;
        delete nsvals;
    }
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(sizeof...(TIs));     // input ports
        std::apply
        (
            [&](auto&... ports)
            {
                std::size_t n{0};
                ((boundInChans[n++].port = &ports),...);
            }, iport
        );
        boundOutChans.resize(sizeof...(TOs));    // output ports
        std::apply
        (
            [&](auto&... ports)
            {
                std::size_t n{0};
                ((boundOutChans[n++].port = &ports),...);
            }, oport
        );
    }
#endif
};

//! Process constructor for a Mealy machine
/*! This class is used to build a finite state machine of type Mealy.
 * Given an initial state, a next-state function, and an output decoding
 * function it creates a Mealy process.
 */
template <class IT, class ST, class OT>
class mealy : public ut_process
{
public:
    UT_in<IT>  iport1;        ///< port for the input channel
    UT_out<OT> oport1;        ///< port for the output channel
    
    //! Type of the partitioning function to be passed to the process constructor
    typedef std::function<void(unsigned int&, const ST&)> gamma_functype;
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(ST&, const ST&, const std::vector<IT>&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(std::vector<OT>&, const ST&,
                                 const std::vector<IT>&)> od_functype;
    
    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    mealy(const sc_module_name& _name,   ///< The module name
           const gamma_functype& _gamma_func,///< The partitioning function
           const ns_functype& _ns_func, ///< The next_state function
           const od_functype& _od_func, ///< The output-decoding function
           const ST& init_st  ///< Initial state
          ) : ut_process(_name), _gamma_func(_gamma_func), _ns_func(_ns_func),
              _od_func(_od_func), init_st(init_st)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_gamma_func",func_name+std::string("_gamma_func")));
        arg_vec.push_back(std::make_tuple("_ns_func",func_name+std::string("_ns_func")));
        arg_vec.push_back(std::make_tuple("_od_func",func_name+std::string("_od_func")));
        std::stringstream ss;
        ss << init_st;
        arg_vec.push_back(std::make_tuple("init_st",ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "UT::mealy";}
    
private:
    //! The functions passed to the process constructor
    gamma_functype _gamma_func;
    ns_functype _ns_func;
    od_functype _od_func;
    // Initial value
    ST init_st;
    
    // Input, output, current state, and next state variables
    std::vector<IT> ivals;
    ST* stval;
    ST* nsval;
    std::vector<OT> ovals;

    //Implementing the abstract semantics
    void init()
    {
        stval = new ST;
        *stval = init_st;
        nsval = new ST;
    }
    
    void prep()
    {
        unsigned int itoks;
        _gamma_func(itoks, *stval);    // determine how many tokens to read
        ivals.resize(itoks);
        for (auto it=ivals.begin();it!=ivals.end();it++)
            *it = iport1.read();
    }
    
    void exec()
    {
        _ns_func(*nsval, *stval, ivals);
        _od_func(ovals, *stval, ivals);
        *stval = *nsval;
    }
    
    void prod()
    {
        write_vec_multiport(oport1, ovals);
        ovals.clear();
    }
    
    void clean()
    {
        delete stval;
        delete nsval;
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
template<typename TO_tuple, typename TI_tuple, typename TS_tuple> class mealyMN;

template <typename... TOs, typename... TIs, typename... TSs>
class mealyMN<std::tuple<TOs...>,std::tuple<TIs...>,std::tuple<TSs...>>: public ut_process
{
public:
    std::tuple<UT_in<TIs>...>  iport;///< tuple of ports for the input channels
    std::tuple<UT_out<TOs>...> oport;///< tuple of ports for the output channels
    
    //! Type of the partitioning function to be passed to the process constructor
    typedef std::function<void(std::array<size_t, sizeof...(TIs)>&,
                                const std::tuple<TSs...>&)> gamma_functype;
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(std::tuple<TSs...>&,
                                const std::tuple<TSs...>&,
                                const std::tuple<std::vector<TIs>...>&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(std::tuple<std::vector<TOs>...>&,
                                const std::tuple<TSs...>&,
                                const std::tuple<std::vector<TIs>...>&)> od_functype;
    
    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    mealyMN(const sc_module_name& _name,        ///< The module name
            const gamma_functype& _gamma_func,  ///< The partitioning function
            const ns_functype& _ns_func,        ///< The next_state function
            const od_functype& _od_func,        ///< The output-decoding function
            const std::tuple<TSs...>& init_st   ///< Initial state
            ) : ut_process(_name), _gamma_func(_gamma_func), _ns_func(_ns_func),
              _od_func(_od_func), init_st(init_st)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_gamma_func",func_name+std::string("_gamma_func")));
        arg_vec.push_back(std::make_tuple("_ns_func",func_name+std::string("_ns_func")));
        arg_vec.push_back(std::make_tuple("_od_func",func_name+std::string("_od_func")));
        std::stringstream ss;
        ss << init_st;
        arg_vec.push_back(std::make_tuple("init_st",ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "UT::mealyMN";}
    
private:
    //! The functions passed to the process constructor
    gamma_functype _gamma_func;
    ns_functype _ns_func;
    od_functype _od_func;
    // Initial value
    std::tuple<TSs...> init_st;
    // consumption rates
    std::array<size_t, sizeof...(TOs)> otoks;
    std::array<size_t, sizeof...(TIs)> itoks;
    
    // Input, output, current state, and next state variables
    std::tuple<std::vector<TOs>...>* ovals;
    std::tuple<TSs...>* stvals;
    std::tuple<TSs...>* nsvals;
    std::tuple<std::vector<TIs>...>* ivals;

    //Implementing the abstract semantics
    void init()
    {
        ovals = new std::tuple<std::vector<TOs>...>;
        stvals = new std::tuple<TSs...>;
        *stvals = init_st;
        nsvals = new std::tuple<TSs...>;
        ivals = new std::tuple<std::vector<TIs>...>;
    }
    
    void prep()
    {
        _gamma_func(itoks, *stvals);    // determine how many tokens to read
        // Size the input buffers
        std::apply([&](auto&... ival) {
            std::apply([&](auto&... itok) {
                (ival.resize(itok), ...);
            }, itoks);
        }, *ivals);
        // Read the input tokens
        std::apply([&](auto&... inport) {
            std::apply([&](auto&... ival) {
                (
                    [&ival,&inport](){
                        for (auto it=ival.begin();it!=ival.end();it++)
                            *it = inport.read();
                    }()
                , ...);
            }, *ivals);
        }, iport);
    }
    
    void exec()
    {
        _ns_func(*nsvals, *stvals, *ivals);
        _od_func(*ovals, *stvals, *ivals);
        *stvals = *nsvals;
    }
    
    void prod()
    {
        std::apply([&](auto&&... port){
            std::apply([&](auto&&... val){
                (write_vec_multiport(port, val), ...);
                (val.clear(), ...);
            }, *ovals);
        }, oport);
    }
    
    void clean()
    {
        delete ivals;
        delete ovals;
        delete stvals;
        delete nsvals;
    }
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(sizeof...(TIs));     // input ports
        std::apply
        (
            [&](auto&... ports)
            {
                std::size_t n{0};
                ((boundInChans[n++].port = &ports),...);
            }, iport
        );
        boundOutChans.resize(sizeof...(TOs));    // output ports
        std::apply
        (
            [&](auto&... ports)
            {
                std::size_t n{0};
                ((boundOutChans[n++].port = &ports),...);
            }, oport
        );
    }
#endif
};

//! Process constructor for a constant source process
/*! This class is used to build a souce process with constant output.
 * Its main purpose is to be used in test-benches.
 * 
 * This class can directly be instantiated to build a process.
 */
template <class T>
class constant : public ut_process
{
public:
    UT_out<T> oport1;            ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    constant(const sc_module_name& _name,      ///< The module name
              const T& init_val,                ///< The constant output value
              const unsigned long long& take=0 ///< number of tokens produced (0 for infinite)
             ) : ut_process(_name), oport1("oport1"),
                 init_val(init_val), take(take)
                 
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
        arg_vec.push_back(std::make_tuple("take", std::to_string(take)));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::constant";}
    
private:
    T init_val;
    unsigned long long take;    // Number of tokens produced
    
    unsigned long long tok_cnt;
    bool infinite;
    
    //Implementing the abstract semantics
    void init()
    {
        if (take==0) infinite = true;
        tok_cnt = 0;
    }
    
    void prep() {}
    
    void exec() {}
    
    void prod()
    {
        if (tok_cnt++ < take || infinite)
            write_multiport(oport1, init_val);
        else wait();
    }
    
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
 * Given an initial state and a function, the process repeatedly applies
 * the function to the current state to produce next state, which is
 * also the process output. It can be used in test-benches.
 */
template <class T>
class source : public ut_process
{
public:
    UT_out<T> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(T&, const T&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    source(const sc_module_name& _name,   ///< The module name
            const functype& _func,         ///< function to be passed
            const T& init_val,              ///< Initial state
            const unsigned long long& take=0 ///< number of tokens produced (0 for infinite)
          ) : ut_process(_name), oport1("oport1"),
              init_st(init_val), take(take), _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
        arg_vec.push_back(std::make_tuple("take", std::to_string(take)));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::source";}
    
private:
    T init_st;        // The current state
    unsigned long long take;    // Number of tokens produced
    
    T* cur_st;        // The current state of the process
    unsigned long long tok_cnt;
    bool infinite;
    
    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        cur_st = new T;
        *cur_st = init_st;
        write_multiport(oport1, *cur_st);
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
            write_multiport(oport1, *cur_st);
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
    vsource(const sc_module_name& _name,           ///< The module name
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
            write_multiport(oport1,out_val);    // write to the output
        }
    }
};

//! Process constructor for a sink process
/*! This class is used to build a sink process which only has an input.
 * Its main purpose is to be used in test-benches. The process repeatedly
 * applies a given function to the current input.
 */
template <class T>
class sink : public ut_process
{
public:
    UT_in<T> iport1;         ///< port for the input channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(const T&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    sink(const sc_module_name& _name,      ///< process name
          const functype& _func             ///< function to be passed
        ) : ut_process(_name), iport1("iport1"), _func(_func)
            
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::sink";}
    
private:
    T* val;         // The current state of the process

    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new T;
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

//! The zips process with two inputs and one output
/*! This process "zips" two incoming signals into one signal of tuples.
 */
template <class T1, class T2>
class zips : public ut_process
{
public:
    UT_in<T1> iport1;        ///< port for the input channel 1
    UT_in<T2> iport2;        ///< port for the input channel 2
    UT_out<std::tuple<std::vector<T1>,std::vector<T2>>> oport1;///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    zips(const sc_module_name& _name,
        const unsigned int& i1toks,
        const unsigned int& i2toks
        ) : ut_process(_name), iport1("iport1"), iport2("iport2"), oport1("oport1"),
            i1toks(i1toks), i2toks(i2toks)
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::zips";}
    
private:
    unsigned int i1toks, i2toks;
    // intermediate values
    std::vector<T1> i1vals;
    std::vector<T2> i2vals;
    
    void init()
    {
        i1vals.resize(i1toks);
        i2vals.resize(i2toks);
    }
    
    void prep()
    {
        for (auto it=i1vals.begin();it!=i1vals.end();it++)
            *it = iport1.read();
        for (auto it=i2vals.begin();it!=i2vals.end();it++)
            *it = iport2.read();
    }
    
    void exec() {}
    
    void prod()
    {
        write_multiport(oport1,std::make_tuple(i1vals,i2vals));  // write to the output
    }
    
    void clean() {}
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(2);     // two input ports
        boundInChans[0].port = &iport1;
        boundInChans[1].port = &iport2;
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

//! The zips process with variable number of inputs and one output
/*! This process "zips" the incoming signals into one signal of tuples.
 */
template <class... Ts>
class zipsN : public ut_process
{
public:
    std::tuple <UT_in<Ts>...> iport;                ///< tuple of ports for the input channels
    UT_out<std::tuple<std::vector<Ts>...> > oport1; ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    zipsN(const sc_module_name& _name,                  ///< process name
            std::array<size_t, sizeof...(Ts)> in_toks   ///< consumption rates for the inputs
            ) : ut_process(_name), in_toks(in_toks), oport1("iport1")
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << in_toks;
        arg_vec.push_back(std::make_tuple("itoks",ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::zipsN";}
private:
    std::array<size_t, sizeof...(Ts)> in_toks;
    // intermediate values
    std::tuple<std::vector<Ts>...>* in_val;
    
    void init()
    {
        in_val = new std::tuple<std::vector<Ts>...>;
        std::apply([&](auto&... ival) {
            std::apply([&](auto&... itok) {
                (ival.resize(itok), ...);
            }, in_toks);
        }, *in_val);
    }
    
    void prep()
    {
        std::apply([&](auto&... inport) {
            std::apply([&](auto&... ival) {
                (
                    [&ival,&inport](){
                        for (auto it=ival.begin();it!=ival.end();it++)
                            *it = inport.read();
                    }()
                , ...);
            }, *in_val);
        }, iport);
    }
    
    void exec() {}
    
    void prod()
    {
        write_multiport(oport1,*in_val);    // write to the output;
    }
    
    void clean()
    {
        delete in_val;
    }
 
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(sizeof...(Ts));    // two output ports
        std::apply
        (
            [&](auto&... ports)
            {
                std::size_t n{0};
                ((boundInChans[n++].port = &ports),...);
            }, iport
        );
        boundOutChans.resize(1);     // only one input port
        boundInChans[0].port = &oport1;
    }
#endif

};

//! The unzip process with one input and two outputs
/*! This process "unzips" a signal of tuples into two separate signals
 */
template <class T1, class T2>
class unzip : public ut_process
{
public:
    UT_in<std::tuple<std::vector<T1>,std::vector<T2>>> iport1;///< port for the input channel
    UT_out<T1> oport1;        ///< port for the output channel 1
    UT_out<T2> oport2;        ///< port for the output channel 2

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * unzips them and writes the results using the output ports
     */
    unzip(const sc_module_name& _name
           ) : ut_process(_name),
               iport1("iport1"), oport1("oport1"), oport2("oport2") {}
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::unzip";}
private:    
    // intermediate values
    std::tuple<std::vector<T1>,std::vector<T2>>* in_val;
    
    void init()
    {
        in_val = new std::tuple<std::vector<T1>,std::vector<T2>>;
    }
    
    void prep()
    {
        *in_val = iport1.read();
    }
    
    void exec() {}
    
    void prod()
    {
        
        write_vec_multiport(oport1,std::get<0>(*in_val));  // write to the output 1
        write_vec_multiport(oport2,std::get<1>(*in_val));  // write to the output 2
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
        boundOutChans.resize(2);    // two output ports
        boundOutChans[0].port = &oport1;
        boundOutChans[1].port = &oport2;
    }
#endif
};

//! The unzip process with one input and variable number of outputs
/*! This process "unzips" the incoming signal into a tuple of signals.
 */
template <class... Ts>
class unzipN : public ut_process
{
public:
    UT_in<std::tuple<std::vector<Ts>...>> iport1;///< port for the input channel
    std::tuple<UT_out<Ts>...> oport;///< tuple of ports for the output channels

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * unzips it and writes the results using the output ports
     */
    unzipN(const sc_module_name& _name
            ) : ut_process(_name), iport1("iport1") {}
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::unzipN";}
private:
    // intermediate values
    std::tuple<std::vector<Ts>...>* in_val;
    
    void init()
    {
        in_val = new std::tuple<std::vector<Ts>...>;
    }
    
    void prep()
    {
        *in_val = iport1.read();
    }
    
    void exec() {}
    
    void prod()
    {
        std::apply([&](auto&&... port){
            std::apply([&](auto&&... val){
                (write_vec_multiport(port, val), ...);
            }, *in_val);
        }, oport);
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
        boundOutChans.resize(sizeof...(Ts));    // two output ports
        std::apply
        (
            [&](auto&... ports)
            {
                std::size_t n{0};
                ((boundOutChans[n++].port = &ports),...);
            }, oport
        );
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
class fanout : public ut_process
{
public:
    UT_in<T> iport1;        ///< port for the input channel
    UT_out<T> oport1;       ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies and writes the results using the output port
     */
    fanout(const sc_module_name& _name)  // module name
         : ut_process(_name) { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::fanout";}
    
private:
    // Inputs and output variables
    T* val;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new T;
    }
    
    void prep()
    {
        *val = iport1.read();
    }
    
    void exec() {}
    
    void prod()
    {
        write_multiport(oport1, *val);
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

//! The zipU process with two inputs and one output
/*! This process "zips" the incoming two signals into one signal of tuples with using control signal
 */
 
template <class T1, class T2, class TCS>
class zipU : public ut_process
{
public:
    UT_in<T1> iport1;                                              ///< port for the input channel 1
    UT_in<T2> iport2;                                             ///< port for the input channel 2
    UT_in<TCS> controlport;                                      ///< port for the control signal   
    UT_out<std::tuple<std::vector<T1>,std::vector<T2>>> oport1; ///< port for the output channel

    typedef std::function<size_t (const TCS&)> gamma_functype;


    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    zipU(const sc_module_name& _name,
        const gamma_functype& _gamma_func_a,
        const gamma_functype& _gamma_func_b
        ) : ut_process(_name), iport1("iport1"), iport2("iport2"), controlport("controlport"), oport1("oport1"),
        _gamma_func_a(_gamma_func_a), _gamma_func_b(_gamma_func_b) 
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "UT::zipU";}
    
private:

    gamma_functype _gamma_func_a, _gamma_func_b;

    // intermediate values
    std::vector<T1> i1vals;
    std::vector<T2> i2vals;
    TCS control_tkn; 

    void init()
    {

    }
    
    void prep()
    {
        control_tkn = controlport.read();
        size_t c1 = _gamma_func_a(control_tkn);
        size_t c2 = _gamma_func_b(control_tkn);
        
        i1vals.resize(c1);
        i2vals.resize(c2);

        for (auto it=i1vals.begin();it!=i1vals.end();it++)
            *it = iport1.read();
        for (auto it=i2vals.begin();it!=i2vals.end();it++)
            *it = iport2.read();
    }
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1,std::make_tuple(i1vals,i2vals))  // write to the output
    }
    
    void clean() {}
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(2);     // two input ports
        boundInChans[0].port = &iport1;
        boundInChans[1].port = &iport2;
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};


}
}

#endif
