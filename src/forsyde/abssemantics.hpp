/**********************************************************************           
    * abssemantics.hpp -- The common abstract semantics for all MoCs  *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: The common base for mapping supported MoCs on top of   *
    *          the SystemC simulation kernel.                         *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef ABSSEMANTICS_HPP
#define ABSSEMANTICS_HPP

#include "type_introspect.hpp"

/*! \file abssemantics.hpp
 * \brief The common abstract semantics for all MoCs.
 * 
 *  The common abstract semantics which is used by other MoCs is
 * provided in this file.
 * It is used by other MoCs to implement their semantics on top of the
 * SystemC DE kernel.
 */

//! The namespace for ForSyDe
/*! General namespace that includes everything provided by the SFF.
 * Each MoC has its own sub-namespace.
 */

#include <sstream>
#include <fstream>


namespace ForSyDe
{

using namespace sc_core;

// Auxilliary Macro definitions
#define WRITE_MULTIPORT(PORT,VAL) \
    for (int WMPi=0;WMPi<PORT.size();WMPi++) PORT[WMPi]->write(VAL);

#define WRITE_VEC_MULTIPORT(PORT,VEC) \
    for (int WMPi=0;WMPi<PORT.size();WMPi++) \
        for (auto WMPit=VEC.begin();WMPit!=VEC.end();WMPit++) \
            PORT[WMPi]->write(*WMPit);

//! Type of the object bound to a port
enum bound_type {PORT, CHANNEL};

//! A helper class used to provide introspective channels
class introspective_channel
{
public:
    //! Name of the tokens in the channels
    virtual const char* token_type() const = 0;
    
    // TODO: remove if proved not to be needed
    //~ //! Size of the tokens in the channels
    //~ virtual unsigned token_size() const = 0;
    
    //! To which MoC does the signal belong
    virtual std::string moc() const = 0;
    
    //! Input port to which a channel is bound
    sc_object* iport;
    
    //! Output port to which a channel is bound
    sc_object* oport;
};

//! A ForSyDe signal is used to inter-connect processes
template <typename T, typename TokenType>
class signal: public sc_fifo<TokenType>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_channel
#endif
{
public:
    signal() : sc_fifo<TokenType>() {}
    signal(sc_module_name name, unsigned size) : sc_fifo<TokenType>(name, size) {}
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    // TODO: remove if proved not to be needed
    //~ //! Returns only the size of the token type
    //~ virtual unsigned token_size() const
    //~ {
        //~ return sizeof(T);
    //~ }
    
    // TODO: remove if proved not to be needed
    //! Returns the name of the token type
    virtual const char* token_type() const
    {
        return IntrospectiveType::traverse<T>();
    }
    
    virtual std::string moc() const = 0;
#endif
};

//! This type is used in the process base class to store structural information
struct PortInfo
{
    sc_object* port;
    // TODO: remove if proved not to be needed
    //~ unsigned toks;
    std::string portType;
};

//! A helper class used to provide introspective ports
class introspective_port
{
public:
    //! To which port it is bound (used for binding ports of composite processes in the hierarchy)
    sc_object* bound_port;
    
    //! Name of the tokens of the port
    virtual const char* token_type() const = 0;

    //! To which MoC does the signal belong
    virtual std::string moc() const = 0;
};

//! The in_port port is used for input ports of ForSyDe processes
template <typename T, typename TokenType, typename ChanType>
class in_port: public sc_fifo_in<TokenType>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
    in_port() : sc_fifo_in<TokenType>(){}
    in_port(const char* name) : sc_fifo_in<TokenType>(name){}
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    //! Record the bounded channels
    void operator()(sc_fifo_in_if<TokenType>& i)
    {
        sc_fifo_in<TokenType>::operator()(i);
        static_cast<ChanType&>(i).iport = this;
    }
    
    //! Record the bounded ports
    void operator()(in_port<T,TokenType,ChanType>& p)
    {
        sc_fifo_in<TokenType>::operator()(p);
        p.bound_port = this;
    }
    
    //! Returns the plain name of the token type
    virtual const char* token_type() const
    {
        return IntrospectiveType::traverse<T>();
    }

    virtual std::string moc() const = 0;
#endif
};

//! The UT_out port is used for output ports of UT processes
template <typename T, typename TokenType, typename ChanType>
class out_port: public sc_fifo_out<TokenType>
#ifdef FORSYDE_INTROSPECTION
            , public ForSyDe::introspective_port
#endif
{
public:
    out_port() : sc_fifo_out<TokenType>(){}
    out_port(const char* name) : sc_fifo_out<TokenType>(name){}
#ifdef FORSYDE_INTROSPECTION
    typedef T type;
    
    // NOTE: The following member functions could be overriden easier if
    //       bind() was declared virtual in the sc_port base classes.
    //       This will happen in SystemC 2.3, so adapt these accordingly.
    //! Record the bounded channels
    void operator()(sc_fifo_out_if<TokenType>& i)
    {
        sc_fifo_out<TokenType>::operator()(i);
        // Register the port-to-port binding
        static_cast<ChanType&>(i).oport = this;
    }
    
    //! Record the bounded ports
    void operator()(out_port<T,TokenType,ChanType>& p)
    {
        sc_fifo_out<TokenType>::operator()(p);
        // Register the port-to-port binding
        p.bound_port = this;
    }
    
    //! Returns the name of the actual type (not abst_ext version)
    virtual const char* token_type() const
    {
        return IntrospectiveType::traverse<T>();
    }

    virtual std::string moc() const = 0;
#endif
};

//! The process constructor which defines the abstract semantics of execution
/*! This class defines a set of methods and their execution order which
 * together define the abstract execution semantics of the processes in
 * ForSyDe-SystemC.
 * In each MoC, process constructors implement the these methods 
 * according to its own semantics. 
 * Additionally, this class contains members which are used to collect
 * and store information about the structure of the models which is used
 * for introspection in the elaboration phase.
 * 
 * Note that this is an abstract class and can not be directly
 * instantiated.
 * The designer uses the process constructors which implement the
 * abstract methods in a specific MoC.
 */
class process : public sc_module
{
private:
    //! 
    SC_HAS_PROCESS(process);

    //! The main and only execution thread of the module
    void worker()
    {
        //  We run the init stage here and not in the constructor to
        // force running it after the elaboration phase.
        init();
        while (1)
        {
            prep();     // The preparaion stage
            exec();     // The execution stage
            prod();     // The production stage
        }
    }

protected:
    //! The init stage
    /*! This stage is executed once in the beginning and is responsible
     * for initialization tasks such as allocating IO buffers, etc.
     */
    virtual void init() = 0;
    
    //! The prep stage
    /*! This stage is executed continuously in a loop and is responsible
     * for preparaing the inputs to the execution phase.
     */
    virtual void prep() = 0;
    
    //! The exece stage
    /*! This stage is executed continuously in a loop and executes the
     * main functionality of the process (e.g., by applying a supplied
     * function).
     */
    virtual void exec() = 0;
    
    //! The prod stage
    /*! This stage is executed continuously in a loop and is responsible
     * for writing the computed results to the output.
     */
    virtual void prod() = 0;
    
    //! The clean stage
    /*! This stage is executed once at the end and is responsible for
     * cleaning jobs such as deallocation of the allocated memories, etc.
     */
    virtual void clean() = 0;
    
    //! This hook is used to run the clean stage
    void end_of_simulation()
    {
        clean();
    }
    
#ifdef FORSYDE_INTROSPECTION

    //! This hook is used to collect additional structural information
    void end_of_elaboration()
    {
        bindInfo();
    }

    //! This method is called during end_of_elaboration to gather binded channels information
    /*! This function should save the pointers to all of the channels
     * objects bound to the input and output channels in boundInChans
     * and boundOutChans respectively
     */
    virtual void bindInfo() = 0;
#endif

public:

#ifdef FORSYDE_INTROSPECTION
    //! Pointers to the input ports and their bound channels
    std::vector<PortInfo> boundInChans;
    //! Pointers to the output ports and their bound channels
    std::vector<PortInfo> boundOutChans;
    
    //! Vector holding a list of argument/value tuples passed to the process constructor
    std::vector<std::tuple<std::string,std::string>> arg_vec;
#endif
 
    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * processes them and writes the results using the output port.
     */
    process(sc_module_name _name    ///< The name of the ForSyDe process
            ): sc_module(_name)
    {
        SC_THREAD(worker);
    }
    
    //! The ForSyDe process type represented by the current module
    virtual std::string forsyde_kind() const = 0;
    
};

}

#endif
