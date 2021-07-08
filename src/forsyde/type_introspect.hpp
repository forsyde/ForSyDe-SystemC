/**********************************************************************           
    * types.hpp -- provides a type reflection mechanism.              *
    *                                                                 *
    * Authors: Hosein Attarzadeh (shan2@kth.se)                       *
    *          George Ungureanu (ugeorge@kth.se)                      *
    *                                                                 *
    * based on: http://stackoverflow.com/questions/1055452/c-get-name-of-type-in-template *
    *                                                                 *
    * Purpose: Provide facilities to store the type information, used *
    *          in introspection.                                      *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License:                                                        *
    *******************************************************************/

#ifndef FORSYDE_TYPE_INTROSPECT_HPP
#define FORSYDE_TYPE_INTROSPECT_HPP

/*! \file types.hpp
 * \brief Provides facilities for type introspection
 * 
 *  This file includes
 *    - a set of basic facilities for registering names or non-POD
 *      C/C++ types to be reflected in the XML output of the
 *      introspection stage.
 *    - a set of basic facilities for declaring and registering custom
 *      types, such as struct
 *    - a set of methods for recursive parsing and introspection of
 *      template data types.
 *    - a singleton container for storing all type structures found
 *      during the introspection phase
 */


#include <iostream>
#include <sstream>
#include <tuple>

#include "abst_ext.hpp"
#include "token.hpp"
#include "rapidxml_print.hpp"

using namespace rapidxml;

// The general case uses RTTI (if the type is not registered explicitly)
#pragma once
template<typename T> const char* get_type_name() {
	return typeid(T).name();
}

// helper macros
#define FIRST_ARG___(N, ...) N
#define FARG_NAME___(N, ...) #N
#define REST_ARGS___(N, ...) __VA_ARGS__
#define FIRST_ARG__(args) FIRST_ARG___ args
#define FARG_NAME__(args) FARG_NAME___ args
#define REST_ARGS__(args) REST_ARGS___ args


// Specialization for each type
#define DEFINE_TYPE(...) \
    template<>const char* get_type_name<__VA_ARGS__>(){return #__VA_ARGS__;}

#define DEFINE_TYPE_NAME(...) \
	template<>const char* get_type_name<REST_ARGS__((__VA_ARGS__))>(){return FIRST_ARG__((__VA_ARGS__);}


// Defines also the streaming operator for user defined types. Must be followed by a function definition. {}
#define DEFINE_TYPE_STREAM(...) \
		DEFINE_TYPE(__VA_ARGS__); \
		void getCustomTypeDefinition(std::ostream &os, const __VA_ARGS__ &obj); \
		std::ostream& operator <<(std::ostream &os, const __VA_ARGS__ &obj) \
		{ \
			getCustomTypeDefinition(os, obj); \
			return os;\
		} \
		void getCustomTypeDefinition(std::ostream &os, const __VA_ARGS__ &obj)


#define TYPEDEF(...)\
    typedef REST_ARGS__((__VA_ARGS__)) FIRST_ARG__((__VA_ARGS__));\
    DEFINE_TYPE_STREAM(FIRST_ARG__((__VA_ARGS__)))


// Helper macro for creating structures. Must be followed by a definition between {}
#define STRUCT(...)\
	typedef struct FIRST_ARG__((__VA_ARGS__)) { \
			REST_ARGS__((__VA_ARGS__)) \
	} FIRST_ARG__((__VA_ARGS__)); \
	DEFINE_TYPE_STREAM(FIRST_ARG__((__VA_ARGS__)))


// Helper macro for creating unions. Must be followed by a definition between {}
#define UNION(...)\
	typedef union FIRST_ARG__((__VA_ARGS__)) { \
			REST_ARGS__((__VA_ARGS__)) \
	} FIRST_ARG__((__VA_ARGS__)); \
	DEFINE_TYPE_STREAM(FIRST_ARG__((__VA_ARGS__)))



// Primitive type name definition
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

namespace ForSyDe
{

#ifdef FORSYDE_TYPE_INTROSPECTION

// Identifiers for primitive types
template<class T> struct isPrimitive { enum { val = 0 }; };
template<> struct isPrimitive<char>                   { enum { val = 1 }; };
template<> struct isPrimitive<short int>              { enum { val = 1 }; };
template<> struct isPrimitive<unsigned short int>     { enum { val = 1 }; };
template<> struct isPrimitive<int>                    { enum { val = 1 }; };
template<> struct isPrimitive<unsigned int>           { enum { val = 1 }; };
template<> struct isPrimitive<long int>               { enum { val = 1 }; };
template<> struct isPrimitive<unsigned long int>      { enum { val = 1 }; };
template<> struct isPrimitive<long long int>          { enum { val = 1 }; };
template<> struct isPrimitive<unsigned long long int> { enum { val = 1 }; };
template<> struct isPrimitive<bool>                   { enum { val = 1 }; };
template<> struct isPrimitive<float>                  { enum { val = 1 }; };
template<> struct isPrimitive<double>                 { enum { val = 1 }; };
template<> struct isPrimitive<long double>            { enum { val = 1 }; };
template<> struct isPrimitive<wchar_t>                { enum { val = 1 }; };

// xml label names
static char* const_name = (char*)"name";
static char* const_data_type = (char*)"type";
static char* const_primitive = (char*)"primitive";
static char* const_custom = (char*)"custom";
static char* const_vector = (char*)"vector";
static char* const_array = (char*)"array";
static char* const_tuple = (char*)"tuple";
static char* const_size = (char*)"size";
static char* const_length = (char*)"length";
static char* const_root_type = (char*)"forsyde_types";


//! Singleton container for storing type structures during introspection
/*! This class is used to store type structures during the introspection phase
 *  as XML DOM document. It identifies types from the ForSyDe-IR by their
 *  name given by the RTTI and stores them only once.
 */
class TypeContainer {
public:
    //! The instance getter.
	static TypeContainer& get() {
		static TypeContainer instance;
		return instance;
	}

    //! Returns the root node of the XML DOM
	xml_node<>* root() { return root_node; }

    //! Allocates and appends a node to the XML documents, and returns its pointer.
    xml_node<>* add_node(xml_node<>* parent, const char* name) {
        xml_node<>* node = xml_doc.allocate_node(node_element, name);
        parent->append_node(node);
        return node;
    }

    //! Allocates and appends an attribute to an XML node, and returns its pointer.
    void add_attribute(xml_node<>* node, const char* attr_name, const char* attr_val) {
        xml_attribute<>* attr = xml_doc.allocate_attribute(attr_name, attr_val);
        node->append_attribute(attr);
    }

    //! The print method writes the XML file to the output.
    /*! The XML structure is already generated, so this command only
     * checks for the availability of the output file and dumps the XML
     * to it.
     */
    void printXML(std::string fileName)
    {
        std::ofstream outFile(fileName);
        if (!outFile.is_open())
            SC_REPORT_ERROR(fileName.c_str(), "file could not be opened to write the introspection output. Does the path exists?");
        outFile << "<?xml version=\"1.0\" ?>" << std::endl;
        outFile << "<!-- Automatically generated by ForSyDe -->" << std::endl;
        outFile << "<!DOCTYPE forsyde_types SYSTEM \"forsyde_types.dtd\" >"  << std::endl;
        outFile << xml_doc;

    }

private:
    //! The (hidden) default constructor.
	TypeContainer() {
		root_node = xml_doc.allocate_node(node_element, const_root_type);
		xml_doc.append_node(root_node);
	};

	~TypeContainer() {
		xml_doc.clear();
	};

	// these are needed to ensure singleton pattern
	TypeContainer(TypeContainer const&);
	void operator=(TypeContainer const&);

    //! The RapidXML DOM
	xml_document<> xml_doc;

	//! The root node
	xml_node<>* root_node;

};

#endif // FORSYDE_TYPE_INTROSPECTION

//! Class containing methods for recursive type introspection
/*! This class is used as a namespace for all template functions used for
 *  recursive type introspection. During the traversal of a type, its structure
 *  is being stored in a TypeContainer singleton.
 */
class IntrospectiveType {
public:
	//! Main (and only) function for parsing a data type
	/*! This function is the only one called during the introspection phase to
	 *  traverse a type.
	 *
	 *  If the FORSYDE_TYPE_INTROSPECTION compiler flag is enabled then it
	 *  updates the TypeContainer with the XML structure extracted. Otherwise
	 *  it just returns its identifier (RTTI or given name).
	 */
	template <typename T>
	static inline const char* traverse() {
		const char* type_name = get_type_name<T>();

#ifdef FORSYDE_TYPE_INTROSPECTION
		xml_node<>* root = TypeContainer::get().root();
		for (auto child = root->first_node(); child; child = child->next_sibling()) {
			if (child->first_attribute(const_name)->value() == type_name ) return type_name;
		}

		xml_node<> *type_node = TypeContainer::get().add_node(root, const_data_type);
		TypeContainer::get().add_attribute(type_node, const_name, type_name);
		add_type_node<T>::get(type_node);
#endif
		return type_name;
	};

#ifdef FORSYDE_TYPE_INTROSPECTION

	// Default template for getting the information of a node, in case no other structure is identified
	template <typename T>
	struct add_type_node {
		static inline void get (xml_node<>* parent) {
			char* node_type = (isPrimitive<T>::val) ? const_primitive : const_custom;
			xml_node<> *primitive_node = TypeContainer::get().add_node(parent, node_type);
			TypeContainer::get().add_attribute(primitive_node, const_name, get_type_name<T>());
			TypeContainer::get().add_attribute(primitive_node, const_size, size_to_char(sizeof(T)));
		}
	};

	// Template specialization for parsing vectors
	template <typename T>
	struct add_type_node<std::vector<T>> {
		static inline void  get (xml_node<>* parent) {
			xml_node<> *vector_node = TypeContainer::get().add_node(parent, const_vector);

			add_type_node<T>::get(vector_node);
		}
	};

	// Template specialization for parsing fixed size (std::)arrays
	template <typename T, size_t S>
	struct add_type_node<std::array<T,S>> {
		static inline void  get (xml_node<>* parent) {
			xml_node<> *vector_node = TypeContainer::get().add_node(parent, const_array);
			TypeContainer::get().add_attribute(vector_node, const_length, size_to_char(S));

			add_type_node<T>::get(vector_node);
		}
	};

	// Template specialization for parsing tuples
	template <typename... T>
	struct add_type_node<std::tuple<T...>> {
		static inline void get (xml_node<>* parent) {
			xml_node<> *tuple_node = TypeContainer::get().add_node(parent, const_tuple);
			TypeContainer::get().add_attribute(tuple_node, const_length, size_to_char(sizeof...(T)));
			traverse_tuple<sizeof...(T), std::tuple<T...>>::get(tuple_node);
		}
	};

	// Template specialization for parsing tuples
	template <typename... T>
	struct add_type_node<token_tuple<T...>> {
		static inline void get (xml_node<>* parent) {
			xml_node<> *tuple_node = TypeContainer::get().add_node(parent, const_tuple);
			TypeContainer::get().add_attribute(tuple_node, const_length, size_to_char(sizeof...(T)));
			traverse_tuple<sizeof...(T), token_tuple<T...>>::get(tuple_node);
		}
	};

	// Default template for traversing a tuple
	template <size_t N, typename Tup>
	struct traverse_tuple {};

	// Template specialization for traversing a tuple with multiple elements
	template <size_t N, typename Head, typename... Tail>
	struct traverse_tuple<N, std::tuple<Head, Tail...>>{
		static inline void get (xml_node<>* parent) {
			add_type_node<Head>::get(parent);
			traverse_tuple<N-1, std::tuple<Tail...>>::get(parent);
		}
	};

	// Template specialization for traversing a tuple with one element
	template <typename Head, typename... Tail>
	struct traverse_tuple<1, std::tuple<Head, Tail...>> {
		static inline void get (xml_node<>* parent) {
			add_type_node<Head>::get(parent);
		}
	};

	// Template specialization for traversing a tuple with multiple elements
	template <size_t N, typename Head, typename... Tail>
	struct traverse_tuple<N, token_tuple<Head, Tail...>>{
		static inline void get (xml_node<>* parent) {
			add_type_node<Head>::get(parent);
			traverse_tuple<N-1, token_tuple<Tail...>>::get(parent);
		}
	};

	// Template specialization for traversing a tuple with one element
	template <typename Head, typename... Tail>
	struct traverse_tuple<1, token_tuple<Head, Tail...>> {
		static inline void get (xml_node<>* parent) {
			add_type_node<Head>::get(parent);
		}
	};



	// Utility function : size_t to char array
    static inline const char* size_to_char(size_t size){
    	std::string str = std::to_string(size);
    	char * writable = new char[str.size() + 1];
    	std::copy(str.begin(), str.end(), writable);
    	writable[str.size()] = '\0';
    	return writable;
    }

	// Utility function : char array size_t
    static inline size_t char_to_size(const char* char_size){
    	std::istringstream iss(char_size);
    	size_t size;
    	iss >> size;
    	return size;
    }

#endif  // FORSYDE_TYPE_INTROSPECTION
};

}

#endif
