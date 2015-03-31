/**********************************************************************
    * ct_lib.hpp -- a library of useful processes in the CT MoC       *
    *                                                                 *
    * Authors:  Hosein Attarzadeh (shan2@kth.se)                      *
    *                                                                 *
    * Purpose: Enriching the CT library.                              *
    *                                                                 *
    * Usage:                                                          *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef CTLIB_H
#define CTLIB_H

#include "ct_moc.hpp"
#include "dde_moc.hpp"
#include "mis.hpp"

/*! \file ct_lib.hpp
 * \brief Implements extra facilities on top of the CT MoC
 *
 *  This file includes the basic process constructors and helper
 * functions for creating advanced CT processes.
 */

namespace ForSyDe
{

namespace CT
{

//! Helper function to construct a coasine source
/*! This class is used to cretae a continuous-time signal source which
 * produces a sinosoid.
 */
class sine : public source
{
public:
    sine(sc_module_name name_,      ///< The Process name
           const sc_time& endT,     ///< The end time of the generated signal
           const sc_time& period,   ///< The signal period (1/f)
           const CTTYPE& ampl      ///< The signal amplitude
           ) : source(name_, [=](CTTYPE& out, const sc_time& t)
                              {
                                  out = ampl*sin(2*M_PI*t/period);
                              }, endT) {}

    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::sine";}
};

//! Helper function to construct a sine source process
/*! This function is used to construct a sine source and connect its
 * output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf>
inline sine* make_sine(std::string pName,
    const sc_time& endT,  ///< The end time of the generated signal
    const sc_time& period,///< The signal period (1/f)
    const CTTYPE& ampl,  ///< The signal amplitude
    OIf& outS
    )
{
    auto p = new sine(pName.c_str(), endT, period, ampl);

    (*p).oport1(outS);

    return p;
}

//! Helper function to construct a coasine source
/*! This class is used to cretae a continuous-time signal source which
 * produces a cosine wave.
 */
class cosine : public source
{
public:
    cosine(sc_module_name name_,      ///< The Process name
           const sc_time& endT,       ///< The end time of the generated signal
           const sc_time& period,     ///< The signal period (1/f)
           const CTTYPE& ampl         ///< The signal amplitude
           ) : source(name_, [=](CTTYPE& out, const sc_time& t)
                              {
                                  out = ampl*cos(2*M_PI*t/period);
                              }, endT) {}

    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::cosine";}
};

//! Helper function to construct a cosine source process
/*! This function is used to construct a cosine source and connect its
 * output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf>
inline cosine* make_cosine(std::string pName,
    const sc_time& endT,  ///< The end time of the generated signal
    const sc_time& period,///< The signal period (1/f)
    const CTTYPE& ampl,  ///< The signal amplitude
    OIf& outS
    )
{
    auto p = new cosine(pName.c_str(), endT, period, ampl);

    (*p).oport1(outS);

    return p;
}

//! Helper function to construct a square source
/*! This class is used to cretae a continuous-time signal source which
 * produces a square wave.
 */
class square : public source
{
public:
    square(sc_module_name name_,      ///< The Process name
           const sc_time& endT,       ///< The end time of the generated signal
           const sc_time& period,     ///< The signal period (1/f)
           const CTTYPE& highS,       ///< The signal high swing
           const CTTYPE& lowS,        ///< The signal low swing
           const double& dutyCycle = 0.5///< The duty cycle (0 to 1)
           ) : source(name_, [=](CTTYPE& out, const sc_time& t)
                              {
                                  double tmp = (t/period);
                                  out = tmp-(long)tmp < dutyCycle ? highS : lowS;
                              }, endT) {}

    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::square";}
};

//! Helper function to construct a square source process
/*! This function is used to construct a square source and connect its
 * output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf>
inline square* make_square(std::string pName,
    const sc_time& endT,  ///< The end time of the generated signal
    const sc_time& period,///< The signal period (1/f)
    const CTTYPE& highS,       ///< The signal high swing
    const CTTYPE& lowS,        ///< The signal low swing
    const double& dutyCycle,   ///< The duty cycle (0 to 1)
    OIf& outS
    )
{
    auto p = new square(pName.c_str(), endT, period, highS, lowS, dutyCycle);

    (*p).oport1(outS);

    return p;
}

//! Process constructor for a continuous-time process which scales the input
/*! This class is used to build continuous-time processes with one input
 * and one output. By passing a constant value to the constructor, the
 * process scales the inputs using it.
 */
class scale : public comb
{
public:
    scale(sc_module_name name_,             ///< The Process name
           const CTTYPE& scaling_factor    ///< The scaling factor
           ) : comb(name_, [=](CTTYPE& out1, const CTTYPE& inp1)
                             {
                                out1 = scaling_factor * inp1;
                             }) {}

    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::scale";}
};

//! Helper function to construct a scale process
/*! This function is used to construct a scale source and connect its
 * input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class IIf>
inline scale* make_scale(std::string pName,
    const CTTYPE& scaling_factor,       ///< The scaling factor
    OIf& outS,
    IIf& inpS
    )
{
    auto p = new scale(pName.c_str(), scaling_factor);

    (*p).iport1(inpS);
    (*p).oport1(outS);

    return p;
}

//! Process constructor for a continuous-time process which adds its inputs
/*! This class is used to build continuous-time processes with two inputs
 * and one output. The process adds its two inputs and produces the output.
 */
class add : public comb2
{
public:
    add(sc_module_name name_        ///< The Process name
        ) : comb2(name_, [=](CTTYPE& out1, const CTTYPE& inp1, const CTTYPE& inp2)
                             {
                                out1 = inp1 + inp2;
                             }) {}

    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::add";}
};

//! Helper function to construct an add process
/*! This function is used to construct an adder and connect its
 * input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class IIf1, class IIf2>
inline add* make_add(std::string pName,
    OIf& outS,
    IIf1& inp1S,
    IIf2& inp2S
    )
{
    auto p = new add(pName.c_str());

    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).oport1(outS);

    return p;
}

//! Process constructor for a continuous-time process which subtracts its inputs
/*! This class is used to build continuous-time processes with two inputs
 * and one output. The process subtracts the second input from the first
 * and produces the output.
 */
class sub : public comb2
{
public:
    sub(sc_module_name name_        ///< The Process name
        ) : comb2(name_, [=](CTTYPE& out1, const CTTYPE& inp1, const CTTYPE& inp2)
                             {
                                out1 = inp1 - inp2;
                             }) {}

    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::sub";}
};

//! Helper function to construct a sub process
/*! This function is used to construct a subtractor and connect its
 * input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class IIf1, class IIf2>
inline sub* make_sub(std::string pName,
    OIf& outS,
    IIf1& inp1S,
    IIf2& inp2S
    )
{
    auto p = new sub(pName.c_str());

    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).oport1(outS);

    return p;
}

//! Process constructor for a continuous-time process which multiplies its inputs
/*! This class is used to build continuous-time processes with two inputs
 * and one output. The process multiplies its two inputs and produces the output.
 */
class mul : public comb2
{
public:
    mul(sc_module_name name_        ///< The Process name
        ) : comb2(name_, [=](CTTYPE& out1, const CTTYPE& inp1, const CTTYPE& inp2)
                             {
                                out1 = inp1 * inp2;
                             }) {}

    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::mul";}
};

//! Helper function to construct a mul process
/*! This function is used to construct a multiplier and connect its
 * input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class IIf1, class IIf2>
inline mul* make_mul(std::string pName,
    OIf& outS,
    IIf1& inp1S,
    IIf2& inp2S
    )
{
    auto p = new mul(pName.c_str());

    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).oport1(outS);

    return p;
}

//! Process constructor for a Gaussian randome wave generator
/*! This class is used to create a continuous-time signal source which
 * produces a Random signal based on the Gaussian distribution
 */
SC_MODULE(gaussian)
{
    CT_out oport1;          ///< port for the output channel

    SY::gaussian gaussian1;
    SY2CT sy2ct1;

    SY::SY2SY<CTTYPE> out_sig;

    //! The constructor requires the module name and the generator parameters
    /*!
     */
    gaussian(sc_module_name _name,          ///< Process name
              const double& gaussVar,       ///< The variance
              const double& gaussMean,      ///< The mean value
              sc_time sample_period          ///< sampling period
          ) : sc_module(_name), gaussian1("gaussian1", gaussVar, gaussMean),
              sy2ct1("sy2ct1", sample_period, HOLD)
    {
        gaussian1.oport1(out_sig);

        sy2ct1.iport1(out_sig);
        sy2ct1.oport1(oport1);
    }
};

//! Helper function to construct a gaussian process
/*! This function is used to construct a gaussian signal generator and
 * connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf>
inline gaussian* make_gaussian(std::string pName,
    const double& gaussVar,       ///< The variance
    const double& gaussMean,      ///< The mean value
    const sc_time sample_period,         ///< sampling period
    OIf& outS
    )
{
    auto p = new gaussian(pName.c_str(), gaussVar, gaussMean, sample_period);

    (*p).oport1(outS);

    return p;
}

//! Process constructor for implementing a linear filter
/*! This class is used to build a process which implements a linear
 * in the CT MoC filter based on the numerator and denominator constants.
 * It internally uses a DDE filter together with CT2DDE and DDE2CT MoC
 * interfaces.
 */
SC_MODULE(filter)
{
    CT_in iport1;           ///< port for the input channel
    CT_out oport1;          ///< port for the output channel;

    CT2DDE<CTTYPE> ct2de1;
    DDE::filter<CTTYPE> filter1;
    DDE2CT<CTTYPE> de2ct1;

    DDE::DDE2DDE<CTTYPE> inp_sig, out_sig;
    DDE::DDE2DDE<unsigned int> smp_sig;

    //! The constructor requires the module name and the filter parameters
    /*!
     */
    filter(sc_module_name _name,            ///< Process name
           std::vector<CTTYPE> numerators,  ///< Numerator constants
           std::vector<CTTYPE> denominators,///< Denominator constants
           sc_time sample_period,           ///< sampling period
           sc_time min_step=sc_time(0.05,SC_NS),///< Minimum time step
           double tol_error=1e-5            ///< Tolerated error
          ) : sc_module(_name), ct2de1("ct2de1"),
              filter1("filter1", numerators, denominators, sample_period, min_step, tol_error),
              de2ct1("de2ct1", HOLD)
    {
        ct2de1.iport1(iport1);
        ct2de1.iport2(smp_sig);
        ct2de1.oport1(inp_sig);

        filter1.iport1(inp_sig);
        filter1.oport1(out_sig);
        filter1.oport2(smp_sig);

        de2ct1.iport1(out_sig);
        de2ct1.oport1(oport1);
    }
};

//! Helper function to construct a linear process
/*! This function is used to construct a CT filter and connect its
 * input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class I1If>
inline filter* make_filter(std::string pName,
    const std::vector<CTTYPE> numerators,  ///< Numerator constants
    const std::vector<CTTYPE> denominators,///< Denominator constants
    const sc_time sample_period,            ///< sampling period
    OIf& outS,
    I1If& inp1S
    )
{
    auto p = new filter(pName.c_str(), numerators, denominators, sample_period);

    (*p).iport1(inp1S);
    (*p).oport1(outS);

    return p;
}

//! Process constructor for implementing a linear filter with fixed step
/*! This class is used to build a process which implements a linear
 * in the CT MoC filter with fixed step based on the numerator and
 * denominator constants.
 * It internally uses a DDE filter together with CT2DDEf and DDE2CT
 * MoC interfaces.
 */
SC_MODULE(filterf)
{
    CT_in iport1;           ///< port for the input channel
    CT_out oport1;          ///< port for the output channel;

    CT2DDEf<CTTYPE> ct2de1;
    DDE::filterf<CTTYPE> filter1;
    DDE2CT<CTTYPE> de2ct1;

    DDE::DDE2DDE<CTTYPE> inp_sig, out_sig;

    //! The constructor requires the module name and the filter parameters
    /*!
     */
    filterf(sc_module_name _name,            ///< Process name
           std::vector<CTTYPE> numerators,  ///< Numerator constants
           std::vector<CTTYPE> denominators,///< Denominator constants
           sc_time sample_period             ///< sampling period
          ) : sc_module(_name), ct2de1("ct2de1", sample_period),
              filter1("filter1", numerators, denominators),
              de2ct1("de2ct1", HOLD)
    {
        ct2de1.iport1(iport1);
        ct2de1.oport1(inp_sig);

        filter1.iport1(inp_sig);
        filter1.oport1(out_sig);

        de2ct1.iport1(out_sig);
        de2ct1.oport1(oport1);
    }
};

//! Helper function to construct a linear process with fixed step size
/*! This function is used to construct a CT filter and connect its
 * input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class I1If>
inline filterf* make_filterf(std::string pName,
    const std::vector<CTTYPE> numerators,  ///< Numerator constants
    const std::vector<CTTYPE> denominators,///< Denominator constants
    const sc_time sample_period,            ///< sampling period
    OIf& outS,
    I1If& inp1S
    )
{
    auto p = new filterf(pName.c_str(), numerators, denominators, sample_period);

    (*p).iport1(inp1S);
    (*p).oport1(outS);

    return p;
}

//! Helper function to construct an integrator
/*! This function is used to construct a CT integrator and connect its
 * input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class I1If>
inline filter* make_integrator(std::string pName,
    const sc_time sample_period,            ///< sampling period
    OIf& outS,
    I1If& inp1S
    )
{
    std::vector<CTTYPE> numerators = {1.0};
    std::vector<CTTYPE> denominators = {1.0, 0.0};

    auto p = new filter(pName.c_str(), numerators, denominators, sample_period);

    (*p).iport1(inp1S);
    (*p).oport1(outS);

    return p;
}

//! Helper function to construct an integrator with fixed step size
/*! This function is used to construct a CT integrator with fixed step
 * size and connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class I1If>
inline filterf* make_integratorf(std::string pName,
    const sc_time sample_period,            ///< sampling period
    OIf& outS,
    I1If& inp1S
    )
{
    std::vector<CTTYPE> numerators = {1.0};
    std::vector<CTTYPE> denominators = {1.0, 0.0};

    auto p = new filterf(pName.c_str(), numerators, denominators, sample_period);

    (*p).iport1(inp1S);
    (*p).oport1(outS);

    return p;
}

//! Process constructor for implementing a PI controller with fixed step
/*! This class is used to build a PI controller with fixed step size
 * based on the proportional and integral gain parameters.
 * It internally uses a scale, an integrator and an adder.
 */
SC_MODULE(pif)
{
    CT_in iport1;           ///< port for the input channel
    CT_out oport1;          ///< port for the output channel;

    fanout fanout1;
    scale scale1;
    filterf integrator1;
    add add1;

    signal fan2p, fan2i, p2add, i2add;

    //! The constructor requires the module name and the gains
    /*!
     */
    pif(sc_module_name _name,       ///< Process name
           const CTTYPE& kp,        ///< Numerator constants
           const CTTYPE& ki,        ///< Denominator constants
           sc_time sample_period    ///< sampling period
          ) : sc_module(_name), fanout1("fanout1"), scale1("scale1", kp),
              integrator1("integrator1", {ki}, {1,0}, sample_period),
              add1("add1")
    {
        fanout1.iport1(iport1);
        fanout1.oport1(fan2p);
        fanout1.oport1(fan2i);

        scale1.iport1(fan2p);
        scale1.oport1(p2add);

        integrator1.iport1(fan2i);
        integrator1.oport1(i2add);

        add1.iport1(p2add);
        add1.iport2(i2add);
        add1.oport1(oport1);
    }
};

//! Helper function to construct a PI controller with fixed step size
/*! This function is used to construct a PI controller with fixed step
 * size and connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class I1If>
inline pif* make_pif(std::string pName,
    const CTTYPE& kp,
    const CTTYPE& ki,
    const sc_time sample_period,
    OIf& outS,
    I1If& inp1S
    )
{
    auto p = new pif(pName.c_str(), kp, ki, sample_period);

    (*p).iport1(inp1S);
    (*p).oport1(outS);

    return p;
}

}
}
#endif
