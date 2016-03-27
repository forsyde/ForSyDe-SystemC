/**********************************************************************           
    * type_helpers.hpp -- provides functions for accessing C types    *
    *                                                                 *
    * Author: George Ungureanu (ugeorge@kth.se)                       *
    *                                                                 *
    * Purpose: Provide facilities to access C types inside ForSyDe    *
    *          signal structures.                                     *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License:                                                        *
    *******************************************************************/

#ifndef FORSYDE_TYPE_HELPERS_HPP
#define FORSYDE_TYPE_HELPERS_HPP

#include <tuple>
#include "abst_ext.hpp"
#include "token.hpp"

/*! \file types.hpp
 *  \brief Provides facilities for addressing C types inside complex containers.
 */

namespace ForSyDe
{


//! Class containing methods for returning references to C types inside channel structures
/*! This class is used as a namespace for the getter helpers which return a
 *  reference to a C type inside a channel structure implemented with template 
 *  classes such as STL types, by addressing them using an index pack.
 */
class TypeGetter {
	template<size_t N, typename T, size_t... Ixs> struct base;
	template<size_t N, typename T, size_t... Ixs>
	struct traverse {
		static inline const void get (T&) {
			throw std::out_of_range( "Too many indexes for the input type!" );
		}
	};

	// Once the index pack has been consumed, return a reference to this C
	//primitive type
	template <typename T> struct base_t { typedef T type; };
	template <typename T> struct base_val {
		static inline const T& get (const T& t) { return t; }
		static inline       T& get (T& t)       { return t; }
	};

	// Once the index pack has been consumed, return a reference to the C array
	// associated with this std::vector
	template <typename T> struct base_t<std::vector<T>> { typedef T type; };
	template <typename T> struct base_val<std::vector<T>> {
		static inline const T& get (const std::vector<T>& t) { return t[0]; }
		static inline       T& get (std::vector<T>& t)       { return t[0]; }
	};

	// Once the index pack has been consumed, return a reference to the C array
	// associated with this std::array
	template <typename T, size_t S> struct base_t<std::array<T,S>> { typedef T type; };
	template <typename T, size_t S> struct base_val<std::array<T,S>> {
		static inline const T& get (const std::array<T,S>& t) { return t.data()[0]; }
		static inline       T& get (std::array<T,S>& t)       { return t.data()[0]; }
	};

	// This is where the index pack is consumed. Once this point has been reached
    // get the base C type addressed by the index pack
	template<typename T, size_t... Ixs>
	struct base<0, T, Ixs...> { typedef typename base_t<T>::type type; };
	template<typename T, size_t... Ixs>
	struct traverse<0, T, Ixs...> {
		static inline const typename base_t<T>::type& get (const T& t) { return base_val<T>::get(t); }
		static inline typename       base_t<T>::type& get (T& t)       { return base_val<T>::get(t); }
	};

	// traversal through a vector
	template<size_t N, typename T, size_t Ix, size_t... Ixs>
	struct base<N, std::vector<T>, Ix, Ixs...> { typedef typename base<N-1, T, Ixs...>::type type; };
	template<size_t N, typename T, size_t Ix, size_t... Ixs>
	struct traverse<N, std::vector<T>, Ix, Ixs...> {
		static inline const typename base<N, std::vector<T>, Ix, Ixs...>::type& get (const std::vector<T>& t) {
			return traverse<N-1, T, Ixs...>::get(t.at(Ix));
		}
		static inline typename       base<N, std::vector<T>, Ix, Ixs...>::type& get (std::vector<T>& t) {
			return traverse<N-1, T, Ixs...>::get(t.at(Ix));
		}
	};

	// traversal through an array
	template<size_t N, typename T, size_t S, size_t Ix, size_t... Ixs>
	struct base<N, std::array<T,S>, Ix, Ixs...> { typedef typename base<N-1, T, Ixs...>::type type; };
	template<size_t N, typename T, size_t S, size_t Ix, size_t... Ixs>
	struct traverse<N, std::array<T,S>, Ix, Ixs...> {
		static inline const typename base<N, std::array<T,S>, Ix, Ixs...>::type& get (const std::array<T,S>& t) {
			return traverse<N-1, T, Ixs...>::get(t.at(Ix));
		}
		static inline typename      base<N, std::array<T,S>, Ix, Ixs...>::type& get (std::array<T,S>& t) {
			return traverse<N-1, T, Ixs...>::get(t.at(Ix));
		}
	};

	// traversal through a tuple
	template<size_t N, typename... T, size_t Ix, size_t... Ixs>
	struct base<N, std::tuple<T...>, Ix, Ixs...> {
		typedef typename base<N-1, typename std::tuple_element<Ix, std::tuple<T...> >::type, Ixs...>::type type;
	};
	template<size_t N, typename... T, size_t Ix, size_t... Ixs>
	struct traverse<N, std::tuple<T...>, Ix, Ixs...> {
		static inline const typename base<N, std::tuple<T...>, Ix, Ixs...>::type& get (const std::tuple<T...>& t) {
			return traverse<N-1, typename std::tuple_element<Ix, std::tuple<T...> >::type, Ixs...> ::get(std::get<Ix>(t));
		}
		static inline typename       base<N, std::tuple<T...>, Ix, Ixs...>::type& get (std::tuple<T...>& t) {
			return traverse<N-1, typename std::tuple_element<Ix, std::tuple<T...> >::type, Ixs...> ::get(std::get<Ix>(t));
		}
	};

	// traversal through a token_tuple
	template<size_t N, typename... T, size_t Ix, size_t... Ixs>
	struct base<N, token_tuple<T...>, Ix, Ixs...> {
		typedef typename base<N-1, typename std::tuple_element<Ix, std::tuple<std::vector<T>...> >::type, Ixs...>::type type;
	};
	template<size_t N, typename... T, size_t Ix, size_t... Ixs>
	struct traverse<N, token_tuple<T...>, Ix, Ixs...> {
		static inline const typename base<N, token_tuple<T...>, Ix, Ixs...>::type& get (const token_tuple<T...>& t) {
			return traverse<N-1, typename std::tuple_element<Ix, std::tuple<std::vector<T>...> >::type, Ixs...> ::get(std::get<Ix>(t.t));
		}
		static inline typename       base<N, token_tuple<T...>, Ix, Ixs...>::type& get (token_tuple<T...>& t) {
			return traverse<N-1, typename std::tuple_element<Ix, std::tuple<std::vector<T>...> >::type, Ixs...> ::get(std::get<Ix>(t.t));
		}
	};
};


/*! Get a reference to the first-level element in a complex template structure 
\code{.cpp}
std::vector<int> a = {1,2,3,4};
int b = ForSyDe::get<1>(a);    // b = 2
\endcode
 */
template<size_t I1, typename Ti>
inline typename TypeGetter::base<1, Ti, I1>::type&
get(Ti& v){ return TypeGetter::traverse<1, Ti, I1>::get(v); }

/*! Get a reference to the second-level element in a complex template structure 
\code{.cpp}
std::tuple<std::vector<int>,char> a = std::make_tuple(std::vector({1,2,3,4,5}), 'b');
int b = ForSyDe::get<0,2>(a);    // b = 3
\endcode
 */
template<size_t I1, size_t I2, typename Ti>
inline typename TypeGetter::base<2, Ti, I1, I2>::type&
get(Ti& v){ return TypeGetter::traverse<2, Ti, I1, I2>::get(v); }


/*! Get a reference to a third-level element in a complex template structure 
\code{.cpp}
std::tuple<std::vector<int>,char> a1 = std::make_tuple(std::vector({1,2,3}), 'a1');
std::tuple<std::vector<int>,char> a2 = std::make_tuple(std::vector({4,5,6}), 'a2');
std::vector<std::tuple<std::vector<int>,char>> a = {a1, a2};
int b = ForSyDe::get<1,0,2>(a)    // b = 6
\endcode
 */
template<size_t I1, size_t I2, size_t I3, typename Ti>
inline typename TypeGetter::base<3, Ti, I1, I2, I3>::type&
get(Ti& v){ return TypeGetter::traverse<3, Ti, I1, I2, I3>::get(v); }

/*! Get a reference to a fourth-level element in a complex template structure 
 */
template<size_t I1, size_t I2, size_t I3, size_t I4, typename Ti>
inline typename TypeGetter::base<4, Ti, I1, I2, I3, I4>::type&
get(Ti& v){ return TypeGetter::traverse<4, Ti, I1, I2, I3, I4>::get(v); }

/*! Get a reference to a fifth-level element in a complex template structure 
 */
template<size_t I1, size_t I2, size_t I3, size_t I4, size_t I5, typename Ti>
inline typename TypeGetter::base<5, Ti, I1, I2, I3, I4, I5>::type&
get(Ti& v){ return TypeGetter::traverse<5, Ti, I1, I2, I3, I4, I5>::get(v); }




template<size_t I1, typename Ti>
inline const typename TypeGetter::base<1, Ti, I1>::type&
get(const Ti& v){ return TypeGetter::traverse<1, Ti, I1>::get(v); }

template<size_t I1, size_t I2, typename Ti>
inline const typename TypeGetter::base<2, Ti, I1, I2>::type&
get(const Ti& v){ return TypeGetter::traverse<2, Ti, I1, I2>::get(v); }

template<size_t I1, size_t I2, size_t I3, typename Ti>
inline const typename TypeGetter::base<3, Ti, I1, I2, I3>::type&
get(const Ti& v){ return TypeGetter::traverse<3, Ti, I1, I2, I3>::get(v); }

template<size_t I1, size_t I2, size_t I3, size_t I4, typename Ti>
inline const typename TypeGetter::base<4, Ti, I1, I2, I3, I4>::type&
get(const Ti& v){ return TypeGetter::traverse<4, Ti, I1, I2, I3, I4>::get(v); }

template<size_t I1, size_t I2, size_t I3, size_t I4, size_t I5, typename Ti>
inline const typename TypeGetter::base<5, Ti, I1, I2, I3, I4, I5>::type&
get(const Ti& v){ return TypeGetter::traverse<5, Ti, I1, I2, I3, I4, I5>::get(v); }


}

#endif

