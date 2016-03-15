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
 #include <sstream>
#include "rapidxml_print.hpp"


using namespace rapidxml;

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
template<typename T> const char* get_type_name() {
	return typeid(T).name();
}


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


class IntrospectiveType {
public:
	IntrospectiveType(xml_document<>* xml_doc, xml_node<>* root) : xml_doc(xml_doc), root(root) {
        const_name = (char*)"name";
        const_data_type = (char*)"data_type";
		const_primitive = (char*)"primitive";
		const_vector = (char*)"vector";
		const_tuple = (char*)"tuple";
        const_size = (char*)"size";
        const_length = (char*)"length";
	}
    ~IntrospectiveType(){}

public:
	template <typename T>
	inline void traverse() {
		// Get the list of module children (ports and other processes)

		for (auto child = root->first_node(); child; child = child->next_sibling()) {
			if (child->first_attribute(const_name)->value() == get_type_name<T>() ) return;
		}

		xml_node<> *type_node = allocate_append_node(root, const_data_type);
		allocate_append_attribute(type_node, const_data_type, get_type_name<T>());

		add_type_node<T>::get(type_node);
	};

	template <typename T>
	struct add_type_node {
		inline void get (xml_node<>* parent) {
			xml_node<> *primitive_node = allocate_append_node(parent, const_primitive);
			allocate_append_attribute(primitive_node, const_name, get_type_name<T>());
			allocate_append_attribute(primitive_node, const_size, size_to_char(sizeof(T)));
		}
	};

	template <typename T>
	struct add_type_node<Vector<T>> {
		inline void  get (xml_node<>* parent) {
			xml_node<> *vector_node = allocate_append_node(parent, const_vector);
			add_type_node<T>::get(vector_node);

			size_t child_size = char_to_size(vector_node->first_node()->first_attribute(const_size)->value());
			allocate_append_attribute(vector_node, const_length, size_to_char(sizeof(T)/child_size));
			allocate_append_attribute(vector_node, const_size, size_to_char(sizeof(T)));
		}
	};

	template <typename... T>
	struct add_type_node<Tuple<T...>> {
		inline void get (xml_node<>* parent) {
			xml_node<> *tuple_node = allocate_append_node(parent, const_tuple);
			traverse_tuple<sizeof...(T), Tuple<T...>>(tuple_node);
			allocate_append_attribute(tuple_node, const_size, size_to_char(sizeof(T)));
		}
	};

	template <size_t N, typename Tup>
	struct traverse_tuple {};

	template <size_t N, typename Head, typename... Tail>
	struct traverse_tuple<N, Tuple<Head, Tail...>>{
		inline void get (xml_node<>* parent) {
			add_type_node<Head>(parent);
			traverse_tuple<N-1, Tuple<Tail...>>(parent);
		}
	};

	template <typename Head, typename... Tail>
	struct traverse_tuple<1, Tuple<Head, Tail...>> {
		inline void get (xml_node<>* parent) {
			add_type_node<Head>(parent);
		}
	};

    //! The RapidXML DOM
	xml_document<>* xml_doc;
	xml_node<>* root;

    //! Some global constant names
	char *const_name, *const_data_type, *const_primitive, *const_vector, *const_tuple, *const_size, *const_length;

    inline xml_node<>* allocate_append_node(xml_node<>* top, const char* name)
    {
        xml_node<>* node = xml_doc.allocate_node(node_element, name);
        top->append_node(node);
        return node;
    }

    inline void allocate_append_attribute(xml_node<>* node, const char* attr_name, const char* attr_val)
    {
        xml_attribute<>* attr = xml_doc.allocate_attribute(attr_name, attr_val);
        node->append_attribute(attr);
    }

    inline const char* size_to_char(size_t size){
    	std::string str = std::to_string(size);
    	char * writable = new char[str.size() + 1];
    	std::copy(str.begin(), str.end(), writable);
    	writable[str.size()] = '\0';
    	return writable;
    }

    inline size_t char_to_size(const char* char_size){
    	std::istringstream iss(char_size);
    	size_t size;
    	iss > size;
    	return size;
    }
};

//~ }

#endif
