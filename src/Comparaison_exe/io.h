#ifndef _IO_H
#define _IO_H

#if defined(_IBMR2) && (__IBMCPP__ < 500)

#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <strstream.h>

#elif defined(CRAY)

#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <strstream.h>

#elif defined(__hpux)

#include <strstream.h>
#include <iomanip.h>

#else

using namespace Lima;

#include <iostream>
#include <iomanip>
#include <fstream>
#include <strstream>

//#if !defined(__sgi)
using std::ostream;
using std::ostrstream;
using std::setprecision;
using std::ends;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
//#endif

#endif

#if defined(__sgi)
inline ostream& operator << (ostream& o, const std::string& s)
{
  return o << s.c_str();
}
#endif

#endif
