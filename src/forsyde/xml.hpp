/**********************************************************************
    * xml.hpp -- Dumps the system model as the abstract XML+C format  * 
    *                                                                 *
    * Authors: Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Dumps the structure and behavior of a system model     *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef XML_HPP
#define XML_HPP

/*! \file xml.hpp
 * \brief Dumps the system model as the XML+C abstract format.
 * 
 *  This file includes functions which can be used in order to export
 * the structure and behavior of a specified system in an abstract
 * format represented as an XML file plus a set of CPP fiels.
 * This format can be used by other tools for further manipulation.
 */

#include <algorithm>
#include "rapidxml_print.hpp"

#include "abssemantics.hpp"

using namespace rapidxml;
using namespace boost;

namespace ForSyDe
{
using namespace sc_core;

// Some Helper Fuctions
//! Extracts the MoC and the process constructor name from a ForSyDe kind.
void get_moc_and_pc(const std::string& kind, std::string& moc, std::string& pc)
{
    moc = kind.substr(0, kind.find(':'));
    pc = kind.substr(kind.rfind(':')+1, kind.length());
}

//! Abstract class used to Export a system as an XML file
/*! This class provides basic facilities to export a ForSyDe-SystemC
 * process network as an XML file.
 */
class XMLExport
{
public:
    //! The constructor takes the gneration path
    XMLExport(std::string path) : path(path)
    {
        // Allocate global names
        const_name = (char*)"name";
        const_leaf_process = (char*)"leaf_process";
        const_composite_process = (char*)"composite_process";
        const_component_name = (char*)"component_name";
        const_process_network = (char*)"process_network";
        const_process_constructor = (char*)"process_constructor";
        const_argument = (char*)"argument";
        const_value = (char*)"value";
        const_moc = (char*)"moc";
        const_type = (char*)"type";
        const_sdf = (char*)"sdf";
        const_sy = (char*)"sy";
        const_de = (char*)"de";
        const_dt = (char*)"dt";
        const_ct = (char*)"ct";
        const_ut = (char*)"ut";
        const_port = (char*)"port";
        const_port_dir = (char*)"port_dir";
        const_direction = (char*)"direction";
        const_in = (char*)"in";
        const_out = (char*)"out";
        const_signal = (char*)"signal";
        const_source = (char*)"source";
        const_source_port = (char*)"source_port";
        const_target = (char*)"target";
        const_target_port = (char*)"target_port";
        const_bound_process = (char*)"bound_process";
        const_bound_port = (char*)"bound_port";
    }
    
    //! The destructor makes sure all XML nodes are deallocated.
    ~XMLExport()
    {
        xml_doc.clear();
    }
    
    //! The traverse function requires the top ForSyDe process name
    /*! It initiates the translation job which is a recursive traversal
     * of the process network.
     */
    void traverse(sc_module* top)
    {
        // Initiate the XML file for this level of hierarchy
        xml_node<>* pn_node = init(top);
        
        // Get the list of module children (ports and other processes)
        std::vector<sc_object*> children = top->get_child_objects();
        
        // Take care of children objects
        for (auto it=children.begin();it!=children.end();it++)
        {
            // Check if it is a module, a port, or something else
            if (is_module(*it))
            {
                // Is it a leaf or a composite process?
                if (is_leaf(*it))      // Leaf process
                {
                    // Add it to the XML document
                    add_leaf_process(static_cast<ForSyDe::process*>(*it), pn_node);
                }
                else
                {                       // Composite process
                    add_composite_process(static_cast<sc_module*>(*it), pn_node);
                    // Recursion step
                    XMLExport dumper(path);
                    dumper.traverse(static_cast<ForSyDe::process*>(*it));
                }
            }
            else if (is_port(*it))
            {
                // TODO: determining port direction can be done in a better way
                char* dir = (*it)->kind()==std::string("sc_fifo_in") ? const_in : const_out;
                const char* bound_process = dynamic_cast<ForSyDe::introspective_port*>(*it)
                    -> bound_port -> get_parent_object() -> basename();
                const char* bound_port = dynamic_cast<ForSyDe::introspective_port*>(*it)
                    -> bound_port -> basename();
                add_port(dynamic_cast<ForSyDe::introspective_port*>(*it), dir, pn_node, bound_process, bound_port);
            }
            else if (is_signal(*it))
            {
                add_signal(dynamic_cast<ForSyDe::introspective_channel*>(*it), pn_node);
            }
        }
        // Last, print the XML structure
        // NOTE: Extract the composite process name based on the
        // convention: "nameX" or "nameXX", where Xs are 0-9
        std::string name_str(top->basename());
        name_str = name_str.substr(0, name_str.find_last_not_of("0123456789")+1).c_str();
        printXML(path + name_str + std::string(".xml"));
        
    }
    
    //! The init member initiates the XML DOM and performs initial settings.
    /*! 
     */
    xml_node<>* init(const sc_module* p)
    {
        // The top most node
        xml_node<> *pn_node = xml_doc.allocate_node(node_element, const_process_network);
        xml_doc.append_node(pn_node);
        // NOTE: Extract the composite process name based on the
        // convention: "nameX" or "nameXX", where Xs are 0-9
        std::string name_str(p->basename());
        char* xml_composite_name = xml_doc.allocate_string(
            name_str.substr(0, name_str.find_last_not_of("0123456789")+1).c_str()
        );
        allocate_append_attribute(pn_node, const_name, xml_composite_name);
        
        return pn_node;
    }
    
    //! The print method writes the XML file to the output.
    /*! The XML structure is already generated, so this command only
     * checks for the availability of the output file and dumps the XML
     * to it.
     */
    void printXML(std::string fileName)
    {
        std::ofstream outFile(fileName);
        if (!outFile.is_open())
            SC_REPORT_ERROR(fileName.c_str(), "file could not be opened");
        outFile << "<?xml version=\"1.0\" ?>" << std::endl;
        outFile << "<!-- Automatically generated by ForSyDe -->" << std::endl;
        outFile << "<!DOCTYPE process_network SYSTEM \"forsyde.dtd\" >"  << std::endl;
        outFile << xml_doc;
    }
    
    //! Add a leaf process
    void add_leaf_process(const ForSyDe::process* p, xml_node<>* pn_node)
    {
        // Determine the process consructor and the belonging MoC
        std::string moc, pc;
        get_moc_and_pc(p->forsyde_kind(), moc, pc);
        char* xml_pc = xml_doc.allocate_string(pc.c_str());
        char* moc_name;
        if (moc=="SDF") moc_name = const_sdf;
        else if (moc=="SY") moc_name = const_sy;
        else if (moc=="DE") moc_name = const_de;
        else if (moc=="DT") moc_name = const_de;
        else if (moc=="CT") moc_name = const_ct;
        else if (moc=="UT") moc_name = const_ut;
        else
        {
            SC_REPORT_ERROR("XML Backend", "MoC could not be deduced from kind.");
            return;
        }
        
        // Add the process node
        xml_node<> *p_node = allocate_append_node(pn_node, const_leaf_process);
        allocate_append_attribute(p_node, const_name, p->basename());
        
            // Add the leaf process ports
            add_leaf_process_ports(p, p_node);
        
            // Add the process constructor node
            xml_node<> *pc_node = allocate_append_node(p_node, const_process_constructor);
            allocate_append_attribute(pc_node, const_name, xml_pc);
            allocate_append_attribute(pc_node, const_moc, moc_name);
            
            // Add arguments
            for (auto it=p->arg_vec.begin();it!=p->arg_vec.end();it++)
            {
                xml_node<> *arg_node = allocate_append_node(pc_node, const_argument);
                char* arg_name = xml_doc.allocate_string(std::get<0>(*it).c_str());
                char* arg_val = xml_doc.allocate_string(std::get<1>(*it).c_str());
                allocate_append_attribute(arg_node, const_name, arg_name);
                allocate_append_attribute(arg_node, const_value, arg_val);
            }
    }
    
    //! Add the ports for a leaf process
    void add_leaf_process_ports(const ForSyDe::process* p, xml_node<>* pn_node)
    {
        for (auto it=p->boundInChans.begin();it!=p->boundInChans.end();it++)
            add_port(dynamic_cast<ForSyDe::introspective_port*>((*it).port), const_in, pn_node);
        for (auto it=p->boundOutChans.begin();it!=p->boundOutChans.end();it++)
            add_port(dynamic_cast<ForSyDe::introspective_port*>((*it).port), const_out, pn_node);
    }
    
    //! Add a composite process
    void add_composite_process(const sc_module* p, xml_node<>* pn_node)
    {
        xml_node<> *p_node = allocate_append_node(pn_node, const_composite_process);
        allocate_append_attribute(p_node, const_name, p->basename());
        // NOTE: Extract the composite process name based on the
        // convention: "nameX" or "nameXX", where Xs are 0-9
        std::string name_str(p->basename());
        char* xml_composite_name = xml_doc.allocate_string(
            name_str.substr(0, name_str.find_last_not_of("0123456789")+1).c_str()
        );
        allocate_append_attribute(p_node, const_component_name, xml_composite_name);
        // Add port nodes
        std::vector<sc_object*> children = p->get_child_objects();
        std::for_each(children.begin(), children.end(), [&](sc_object* it)
        {
            if (is_port(it))
            {
                // TODO: determining port direction can be done in a better way
                char* dir = it->kind()==std::string("sc_fifo_in") ? const_in : const_out;
                add_port(dynamic_cast<ForSyDe::introspective_port*>(it), dir, p_node);
            }
        });
    }
    
    //! Add a port
    void add_port(introspective_port* port, const char* dir, xml_node<>* pn_node, 
                  const char* bound_process=NULL, const char* bound_port=NULL)
    {
        xml_node<> *p_node = allocate_append_node(pn_node, const_port);
        if (port != NULL)
        {
            allocate_append_attribute(p_node, const_name, dynamic_cast<sc_object*>(port)->basename());
            allocate_append_attribute(p_node, const_type, port->token_type());
            allocate_append_attribute(p_node, const_direction, dir);
        }
        if (bound_process != NULL && bound_port != NULL)
        {
            allocate_append_attribute(p_node, const_bound_process, bound_process);
            allocate_append_attribute(p_node, const_bound_port, bound_port);
        }
    }
    
    //! Add a ForSyDe signal
    void add_signal(introspective_channel* sig, xml_node<>* pn_node)
    {
        xml_node<> *sig_node = allocate_append_node(pn_node, const_signal);
        allocate_append_attribute(sig_node, const_name, dynamic_cast<sc_object*>(sig)->basename());
        char* moc_name;
        if (sig->moc()=="SDF") moc_name = const_sdf;
        else if (sig->moc()=="SY") moc_name = const_sy;
        else if (sig->moc()=="DE") moc_name = const_de;
        else if (sig->moc()=="DT") moc_name = const_dt;
        else if (sig->moc()=="CT") moc_name = const_ct;
        else if (sig->moc()=="UT") moc_name = const_ut;
        else
        {
            SC_REPORT_ERROR("XML Backend", "MoC could not be deduced from kind.");
            return;
        }
        allocate_append_attribute(sig_node, const_moc, moc_name);
        allocate_append_attribute(sig_node, const_type, sig->token_type());
        allocate_append_attribute(sig_node, const_source, sig->oport->get_parent_object()->basename());
        allocate_append_attribute(sig_node, const_source_port, sig->oport->basename());
        allocate_append_attribute(sig_node, const_target, sig->iport->get_parent_object()->basename());
        allocate_append_attribute(sig_node, const_target_port, sig->iport->basename());
    }
    
    //! Check if the systemC object is a leaf process
    inline bool is_leaf(sc_object* module)
    {
        return dynamic_cast<ForSyDe::process*>(module) != NULL;
    }
    
    //! Check if the systemC object is module
    inline bool is_module(const sc_object* module)
    {
        return module->kind() == std::string("sc_module");
    }
    
    //! Check if the systemC object is port
    inline bool is_port(sc_object* port)
    {
        return dynamic_cast<introspective_port*>(port) != NULL;
    }
    
    //! Check if the systemC object is a ForSyDe signal
    inline bool is_signal(const sc_object* sig)
    {
        return sig->kind() == std::string("sc_fifo");
    }

private:
    //! The Path for generating the output
    std::string path; 
    
    //! The RapidXML DOM
    xml_document<> xml_doc;
    
    //! Some global constant names
    char *const_name, *const_leaf_process, *const_composite_process, 
         *const_process_network, *const_process_constructor, *const_moc,
         *const_type, *const_port, *const_sdf, *const_sy, *const_de,
         *const_dt, *const_ct, *const_ut,
         *const_port_dir, *const_direction, *const_in, *const_out,
         *const_signal, *const_component_name, *const_argument, *const_value,
         *const_source, *const_source_port, *const_target, *const_target_port,
         *const_bound_process, *const_bound_port;
    
    inline xml_node<>* allocate_append_node(xml_node<>* top, const char* name)
    {
        xml_node<>* node = xml_doc.allocate_node(node_element, name);
        top->append_node(node);
        return node;
    }
    
    inline void allocate_append_attribute(xml_node<>* node, const char* attr_name, const char* attr_val)
    {
        xml_attribute<>* attr = xml_doc.allocate_attribute(attr_name, attr_val);
        node->append_attribute(attr);
    }
    
};



}

#endif
