/**********************************************************************
    * mis.hpp -- MoC interfaces for the SystemC map of the ForSyDe    *
    *          library                                                *
    *                                                                 *
    * Authors: Gilmar Besera (gilmar@kth.se)                          *
    *          Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing MoC interfaces for ForSyDe-SystemC           *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef MIS_HPP
#define MIS_HPP

/*! \file mis.hpp
 * \brief Implements the MoC interfaces between different MoCs
 * 
 *  This file includes the basic process constructors and other
 * facilities used for creating MoC interfaces between different MoCs.
 */

namespace ForSyDe
{
using namespace sc_core;

//! Operation modes for the SY2CT converter
enum A2DMode {LINEAR, HOLD};

//! Process constructor for a SY2CT MoC interfaces
/*! This class is used to build a MoC interface which converts an SY 
 * signal to a CT one. It can be used to implement digital-to-analog
 * converters. There are two operating modes which can be configured using
 * the initial values of the constructor:
 * - sample and hold
 * - linear interpolation
 */
class SY2CT : public process
{
public:
    SY::SY_in<CTTYPE> iport1;      ///< port for the input channel
	CT::CT_out oport1;              ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    SY2CT(sc_module_name _name,      ///< process name
          sc_time sample_period,     ///< The sampling period
          A2DMode op_mode = HOLD    ///< The operation mode
          ) : process(_name), iport1("iport1"), oport1("oport1"),
              sample_period(sample_period), op_mode(op_mode)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << sample_period;
        arg_vec.push_back(std::make_tuple("sample_period", ss.str()));
        ss.str("");
        ss << op_mode;
        arg_vec.push_back(std::make_tuple("op_mode", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "MI::SY2CT";}

private:
    sc_time sample_period;
	A2DMode op_mode;
    
    // Internal variables
    CTTYPE previousVal, currentVal;
    sub_signal subsig;
    unsigned long iter;
    
    //Implementing the abstract semantics
    void init()
    {
        currentVal = previousVal = 0;
        iter = 0;
    }
    
    void prep()
    {
        currentVal = (CTTYPE)from_abst_ext(iport1.read(), previousVal);
    }
    
    void exec()
    {
        set_range(subsig, sample_period*iter, sample_period*(iter+1));
        if(op_mode==HOLD)
        {
            CTTYPE pv = previousVal;
            set_function(subsig,[pv](const sc_time& t)
                                {
                                    return pv;
                                }
            );
        }
        else 
        {
            CTTYPE dv = currentVal - previousVal;
            CTTYPE pv = previousVal;
            unsigned long itr = iter;
            sc_time sp = sample_period;
            set_function(subsig,[pv,itr,sp,dv](const sc_time& t)
            {
                return (t-itr*sp)/sp*dv + pv;
            });
        }
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, subsig)
        wait(get_end_time(subsig) - sc_time_stamp());
        iter++;
        previousVal = currentVal;
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

//! Process constructor for a CT2SY MoC interface
/*! This class is used to build a MoC interface which converts an CT 
 * signal to a SY one with fixed sampling rate. It can be used to implement 
 * analog-to-digital converters.
 */
class CT2SY : public process
{
public:
    CT::CT_in iport1;           ///< port for the input channel
    SY::SY_out<CTTYPE> oport1; ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    CT2SY(sc_module_name _name,      ///< process name
          sc_time sample_period      ///< The sampling period
          ) : process(_name), iport1("iport1"), oport1("oport1"),
              sample_period(sample_period)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << sample_period;
        arg_vec.push_back(std::make_tuple("sample_period", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "MI::CT2SY";}

private:
    sc_time sample_period;
    
    // Internal variables
    sub_signal in_ss;
    CTTYPE out_val;
    sc_time local_time, sampling_time;
    
    //Implementing the abstract semantics
    void init()
    {
        local_time = sampling_time = SC_ZERO_TIME;
    }
    
    void prep()
    {
        while (sampling_time >= local_time)
        {
            in_ss = iport1.read();
            local_time = get_end_time(in_ss);
        }
    }
    
    void exec()
    {
        out_val = in_ss(sampling_time);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, out_val)
        wait(sampling_time - sc_time_stamp());
        sampling_time += sample_period;
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

//! Process constructor for a CT2DDE MoC interface
/*! This class is used to build a MoC interface which converts an CT 
 * signal to a DDE one with adaptive sampling rate. It can be used to
 * implement analog-to-digital converters with adaptive sampling rates.
 */
template<class T>
class CT2DDE : public process
{
public:
    CT::CT_in iport1;               ///< port for the input channel
    DDE::DDE_in<unsigned int> iport2; ///< port for the sampling channel
    DDE::DDE_out<T> oport1;           ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    CT2DDE(sc_module_name _name       ///< process name
          ) : process(_name), iport1("iport1"), oport1("oport1")
    {}
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "MI::CT2DDE";}

private:    
    // Internal variables
    sub_signal f;
    std::vector<sub_signal > vecCTsignal; // a queue to be committed
    //~ sub_signal in_val;
    abst_ext<T> out_val;
    sc_time samplingT;
    unsigned int samplingType, iter;
    
    //Implementing the abstract semantics
    void init()
    {
        iter = 0;
        //~ in_val = iport1.read();
        //~ cur_time = get_start_time(in_val);
    }
    
    void prep()
    {
        //~ while (cur_time >= get_end_time(in_val)) in_val = iport1.read();
        auto e = iport2.read();
        samplingType = unsafe_from_abst_ext(get_value(e)); // FIXME: what if absent?
        samplingT = get_time(e);
    }
    
    void exec() {}
    
    void prod()
    {
        //FIXME: this code should be split between prep, prod, and probably exec
        if(samplingType!=1)
        { 
            // just sampling (without commitment) in 
            // adaptive mode '0',  or non-adapitve mode '2'
            if(iter==0)
            { 
                f = iport1.read();
                if(samplingType==0)
                    vecCTsignal.push_back(f);
            }
            if((samplingT >= get_start_time(f)) && (samplingT < get_end_time(f)))
            {
                WRITE_MULTIPORT(oport1,ttn_event<T>(f(samplingT), samplingT))
                wait(samplingT - sc_time_stamp());
            }
            else if(samplingT >= get_end_time(f))
            {
                f = iport1.read();
                if(samplingType==0)
                    vecCTsignal.push_back(f);
                if ((samplingT >= get_start_time(f)) && (samplingT < get_end_time(f)))
                {
                    WRITE_MULTIPORT(oport1,ttn_event<T>(f(samplingT), samplingT))
                    wait(samplingT - sc_time_stamp());
                }
                else
                {
                    while(samplingT >= get_end_time(f))
                    {
                        f = iport1.read();
                        if(samplingType==0)
                            vecCTsignal.push_back(f);
                    }
                    WRITE_MULTIPORT(oport1,ttn_event<T>(f(samplingT), samplingT))
                    wait(samplingT - sc_time_stamp());
                }
            }
            else
            {
                // To check the sampling from the queue
                while(!vecCTsignal.empty())
                {
                    if(samplingT >= get_end_time(vecCTsignal.front()))
                        vecCTsignal.erase(vecCTsignal.begin());
                    else
                    {
                        WRITE_MULTIPORT(oport1,ttn_event<T>(vecCTsignal.front()(samplingT), samplingT))
                        wait(samplingT - sc_time_stamp());
                        break;
                    }
                }
                if(vecCTsignal.empty())
                    assert(0);  // if have not get the sampling
            }
        }
        else
        {
            // a commitment event in adaptive mode '1'
            while(!vecCTsignal.empty())
            {
                if(samplingT >= get_end_time(vecCTsignal.front()))
                    vecCTsignal.erase(vecCTsignal.begin());
                else
                    break;
            }
            if(vecCTsignal.empty())
                assert(0);  // if have not get the sampling
        }
        iter++;
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

//! Process constructor for a CT2DDEf MoC interface
/*! This class is used to build a MoC interface which converts a CT 
 * signal to a DDE one with fixed sampling rate. It can be used to
 * implement analog-to-digital converters with fixed sampling rates.
 */
template<class T>
class CT2DDEf : public process
{
public:
    CT::CT_in iport1;               ///< port for the input channel
    DDE::DDE_out<T> oport1;           ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    CT2DDEf(sc_module_name _name,    ///< process name
           sc_time samp_period      ///< sampling period
          ) : process(_name), iport1("iport1"), oport1("oport1"), samp_period(samp_period)
    {}
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "MI::CT2DDEf";}

private:    
    // Internal variables
    sc_time samp_period;
    abst_ext<T> out_val;
    sc_time local_time, sampling_time;
    sub_signal in_ss;
    
    //Implementing the abstract semantics
    void init()
    {
        local_time = sampling_time = SC_ZERO_TIME;
    }
    
    void prep()
    {
        while (sampling_time >= local_time)
        {            
            in_ss = iport1.read();
            local_time = get_end_time(in_ss);
        }
    }
    
    void exec()
    {
        out_val = abst_ext<T>(in_ss(sampling_time));
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1,ttn_event<T>(out_val, sampling_time))
        wait(sampling_time - sc_time_stamp());
        sampling_time += samp_period;
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

//! Process constructor for a DDE2CT MoC interfaces
/*! This class is used to build a MoC interfaces which converts a DDE 
 * signal to a CT one. It can be used to implement digital-to-analog
 * converters. There are two operating modes which can be configured using
 * the initial values of the constructor:
 * - sample and hold
 * - linear interpolation
 */
template<class T>
class DDE2CT : public process
{
public:
    DDE::DDE_in<T> iport1;        ///< port for the input channel
	CT::CT_out oport1;          ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    DDE2CT(sc_module_name _name,      ///< process name
          A2DMode op_mode = HOLD    ///< The operation mode
          ) : process(_name), iport1("iport1"), oport1("oport1"),
              op_mode(op_mode)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << op_mode;
        arg_vec.push_back(std::make_tuple("op_mode", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "MI::DDE2CT";}

private:
	A2DMode op_mode;
    
    // Internal variables
    CTTYPE previousVal, currentVal;
    sc_time previousT, currentT;
    sub_signal subsig;
    
    //Implementing the abstract semantics
    void init()
    {
        previousVal = currentVal = 0;
        previousT = currentT = SC_ZERO_TIME;
    }
    
    void prep()
    {
        auto in_ev = iport1.read();
        currentVal = (double)from_abst_ext(get_value(in_ev), previousVal);
        currentT = get_time(in_ev);
    }
    
    void exec()
    {
        set_range(subsig, previousT, currentT);
        if(op_mode==HOLD)
        {
            CTTYPE pv = previousVal;
            set_function(subsig,[=](sc_time t){
						return pv;
						});
        }
        else 
        {
            CTTYPE dv = currentVal - previousVal;
            sc_time dt = currentT - previousT;
            sc_time pt = previousT;
            CTTYPE pv = previousVal;
            set_function(subsig,[=](sc_time t)->CTTYPE{
                    return ((t-pt)/dt*dv + pv);
            });
        }
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, subsig)
        wait(get_end_time(subsig) - sc_time_stamp());
        previousVal = currentVal;
        previousT = currentT;
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

//! Process constructor for a SY2SDF MoC interfaces
/*! This class is used to build a MoC interface which converts an SY 
 * signal to an SDF one.
 */
template<class T>
class SY2SDF : public process
{
public:
    SY::SY_in<T> iport1;        ///< port for the input channel
	SDF::SDF_out<T> oport1;     ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    SY2SDF(sc_module_name _name     ///< process name
          ) : process(_name), iport1("iport1"), oport1("oport1")
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << 1;
        arg_vec.push_back(std::make_tuple("o1toks", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "MI::SY2SDF";}

private:
    
    // Internal variables
    T* val;
    
    //Implementing the abstract semantics
    void init()
    {
        val = new T;
    }
    
    void prep()
    {
        auto tok = iport1.read();
        while (is_absent(tok))
            tok = iport1.read();
        *val = unsafe_from_abst_ext(tok);
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

//! Process constructor for a SDF2SY MoC interface
/*! This class is used to build a MoC interface which converts an SDF 
 * signal to a SY one.
 */
template<class T>
class SDF2SY : public process
{
public:
    SDF::SDF_in<T> iport1;  ///< port for the input channel
    SY::SY_out<T> oport1;   ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    SDF2SY(sc_module_name _name     ///< process name
          ) : process(_name), iport1("iport1"), oport1("oport1")
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << 1;
        arg_vec.push_back(std::make_tuple("i1toks", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "MI::SDF2SY";}

private:
    
    // Internal variables
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
        WRITE_MULTIPORT(oport1, abst_ext<T>(*val))
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

//! Process constructor for a SY2DDE MoC interfaces
/*! This class is used to build a MoC interface which converts an SY 
 * signal to a DDE one.
 */
template<class T>
class SY2DDE : public process
{
public:
    SY::SY_in<T> iport1;        ///< port for the input channel
	DDE::DDE_out<T> oport1;       ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    SY2DDE(sc_module_name _name,     ///< process name
          sc_time sample_period     ///< The unified period length
          ) : process(_name), iport1("iport1"), oport1("oport1"),
              sample_period(sample_period)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << sample_period;
        arg_vec.push_back(std::make_tuple("sample_period", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "MI::SY2DDE";}

private:
    sc_time sample_period;
    
    // Internal variables
    abst_ext<T>* tok;
    T* val;
    sc_time cur_time;
    
    //Implementing the abstract semantics
    void init()
    {
        tok = new abst_ext<T>();
        val = new T;
        cur_time = SC_ZERO_TIME;
    }
    
    void prep()
    {
        *tok = iport1.read();
        if (is_present(*tok))
            *val = unsafe_from_abst_ext(*tok);
    }
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, tt_event<T>(*val,cur_time))
        wait(cur_time - sc_time_stamp());
        cur_time += sample_period;
    }
    
    void clean()
    {
        delete val;
        delete tok;
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

//! Process constructor for a DDE2SY MoC interface
/*! This class is used to build a MoC interface which converts a DDE 
 * signal to an SY one.
 */
template<class T>
class DDE2SY : public process
{
public:
    DDE::DDE_in<T> iport1;  ///< port for the input channel
    SY::SY_out<T> oport1;   ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    DDE2SY(sc_module_name _name,     ///< process name
          sc_time sample_period     ///< The unified period length
          ) : process(_name), iport1("iport1"), oport1("oport1"),
              sample_period(sample_period)
    {
#ifdef FORSYDE_INTROSPECTION
        std::stringstream ss;
        ss << sample_period;
        arg_vec.push_back(std::make_tuple("sample_period", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "MI::DDE2SY";}

private:
    sc_time sample_period;
    
    // Internal variables
    tt_event<T>* tok;
    T* prev_val;
    sc_time cur_time;
    
    //Implementing the abstract semantics
    void init()
    {
        tok = new tt_event<T>();
        prev_val = new T;
        cur_time = SC_ZERO_TIME;
        *tok = iport1.read();
    }
    
    void prep()
    {
        while (get_time(*tok) <= cur_time)
        {
            *prev_val = get_value(*tok);
            *tok = iport1.read();
        }
    }
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, abst_ext<T>(*prev_val))
        cur_time += sample_period;
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

}

#endif
