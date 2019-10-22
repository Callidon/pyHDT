/**
 * hdt_document.hpp
 * Author: Thomas MINIER - MIT License 2017-2019
 */

#ifndef PYHDT_DOCUMENT_HPP
#define PYHDT_DOCUMENT_HPP

#include <pybind11/pybind11.h>
#include "HDT.hpp"
#include "QueryProcessor.hpp"
#include "pyhdt_types.hpp"
#include "triple_iterator.hpp"
#include "triple_iterator_bytes.hpp"
#include "tripleid_iterator.hpp"
#include "join_iterator.hpp"
#include "join_iterator_bytes.hpp"
#include <list>
#include <string>
#include <vector>
namespace py = pybind11;

// The result of a search for a triple pattern in a HDT document:
// a tuple (matching RDF triples, nb of matching RDF triples)
typedef std::tuple<TripleIterator *, size_t> search_results;

// The result of a search for a triple pattern in a HDT document:
// a tuple (matching RDF triples, nb of matching RDF triples)
typedef std::tuple<TripleIteratorBytes *, size_t> search_results_bytes;

// Same as seach_results, but for an iterator over triple ids
typedef std::tuple<TripleIDIterator *, size_t> search_results_ids;

/*!
 * HDTDocument is the main entry to manage an hdt document
 * \author Thomas Minier
 */
class HDTDocument {
private:
  std::string hdt_file;
  hdt::HDT *hdt;
  hdt::QueryProcessor *processor;
  HDTDocument(std::string file, bool map, bool indexed);

public:
  /*!
   * Destructor
   */
  ~HDTDocument();

  /*!
   * Get the path to the HDT file currently loaded
   * @return The path to the HDT file currently loaded
   */
  std::string getFilePath();

  /*!
   * Implementation for Python function "__repr__"
   * @return A string representation of the object
   */
  std::string python_repr();

  /*!
   * Get the total number of triples in the HDT document
   * @return The total number of triples in the HDT document
   */
  unsigned int getNbTriples();

  /*!
   * Get the number of distinct subjects in the HDT document
   * @return The number of distinct subjects in the HDT document
   */
  unsigned int getNbSubjects();

  /*!
   * Get the number of distinct predicates in the HDT document
   * @return The number of distinct predicates in the HDT document
   */
  unsigned int getNbPredicates();

  /*!
   * Get the number of distinct objects in the HDT document
   * @return The number of distinct objects in the HDT document
   */
  unsigned int getNbObjects();

  /*!
   * Get the number of shared subjects-objects in the HDT document
   * @return The number of shared subjects-objects in the HDT document
   */
  unsigned int getNbShared();

  /*!
   * Static factory method used to create a new HDT Document
   * @param file - Path to the HDT file
   * @param map - True maps the HDT file (faster), False loads everything in memory
   * @param indexed -  True if the HDT must be loaded with indexes, False otherwise
   */
  static HDTDocument create(std::string file, bool map, bool indexed) {
    return HDTDocument(file, map, indexed);
  }

  /*!
   * Convert a TripleID to a string RDF triple
   * @param  subject   - Triple's subject
   * @param  predicate - Triple's predicate
   * @param  object    - Triple's object
   * @return The associated RDF triple
   */
  triple convertTripleID(unsigned int subject, unsigned int predicate,
                     unsigned int object);

  /**
   * Convert an Object Identifier into the equivalent an RDF term
   * @param  id  - Object Identifier
   * @param  pos - Identifier position (subject, predicate or object)
   * @return The an RDF term equivalent to the Object Identifier
   */
  string convertID(unsigned int id, IdentifierPosition pos);

  /**
   * Convert an RDF term into the associated an Object Identifier.
   * @param  term  - RDF Term in string format
   * @param  pos - Identifier position (subject, predicate or object)
   * @return The Object Identifier associated with the RDF term
   */
  unsigned int convertTerm(std::string term, IdentifierPosition pos);

  /*!
   * Search all matching triples for a triple pattern, whith an optional limit and offset.
   * Returns a tuple<TripleIterator*, cardinality>
   * @param subject   - Triple pattern's subject
   * @param predicate - Triple pattern's predicate
   * @param object    - Triple pattern's object
   * @param limit     - (Optional) Maximum number of matching triples to read
   * @param offset    - (Optional) Number of matching triples to skip
   * @return A tuple (TripleIterator*, cardinality)
   */
  search_results search(std::string subject, std::string predicate,
                        std::string object, unsigned int limit = 0,
                        unsigned int offset = 0);

  /*!
   * Same as HDTDocument#search, but search for TripleIDs instead.
   * Returns a tuple<TripleIDIterator*, cardinality>
   * @param subject   - Triple pattern's subject identifier
   * @param predicate - Triple pattern's predicate identifier
   * @param object    - Triple pattern's object identifier
   * @param limit     - (Optional) Maximum number of matching triples to read
   * @param offset    - (Optional) Number of matching triples to skip
   * @return A tuple (TripleIDIterator*, cardinality)
   */
  search_results_ids searchIDs(unsigned int subject, unsigned int predicate,
                               unsigned int object, unsigned int limit = 0,
                               unsigned int offset = 0);

  /**
   * Evaluate a join between a set of triple patterns using a JoinIterator.
   * @param  patterns - Set of triple patterns
   * @return A JoinIterator* used to evaluated the join.
   */
  JoinIterator * searchJoin(std::vector<triple> patterns);

  // ============== BYTES REPRESENTATION ==============
  // Author: Arnaud GRALL - MIT License 2017-2019
  /*!
   * Search all matching triples for a triple pattern, whith an optional limit and offset. Returns bytes instead of string
   * Returns a tuple<TripleIterator*, cardinality>
   * @param subject   - Triple pattern's subject
   * @param predicate - Triple pattern's predicate
   * @param object    - Triple pattern's object
   * @param limit     - (Optional) Maximum number of matching triples to read
   * @param offset    - (Optional) Number of matching triples to skip
   * @return A tuple (TripleIterator*, cardinality)
   */
  search_results_bytes searchBytes(std::string subject, std::string predicate,
                        std::string object, unsigned int limit = 0,
                        unsigned int offset = 0);
  /**
   * Evaluate a join between a set of triple patterns using a JoinIterator.
   * @param  patterns - Set of triple patterns
   * @return A JoinIterator* used to evaluated the join.
   */
  JoinIteratorBytes * searchJoinBytes(std::vector<triple> patterns);
  /*!
   * Convert a TripleID to a RDF triple as bytes
   * @param  subject   - Triple's subject
   * @param  predicate - Triple's predicate
   * @param  object    - Triple's object
   * @return The associated RDF triple
   */
  triple_bytes convertTripleIDBytes(unsigned int subject, unsigned int predicate,
                     unsigned int object);

  /**
   * Convert an Object Identifier into the equivalent an RDF term as bytes
   * @param  id  - Object Identifier
   * @param  pos - Identifier position (subject, predicate or object)
   * @return The an RDF term equivalent to the Object Identifier
   */
  py::bytes convertIDBytes(unsigned int id, IdentifierPosition pos);
};

#endif /* PYHDT_DOCUMENT_HPP */
