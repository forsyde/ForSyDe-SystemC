/**********************************************************************
    * dis.hpp -- domain interfaces for the SystemC map of the ForSyDe *
    *          library                                                *
    *                                                                 *
    * Authors: Gilmar Besera (gilmar@kth.se)                          *
    *          Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Providing domain interfaces for ForSyDe-SystemC        *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DIS_HPP
#define DIS_HPP

/*! \file di.h
 * \brief Implements the domain interfaces between different MoCs
 * 
 *  This file includes the basic process constructors and other
 * facilities used for creating domain interfaces between different MoCs.
 */

namespace ForSyDe
{
using namespace sc_core;

// Auxilliary Macro definitions
#define WRITE_MULTIPORT(PORT,VAL) \
    for (int i=0;i<PORT.size();i++) PORT[i]->write(VAL);

//! Operation modes for the SY2CT converter
enum SY2CTMode {LINEAR, HOLD};

//! Process constructor for a SY2CT domain interfaces
/*! This class is used to build a domain interfaces which converts an SY 
 * signal to a CT one. It can be used to implement digital-to-analog
 * converters. There are two operating modes which can be configured using
 * the initial values of the constructor:
 * - sample and hold
 * - linear interpolation
 */
template<class T> 
SC_MODULE(SY2CT)
{
	sc_fifo_in<T> iport;                ///< port for the input channel
	sc_fifo_out<CT::SubSignal> oport;   ///< port for the output channel

	SC_HAS_PROCESS(SY2CT);

	SY2CT(sc_module_name name_,         ///< The module name
          sc_time sampleT,              ///< The sampling time
          SY2CTMode oMode = HOLD///< The operation mode
          ) : sc_module(name_), sampT(sampleT), mode(oMode)
	{
		SC_THREAD(worker);
	}

	void worker() 
	{
		CTTYPE previousVal, currentVal;
		sc_time sampT2(sampT);
		
		for(int i=0; ;i++)
		{

			previousVal = currentVal;

			currentVal = (CTTYPE) iport.read();

			CT::SubSignal subSig;
			subSig.setRange(sampT*i, sampT*(i+1));
            
			if(mode==HOLD)
			{
				subSig.setF([=](sc_time t){
						return currentVal;
						});
			}
			else 
			{
				CTTYPE v = currentVal - previousVal;

				subSig.setF([=](sc_time t)->CTTYPE{
                        return (v * (t-i*sampT2)/sampT2 + previousVal);
				});
			}
			WRITE_MULTIPORT(oport,subSig);
		}
	}

 private:
	sc_time sampT;
	SY2CTMode mode;
	
};

//! Process constructor for a CT2SY domain interface
/*! This class is used to build a domain interface which converts an CT 
 * signal to a SY one with fixed sampling rate. It can be used to implement 
 * analog-to-digital converters.
 */
template<class T> 
SC_MODULE(CT2SY)
{
    sc_fifo_in<CT::SubSignal> iport;    ///< port for the input channel
    sc_fifo_out<T> oport;               ///< port for the output channel

    SC_HAS_PROCESS(CT2SY);

    CT2SY(sc_module_name _name,         ///< The module name
          sc_time sampleT               ///< The sampling time
          ) : sc_module(_name), sampT(sampleT)
    {
        SC_THREAD(worker);
    }

    void worker() 
    {
        CT::SubSignal in_val;
        in_val = iport.read();
        sc_time curTime(in_val.getStartT());
        while (1)
        {
            while (curTime > in_val.getEndT()) in_val = iport.read();
            WRITE_MULTIPORT(oport,in_val(curTime));// run the function
            curTime += sampT;
        }
    }

private:
    sc_time sampT;
};


}

#endif
