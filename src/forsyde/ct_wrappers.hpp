/**********************************************************************           
    * ct_wrappers.hpp -- Co-simulation wrappers for the CT MOC        *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *          FMI code adopted from QTronic GmbH's FMI 2.0 SDK       *
    *                                                                 *
    * Purpose: Providing co-simulation wrappers for integration of    *
    *          foreign models into ForSyDe                            *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef CT_WRAPPERS_HPP
#define CT_WRAPPERS_HPP

/*! \file ct_wrappers.hpp
 * \brief Implements a set of co-simulation wrappers in the CT MoC
 * 
 *  This file includes a set of co-simulation wrappers for integration
 * of freign models into the continuous-time model of computation of
 * ForSyDe.
 */

#include "fmi2/fmi2.h"
#include "fmi2/sim_support.h"

#include "sub_signal.hpp"
#include "ct_process.hpp"

namespace ForSyDe
{

namespace CT
{

using namespace sc_core;

//! Process constructor for a co-simulation FMU wrapper with one input and one output
/*! This class is used to build an FMI wrapper with one input and one
 * output. It uses the Functional Mock-up Interface (FMI 2.0) in
 * co-simulation mode to communicate with a Functional Mock-up Unit (FMU)
 * which includes a numeric solver or interfaces to a solver tool.
 */
class fmi2cswrap : public ct_process
{
public:
    CT_in  iport1;       ///< port for the input channel
    CT_out oport1;       ///< port for the output channel

    //! The constructor requires the module name
    /*! It creates an SC_THREAD which reads data from its input port,
     * applies the user-imlpemented function to it and writes the
     * results using the output port
     */
    fmi2cswrap(sc_module_name _name,    ///< process name
         const std::string& fmu_file,   ///< The FMU file name
         const unsigned int& input_index,///< The index of input variable
         const unsigned int& output_index,///< The index of output variable
         const sc_time& sample_period   ///< The fixed sampling period
         ) : ct_process(_name), iport1("iport1"), oport1("oport1"),
             fmuFileName(fmu_file), input_index(input_index),
             output_index(output_index), h(sample_period)
    {
#ifdef FORSYDE_INTROSPECTION
        arg_vec.push_back(std::make_tuple("fmuFileName",fmuFileName));
        std::stringstream ss;
        ss << input_index;
        arg_vec.push_back(std::make_tuple("input_index", ss.str()));
        ss.str("");
        ss << output_index;
        arg_vec.push_back(std::make_tuple("output_index", ss.str()));
        ss.str("");
        ss << sample_period;
        arg_vec.push_back(std::make_tuple("sample_period", ss.str()));
#endif
    }
    
    //! Specifying from which process constructor is the module built
    std::string forsyde_kind() const {return "CT::fmi2cswrap";}

private:
    // Inputs and output variables
    sub_signal oval;
    sub_signal ival1;
    
    //! The function passed to the process constructor
    std::string fmuFileName;
    unsigned int input_index, output_index;
    sc_time h;
    
    sc_time time;
    FMU fmu;                        // the fmu to simulate
    const char *guid;               // global unique id of the fmu
    const char *instanceName;       // instance name
    fmi2Component c;                // instance of the fmu
    fmi2Status fmi2Flag;            // return code of the fmu functions
    char *fmuResourceLocation;      // path to the fmu resources as URL
    fmi2Boolean visible;            // no simulator user interface
    fmi2CallbackFunctions callbacks = {fmuLogger, calloc, free, NULL, &fmu};// called by the model during simulation
    ModelDescription* md;           // handle to the parsed XML file
    fmi2Boolean toleranceDefined;   // true if model description define tolerance
    fmi2Real tolerance;             // used in setting up the experiment
    ValueStatus vs;
    Element *defaultExp;
    
    //Implementing the abstract semantics
    void init()
    {
        time = SC_ZERO_TIME;
        fmuResourceLocation = getTempResourcesLocation();
        visible = fmi2False;
        //~ callbacks = {fmuLogger, calloc, free, NULL, fmu};
        toleranceDefined = fmi2False;
        tolerance = 0;
        //~ vs = 0;
        
        // load the FMU
        loadFMU(fmuFileName.c_str(), &fmu);
        
        // run the simulation
        // instantiate the fmu   
        md = fmu.modelDescription;
        guid = getAttributeValue((Element *)md, att_guid);
        instanceName = getAttributeValue((Element *)getCoSimulation(md),
            att_modelIdentifier);
        c = fmu.instantiate(instanceName, fmi2CoSimulation, guid,
            fmuResourceLocation, &callbacks, visible, fmi2False/*logging off*/);
        free(fmuResourceLocation);
        if (!c) return SC_REPORT_ERROR(name(),"could not instantiate model");
        
        defaultExp = getDefaultExperiment(md);
        if (defaultExp) tolerance = getAttributeDouble(defaultExp, att_tolerance, &vs);
        if (vs == valueDefined) {
            toleranceDefined = fmi2True;
        }
        
        fmi2Flag = fmu.setupExperiment(c, toleranceDefined, tolerance, 0, fmi2True, 1000/* FIXME */);
        if (fmi2Flag > fmi2Warning) {
            return SC_REPORT_ERROR(name(),"could not initialize model; failed FMI setup experiment");
        }
        
        fmi2Flag = fmu.enterInitializationMode(c);
        if (fmi2Flag > fmi2Warning) {
            return SC_REPORT_ERROR(name(),"could not initialize model; failed FMI enter initialization mode");
        }
        
        fmi2Flag = fmu.exitInitializationMode(c);
        if (fmi2Flag > fmi2Warning) {
            return SC_REPORT_ERROR(name(),"could not initialize model; failed FMI exit initialization mode");
        }
        
        // output solution for time t0
        // FIXME: outputRow(fmu, c, tStart, file, separator, fmi2False); // output values
        //~ auto res = getRealOutput(fmu, c, 0);
        //~ oval = sub_signal(time, time+h,
                    //~ [this,res](const sc_time& t)
                    //~ {
                        //~ return res;
                    //~ }
               //~ );
        //~ write_multiport(oport1, oval)
        //~ time += h;
        //~ wait(time - sc_time_stamp());
        ival1 = iport1.read();
    }
    
    void prep()
    {
        while (time >= get_end_time(ival1)) ival1 = iport1.read();
        setRealInput(&fmu, c, input_index, ival1(time));
    }
    
    void exec()
    {
        fmi2Flag = fmu.doStep(c, time.to_seconds(), h.to_seconds(), fmi2True);
        if (fmi2Flag == fmi2Discard) {
            fmi2Boolean b;
            // check if model requests to end simulation
            if (fmi2OK != fmu.getBooleanStatus(c, fmi2Terminated, &b)) {
                return SC_REPORT_ERROR(name(),"could not complete simulation of the model. getBooleanStatus return other than fmi2OK");
            }
            if (b == fmi2True) {
                return SC_REPORT_ERROR(name(),"the model requested to end the simulation");
            }
            return SC_REPORT_ERROR(name(),"could not complete simulation of the model");
        }
        if (fmi2Flag != fmi2OK)
            return SC_REPORT_ERROR(name(),"could not complete simulation of the model");
        
        // FIXME: res = outputRow(fmu, c, time, file, separator, fmi2False); // output values for this step
        auto res = getRealOutput(&fmu, c, output_index);
        oval = sub_signal(time, time+h,
                    [this,res](const sc_time& t)
                    {
                        return res;
                    }
               );
    }
    
    void prod()
    {
        write_multiport(oport1, oval)
        time += h;
        wait(time - sc_time_stamp());
    }
    
    void clean()
    {
        // end simulation
        fmu.terminate(c);
        fmu.freeInstance(c);
    
        #ifdef _MSC_VER
            FreeLibrary(fmu.dllHandle);
        #else
            dlclose(fmu.dllHandle);
        #endif
        freeModelDescription(fmu.modelDescription);
        deleteUnzippedFiles();
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

//! Helper function to construct a pipewrap process
/*! This function is used to construct a pipe wrapper process (SystemC
 * module) and connect its input and output signals.
 * It provides a more functional style definition of a ForSyDe process.
 * It also removes bilerplate code by using type-inference feature of
 * C++ and automatic binding to the input and output FIFOs.
 */
template <class OIf, class I1If>
inline fmi2cswrap* make_fmi2cswrap(const std::string& pName,
    const std::string& fmu_file,
    const unsigned& input_index,
    const unsigned& output_index,
    const sc_time& sample_period,
    OIf& outS,
    I1If& inp1S
    )
{
    auto p = new fmi2cswrap(pName.c_str(), fmu_file, input_index, output_index, sample_period);
    
    (*p).iport1(inp1S);
    (*p).oport1(outS);
    
    return p;
}

}
}

#endif
