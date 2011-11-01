/**********************************************************************           
    * ctmoc.hpp -- The continuous-time model of computation           *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *          Based on the HetMoC library by Jun Zhu (junz@kth.se)   *
    *                                                                 *
    * Purpose: Providing promitive element required for modeling      *
    *          continuous-time systems in ForSyDe-SystemC             *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef CTMOC_HPP
#define CTMOC_HPP

/*! \file ctmoc.h
 * \brief Implements the continuous-time Model of Computation
 * 
 *  This file includes the basic process constructors and other
 * facilities used for modeling in the continuous-time model of
 * computation.
 */

#include <fstream>
#include <string>
#include <functional>
#include <boost/numeric/ublas/matrix.hpp>

namespace ForSyDe
{
//! The namespace for synchronous MoC
/*! This namespace includes constructs used for building models in the
 * continuous-time MoC.
 */
namespace CT
{

using namespace sc_core;
using namespace boost::numeric::ublas;

// Auxilliary Macro definitions
#define WRITE_MULTIPORT(PORT,VAL) \
    for (int i=0;i<PORT.size();i++) PORT[i]->write(VAL);

#define CTTYPE double
typedef matrix<CTTYPE> MatrixDouble;

//! The sub-signal type used to construct a CT signal
/*! This class is used to build a sub-signal which is a function that is
 * valid on a range. A consecutive stream of tokens of type SubSignal
 * forms a CT signal.
 * 
 * The range is defined by a start time and and end time of type sc_time.
 * The supplied fuction can be a function pointer, a function object or
 * a c++0x lambda function.
 */
struct SubSignal
{
public:
    //! The constructor used for sub-signal definition
    /*! 
     */
    SubSignal(const sc_time& st,         ///< Beginning of the range
              const sc_time& et,         ///< End of the range
              const std::function<CTTYPE (const sc_time&)>& f) ///< The function over the range
        : startT(st), endT(et), _f(f) {}
    
    //! A dummy constructor used for sub-signal definition without initialization
    /*! 
     */
    SubSignal() {}
    
    //! The copy constructor
    /*! 
     */
    SubSignal(const SubSignal& ss       ///< Object to be copied
              )
        : startT(ss.getStartT()), endT(ss.getEndT()), _f(ss.getF()) {}
    
    //! The assignment operator
    /*! 
     */
    SubSignal& operator=(const SubSignal& ss  ///< Object to be assigned
                        ){
        startT = ss.getStartT();
        endT = ss.getEndT();
        _f = ss.getF();
        return *this;
    }
    
    //! The overloaded () operator makes the sub-signal a function object
    /*! It allows to sample the signal with a convinient syntax.
     * Additionally, it checks the sampling time validity with respect
     * to the range.
     */
    CTTYPE operator() (const sc_time& valAt) const {
        if ((valAt>=startT) && (valAt<=endT))
            return _f(valAt);
        else
            SC_REPORT_ERROR("Using ForSyDe::CT","Access out of sub-signal range");
    }
    
    //! A helper function used to get the beginning of the range
    /*! 
     */
    sc_time getStartT() const {return startT;}
    
    //! A helper function used to get the end of the range
    /*! 
     */
    sc_time getEndT() const {return endT;}
    
    //! A helper function used to get the functions in range
    /*! 
     */
    std::function<CTTYPE (const sc_time&)> getF() const {return _f;}

    //! A helper function used to set the start and end of the range
    /*! 
     */
    void setRange(sc_time st, sc_time et) {startT = st; endT = et;}
    
    //! A helper function used to set the function in the range
    /*! 
     */
    void setF(const std::function<CTTYPE (const sc_time&)>& f) {_f=f;}
    
    friend std::ostream& operator<< (std::ostream& os, SubSignal &subSig)
    {
        os << "Sub-signal starting at: " << subSig.getStartT() 
           << " and ending at:" << subSig.getEndT() << std::endl;
        return os;
    }
private:
    sc_time startT;
    sc_time endT;
    std::function<CTTYPE (const sc_time&)> _f;
};

//! Process constructor for a continuous-time process with one input and one output
/*! This class is used to build continuous-time processes with one input
 * and one output.
 * 
 * Note that this is an abstract class and can not be directly
 * instantiated. The designer should derive from this class and
 * implement the _func function which is applied on the input to obtain
 * the output.
 */
class comb : public sc_module
{
public:
    sc_fifo_in<SubSignal>  iport;        ///< port for the input channel
    sc_fifo_out<SubSignal> oport;        ///< port for the output channel

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
        SubSignal inSubSig;
        SubSignal outSubSig;
        while (1)
        {
            inSubSig = iport.read();  // read from input
            outSubSig = SubSignal(inSubSig.getStartT(), inSubSig.getEndT(),
                [=](const sc_time& t) -> CTTYPE {return _func(inSubSig(t));});
            //out_val = in_val;       // do the calculation
            WRITE_MULTIPORT(oport,outSubSig);    // write to the output
        }
    }

protected:
    //! The main caclulation function
    /*! It is abstract and the user should provide an implementation for
     * it in the derived class.
     */
    virtual CTTYPE (_func)(CTTYPE) = 0;
};

//! Process constructor for a combinational process with two inputs and one output
/*! similar to comb with two inputs
 */
class comb2 : public sc_module
{
public:
    sc_fifo_in<SubSignal> iport1;      ///< port for the input channel 1
    sc_fifo_in<SubSignal> iport2;      ///< port for the input channel 2
    sc_fifo_out<SubSignal> oport;      ///< port for the output channel

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
        SubSignal in1SubSig;
        SubSignal in2SubSig;
        SubSignal outSubSig;
        enum EvalState {Aligned, SS1Earlier, SS2Earlier} evalSt = Aligned;
        sc_time outEndT(0,SC_SEC);
        while (1)
        {
            if (evalSt==Aligned){
                in1SubSig = iport1.read();  // read from input 1
                in2SubSig = iport2.read();  // read from input 2
            }else if (evalSt==SS1Earlier)
                in1SubSig = iport1.read();  // read from input 1
            else in2SubSig = iport2.read();  // read from input 2
            
            sc_time in1EndT = in1SubSig.getEndT();
            sc_time in2EndT = in2SubSig.getEndT();
            if(in1EndT > in2EndT) {
                outEndT = in2EndT;
                evalSt = SS2Earlier;
            } else if(in1EndT < in2EndT) {
                outEndT = in1EndT;
                evalSt = SS1Earlier;
            } else {
                outEndT = in1EndT;
                evalSt = Aligned;
            }
            
            outSubSig = SubSignal(std::max(in1SubSig.getStartT(),in2SubSig.getStartT()),
                outEndT, [=](const sc_time& t) -> CTTYPE {
                    return _func(in1SubSig(t), in2SubSig(t));
                });
            WRITE_MULTIPORT(oport,outSubSig);     // write to the output
        }
    }

protected:
    //! The main caclulation function
    /*! It is abstract and the user should provide an implementation for
     * it in the derived class.
     */
    virtual CTTYPE (_func)(CTTYPE, CTTYPE) = 0;
};

//! Process constructor for a delay element
/*! This class is used to build a process which delays the input CT signal.
 * It operates by adding the specified delay value to the start and end
 * times for ranges of every sub-signal element.
 * 
 * The resulting process does not buffer anything from the signal.
 */
template <class IOTYP>
class delay : public sc_module
{
public:
    sc_fifo_in<SubSignal>  iport;        ///< port for the input channel
    sc_fifo_out<SubSignal> oport;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which inserts the initial element, reads
     * data from its input port, and writes the results using the output
     * port.
     */
    delay(sc_module_name _name,         ///< Process name
          sc_time delT                  ///< delay time
          )
         :sc_module(_name), delayTime(delT)
    {
        SC_THREAD(worker);
    }
private:
    sc_time delayTime;
    SC_HAS_PROCESS(delay);

    //! The main and only execution thread of the module
    void worker()
    {
        SubSignal inSubSig;
        while (1)
        {
            inSubSig = iport.read();  // read from input
            inSubSig.setRange(inSubSig.getStartT()+delayTime,
                              inSubSig.getEndT()+delayTime);
            WRITE_MULTIPORT(oport,inSubSig);    // write to the output
        }
    }
};

//! Process constructor for a shift element
/*! This class is used to build a process which shifts the shape of the
 * input signal by a given value to the right.
 */
template <class IOTYP>
class shift : public sc_module
{
public:
    sc_fifo_in<SubSignal>  iport;        ///< port for the input channel
    sc_fifo_out<SubSignal> oport;        ///< port for the output channel

    //! The constructor requires the module name
    /*! 
     */
    shift(sc_module_name _name,         ///< Process name
          sc_time delT                  ///< delay time
          )
         :sc_module(_name), delayTime(delT)
    {
        SC_THREAD(worker);
    }
private:
    sc_time delayTime;
    SC_HAS_PROCESS(shift);

    //! The main and only execution thread of the module
    void worker()
    {
        SubSignal inSubSig;//, zeroSubSig;
        //zeroSubSig = SubSignal(sc_time(0,SC_SEC),delayTime,
        //                   [&](const sc_time& t) -> CTTYPE {return 0;});
        //for (int i=0;i<oport.size();i++)
        //    oport[i]->write(zeroSubSig);    // write to the output
        while (1)
        {
            inSubSig = iport.read();  // read from input
            inSubSig.setRange(inSubSig.getStartT()+delayTime,
                              inSubSig.getEndT()+delayTime);
            inSubSig.setF([&delayTime,&inSubSig](sc_time t)->CTTYPE{
                            return inSubSig.getF()(t-delayTime);
                          });
            WRITE_MULTIPORT(oport,inSubSig);    // write to the output
        }
    }
};

//! Process constructor for an integrator element
/*! This class is used to build a process which integrates the input
 * signal.
 */
class integrate : public sc_module
{
public:
    sc_fifo_in<SubSignal>  iport;        ///< port for the input channel
    sc_fifo_out<SubSignal> oport;        ///< port for the output channel

    //! The constructor requires the module name and a fixed sampling time
    /*! 
     */
    integrate(sc_module_name _name,         ///< Process name
              CTTYPE cvalue,                ///< Constant value 'c'
              sc_time sampleT               ///< sampling time
             )
         :sc_module(_name), cval(cvalue), sampleTime(sampleT)
    {
        SC_THREAD(ct2de);
        SC_THREAD(worker);
        SC_THREAD(de2ct);
    }
private:
    CTTYPE cval;
    sc_time sampleTime;
    sc_fifo<CTTYPE> sampledIns, sampledOuts;
    SC_HAS_PROCESS(integrate);
    
    void ct2de()
    {
        sc_time curct2deT(0, SC_SEC);
        SubSignal inSubSig;
        CTTYPE sampledIn;
        inSubSig = iport.read();
        while (1)
        {
            if (curct2deT >= inSubSig.getEndT())
                inSubSig = iport.read();
            sampledIn = inSubSig(curct2deT);
            sampledIns.write(sampledIn);
            curct2deT += sampleTime;
        }
    }
    void worker()
    {
        sc_time curT(0, SC_SEC), prevT(0, SC_SEC);
        CTTYPE curIn, prevIn, res = cval;
        prevIn = sampledIns.read();
        while (1)
        {
            curT += sampleTime;
            curIn = sampledIns.read();
            res += (curIn+prevIn)/2*((curT-prevT).to_seconds());
            sampledOuts.write(res);
            prevT = curT;
            prevIn = curIn;
        }
    }
    void de2ct()
    {
        sc_time curde2ctT(0, SC_SEC), prevde2ctT(0, SC_SEC);
        SubSignal outSubSig;
        CTTYPE cursampledOut, prevsampledOut;
        prevsampledOut = sampledOuts.read();
        while (1)
        {
            curde2ctT += sampleTime;
            cursampledOut = sampledOuts.read();
            outSubSig = SubSignal(prevde2ctT, curde2ctT,
                        [=](sc_time t)
                        {
                            return prevsampledOut + 
                            (t-prevde2ctT) / (curde2ctT-prevde2ctT) *
                            (cursampledOut-prevsampledOut);
                        });
            oport.write(outSubSig);
            prevde2ctT = curde2ctT;
            prevsampledOut = cursampledOut;
            
        }
    }
};

//! Process constructor for aimplementing a linear filter
/*! This class is used to build a process which implements a linear filter based
 * on the numerator and denominator constants.
 */
class filter : public sc_module
{
public:
    sc_fifo_in<SubSignal>  iport;        ///< port for the input channel
    sc_fifo_out<SubSignal> oport;        ///< port for the output channel

    //! The constructor requires the module name and a fixed sampling time
    /*! 
     */
    filter(sc_module_name _name,            ///< Process name
           std::vector<CTTYPE> numerators,  ///< Numerator constants
           std::vector<CTTYPE> denominators,///< Denominator constants
           sc_time sampleT                  ///< sampling time
          )
         :sc_module(_name), nums(numerators), dens(denominators), sampleTime(sampleT)
    {
        int /*nn = nums.size(),*/ nd = dens.size();
        a = MatrixDouble(nd-1,nd-1);
        b = MatrixDouble(nd-1,1);
        c = MatrixDouble(1,nd-1);
        d = MatrixDouble(1,1);
    
        tf2ss(nums,dens,a,b,c,d);
        
        // State number
        int numState = a.size1();
        assert(a.size1() == a.size2());
        x = zero_matrix<double>(numState,1);;
        u = MatrixDouble(1,1), u_1 = MatrixDouble(1,1);
        y1 = MatrixDouble(1,1);
        k1 = MatrixDouble(numState,1);
        k2 = MatrixDouble(numState,1);
        k3 = MatrixDouble(numState,1);
        k4 = MatrixDouble(numState,1);
        
        SC_THREAD(ct2de);
        SC_THREAD(worker);
        SC_THREAD(de2ct);
    }
private:
    std::vector<CTTYPE> nums, dens;
    MatrixDouble a, b, c, d;
    // states
    MatrixDouble x, x1;
    // current and previous input/time.
    MatrixDouble u, u_1; 
    sc_time t, t_1, h;
    // output
    MatrixDouble y1;
    // Some helper matrices used in RK solver
    MatrixDouble k1,k2,k3,k4;
    
    sc_time sampleTime;
    sc_fifo<CTTYPE> sampledIns, sampledOuts;
    SC_HAS_PROCESS(filter);
    
    // To obtain state space matrices from transfer function.
    // We assume there are non-zero leading coefficients in num and denom.
    int tf2ss(std::vector<double> & num_, std::vector<double> & den_, MatrixDouble & a, 
          MatrixDouble & b, MatrixDouble & c, MatrixDouble & d)
    {
        std::vector<double> num, den;
        // sizes checking
        int nn = num_.size(), nd = den_.size();
        if(nn >= nd)
        {
            std::cerr << "ERROR: " << "degree(num) = " << nn
            << " >= degree(denom) = " << nd << std::endl;
            abort();
        }
        double dCoef1 = den_.at(0);
        if(nd==1)
        {
            //~ a = NULL, b = NULL, c = NULL;
            d = MatrixDouble(1,1);
            d(0,0) = num_.at(0)/dCoef1;
        }
        else
        {
            if ((nd - nn) > 0)
            {
                // Pad num so that degree(num) == degree(denom)
                for(int i=0; i<nd; i++)
                {
                    if(i<(nd-nn))
                        num.push_back(0.0);
                    else
                        num.push_back(num_.at(i-nd+nn));
                }
            }
        
            // Normalizing w.r.t the leading coefficient of denominator
            for(unsigned int i=0; i<num.size(); i++)
                num.at(i) /= dCoef1;
            for(unsigned int i=0; i<den_.size(); i++)
                den_.at(i) /= dCoef1;
            for(unsigned int i=0; i<(den_.size()-1); i++)
                den.push_back(den_.at(i+1));
        
            // Form A (nd-1)*(nd-1)
            a = zero_matrix<double> (a.size1(), a.size2());
            if(nd > 2)
            {
                // The eyes (up-right corner) are set to '1'
                for(int i=0; i<(nd-2); i++)
                    for(int j=0; j<(nd-1); j++)
                        if((j-i)==1) a(i,j) = 1.0;
                // The lower row(s)
                for(int j=0; j<(nd-1); j++)
                    a(nd-2,j) = 0-den.at(nd-2-j);
            }
            else
                a(0,0) = 0-den.at(0);
            //
            // Form B (nd-1)*1
            b = zero_matrix<double> (b.size1(), b.size2());
            b(nd-2,0) = 1.0;
            //
            // Form C 1*(nd-1)
            for(int j=0; j< nd-1; j++)
                c(0,nd-2-j) = num.at(j+1) - num.at(0)*den.at(j);
            //
            // Form D 1*1
            d(0,0) = num.at(0);
        }
        
        return 0;
    }
    
    void rkSolver(MatrixDouble a, MatrixDouble b, MatrixDouble c,
                  MatrixDouble d, MatrixDouble u_k, MatrixDouble u_k_1,
                  MatrixDouble x, double h, MatrixDouble &x_, MatrixDouble &y)
    {
        // Some helper matrices used in RK solver
        MatrixDouble k1,k2,k3,k4;
        k1 = prod(a,x) + prod(b,u_k_1);
        k2 = prod(a,(x+k1*(h/2.0))) + prod(b,(u_k_1 + u_k)) * 0.5;
        k3 = prod(a,(x+k2*(h/2.0))) + prod(b,(u_k_1 + u_k)) * 0.5;
        k4 = prod(a,(x+k3*h)) + prod(b,u_k);
        x_ = x + (k1 + 2.0*k2 + 2.0*k3 + k4) * (h/6.0);
        y = prod(c,x_) + prod(d,u_k);
    }
    
    void ct2de()
    {
        sc_time curct2deT(0, SC_SEC);
        SubSignal inSubSig;
        CTTYPE sampledIn;
        inSubSig = iport.read();
        while (1)
        {
            if (curct2deT >= inSubSig.getEndT())
                inSubSig = iport.read();
            sampledIn = inSubSig(curct2deT);
            sampledIns.write(sampledIn);
            curct2deT += sampleTime;
        }
    }
    void worker()
    {
        sc_time curT(0, SC_SEC), prevT(0, SC_SEC), t_1(prevT);
        CTTYPE curIn, prevIn;
        prevIn = sampledIns.read(); u_1(0,0) = prevIn;
        while (1)
        {
            curT += sampleTime; t = curT;
            curIn = sampledIns.read(); u(0,0) = curIn;
            h = t - t_1;
            // RK solver (a state vector 'x')
            rkSolver(a, b, c, d, u, u_1, x, h.to_seconds(), x1, y1);
            x = x1;
            sampledOuts.write(y1(0,0));
            prevT = curT; t_1 = t;
            prevIn = curIn; u_1(0,0) = u(0,0);
        }
    }
    void de2ct()
    {
        sc_time curde2ctT(0, SC_SEC), prevde2ctT(0, SC_SEC);
        SubSignal outSubSig;
        CTTYPE cursampledOut, prevsampledOut;
        prevsampledOut = sampledOuts.read();
        while (1)
        {
            curde2ctT += sampleTime;
            cursampledOut = sampledOuts.read();
            outSubSig = SubSignal(prevde2ctT, curde2ctT,
                        [=](sc_time t)
                        {
                            return prevsampledOut + 
                            (t-prevde2ctT) / (curde2ctT-prevde2ctT) *
                            (cursampledOut-prevsampledOut);
                        });
            oport.write(outSubSig);
            prevde2ctT = curde2ctT;
            prevsampledOut = cursampledOut;
            
        }
    }
};

//! Process constructor for a constant source process
/*! This class is used to build a souce process with constant output.
 * Its main purpose is to be used in test-benches.
 * 
 * This class can directly be instantiated to build a process.
 */
//template <class OTYP>
class constant : public sc_module
{
public:
    sc_fifo_out<SubSignal> oport;     ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    constant(sc_module_name _name, CTTYPE val)  // module name and const val
         :sc_module(_name), cval(val)
    {
        
        SC_THREAD(worker);
    }
private:
    CTTYPE cval;
    SC_HAS_PROCESS(constant);

    //! The main and only execution thread of the module
    void worker()
    {
        while (1)
        {
            auto mylambda = [&](sc_time t){return cval;};
            sc_time st(0,SC_SEC);
            sc_time et(1,SC_SEC);
            SubSignal ss = SubSignal(st,et, mylambda);
            WRITE_MULTIPORT(oport,ss);    // write to the output
        }
    }
};

//! Process constructor for a source process
/*! This class is used to build a souce process which only has an output.
 * Given a function, the process generates a source process which
 * generates a continuous signal made out of a single SubSignal. The
 * start and end times of the signals should aso be mentioned.
 * 
 */
//template <class OTYP>
class source : public sc_module
{
public:
    sc_fifo_out<SubSignal> oport;     ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * and writes the result using the output port
     */
    source(sc_module_name _name,                    ///< The module name
             const sc_time& st,                     ///< Start time
             const sc_time& et,                     ///< End time
             const std::function<CTTYPE (sc_time)>& f ///< Supplied function
            )
         :sc_module(_name), subSigStart(st), subSigEnd(et), subSigF(f)
    {        
        SC_THREAD(worker);
    }
    source() {}
private:
    sc_time subSigStart, subSigEnd;
    std::function<CTTYPE (sc_time)> subSigF;
    SC_HAS_PROCESS(source);

    //! The main and only execution thread of the module
    void worker()
    {
        //~ auto mylambda = [&](sc_time t){return subSigF(t);};
        SubSignal tmpSubSig = SubSignal(subSigStart, subSigEnd, subSigF);
        //while (1)
        {
            WRITE_MULTIPORT(oport,tmpSubSig);    // write to the output
        }
    }
};

//! Process constructor for a vsource process
/*! This class is used to build a souce process which only has an output.
 * Given a vector of SubSignal s, the process generates a source process
 * which generates a continuous signal made by concatenation of these
 * SubSignal s.
 */
//template <class OTYP>
class vsource : public sc_module
{
public:
    sc_fifo_out<SubSignal> oport;     ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which outputs the suplied SubSignal s.
     */
    vsource(sc_module_name _name,                    ///< The module name
             const std::vector<SubSignal>& _subSigs
            )
         :sc_module(_name), subSigs(_subSigs)
    {        
        SC_THREAD(worker);
    }
    vsource(){}
private:
    std::vector<SubSignal> subSigs;
    SC_HAS_PROCESS(vsource);

    //! The main and only execution thread of the module
    void worker()
    {
        for (std::vector<SubSignal>::iterator it=subSigs.begin();
            it != subSigs.end(); it++)
                WRITE_MULTIPORT(oport,*it); // write to the output
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

//! Process constructor for a print process
/*! This class is used to build a sink process which only has an input.
 * Its main purpose is to be used in test-benches.
 * 
 * The resulting process prints the sampled data continuously.
 */
//template <class ITYP>
class printSig : public sc_module
{
public:
    sc_fifo_in<SubSignal> iport;         ///< port for the input channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    printSig(sc_module_name _name, const sc_time& sampT)  // module name
         :sc_module(_name), sampleT(sampT)
    {
        SC_THREAD(worker);
    }
private:
    SC_HAS_PROCESS(printSig);

    //! The main and only execution thread of the module
    void worker()
    {
        SubSignal in_val;
        in_val = iport.read();
        sc_time curTime(sc_time_stamp());
        while (1)
        {
            curTime = sc_time_stamp();
            while (curTime > in_val.getEndT()) in_val = iport.read();
            std::cout << name() << ": value at " << curTime << " is "
                      << in_val(curTime) << std::endl;// run the function
            wait(sampleT);
        }
    }

protected:
    sc_time sampleT;
};

//! Process constructor for a trace process
/*! This class is used to build a sink process which only has an input.
 * Its main purpose is to be used in test-benches.
 * 
 * The resulting process prints the sampled data as a trace in an output
 * ".dat" file which can be plotted using gaw or gwave.
 */
//template <class ITYP>
class traceSig : public sc_module
{
public:
    sc_fifo_in<SubSignal> iport;         ///< port for the input channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    traceSig(sc_module_name _name,     ///< Process name
             const sc_time& sampT      ///< Sampling time
             ):sc_module(_name), sampleT(sampT)
    {
        SC_THREAD(worker);
    }

private:
    SC_HAS_PROCESS(traceSig);

    //! The main and only execution thread of the module
    void worker()
    {
        std::ofstream outFile(name()+std::string(".dat"));
        if (!outFile.is_open())
            SC_REPORT_ERROR(name(),"file could not be opened");
        outFile << "#time " << name() << std::endl;
        SubSignal in_val;
        in_val = iport.read();
        sc_time curTime(sc_time_stamp());
        while (1)
        {
            curTime = sc_time_stamp();
            while (curTime > in_val.getEndT()) in_val = iport.read();
            outFile << curTime.to_seconds() << " " << in_val(curTime)
                    << std::endl;
            wait(sampleT);
        }
        outFile.close();
    }

protected:
    sc_time sampleT;
};

//! Process constructor for a multi-input trace process
/*! This class is used to build a sink process which has a multi-port input.
 * Its main purpose is to be used in test-benches.
 * 
 * The resulting process prints the sampled data as a trace in an output
 * ".dat" file which can be plotted using gaw or gwave.
 * 
 * Note that the timing is done with reference to the first signal.
 */
//template <class ITYP>
class traceSigs : public sc_module
{
public:
    sc_fifo_in<SubSignal> iport;         ///< multi-port for the input channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which runs the user-imlpemented function
     * in each cycle.
     */
    traceSigs(sc_module_name _name,     ///< Process name
              const sc_time& sampT      ///< Sampling time
              ):sc_module(_name), sampleT(sampT)
    {
        SC_THREAD(worker);
    }

private:
    SC_HAS_PROCESS(traceSigs);

    //! The main and only execution thread of the module
    void worker()
    {
        std::ofstream outFile(name()+std::string(".dat"));
        if (!outFile.is_open()) 
            SC_REPORT_ERROR(name(),"file could not be opened");
        // write the header
        outFile << "#time(s)";
        for (int i=0;i<iport.size();i++)
            outFile << " " << name() << "(" << i << ")";
        outFile << std::endl;
        // start reading from the ports
        SubSignal in_val[iport.size()];
        for (int i=0;i<iport.size();i++)
            in_val[i] = iport[i]->read();
        sc_time curTime(sc_time_stamp());
        while (1)
        {
            curTime = sc_time_stamp();
            for (int i=0;i<iport.size();i++)
                while (curTime > in_val[i].getEndT())
                    in_val[i] = iport[i]->read();
            // print one line
            outFile << curTime.to_seconds();
            for (int i=0;i<iport.size();i++)
                outFile << " " << in_val[i](curTime);
            outFile << std::endl;
            wait(sampleT);
        }
        outFile.close();
    }

protected:
    sc_time sampleT;
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
class fanout : public sc_module
{
public:
    sc_fifo_in<SubSignal> iport;        ///< port for the input channel
    sc_fifo_out<SubSignal> oport;       ///< port for the output channel

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
        SubSignal in_val;
        SubSignal out_val;
        while (1)
        {
            in_val = iport.read();  // read from input
            out_val = in_val;       // same output
            WRITE_MULTIPORT(oport,out_val);    // write to the output
        }
    }

};


}
}

#endif
