#ifndef _ERREUR_H
#define _ERREUR_H

#include "config.h"
#include "size.h"

#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

class erreur 
{
private:
  IN_STD string str;

public:
  erreur(const IN_STD string& what_arg) : str(what_arg) {}

  virtual ~erreur() {}
  
  virtual const char * what() const 
  {
    return str.c_str();
  }
 virtual size_t length ( ) const
 { return str.length ( ); }
};

class read_erreur : public erreur
{
public:
  read_erreur(const IN_STD string& what_arg) : erreur(what_arg) {}

  virtual ~read_erreur ()  {}
};

class eof_erreur : public  erreur
{
public:
  eof_erreur(const IN_STD string& what_arg) : erreur(what_arg) {}

  virtual ~eof_erreur ()  {}
};

class argument_erreur : public  erreur
{
public:
  argument_erreur(const IN_STD string& what_arg) :  erreur(what_arg) {}

  virtual ~argument_erreur()  {}
};

class range_erreur : public  erreur
{
public:
  range_erreur(const IN_STD string& what_arg) :  erreur(what_arg) {}

  virtual ~range_erreur ()  {}
};

class alloc_erreur : public  erreur
{
public:
  alloc_erreur(const IN_STD string& what_arg) :  erreur(what_arg) {}

  virtual ~alloc_erreur ()  {}
};

class write_erreur : public erreur
{
public:
  write_erreur(const IN_STD string& what_arg) :  erreur(what_arg) {}

  virtual ~write_erreur ()  {}
};

class open_erreur : public  erreur
{
public:
  open_erreur(const IN_STD string& what_arg) :  erreur(what_arg) {}

  virtual ~open_erreur ()  {}
};

class att_erreur : public  erreur
{
public:
  att_erreur(const IN_STD string& what_arg) :  erreur(what_arg) {}

  virtual ~att_erreur ()  {}
};

class memory_erreur : public  erreur
{
public:
  memory_erreur(const IN_STD string& what_arg) : erreur(what_arg) {}

  virtual ~memory_erreur ()  {}
};

IN_STD string to_str(size_type val);
IN_STD string to_str(const IN_STD string& str);


#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif
