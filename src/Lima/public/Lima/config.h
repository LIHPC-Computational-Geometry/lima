#ifndef _CONFIG_H
#define _CONFIG_H


// La classe IString ne fonctionne pas en 64 bits.
#if defined(_IBMR2) & defined(M64BITS)

// Pas de namespace avec ce compilateur IBM.
#define BEGIN_NAMESPACE_LIMA
#define END_NAMESPACE_LIMA

#ifndef IN_STD
#define IN_STD
#endif	// IN_STD

// Pas de STL standard avec ce compilateur IBM.
#include <vector.h>

// Classe string réduite de remplacement.
class string 
{
private:
  char * m_str;
public:
  typedef size_t size_type;
  static size_type npos;

  string() ;
  string(const char* str);
  string(const string& str);
  const char* c_str() const ;
  string & operator += ( const string &);
  string & operator = (const string &);
  bool operator == (const string &) const;
  size_t size() const;
  ~string();
  size_type find_last_not_of(char c) const;
  void erase(size_type pos);
};

inline
string operator + (const string& s1, const string& s2)
{
  string s = s1;
  s += s2;
  return s;
}

inline
string operator + (const string& s1, const char* s2)
{
  string s = s1;
  s += s2;
  return s;
}

inline
string operator + (const char* s1, const string& s2)
{
  string s(s1);
  s += s2;
  return s;
}


#elif defined(_IBMR2)

#if __IBMCPP__ < 500

// Pas de namespace avec les versions anciennes du compilateur IBM.
#define BEGIN_NAMESPACE_LIMA
#define END_NAMESPACE_LIMA

#ifndef IN_STD
#define IN_STD
#endif	// IN_STD

// Pas de STL standard avec les versions anciennes du compilateur IBM.
#include <istring.hpp>
#include <vector.h>

// Utilisation de la classe IString pour construire la classe string.
class string : public IString
{
public:
  typedef size_t size_type;

  string() : IString() {}
  string(const char* str) : IString(str) {}
  string(const string& str) : IString(str) {}
  const char* c_str() const { return *this; }
  size_type find_last_not_of(char c) const;
  string& erase(size_type p0 = 0);

  static const size_type npos;
};

inline
string operator + (const string& s1, const string& s2)
{
  string s = s1;
  s += s2;
  return s;
}

inline
string operator + (const string& s1, const char* s2)
{
  string s = s1;
  s += s2;
  return s;
}

inline
string operator + (const char* s1, const string& s2)
{
  string s(s1);
  s += s2;
  return s;
}

inline
string::size_type string::find_last_not_of(char c) const
{
  return lastIndexOfAnyBut(c);
}

inline
string& string::erase(size_type p)
{
  IString::remove(p);
  return *this;
}

#else	// #if __IBMCPP__ < 500 => Visual Age xlC 5 et +

#define BEGIN_NAMESPACE_LIMA namespace Lima {
#define END_NAMESPACE_LIMA   }

#ifndef IN_STD
#define IN_STD std::
#endif	// IN_STD

#include <string>
#include <vector>

#endif	// #if __IBMCPP__ < 500

#elif defined(CRAY)

// Pas de STL standard avec le compilateur CRAY
#include <mstring.h>
#include <vector.h>

// Pas de namespace avec le compilateur CRAY
#define BEGIN_NAMESPACE_LIMA
#define END_NAMESPACE_LIMA

#ifndef IN_STD
#define IN_STD
#endif	// IN_STD


#elif defined(__hpux)

// pas de namespace

#define BEGIN_NAMESPACE_LIMA
#define END_NAMESPACE_LIMA

#ifndef IN_STD
#define IN_STD
#endif	// IN_STD

#include <string>
#include <vector>

#else

// Dans les autres cas namespace et STL standard.

#define BEGIN_NAMESPACE_LIMA namespace Lima {
#define END_NAMESPACE_LIMA   }

#ifndef IN_STD
#define IN_STD std::
#endif	// IN_STD

#include <string>
#include <vector>

#endif

#endif
