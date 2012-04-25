/**********************************************************************           
    * abst_ext.hpp -- Absent-extended values data-type                *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Extend normal values with their absent-extended        *
    *          version, required by some MoCs (e.g., synhronous)      *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef ABST_EXT_HPP
#define ABST_EXT_HPP

/*! \file abst_ext.h
 * \brief Implements the Absent-extended values
 */

namespace ForSyDe
{

using namespace sc_core;

//! Absent-extended data types
/*! This template class extends a type T to its absent-extended version.
 * Values of this type could be either absent, or present with a specific
 * value.
 */
template <typename T>
class abst_ext
{
public:
    //! The constructor with a present value
    abst_ext(const T& val) : present(true), value(val) {}
    
    //! The constructor with an absent value
    abst_ext() : present(false) {}
    
    //! Converts a value from an extended value, returning a default value if absent
    T from_abst_ext (const T& defval) const
    {
        if (present) return value; else return defval;
    }
    
    //! Unsafely converts a value from an extended value assuming it is present
    T unsafe_from_abst_ext () const {return value;}
    
    //! Sets absent
    void set_abst() {present=false;}
    
    //! Sets the value
    void set_val(const T& val) {present=true;value=val;}
    
    //! Checks for the absence of a value
    bool is_absent() const {return !present;}
    
    //! Checks for the presence of a value
    bool is_present() const {return present;}
    
    //! Checks for the equivalence of two absent-extended values
    bool operator== (const abst_ext& rs) const
    {
        if (is_absent() || rs.is_absent())
            return is_absent() && rs.is_absent();
        else
            return unsafe_from_abst_ext() == rs.unsafe_from_abst_ext();
    }
    
    //! Overload the streaming operator to enable SystemC communiation
    friend std::ostream& operator<< (std::ostream& os, const abst_ext &abst_ext)
    {
        if (abst_ext.is_present())
            os << abst_ext.unsafe_from_abst_ext();
        else
            os << "_";
        return os;
    }
private:
    bool present;
    T value;
};

}
#endif
