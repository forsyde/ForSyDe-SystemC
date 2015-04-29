/**********************************************************************           
    * parallel_sim.hpp -- Primitives used for parallel simulation     *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing primitive elements required for simulating   *
    *          ForSyDe models using MPI                               *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef PARALLELSIM_HPP
#define PARALLELSIM_HPP

/*! \file parallel_sim.hpp
 * \brief Definition of sender and receiver processes
 * 
 *  This file includes the basic process constructors and other
 * facilities used for enabling parallel simulations.
 */

#include <mpi.h>

namespace ForSyDe
{

namespace SY
{

using namespace sc_core;


//! Process constructor for a sender process with one input
/*! This class is used to build a processes with one input. It transmits
 * the non-absent events it receives using an MPI_send command.
 */
template <typename T1>
class sender : public sy_process
{
public:
    SY_in<T1>  iport1;       ///< port for the input channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    sender(sc_module_name _name,     ///< process name
           int destination,          ///< MPI rank of the destination process
           int tag                   ///< MPI tag of the message
         ) : sy_process(_name), iport1("iport1"),
             destination(destination), tag(tag)
    {
#ifdef FORSYDE_INTROSPECTION
        arg_vec.push_back(std::make_tuple("destination",std::to_string(destination)));
        arg_vec.push_back(std::make_tuple("tag",std::to_string(tag)));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::sender";}

private:
    int destination;
    int tag;
    
    // Inputs and output variables
    T1* ival1;
    
    //Implementing the abstract semantics
    void init()
    {
        ival1 = new T1;
    }
    
    void prep()
    {
        abst_ext<T1> temp_val = iport1.read();
        *ival1 = unsafe_from_abst_ext(temp_val);
    }
    
    void exec() {}
    
    void prod()
    {
        MPI_Request request;
        MPI_Status status;
        MPI_Isend(ival1, sizeof(T1), MPI_BYTE, destination, tag, MPI_COMM_WORLD, &request);
        int flag=0;
        while (true)
        {
            MPI_Test(&request, &flag, &status);
            if (flag) break; else wait(0,SC_NS);
        }
    }
    
    void clean()
    {
        delete ival1;
    }
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundInChans.resize(1);     // only one input port
        boundInChans[0].port = &iport1;
    }
#endif
};

//! Process constructor for a receiver process with one output
/*! This class is used to build a processes with one output.
 * It receives non-absent events via MPI_recv command and writes them to
 * its output signal.
 */
template <typename T0>
class receiver : public sy_process
{
public:
    SY_out<T0>  oport1;       ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    receiver(sc_module_name _name,     ///< process name
           int source,                 ///< MPI rank of the source process
           int tag                     ///< MPI tag of the message
         ) : sy_process(_name), oport1("oport1"),
             source(source), tag(tag)
    {
#ifdef FORSYDE_INTROSPECTION
        arg_vec.push_back(std::make_tuple("source",std::to_string(source)));
        arg_vec.push_back(std::make_tuple("tag",std::to_string(tag)));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "SY::receiver";}

private:
    int source;
    int tag;
    
    // Inputs and output variables
    abst_ext<T0>* oval1;
    
    //Implementing the abstract semantics
    void init()
    {
        oval1 = new abst_ext<T0>;
    }
    
    void prep()
    {
        T0 temp_val;
        MPI_Status status;
        MPI_Request request;
        MPI_Irecv(&temp_val, sizeof(T0), MPI_BYTE, source, tag, MPI_COMM_WORLD, &request);
        int flag=0;
        while (true)
        {
            MPI_Test(&request, &flag, &status);
            if (flag) break; else wait(0,SC_NS);
        }
        set_val(*oval1, temp_val);
    }
    
    void exec() {}
    
    void prod()
    {
        WRITE_MULTIPORT(oport1, *oval1)
    }
    
    void clean()
    {
        delete oval1;
    }
    
#ifdef FORSYDE_INTROSPECTION
    void bindInfo()
    {
        boundOutChans.resize(1);    // only one output port
        boundOutChans[0].port = &oport1;
    }
#endif
};


}
}

#endif
