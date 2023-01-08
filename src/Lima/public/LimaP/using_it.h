#ifndef _USING_IT_H
#define _USING_IT_H

#include "config_it.h"


// La classe IString ne fonctionne pas en 64 bits.
#if defined(_IBMR2) 
#if __IBMCPP__ < 500

// Pas de STL standard avec les versions anciennes du compilateur IBM,
// ni de namespace.

#else	// #if __IBMCPP__ < 500 => Visual Age xlC 5 et +

using std::vector;

using std::string;

using std::max;
using std::min;
using std::swap;
using std::lower_bound;
using std::find_if;
using std::find;
using std::for_each;
using std::max_element;
using std::reverse;
using std::copy;
using std::istrstream;

using std::auto_ptr;

using std::iterator;
using std::map;
using std::multimap;
using std::istringstream;
using std::istream_iterator;
using std::pair;


#endif	// #if __IBMCPP__ < 500

#elif defined(CRAY)

// Pas de STL standard avec le compilateur CRAY

// Pas de namespace avec le compilateur CRAY


#elif defined(__hpux)

// pas de namespace



#else

// Dans les autres cas namespace et STL standard.


using std::vector;

using std::string;

using std::max;
using std::min;
using std::swap;
using std::lower_bound;
using std::find_if;
using std::find;
using std::for_each;
using std::max_element;
using std::reverse;
using std::copy;

using std::auto_ptr;

using std::exception;

using std::ifstream;
using std::istrstream;

using std::iterator;
using std::map;
using std::multimap;
using std::istringstream;
using std::istream_iterator;
using std::pair;

#endif

#endif
