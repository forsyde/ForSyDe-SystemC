/**********************************************************************           
    * sy_wrappers.hpp -- Co-simulation wrappers for the SY MOC        *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing co-simulation wrappers for integration of    *
    *          foreign models into ForSyDe                            *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SY_WRAPPERS_HPP
#define SY_WRAPPERS_HPP

/*! \file sy_wrappers.hpp
 * \brief Implements a set of co-simulation wrappers in the SY MoC
 * 
 *  This file includes a set of co-simulation wrappers for integration
 * of freign models into the synchronous model of computation of ForSyDe.
 */

#include <functional>
#include <tuple>

#include "mi_gdb.h"   //! Based on the GDB/Machine Interface library (libmigdb: http://libmigdb.sourceforge.net/) 

#include "abst_ext.hpp"
#include "sy_process.hpp"

namespace ForSyDe
{

namespace SY
{

using namespace sc_core;

//! Process constructor for a GDB wrapper with one input and one output
/*! This class is used to build GDB wrapper with one input and one
 * output. It uses the GDB machine interface (GDB/MI) to communicate
 * with an instance of GDB. The class is parameterized for input and
 * output data-types.
 */
template <typename T0, typename T1>
class gdbwrap : public sy_process
{
public:
    SY_in<T1>  iport1;       ///< port for the input channel
    SY_out<T0> oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * provides it to the external model, triggers the execution of the
     * external model, collects the produced outputs and writes them
     * using the output port
     */
    gdbwrap(const sc_module_name& _name,      ///< process name
         const std::string& exec_name         ///< name of the executable file
         ) : sy_process(_name), iport1("iport1"), oport1("oport1"),
             exec_name(exec_name)
    {
#ifdef FORSYDE_INTROSPECTION
        std::string func_name = std::string(basename());
        func_name = func_name.substr(0, func_name.find_last_not_of("0123456789")+1);
        arg_vec.push_back(std::make_tuple("exec_name",exec_name));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::gdbwrap";}

private:
    // Inputs and output variables
    T0* oval;
    abst_ext<T1>* ival1;
    std::istringstream oval_str;
    std::ostringstream ival1_str;
    
    //! The function passed to the process constructor
    std::string exec_name;
    
    // Debugger-specific definitions
    MIDebugger d;                       
    mi_bkpt *bk_in1, *bk_out;
    
    //Implementing the abstract semantics
    void init()
    {
      oval = new T0;
      ival1 = new abst_ext<T1>;
      // Connect to gdb child.
      if (!d.Connect())
        SC_REPORT_ERROR(name(),"Connection to child GDB instance failed.");
      
      // Set the name of the child and the command line aguments.
      // It also opens the xterm.
      if (!d.SelectTargetX11(exec_name.c_str()))
        SC_REPORT_ERROR(name(),"Error executing the external model");
      
      /* Set the breakpoints */
      bk_in1=d.Breakpoint("forsyde_read_in1");
      bk_out=d.Breakpoint("forsyde_write_out");
      if (!bk_in1 || !bk_out)
        SC_REPORT_ERROR(name(),"Error Setting the breakpoints");
      mi_free_bkpt(bk_in1);
      mi_free_bkpt(bk_out);
      
      /* disabling the async mode */
      d.Send("-gdb-set mi-async off");
    }
    
    void prep()
    {
        *ival1 = iport1.read();
        ival1_str<<unsafe_from_abst_ext(*ival1);
        d.RunOrContinue();
        d.StepOver();
        d.ModifyExpression("forsyde_in1",const_cast<char*>(ival1_str.str().c_str()));
    }
    
    void exec() {}
    
    void prod()
    {
      d.RunOrContinue();
      d.StepOver();
      if (d.EvalExpression("forsyde_out") == 0) std::cout << "error!" << std::endl;
      oval_str.str(d.EvalExpression("forsyde_out"));
      oval_str >> *oval;
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

//! Helper function to construct a gdbwrap process
/*! This function is used to construct a GDB wrapper process (SystemC
 * module) and connect its output and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class I1If>
inline gdbwrap<T0,T1>* make_gdbwrap(const std::string& pName,
    const std::string& exec_name,
    OIf<T0>& outS,
    I1If<T1>& inp1S
    )
{
    auto p = new gdbwrap<T0,T1>(pName.c_str(), exec_name);
    
    (*p).iport1(inp1S);
    (*p).oport1(outS);
    
    return p;
}

}
}

#endif
