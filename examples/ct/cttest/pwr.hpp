/**********************************************************************           
    * pwr.hpp -- a process that computes square of a signal           *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a simple program.                     *
    *                                                                 *
    * Usage:   Toy CT example                                         *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#include <forsyde.hpp>

using namespace ForSyDe::CT;

class pwr : public comb
{
public:
    pwr(sc_module_name _name) : comb(_name) {}
private:
    CTTYPE _func(CTTYPE a)
    {
        return a * a;
    }
};
