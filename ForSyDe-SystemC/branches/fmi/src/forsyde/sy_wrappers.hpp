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
#include <iostream>

#include "mi_gdb.h"   //! Based on the GDB/Machine Interface library (libmigdb: http://libmigdb.sourceforge.net/)
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>

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
      
      // Start the executation of the external model
      async_run(d.RunOrContinue());
      
      /* disabling the async mode */
      //~ d.Send("-gdb-set mi-async off");
    }
    
    void prep()
    {
        *ival1 = iport1.read();
        ival1_str<<unsafe_from_abst_ext(*ival1);
        async_run(d.StepOver());
        d.ModifyExpression("forsyde_in1",const_cast<char*>(ival1_str.str().c_str()));
        ival1_str.str(std::string());
    }
    
    void exec()
    {
      // Resume execution
      async_run(d.Continue());
    }
    
    void prod()
    {
      async_run(d.StepOver());
      oval_str.str(d.EvalExpression("forsyde_out"));
      oval_str >> *oval;
      oval_str.clear();
      WRITE_MULTIPORT(oport1, abst_ext<T0>(*oval))
      
      // Resume execution
      async_run(d.Continue());
    }
    
    void clean()
    {
      d.TargetUnselect();
      d.Disconnect();
      delete ival1;
      delete oval;
    }
    
    inline void async_run(int res)
    {
      if (!res)
        SC_REPORT_ERROR(name(),"Error in GDB command execution!");
      
      mi_stop *sr;
      while (!d.Poll(sr)) wait(SC_ZERO_TIME);
      if (sr)
        mi_free_stop(sr);
      else
        SC_REPORT_ERROR(name(),mi_error_from_gdb);
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

//! Process constructor for a pipe wrapper with one input and one output
/*! This class is used to build pipe wrapper with one input and one
 * output. It uses the Unix pipe to communicate and synchronize with an
 * external simulator. The class is parameterized for input and output
 * data-types.
 */
template <typename T0, typename T1>
class pipewrap : public sy_process
{
public:
    SY_in<T1>  iport1;       ///< port for the input channel
    SY_out<T0> oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * provides it to the external model, collects the produced outputs
     * and writes them using the output port
     */
    pipewrap(const sc_module_name& _name,     ///< process name
         const int& offset,                   ///< The offset between the input and output. Positive: read from the model first. Negative: write to the model first.
         const std::string& pipe_path         ///< name of the folder containing forsyde_in1 and forsyde_out pipes
         ) : sy_process(_name), iport1("iport1"), oport1("oport1"),
             offset(offset), pipe_path(pipe_path)
    {
#ifdef FORSYDE_INTROSPECTION
        arg_vec.push_back(std::make_tuple("pipe_path",pipe_path));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::pipewrap";}

private:
    // Inputs and output variables
    T0* oval;
    abst_ext<T1>* ival1;
    std::istringstream oval_str;
    std::ostringstream ival_str;
    
    //! The offset between the input and output
    int offset;
    //! The function passed to the process constructor
    std::string pipe_path;
    
    // Communication pipes
    FILE* inp_pipe;      // Input (to the external model) pipe
    FILE* out_pipe;      // Output (from the external model) pipe
    int inp_pipe_fd, out_pipe_fd;
    bool initiated;
    
    //Implementing the abstract semantics
    void init()
    {
      oval = new T0;
      ival1 = new abst_ext<T1>;
      
      initiated =false;
      inp_pipe_fd = out_pipe_fd = 0;
      
      // Open the pipes. They might be opened in any order
      ival_str.str(pipe_path + "/" + basename() + "_inp");
      oval_str.str(pipe_path + "/" + basename() + "_out");
      
      // TODO: improve error detection for openning the pipes
      while (inp_pipe_fd<=0 || out_pipe_fd<=0) // both pipes are not open
      {
        if (inp_pipe_fd<=0)
        {
          inp_pipe_fd = open(ival_str.str().c_str(), O_WRONLY|O_NONBLOCK);
          if (inp_pipe_fd > 0)
            inp_pipe = fdopen(inp_pipe_fd, "w");
        }
        if (out_pipe_fd<=0)
        {
          out_pipe_fd = open(oval_str.str().c_str(), O_RDONLY|O_NONBLOCK);
          if (out_pipe_fd > 0)
            out_pipe = fdopen(out_pipe_fd, "r");
        }
        wait(SC_ZERO_TIME);
      }
      ival_str.str(std::string());
      oval_str.str(std::string());
    }
    
    void prep()
    {
        *ival1 = iport1.read();
        ival_str<<unsafe_from_abst_ext(*ival1);
        
        while(fprintf(inp_pipe,"%s\n",ival_str.str().c_str())==0)
        {
            if (ferror(inp_pipe) && errno==EAGAIN)
            {
                clearerr(inp_pipe);
                wait(SC_ZERO_TIME);
            }
            else
                SC_REPORT_ERROR(name(),"Error writing to the input pipe.");
        }
        ival_str.str(std::string());
    }
    
    void exec() {}
    
    void prod()
    {
      // FIXME: generalize over the hard coded value 10
      char buf[80];
      while(fgets(buf,80,out_pipe)==NULL)
      {
          //if (errno==ENXIO) printf("ENXIO\n");
          if (feof(out_pipe))
          {
              if (initiated) // Input exhausted!
                  wait();
              else
              {
                  clearerr(out_pipe);
                  wait(SC_ZERO_TIME);
              }
          }
          else if (ferror(out_pipe) && errno==EAGAIN)
              wait(SC_ZERO_TIME);
          else
              SC_REPORT_ERROR(name(),"Error reading from the output pipe.");
      }
      initiated = true;
      
      oval_str.str(std::string(buf));
      oval_str >> *oval;
      oval_str.clear();
      WRITE_MULTIPORT(oport1, abst_ext<T0>(*oval))
    }
    
    void clean()
    {
      fclose(inp_pipe);
      fclose(out_pipe);
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

//! Process constructor for a pipe wrapper with one input and one output
/*! This class is used to build pipe wrapper with two inputs and one
 * output. It uses the Unix pipe to communicate and synchronize with an
 * external simulator. The class is parameterized for input and output
 * data-types.
 */
template <typename T0, typename T1, typename T2>
class pipewrap2 : public sy_process
{
public:
    SY_in<T1>  iport1;       ///< port for the input channel
    SY_in<T2>  iport2;       ///< port for the second channel
    SY_out<T0> oport1;        ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * provides it to the external model, collects the produced outputs
     * and writes them using the output port
     */
    pipewrap2(const sc_module_name& _name,    ///< process name
         const int& offset,                   ///< The offset between the input and output. Positive: read from the model first. Negative: write to the model first.
         const std::string& pipe_path         ///< name of the folder containing forsyde_in1 and forsyde_out pipes
         ) : sy_process(_name), iport1("iport1"), iport2("iport2"), 
             oport1("oport1"), offset(offset), pipe_path(pipe_path)
    {
#ifdef FORSYDE_INTROSPECTION
        arg_vec.push_back(std::make_tuple("pipe_path",pipe_path));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::pipewrap2";}

private:
    // Inputs and output variables
    T0* oval;
    abst_ext<T1>* ival1;
    abst_ext<T2>* ival2;
    std::istringstream oval_str;
    std::ostringstream ival_str;
    
    //! The offset between the input and output
    int offset;
    //! The function passed to the process constructor
    std::string pipe_path;
    
    // Communication pipes
    FILE* inp_pipe;      // Input (to the external model) pipe
    FILE* out_pipe;      // Output (from the external model) pipe
    int inp_pipe_fd, out_pipe_fd;
    bool initiated;
    
    //Implementing the abstract semantics
    void init()
    {
      oval = new T0;
      ival1 = new abst_ext<T1>;
      ival2 = new abst_ext<T2>;
      
      initiated =false;
      inp_pipe_fd = out_pipe_fd = 0;
      
      // Open the pipes. They might be opened in any order
      ival_str.str(pipe_path + "/" + basename() + "_inp");
      oval_str.str(pipe_path + "/" + basename() + "_out");
      
      // TODO: improve error detection for openning the pipes
      while (inp_pipe_fd<=0 || out_pipe_fd<=0) // both pipes are not open
      {
        if (inp_pipe_fd<=0)
        {
          inp_pipe_fd = open(ival_str.str().c_str(), O_WRONLY|O_NONBLOCK);
          if (inp_pipe_fd > 0)
            inp_pipe = fdopen(inp_pipe_fd, "w");
        }
        if (out_pipe_fd<=0)
        {
          out_pipe_fd = open(oval_str.str().c_str(), O_RDONLY|O_NONBLOCK);
          if (out_pipe_fd > 0)
            out_pipe = fdopen(out_pipe_fd, "r");
        }
        wait(SC_ZERO_TIME);
      }
      ival_str.str(std::string());
      oval_str.str(std::string());
    }
    
    void prep()
    {
        if (offset<=0)
        {
            *ival1 = iport1.read();
            *ival2 = iport2.read();
            ival_str<<unsafe_from_abst_ext(*ival1)<<" "<<unsafe_from_abst_ext(*ival2);
            
            while(fprintf(inp_pipe,"%s\n",ival_str.str().c_str())==0)
            {
                if (ferror(inp_pipe) && errno==EAGAIN)
                {
                    clearerr(inp_pipe);
                    wait(SC_ZERO_TIME);
                }
                else
                    SC_REPORT_ERROR(name(),"Error writing to the input pipe.");
            }
            fflush(inp_pipe);
            ival_str.str(std::string());
        }
    }
    
    void exec() {}
    
    void prod()
    {
        if (offset>=0)
        {
            // FIXME: generalize over the hard coded value 80
            char buf[80];
            while(fgets(buf,80,out_pipe)==NULL)
            {
                //if (errno==ENXIO) printf("ENXIO\n");
                if (feof(out_pipe))
                {
                    if (initiated) // Input exhausted!
                        wait();
                    else
                    {
                        clearerr(out_pipe);
                        wait(SC_ZERO_TIME);
                    }
                }
                else if (ferror(out_pipe) && errno==EAGAIN)
                    wait(SC_ZERO_TIME);
                else
                    SC_REPORT_ERROR(name(),"Error reading from the output pipe.");
            }
            initiated = true;
            
            oval_str.str(std::string(buf));
            oval_str >> *oval;
            oval_str.clear();
            WRITE_MULTIPORT(oport1, abst_ext<T0>(*oval))
        }
        if (offset<0) offset++;
        else if (offset>0) offset--;
    }
    
    void clean()
    {
      fclose(inp_pipe);
      fclose(out_pipe);
      delete ival1;
      delete ival2;
      delete oval;
    }
    
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

//! Helper function to construct a pipewrap process
/*! This function is used to construct a pipe wrapper process (SystemC
 * module) and connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class I1If>
inline pipewrap<T0,T1>* make_pipewrap(const std::string& pName,
    const int& offset,
    const std::string& path_name,
    OIf<T0>& outS,
    I1If<T1>& inp1S
    )
{
    auto p = new pipewrap<T0,T1>(pName.c_str(), offset, path_name);
    
    (*p).iport1(inp1S);
    (*p).oport1(outS);
    
    return p;
}

//! Helper function to construct a pipewrap process
/*! This function is used to construct a pipe wrapper process (SystemC
 * module) and connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class T0, template <class> class OIf,
          class T1, template <class> class I1If,
          class T2, template <class> class I2If>
inline pipewrap2<T0,T1,T2>* make_pipewrap2(const std::string& pName,
    const int& offset,
    const std::string& path_name,
    OIf<T0>& outS,
    I1If<T1>& inp1S,
    I2If<T2>& inp2S
    )
{
    auto p = new pipewrap2<T0,T1,T2>(pName.c_str(), offset, path_name);
    
    (*p).iport1(inp1S);
    (*p).iport2(inp2S);
    (*p).oport1(outS);
    
    return p;
}

}
}

#endif
