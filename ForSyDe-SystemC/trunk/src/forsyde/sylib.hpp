/**********************************************************************           
    * sylib.hpp -- a library of useful processes in the SY MoC        *
    *                                                                 *
    * Authors:  Hosien Attarzadeh (shan2@kth.se)                      *
    *                                                                 *
    * Purpose: Enriching the SY library.                              *
    *                                                                 *
    * Usage:                                                          *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SYLIB_H
#define SYLIB_H

#include "symoc.h"

namespace ForSyDe
{
    
namespace SY
{

//! Process constructor for a Gaussian randome wave generator
/*! This class is used to cretae a synchronous signal source which 
 * produces a Random signal based on the Gaussian distribution
 */
class gaussian : public source<double>
{
public:
    gaussian(sc_module_name name_,      ///< The Process name
             const double& gaussVar,    ///< The variance
             const double& gaussMean    ///< The mean value
            ) : source(name_, 0), gaussVar(gaussVar), gaussMean(gaussMean) {}
protected:
    double _func(double inp)
    {
        double rnd1,rnd2,G,Q,Q1,Q2;
        do
        { 
            rnd1 = ((double)my_rand()) / ((double)2147483647) ;
            rnd2 = ((double)my_rand()) / ((double)2147483647) ;

            Q1 = 2.0 * rnd1 - 1.0 ;
            Q2 = 2.0 * rnd2 - 1.0 ;

            Q = Q1 * Q1 + Q2 * Q2 ;
    
        } while (Q > 1.0) ;

        G = gaussMean+sqrt(gaussVar)*(sqrt(-2.0*log(Q)/Q)*Q1) ;
        return G;
    }
private:
    double gaussVar, gaussMean;
    // state variable:  
    bool shiftreg[64];	// boolean array for the LFSR random number generator
    void init()
    {
        long int seed = 11206341;
        for(int i=63; i>=0; i--) {	// the LFSR shiftregister is initialized with 0
          if(seed>=pow(2.,(double)i)) {
        shiftreg[i]=true;
        seed-=(long int)pow(2.,(double)i);
          }
          else shiftreg[i]=false;
        } 
    }  

  int my_rand() { 
    bool zw = (shiftreg[59]==shiftreg[60]==shiftreg[62]==shiftreg[63]); // computing feedback
    for(int i=63; i>0; i--) {
      shiftreg[i] = shiftreg[i-1];	// shifting
    }
    shiftreg[0] = zw; 			// writing the feedback bit
    double val = 0.;
    for(int i=0; i<31; i++) {
      if(shiftreg[2*i]) val+=pow(2.,(double)i); // extracting random number
    }
    return((int)floor(val)); 
  }
};

}
}
#endif
