#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

/**********************************************************************           
    * token.hpp -- Containers for tokens in UT MoCs                   *
    *                                                                 *
    * Author:  George Ungureanu (ugeorge@kth.se)                      *
    *                                                                 *
    * Purpose: Hides vector containers used for implementing multiple *
    *          produced/consumed tokens, used in some MoCs (e.g. SDF) *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


/*! \file token.hpp
 * \brief Implements the token containers
 */

namespace ForSyDe
{

template<typename T>
using tokens = std::vector<T>;

template<typename... T>
struct token_tuple {
	typedef std::tuple<tokens<T>...> type;
	type t;

    token_tuple() {}
    token_tuple(std::initializer_list<size_t> list) { traverse_tuple<sizeof...(T), type>::resize(list.end(), t); }
	explicit token_tuple(const std::tuple<tokens<T>...> t_) : t(t_) {};
	token_tuple(const token_tuple<T...> & t_) : t(t_.t){}

    void resize(std::initializer_list<size_t> list) { traverse_tuple<sizeof...(T), type>::resize(list.end(), t); }
	token_tuple<T...>& operator=(const token_tuple<T...>& rhs) { t = rhs.t; return *this;}
	token_tuple<T...>& operator=(const std::tuple<tokens<T>...>& rhs) { t = rhs; return *this;}

private:

	// Default template for traversing a tuple
	template <size_t N, typename Tup>
	struct traverse_tuple {};

	// Template specialization for traversing a tuple with multiple elements
	template <size_t N, typename TT, typename... Tail>
	struct traverse_tuple<N, std::tuple<tokens<TT>, Tail...>>{
		static inline void resize(std::initializer_list<size_t>::iterator size, std::tuple<tokens<TT>, Tail...>& tv) {
			--size;
			std::get<N-1>(tv).resize(*size);
			traverse_tuple<N-1, std::tuple<tokens<TT>, Tail...>>::resize(size, tv);
		}
	};

	// Template specialization for traversing a tuple with one element
	template <typename TT, typename... Tail>
	struct traverse_tuple<1, std::tuple<tokens<TT>, Tail...>>{
		static inline void resize(std::initializer_list<size_t>::iterator size, std::tuple<tokens<TT>, Tail...>& tv) {
			std::get<0>(tv).resize(*size);
		}
	};
};

template<typename... T>
std::ostream& operator <<(std::ostream &os, const token_tuple<T...> &obj) {
	return os;
}


// TODO: make an initializer traversal
template<typename T>
tokens<T> init(size_t n){
	return tokens<T>(n);
}

template<typename... T>
tokens<token_tuple<T...>> init(size_t n, std::initializer_list<size_t> list){
	return tokens<token_tuple<T...>>(n, token_tuple<T...>(list));
}

}

#endif
