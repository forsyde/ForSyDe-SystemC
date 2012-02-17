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
 
#include <functional>
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

//! This type is used in the process base class to store structural information
struct PortInfo
{
    sc_object* port;
    unsigned toks;
    std::vector<sc_object*> boundChans;
    std::string portType;
};

//! A helper class used to provide interface for accessing size of the signal elements
class SDF2SDF_Size
{
public:
    virtual unsigned tokenSize() = 0;
};

//! The SDF signal used to inter-connect SDF processes
template <class T>
class SDF2SDF: public sc_fifo<T>, public SDF2SDF_Size
{
public:
    SDF2SDF() : sc_fifo<T>() {}
    SDF2SDF(sc_module_name name, unsigned size) : sc_fifo<T>(name, size) {}
    virtual unsigned tokenSize()
    {
        return sizeof(T);
    }
};

//! Base class for all process constructors in the SDF MoC
/*! It captures the requirements of a typical ForSyDe process as a
 * sc_module.
 */
class process: public sc_module
{
public:
    //! Token rates associated to the input ports
    std::vector<unsigned> itoks;
    //! Token rates associated to the output ports
    std::vector<unsigned> otoks;
    //! Pointers to the input ports and their bound channels
    std::vector<PortInfo> boundInChans;
    //! Pointers to the output ports and their bound channels
    std::vector<PortInfo> boundOutChans;
    
    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * processes them and writes the results using the output port.
     */
    process(sc_module_name _name): sc_module(_name)
    {
        SC_THREAD(worker);
    }
    
    //! The ForSyDe process type rprsented by the current module
    virtual std::string ForSyDe_kind() = 0;
    
protected:
    //! The main and only execution thread of the module
    virtual void worker() = 0;
    
    //! This method is called during end_of_elaboration to gather binded channels information
    /*! This function should save the pointers to all of the channels
     * objects bound to the input and output channels in boundInChans
     * and boundOutChans respectively
     */
    virtual void bindInfo() = 0;

private:
    SC_HAS_PROCESS(process);
    
    void end_of_elaboration()
    {
        bindInfo();
    }
};


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
class comb : public process
{
public:
    sc_fifo_in<ITYP>  iport;        ///< port for the input channel
    sc_fifo_out<OTYP> oport;        ///< port for the output channel
    
    typedef std::function<void(std::vector<OTYP>&,
                                const std::vector<ITYP>&
                                )> functype;

    //! The constructor requires the module name and the number of tokens to be produced and consumed
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb(sc_module_name _name,
         functype _func,
         unsigned inToks,
         unsigned outToks
        ):process(_name), _func(_func)
    {
        itoks.push_back(inToks);
        otoks.push_back(outToks);
    }
    
    std::string ForSyDe_kind() {return "SDF::comb";};
    
private:

    //! The main and only execution thread of the module
    void worker()
    {
        std::vector<ITYP> in_vals(itoks[0]);
        std::vector<OTYP> out_vals(otoks[0]);
        while (1)
        {
            // read from input
            for (unsigned i=0;i<itoks[0];i++) in_vals[i] = iport.read();
            _func(out_vals, in_vals);// do the calculation
            WRITE_VEC_MULTIPORT(oport,out_vals,otoks[0]);  // write to the output
        }
    }
    
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport;
        boundInChans[0].toks = itoks[0];
        boundInChans[0].portType = typeid(ITYP).name();
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport;
        boundOutChans[0].toks = otoks[0];
        boundOutChans[0].portType = typeid(OTYP).name();
        for (int i=0;i<iport.size();i++)
            boundInChans[0].boundChans.push_back(dynamic_cast<sc_object*>(iport[i]));
        for (int i=0;i<oport.size();i++)
            boundOutChans[0].boundChans.push_back(dynamic_cast<sc_object*>(oport[i]));
    }
    
    functype _func;

//~ protected:
    //~ //! The main caclulation function
    //~ /*! It is abstract and the user should provide an implementation for
     //~ * it in the derived class.
     //~ */
    //~ virtual std::vector<OTYP> (_func)(std::vector<ITYP>) = 0;
};

//! Process constructor for a combinational process with two inputs and one output
/*! similar to comb with two inputs
 */
template <class I1TYP, class I2TYP, class OTYP>
class comb2 : public process
{
public:
    sc_fifo_in<I1TYP> iport1;       ///< port for the input channel 1
    sc_fifo_in<I2TYP> iport2;       ///< port for the input channel 2
    sc_fifo_out<OTYP> oport;        ///< port for the output channel
    
    typedef std::function<void(std::vector<OTYP>&,
                                const std::vector<I1TYP>&,
                                const std::vector<I2TYP>&
                                )> functype;

    //! The constructor requires the module name and the number of tokens to be produced and consumed
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb2(sc_module_name _name,
          functype _func,
          unsigned in1Toks,
          unsigned in2Toks,
          unsigned outToks
         ):process(_name), _func(_func)
    {
        itoks.push_back(in1Toks);
        itoks.push_back(in2Toks);
        otoks.push_back(outToks);
    }
    
    std::string ForSyDe_kind() {return "SDF::comb2";};
    
private:

    //! The main and only execution thread of the module
    void worker()
    {
        std::vector<I1TYP> in_vals1(itoks[0]);
        std::vector<I2TYP> in_vals2(itoks[1]);
        std::vector<OTYP> out_vals(otoks[0]);
        while (1)
        {
            // read from inputs
            for (unsigned i=0;i<itoks[0];i++) in_vals1[i] = iport1.read();
            for (unsigned i=0;i<itoks[1];i++) in_vals2[i] = iport2.read();
            _func(out_vals, in_vals1, in_vals2);// do the calculation
            WRITE_VEC_MULTIPORT(oport,out_vals,otoks[0]);    // write to the output
        }
    }
    
    void bindInfo()
    {
        boundInChans.resize(2);     // two input ports
        boundInChans[0].port = &iport1;
        boundInChans[0].toks = itoks[0];
        boundInChans[0].portType = typeid(I1TYP).name();
        boundInChans[1].port = &iport2;
        boundInChans[1].toks = itoks[1];
        boundInChans[1].portType = typeid(I2TYP).name();
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport;
        boundOutChans[0].toks = otoks[0];
        boundOutChans[0].portType = typeid(OTYP).name();
        for (int i=0;i<iport1.size();i++)
            boundInChans[0].boundChans.push_back(dynamic_cast<sc_object*>(iport1[i]));
        for (int i=0;i<iport2.size();i++)
            boundInChans[1].boundChans.push_back(dynamic_cast<sc_object*>(iport2[i]));
        for (int i=0;i<oport.size();i++)
            boundOutChans[0].boundChans.push_back(dynamic_cast<sc_object*>(oport[i]));
    }
    
    functype _func;


};

//! Process constructor for a combinational process with three inputs and one output
/*! similar to comb with three inputs
 */
template <class I1TYP, class I2TYP, class I3TYP, class OTYP>
class comb3 : public process
{
public:
    sc_fifo_in<I1TYP> iport1;       ///< port for the input channel 1
    sc_fifo_in<I2TYP> iport2;       ///< port for the input channel 2
    sc_fifo_in<I3TYP> iport3;       ///< port for the input channel 3
    sc_fifo_out<OTYP> oport;        ///< port for the output channel
    
    typedef std::function<void(std::vector<OTYP>&,
                                std::vector<I1TYP>&,
                                std::vector<I2TYP>&,
                                std::vector<I3TYP>&
                                )> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb3(sc_module_name _name,
          functype _func,
          unsigned in1Toks,
          unsigned in2Toks,
          unsigned in3Toks,
          unsigned int outToks
         ):process(_name), _func(_func)
    {
        itoks.push_back(in1Toks);
        itoks.push_back(in2Toks);
        itoks.push_back(in3Toks);
        otoks.push_back(outToks);
    }
    
    std::string ForSyDe_kind() {return "SDF::comb3";};
    
private:

    //! The main and only execution thread of the module
    void worker()
    {
        std::vector<I1TYP> in_vals1(itoks[0]);
        std::vector<I2TYP> in_vals2(itoks[1]);
        std::vector<I3TYP> in_vals3(itoks[2]);
        std::vector<OTYP> out_vals(otoks[0]);
        while (1)
        {
            // read from inputs
            for (unsigned i=0;i<itoks[0];i++) in_vals1[i] = iport1.read();
            for (unsigned i=0;i<itoks[1];i++) in_vals2[i] = iport2.read();
            for (unsigned i=0;i<itoks[2];i++) in_vals3[i] = iport3.read();
            _func(out_vals, in_vals1, in_vals2, in_vals3);// do the calculation
            WRITE_VEC_MULTIPORT(oport,out_vals,otoks[0]);    // write to the output
        }
    }
    
    void bindInfo()
    {
        boundInChans.resize(3);     // three input ports
        boundInChans[0].port = &iport1;
        boundInChans[0].toks = itoks[0];
        boundInChans[0].portType = typeid(I1TYP).name();
        boundInChans[1].port = &iport2;
        boundInChans[1].toks = itoks[1];
        boundInChans[1].portType = typeid(I2TYP).name();
        boundInChans[2].port = &iport3;
        boundInChans[2].toks = itoks[2];
        boundInChans[2].portType = typeid(I3TYP).name();
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport;
        boundOutChans[0].toks = otoks[0];
        boundOutChans[0].portType = typeid(OTYP).name();
        for (int i=0;i<iport1.size();i++)
            boundInChans[0].boundChans.push_back(dynamic_cast<sc_object*>(iport1[i]));
        for (int i=0;i<iport2.size();i++)
            boundInChans[1].boundChans.push_back(dynamic_cast<sc_object*>(iport2[i]));
        for (int i=0;i<iport3.size();i++)
            boundInChans[2].boundChans.push_back(dynamic_cast<sc_object*>(iport3[i]));
        for (int i=0;i<oport.size();i++)
            boundOutChans[0].boundChans.push_back(dynamic_cast<sc_object*>(oport[i]));
    }
    
    functype _func;
};

//! Process constructor for a combinational process with four inputs and one output
/*! similar to comb with three inputs
 */
template <class I1TYP, class I2TYP, class I3TYP, class I4TYP,class OTYP>
class comb4 : public process
{
public:
    sc_fifo_in<I1TYP> iport1;       ///< port for the input channel 1
    sc_fifo_in<I2TYP> iport2;       ///< port for the input channel 2
    sc_fifo_in<I3TYP> iport3;       ///< port for the input channel 3
    sc_fifo_in<I4TYP> iport4;       ///< port for the input channel 4
    sc_fifo_out<OTYP> oport;        ///< port for the output channel
    
    typedef std::function<void(std::vector<OTYP>&,
                                std::vector<I1TYP>&,
                                std::vector<I2TYP>&,
                                std::vector<I3TYP>&,
                                std::vector<I4TYP>&
                                )> functype;

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb4(sc_module_name _name,
          functype _func,
          unsigned in1Toks,
          unsigned in2Toks,
          unsigned in3Toks,
          unsigned in4Toks,
          unsigned outToks
         ):process(_name), _func(_func)
    {
        itoks.push_back(in1Toks);
        itoks.push_back(in2Toks);
        itoks.push_back(in3Toks);
        itoks.push_back(in4Toks);
        otoks.push_back(outToks);
    }
    
    std::string ForSyDe_kind() {return "SDF::comb4";};
    
private:

    //! The main and only execution thread of the module
    void worker()
    {
        std::vector<I1TYP> in_vals1(itoks[0]);
        std::vector<I2TYP> in_vals2(itoks[1]);
        std::vector<I3TYP> in_vals3(itoks[2]);
        std::vector<I4TYP> in_vals4(itoks[3]);
        std::vector<OTYP> out_vals(otoks[0]);
        while (1)
        {
            // read from inputs
            for (unsigned int i=0;i<itoks[0];i++) in_vals1[i] = iport1.read();
            for (unsigned int i=0;i<itoks[1];i++) in_vals2[i] = iport2.read();
            for (unsigned int i=0;i<itoks[2];i++) in_vals3[i] = iport3.read();
            for (unsigned int i=0;i<itoks[3];i++) in_vals4[i] = iport4.read();
            _func(out_vals, in_vals1, in_vals2, in_vals3, in_vals4);// do the calculation
            WRITE_VEC_MULTIPORT(oport,out_vals,otoks[0]);    // write to the output
        }
    }
    
    void bindInfo()
    {
        boundInChans.resize(4);     // four input ports
        boundInChans[0].port = &iport1;
        boundInChans[0].toks = itoks[0];
        boundInChans[0].portType = typeid(I1TYP).name();
        boundInChans[1].port = &iport2;
        boundInChans[1].toks = itoks[1];
        boundInChans[1].portType = typeid(I2TYP).name();
        boundInChans[2].port = &iport3;
        boundInChans[2].toks = itoks[2];
        boundInChans[2].portType = typeid(I3TYP).name();
        boundInChans[3].port = &iport4;
        boundInChans[3].toks = itoks[3];
        boundInChans[3].portType = typeid(I4TYP).name();
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport;
        boundOutChans[0].toks = otoks[0];
        boundOutChans[0].portType = typeid(OTYP).name();
        for (int i=0;i<iport1.size();i++)
            boundInChans[0].boundChans.push_back(dynamic_cast<sc_object*>(iport1[i]));
        for (int i=0;i<iport2.size();i++)
            boundInChans[1].boundChans.push_back(dynamic_cast<sc_object*>(iport2[i]));
        for (int i=0;i<iport3.size();i++)
            boundInChans[2].boundChans.push_back(dynamic_cast<sc_object*>(iport3[i]));
        for (int i=0;i<iport4.size();i++)
            boundInChans[3].boundChans.push_back(dynamic_cast<sc_object*>(iport4[i]));
        for (int i=0;i<oport.size();i++)
            boundOutChans[0].boundChans.push_back(dynamic_cast<sc_object*>(oport[i]));
    }
    
    functype _func;
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
class delay : public process
{
public:
    sc_fifo_in<IOTYP>  iport;        ///< port for the input channel
    sc_fifo_out<IOTYP> oport;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial element, reads
     * data from its input port, and writes the results using the output
     * port.
     */
    delay(sc_module_name _name,
          IOTYP ival
         ):process(_name), init_val(ival)
    {
        
    }
    
    std::string ForSyDe_kind() {return "SDF::delay";};
    
    IOTYP init_val;
    
private:

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
    
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input ports
        boundInChans[0].port = &iport;
        boundInChans[0].toks = 1;
        boundInChans[0].portType = typeid(IOTYP).name();
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport;
        boundOutChans[0].toks = 1;
        boundOutChans[0].portType = typeid(IOTYP).name();
        for (int i=0;i<iport.size();i++)
            boundInChans[0].boundChans.push_back(dynamic_cast<sc_object*>(iport[i]));
        for (int i=0;i<oport.size();i++)
            boundOutChans[0].boundChans.push_back(dynamic_cast<sc_object*>(oport[i]));
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
class delayn : public process
{
public:
    sc_fifo_in<IOTYP>  iport;        ///< port for the input channel
    sc_fifo_out<IOTYP> oport;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial elements,
     * reads data from its input port, and writes the results using the
     * output port.
     */
    delayn(sc_module_name _name,    ///< module (process) name
             IOTYP ival,            ///< initial value
             unsigned n             ///< number of delay elements
          ):process(_name), init_val(ival), ns(n)
    {
        
    }
    
    std::string ForSyDe_kind() {return "SDF::delayn";};
    
    IOTYP init_val;
    
    unsigned int ns;
    
private:

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
    
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input ports
        boundInChans[0].port = &iport;
        boundInChans[0].toks = 1;
        boundInChans[0].portType = typeid(IOTYP).name();
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport;
        boundOutChans[0].toks = 1;
        boundOutChans[0].portType = typeid(IOTYP).name();
        for (int i=0;i<iport.size();i++)
            boundInChans[0].boundChans.push_back(dynamic_cast<sc_object*>(iport[i]));
        for (int i=0;i<oport.size();i++)
            boundOutChans[0].boundChans.push_back(dynamic_cast<sc_object*>(oport[i]));
    }
};

//! Process constructor for a constant source process
/*! This class is used to build a souce process with constant output.
 * Its main purpose is to be used in test-benches.
 * 
 * This class can directly be instantiated to build a process.
 */
template <class OTYP>
class constant : public process
{
public:
    sc_fifo_out<OTYP> oport;     ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    constant(sc_module_name _name,
             OTYP val
            ):process(_name), cval(val)
    {
        
    }
    
    std::string ForSyDe_kind() {return "SDF::constant";};
    
    OTYP cval;
    
private:

    //! The main and only execution thread of the module
    void worker()
    {
        while (1)
        {
            WRITE_MULTIPORT(oport,cval);    // write to the output
        }
    }
    
    void bindInfo()
    {
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport;
        boundOutChans[0].toks = 1;
        boundOutChans[0].portType = typeid(OTYP).name();
        for (int i=0;i<oport.size();i++)
            boundOutChans[0].boundChans.push_back(dynamic_cast<sc_object*>(oport[i]));
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
class source : public process
{
public:
    sc_fifo_out<OTYP> oport;     ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    source(sc_module_name _name,   ///< The module name
             OTYP ist                ///< Initial state
          ):process(_name), init_st(ist)
    {
        
    }
    
    std::string ForSyDe_kind() {return "SDF::source";};
    
    OTYP init_st;
    
private:

    //! The main and only execution thread of the module
    void worker()
    {
        OTYP curst_val = init_st, nxst_val;
        WRITE_MULTIPORT(oport,curst_val);      // write the initial state
        while (1)
        {
            _func(nxst_val, curst_val);        // produce a new value
            curst_val = nxst_val;
            WRITE_MULTIPORT(oport,curst_val);  // write to the output
        }
    }
    
    void bindInfo()
    {
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport;
        boundOutChans[0].toks = 1;
        boundOutChans[0].portType = typeid(OTYP).name();
        for (int i=0;i<oport.size();i++)
            boundOutChans[0].boundChans.push_back(dynamic_cast<sc_object*>(oport[i]));
    }

protected:
    //! The main caclulation function
    /*! It is abstract and the user should provide an implementation for
     * it in the derived class.
     */
    virtual void (_func)(OTYP&, const OTYP&) = 0;
};

//! Process constructor for a source process with vector input
/*! This class is used to build a souce process which only has an output.
 * Given the test bench vector, the process iterates over the emenets
 * of the vector and outputs one value on each evaluation cycle.
 */
template <class OTYP>
class vsource : public process
{
public:
    sc_fifo_out<OTYP> oport;     ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which writes the result using the output
     * port.
     */
    vsource(sc_module_name _name,           ///< The module name
            const std::vector<OTYP>& invec  ///< Initial vector
           ):process(_name), in_vec(invec)
    {
        
    }
    
    std::string ForSyDe_kind() {return "SDF::vsource";};
    
    std::vector<OTYP> in_vec;
    
private:

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
    
    void bindInfo()
    {
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport;
        boundOutChans[0].toks = 1;
        boundOutChans[0].portType = typeid(OTYP).name();
        for (int i=0;i<oport.size();i++)
            boundOutChans[0].boundChans.push_back(dynamic_cast<sc_object*>(oport[i]));
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
class sink : public process
{
public:
    sc_fifo_in<ITYP> iport;         ///< port for the input channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    sink(sc_module_name _name
        ):process(_name)
    {
        
    }
    
    std::string ForSyDe_kind() {return "SDF::sink";};
private:

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
    
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input ports
        boundInChans[0].port = &iport;
        boundInChans[0].toks = 1;
        boundInChans[0].portType = typeid(ITYP).name();
        for (int i=0;i<iport.size();i++)
            boundInChans[0].boundChans.push_back(dynamic_cast<sc_object*>(iport[i]));
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
class zipN : public process
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
         std::vector<unsigned> inToks
        ):process(_name)
    {
        if (inToks.size()!=sizeof...(ITYPs))
            SC_REPORT_ERROR(name(),"Wrong number of consumption rates provided");
        itoks = inToks;
        otoks.push_back(1);
    }
    
    std::string ForSyDe_kind() {return "SDF::zipN";};
    
private:

    //! The main and only execution thread of the module
    void worker()
    {
        std::tuple<std::vector<ITYPs>...> in_vals;
        while (1)
        {
            in_vals = sc_fifo_tuple_read<ITYPs...>(iport, itoks);
            WRITE_MULTIPORT(oport,in_vals);    // write to the output
        }
    }
    
    void bindInfo()
    {
        boundInChans.resize(sizeof...(ITYPs));
        boundOutChans.resize(1);
        boundOutChans[0].port = &oport;
        boundOutChans[0].toks = otoks[0];
        boundOutChans[0].portType = typeid(std::tuple<std::vector<ITYPs>...>).name();
        populateIPorts<ITYPs...>(boundInChans, itoks, iport);
        for (int i=0;i<oport.size();i++)
            boundOutChans[0].boundChans.push_back(dynamic_cast<sc_object*>(oport[i]));
    }
    
    // Begin of some TMP magic //
    template<size_t N,class R, class T>
    struct fifo_read_helper
    {
        static void read(R& ret, T& t, const std::vector<unsigned int>& itoks)
        {
            fifo_read_helper<N-1,R,T>::read(ret,t,itoks);
            for (unsigned int i=0;i<itoks[N];i++)
                std::get<N>(ret).push_back(std::get<N>(t).read());
        }
    };

    template<class R, class T>
    struct fifo_read_helper<0,R,T>
    {
        static void read(R& ret, T& t, const std::vector<unsigned int>& itoks)
        {
            for (unsigned int i=0;i<itoks[0];i++)
                std::get<0>(ret).push_back(std::get<0>(t).read());
        }
    };

    template<class... T>
    std::tuple<std::vector<T>...> sc_fifo_tuple_read(std::tuple<sc_fifo_in<T>...>& ports,
                                                     const std::vector<unsigned int>& itoks)
    {
        std::tuple<std::vector<T>...> ret;
        fifo_read_helper<sizeof...(T)-1,
                         std::tuple<std::vector<T>...>,
                         std::tuple<sc_fifo_in<T>...>>::read(ret,ports,itoks);
        return ret;
    }
    // End of TMP magic //
    // Begin of some TMP magic //
    template<size_t N, class T>
    struct populateIPorts_helper
    {
        static void populate(std::vector<PortInfo>& boundInChans,
                             const std::vector<unsigned>& itoks, T& ports)
        {
            populateIPorts_helper<N-1,T>::populate(boundInChans,itoks,ports);
            boundInChans[N].port = &std::get<N>(ports);
            boundInChans[N].toks = itoks[N];
            //~ boundInChans[N].portType = typeid(*std::get<N>(ports).data_type).name();
            for (int i=0;i<std::get<N>(ports).size();i++)
                boundInChans[N].boundChans.push_back(
                    dynamic_cast<sc_object*>(std::get<N>(ports)[i])
                );
        }
    };

    template<class T>
    struct populateIPorts_helper<0,T>
    {
        static void populate(std::vector<PortInfo>& boundInChans,
                             const std::vector<unsigned>& itoks, T& ports)
        {
            boundInChans[0].port = &std::get<0>(ports);
            boundInChans[0].toks = itoks[0];
            //~ boundInChans[0].portType = typeid(*std::get<0>(ports).data_type).name();
            for (int i=0;i<std::get<0>(ports).size();i++)
                boundInChans[0].boundChans.push_back(
                    dynamic_cast<sc_object*>(std::get<0>(ports)[i])
                );
        }
    };

    template<class... T>
    void populateIPorts(std::vector<PortInfo>& boundInChans,
                        const std::vector<unsigned>& itoks,
                        std::tuple<sc_fifo_in<T>...>& ports)
    {
        populateIPorts_helper<sizeof...(T)-1,
            std::tuple<sc_fifo_in<T>...>>::populate(boundInChans, itoks, ports);
    }
    // End of TMP magic //
};

//! The unzip process with one input and two outputs
/*! This process "unzips" a signal of tuples into two separate signals
 */
template <class OTYP1, class OTYP2>
class unzip : public process
{
public:
    sc_fifo_in<std::tuple<
                          std::vector<OTYP1>,std::vector<OTYP2>
                          >> iport;         ///< port for the input channel
    sc_fifo_out<OTYP1> oport1; ///< port for the output channel 1
    sc_fifo_out<OTYP2> oport2; ///< port for the output channel 2

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * unzips them and writes the results using the output ports
     */
    unzip(sc_module_name _name,
          unsigned out1Toks,
          unsigned out2Toks
         ):process(_name)
    {
        itoks.push_back(1);
        otoks.push_back(out1Toks);
        otoks.push_back(out2Toks);
    }
    
    std::string ForSyDe_kind() {return "SDF::unzip";};
    
private:

    //! The main and only execution thread of the module
    void worker()
    {
        std::tuple<std::vector<OTYP1>,std::vector<OTYP2>> in_val;
        std::vector<OTYP1> out_val1;
        std::vector<OTYP2> out_val2;
        while (1)
        {
            in_val = iport.read();  // read from input
            std::tie(out_val1,out_val2) = in_val;
            WRITE_VEC_MULTIPORT(oport1,out_val1,otoks[0]);// write to the output
            WRITE_VEC_MULTIPORT(oport2,out_val2,otoks[1]);// write to the output
        }
    }
    
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport;
        boundInChans[0].toks = itoks[0];
        boundInChans[0].portType = typeid(std::tuple<std::vector<OTYP1>,
                                                     std::vector<OTYP2>>).name();
        boundOutChans.resize(2);    // two output ports
        boundOutChans[0].port = &oport1;
        boundOutChans[0].toks = otoks[0];
        boundOutChans[0].portType = typeid(OTYP1).name();
        boundOutChans[1].port = &oport2;
        boundOutChans[1].toks = otoks[1];
        boundOutChans[1].portType = typeid(OTYP2).name();
        for (int i=0;i<iport.size();i++)
            boundInChans[0].boundChans.push_back(dynamic_cast<sc_object*>(iport[i]));
        for (int i=0;i<oport1.size();i++)
            boundOutChans[0].boundChans.push_back(dynamic_cast<sc_object*>(oport1[i]));
        for (int i=0;i<oport2.size();i++)
            boundOutChans[1].boundChans.push_back(dynamic_cast<sc_object*>(oport2[i]));
    }

};

//! The unzip process with one input and variable number of outputs
/*! This process "unzips" the incoming signal into a tuple of signals.
 */
template <class... ITYPs>
class unzipN : public process
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
    unzipN(sc_module_name _name,
           std::vector<unsigned> outToks
          ):process(_name)
    {
        if (outToks.size()!=sizeof...(ITYPs))
            SC_REPORT_ERROR(name(),"Wrong number of production rates provided");
        itoks.push_back(1);
        otoks = outToks;
    }
    
    std::string ForSyDe_kind() {return "SDF::unzipN";};
    
private:

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
    
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport;
        boundInChans[0].toks = itoks[0];
        boundOutChans.resize(sizeof...(ITYPs));    // output ports
        for (int i=0;i<iport.size();i++)
            boundInChans[0].boundChans.push_back(dynamic_cast<sc_object*>(iport[i]));
        populateOPorts<ITYPs...>(boundOutChans, otoks, oport);
    }
    
    // Begin of some TMP magic //
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
    // End of TMP magic //
    // Begin of some TMP magic //
    template<size_t N, class T>
    struct populateOPorts_helper
    {
        static void populate(std::vector<PortInfo>& boundOutChans,
                             const std::vector<unsigned>& otoks, T& ports)
        {
            populateOPorts_helper<N-1,T>::populate(boundOutChans,otoks,ports);
            boundOutChans[N].port = &std::get<N>(ports);
            boundOutChans[N].toks = otoks[N];
            for (int i=0;i<std::get<N>(ports).size();i++)
                boundOutChans[N].boundChans.push_back(
                    dynamic_cast<sc_object*>(std::get<N>(ports)[i])
                );
        }
    };

    template<class T>
    struct populateOPorts_helper<0,T>
    {
        static void populate(std::vector<PortInfo>& boundOutChans,
                             const std::vector<unsigned>& otoks, T& ports)
        {
            boundOutChans[0].port = &std::get<0>(ports);
            boundOutChans[0].toks = otoks[0];
            for (int i=0;i<std::get<0>(ports).size();i++)
                boundOutChans[0].boundChans.push_back(
                    dynamic_cast<sc_object*>(std::get<0>(ports)[i])
                );
        }
    };

    template<class... T>
    void populateOPorts(std::vector<PortInfo>& boundOutChans,
                        const std::vector<unsigned>& otoks,
                        std::tuple<sc_fifo_out<T>...>& ports)
    {
        populateOPorts_helper<sizeof...(T)-1,
            std::tuple<sc_fifo_out<T>...>>::populate(boundOutChans, otoks, ports);
    }
    // End of TMP magic //
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
class fanout : public process
{
public:
    sc_fifo_in<IOTYP> iport;        ///< port for the input channel
    sc_fifo_out<IOTYP> oport;       ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies and writes the results using the output port
     */
    fanout(sc_module_name _name
          ):process(_name)
    {
        
    }
    
    std::string ForSyDe_kind() {return "SDF::fanout";};
    
private:

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
    
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input ports
        boundInChans[0].port = &iport;
        boundInChans[0].toks = 1;
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport;
        boundOutChans[0].toks = 1;
        for (int i=0;i<iport.size();i++)
            boundInChans[0].boundChans.push_back(dynamic_cast<sc_object*>(iport[i]));
        for (int i=0;i<oport.size();i++)
            boundOutChans[0].boundChans.push_back(dynamic_cast<sc_object*>(oport[i]));
    }

};

}
}

#endif
