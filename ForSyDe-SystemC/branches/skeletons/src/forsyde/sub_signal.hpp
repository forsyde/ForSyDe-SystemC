/**********************************************************************           
    * sub_signal.hpp -- Data type for representing a function over an *
    *                   interval.                                     *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Represents a sb-component of a CT signal.              *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SUB_SIGNAL_HPP
#define SUB_SIGNAL_HPP

/*! \file sub_signal.hpp
 * \brief Implements the sub-components of a CT signal
 */

namespace ForSyDe
{

using namespace sc_core;

//! Type of the values used in the CT MoC (currently fixed)
typedef double CTTYPE;

//! The sub-signal type used to construct a CT signal
/*! This class is used to build a sub-signal which is a function that is
 * valid on a range. A consecutive stream of tokens of type sub_signal
 * forms a CT signal.
 * 
 * The range is defined by a start time and and end time of type sc_time.
 * The supplied fuction can be a function pointer, a function object or
 * a C++11 lambda function.
 */
class sub_signal
{
public:
    
    typedef std::function<CTTYPE(const sc_time&)> functype;
    
    //! The constructor used for sub-signal definition
    /*! 
     */
    sub_signal(const sc_time& st,         ///< Beginning of the range
               const sc_time& et,         ///< End of the range
               const functype& f) ///< The function over the range
        : start_time(st), end_time(et), _f(f) {}
    
    //! A dummy constructor used for sub-signal definition without initialization
    /*! 
     */
    sub_signal() {}
    
    //! The copy constructor
    /*! 
     */
    sub_signal(const sub_signal& ss       ///< Object to be copied
              )
        : start_time(get_start_time(ss)), end_time(get_end_time(ss)), _f(get_function(ss)) {}
    
    //! The assignment operator
    /*! 
     */
    sub_signal& operator=(const sub_signal& ss  ///< Object to be assigned
                        ){
        start_time = get_start_time(ss);
        end_time = get_end_time(ss);
        _f = get_function(ss);
        return *this;
    }
    
    //! The overloaded () operator makes the sub-signal a function object
    /*! It allows to sample the signal with a convinient syntax.
     * Additionally, it checks the sampling time validity with respect
     * to the range.
     */
    CTTYPE operator() (const sc_time& valAt) const
    {
        if ((valAt>=start_time) && (valAt<end_time))
            return _f(valAt);
        else
        {
            SC_REPORT_ERROR("Using ForSyDe::CT","Access out of sub-signal range");
            return -1;
        }
    }
    
    //! A helper function used to get the beginning of the range
    /*! 
     */
    inline friend sc_time get_start_time(const sub_signal& ss)
    {
        return ss.start_time;
    }
    
    //! A helper function used to get the end of the range
    /*! 
     */
    inline friend sc_time get_end_time(const sub_signal& ss)
    {
        return ss.end_time;
    }
    
    //! A helper function used to get the functions in range
    /*! 
     */
    inline friend std::function<CTTYPE(const sc_time&)> get_function(const sub_signal& ss)
    {
        return ss._f;
    }

    //! A helper function used to set the start and end of the range
    /*! 
     */
    inline friend void set_range(sub_signal& ss, sc_time st, sc_time et)
    {
        ss.start_time = st;
        ss.end_time = et;
    }
    
    //! A helper function used to set the function in the range
    /*! 
     */
    inline friend void set_function(sub_signal& ss, const std::function<CTTYPE(const sc_time&)>& f)
    {
        ss._f = f;
    }
    
    friend std::ostream& operator<< (std::ostream& os, sub_signal &subSig)
    {
        os << "(" << get_start_time(subSig) << ", " 
           << get_end_time(subSig) << ") -> f";
        return os;
    }
private:
    sc_time start_time;
    sc_time end_time;
    functype _f;
};

}
#endif
