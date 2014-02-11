/**********************************************************************           
    * sy_process_constructors.hpp -- Process constructors in the SY   *
    *                                MOC                              *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing basic process constructors for modeling      *
    *          synchronous systems in ForSyDe-SystemC                 *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SY_PROCESS_CONSTRUCTORS_HPP
#define SY_PROCESS_CONSTRUCTORS_HPP

/*! \file sy_process_constructors.hpp
 * \brief Implements the basic process constructors in the SY MoC
 * 
 *  This file includes the basic process constructors used for modeling
 * in the synchronous model of computation.
 */

#include <functional>
#include <tuple>
#include <array>
#include <algorithm>

#include "abst_ext.hpp"
#include "sy_process.hpp"

namespace ForSyDe
{

namespace SY
{

using namespace sc_core;

//! Process constructor for a combinational process with one input and one output
/*! This class is used to build combinational processes with one input
 * and one output. The class is parameterized for input and output
 * data-types.
 */
template <typename T0, typename T1>
class comb : public sy_process
{
public:
    SY_in<T1>  iport1;       ///< port for the input channel
    SY_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(abst_ext<T0>&,const abst_ext<T1>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    comb(const sc_module_name& _name,      ///< process name
         const functype& _func             ///< function to be passed
         ) : sy_process(_name), iport1("iport1"), oport1("oport1"),
             _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::comb";}

private:
    // Inputs and output variables
    abst_ext<T0>* oval;
    abst_ext<T1>* ival1;
    
    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        oval = new abst_ext<T0>;
        ival1 = new abst_ext<T1>;
    }
    
    void prep()
    {
        *ival1 = iport1.read();
    }
    
    void exec()
    {
        _func(*oval, *ival1);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *oval)
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
class comb2 : public sy_process
{
public:
    SY_in<T1> iport1;        ///< port for the input channel 1
    SY_in<T2> iport2;        ///< port for the input channel 2
    SY_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(abst_ext<T0>&, const abst_ext<T1>&,
                                              const abst_ext<T2>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb2(const sc_module_name& _name,      ///< process name
           const functype& _func             ///< function to be passed
          ) : sy_process(_name), iport1("iport1"), iport2("iport2"), oport1("oport1"),
              _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::comb2";}
private:
    // Inputs and output variables
    abst_ext<T0>* oval;
    abst_ext<T1>* ival1;
    abst_ext<T2>* ival2;
    
    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        oval = new abst_ext<T0>;
        ival1 = new abst_ext<T1>;
        ival2 = new abst_ext<T2>;
    }
    
    void prep()
    {
        *ival1 = iport1.read();
        *ival2 = iport2.read();
    }
    
    void exec()
    {
        _func(*oval, *ival1, *ival2);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *oval)
    }
    
    void clean()
    {
        delete ival2;
        delete ival1;
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

//! Process constructor for a combinational process with three inputs and one output
/*! similar to comb with three inputs
 */
template <typename T0, typename T1, typename T2, typename T3>
class comb3 : public sy_process
{
public:
    SY_in<T1> iport1;        ///< port for the input channel 1
    SY_in<T2> iport2;        ///< port for the input channel 2
    SY_in<T3> iport3;        ///< port for the input channel 3
    SY_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(abst_ext<T0>&, const abst_ext<T1>&,
                                              const abst_ext<T2>&,
                                              const abst_ext<T3>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb3(const sc_module_name& _name,      ///< process name
           const functype& _func             ///< function to be passed
          ) : sy_process(_name), iport1("iport1"), iport2("iport2"), iport3("iport3"), 
              oport1("oport1"), _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::comb3";}
    
private:
    // Inputs and output variables
    abst_ext<T0>* oval;
    abst_ext<T1>* ival1;
    abst_ext<T2>* ival2;
    abst_ext<T3>* ival3;

    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        oval = new abst_ext<T0>;
        ival1 = new abst_ext<T1>;
        ival2 = new abst_ext<T2>;
        ival3 = new abst_ext<T3>;
    }
    
    void prep()
    {
        *ival1 = iport1.read();
        *ival2 = iport2.read();
        *ival3 = iport3.read();
    }
    
    void exec()
    {
        _func(*oval, *ival1, *ival2, *ival3);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *oval)
    }
    
    void clean()
    {
        delete ival3;
        delete ival2;
        delete ival1;
        delete oval;
    }
    
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

//! Process constructor for a combinational process with four inputs and one output
/*! similar to comb with four inputs
 */
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class comb4 : public sy_process
{
public:
    SY_in<T1> iport1;       ///< port for the input channel 1
    SY_in<T2> iport2;       ///< port for the input channel 2
    SY_in<T3> iport3;       ///< port for the input channel 3
    SY_in<T4> iport4;       ///< port for the input channel 4
    SY_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(abst_ext<T0>&, const abst_ext<T1>&,
                                             const abst_ext<T2>&,
                                             const abst_ext<T3>&,
                                             const abst_ext<T4>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb4(const sc_module_name& _name,      ///< process name
           const functype& _func             ///< function to be passed
          ) : sy_process(_name), iport1("iport1"), iport2("iport2"), 
              iport3("iport3"), iport4("iport4"), _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "SY::comb4";}
    
private:
    // Inputs and output variables
    abst_ext<T0>* oval;
    abst_ext<T1>* ival1;
    abst_ext<T2>* ival2;
    abst_ext<T3>* ival3;
    abst_ext<T4>* ival4;
    
    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        oval = new abst_ext<T0>;
        ival1 = new abst_ext<T1>;
        ival2 = new abst_ext<T2>;
        ival3 = new abst_ext<T3>;
        ival4 = new abst_ext<T4>;
    }
    
    void prep()
    {
        *ival1 = iport1.read();
        *ival2 = iport2.read();
        *ival3 = iport3.read();
        *ival4 = iport4.read();
    }
    
    void exec()
    {
        _func(*oval, *ival1, *ival2, *ival3, *ival4);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *oval)
    }
    
    void clean()
    {
        delete ival4;
        delete ival3;
        delete ival2;
        delete ival1;
        delete oval;
    }
    
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

//! Process constructor for a combinational process with an array of inputs and one output
/*! similar to comb with an array of inputs
 */
template <typename T0, typename T1, std::size_t N>
class combX : public sy_process
{
public:
    std::array<SY_in<T1>,N> iport;       ///< port for the input channel 1
    SY_out<T0> oport1;        ///< port for the output channel

    //! Type of the function to be passed to the process constructor
    typedef std::function<void(abst_ext<T0>&, const std::array<abst_ext<T1>,N>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    combX(const sc_module_name& _name,      ///< process name
           const functype& _func             ///< function to be passed
          ) : sy_process(_name), _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }

    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "SY::combX";}

private:
    // Inputs and output variables
    abst_ext<T0>* oval;
    std::array<abst_ext<T1>,N> ival;

    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        oval = new abst_ext<T0>;
    }

    void prep()
    {
    	for (unsigned int i=0; i<N; i++)
    		ival[i] = iport[i].read();
    }

    void exec()
    {
        _func(*oval, ival);
    }

    void prod()
    {
        WRITE_MULTIPORT(oport1, *oval)
    }

    void clean()
    {
        delete oval;
    }

#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(N);     // only one input port
        for (unsigned int i=0; i<N; i++)
        	boundInChans[i].port = &iport[i];
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
class delay : public sy_process
{
public:
    SY_in<T>  iport1;       ///< port for the input channel
    SY_out<T> oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial element, reads
     * data from its input port, and writes the results using the output
     * port.
     */
    delay(const sc_module_name& _name,      ///< process name
           const abst_ext<T>& init_val      ///< initial value
          ) : sy_process(_name), iport1("iport1"), oport1("oport1"),
              init_val(init_val)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::delay";}
    
private:
    // Initial value
    abst_ext<T> init_val;
    
    // Inputs and output variables
    abst_ext<T>* val;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new abst_ext<T>;
        WRITE_MULTIPORT(oport1, init_val)
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
class delayn : public sy_process
{
public:
    SY_in<T>  iport1;       ///< port for the input channel
    SY_out<T> oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial elements,
     * reads data from its input port, and writes the results using the
     * output port.
     */
    delayn(const sc_module_name& _name,      ///< process name
            const abst_ext<T>& init_val,    ///< initial value
            const unsigned int& n            ///< number of delay elements
          ) : sy_process(_name), iport1("iport1"), oport1("oport1"),
              init_val(init_val), ns(n)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
        ss.str("");
        ss << n;
        arg_vec.push_back(std::make_tuple("n", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::delayn";}
    
private:
    // Initial value
    abst_ext<T> init_val;
    unsigned int ns;
    
    // Inputs and output variables
    abst_ext<T>* val;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new abst_ext<T>;
        for (int i=0; i<ns; i++)
            WRITE_MULTIPORT(oport1, init_val)
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
class moore : public sy_process
{
public:
    SY_in<IT>  iport1;        ///< port for the input channel
    SY_out<OT> oport1;        ///< port for the output channel
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(ST&, const ST&, const abst_ext<IT>&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(abst_ext<OT>&, const ST&)> od_functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    moore(const sc_module_name& _name,      ///< process name
           const ns_functype& _ns_func, ///< The next_state function
           const od_functype& _od_func, ///< The output-decoding function
           const ST& init_st  ///< Initial state
          ) : sy_process(_name), _ns_func(_ns_func), _od_func(_od_func),
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
    std::string forsyde_kind() const{return "SY::moore";}
    
private:
    // Initial value
    ST init_st;
    
    //! The functions passed to the process constructor
    ns_functype _ns_func;
    od_functype _od_func;
    
    bool first_run;
    
    // Input, output, current state, and next state variables
    abst_ext<IT>* ival;
    ST* stval;
    ST* nsval;
    abst_ext<OT>* oval;

    //Implementing the abstract semantics
    void init()
    {
        ival = new abst_ext<IT>;
        stval = new ST;
        *stval = init_st;
        nsval = new ST;
        oval = new abst_ext<OT>;
        // First evaluation cycle
        first_run = true;
    }
    
    void prep()
    {
        if (first_run)
            first_run = false;
        else
            *ival = iport1.read();
    }
    
    void exec()
    {
        _ns_func(*nsval, *stval, *ival);
        _od_func(*oval, *stval);
        *stval = *nsval;
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *oval)
    }
    
    void clean()
    {
        delete ival;
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

//! Process constructor for a Mealy machine
/*! This class is used to build a finite state machine of type Mealy.
 * Given an initial state, a next-state function, and an output decoding
 * function it creates a Mealy process.
 */
template <class IT, class ST, class OT>
class mealy : public sy_process
{
public:
    SY_in<IT>  iport1;        ///< port for the input channel
    SY_out<OT> oport1;        ///< port for the output channel
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(ST&, const ST&, 
                                                const abst_ext<IT>&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(abst_ext<OT>&, const ST&,
                                                const abst_ext<IT>&)> od_functype;
    
    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    mealy(const sc_module_name& _name,      ///< process name
           const ns_functype& _ns_func, ///< The next_state function
           const od_functype& _od_func, ///< The output-decoding function
           const ST& init_st  ///< Initial state
          ) : sy_process(_name), _ns_func(_ns_func), _od_func(_od_func),
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
    std::string forsyde_kind() const{return "SY::mealy";}
    
private:
    //! The functions passed to the process constructor
    ns_functype _ns_func;
    od_functype _od_func;
    // Initial value
    ST init_st;
    
    // Input, output, current state, and next state variables
    abst_ext<IT>* ival;
    ST* stval;
    ST* nsval;
    abst_ext<OT>* oval;

    //Implementing the abstract semantics
    void init()
    {
        ival = new abst_ext<IT>;
        stval = new ST;
        *stval = init_st;
        nsval = new ST;
        oval = new abst_ext<OT>;
    }
    
    void prep()
    {
        *ival = iport1.read();
    }
    
    void exec()
    {
        _ns_func(*nsval, *stval, *ival);
        _od_func(*oval, *stval, *ival);
        *stval = *nsval;
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *oval)
    }
    
    void clean()
    {
        delete ival;
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

//! Process constructor for a fill process
/*! The process constructor fill creates a process that fills an absent-
 * extended signal with present values by replacing absent values with a
 * given value.
 */
template <class T>
class fill : public sy_process
{
public:
    SY_in<T> iport1;              ///< port for the input channel
    SY_out<T> oport1;             ///< port for the output channel

    //! The constructor requires the process name and a default value
    /*! It creates an SC_THREAD which fills the signal result using the
     * output port
     */
    fill(const sc_module_name& _name,      ///< process name
          const T& def_val                  ///< default value
         ) : sy_process(_name), def_val(def_val)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << def_val;
        arg_vec.push_back(std::make_tuple("def_val", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::fill";}
    
private:
    // The default value
    T def_val;
    
    // Inputs and output variables
    abst_ext<T>* ival;
    abst_ext<T>* oval;

    //Implementing the abstract semantics
    void init()
    {
        ival = new abst_ext<T>;
        oval = new abst_ext<T>;
    }
    
    void prep()
    {
        *ival = iport1.read();
    }
    
    void exec()
    {
        *oval = abst_ext<T>(ival->from_abst_ext(def_val));
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *oval)
    }
    
    void clean()
    {
        delete ival;
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

//! Process constructor for a hold process
/*! The process constructor hold creates a process that fills an absent-
 * extended signal with values by replacing absent values by the
 * preceding present value. Only in cases, where no preceding value
 * exists, the absent value is replaced by a default value.
 */
template <class T>
class hold : public sy_process
{
public:
    SY_in<T> iport1;              ///< port for the input channel
    SY_out<T> oport1;             ///< port for the output channel

    //! The constructor requires the process name and a default value
    /*! It creates an SC_THREAD which fills the signal result using the
     * output port
     */
    hold(const sc_module_name& _name,      ///< process name
          const T& def_val                   ///< default value
         ) : sy_process(_name), def_val(def_val)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << def_val;
        arg_vec.push_back(std::make_tuple("def_val", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::hold";}
    
private:
    // The efault value
    T def_val;
    
    // Input and default output variables
    abst_ext<T>* ival;
    abst_ext<T>* oval;

    //Implementing the abstract semantics
    void init()
    {
        ival = new abst_ext<T>;
        oval = new abst_ext<T>;
        *oval = abst_ext<T>(def_val);
    }
    
    void prep()
    {
        *ival = iport1.read();
    }
    
    void exec()
    {
        *oval = ival->is_present() ? *ival : *oval;
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *oval)
    }
    
    void clean()
    {
        delete ival;
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

//! Process constructor for a constant source process
/*! This class is used to build a souce process with constant output.
 * Its main purpose is to be used in test-benches.
 * 
 * This class can directly be instantiated to build a process.
 */
template <class T>
class constant : public sy_process
{
public:
    SY_out<T> oport1;            ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    constant(const sc_module_name& _name,      ///< process name
              const abst_ext<T>& init_val,     ///< The constant output value
              const unsigned long long& take=0 ///< number of tokens produced (0 for infinite)
             ) : sy_process(_name), oport1("oport1"),
                 init_val(init_val), take(take)
                 
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
        ss.str("");
        ss << take;
        arg_vec.push_back(std::make_tuple("take", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::constant";}
    
private:
    abst_ext<T> init_val;
    unsigned long long take;    // Number of tokens produced
    
    unsigned long long tok_cnt;
    bool infinite;
    
    //Implementing the abstract semantics
    void init()
    {
        infinite = take==0 ? true : false;
        tok_cnt = 0;
    }
    
    void prep() {}
    
    void exec() {}
    
    void prod()
    {
        if (tok_cnt++ < take || infinite)
            WRITE_MULTIPORT(oport1, init_val)
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
class source : public sy_process
{
public:
    SY_out<T> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(abst_ext<T>&, const abst_ext<T>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    source(const sc_module_name& _name,      ///< process name
            const functype& _func,         ///< function to be passed
            const abst_ext<T>& init_val,    ///< Initial state
            const unsigned long long& take=0 ///< number of tokens produced (0 for infinite)
          ) : sy_process(_name), oport1("oport1"),
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
    std::string forsyde_kind() const {return "SY::source";}
    
private:
    abst_ext<T> init_st;        // The current state
    unsigned long long take;    // Number of tokens produced
    
    abst_ext<T>* cur_st;        // The current state of the process
    unsigned long long tok_cnt;
    bool infinite;
    
    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        cur_st = new abst_ext<T>;
        *cur_st = init_st;
        WRITE_MULTIPORT(oport1, *cur_st)
        infinite = take==0 ? true : false;
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
    }
#endif
};

//! Process constructor for a source process with vector input
/*! This class is used to build a souce process which only has an output.
 * Given the test bench vector, the process iterates over the emenets
 * of the vector and outputs one value on each evaluation cycle.
 */
template <class T>
class vsource : public sy_process
{
public:
    SY_out<T> oport1;     ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which writes the result using the output
     * port.
     */
    vsource(const sc_module_name& _name,      ///< process name
            const std::vector<abst_ext<T>>& in_vec  ///< Initial vector
            ) : sy_process(_name), in_vec(in_vec)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << in_vec;
        arg_vec.push_back(std::make_tuple("in_vec", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::vsource";}
    
private:
    std::vector<abst_ext<T>> in_vec;
    
    unsigned long tok_cnt;

    //Implementing the abstract semantics
    void init()
    {
        tok_cnt = 0;
    }
    
    void prep() {}
    
    void exec() {}
    
    void prod()
    {
        if (tok_cnt < in_vec.size())
        {
            WRITE_MULTIPORT(oport1, in_vec[tok_cnt])
            tok_cnt++;
        }
        else
            wait();
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

//! Process constructor for a sink process
/*! This class is used to build a sink process which only has an input.
 * Its main purpose is to be used in test-benches. The process repeatedly
 * applies a given function to the current input.
 */
template <class T>
class sink : public sy_process
{
public:
    SY_in<T> iport1;         ///< port for the input channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(const abst_ext<T>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    sink(const sc_module_name& _name,      ///< process name
          const functype& _func             ///< function to be passed
        ) : sy_process(_name), iport1("iport1"), _func(_func)
            
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }

    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::sink";}
    
private:
    abst_ext<T>* val;         // The current state of the process

    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new abst_ext<T>;
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
class zip : public sy_process
{
public:
    SY_in<T1> iport1;        ///< port for the input channel 1
    SY_in<T2> iport2;        ///< port for the input channel 2
    SY_out<std::tuple<abst_ext<T1>,abst_ext<T2>>> oport1;///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    zip(const sc_module_name& _name      ///< process name
        )
         :sy_process(_name), iport1("iport1"), iport2("iport2"), oport1("oport1")
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::zip";}
    
private:
    // intermediate values
    abst_ext<T1>* ival1;
    abst_ext<T2>* ival2;
    
    void init()
    {
        ival1 = new abst_ext<T1>;
        ival2 = new abst_ext<T2>;
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
            typedef std::tuple<abst_ext<T1>,abst_ext<T2>> TT;
            WRITE_MULTIPORT(oport1,abst_ext<TT>())  // write to the output 1
        }
        else
        {
            WRITE_MULTIPORT(oport1,std::make_tuple(*ival1,*ival2))  // write to the output
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
        boundInChans[1].port = &iport2;
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

//! The zipX process with an array of inputs and one output
/*! This process "zips" an array of incoming signals into one signal of arrays.
 */
template <class T1, std::size_t N>
class zipX : public sy_process
{
public:
    std::array<SY_in<T1>,N> iport;              ///< port array for the input channels
    SY_out<std::array<abst_ext<T1>,N>> oport1;  ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    zipX(const sc_module_name& _name      ///< process name
        )
         :sy_process(_name), oport1("oport1")
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::zipX";}
    
private:
    // intermediate values
    std::array<abst_ext<T1>,N> ival;
    
    void init() {}
    
    void prep()
    {
        for (int i=0; i<N; i++)
            ival[i] = iport[i].read();
    }
    
    void exec() {}
    
    void prod()
    {
        typedef std::array<abst_ext<T1>,N> TT;
        if (std::all_of(ival.begin(), ival.end(), [](abst_ext<T1> ivalx){return ivalx.is_absent();}))
        {
            WRITE_MULTIPORT(oport1,abst_ext<TT>())  // write to the output 1
        }
        else
        {
            WRITE_MULTIPORT(oport1,abst_ext<TT>(ival))  // write to the output
        }
    }
    
    void clean() {}
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(N);     // N input ports
        for (int i=0;i<N;i++)
            boundInChans[i].port = &iport[i];
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

//! The zip process with variable number of inputs and one output
/*! This process "zips" the incoming signals into one signal of tuples.
 */
template <class... Ts>
class zipN : public sy_process
{
public:
    std::tuple <SY_in<Ts>...> iport;///< tuple of ports for the input channels
    SY_out<std::tuple<abst_ext<Ts>...> > oport1;///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    zipN(const sc_module_name& _name      ///< process name
         )
         : sy_process(_name), oport1("oport1")
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::zipN";}
private:
    // intermediate values
    std::tuple<abst_ext<Ts>...>* in_vals;
    
    void init()
    {
        in_vals = new std::tuple<abst_ext<Ts>...>;
    }
    
    void prep()
    {
        *in_vals = sc_fifo_tuple_read<Ts...>(iport);
    }
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1,*in_vals)    // write to the output
    }
    
    void clean()
    {
        delete in_vals;
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
    std::tuple<abst_ext<T>...> sc_fifo_tuple_read(std::tuple<SY_in<T>...>& ports)
    {
        std::tuple<abst_ext<T>...> ret;
        fifo_read_helper<sizeof...(T)-1,
                         std::tuple<abst_ext<T>...>,
                         std::tuple<SY_in<T>...>>::read(ret,ports);
        return ret;
    }

};

//! The unzip process with one input and two outputs
/*! This process "unzips" a signal of tuples into two separate signals
 */
template <class T1, class T2>
class unzip : public sy_process
{
public:
    SY_in<std::tuple<abst_ext<T1>,abst_ext<T2>>> iport1;///< port for the input channel
    SY_out<T1> oport1;        ///< port for the output channel 1
    SY_out<T2> oport2;        ///< port for the output channel 2

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * unzips them and writes the results using the output ports
     */
    unzip(const sc_module_name& _name      ///< process name
          )
         :sy_process(_name), iport1("iport1"), oport1("oport1"), oport2("oport2")
    {}
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::unzip";}
private:
    // intermediate values
    abst_ext<std::tuple<abst_ext<T1>,abst_ext<T2>>>* in_val;
    
    void init()
    {
        in_val = new abst_ext<std::tuple<abst_ext<T1>,abst_ext<T2>>>;
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
            WRITE_MULTIPORT(oport1,abst_ext<T1>())  // write to the output 1
            WRITE_MULTIPORT(oport2,abst_ext<T2>())  // write to the output 2
        }
        else
        {
            WRITE_MULTIPORT(oport1,std::get<0>(in_val->unsafe_from_abst_ext()))  // write to the output 1
            WRITE_MULTIPORT(oport2,std::get<1>(in_val->unsafe_from_abst_ext()))  // write to the output 2
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
        boundOutChans.resize(2);    // two output ports
        boundOutChans[0].port = &oport1;
        boundOutChans[1].port = &oport2;
    }
#endif
};

//! The unzipX process with one input and an array of outputs
/*! This process "unzips" a signal of arrays into an array of separate signals
 */
template <class T1, std::size_t N>
class unzipX : public sy_process
{
public:
    SY_in<std::array<abst_ext<T1>,N>> iport1;///< port for the input channel
    std::array<SY_out<T1>,N> oport;///< port array for the output channels

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * unzips them and writes the results using the output ports
     */
    unzipX(const sc_module_name& _name      ///< process name
          )
         :sy_process(_name), iport1("iport1")
    {}
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::unzipX";}
private:
    // intermediate values
    abst_ext<std::array<abst_ext<T1>,N>>* in_val;
    
    void init()
    {
        in_val = new abst_ext<std::array<abst_ext<T1>,N>>;
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
            for (int i=0; i<N; i++)
                WRITE_MULTIPORT(oport[i],abst_ext<T1>())  // write to the output i
        }
        else
        {
            for (int i=0; i<N; i++)
                WRITE_MULTIPORT(oport[i],in_val->unsafe_from_abst_ext()[i])  // write to the output i
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
        boundOutChans.resize(N);    // two output ports
        for (int i=0;i<N;i++)
            boundOutChans[i].port = &oport[i];
    }
#endif
};

//! The unzip process with one input and variable number of outputs
/*! This process "unzips" the incoming signal into a tuple of signals.
 */
template <class... Ts>
class unzipN : public sy_process
{
public:
    SY_in<std::tuple<abst_ext<Ts>...>> iport1;///< port for the input channel
    std::tuple<SY_out<Ts>...> oport;///< tuple of ports for the output channels

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * unzips it and writes the results using the output ports
     */
    unzipN(const sc_module_name& _name      ///< process name
           )
          :sy_process(_name), iport1("iport1")
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::unzipN";}
private:
    // intermediate values
    abst_ext<std::tuple<abst_ext<Ts>...>>* in_val;
    
    void init()
    {
        in_val = new abst_ext<std::tuple<abst_ext<Ts>...>>;
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
            std::tuple<abst_ext<Ts>...> all_abs;
            fifo_tuple_write<Ts...>(all_abs, oport);
        }
        else
        {
            fifo_tuple_write<Ts...>(in_val->unsafe_from_abst_ext(), oport);
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
    void fifo_tuple_write(const std::tuple<abst_ext<T>...>& vals,
                             std::tuple<SY_out<T>...>& ports)
    {
        fifo_write_helper<sizeof...(T)-1,
                          std::tuple<abst_ext<T>...>,
                          std::tuple<SY_out<T>...>>::write(vals,ports);
    }

#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport1;
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
                             std::tuple<SY_out<T>...>& ports)
    {
        register_ports_helper<sizeof...(T)-1,
                              std::tuple<SY_out<T>...>&>::reg_port(boundChans,ports);
    }
#endif

};

//! The group process with one input and one absent-extended output
/*! It groups values into a vector of specified size n, which takes n
 * cycles. While the grouping takes place the output from this process
 * consists of absent values.
 */
template <class T>
class group : public sy_process
{
public:
    SY_in<T> iport1;                           ///< port for the input channel
    SY_out<std::vector<abst_ext<T>>> oport1;    ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * groups together n samples and writes the results using the output
     * port.
     */
    group(const sc_module_name& _name,      ///< process name
           const unsigned long& samples       ///< Number of samples in each group
          )
         :sy_process(_name), samples(samples)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << samples;
        arg_vec.push_back(std::make_tuple("samples", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::group";}
    
private:
    // Number of samples in each group
    unsigned long samples;
    
    unsigned long samples_took;
    
    // The output vector
    std::vector<abst_ext<T>>* oval;
    
    //Implementing the abstract semantics
    void init()
    {
        oval = new std::vector<abst_ext<T>>;
        oval->resize(samples);
        samples_took = 0;
    }
    
    void prep()
    {
        (*oval)[samples_took] = iport1.read();
        samples_took++;
    }
    
    void exec() {}
    
    void prod()
    {
        if (samples_took==samples)
        {
            WRITE_MULTIPORT(oport1, *oval)
            samples_took = 0;
        }
        else
            WRITE_MULTIPORT(oport1, abst_ext<std::vector<abst_ext<T>>>())
    }
    
    void clean()
    {
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
class fanout : public sy_process
{
public:
    SY_in<T> iport1;        ///< port for the input channel
    SY_out<T> oport1;       ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies and writes the results using the output port
     */
    fanout(const sc_module_name& _name      ///< process name
           ) 
         : sy_process(_name) { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::fanout";}
    
private:
    // Inputs and output variables
    abst_ext<T>* val;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new abst_ext<T>;
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
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

}
}

#endif
