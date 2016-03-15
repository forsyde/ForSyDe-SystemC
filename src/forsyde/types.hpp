/**********************************************************************           
    * types.hpp -- provides a simple type reflection mechanism.       *
    *                                                                 *
    * Author:  Hosein Attarzadeh (shan2@kth.se) based on:             *
    * http://stackoverflow.com/questions/1055452/c-get-name-of-type-in-template *
    *                                                                 *
    * Purpose: Provide facilities to store the type names, used in    *
    *          introspection.                                         *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License:                                                        *
    *******************************************************************/

#ifndef TYPES_HPP
#define TYPES_HPP

#include <iostream>

/*! \file types.hpp
 * \brief Provides facilities for basic type introspection
 * 
 *  This file includes a a set of basic facilities for registering names
 * for non-POD C/C++ types to be reflected in the XML output of the
 * interospection stage.
 */

//~ namespace ForSyDe
//~ {

// The general case uses RTTI (if the type is not registered explicitly)
#pragma once
template<typename T> const char* get_type_name() {return typeid(T).name();}


template <typename T>
using Vector = std::vector<T>;

template <typename... T>
using Tuple = std::tuple<T...>;

// Specialization for each type
#define DEFINE_TYPE(X) \
    template<>const char* get_type_name<X>(){return #X;}
// Another version where we explicitly provide the type name (for complex types)
#define DEFINE_TYPE_NAME(X,N) \
    template<>const char* get_type_name<X>(){return N;}

// Specialization for base types

DEFINE_TYPE(char);
DEFINE_TYPE(short int);
DEFINE_TYPE(unsigned short int);
DEFINE_TYPE(int);
DEFINE_TYPE(unsigned int);
DEFINE_TYPE(long int);
DEFINE_TYPE(unsigned long int);
DEFINE_TYPE(long long int);
DEFINE_TYPE(unsigned long long int);
DEFINE_TYPE(bool);
DEFINE_TYPE(float);
DEFINE_TYPE(double);
DEFINE_TYPE(long double);
DEFINE_TYPE(wchar_t);


template<typename T> std::string get_type_size() {return std::to_string(sizeof(T));}


struct type_info {
	std::string name = "";
	std::string size = "";

	void surround(const std::string& before, const std::string& after) {
		name = before + name + after;
		size = before + size + after;
	}

	void concat(type_info& rhs) {
		name += rhs.name;
		size += rhs.size;
	}
};

class SmartTypes {
public:
	template <typename T>
	struct base_type {
		static inline void get(type_info& info) {
			std::cout<< "base nothing" << std::endl;
			info.name += get_type_name<T>();
			info.size += get_type_size<T>();
		}
	};


	template <typename T>
	struct base_type<Vector<T>> {
		static inline void get(type_info& info) {
			std::cout<< "base vector" << std::endl;
			type_context<T>::get(info);
			info.size += ":" + std::to_string(sizeof(T));
		}
	};

	template <typename... T>
	struct base_type<Tuple<T...>> {
		static inline void get(type_info& info) {
			std::cout<< "base tuple" << std::endl;
			type_context<Tuple<T...>>::get(info);
		}
	};

	template <size_t N, typename Tup>
	struct traverse_tuple {};

	template <size_t N, typename Head, typename... Tail>
	struct traverse_tuple<N, Tuple<Head, Tail...>>{
		static inline void get(type_info& info) {
			std::cout<< "traversal" << std::endl;
			type_context<Head>::get(info);
			info.surround("", ",");
			traverse_tuple<N-1, Tuple<Tail...>>::get(info);
		}
	};

	template <typename Head, typename... Tail>
	struct traverse_tuple<1, Tuple<Head, Tail...>> {
		static inline void get(type_info& info) {
			std::cout<< "traversal end" << std::endl;
			type_context<Head>::get(info);
		}
	};


	template <typename T>
	struct type_context {
		static inline void get(type_info& info) {
			std::cout<< "context nothing" << std::endl;
			base_type<T>::get(info);
		}
	};


	template <typename T>
	struct type_context<Vector<T>> {
		static inline void get(type_info& info) {
			std::cout<< "context vector" << std::endl;
			type_info new_vector;
			base_type<Vector<T>>::get(new_vector);
			std::cout<< "end vector" << std::endl;
			new_vector.surround("[","]");
			info.concat(new_vector);
		}
	};

	template <typename... T>
	struct type_context<Tuple<T...>> {
		static inline void get(type_info& info) {
			std::cout<< "context tuple" << std::endl;
			type_info new_tuple;
			traverse_tuple<sizeof...(T), Tuple<T...>>::get(new_tuple);
			new_tuple.surround("(",")");
			info.concat(new_tuple);

		}
	};

	template <typename T>
	static inline type_info get_smart_type_info() {
		type_info info;
		type_context<T>::get(info);
		return info;
	};
};

template<typename X>
const char* get_smart_type_name() {
	std::string str = SmartTypes::get_smart_type_info<X>().name;
	char * writable = new char[str.size() + 1];
	std::copy(str.begin(), str.end(), writable);
	writable[str.size()] = '\0';
	return writable;
}

//~ }

#endif
