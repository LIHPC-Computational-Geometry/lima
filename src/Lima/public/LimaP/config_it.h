#ifndef _CONFIG_IT_H
#define _CONFIG_IT_H

#include <Lima/config.h>


// La classe IString ne fonctionne pas en 64 bits.
#if defined(_IBMR2) 
#if __IBMCPP__ < 500

// Pas de STL standard avec les versions anciennes du compilateur IBM,
// ni de namespace.
#include <algo.h>
#include <stdio.h>
#include <ctype.h>

#else	// #if __IBMCPP__ < 500 => Visual Age xlC 5 et +

#include <vector>

#include <string>

#include <algorithm>

#include <memory>

#include <strstream>

#include<sstream>

#include<fstream>

#include<iterator>

#include<map>


#endif	// #if __IBMCPP__ < 500

#elif defined(CRAY)

// Pas de STL standard avec le compilateur CRAY
#include <stdio.h>
#include <ctype.h>
#include <vector.h>
#include <algo.h>

// Pas de namespace avec le compilateur CRAY


#elif defined(__hpux)

// pas de namespace

#include <algorithm>

#include <strstream.h>


#else

// Dans les autres cas namespace et STL standard.


#include <vector>

#include <string>

#include <algorithm>

#include <memory>

#include <stdexcept>

#include <strstream>

#include<sstream>

#include<iterator>

#include<map>


#endif

#endif
