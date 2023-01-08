#include <Lima/config.h>

#if defined(_IBMR2) & defined (M64BITS)

// Une classe String de service minimum pour Lima... Urgence...

string::size_type string::npos = -1;

string::string()
{
  m_str = 0;
}

string::string(const char * str)
{
  m_str = new char [strlen(str)+1];
  strcpy(m_str, str);
}

string::string(const string & str)
{
  m_str = new char [ str.size() + 1];
  strcpy(m_str, str.m_str);
}

const char *
string::c_str() const
{
  return m_str;
}

size_t string::size() const
{
  if (m_str == 0)
    return 0;
  else
    return strlen(m_str);
}

string::~string()
{
  if (m_str)
    delete [] m_str;
}

string  & string::operator = (const string & s1)
{
  if (&s1 != this)
  {
    if (m_str)
    delete [] m_str;
    m_str = new char [ s1.size() + 1];
    strcpy(m_str, s1.m_str);
  }
  return * this;
}

string & string::operator += (const string & s2)
{
  char * temp = new char [size() + s2.size() + 1];
  *temp = 0;
  strcat(temp, m_str);
  strcat(temp, s2.m_str);
  if (m_str)
  delete [] m_str;
  m_str = temp;
  return * this;
  
}


  
bool string::operator == (const string & s) const
{
  if (m_str == 0 && s.m_str == 0)
  return true;

  if (m_str != 0 && s.m_str == 0)
  return false;

  if (m_str == 0 && s.m_str != 0)
  return false;
  
  return (!strcmp(m_str, s.m_str));
  
}

string::size_type string::find_last_not_of(char c) const
{
  return npos;
}

void string::erase(string::size_type pos)
{
  
}

		 
#endif

#if defined(_IBMR2) & !defined(M64BITS)
#if __IBMCPP__ < 500
const string::size_type string::npos = 0;
#endif	// #if __IBMCPP__ < 500
#endif
