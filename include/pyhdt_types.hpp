/**
 * hdt_types.hpp
 * Author: Thomas MINIER - MIT License 2017-2018
 */

#ifndef PYHDT_TYPES_HPP
#define PYHDT_TYPES_HPP

#include <list>
#include <string>
#include <tuple>

// A RDF Triple. RDF terms are represented as simple strings by HDT.
typedef std::tuple<std::string, std::string, std::string> triple;

// A RDF triple composed of IDs from HDT dictionnary
typedef std::tuple<unsigned int, unsigned int, unsigned int> triple_id;

// A list of RDF triples
typedef std::list<triple> triple_list;

// A list of RDF triples IDs
typedef std::list<triple_id> triple_ids_list;

// A hint over the cardinality of a triple pattern
// The right element of the tuple is True if the hint is accurate, False otherwise
typedef std::tuple<size_t, bool> size_hint;

#endif /* PYHDT_TYPES_HPP */
