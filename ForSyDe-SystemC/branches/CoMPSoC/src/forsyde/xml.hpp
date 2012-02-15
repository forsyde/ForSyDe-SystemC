/**********************************************************************
    * xml.hpp -- Dumps the structure of a system specification as an  *
    *          XML file.                                              *
    *                                                                 *
    * Authors: Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Dumps the structure of a system                        *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef XML_HPP
#define XML_HPP

/*! \file xml.h
 * \brief Dumps the structure of a system specification as an XML file.
 * 
 *  This file includes functions which can be used in the sc_main
 * function before sc_start in order to export the structure of a
 * specified system in an XML format accepted as input to other tools.
 */

#include <vector>
#include <tuple>
#include <map>
#include <iostream>
#include <dirent.h>
#include <algorithm>
#include "rapidxml_print.hpp"

using namespace rapidxml;

namespace ForSyDe
{
using namespace sc_core;

struct ChannelInfo
{
    sc_object* srcActor;
    sc_object* srcPort;
    unsigned   prodRate;
    sc_object* desActor;
    sc_object* desPort;
    unsigned   consRate;
    unsigned   tokenSize;
    unsigned   initToks;
};

//! Abstract class used to Export a system as an XML file
/*! This class provides basic facilities to export a ForSyDe-SystemC
 * process network as an XML file. For each specific platform there will
 * be another class derived form this one to comply with the format
 * required by the target platform. The derived classes will implement
 * the pure virtual functions in order to generate appropriate XML
 * format.
 */
class XMLExport
{
public:
    //! The traverse function requires the top ForSyDe process name
    /*! It initiates the translation job which is a recursive traversal
     * of the process network, calling pure virtual functions to create
     * target-specific output XML format.
     */
    void traverse(sc_module* top)
    {
        xml_node<> * top_node = init(top);
        if (isModule(top))
        {
            xml_node<> * node = addProcess(top, top_node);
            std::vector<sc_object*> children = top->get_child_objects();
            for ( unsigned i = 0; i < children.size(); i++ )
                recurseNode(children[i], node);
        }
        // Now write the channels
        for (auto it=channels.begin();it!=channels.end();it++)
        {
            xml_node<> *node = doc.allocate_node(node_element, "channel");
            top_node->append_node(node);
            xml_attribute<> *chanNameAttr = doc.allocate_attribute("name", (it->first).c_str());
            node->append_attribute(chanNameAttr);
            if (std::get<0>(it->second))
            {
            xml_attribute<> *srcActorAttr = doc.allocate_attribute("srcActor", (std::get<0>(it->second))->name());
            node->append_attribute(srcActorAttr);
            }
            if (std::get<1>(it->second))
            {
            xml_attribute<> *srcPortAttr = doc.allocate_attribute("srcPort", (std::get<1>(it->second))->name());
            node->append_attribute(srcPortAttr);
            }
            if (std::get<2>(it->second))
            {
            xml_attribute<> *desActorAttr = doc.allocate_attribute("desActor", (std::get<2>(it->second))->name());
            node->append_attribute(desActorAttr);
            }
            if (std::get<3>(it->second))
            {
            xml_attribute<> *desPortAttr = doc.allocate_attribute("desPort", (std::get<3>(it->second))->name());
            node->append_attribute(desPortAttr);
            }
        }
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
        outFile << "<!-- Automatically generated by ForSyDe -->" << std::endl;
        outFile << doc;
    }

protected:
    
    //! Initialization of the XML document
    /*! This method adds the header information to the XML document.
     */
    virtual xml_node<>* init(sc_module*) = 0;
    
    //! Adds all the information associated to a ForSyDe process to the XML
    /*! This method is called for every single node in the hierarchy.
     */
    virtual xml_node<>* addProcess(sc_object*, xml_node<>*) = 0;
    
    //! Adds all the information associated to a ForSyDe channel to the XML
    /*! This method is called for every single channel in the hierarchy.
     */
    virtual void addChannel(sc_object*, xml_node<>*) = 0;
    
    //! The recursive function used for the graph traversal.
    /*! 
     */
    virtual void recurseNode(sc_object*, xml_node<>*) = 0;
    
    //! Checks if the SystemC object under question has the sc_module kind
    /*! 
     */
    inline bool isModule(sc_object* obj)
    {
        return !strcmp(obj->kind(), "sc_module");
    }

    xml_document<> doc;
    std::map<std::string,
            std::tuple<sc_object*,sc_object*,sc_object*,sc_object*>
            > channels;
};

//! Class used to Export a system specification for the ComPMSoC platform
/*! This class provides basic facilities to export a ForSyDe-SystemC
 * process network as an XML file for the CoMPSoC platform. The process
 * passed to it and all the processes included in the hierarchy should
 * be from the valid MoCs SY(), SDF, or CSDF.
 */
class CoMPSoCExport
{

public:

    xml_node<>* init(sc_module* p)
    {
        // Header Information
        xml_node<> *nestNode = doc.allocate_node(node_element, "nest");
        doc.append_node(nestNode);
        xml_attribute<> *xmlnsAttr = doc.allocate_attribute("xmlns", "uri:nest");
        nestNode->append_attribute(xmlnsAttr);
        xml_attribute<> *xsiAttr = doc.allocate_attribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
        nestNode->append_attribute(xsiAttr);
        xml_attribute<> *typeAttr = doc.allocate_attribute("type", "csdf");
        nestNode->append_attribute(typeAttr);
        xml_attribute<> *verAttr = doc.allocate_attribute("version", "1.0");
        nestNode->append_attribute(verAttr);
        xml_attribute<> *schemaAttr = doc.allocate_attribute("xsi:schemaLocation", "uri:nest http://www.es.ele.tue.nl/nest/xsd/csdf.xsd");
        nestNode->append_attribute(schemaAttr);
        
        // The top most node
        xml_node<> *appNode = doc.allocate_node(node_element, "applicationGraph");
        nestNode->append_node(appNode);
        xml_attribute<> *appNameAttr = doc.allocate_attribute("name", p->name());
        appNode->append_attribute(appNameAttr);
        
        return appNode;
    }
    
    //! The traverse function requires the top ForSyDe process name
    /*! It initiates the translation job which is a recursive traversal
     * of the process network.
     */
    void traverse(sc_module* top)
    {
        xml_node<> * appNode = init(top);
        
        // The csdfgraph node which includes the system structure
        xml_node<> *csdfGraphNode = doc.allocate_node(node_element, "csdfgraph");
        appNode->append_node(csdfGraphNode);
        xml_attribute<> *csdfGraphNameAttr = doc.allocate_attribute("name", top->name());
        csdfGraphNode->append_attribute(csdfGraphNameAttr);
        
        // First, find and add the leaf processes, during recursion
        // throughout the system structure more information is also
        // collected which will be used in later stages.
        if (isModule(top))
            recurseNode(top, csdfGraphNode);
        
        // Second, optimize and specialize
        // Change the delay elements to initial tokens on arcs
        for (auto it=processes.begin();it!=processes.end();it++)
        {
            if (!strcmp(it->second->ForSyDe_kind().c_str(), "SDF::delayn"))
            {
                // find the incoming and outgoing channels
                decltype(channels.begin()) incoming, outgoing;
                for (auto it2=channels.begin();it2!=channels.end();it2++)
                {
                    if (it2->second.desActor == it->second)
                        incoming = it2;
                    if (it2->second.srcActor == it->second)
                        outgoing = it2;
                }
                // connect the output of the incoming channel to the input
                // of the destination actor
                incoming->second.desActor = outgoing->second.desActor;
                incoming->second.desPort = outgoing->second.desPort;
                incoming->second.consRate = outgoing->second.consRate;
                // remove the outgoing channel
                channels.erase(outgoing);
                // specify the initial tokens on channels
                incoming->second.initToks = dynamic_cast<SDF::delayn<float>*>(it->second)->ns;
                // remove the delay element
                processes.erase(it);
            }
        }
        // Merge unzip processes with their previous actor and zip
        // processes with their next actor.
        // TODO : Cover other unzips
        for (auto it=processes.begin();it!=processes.end();it++)
        {
            if (!strcmp(it->second->ForSyDe_kind().c_str(), "SDF::unzip"))
            {
                // find the incoming and outgoing channels
                decltype(channels.begin()) zipChan;
                std::vector<decltype(channels.begin())> unzipChans;
                for (auto it2=channels.begin();it2!=channels.end();it2++)
                {
                    if (it2->second.desActor == it->second)
                        zipChan = it2;
                    if (it2->second.srcActor == it->second)
                        unzipChans.push_back(it2);
                }
                // connect the outputs of unzip to the original actor
                unzipChans[0]->second.srcActor = zipChan->second.srcActor;
                //unzipChans[0]->second.srcPort = zipChan->second.srcActor;
                unzipChans[1]->second.srcActor = zipChan->second.srcActor;
                // remove the zipped channel
                channels.erase(zipChan);
                // remove the unzip process
                processes.erase(it);
            }
            else if (!strcmp(it->second->ForSyDe_kind().c_str(), "SDF::unzipN"))
            {
                // find the incoming and outgoing channels
                decltype(channels.begin()) zipChan;
                std::vector<decltype(channels.begin())> unzipChans;
                for (auto it2=channels.begin();it2!=channels.end();it2++)
                {
                    if (it2->second.desActor == it->second)
                        zipChan = it2;
                    if (it2->second.srcActor == it->second)
                        unzipChans.push_back(it2);
                }
                // connect the outputs of unzip to the original actor
                for (auto it3=unzipChans.begin();it3!=unzipChans.end();it3++)
                    (*it3)->second.srcActor = zipChan->second.srcActor;
                // remove the zipped channel
                channels.erase(zipChan);
                // remove the unzip process
                processes.erase(it);
            }
            else if (!strcmp(it->second->ForSyDe_kind().c_str(), "SDF::zipN"))
            {
                // find the incoming and outgoing channels
                decltype(channels.begin()) zipChan;
                std::vector<decltype(channels.begin())> unzipChans;
                for (auto it2=channels.begin();it2!=channels.end();it2++)
                {
                    if (it2->second.desActor == it->second)
                        unzipChans.push_back(it2);
                    if (it2->second.srcActor == it->second)
                        zipChan = it2;
                }
                // connect the inputs of zip to the original actor
                for (auto it3=unzipChans.begin();it3!=unzipChans.end();it3++)
                    (*it3)->second.desActor = zipChan->second.desActor;
                // remove the zipped channel
                channels.erase(zipChan);
                // remove the zip process
                processes.erase(it);
            }
        }
        
        // Third, write the actors and channels based on the information
        // from the previous stage
        for (auto it=processes.begin();it!=processes.end();it++)
        {
            // Add it to the XML document
            xml_node<> *top_node = doc.allocate_node(node_element, "actor");
            csdfGraphNode->append_node(top_node);
            xml_attribute<> *actorNameAttr = doc.allocate_attribute("name", (it->first).c_str());
            top_node->append_attribute(actorNameAttr);
            xml_attribute<> *actorSizeAttr = doc.allocate_attribute("size", "1");
            top_node->append_attribute(actorSizeAttr);
            
            // look into all channels to find ports of the current process
            // TODO: improve this by using a better data structure for
            //       system graph representation.
            std::vector<sc_object*> visited;
            for (auto it2=channels.begin();it2!=channels.end();it2++)
            {
                // Add the input ports to XML
                if (it2->second.desActor == it->second)
                {
                    // If it is not already added
                    if (std::find(visited.begin(),visited.end(),it2->second.desPort)!=visited.end()) continue;
                    xml_node<> *node = doc.allocate_node(node_element, "port");
                    top_node->append_node(node);
                    xml_attribute<> *portNameAttr = doc.allocate_attribute("name", it2->second.desPort->name());
                    node->append_attribute(portNameAttr);
                    xml_attribute<> *portTypeAttr = doc.allocate_attribute("type", "in");
                    node->append_attribute(portTypeAttr);
                    std::stringstream sizeS;
                    sizeS << it2->second.consRate;
                    char *sizeStr = doc.allocate_string(sizeS.str().c_str());
                    xml_attribute<> *portSizeAttr = doc.allocate_attribute("rate", sizeStr);
                    node->append_attribute(portSizeAttr);
                    visited.push_back(it2->second.desPort); // mark it as visited
                }
                // Add the output ports to XML
                if (it2->second.srcActor == it->second)
                {
                    // If it is not already added
                    if (std::find(visited.begin(),visited.end(),it2->second.srcPort)!=visited.end()) continue;
                    xml_node<> *node = doc.allocate_node(node_element, "port");
                    top_node->append_node(node);
                    xml_attribute<> *portNameAttr = doc.allocate_attribute("name", it2->second.srcPort->name());
                    node->append_attribute(portNameAttr);
                    xml_attribute<> *portTypeAttr = doc.allocate_attribute("type", "out");
                    node->append_attribute(portTypeAttr);
                    std::stringstream sizeS;
                    sizeS << it2->second.prodRate;
                    char *sizeStr = doc.allocate_string(sizeS.str().c_str());
                    xml_attribute<> *portSizeAttr = doc.allocate_attribute("rate", sizeStr);
                    node->append_attribute(portSizeAttr);
                    visited.push_back(it2->second.srcPort); // mark it as visited
                }
            }
        }
        
        for (auto it=channels.begin();it!=channels.end();it++)
        {
            xml_node<> *node = doc.allocate_node(node_element, "channel");
            csdfGraphNode->append_node(node);
            xml_attribute<> *chanNameAttr = doc.allocate_attribute("name", (it->first).c_str());
            node->append_attribute(chanNameAttr);
            if (it->second.srcActor)
            {
            xml_attribute<> *srcActorAttr = doc.allocate_attribute("srcActor", it->second.srcActor->name());
            node->append_attribute(srcActorAttr);
            }
            if (it->second.srcPort)
            {
            xml_attribute<> *srcPortAttr = doc.allocate_attribute("srcPort", it->second.srcPort->name());
            node->append_attribute(srcPortAttr);
            }
            if (it->second.desActor)
            {
            xml_attribute<> *desActorAttr = doc.allocate_attribute("dstActor", it->second.desActor->name());
            node->append_attribute(desActorAttr);
            }
            if (it->second.desPort)
            {
            xml_attribute<> *desPortAttr = doc.allocate_attribute("dstPort", it->second.desPort->name());
            node->append_attribute(desPortAttr);
            }
            if (it->second.initToks != 0)
            {
            std::stringstream sizeS;
            sizeS << it->second.initToks;
            char *sizeStr = doc.allocate_string(sizeS.str().c_str());
            xml_attribute<> *initToksAttr = doc.allocate_attribute("initialTokens", sizeStr);
            node->append_attribute(initToksAttr);
            }
        }
        
        // The csdfProperties node and nodes below it
        xml_node<> *csdfPropsNode = doc.allocate_node(node_element, "csdfProperties");
        appNode->append_node(csdfPropsNode);
        
            xml_node<> *unitsNode = doc.allocate_node(node_element, "units");
            csdfPropsNode->append_node(unitsNode);
            
            xml_node<> *timeUnitNode = doc.allocate_node(node_element, "timeUnit", "1 ns");
            unitsNode->append_node(timeUnitNode);
            
            xml_node<> *dataUnitNode = doc.allocate_node(node_element, "dataUnit", "1 B");
            unitsNode->append_node(dataUnitNode);
            
            // Actor Properties
            for (auto it=processes.begin();it!=processes.end();it++)
            {
                xml_node<> *node = doc.allocate_node(node_element, "actorProperties");
                csdfPropsNode->append_node(node);
                xml_attribute<> *actorNameAttr = doc.allocate_attribute("actor", (it->first).c_str());
                node->append_attribute(actorNameAttr);
                
                    xml_node<> *processorNode = doc.allocate_node(node_element, "processor");
                    node->append_node(processorNode);
                    xml_attribute<> *processorTypeAttr = doc.allocate_attribute("type", "microblaze");
                    processorNode->append_attribute(processorTypeAttr);
                    xml_attribute<> *processorDefaultAttr = doc.allocate_attribute("default", "true");
                    processorNode->append_attribute(processorDefaultAttr);
                    
                        xml_node<> *defaultPropsNode = doc.allocate_node(node_element, "defaultProperties");
                        processorNode->append_node(defaultPropsNode);
                        
                            xml_node<> *memNode = doc.allocate_node(node_element, "memory");
                            defaultPropsNode->append_node(memNode);
                        
                            xml_node<> *implNode = doc.allocate_node(node_element, "implementation");
                            defaultPropsNode->append_node(implNode);
                            
                                xml_node<> *funcNode = doc.allocate_node(node_element, "function");
                                implNode->append_node(funcNode);
                                char* ts = doc.allocate_string(getFuncName(it->first).c_str());
                                xml_attribute<> *fsymbolAttr = doc.allocate_attribute("symbol", ts);
                                funcNode->append_attribute(fsymbolAttr);
                                
                                    xml_node<> *argMapNode = doc.allocate_node(node_element, "argumentMapping");
                                    funcNode->append_node(argMapNode);
                                    
                                    // add argument mapping for the inputs
                                    int fifoIndex=0;
                                    for (auto it2=channels.begin();it2!=channels.end();it2++)
                                    {
                                        if (it2->second.desActor==it->second)
                                        {
                                            xml_node<> *argMapElmNode = doc.allocate_node(node_element, "argument");
                                            argMapNode->append_node(argMapElmNode);
                                            std::stringstream tss; tss << fifoIndex++;
                                            char* num = doc.allocate_string(tss.str().c_str());
                                            xml_attribute<> *argMapNumAttr = doc.allocate_attribute("number", num);
                                            argMapElmNode->append_attribute(argMapNumAttr);
                                            xml_attribute<> *argMapPortAttr = doc.allocate_attribute("port", it2->second.desPort->name());
                                            argMapElmNode->append_attribute(argMapPortAttr);
                                        }
                                    }
                                    
                                    // add argument mapping for the inputs
                                    for (auto it2=channels.begin();it2!=channels.end();it2++)
                                    {
                                        if (it2->second.srcActor==it->second)
                                        {
                                            xml_node<> *argMapElmNode = doc.allocate_node(node_element, "argument");
                                            argMapNode->append_node(argMapElmNode);
                                            std::stringstream tss; tss << fifoIndex++;
                                            char* num = doc.allocate_string(tss.str().c_str());
                                            xml_attribute<> *argMapNumAttr = doc.allocate_attribute("number", num);
                                            argMapElmNode->append_attribute(argMapNumAttr);
                                            xml_attribute<> *argMapPortAttr = doc.allocate_attribute("port", it2->second.srcPort->name());
                                            argMapElmNode->append_attribute(argMapPortAttr);
                                        }
                                    }
                                
                                xml_node<> *srcFilesNode = doc.allocate_node(node_element, "sourceFiles");
                                implNode->append_node(srcFilesNode);
                                
                                    xml_node<> *fileNode = doc.allocate_node(node_element, "file");
                                    srcFilesNode->append_node(fileNode);
                                    char* ts2 = doc.allocate_string(getFuncName(it->first).append(".cpp").c_str());
                                    xml_attribute<> *fNameAttr = doc.allocate_attribute("file", ts2);
                                    fileNode->append_attribute(fNameAttr);
                    
                    xml_node<> *phasePropsNode = doc.allocate_node(node_element, "phaseProperties");
                    processorNode->append_node(phasePropsNode);
            }
            
            // Channel Properties
            for (auto it=channels.begin();it!=channels.end();it++)
            {
                xml_node<> *node = doc.allocate_node(node_element, "channelProperties");
                csdfPropsNode->append_node(node);
                xml_attribute<> *chanNameAttr = doc.allocate_attribute("channel", (it->first).c_str());
                node->append_attribute(chanNameAttr);
                
                xml_node<> *childNode = doc.allocate_node(node_element, "tokenSize");
                node->append_node(childNode);
                std::stringstream sizeS;
                sizeS << it->second.tokenSize;
                char *sizeStr = doc.allocate_string(sizeS.str().c_str());
                xml_attribute<> *tokSizeAttr = doc.allocate_attribute("size", sizeStr);
                childNode->append_attribute(tokSizeAttr);
            }
            
            // Graph Properties
            xml_node<> *graphPropsNode = doc.allocate_node(node_element, "graphProperties");
            csdfPropsNode->append_node(graphPropsNode);
            
                xml_node<> *throughputNode = doc.allocate_node(node_element, "throughput", "0.000000001");
                graphPropsNode->append_node(throughputNode);
    }
    
    void addProcess(sc_object* p, xml_node<>* top_node)
    {
        // Add it to the data structure
        processes[p->name()] = dynamic_cast<SDF::process*>(p);
    }
    
    void recurseNode(sc_object* obj, xml_node<>* top_node)
    {
        // Add a process to the hierarchy
        if (isModule(obj))
        {
            // Get the list of module children (ports and other modules)
            std::vector<sc_object*> children = obj->get_child_objects();
            // Add it to the XML if it is a leaf ForSyDe process
            if (!isComposite(children))
            {
                addProcess(obj, top_node);
                
                // scan for ports to populate channel information
                auto boundInChans = dynamic_cast<SDF::process*>(obj)->boundInChans;
                for (auto it=boundInChans.begin();it!=boundInChans.end();it++)
                    addInputPort(*it);
            
                auto boundOutChans = dynamic_cast<SDF::process*>(obj)->boundOutChans;
                for (auto it=boundOutChans.begin();it!=boundOutChans.end();it++)
                    addOutputPort(*it);
            }
            else
                // Recursion step: for composite processes we destroy the
                // hierarchy by passing the top XML node down to the next
                // recursion step and ommitting the port detection
                for ( unsigned i = 0; i < children.size(); i++ )
                    recurseNode(children[i], top_node);
        }
        else if (isChannel(obj))
        {
            channels[obj->name()].tokenSize = 
                dynamic_cast<SDF::SDF2SDF_Size*>(obj)->tokenSize();
        }
    }
    
    void addOutputPort(const SDF::PortInfo& portInfo)
    {
        for (auto it=portInfo.boundChans.begin();it!=portInfo.boundChans.end();it++)
        {
            std::string chanName = (*it) -> name();
            channels[chanName].srcActor = portInfo.port->get_parent_object();
            channels[chanName].srcPort = portInfo.port;
            channels[chanName].prodRate = portInfo.toks;
        }
    }
    
    void addInputPort(const SDF::PortInfo& portInfo)
    {
        for (auto it=portInfo.boundChans.begin();it!=portInfo.boundChans.end();it++)
        {
            std::string chanName = (*it) -> name();
            channels[chanName].desActor = portInfo.port->get_parent_object();
            channels[chanName].desPort = portInfo.port;
            channels[chanName].consRate = portInfo.toks;
        }
    }
    
    //! The printXML method writes the XML file to the output.
    /*! The XML structure is already generated, so this command only
     * checks for the availability of the output file and dumps the XML
     * to it.
     */
    void printXML(std::string fileName)
    {
        std::ofstream outFile(fileName);
        if (!outFile.is_open())
            SC_REPORT_ERROR(fileName.c_str(), "file could not be opened");
        outFile << "<!-- Automatically generated by ForSyDe -->" << std::endl;
        outFile << doc;
    }
    
    //! The printSrc method extracts and writes the pure functions
    /*! It looks for all .hpp files in the provided path, and extracts
     * all of the code sections enclosed by "#pragma ForSyDe begin process.fname"
     * and "#pragma ForSyDe end process.fname" and generates separate
     * "process.fname.cpp" files for each pair.
     */
    void printSrc(const char* projectPath, const char* exportPath)
    {
        struct dirent *entry;
        DIR *dp;
        std::string line;
        
        dp = opendir(projectPath);
        if (dp == NULL) std::cout << "Invalid path for source lookup." << std::endl;
        while((entry = readdir(dp)))
        {
            std::string curFile(entry->d_name); // Get the file name
            // Has .hpp extention?
            size_t pos = curFile.rfind(".hpp");
            if (pos == curFile.npos) continue;
            
            // Open the file for reading
            std::ifstream iFile(curFile);
            if (!iFile.is_open())
            {
                std::cout << "Can not open file: "<< curFile << std::endl;
                continue;
            }
            // check for ForSyDe pragmas
            while (iFile.good())
            {
                getline(iFile,line);
                size_t pos = line.find("#pragma ForSyDe begin ");
                if (pos == line.npos) continue;
                // Now we have found a match
                // Get the function name
                std::string funName(
                    line.substr(
                        std::string("#pragma ForSyDe begin ").length()
                    )
                );
                
                std::ofstream oFile(exportPath+funName+".c");
                if (!oFile.is_open())
                {
                    std::cout << "Can not create file: "<< funName << ".c" << std::endl;
                    continue;
                }
                oFile << "/* Automatically extracted by ForSyDe */" << std::endl;
                // write the function signature
                oFile << "void "<< funName <<"(int task_id, void *** data_in, void *** data_out, int csdf_activation){" << std::endl;
                // write input/output adaptation interface
                //// Deduce the process name that uses this function
                std::string processName =
                    funName.substr(0,funName.rfind("_func"));
                //// write the adaptation code for input and output
                ////// find a matching process
                for (auto it=processes.begin();it!=processes.end();it++)
                {
                    if (getFuncName(it->first)==funName)
                    {
                        for (unsigned i=0;i<it->second->itoks.size();i++)
                            oFile << "    " << "void" << "** inp" << i+1
                                  << " = data_in[" 
                                  << i << "];" << std::endl;
                        for (unsigned i=0;i<it->second->otoks.size();i++)
                            oFile << "    " << "void" << "** out" << i+1
                                  << " = data_out["
                                  << i << "];" << std::endl << std::endl;
                        break;
                    }
                }
                // iterate through until the end of section
                while (iFile.good() && oFile.good())
                {
                    getline(iFile,line);
                    pos = line.find("#pragma ForSyDe end");
                    if (pos != line.npos)
                    {
                        oFile << "}" << std::endl;
                        oFile.close();
                        break;
                    }
                    // pointerize input variable names
                    size_t lookstart = 0;
                    while (1)
                    {
                        size_t posi = line.find("inp", lookstart);
                        if (posi != line.npos)
                        {
                            line.insert(posi, "*");
                            lookstart = posi+4;
                        }
                        else
                            break;
                    }
                    //pointerize output variable names
                    lookstart = 0;
                    while (1)
                    {
                        size_t posi = line.find("out", lookstart);
                        if (posi != line.npos)
                        {
                            line.insert(posi, "*");
                            lookstart = posi+4;
                        }
                        else
                            break;
                    }
                    oFile << line << std::endl;
                }
            }
            
            iFile.close();
        }
            
        closedir(dp);
    }
    
    //! returns the expected function name containing the actor functionality based on convention
    /*! 
     */
    inline std::string getFuncName(const std::string& actorName)
    {
        size_t pos = actorName.rfind('.');
        std::string fname = pos==actorName.npos ? actorName : 
                                                  actorName.substr(pos+1);
        pos = fname.find_last_not_of("0123456789");
        fname = (pos==fname.npos) ? fname : fname.substr(0,pos+1);
        return fname.append("_func");
    }
    
    //! Checks if the SystemC object under question has the sc_module kind
    /*! 
     */
    inline bool isModule(sc_object* obj)
    {
        return !strcmp(obj->kind(), "sc_module");
    }
    
    //! Checks if the SystemC object under question has the sc_fifo kind
    /*! 
     */
    inline bool isChannel(sc_object* obj)
    {
        return !strcmp(obj->kind(), "sc_fifo");
    }
    
    bool isComposite(const std::vector<sc_object*>& children)
    {
        bool isComposite = false;
        // Check if it is a composite module
        for ( unsigned i = 0; i < children.size(); i++ )
            if (isModule(children[i]))
                isComposite = true;
        return isComposite;
    }

    //! The output XML Document Object Model instance
    xml_document<> doc;
    //! The associative map cotainer gathering channel information 
    std::map<std::string,ChannelInfo> channels;
    //! The associative map cotainer gathering process information
    std::map<std::string,SDF::process*> processes;

};

}

#endif
