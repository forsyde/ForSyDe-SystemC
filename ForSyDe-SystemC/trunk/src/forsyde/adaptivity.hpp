/**********************************************************************           
    * adaptivity.hpp -- Definition of some adaptive processes         *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing promitive element required for modeling      *
    *          adaptive systems in ForSyDe-SystemC                    *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef ADAPTIVITY_HPP
#define ADAPTIVITY_HPP

/*! \file adaptivity.hpp
 * \brief Definition of some adaptive processes
 * 
 *  This file includes the basic process constructors and other
 * facilities used for modeling adaptive systems.
 */

#include "sy_moc.hpp"
#include <functional>

namespace ForSyDe
{

namespace SY
{

using namespace sc_core;

// Auxilliary Macro definitions
#define WRITE_MULTIPORT(PORT,VAL) \
    for (int i=0;i<PORT.size();i++) PORT[i]->write(VAL);


//! Process constructor for a combinational adaptive process with one input and one output
/*! This class is used to build combinational processes with one input
 * and one output. The class is parameterized for input and output
 * data-types.
 */
template <class ITYP, class OTYP>
class apply : public sc_module
{
public:
    SY_in<ITYP>  iport;        ///< port for the input channel
    SY_out<OTYP> oport;        ///< port for the output channel
    
    //! Type of the function to be passed to the process constructor
    typedef std::function<abst_ext<OTYP>(const abst_ext<ITYP>&)> functype;
    SY_in<functype> fport;     ///< port for the function channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    apply(sc_module_name _name)  // module name
         :sc_module(_name)
    {
        SC_THREAD(worker);
    }
private:
    SC_HAS_PROCESS(apply);

    //! The main and only execution thread of the module
    void worker()
    {
        abst_ext<ITYP> in_val;
        abst_ext<OTYP> out_val;
        functype cur_f;
        while (1)
        {
            in_val = iport.read();  // read from input
            cur_f = unsafe_from_abst_ext(fport.read());  // read the function
            out_val = cur_f(in_val);// do the calculation
            WRITE_MULTIPORT(oport,out_val);    // write to the output
        }
    }
};

//! Helper function to construct a comb process
/*! This function is used to construct a process (SystemC module) and
 * connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class I1If,
                    template <class> class FIf>
inline apply<T0,T1>* make_apply(const std::string& pName,
    OIf<T0>& outS,
    I1If<T1>& inp1S,
    FIf<typename apply<T0,T1>::functype>& fS
    )
{
    auto p = new apply<T0,T1>(pName.c_str());
    
    (*p).iport(inp1S);
    (*p).oport(outS);
    (*p).fport(fS);
    
    return p;
}

}
}

#endif
