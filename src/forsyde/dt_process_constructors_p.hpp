/**********************************************************************           
    * dt_process_constructors.hpp -- Process constructors in the DT   *
    *                          MOC with timer based process invocation*
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing basic process constructors for modeling      *
    *          discrete-time systems in ForSyDe-SystemC               *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DT_PROCESS_CONSTRUCTORS_P_HPP
#define DT_PROCESS_CONSTRUCTORS_P_HPP

/*! \file dt_process_constructors.hpp
 * \brief Implements the basic process constructors in the DT MoC with timer invocation
 * 
 *  This file includes the basic process constructors used for modeling
 * in the discrete-time model of computation.
 */

#include <functional>
#include <tuple>

#include "abst_ext.hpp"
#include "dt_process.hpp"

namespace ForSyDe
{

namespace DT
{

namespace P
{

using namespace sc_core;

//! Process constructor for a mealy machine
/*! This class is used to build a timed Mealy state machine.
 * Given a partitioning function, a next-state function, an output decoding
 * function, and an initial state, it creates a timed Mealy process.
 */
template <class IT, class ST, class OT>
class mealy : public dt_process
{
public:
    DT_in<IT>  iport1;        ///< port for the input channel
    DT_out<OT> oport1;        ///< port for the output channel
    
    //! Type of the partitioning function to be passed to the process constructor
    typedef std::function<void(size_t&, const ST&)> gamma_functype;
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(ST&, 
                                const ST&,
                                const std::vector<IT>&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(std::vector<OT>&, 
                                const ST&,
                                const std::vector<IT>&)> od_functype;
    
    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented functions to the input and current
     * state and writes the results using the output port
     */
    mealy(sc_module_name _name,    ///< The module name
           gamma_functype gamma,    ///< The input partitioning function
           ns_functype _ns_func,    ///< The next_state function
           od_functype _od_func,    ///< The output-decoding function
           ST init_st               ///< Initial state
          ) : dt_process(_name), gamma(gamma), _ns_func(_ns_func),
              _od_func(_od_func), init_st(init_st)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("gamma",func_name+std::string("_gamma")));
        arg_vec.push_back(std::make_tuple("_ns_func",func_name+std::string("_ns_func")));
        arg_vec.push_back(std::make_tuple("_od_func",func_name+std::string("_od_func")));
        std::stringstream ss;
        ss << init_st;
        arg_vec.push_back(std::make_tuple("init_st",ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const{return "DT::P::mealy";}
    
private:    
    //! The functions passed to the process constructor
    gamma_functype gamma;
    ns_functype _ns_func;
    od_functype _od_func;
    
    // Initial value
    ST init_st;
    
    // Input, output, current state, and next state variables
    std::vector<IT> ivals;
    ST* stval;
    ST* nsval;
    std::vector<OT> ovals;
    
    size_t itoks;

    // Whether the function should be invoked in this iteration
    bool invoke;
    
    // The current input/output time
    size_t tin;
    size_t tout;
    size_t k;
    
    //Implementing the abstract semantics
    void init()
    {
        tin = tout = k = 0;
        stval = new ST;
        *stval = init_st;
        nsval = new ST;
    }
    
    void prep()
    {
        // Determine the number of event to be read
        gamma(itoks, *stval);
        // Read the input events
        ivals.clear();
        for (size_t i=0; i<itoks; i++)
        {
            auto tmp = iport1.read();
            if (is_present(tmp))
                ivals.push_back(unsafe_from_abst_ext(tmp));
        }
        // Update tin with the number of tokens read
        tin += itoks;
    }
    
    void exec()
    {
        _ns_func(*nsval, *stval, ivals);
        _od_func(ovals, *stval, ivals);
        *stval = *nsval;
    }
    
    void prod()
    {
        // Update k
        k = std::max((int)tin-(int)tout-1, 0);

        // First write the required absent events to ensure casaulity
        for (size_t i=0; i<k; i++)
            write_multiport(oport1, abst_ext<OT>());

        // Then write out the result
        write_vec_multiport(oport1, ovals);

        // Update tout with the total number of written tokens
        tout += (k+ovals.size());
        
        // clean up the input and output vectors
        ivals.clear();
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
template<typename TO_tuple, typename TI_tuple, typename TS> class mealyMN;

template <typename... TOs, typename... TIs, typename TS>
class mealyMN<std::tuple<TOs...>,std::tuple<TIs...>,TS>: public dt_process
{
public:
    std::tuple<DT_in<TIs>...>  iport;///< tuple of ports for the input channels
    std::tuple<DT_out<TOs>...> oport;///< tuple of ports for the output channels
    
    //! Type of the partitioning function to be passed to the process constructor
    typedef std::function<void(size_t&,
                                const TS&)> gamma_functype;
    
    //! Type of the next-state function to be passed to the process constructor
    typedef std::function<void(TS&,
                                const TS&,
                                const std::tuple<std::vector<TIs>...>&)> ns_functype;
    
    //! Type of the output-decoding function to be passed to the process constructor
    typedef std::function<void(std::tuple<std::vector<TOs>...>&,
                                const TS&,
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
            const TS& init_st                   ///< Initial state
            ) : dt_process(_name), _gamma_func(_gamma_func), _ns_func(_ns_func),
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
    std::string forsyde_kind() const{return "DT::P::mealyMN";}
    
private:
    //! The functions passed to the process constructor
    gamma_functype _gamma_func;
    ns_functype _ns_func;
    od_functype _od_func;
    // Initial value
    TS init_st;
    // consumption rates
    size_t itoks;
    
    // Input, output, current state, and next state variables
    std::tuple<std::vector<TOs>...>* ovals;
    TS* stvals;
    TS* nsvals;
    std::tuple<std::vector<TIs>...>* ivals;

    // The current input/output time
    std::array<size_t, sizeof...(TOs)> ks;
    size_t tin;
    std::array<size_t, sizeof...(TOs)> touts;

    //Implementing the abstract semantics
    void init()
    {
        tin = 0;
        std::fill_n(touts.begin(), sizeof...(TOs), 0);
        std::fill_n(ks.begin(), sizeof...(TOs), 0);
        ovals = new std::tuple<std::vector<TOs>...>;
        stvals = new TS;
        *stvals = init_st;
        nsvals = new TS;
        ivals = new std::tuple<std::vector<TIs>...>;
    }
    
    void prep()
    {
        // Determine the number of event to be read
        _gamma_func(itoks, *stvals);
        // Size the input and output buffers
        std::apply([&](auto&... ival) {
            (ival.clear(), ...);
        }, *ivals);
        // Read the input tokens
        std::apply([&](auto&... inport) {
            std::apply([&](auto&... ival) {
                (
                    [&ival,&inport,this](){
                        for (size_t i=0; i<itoks; i++)
                        {
                            auto tmp = inport.read();
                            if (is_present(tmp))
                                ival.push_back(unsafe_from_abst_ext(tmp));
                        }
                    }()
                , ...);
            }, *ivals);
        }, iport);
        // update tin with the number of tokens read
        tin += itoks;
    }
    
    void exec()
    {
        _ns_func(*nsvals, *stvals, *ivals);
        _od_func(*ovals, *stvals, *ivals);
        *stvals = *nsvals;
    }
    
    void prod()
    {
        // Update ks
        for (size_t i=0; i<sizeof...(TOs); i++)
            ks[i] = std::max((int)tin-(int)touts[i]-1, 0);

        // First write the required absent events to ensure casaulity
        std::apply([&](auto&... oport) {
            std::apply([&](auto&&... val){
                size_t n{0};
                (
                    [&oport,&val,&n,this](){
                        for (size_t i=0;i<ks[n];i++)
                        {   
                            using T = std::decay_t<decltype(val[0])>;
                            write_multiport(oport, abst_ext<T>());
                        }
                        n++;
                    }()
                , ...);
            }, *ovals);
        }, oport);
        
        // Then write out the result
        std::apply([&](auto&&... port){
            std::apply([&](auto&&... val){
                (write_vec_multiport(port, val), ...);
            }, *ovals);
        }, oport);

        // Update tout with the total number of written tokens
        std::apply([&](auto&&... val){
            size_t n{0};
            ((touts[n] += (ks[n]+val.size())), ...);
            n++;
            (val.clear(), ...);
        }, *ovals);
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
        [&]()
        {
            std::size_t n{0};
            ((boundInChans[n++].portType = typeid(TIs).name()),...);
        }();
        boundOutChans.resize(sizeof...(TOs));    // output ports
        std::apply
        (
            [&](auto&... ports)
            {
                std::size_t n{0};
                ((boundOutChans[n++].port = &ports),...);
            }, oport
        );
        [&]()
        {
            std::size_t n{0};
            ((boundOutChans[n++].portType = typeid(TOs).name()),...);
        }();
    }
#endif
};



}
}
}

#endif
