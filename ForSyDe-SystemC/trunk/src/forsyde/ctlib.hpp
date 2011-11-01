/**********************************************************************           
    * ctlib.hpp -- a library of useful processes in the CT MoC        *
    *                                                                 *
    * Authors:  Hosien Attarzadeh (shan2@kth.se)                      *
    *           Gilmar Besera (gilmar@kth.se)                         *
    *                                                                 *
    * Purpose: Enriching the CT library.                              *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef CTLIB_HPP
#define CTLIB_HPP

#include "ctmoc.hpp"

namespace ForSyDe
{
    
namespace CT
{
    
 //! Process constructor for a continuous-time process which scales the input
/*! This class is used to build continuous-time processes with one input
 * and one output. By passing a constant value to the constructor, the
 * process scales the inputs using it.
 */   
class scale : public comb
{
public:
    scale(sc_module_name _name,     ///< Process name
          CTTYPE scft               ///< Scaling factor
          ) : comb(_name), factor(scft) {}
private:
    CTTYPE factor;
    CTTYPE _func(CTTYPE a)
    {
        return factor * a;
    }
};

 //! Process constructor for a continuous-time process which adds the inputs
/*! This class is used to build continuous-time processes with two inputs
 * and one output. It adds the input signals together to produce the output.
 */
class add : public comb2
{
public:
    add(sc_module_name _name        ///< Process name
        ) : comb2(_name) {}
private:
    CTTYPE _func(CTTYPE a, CTTYPE b)
    {
        return a + b;
    }
};

 //! Process constructor for a continuous-time process which subtracts the inputs
/*! This class is used to build continuous-time processes with two inputs
 * and one output. It subtracts the second input from the first one to
 * produce the output.
 */
class sub : public comb2
{
public:
    sub(sc_module_name _name        ///< Process name
        ) : comb2(_name) {}
private:
    CTTYPE _func(CTTYPE a, CTTYPE b)
    {
        return a - b;
    }
};

 //! Process constructor for a continuous-time process which multiplies the inputs
/*! This class is used to build continuous-time processes with two inputs
 * and one output. It multiplies the input signals together to produce
 * the output.
 */
class mult : public comb2
{
public:
    mult(sc_module_name _name        ///< Process name
         ) : comb2(_name) {}
private:
    CTTYPE _func(CTTYPE a, CTTYPE b)
    {
        return a * b;
    }
};

 //! Process constructor for a sinusoid
/*! This class is used to cretae a continuous-time signal source which 
 * produces a sine wave.
 */
class sine : public source
{
public:
    sine(sc_module_name name_,      ///< The Process name
         const sc_time& endT,       ///< The end time of the generated signal
         const sc_time& period,     ///< The signal period (1/f)
         const CTTYPE& ampl         ///< The signal amplitude
         ) : source(name_, sc_time(0, SC_SEC), endT,[=](sc_time t){
                                    return ampl*sin(2*M_PI*t/period);
                                                                  }) {}
};

 //! Process constructor for a cosine wave
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
           ) : source(name_, sc_time(0, SC_SEC), endT,[=](sc_time t){
                                      return ampl*cos(2*M_PI*t/period);
                                                                    }) {}
};

//! Process constructor for a square-wave generator
/*! This class is used to cretae a continuous-time signal source which 
 * produces a square-wave with adjustable parameters.
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
          ) : source(name_, sc_time(0, SC_SEC), endT,[=](sc_time t){
                        double tmp = (t/period);
                        return tmp-(long)tmp < dutyCycle ? highS : lowS;
                                                                  }) {}
};

//~ //! Process constructor for a Gaussian randome wave generator
//~ /*! This class is used to cretae a continuous-time signal source which 
 //~ * produces a Random signal based on the Gaussian distribution
 //~ */
//~ class gaussian : public source
//~ {
//~ public:
    //~ gaussian(sc_module_name name_,      ///< The Process name
             //~ //const sc_time& period,     ///< The signal period
             //~ const double& gaussVar,    ///< The variance
             //~ const double& gaussMean,   ///< The mean value
             //~ const sc_time& endT        ///< The end time of the generated signal
            //~ ) : source(name_, sc_time(0, SC_SEC), endT,[=](sc_time t){
                    //~ double rnd1,rnd2,G,Q,Q1,Q2;
                     //~ 
                    //~ while(1) {
                      //~ do { 
                            //~ rnd1 = ((double)my_rand()) / ((double)2147483647) ;
                            //~ rnd2 = ((double)my_rand()) / ((double)2147483647) ;
                    //~ 
                            //~ Q1 = 2.0 * rnd1 - 1.0 ;
                            //~ Q2 = 2.0 * rnd2 - 1.0 ;
                    //~ 
                            //~ Q = Q1 * Q1 + Q2 * Q2 ;
                    //~ 
                        //~ } while (Q > 1.0) ;
//~ 
                        //~ G = gaussMean+sqrt(gaussVar)*(sqrt(-2.0*log(Q)/Q)*Q1) ;
                        //~ return G;
                    //~ }
                                                                  //~ }) {}
//~ private:
    //~ // state variable:  
    //~ bool shiftreg[64];	// boolean array for the LFSR random number generator
    //~ void init()
    //~ {
        //~ long int seed = 11206341;
        //~ for(int i=63; i>=0; i--) {	// the LFSR shiftregister is initialized with 0
          //~ if(seed>=pow(2.,(double)i)) {
        //~ shiftreg[i]=true;
        //~ seed-=(long int)pow(2.,(double)i);
          //~ }
          //~ else shiftreg[i]=false;
        //~ } 
    //~ }  
//~ 
  //~ int my_rand() { 
    //~ bool zw = (shiftreg[59]==shiftreg[60]==shiftreg[62]==shiftreg[63]); // computing feedback
    //~ for(int i=63; i>0; i--) {
      //~ shiftreg[i] = shiftreg[i-1];	// shifting
    //~ }
    //~ shiftreg[0] = zw; 			// writing the feedback bit
    //~ double val = 0.;
    //~ for(int i=0; i<31; i++) {
      //~ if(shiftreg[2*i]) val+=pow(2.,(double)i); // extracting random number
    //~ }
    //~ return((int)floor(val)); 
  //~ }
//~ };

}
}
#endif
