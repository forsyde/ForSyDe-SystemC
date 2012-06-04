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

/*! \file sy_process_constructors.h
 * \brief Implements the basic process constructors in the SY MoC
 * 
 *  This file includes the basic process constructors used for modeling
 * in the synchronous model of computation.
 */

#include <functional>
#include <tuple>

#include "abst_ext.hpp"
#include "sy_process.hpp"

namespace ForSyDe
{

namespace SY
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
    comb(sc_module_name _name,      ///< process name
         functype _func             ///< function to be passed
         ) : sy_process(_name), iport1("iport1"), oport1("oport1"),
             _func(_func)
    {
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
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
        WRITE_MULTIPORT(oport1, *oval);
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
        boundInChans[0].portType = typeid(T1).name();
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
        boundOutChans[0].portType = typeid(T0).name();
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
    comb2(sc_module_name _name,      ///< process name
          functype _func             ///< function to be passed
          ) : sy_process(_name), iport1("iport1"), iport2("iport2"), oport1("oport1"),
              _func(_func)
    {
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
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
        WRITE_MULTIPORT(oport1, *oval);
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
        boundInChans[0].portType = typeid(T1).name();
        boundInChans[1].port = &iport2;
        boundInChans[1].portType = typeid(T2).name();
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
        boundOutChans[0].portType = typeid(T0).name();
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
    comb3(sc_module_name _name,      ///< process name
          functype _func             ///< function to be passed
          ) : sy_process(_name), _func(_func)
    {
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
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
        WRITE_MULTIPORT(oport1, *oval);
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
        boundInChans[0].portType = typeid(T1).name();
        boundInChans[1].port = &iport2;
        boundInChans[1].portType = typeid(T2).name();
        boundInChans[2].port = &iport3;
        boundInChans[2].portType = typeid(T3).name();
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
        boundOutChans[0].portType = typeid(T0).name();
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
    typedef std::function<void(abst_ext<T0>, const abst_ext<T1>&,
                                             const abst_ext<T2>&,
                                             const abst_ext<T3>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb4(sc_module_name _name,      ///< process name
          functype _func             ///< function to be passed
          ) : sy_process(_name), _func(_func)
    {
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "SY::comb4";}
    
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
        _func(oval, *ival1, *ival2, *ival3);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *oval);
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
        boundInChans.resize(4);     // only one input port
        boundInChans[0].port = &iport1;
        boundInChans[0].portType = typeid(T1).name();
        boundInChans[1].port = &iport2;
        boundInChans[1].portType = typeid(T2).name();
        boundInChans[2].port = &iport3;
        boundInChans[2].portType = typeid(T3).name();
        boundInChans[3].port = &iport4;
        boundInChans[3].portType = typeid(T4).name();
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
        boundOutChans[0].portType = typeid(T0).name();
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
    delay(sc_module_name _name,     ///< process name
          abst_ext<T> init_val      ///< initial value
          ) : sy_process(_name), iport1("iport1"), oport1("oport1"),
              init_val(init_val)
    {
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
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
        WRITE_MULTIPORT(oport1, init_val);
    }
    
    void prep()
    {
        *val = iport1.read();
    }
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *val);
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
    delayn(sc_module_name _name,    ///< process name
           abst_ext<T> init_val,    ///< initial value
           unsigned int n            ///< number of delay elements
          ) : sy_process(_name), init_val(init_val), ns(n)
    {
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
        ss.str("");
        ss << n;
        arg_vec.push_back(std::make_tuple("n", ss.str()));
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
            WRITE_MULTIPORT(oport1, init_val);
    }
    
    void prep()
    {
        *val = iport1.read();
    }
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *val);
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

//! Process constructor for a Moore machine
/*! This class is used to build a finite state machine of type Moore.
 * Given an initial state, a next-state function, and an output decoding
 * function it creates a Moore process.
 * 
 * Note that this is an abstract class and can not be directly
 * instantiated. The designer should derive from this class and
 * implement the _ns_func and _od_func functions.
 */
template <class ITYP, class STYP, class OTYP>
class moore : public sc_module
{
public:
    sc_fifo_in<ITYP>  iport;        ///< port for the input channel
    sc_fifo_out<OTYP> oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    moore(sc_module_name _name,   ///< The module name
            STYP ist                ///< Initial state
            )
         :sc_module(_name), init_st(ist)
    {
        SC_THREAD(worker);
    }
private:
    STYP init_st;
    SC_HAS_PROCESS(moore);

    //! The main and only execution thread of the module
    void worker()
    {
        ITYP in_val;
        STYP ns_val, st_val = init_st;
        OTYP out_val;
        while (1)
        {
            in_val = iport.read();              // read from input
            ns_val = _ns_func(st_val,in_val);   // calculate next state
            out_val = _od_func(st_val);         // calculate output
            WRITE_MULTIPORT(oport1,out_val);    // write to the output
            st_val = ns_val;                    // update the state
        }
    }

protected:
    //! The next-state caclulation function
    /*! It is pure and the user should provide an implementation for
     * it in the derived class.
     */
    virtual STYP (_ns_func)(STYP, ITYP) = 0;
    
    //! The output-decoding caclulation function
    /*! It is pure and the user should provide an implementation for
     * it in the derived class.
     */
    virtual OTYP (_od_func)(STYP) = 0;
};

//! Process constructor for a Mealy machine
/*! This class is used to build a finite state machine of type Mealy.
 * Given an initial state, a next-state function, and an output decoding
 * function it creates a Mealy process.
 * 
 * Note that this is an abstract class and can not be directly
 * instantiated. The designer should derive from this class and
 * implement the _ns_func and _od_func functions.
 */
template <class ITYP, class STYP, class OTYP>
class mealy : public sc_module
{
public:
    sc_fifo_in<ITYP>  iport;        ///< port for the input channel
    sc_fifo_out<OTYP> oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    mealy(sc_module_name _name,   ///< The module name
            STYP ist                ///< Initial state
            )
         :sc_module(_name), init_st(ist)
    {
        SC_THREAD(worker);
    }
private:
    STYP init_st;
    SC_HAS_PROCESS(mealy);

    //! The main and only execution thread of the module
    void worker()
    {
        ITYP in_val;
        STYP ns_val, st_val = init_st;
        OTYP out_val;
        while (1)
        {
            in_val = iport.read();              // read from input
            ns_val = _ns_func(st_val,in_val);   // calculate next state
            out_val = _od_func(st_val,in_val);  // calculate output
            WRITE_MULTIPORT(oport1,out_val);    // write to the output
            st_val = ns_val;                    // update the state
        }
    }

protected:
    //! The next-state caclulation function
    /*! It is pure and the user should provide an implementation for
     * it in the derived class.
     */
    virtual STYP (_ns_func)(STYP, ITYP) = 0;
    
    //! The output-decoding caclulation function
    /*! It is pure and the user should provide an implementation for
     * it in the derived class.
     */
    virtual OTYP (_od_func)(STYP, ITYP) = 0;
};

//! Process constructor for a two input Mealy machine
/*! This class is used to build a finite state machine of type Mealy.
 * Given an initial state, a next-state function, and an output decoding
 * function it creates a Mealy process.
 * 
 * Note that this is an abstract class and can not be directly
 * instantiated. The designer should derive from this class and
 * implement the _ns_func and _od_func functions.
 */
template <class ITYP1, class ITYP2, class STYP, class OTYP>
class mealy2 : public sc_module
{
public:
    sc_fifo_in<ITYP1>  iport1;       ///< port for the input channel
    sc_fifo_in<ITYP2>  iport2;       ///< port for the input channel
    sc_fifo_out<OTYP> oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    mealy2(sc_module_name _name,   ///< The module name
            STYP ist                ///< Initial state
            )
         :sc_module(_name), init_st(ist)
    {
        SC_THREAD(worker);
    }
private:
    STYP init_st;
    SC_HAS_PROCESS(mealy2);

    //! The main and only execution thread of the module
    void worker()
    {
        ITYP1 in_val1;
        ITYP2 in_val2;
        STYP ns_val, st_val = init_st;
        OTYP out_val;
        while (1)
        {
            in_val1 = iport1.read();            // read from input
            in_val2 = iport2.read();            // read from input
            ns_val  = _ns_func(st_val,in_val1,in_val2);// calculate next state
            out_val = _od_func(st_val,in_val1,in_val2);// calculate output
            WRITE_MULTIPORT(oport1,out_val);    // write to the output
            st_val = ns_val;                    // update the state
        }
    }

protected:
    //! The next-state caclulation function
    /*! It is pure and the user should provide an implementation for
     * it in the derived class.
     */
    virtual STYP (_ns_func)(STYP, ITYP1, ITYP2) = 0;
    
    //! The output-decoding caclulation function
    /*! It is pure and the user should provide an implementation for
     * it in the derived class.
     */
    virtual OTYP (_od_func)(STYP, ITYP1, ITYP2) = 0;
};

//! Process constructor for a fill process
/*! The process constructor fill creates a process that fills an absent-
 * extended signal with present values by replacing absent values with a
 * given value.
 * 
 * The output signal is not any more of the type abst_ext. 
 */
template <class TYP>
class fill : public sc_module
{
public:
    sc_fifo_out<abst_ext<TYP> > iport;   ///< port for the input channel
    sc_fifo_out<TYP> oport1;             ///< port for the output channel

    //! The constructor requires the process name and a default value
    /*! It creates an SC_THREAD which fills the signal result using the
     * output port
     */
    fill(sc_module_name _name,      ///< process name
         TYP dval                   ///< default value
         ):sc_module(_name), defval(dval)
    {
        
        SC_THREAD(worker);
    }
private:
    TYP defval;
    SC_HAS_PROCESS(fill);

    //! The main and only execution thread of the module
    void worker()
    {
        abst_ext<TYP> in_val;
        TYP out_val;
        while (1)
        {
            in_val = iport.read();  // read from the input
            out_val = in_val.from_abst_ext(defval); // fill the output signal
            WRITE_MULTIPORT(oport1,out_val);      // write to the output
        }
    }
};

//! Process constructor for a hold process
/*! The process constructor hold creates a process that fills an absent-
 * extended signal with values by replacing absent values by the
 * preceding present value. Only in cases, where no preceding value
 * exists, the absent value is replaced by a default value.
 * 
 * The output signal is not any more of the type abst_ext. 
 */
template <class TYP>
class hold : public sc_module
{
public:
    sc_fifo_in<abst_ext<TYP> > iport;   ///< port for the input channel
    sc_fifo_out<TYP> oport1;             ///< port for the output channel

    //! The constructor requires the process name and a default value
    /*! It creates an SC_THREAD which fills the signal result using the
     * output port
     */
    hold(sc_module_name _name,      ///< process name
         TYP dval                   ///< default value
         ):sc_module(_name), defval(dval)
    {
        
        SC_THREAD(worker);
    }
private:
    TYP defval;
    SC_HAS_PROCESS(hold);

    //! The main and only execution thread of the module
    void worker()
    {
        abst_ext<TYP> in_val;
        TYP out_val;
        while (1)
        {
            in_val = iport.read();  // read from the input
            // update the default value
            defval = in_val.is_present() ? in_val.unsafe_from_abst_ext()
                                         : defval;
            // fill the output signal
            out_val = defval;
            WRITE_MULTIPORT(oport1,out_val);    // write to the output
        }
    }
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
    constant(sc_module_name _name,      ///< The module name
              abst_ext<T> init_val,     ///< The constant output value
              unsigned long long take=0 ///< number of tokens produced (0 for infinite)
             ) : sy_process(_name), oport1("oport1"),
                 init_val(init_val), take(take)
                 
    {
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
        ss.str("");
        ss << take;
        arg_vec.push_back(std::make_tuple("take", ss.str()));
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
        if (take==0) infinite = true;
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
    source(sc_module_name _name,   ///< The module name
           functype _func,         ///< function to be passed
           abst_ext<T> init_val,    ///< Initial state
           unsigned long long take=0 ///< number of tokens produced (0 for infinite)
          ) : sy_process(_name), oport1("oport1"),
              init_st(init_val), take(take), _func(_func)
    {
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
        std::stringstream ss;
        ss << init_val;
        arg_vec.push_back(std::make_tuple("init_val", ss.str()));
        ss.str("");
        ss << take;
        arg_vec.push_back(std::make_tuple("take", ss.str()));
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
        WRITE_MULTIPORT(oport1, *cur_st);
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
            WRITE_MULTIPORT(oport1,out_val);    // write to the output
        }
    }
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
    sink(sc_module_name _name,      ///< process name
         functype _func             ///< function to be passed
        ) : sy_process(_name), iport1("iport1"), _func(_func)
            
    {
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
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
        boundInChans[0].portType = typeid(T).name();
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
template <class ITYP1, class ITYP2>
class zip : public sc_module
{
public:
    sc_fifo_in<ITYP1> iport1;        ///< port for the input channel 1
    sc_fifo_in<ITYP2> iport2;        ///< port for the input channel 2
    sc_fifo_out<std::tuple<ITYP1,ITYP2> > oport1;///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    zip(sc_module_name _name)
         :sc_module(_name)
    {
        SC_THREAD(worker);
    }
private:
    SC_HAS_PROCESS(zip);

    //! The main and only execution thread of the module
    void worker()
    {
        ITYP1 in_val1;
        ITYP2 in_val2;
        std::tuple<ITYP1,ITYP2> out_val;
        while (1)
        {
            in_val1 = iport1.read();  // read from input 1
            in_val2 = iport2.read();  // read from input 2
            out_val = std::make_tuple(in_val1,in_val2); // make the output
            WRITE_MULTIPORT(oport1,out_val);    // write to the output
        }
    }

};

//! The zip3 process with three inputs and one output
/*! This process "zips" three incoming signals into one signal of tuples.
 */
template <class ITYP1, class ITYP2, class ITYP3>
class zip3 : public sc_module
{
public:
    sc_fifo_in<ITYP1> iport1;        ///< port for the input channel 1
    sc_fifo_in<ITYP2> iport2;        ///< port for the input channel 2
    sc_fifo_in<ITYP2> iport3;        ///< port for the input channel 2
    sc_fifo_out<std::tuple<ITYP1,ITYP2,ITYP3> > oport1;///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    zip3(sc_module_name _name)
         :sc_module(_name)
    {
        SC_THREAD(worker);
    }
private:
    SC_HAS_PROCESS(zip3);

    //! The main and only execution thread of the module
    void worker()
    {
        ITYP1 in_val1;
        ITYP2 in_val2;
        ITYP3 in_val3;
        std::tuple<ITYP1,ITYP2,ITYP3> out_val;
        while (1)
        {
            in_val1 = iport1.read();  // read from input 1
            in_val2 = iport2.read();  // read from input 2
            in_val3 = iport3.read();  // read from input 3
            out_val = std::make_tuple(in_val1,in_val2,in_val3); // make the output
            WRITE_MULTIPORT(oport1,out_val);    // write to the output
        }
    }

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
            WRITE_MULTIPORT(oport1,in_vals);    // write to the output
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
    unzip(sc_module_name _name)
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
            WRITE_MULTIPORT(oport1,abst_ext<T1>());  // write to the output 1
            WRITE_MULTIPORT(oport2,abst_ext<T2>());  // write to the output 2
        }
        else
        {
            WRITE_MULTIPORT(oport1,std::get<0>(in_val->unsafe_from_abst_ext()));  // write to the output 1
            WRITE_MULTIPORT(oport2,std::get<1>(in_val->unsafe_from_abst_ext()));  // write to the output 2
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
template <class... ITYPs>
class unzipN : public sc_module
{
public:
    sc_fifo_in<std::tuple<ITYPs...> > iport;///< port for the input channel
    std::tuple <sc_fifo_out<ITYPs>...> oport;///< tuple of ports for the output channels

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * unzips it and writes the results using the output ports
     */
    unzipN(sc_module_name _name)
          :sc_module(_name)
    {
        SC_THREAD(worker);
    }
private:
    SC_HAS_PROCESS(unzipN);

    //! The main and only execution thread of the module
    void worker()
    {
        std::tuple<ITYPs...> in_vals;
        while (1)
        {
            in_vals = iport.read();
            sc_fifo_tuple_write<ITYPs...>(in_vals, oport);
        }
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
    void sc_fifo_tuple_write(const std::tuple<T...>& vals,
                             std::tuple<sc_fifo_out<T>...>& ports)
    {
        fifo_write_helper<sizeof...(T)-1,
                          std::tuple<T...>,
                          std::tuple<sc_fifo_out<T>...>>::write(vals,ports);
    }

};

//! The group process with one input and one absent-extended output
/*! It groups values into a vector of specified size n, which takes n
 * cycles. While the grouping takes place the output from this process
 * consists of absent values.
 */
template <class TYP>
class group : public sc_module
{
public:
    sc_fifo_in<TYP> iport;                         ///< port for the input channel
    sc_fifo_out<abst_ext<std::vector<TYP> > > oport1;///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * groups together n samples and writes the results using the output
     * port.
     */
    group(sc_module_name _name,         ///< The module name
          int n                         ///< Number of samples in each group
          )
         :sc_module(_name), grppts(n)
    {
        SC_THREAD(worker);
    }
private:
    int grppts;
    SC_HAS_PROCESS(group);

    //! The main and only execution thread of the module
    void worker()
    {
        std::vector<TYP> in_val(grppts);
        abst_ext<std::vector<TYP> > out_val;
        while (1)
        {
            for (int i=0;i<grppts;i++)
            {
                in_val[i] = iport.read();  // read from input and group
                if (i<grppts-1)
                {
                    out_val.setAbst(); // Absent Output
                    WRITE_MULTIPORT(oport1,out_val);// write to the output
                }
            }
            out_val.setVal(in_val); // the grouped input
            WRITE_MULTIPORT(oport1,out_val);        // write to the output
        }
    }

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
template <class IOTYP>
class fanout : public sc_module
{
public:
    sc_fifo_in<IOTYP> iport;        ///< port for the input channel
    sc_fifo_out<IOTYP> oport1;       ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies and writes the results using the output port
     */
    fanout(sc_module_name _name)  // module name
         :sc_module(_name)
    {
        SC_THREAD(worker);
    }
private:
    SC_HAS_PROCESS(fanout);

    //! The main and only execution thread of the module
    void worker()
    {
        IOTYP in_val;
        IOTYP out_val;
        while (1)
        {
            in_val = iport.read();  // read from input
            out_val = in_val;        // same output
            WRITE_MULTIPORT(oport1,out_val);    // write to the output
        }
    }

};

}
}

#endif
