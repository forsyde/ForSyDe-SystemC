/**********************************************************************
    * sdf_types.hpp -- Specific types for the SDF MoC                 *
    *                                                                 *
    * Author:  George Ungureanu (ugeorge@kth.se)                      *
    *                                                                 *
    * Purpose: Providing promitive element required for modeling      *
    *          synchronous dataflow systems in ForSyDe-SystemC        *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef SDF_TYPES_HPP
#define SDF_TYPES_HPP

/*! \file sdf_moc.hpp
 * \brief Implements the Synchronous Dataflow (SDF) Model of Computation
 *
 *  This file includes the basic process constructors and other
 * facilities used for modeling in the SDF model of computation.
 */

#include <functional>
#include <tuple>
#include <initializer_list>


namespace ForSyDe {

namespace SDF {


template<typename T>
using tokens = std::vector<T>;


template<typename... T>
struct token_tuple {
	typedef std::tuple<tokens<T>...> type;

	type t;

    token_tuple() {}

    token_tuple(std::initializer_list<size_t> list) {
    	traverse_tuple<sizeof...(T), type>::resize(list.end(), t);
    }

    void resize(std::initializer_list<size_t> list) {
    	traverse_tuple<sizeof...(T), type>::resize(list.end(), t);
    }

    template< std::size_t I, std::size_t N >
    auto get() { return std::get<I>(t)[N]; }

    template< std::size_t I>
    auto get(size_t n) { return std::get<I>(t)[n]; }

	explicit token_tuple(const std::tuple<tokens<T>...> t_) : t(t_) {};
	token_tuple(const token_tuple<T...> & t_) : t(t_.t){}
	token_tuple<T...>& operator=(const token_tuple<T...>& rhs) { t = rhs.t; return *this;}
	token_tuple<T...>& operator=(const std::tuple<tokens<T>...>& rhs) { t = rhs; return *this;}

	/*operator const std::tuple<tokens<T>...> & () const {return t; }
	operator std::tuple<tokens<T>...> & () { return t; }
	bool operator==(const token_tuple<T...> & rhs) const { return t == rhs.t; }
	bool operator<(const token_tuple<T...> & rhs) const { return t < rhs.t; }*/

private:

	// Default template for traversing a tuple
	template <size_t N, typename Tup>
	struct traverse_tuple {};

	// Template specialization for traversing a tuple with multiple elements
	template <size_t N, typename TT, typename... Tail>
	struct traverse_tuple<N, std::tuple<tokens<TT>, Tail...>>{
		static inline void resize(std::initializer_list<size_t>::iterator size, std::tuple<tokens<TT>, Tail...> tv) {
			--size;
			std::get<N-1>(tv).resize(*size);
			traverse_tuple<N-1, std::tuple<tokens<TT>, Tail...>>::resize(size, tv);
		}
	};

	// Template specialization for traversing a tuple with one element
	template <typename TT, typename... Tail>
	struct traverse_tuple<1, std::tuple<tokens<TT>, Tail...>>{
		static inline void resize(std::initializer_list<size_t>::iterator size, std::tuple<tokens<TT>, Tail...> tv) {
			std::get<0>(tv).resize(*size);
		}
	};

};

template<typename... T>
std::ostream& operator <<(std::ostream &os, const token_tuple<T...> &obj) {
	return os;
}


template<typename T>
tokens<T> init(size_t n){
	return tokens<T>(n);
}
template<typename T>
void resize(size_t n, tokens<T> v){
	return v.resize(n);
}
template<std::size_t N, typename T>
T& get(tokens<T> v){
	return v.at(N);
}

template<typename... T>
tokens<token_tuple<T...>> init(size_t n, std::initializer_list<size_t> list){
	return tokens<token_tuple<T...>>(n, token_tuple<T...>(list));
}
template<typename... T>
void resize(size_t n, std::initializer_list<size_t> list, tokens<token_tuple<T...>> v ){
	return v.resize(n);
}
template< std::size_t N1, std::size_t N2, std::size_t N3, typename... T >
auto get(tokens<token_tuple<T...>> v) {
	return v[N1].get<N2,N3>();
}
template< std::size_t N1, std::size_t N2, typename... T >
auto get_tokens(tokens<token_tuple<T...>> v) {
	return std::get<N2>(v[N1].t);
}


}
}

#endif


