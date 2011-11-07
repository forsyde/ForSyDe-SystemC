/**********************************************************************           
    * sdfmoc.hpp -- The synchronous data-flow model of computation    *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing promitive element required for modeling      *
    *          SDF graphs in ForSyDe-SystemC                          *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SDFMOC_HPP
#define SDFMOC_HPP

/*! \file sdfmoc.h
 * \brief Implements the Synchronous Data-Flow Model of Computation
 * 
 *  This file includes the basic process constructors and other
 * facilities used for modeling in the synchronous data-flow model of
 * computation.
 */
 
#include <vector>

namespace ForSyDe
{
//! The namespace for SDF MoC
/*! This namespace includes constructs used for building models in the
 * synchronous dataflow MoC.
 */ 
namespace SDF
{

using namespace sc_core;

// Auxilliary Macro definitions
#define WRITE_MULTIPORT(PORT,VAL) \
    for (int i=0;i<PORT.size();i++) PORT[i]->write(VAL);

#define WRITE_VEC_MULTIPORT(PORT,VEC,TOKS) \
    for (int i=0;i<PORT.size();i++) \
        for (unsigned int j=0;j<TOKS;j++) PORT[i]->write(VEC[j]);

//! Process constructor for a combinational process with one input and one output
/*! This class is used to build combinational processes with one input
 * and one output. The class is parameterized for input and output
 * data-types.
 * 
 * Note that this is an abstract class and can not be directly
 * instantiated. The designer should derive from this class and
 * implement the _func function which is applied on the input to obtain
 * the output on each evaluation cycle.
 */
template <class ITYP, class OTYP>
class comb : public sc_module
{
public:
    sc_fifo_in<ITYP>  iport;        ///< port for the input channel
    sc_fifo_out<OTYP> oport;        ///< port for the output channel

    //! The constructor requires the module name and the number of tokens to be produced and consumed
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb(sc_module_name _name, unsigned int itoks, unsigned int otoks)
         :sc_module(_name), citoks(itoks), cotoks(otoks)
    {
        SC_THREAD(worker);
    }
private:
    unsigned int citoks, cotoks;
    SC_HAS_PROCESS(comb);

    //! The main and only execution thread of the module
    void worker()
    {
        std::vector<ITYP> in_vals(citoks);
        std::vector<OTYP> out_vals(cotoks);
        while (1)
        {
            for (unsigned int i=0;i<citoks;i++) in_vals[i] = iport.read();  // read from input
            out_vals = _func(in_vals);// do the calculation
             WRITE_VEC_MULTIPORT(oport,out_vals,cotoks);  // write to the output
        }
    }

protected:
    //! The main caclulation function
    /*! It is abstract and the user should provide an implementation for
     * it in the derived class.
     */
    virtual std::vector<OTYP> (_func)(std::vector<ITYP>) = 0;
};

//! Process constructor for a combinational process with two inputs and one output
/*! similar to comb with two inputs
 */
template <class I1TYP, class I2TYP, class OTYP>
class comb2 : public sc_module
{
public:
    sc_fifo_in<I1TYP> iport1;       ///< port for the input channel 1
    sc_fifo_in<I2TYP> iport2;       ///< port for the input channel 2
    sc_fifo_out<OTYP> oport;        ///< port for the output channel

    //! The constructor requires the module name and the number of tokens to be produced and consumed
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb2(sc_module_name _name, unsigned int i1toks, unsigned int i2toks,
             unsigned int otoks)
         :sc_module(_name), ci1toks(i1toks), ci2toks(i2toks), cotoks(otoks)
    {
        SC_THREAD(worker);
    }
private:
    unsigned int ci1toks, ci2toks, cotoks;
    SC_HAS_PROCESS(comb2);

    //! The main and only execution thread of the module
    void worker()
    {
        std::vector<I1TYP> in_vals1(ci1toks);
        std::vector<I2TYP> in_vals2(ci2toks);
        std::vector<OTYP> out_vals(cotoks);
        while (1)
        {
            for (unsigned int i=0;i<ci1toks;i++) in_vals1[i] = iport1.read();  // read from input
            for (unsigned int i=0;i<ci2toks;i++) in_vals2[i] = iport2.read();  // read from input
            out_vals = _func(in_vals1, in_vals2);// do the calculation
            WRITE_VEC_MULTIPORT(oport,out_vals,cotoks);    // write to the output
        }
    }

protected:
    //! The main caclulation function
    /*! It is abstract and the user should provide an implementation for
     * it in the derived class.
     */
    virtual std::vector<OTYP> (_func)(std::vector<I1TYP>, std::vector<I2TYP>) = 0;
};

//! Process constructor for a combinational process with three inputs and one output
/*! similar to comb with three inputs
 */
template <class I1TYP, class I2TYP, class I3TYP, class OTYP>
class comb3 : public sc_module
{
public:
    sc_fifo_in<I1TYP> iport1;       ///< port for the input channel 1
    sc_fifo_in<I2TYP> iport2;       ///< port for the input channel 2
    sc_fifo_in<I3TYP> iport3;       ///< port for the input channel 3
    sc_fifo_out<OTYP> oport;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb3(sc_module_name _name, unsigned int i1toks, unsigned int i2toks,
             unsigned int i3toks, unsigned int otoks)
         :sc_module(_name), ci1toks(i1toks), ci2toks(i2toks), ci3toks(i3toks), cotoks(otoks)
    {
        SC_THREAD(worker);
    }
private:
    unsigned int ci1toks, ci2toks, ci3toks, cotoks;
    SC_HAS_PROCESS(comb3);

    //! The main and only execution thread of the module
    void worker()
    {
        std::vector<I1TYP> in_vals1(ci1toks);
        std::vector<I2TYP> in_vals2(ci2toks);
        std::vector<I3TYP> in_vals3(ci3toks);
        std::vector<OTYP> out_vals(cotoks);
        while (1)
        {
            for (unsigned int i=0;i<ci1toks;i++) in_vals1[i] = iport1.read();  // read from input
            for (unsigned int i=0;i<ci2toks;i++) in_vals2[i] = iport2.read();  // read from input
            for (unsigned int i=0;i<ci3toks;i++) in_vals3[i] = iport3.read();  // read from input
            out_vals = _func(in_vals1, in_vals2, in_vals3);// do the calculation
            WRITE_VEC_MULTIPORT(oport,out_vals,cotoks);    // write to the output
        }
    }

protected:
    //! The main caclulation function
    /*! It is abstract and the user should provide an implementation for
     * it in the derived class.
     */
    virtual std::vector<OTYP> (_func)(std::vector<I1TYP>, std::vector<I2TYP>, std::vector<I3TYP>) = 0;
};

//! Process constructor for a combinational process with four inputs and one output
/*! similar to comb with three inputs
 */
template <class I1TYP, class I2TYP, class I3TYP, class I4TYP,class OTYP>
class comb4 : public sc_module
{
public:
    sc_fifo_in<I1TYP> iport1;       ///< port for the input channel 1
    sc_fifo_in<I2TYP> iport2;       ///< port for the input channel 2
    sc_fifo_in<I3TYP> iport3;       ///< port for the input channel 3
    sc_fifo_in<I4TYP> iport4;       ///< port for the input channel 4
    sc_fifo_out<OTYP> oport;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb4(sc_module_name _name, unsigned int i1toks, unsigned int i2toks,
             unsigned int i3toks, unsigned int i4toks, unsigned int otoks)
         :sc_module(_name), ci1toks(i1toks), ci2toks(i2toks), 
         ci3toks(i3toks), ci4toks(i4toks), cotoks(otoks)
    {
        SC_THREAD(worker);
    }
private:
    unsigned int ci1toks, ci2toks, ci3toks, ci4toks, cotoks;
    SC_HAS_PROCESS(comb4);

    //! The main and only execution thread of the module
    void worker()
    {
        std::vector<I1TYP> in_vals1(ci1toks);
        std::vector<I2TYP> in_vals2(ci2toks);
        std::vector<I3TYP> in_vals3(ci3toks);
        std::vector<I4TYP> in_vals4(ci4toks);
        std::vector<OTYP> out_vals(cotoks);
        while (1)
        {
            for (unsigned int i=0;i<ci1toks;i++) in_vals1[i] = iport1.read();  // read from input
            for (unsigned int i=0;i<ci2toks;i++) in_vals2[i] = iport2.read();  // read from input
            for (unsigned int i=0;i<ci3toks;i++) in_vals3[i] = iport3.read();  // read from input
            for (unsigned int i=0;i<ci4toks;i++) in_vals4[i] = iport4.read();  // read from input
            out_vals = _func(in_vals1, in_vals2, in_vals3, in_vals4);// do the calculation
            WRITE_VEC_MULTIPORT(oport,out_vals,cotoks);    // write to the output
        }
    }

protected:
    //! The main caclulation function
    /*! It is abstract and the user should provide an implementation for
     * it in the derived class.
     */
    virtual std::vector<OTYP> (_func)(std::vector<I1TYP>, std::vector<I2TYP>, std::vector<I3TYP>, std::vector<I4TYP>) = 0;
};

//! Process constructor for a delay element (initial token)
/*! This class is used to insert an initial token on an SDF arc. Initial
 * tokens are used in SDF models to avoid initial deadlocks in the graphs
 * which include cycles. Given an initial value, it inserts this value at
 * the beginning of the output stream and passes the rest of the inputs 
 * to its output, untouched. The class is parameterized for its input/output
 * data-types.
 * 
 * It is also mandatory to include at least one delay element in the
 * feedback loops since combinational loops are forbidden in ForSyDe.
 */
template <class IOTYP>
class delay : public sc_module
{
public:
    sc_fifo_in<IOTYP>  iport;        ///< port for the input channel
    sc_fifo_out<IOTYP> oport;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial element, reads
     * data from its input port, and writes the results using the output
     * port.
     */
    delay(sc_module_name _name, IOTYP ival)  // module name, init val
         :sc_module(_name), init_val(ival)
    {
        SC_THREAD(worker);
    }
private:
    IOTYP init_val;
    SC_HAS_PROCESS(delay);

    //! The main and only execution thread of the module
    void worker()
    {
        IOTYP in_val;
        WRITE_MULTIPORT(oport,init_val);    // write the initial value
        while (1)
        {
            in_val = iport.read();  // read from input
            WRITE_MULTIPORT(oport,in_val);    // write to the output
        }
    }
};

//! Process constructor for a n-delay element
/*! This class is used to build an initialization process similar to dalay
 * but with an extra initial variable which sets the number of delay
 * elements (initial tokens). Given an initial value, it inserts the
 * initial value n times at the the beginning of output stream and
 * passes the rest of the inputs to its output untouched. The class is
 * parameterized for its input/output data-types.
 */
template <class IOTYP>
class delayn : public sc_module
{
public:
    sc_fifo_in<IOTYP>  iport;        ///< port for the input channel
    sc_fifo_out<IOTYP> oport;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial elements,
     * reads data from its input port, and writes the results using the
     * output port.
     */
    delayn(sc_module_name _name,   ///< module (process) name
             IOTYP ival,             ///< initial value
             unsigned int n          ///< number of delay elements
             )  // module name, init val
         :sc_module(_name), init_val(ival), ns(n)
    {
        SC_THREAD(worker);
    }
private:
    IOTYP init_val;
    unsigned int ns;
    SC_HAS_PROCESS(delayn);

    //! The main and only execution thread of the module
    void worker()
    {
        IOTYP in_val;
        for (unsigned int j=0;j<ns;j++)
            WRITE_MULTIPORT(oport,init_val);    // write the initial value
        while (1)
        {
            in_val = iport.read();  // read from input
            WRITE_MULTIPORT(oport,in_val);    // write to the output
        }
    }
};

//! Process constructor for a constant source process
/*! This class is used to build a souce process with constant output.
 * Its main purpose is to be used in test-benches.
 * 
 * This class can directly be instantiated to build a process.
 */
template <class OTYP>
class constant : public sc_module
{
public:
    sc_fifo_out<OTYP> oport;     ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    constant(sc_module_name _name, OTYP val)  // module name and const val
         :sc_module(_name), cval(val)
    {
        
        SC_THREAD(worker);
    }
private:
    OTYP cval;
    SC_HAS_PROCESS(constant);

    //! The main and only execution thread of the module
    void worker()
    {
        while (1)
        {
            WRITE_MULTIPORT(oport,cval);    // write to the output
        }
    }
};

//! Process constructor for a source process
/*! This class is used to build a souce process which only has an output.
 * Given an initial state and a function, the process repeatedly applies
 * the function to the current state to produce next state, which is
 * also the process output. It can be used in test-benches.
 * 
 * Note that this is an abstract class and can not be directly
 * instantiated. The designer should derive from this class and
 * implement the _func function which is run on each evaluation cycle to
 * produce an output.
 */
template <class OTYP>
class source : public sc_module
{
public:
    sc_fifo_out<OTYP> oport;     ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    source(sc_module_name _name,   ///< The module name
             OTYP ist                ///< Initial state
            )
         :sc_module(_name), init_st(ist)
    {
        SC_THREAD(worker);
    }
private:
    OTYP init_st;
    SC_HAS_PROCESS(source);

    //! The main and only execution thread of the module
    void worker()
    {
        OTYP st_val = init_st;
        WRITE_MULTIPORT(oport,st_val);    // write the initial state
        while (1)
        {
            st_val = _func(st_val);        // produce a new value
            WRITE_MULTIPORT(oport,st_val);    // write to the output
        }
    }

protected:
    //! The main caclulation function
    /*! It is abstract and the user should provide an implementation for
     * it in the derived class.
     */
    virtual OTYP (_func)(OTYP) = 0;
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
    sc_fifo_out<OTYP> oport;     ///< port for the output channel

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
            WRITE_MULTIPORT(oport,out_val);    // write to the output
        }
    }
};

//! Process constructor for a sink process
/*! This class is used to build a sink process which only has an input.
 * Its main purpose is to be used in test-benches.
 * 
 * Note that this is an abstract class and can not be directly
 * instantiated. The designer should derive from this class and
 * implement the _func function which is run in each evaluation cycle on
 * the received input.
 */
template <class ITYP>
class sink : public sc_module
{
public:
    sc_fifo_in<ITYP> iport;         ///< port for the input channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    sink(sc_module_name _name)  // module name
         :sc_module(_name)
    {
        SC_THREAD(worker);
    }
private:
    SC_HAS_PROCESS(sink);

    //! The main and only execution thread of the module
    void worker()
    {
        ITYP in_val;
        while (1)
        {
            in_val = iport.read();
            _func(in_val);       // run the function
        }
    }

protected:
    //! The main caclulation function
    /*! It is abstract and the user should provide an implementation for
     * it in the derived class.
     */
    virtual void (_func)(ITYP) = 0;
};

//! The zip process with variable number of inputs and one output
/*! This process "zips" the incoming signals into one signal of tuples.
 */
template <class... ITYPs>
class zipN : public sc_module
{
public:
    std::tuple <sc_fifo_in<ITYPs>...> iport;///< tuple of ports for the input channels
    sc_fifo_out<std::tuple<
                           std::vector<ITYPs>...
                           > > oport;   ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * zips them together and writes the results using the output port
     */
    zipN(sc_module_name _name, ///< Module name
         std::vector<unsigned int> itoks
        )
         :sc_module(_name), citoks(itoks)
    {
        if (itoks.size()!=sizeof...(ITYPs))
            SC_REPORT_ERROR(name(),"Wrong number of consumption rates provided");
        SC_THREAD(worker);
    }
private:
    std::vector<unsigned int> citoks;
    SC_HAS_PROCESS(zipN);

    //! The main and only execution thread of the module
    void worker()
    {
        std::tuple<std::vector<ITYPs>...> in_vals;
        while (1)
        {
            in_vals = sc_fifo_tuple_read<ITYPs...>(iport, citoks);
            WRITE_MULTIPORT(oport,in_vals);    // write to the output
        }
    }
    
    template<size_t N,class R, class T>
    struct fifo_read_helper
    {
        static void read(R& ret, T& t, const std::vector<unsigned int>& citoks)
        {
            fifo_read_helper<N-1,R,T>::read(ret,t,citoks);
            for (unsigned int i=0;i<citoks[N];i++)
                std::get<N>(ret).push_back(std::get<N>(t).read());
        }
    };

    template<class R, class T>
    struct fifo_read_helper<0,R,T>
    {
        static void read(R& ret, T& t, const std::vector<unsigned int>& citoks)
        {
            for (unsigned int i=0;i<citoks[0];i++)
                std::get<0>(ret).push_back(std::get<0>(t).read());
        }
    };

    template<class... T>
    std::tuple<std::vector<T>...> sc_fifo_tuple_read(std::tuple<sc_fifo_in<T>...>& ports,
                                                     const std::vector<unsigned int>& citoks)
    {
        std::tuple<std::vector<T>...> ret;
        fifo_read_helper<sizeof...(T)-1,
                         std::tuple<std::vector<T>...>,
                         std::tuple<sc_fifo_in<T>...>>::read(ret,ports,citoks);
        return ret;
    }

};

//! The unzip process with one input and variable number of outputs
/*! This process "unzips" the incoming signal into a tuple of signals.
 */
template <class... ITYPs>
class unzipN : public sc_module
{
public:
    sc_fifo_in<std::tuple<
                          std::vector<ITYPs>...
                          > > iport;///< port for the input channel
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
        std::tuple<std::vector<ITYPs>...> in_vals;
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
            for (unsigned int i=0;i<(std::get<N>(vals)).size();i++)
                std::get<N>(t).write(std::get<N>(vals)[i]);
        }
    };

    template<class R, class T>
    struct fifo_write_helper<0,R,T>
    {
        static void write(const R& vals, T& t)
        {
            for (unsigned int i=0;i<(std::get<0>(vals)).size();i++)
                std::get<0>(t).write(std::get<0>(vals)[i]);
        }
    };

    template<class... T>
    void sc_fifo_tuple_write(const std::tuple<std::vector<T>...>& vals,
                             std::tuple<sc_fifo_out<T>...>& ports)
    {
        fifo_write_helper<sizeof...(T)-1,
                          std::tuple<std::vector<T>...>,
                          std::tuple<sc_fifo_out<T>...>>::write(vals,ports);
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
    sc_fifo_out<IOTYP> oport;       ///< port for the output channel

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
            WRITE_MULTIPORT(oport,out_val);    // write to the output
        }
    }

};

}
}

#endif
