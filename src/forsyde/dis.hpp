/**********************************************************************
    * dis.hpp -- domain interfaces for the SystemC map of the ForSyDe *
    *          library                                                *
    *                                                                 *
    * Authors: Gilmar Besera (gilmar@kth.se)                          *
    *          Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing domain interfaces for ForSyDe-SystemC        *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DIS_HPP
#define DIS_HPP

/*! \file dis.hpp
 * \brief Implements the domain interfaces between different MoCs
 * 
 *  This file includes the basic process constructors and other
 * facilities used for creating domain interfaces between different MoCs.
 */

namespace ForSyDe
{
using namespace sc_core;

//! Operation modes for the SY2CT converter
enum A2DMode {LINEAR, HOLD};

//! Process constructor for a SY2CT domain interfaces
/*! This class is used to build a domain interfaces which converts an SY 
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
    std::string forsyde_kind() const {return "SY2CT";}

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
        currentVal = (CTTYPE)from_abst_ext(iport1.read(), previousVal);
        iter = 0;
    }
    
    void prep()
    {
        previousVal = currentVal;
        currentVal = (CTTYPE)from_abst_ext(iport1.read(), previousVal);
    }
    
    void exec()
    {
        set_range(subsig, sample_period*iter, sample_period*(iter+1));
        if(op_mode==HOLD)
        {
            // FIXME: the following intermediate variables shouldn't be necassary
            //        but generates an error in GCC 4.7
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
            // FIXME: the following intermediate variables shouldn't be necassary
            //        but generates an error in GCC 4.7
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
        iter++;
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

//! Process constructor for a CT2SY domain interface
/*! This class is used to build a domain interface which converts an CT 
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
    std::string forsyde_kind() const {return "CT2SY";}

private:
    sc_time sample_period;
    
    // Internal variables
    sub_signal in_val;
    CTTYPE out_val;
    sc_time cur_time;
    
    //Implementing the abstract semantics
    void init()
    {
        in_val = iport1.read();
        cur_time = get_start_time(in_val);
    }
    
    void prep()
    {
        while (cur_time >= get_end_time(in_val)) in_val = iport1.read();
    }
    
    void exec()
    {
        out_val = in_val(cur_time);
    }
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, out_val)
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

//! Process constructor for a CT2DE domain interface
/*! This class is used to build a domain interface which converts an CT 
 * signal to a DE one with adaptive sampling rate. It can be used to
 * implement analog-to-digital converters with adaptive sampling rates.
 */
template<class T>
class CT2DE : public process
{
public:
    CT::CT_in iport1;               ///< port for the input channel
    DE::DE_in<unsigned int> iport2; ///< port for the sampling channel
    DE::DE_out<T> oport1;           ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    CT2DE(sc_module_name _name       ///< process name
          ) : process(_name), iport1("iport1"), oport1("oport1")
    {}
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT2DE";}

private:    
    // Internal variables
    sub_signal f;
    std::vector<sub_signal > vecCTsignal; // a queue to be committed
    //~ sub_signal in_val;
    T out_val;
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
        samplingType = get_value(e);
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
                WRITE_MULTIPORT(oport1,tt_event<T>(f(samplingT), samplingT))
            else if(samplingT >= get_end_time(f))
            {
                f = iport1.read();
                if(samplingType==0)
                    vecCTsignal.push_back(f);
                if ((samplingT >= get_start_time(f)) && (samplingT < get_end_time(f)))
                    WRITE_MULTIPORT(oport1,tt_event<T>(f(samplingT), samplingT))
                else
                {
                    while(samplingT >= get_end_time(f))
                    {
                        f = iport1.read();
                        if(samplingType==0)
                            vecCTsignal.push_back(f);
                    }
                    WRITE_MULTIPORT(oport1,tt_event<T>(f(samplingT), samplingT))
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
                        WRITE_MULTIPORT(oport1,tt_event<T>(vecCTsignal.front()(samplingT), samplingT))
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

//! Process constructor for a DE2CT domain interfaces
/*! This class is used to build a domain interfaces which converts a DE 
 * signal to a CT one. It can be used to implement digital-to-analog
 * converters. There are two operating modes which can be configured using
 * the initial values of the constructor:
 * - sample and hold
 * - linear interpolation
 */
template<class T>
class DE2CT : public process
{
public:
    DE::DE_in<T> iport1;        ///< port for the input channel
	CT::CT_out oport1;          ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    DE2CT(sc_module_name _name,      ///< process name
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
    std::string forsyde_kind() const {return "DE2CT";}

private:
	A2DMode op_mode;
    
    // Internal variables
    CTTYPE previousVal, currentVal;
    sc_time previousT, currentT;
    sub_signal subsig;
    
    //Implementing the abstract semantics
    void init()
    {
        auto in_ev = iport1.read();
        currentVal = (double)get_value(in_ev);
        currentT = get_time(in_ev);
    }
    
    void prep()
    {
        previousVal = currentVal;
        previousT = currentT;
        
        auto in_ev = iport1.read();
        currentVal = (double)get_value(in_ev);
        currentT = get_time(in_ev);
    }
    
    void exec()
    {
        set_range(subsig, previousT, currentT);
        if(op_mode==HOLD)
        {
            // FIXME: the following intermediate variables shouldn't be necassary
            //        but generates an error in GCC 4.7
            CTTYPE pv = previousVal;
            set_function(subsig,[=](sc_time t){
						return pv;
						});
        }
        else 
        {
            CTTYPE dv = currentVal - previousVal;
            sc_time dt = currentT - previousT;
            // FIXME: the following intermediate variables shouldn't be necassary
            //        but generates an error in GCC 4.7
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
