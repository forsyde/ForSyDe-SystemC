/**********************************************************************           
    * symoc.hpp -- The synchronous model of computation               *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing promitive element required for modeling      *
    *          synchronous systems in ForSyDe-SystemC                 *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SYMOC_HPP
#define SYMOC_HPP

/*! \file symoc.h
 * \brief Implements the synchronous Model of Computation
 * 
 *  This file includes the basic process constructors and other
 * facilities used for modeling in the synchronous model of computation.
 */

#include <tuple>

//! The namespace for ForSyDe
/*! General namespace that includes everything provided by the SFF.
 * Each MoC has its own sub-namespace.
 */
namespace ForSyDe
{
//! The namespace for synchronous MoC
/*! This namespace includes constructs used for building models in the
 * synchronous MoC.
 */
namespace SY
{

using namespace sc_core;

// Auxilliary Macro definitions
#define WRITE_MULTIPORT(PORT,VAL) \
    for (int i=0;i<PORT.size();i++) PORT[i]->write(VAL);

//! Absent-extended data types
/*! This template class extends a type TYP to its absent-extended version.
 * Values of this type could be either absent, or present with a specific
 * value.
 */
template <class TYP>
class AbstExt
{
public:
    //! The constructor with a present value
    AbstExt(const TYP& val) : present(true), value(val) {}
    
    //! The constructor with an absent value
    AbstExt() : present(false) {}
    
    //! Converts a value from an extended value, returning a default value if absent
    TYP fromAbstExt (const TYP& defval) const
    {
        if (present) return value; else return defval;
    }
    
    //! Unsafely converts a value from an extended value assuming it is present
    TYP unsafeFromAbstExt () const {return value;}
    
    //! Sets absent
    void setAbst() {present=false;}
    
    //! Sets the value
    void setVal(const TYP& val) {present=true;value=val;}
    
    //! Checks for the absence of a value
    bool isAbsent() const {return !present;}
    
    //! Checks for the presence of a value
    bool isPresent() const {return present;}
    
    bool operator== (const AbstExt& rs) const
    {
        if (isAbsent() || rs.isAbsent())
            return isAbsent() && rs.isAbsent();
        else
            return unsafeFromAbstExt() == rs.unsafeFromAbstExt();
    }
    
    friend std::ostream& operator<< (std::ostream& os, const AbstExt &abstExt)
    {
        if (abstExt.isPresent())
            os << abstExt.unsafeFromAbstExt();
        else
            os << "_";
        return os;
    }
private:
    bool present;
    TYP value;
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
class comb : public sc_module
{
public:
    sc_fifo_in<ITYP>  iport;        ///< port for the input channel
    sc_fifo_out<OTYP> oport;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    comb(sc_module_name _name)  // module name
         :sc_module(_name)
    {
        SC_THREAD(worker);
    }
private:
    SC_HAS_PROCESS(comb);

    //! The main and only execution thread of the module
    void worker()
    {
        ITYP in_val;
        OTYP out_val;
        while (1)
        {
            in_val = iport.read();  // read from input
            out_val = _func(in_val);// do the calculation
            WRITE_MULTIPORT(oport,out_val);    // write to the output
        }
    }

protected:
    //! The main caclulation function
    /*! It is abstract and the user should provide an implementation for
     * it in the derived class.
     */
    virtual OTYP (_func)(ITYP) = 0;
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

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * applies the user-imlpemented function to them and writes the
     * results using the output port
     */
    comb2(sc_module_name _name)  // module name
         :sc_module(_name)
    {
        SC_THREAD(worker);
    }
private:
    SC_HAS_PROCESS(comb2);

    //! The main and only execution thread of the module
    void worker()
    {
        I1TYP in_val1;
        I2TYP in_val2;
        OTYP out_val;
        while (1)
        {
            in_val1 = iport1.read();  // read from input
            in_val2 = iport2.read();  // read from input
            out_val = _func(in_val1, in_val2); // do the calculation
            WRITE_MULTIPORT(oport,out_val);     // write to the output
        }
    }

protected:
    //! The main caclulation function
    /*! It is abstract and the user should provide an implementation for
     * it in the derived class.
     */
    virtual OTYP (_func)(I1TYP, I2TYP) = 0;
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
    comb3(sc_module_name _name)  // module name
         :sc_module(_name)
    {
        SC_THREAD(worker);
    }
private:
    SC_HAS_PROCESS(comb3);

    //! The main and only execution thread of the module
    void worker()
    {
        I1TYP in_val1;
        I2TYP in_val2;
        I3TYP in_val3;
        OTYP out_val;
        while (1)
        {
            in_val1 = iport1.read();  // read from input
            in_val2 = iport2.read();  // read from input
            in_val3 = iport3.read();  // read from input
            out_val = _func(in_val1, in_val2, in_val3); // do the calculation
            WRITE_MULTIPORT(oport,out_val);     // write to the output
        }
    }

protected:
    //! The main caclulation function
    /*! It is abstract and the user should provide an implementation for
     * it in the derived class.
     */
    virtual OTYP (_func)(I1TYP, I2TYP, I3TYP) = 0;
};

//! Process constructor for a combinational process with three inputs and one output
/*! similar to comb with four inputs
 */
template <class I1TYP, class I2TYP, class I3TYP, class I4TYP, class OTYP>
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
    comb4(sc_module_name _name)  // module name
         :sc_module(_name)
    {
        SC_THREAD(worker);
    }
private:
    SC_HAS_PROCESS(comb4);

    //! The main and only execution thread of the module
    void worker()
    {
        I1TYP in_val1;
        I2TYP in_val2;
        I3TYP in_val3;
        I4TYP in_val4;
        OTYP out_val;
        while (1)
        {
            in_val1 = iport1.read();  // read from input
            in_val2 = iport2.read();  // read from input
            in_val3 = iport3.read();  // read from input
            in_val4 = iport4.read();  // read from input
            out_val = _func(in_val1, in_val2, in_val3, in_val4); // do the calculation
            WRITE_MULTIPORT(oport,out_val);     // write to the output
        }
    }

protected:
    //! The main caclulation function
    /*! It is abstract and the user should provide an implementation for
     * it in the derived class.
     */
    virtual OTYP (_func)(I1TYP, I2TYP, I3TYP, I4TYP) = 0;
};

//! Process constructor for a delay element
/*! This class is used to build the most basic sequential process which
 * is a delay element. Given an initial value, it inserts this value at
 * the beginning of output stream and passes the rest of the inputs to
 * its output untouched. The class is parameterized for its input/output
 * data-type.
 * 
 * It is mandatory to include at least one delay element in the feedback
 * loops since combinational loops are forbidden in ForSyDe.
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
        for (int i=0;i<oport.size();i++)
            oport[i]->write(init_val);    // write the initial value
        while (1)
        {
            in_val = iport.read();  // read from input
            WRITE_MULTIPORT(oport,in_val);    // write to the output
        }
    }
};

//! Process constructor for a n-delay element
/*! This class is used to build a sequential process similar to dalay
 * but with an extra initial variable which sets the number of delay
 * elements (initial tokens). Given an initial value, it inserts the
 * initial value n times at the the beginning of output stream and
 * passes the rest of the inputs to its output untouched. The class is
 * parameterized for its input/output data-type.
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
        for (int j=0;j<ns;j++)
            for (int i=0;i<oport.size();i++)
                oport[i]->write(init_val);    // write the initial value
        while (1)
        {
            in_val = iport.read();  // read from input
            WRITE_MULTIPORT(oport,in_val);    // write to the output
        }
    }
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
    sc_fifo_out<OTYP> oport;        ///< port for the output channel

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
            WRITE_MULTIPORT(oport,out_val);    // write to the output
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
    sc_fifo_out<OTYP> oport;        ///< port for the output channel

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
            WRITE_MULTIPORT(oport,out_val);    // write to the output
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
    sc_fifo_out<OTYP> oport;        ///< port for the output channel

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
            WRITE_MULTIPORT(oport,out_val);    // write to the output
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
 * The output signal is not any more of the type AbstExt. 
 */
template <class TYP>
class fill : public sc_module
{
public:
    sc_fifo_out<AbstExt<TYP> > iport;   ///< port for the input channel
    sc_fifo_out<TYP> oport;             ///< port for the output channel

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
        AbstExt<TYP> in_val;
        TYP out_val;
        while (1)
        {
            in_val = iport.read();  // read from the input
            out_val = in_val.fromAbstExt(defval); // fill the output signal
            WRITE_MULTIPORT(oport,out_val);      // write to the output
        }
    }
};

//! Process constructor for a hold process
/*! The process constructor hold creates a process that fills an absent-
 * extended signal with values by replacing absent values by the
 * preceding present value. Only in cases, where no preceding value
 * exists, the absent value is replaced by a default value.
 * 
 * The output signal is not any more of the type AbstExt. 
 */
template <class TYP>
class hold : public sc_module
{
public:
    sc_fifo_in<AbstExt<TYP> > iport;   ///< port for the input channel
    sc_fifo_out<TYP> oport;             ///< port for the output channel

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
        AbstExt<TYP> in_val;
        TYP out_val;
        while (1)
        {
            in_val = iport.read();  // read from the input
            // update the default value
            defval = in_val.isPresent() ? in_val.unsafeFromAbstExt()
                                        : defval;
            // fill the output signal
            out_val = defval;
            WRITE_MULTIPORT(oport,out_val);    // write to the output
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
             OTYP ist,             ///< Initial state
             unsigned long long take=0 ///< number of tokens produced (0 for infinite)
            )
         :sc_module(_name), init_st(ist), take(take)
    {
        SC_THREAD(worker);
    }
private:
    OTYP init_st;
    unsigned long long take;
    SC_HAS_PROCESS(source);

    //! The main and only execution thread of the module
    void worker()
    {
        OTYP st_val = init_st;
        WRITE_MULTIPORT(oport,st_val);    // write the initial state
        for (unsigned long long i=1;i!=take;i++)
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
    sc_fifo_out<std::tuple<ITYP1,ITYP2> > oport;///< port for the output channel

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
            WRITE_MULTIPORT(oport,out_val);    // write to the output
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
    sc_fifo_out<std::tuple<ITYP1,ITYP2,ITYP3> > oport;///< port for the output channel

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
            WRITE_MULTIPORT(oport,out_val);    // write to the output
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
    sc_fifo_out<std::tuple<ITYPs...> > oport;///< port for the output channel

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
            WRITE_MULTIPORT(oport,in_vals);    // write to the output
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
template <class OTYP1, class OTYP2>
class unzip : public sc_module
{
public:
    sc_fifo_in<std::tuple<OTYP1,OTYP2> > iport;///< port for the input channel
    sc_fifo_out<OTYP1> oport1;        ///< port for the output channel 1
    sc_fifo_out<OTYP2> oport2;        ///< port for the output channel 2

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input ports,
     * unzips them and writes the results using the output ports
     */
    unzip(sc_module_name _name)
         :sc_module(_name)
    {
        SC_THREAD(worker);
    }
private:
    SC_HAS_PROCESS(unzip);

    //! The main and only execution thread of the module
    void worker()
    {
        std::tuple<OTYP1,OTYP2> in_val;
        OTYP1 out_val1;
        OTYP2 out_val2;
        while (1)
        {
            in_val = iport.read();  // read from input
            std::tie(out_val1,out_val2) = in_val;
            WRITE_MULTIPORT(oport1,out_val1);  // write to the output 1
            WRITE_MULTIPORT(oport2,out_val2);  // write to the output 2
        }
    }

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
    sc_fifo_out<AbstExt<std::vector<TYP> > > oport;///< port for the output channel

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
        AbstExt<std::vector<TYP> > out_val;
        while (1)
        {
            for (int i=0;i<grppts;i++)
            {
                in_val[i] = iport.read();  // read from input and group
                if (i<grppts-1)
                {
                    out_val.setAbst(); // Absent Output
                    WRITE_MULTIPORT(oport,out_val);// write to the output
                }
            }
            out_val.setVal(in_val); // the grouped input
            WRITE_MULTIPORT(oport,out_val);        // write to the output
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
