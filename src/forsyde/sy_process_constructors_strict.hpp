/**********************************************************************           
    * sy_process_constructors_strict.hpp -- Strict process            *
    *                                       constructors in the SY MOC*
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing basic process constructors for modeling      *
    *          strict synchronous systems in ForSyDe-SystemC          *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SY_PROCESS_CONSTRUCTORS_STRICT_HPP
#define SY_PROCESS_CONSTRUCTORS_STRICT_HPP

/*! \file sy_process_constructors_strict.hpp
 * \brief Implements the basic strict process constructors in the SY MoC
 * 
 *  This file includes the basic process constructors used for modeling
 * in the synchronous model of computation without dealing with absent
 * events.
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

//! Process constructor for a strict combinational process with one input and one output
/*! This class is used to build combinational processes with one input
 * and one output. The class is parameterized for input and output
 * data-types.
 */
template <typename T0, typename T1>
class scomb : public sy_process
{
public:
    SY_in<T1>  iport1;       ///< port for the input channel
    SY_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(T0&,const T1&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    scomb(const sc_module_name& _name,      ///< process name
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
    std::string forsyde_kind() const {return "SY::scomb";}

private:
    // Inputs and output variables
    T0* oval;
    T1* ival1;
    
    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        oval = new T0;
        ival1 = new T1;
    }
    
    void prep()
    {
        auto ival1_temp = iport1.read();
        CHECK_PRESENCE(ival1_temp);
        *ival1 = unsafe_from_abst_ext(ival1_temp);
    }
    
    void exec()
    {
        _func(*oval, *ival1);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, abst_ext<T0>(*oval))
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

//! Process constructor for a strict combinational process with two inputs and one output
/*! similar to comb with two inputs
 */
template <typename T0, typename T1, typename T2>
class scomb2 : public sy_process
{
public:
    SY_in<T1> iport1;        ///< port for the input channel 1
    SY_in<T2> iport2;        ///< port for the input channel 2
    SY_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(T0&, const T1&, const T2&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    scomb2(const sc_module_name& _name,      ///< process name
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
    std::string forsyde_kind() const {return "SY::wcomb2";}
private:
    // Inputs and output variables
    T0* oval;
    T1* ival1;
    T2* ival2;
    
    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        oval = new T0;
        ival1 = new T1;
        ival2 = new T2;
    }
    
    void prep()
    {
        auto ival1_temp = iport1.read();
        auto ival2_temp = iport2.read();
        CHECK_PRESENCE(ival1_temp);
        CHECK_PRESENCE(ival2_temp);
        *ival1 = unsafe_from_abst_ext(ival1_temp);
        *ival2 = unsafe_from_abst_ext(ival2_temp);
    }
    
    void exec()
    {
        _func(*oval, *ival1, *ival2);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, abst_ext<T0>(*oval))
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

//! Process constructor for a strict combinational process with three inputs and one output
/*! similar to comb with three inputs
 */
template <typename T0, typename T1, typename T2, typename T3>
class scomb3 : public sy_process
{
public:
    SY_in<T1> iport1;        ///< port for the input channel 1
    SY_in<T2> iport2;        ///< port for the input channel 2
    SY_in<T3> iport3;        ///< port for the input channel 3
    SY_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(T0&, const T1&, const T2&, 
                                    const T3&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    scomb3(const sc_module_name& _name,      ///< process name
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
    std::string forsyde_kind() const {return "SY::scomb3";}
    
private:
    // Inputs and output variables
    T0* oval;
    T1* ival1;
    T2* ival2;
    T3* ival3;

    //! The function passed to the process constructor
    functype _func;
    
    //Implementing the abstract semantics
    void init()
    {
        oval = new T0;
        ival1 = new T1;
        ival2 = new T2;
        ival3 = new T3;
    }
    
    void prep()
    {
        auto ival1_temp = iport1.read();
        auto ival2_temp = iport2.read();
        auto ival3_temp = iport3.read();
        CHECK_PRESENCE(ival1_temp);
        CHECK_PRESENCE(ival2_temp);
        CHECK_PRESENCE(ival3_temp);
        *ival1 = unsafe_from_abst_ext(ival1_temp);
        *ival2 = unsafe_from_abst_ext(ival2_temp);
        *ival3 = unsafe_from_abst_ext(ival3_temp);
    }
    
    void exec()
    {
        _func(*oval, *ival1, *ival2, *ival3);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, abst_ext<T0>(*oval))
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

//! Process constructor for a strict combinational process with four inputs and one output
/*! similar to comb with four inputs
 */
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class scomb4 : public sy_process
{
public:
    SY_in<T1> iport1;       ///< port for the input channel 1
    SY_in<T2> iport2;       ///< port for the input channel 2
    SY_in<T3> iport3;       ///< port for the input channel 3
    SY_in<T4> iport4;       ///< port for the input channel 4
    SY_out<T0> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(T0&, const T1&, const T2&,
                                    const T3&, const T4&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    scomb4(const sc_module_name& _name,      ///< process name
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
    std::string forsyde_kind() const{return "SY::scomb4";}
    
private:
    // Inputs and output variables
    T0* oval;
    T1* ival1;
    T2* ival2;
    T3* ival3;
    T4* ival4;
    
    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        oval = new T0;
        ival1 = new T1;
        ival2 = new T2;
        ival3 = new T3;
        ival4 = new T4;
    }
    
    void prep()
    {
        auto ival1_temp = iport1.read();
        auto ival2_temp = iport2.read();
        auto ival3_temp = iport3.read();
        auto ival4_temp = iport4.read();
        CHECK_PRESENCE(ival1_temp);
        CHECK_PRESENCE(ival2_temp);
        CHECK_PRESENCE(ival3_temp);
        CHECK_PRESENCE(ival4_temp);
        *ival1 = unsafe_from_abst_ext(ival1_temp);
        *ival2 = unsafe_from_abst_ext(ival2_temp);
        *ival3 = unsafe_from_abst_ext(ival3_temp);
        *ival4 = unsafe_from_abst_ext(ival4_temp);
    }
    
    void exec()
    {
        _func(*oval, *ival1, *ival2, *ival3, *ival4);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, abst_ext<T0>(*oval))
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

//! Process constructor for a strict combinational process with an array of inputs and one output
/*! similar to comb with an array of inputs
 */
template <typename T0, typename T1, std::size_t N>
class scombX : public sy_process
{
public:
    std::array<SY_in<T1>,N> iport;       ///< port for the input channel 1
    SY_out<T0> oport1;        ///< port for the output channel

    //! Type of the function to be passed to the process constructor
    typedef std::function<void(T0&, const std::array<T1,N>&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    scombX(const sc_module_name& _name,      ///< process name
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
    std::string forsyde_kind() const{return "SY::scombX";}

private:
    // Inputs and output variables
    T0* oval;
    std::array<T1,N> ival;

    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        oval = new T0;
    }

    void prep()
    {
        for (size_t i=0; i<N; i++)
        {
            auto ival_temp = iport.read();
            CHECK_PRESENCE(ival_temp);
    		ival[i] = unsafe_from_abst_ext(ival_temp);
        }
    }

    void exec()
    {
        _func(*oval, ival);
    }

    void prod()
    {
        WRITE_MULTIPORT(oport1, abst_ext<T0>(*oval))
    }

    void clean()
    {
        delete oval;
    }

#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(N);     // only one input port
        for (size_t i=0; i<N; i++)
        	boundInChans[i].port = &iport[i];
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};

//! A data-parallel process constructor for a strict combinational process with input and output array types
/*! similar to comb with an array of inputs
 */
template <typename T0, typename T1, std::size_t N>
class sdpmap : public sy_process
{
public:
    SY_in<std::array<T1,N>> iport1;       ///< port for the input channel 1
    SY_out<std::array<T0,N>> oport1;        ///< port for the output channel

    //! Type of the function to be passed to the process constructor
    typedef std::function<void(T0&, const T1&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to each element and writes
     * the results using the output port
     */
    sdpmap(const sc_module_name& _name,    ///< process name
           const functype& _func            ///< function to be passed
          ) : sy_process(_name), _func(_func)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
#endif
    }

    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "SY::sdpmap";}

private:
    // Inputs and output variables
    std::array<T0,N> oval;
    std::array<T1,N> ival;

    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init() {}

    void prep()
    {
        auto ival_temp = iport1.read();
        CHECK_PRESENCE(ival_temp);
        ival = unsafe_from_abst_ext(ival_temp);
    }

    void exec()
    {
        #ifdef FORSYDE_OPENMP
        #pragma omp parallel for
        #endif
        for (size_t i=0; i<N; i++)
        {
            _func(oval[i], ival[i]);
        }
    }

    void prod()
    {
        auto tempval = abst_ext<std::array<T0,N>>(oval);
        WRITE_MULTIPORT(oport1, tempval)
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

//! A data-parallel process constructor for a strict reduce process with an array of inputs and one output
/*! This process constructor implements the strict version of the well-
 * known reduce operation, common in data-parallel applications.
 */
template <typename T0, std::size_t N>
class sdpreduce : public sy_process
{
public:
    SY_in<std::array<T0,N>> iport1;     ///< port for the input channel 1
    SY_out<T0> oport1;                  ///< port for the output channel

    //! Type of the function to be passed to the process constructor
    typedef std::function<void(T0&, const T0&, const T0&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    sdpreduce(const sc_module_name& _name,      ///< process name
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
    std::string forsyde_kind() const{return "SY::sdpreduce";}

private:
    // Inputs and output variables
    T0* oval;
    std::array<T0,N> ival;

    //! The function passed to the process constructor
    functype _func;

    //Implementing the abstract semantics
    void init()
    {
        oval = new T0;
    }

    void prep()
    {
        auto ival1_temp = iport1.read();
        CHECK_PRESENCE(ival1_temp);
        ival = unsafe_from_abst_ext(ival1_temp);
    }

    void exec()
    {
        T0 res = T0();
        #ifdef FORSYDE_OPENMP  // this can be enhanced with the new delare reduction clause in OpenMP 4.0
        
        #pragma omp parallel shared(res) // Create omp threads
        {
            T0 val = T0();  // val can be declared as local variable (for each thread) 
            #pragma omp for nowait
            for (int i = 0; i < N; ++i)
            {
                _func(val, val, ival[i]);
            }
            #pragma omp critical
            {
                _func(res, res, val);
            }
        }
        
        #else
        
        res = ival[0];
        for (size_t i=1;i<N;i++)
            _func(res, res, ival[i]);
        
        #endif
        *oval = res;
    }

    void prod()
    {
        WRITE_MULTIPORT(oport1, abst_ext<T0>(*oval))
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

//! A data-parallel process constructor for a strict scan process with input and output array types
/*! This process constructor implements the strict version of the well-
 * known scan operation, common in data-parallel applications.
 * For the special case of the addition operation, it is called all
 * partial sums or sum-prefix in the literature.
 */
template <typename T0, typename T1, std::size_t N>
class sdpscan : public sy_process
{
public:
    SY_in<std::array<T1,N>> iport1;       ///< port for the input channel 1
    SY_out<std::array<T0,N>> oport1;        ///< port for the output channel

    //! Type of the function to be passed to the process constructor
    typedef std::function<void(T0&, const T0&, const T1&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    sdpscan(const sc_module_name& _name,      ///< process name
           const functype& _func,             ///< function to be passed
           const T0& init_res                 ///< initial value for running result
          ) : sy_process(_name), _func(_func), init_res(init_res)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("_func",func_name+std::string("_func")));
        std::stringstream ss;
        ss << init_res;
        arg_vec.push_back(std::make_tuple("init_res",ss.str()));
#endif
    }

    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "SY::sdpscan";}

private:
    // Inputs and output variables
    std::array<T0,N> oval;
    std::array<T0,N> ival;

    //! The function passed to the process constructor
    functype _func;
    
    T0 init_res;

    //Implementing the abstract semantics
    void init() {}

    void prep()
    {
        auto ival1_temp = iport1.read();
        CHECK_PRESENCE(ival1_temp);
        ival = unsafe_from_abst_ext(ival1_temp);
    }

    void exec()
    {
        _func(oval[0], init_res, ival[0]);
        for (size_t i=1;i<N;i++)
            _func(oval[i], oval[i-1], ival[i]);
    }

    void prod()
    {
        auto tempval = abst_ext<std::array<T0,N>>(oval);
        WRITE_MULTIPORT(oport1, tempval)
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

//! Process constructor for a strict delay element
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
class sdelay : public sy_process
{
public:
    SY_in<T>  iport1;       ///< port for the input channel
    SY_out<T> oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial element, reads
     * data from its input port, and writes the results using the output
     * port.
     */
    sdelay(const sc_module_name& _name,  ///< process name
           const T& init_val            ///< initial value
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
    std::string forsyde_kind() const {return "SY::sdelay";}
    
private:
    // Initial value
    T init_val;
    
    // Inputs and output variables
    T* val;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new T;
        WRITE_MULTIPORT(oport1, abst_ext<T>(init_val))
    }
    
    void prep()
    {
        auto ival1_temp = iport1.read();
        CHECK_PRESENCE(ival1_temp);
        *val = unsafe_from_abst_ext(ival1_temp);
    }
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, abst_ext<T>(*val))
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

//! Process constructor for a strict n-delay element
/*! This class is used to build a sequential process similar to dalay
 * but with an extra initial variable which sets the number of delay
 * elements (initial tokens). Given an initial value, it inserts the
 * initial value n times at the the beginning of output stream and
 * passes the rest of the inputs to its output untouched. The class is
 * parameterized for its input/output data-type.
 */
template <class T>
class sdelayn : public sy_process
{
public:
    SY_in<T>  iport1;       ///< port for the input channel
    SY_out<T> oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial elements,
     * reads data from its input port, and writes the results using the
     * output port.
     */
    sdelayn(const sc_module_name& _name,    ///< process name
            const T& init_val,              ///< initial value
            const unsigned int& n           ///< number of delay elements
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
    std::string forsyde_kind() const {return "SY::sdelayn";}
    
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
        for (int i=0; i<ns; i++)
            WRITE_MULTIPORT(oport1, abst_ext<T>(init_val))
    }
    
    void prep()
    {
        auto ival1_temp = iport1.read();
        CHECK_PRESENCE(ival1_temp);
        *val = unsafe_from_abst_ext(ival1_temp);
    }
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, abst_ext<T>(*val))
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

//! Process constructor for a strict Moore machine
/*! This class is used to build a finite state machine of type Moore.
 * Given an initial state, a next-state function, and an output decoding
 * function it creates a Moore process.
 */
template <class IT, class ST, class OT>
class smoore : public sy_process
{
public:
    SY_in<IT>  iport1;        ///< port for the input channel
    SY_out<OT> oport1;        ///< port for the output channel
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(ST&, const ST&, const IT&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(OT&, const ST&)> od_functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    smoore(const sc_module_name& _name,      ///< process name
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
    std::string forsyde_kind() const{return "SY::smoore";}
    
private:
    // Initial value
    ST init_st;
    
    //! The functions passed to the process constructor
    ns_functype _ns_func;
    od_functype _od_func;
    
    bool first_run;
    
    // Input, output, current state, and next state variables
    IT* ival;
    ST* stval;
    ST* nsval;
    OT* oval;

    //Implementing the abstract semantics
    void init()
    {
        ival = new IT;
        stval = new ST;
        *stval = init_st;
        nsval = new ST;
        oval = new OT;
        // First evaluation cycle
        first_run = true;
    }
    
    void prep()
    {
        if (!first_run)
        {
            auto ival_temp = iport1.read();
            CHECK_PRESENCE(ival_temp);
            *ival = unsafe_from_abst_ext(ival_temp);
        }
    }
    
    void exec()
    {
        if (first_run)
            first_run = false;
        else
        {
            _ns_func(*nsval, *stval, *ival);
            *stval = *nsval;
        }
        _od_func(*oval, *stval);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, abst_ext<OT>(*oval))
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

//! Process constructor for a strict Mealy machine
/*! This class is used to build a finite state machine of type Mealy.
 * Given an initial state, a next-state function, and an output decoding
 * function it creates a Mealy process.
 */
template <class IT, class ST, class OT>
class smealy : public sy_process
{
public:
    SY_in<IT>  iport1;        ///< port for the input channel
    SY_out<OT> oport1;        ///< port for the output channel
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(ST&, const ST&, const IT&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(OT&, const ST&, const IT&)> od_functype;
    
    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    smealy(const sc_module_name& _name,      ///< process name
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
    std::string forsyde_kind() const{return "SY::smealy";}
    
private:
    //! The functions passed to the process constructor
    ns_functype _ns_func;
    od_functype _od_func;
    // Initial value
    ST init_st;
    
    // Input, output, current state, and next state variables
    IT* ival;
    ST* stval;
    ST* nsval;
    OT* oval;

    //Implementing the abstract semantics
    void init()
    {
        ival = new IT;
        stval = new ST;
        *stval = init_st;
        nsval = new ST;
        oval = new OT;
    }
    
    void prep()
    {
        auto ival_temp = iport1.read();
        CHECK_PRESENCE(ival_temp);
        *ival = unsafe_from_abst_ext(ival_temp);
    }
    
    void exec()
    {
        _ns_func(*nsval, *stval, *ival);
        _od_func(*oval, *stval, *ival);
        *stval = *nsval;
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, abst_ext<OT>(*oval))
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

//! Process constructor for a strict constant source process
/*! This class is used to build a souce process with constant output.
 * Its main purpose is to be used in test-benches.
 * 
 * This class can directly be instantiated to build a process.
 */
template <class T>
class sconstant : public sy_process
{
public:
    SY_out<T> oport1;            ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    sconstant(const sc_module_name& _name,      ///< process name
              const T& init_val,                ///< The constant output value
              const unsigned long long& take=0  ///< number of tokens produced (0 for infinite)
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
    std::string forsyde_kind() const {return "SY::sconstant";}
    
private:
    T init_val;
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
            WRITE_MULTIPORT(oport1, abst_ext<T>(init_val))
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

//! Process constructor for a strict source process
/*! This class is used to build a souce process which only has an output.
 * Given an initial state and a function, the process repeatedly applies
 * the function to the current state to produce next state, which is
 * also the process output. It can be used in test-benches.
 */
template <class T>
class ssource : public sy_process
{
public:
    SY_out<T> oport1;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(T&, const T&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    ssource(const sc_module_name& _name,    ///< process name
            const functype& _func,          ///< function to be passed
            const T& init_val,              ///< Initial state
            const unsigned long long& take=0///< number of tokens produced (0 for infinite)
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
    std::string forsyde_kind() const {return "SY::ssource";}
    
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
        WRITE_MULTIPORT(oport1, abst_ext<T>(*cur_st))
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
            WRITE_MULTIPORT(oport1, abst_ext<T>(*cur_st))
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

//! Process constructor for a strict source process with vector input
/*! This class is used to build a souce process which only has an output.
 * Given the test bench vector, the process iterates over the emenets
 * of the vector and outputs one value on each evaluation cycle.
 */
template <class T>
class svsource : public sy_process
{
public:
    SY_out<T> oport1;     ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which writes the result using the output
     * port.
     */
    svsource(const sc_module_name& _name,   ///< process name
            const std::vector<T>& in_vec    ///< Initial vector
            ) : sy_process(_name), in_vec(in_vec)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << in_vec;
        arg_vec.push_back(std::make_tuple("in_vec", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::svsource";}
    
private:
    std::vector<T> in_vec;
    
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
            WRITE_MULTIPORT(oport1, abst_ext<T>(in_vec[tok_cnt]))
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

//! Process constructor for a strict sink process
/*! This class is used to build a sink process which only has an input.
 * Its main purpose is to be used in test-benches. The process repeatedly
 * applies a given function to the current input.
 */
template <class T>
class ssink : public sy_process
{
public:
    SY_in<T> iport1;         ///< port for the input channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<void(const T&)> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    ssink(const sc_module_name& _name,      ///< process name
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
    std::string forsyde_kind() const {return "SY::ssink";}
    
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
        auto val_temp = iport1.read();
        CHECK_PRESENCE(val_temp);
        *val = unsafe_from_abst_ext(val_temp);
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

//! The strict zip process with two inputs and one output
/*! This process "zips" two incoming signals into one signal of tuples.
 */
template <class T1, class T2>
class szip : public sy_process
{
public:
    SY_in<T1> iport1;        ///< port for the input channel 1
    SY_in<T2> iport2;        ///< port for the input channel 2
    SY_out<std::tuple<T1,T2>> oport1;///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    szip(const sc_module_name& _name      ///< process name
        )
         :sy_process(_name), iport1("iport1"), iport2("iport2"), oport1("oport1")
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::szip";}
    
private:
    // intermediate values
    T1* ival1;
    T2* ival2;
    
    void init()
    {
        ival1 = new T1;
        ival2 = new T2;
    }
    
    void prep()
    {
        auto ival1_temp = iport1.read();
        auto ival2_temp = iport2.read();
        CHECK_PRESENCE(ival1_temp);
        CHECK_PRESENCE(ival2_temp);
        *ival1 = unsafe_from_abst_ext(ival1_temp);
        *ival2 = unsafe_from_abst_ext(ival2_temp);
    }
    
    void exec() {}
    
    void prod()
    {
        auto outval = abst_ext<std::tuple<T1,T2>>(std::make_tuple(*ival1,*ival2));
        WRITE_MULTIPORT(oport1, outval)  // write to the output
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

//! The strict zipX process with an array of inputs and one output
/*! This process "zips" an array of incoming signals into one signal of arrays.
 */
template <class T1, std::size_t N>
class szipX : public sy_process
{
public:
    std::array<SY_in<T1>,N> iport;      ///< port array for the input channels
    SY_out<std::array<T1,N>> oport1;    ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    szipX(const sc_module_name& _name      ///< process name
        )
         :sy_process(_name), oport1("oport1")
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::szipX";}
    
private:
    // intermediate values
    std::array<T1,N> ival;
    
    void init() {}
    
    void prep()
    {
        for (size_t i=0; i<N; i++)
        {
            auto ival_temp = iport[i].read();
            CHECK_PRESENCE(ival_temp);
            ival[i] = unsafe_from_abst_ext(ival_temp);
        }
    }
    
    void exec() {}
    
    void prod()
    {
        auto oval = abst_ext<std::array<T1,N>>(ival);
        WRITE_MULTIPORT(oport1,oval)  // write to the output
    }
    
    void clean() {}
    
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

//! The strict zip process with variable number of inputs and one output
/*! This process "zips" the incoming signals into one signal of tuples.
 */
template <class... Ts>
class szipN : public sy_process
{
public:
    std::tuple<SY_in<Ts>...> iport;///< tuple of ports for the input channels
    SY_out<std::tuple<Ts...> > oport1;///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    szipN(const sc_module_name& _name      ///< process name
         )
         : sy_process(_name), oport1("oport1")
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::szipN";}
private:
    // intermediate values
    std::tuple<Ts...>* in_vals;
    
    void init()
    {
        in_vals = new std::tuple<Ts...>;
    }
    
    void prep()
    {
        *in_vals = sc_fifo_tuple_read<Ts...>(iport);
    }
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1,abst_ext<std::tuple<Ts...>>(*in_vals))    // write to the output
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
            auto ival_temp = std::get<0>(t).read();
            //~ CHECK_PRESENCE(ival_temp);
            if (is_absent(ival_temp)) SC_REPORT_ERROR("szipN","Unexpected absent value received in");
            std::get<0>(ret) = unsafe_from_abst_ext(ival_temp);
        }
    };

    template<class... T>
    std::tuple<T...> sc_fifo_tuple_read(std::tuple<SY_in<T>...>& ports)
    {
        std::tuple<T...> ret;
        fifo_read_helper<sizeof...(T)-1,
                         std::tuple<T...>,
                         std::tuple<SY_in<T>...>>::read(ret,ports);
        return ret;
    }

};

//! The strict unzip process with one input and two outputs
/*! This process "unzips" a signal of tuples into two separate signals
 */
template <class T1, class T2>
class sunzip : public sy_process
{
public:
    SY_in<std::tuple<T1,T2>> iport1;///< port for the input channel
    SY_out<T1> oport1;        ///< port for the output channel 1
    SY_out<T2> oport2;        ///< port for the output channel 2

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * unzips them and writes the results using the output ports
     */
    sunzip(const sc_module_name& _name      ///< process name
          )
         :sy_process(_name), iport1("iport1"), oport1("oport1"), oport2("oport2")
    {}
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::sunzip";}
private:
    // intermediate values
    abst_ext<std::tuple<T1,T2>>* in_val;
    
    void init()
    {
        in_val = new abst_ext<std::tuple<T1,T2>>;
    }
    
    void prep()
    {
        *in_val = iport1.read();
        CHECK_PRESENCE(*in_val);
    }
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1,abst_ext<T1>(std::get<0>(*in_val)))  // write to the output 1
        WRITE_MULTIPORT(oport2,abst_ext<T2>(std::get<1>(*in_val)))  // write to the output 2
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

//! The sunzipX process with one input and an array of outputs
/*! This process "unzips" a signal of arrays into an array of separate signals
 */
template <class T1, std::size_t N>
class sunzipX : public sy_process
{
public:
    SY_in<std::array<T1,N>> iport1;///< port for the input channel
    std::array<SY_out<T1>,N> oport;///< port array for the output channels

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * unzips them and writes the results using the output ports
     */
    sunzipX(const sc_module_name& _name      ///< process name
          )
         :sy_process(_name), iport1("iport1")
    {}
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::sunzipX";}
private:
    // intermediate values
    abst_ext<std::array<T1,N>>* in_val;
    
    void init()
    {
        in_val = new abst_ext<std::array<T1,N>>;
    }
    
    void prep()
    {
        *in_val = iport1.read();
        CHECK_PRESENCE(*in_val);
    }
    
    void exec() {}
    
    void prod()
    {
        for (size_t i=0; i<N; i++)
            WRITE_MULTIPORT(oport[i],abst_ext<T1>(in_val->unsafe_from_abst_ext()[i]))  // write to the output i
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
        boundOutChans.resize(N);    // output ports
        for (size_t i=0;i<N;i++)
            boundOutChans[i].port = &oport[i];
    }
#endif
};

//! The unzip process with one input and variable number of outputs
/*! This process "unzips" the incoming signal into a tuple of signals.
 */
template <class... Ts>
class sunzipN : public sy_process
{
public:
    SY_in<std::tuple<Ts...>> iport1;///< port for the input channel
    std::tuple<SY_out<Ts>...> oport;///< tuple of ports for the output channels

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * unzips it and writes the results using the output ports
     */
    sunzipN(const sc_module_name& _name      ///< process name
           )
          :sy_process(_name), iport1("iport1")
    { }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::sunzipN";}
private:
    // intermediate values
    abst_ext<std::tuple<Ts...>>* in_val;
    
    void init()
    {
        in_val = new abst_ext<std::tuple<Ts...>>;
    }
    
    void prep()
    {
        *in_val = iport1.read();
        CHECK_PRESENCE(*in_val);
    }
    
    void exec() {}
    
    void prod()
    {
        fifo_tuple_write<Ts...>(in_val->unsafe_from_abst_ext(), oport);
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
    void fifo_tuple_write(const std::tuple<T...>& vals,
                             std::tuple<SY_out<T>...>& ports)
    {
        fifo_write_helper<sizeof...(T)-1,
                          std::tuple<T...>,
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

//! The strict group process with one input and one output
/*! It groups values into a vector of specified size n, which takes n
 * cycles. While the grouping takes place the output from this process
 * consists of absent values.
 */
template <class T>
class sgroup : public sy_process
{
public:
    SY_in<T> iport1;                    ///< port for the input channel
    SY_out<std::vector<T>> oport1;      ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * groups together n samples and writes the results using the output
     * port.
     */
    sgroup(const sc_module_name& _name,      ///< process name
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
    std::string forsyde_kind() const {return "SY::sgroup";}
    
private:
    // Number of samples in each group
    unsigned long samples;
    
    unsigned long samples_took;
    
    // The output vector
    std::vector<T>* oval;
    
    //Implementing the abstract semantics
    void init()
    {
        oval = new std::vector<T>;
        oval->resize(samples);
        samples_took = 0;
    }
    
    void prep()
    {
        auto val_temp = iport1.read();
        CHECK_PRESENCE(val_temp);
        (*oval)[samples_took] = unsafe_from_abst_ext(val_temp);
        samples_took++;
    }
    
    void exec() {}
    
    void prod()
    {
        if (samples_took==samples)
        {
            WRITE_MULTIPORT(oport1, abst_ext<std::vector<T>>(*oval))
            samples_took = 0;
        }
        else
            WRITE_MULTIPORT(oport1, abst_ext<std::vector<T>>())
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

}
}

#endif
