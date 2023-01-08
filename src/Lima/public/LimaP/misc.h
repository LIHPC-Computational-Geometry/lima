#ifndef _MISC_H
#define _MISC_H

#include "config_it.h"
#include <Lima/size.h>
#include "file.h"

BEGIN_NAMESPACE_LIMA

class string_fortran
{
public:
  string_fortran(const char* str);
  string_fortran(const char* buf, int size);
  ~string_fortran();
    
  operator char*() {return m_data;};
  operator IN_STD string() {return IN_STD string(m_data);};
    
  void vers_fortran(char* buf, int size);
    
private:
  char*    m_data;
};


//////////////////////////////////////////////////////////////////////////////
/*!
  \brief comme atoi mais lit uniquement les n premiers caracteres
  \return int
*/
extern int atoin
(
 char* c, //!< chaine
 int n    //!< nombre de caracteres maximum a prendre en compte
 );

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief comme atof mais lit uniquement les n premiers caracteres
  \return double
*/
extern double atofn
(
 char* c, //!< chaine
 int n    //!< nombre de caracteres maximum a prendre en compte
 );


//////////////////////////////////////////////////////////////////////////////
/*!
  \brief comme "fgets" avec remplacement de \n par \0 et gestion exception
  \exception read_erreur "Flot de données interrompu\n"
  \exception eof_erreur "Fin de fichier rencontrée prématurement\n"
*/
extern char* fgets2
(
 char* buf,  //!< buffer
 int   lon,  //!< longueur du buffer
 File* fp    //!< descripteur de fichier
 );


//////////////////////////////////////////////////////////////////////////////
/*!
  \brief comme "fprintf" avec gestion exception
  \exception write_erreur
  \exception eof_erreur
*/
extern int fprintf2
(
 File* fd,      //!< descripteur de fichier
 const char* format,  //!< format d'ecriture
 ...            //!< arguments eventuels
 );

extern char* fgets2
(
 char* buf,  //!< buffer
 int   lon,  //!< longueur du buffer
 File& fd    //!< descripteur de fichier
 );
END_NAMESPACE_LIMA

#endif
