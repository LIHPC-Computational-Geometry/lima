#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <values.h>

#include "LimaP/misc.h"
#include <Lima/erreur.h>
#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA

string_fortran::string_fortran(const char* str)
{
  m_data = new char[strlen(str)+1];
  strcpy(m_data, str);
}
  
string_fortran::string_fortran(const char* buf, int size)
{
  const char* ptr = buf+size;
  do{
    --ptr;
  }while(isspace(*ptr));
  ++ptr;
    
  m_data = new char[ptr-buf+1];
  memcpy(m_data, buf, ptr-buf);
  m_data[ptr-buf]='\0';
}

string_fortran::~string_fortran()
{
  delete[] m_data;
}
  
void string_fortran::vers_fortran(char* buf, int size)
{
  memset(buf, ' ', size);
  memcpy(buf, m_data, min(size, (int)strlen(m_data)));
}

string to_str(size_type val)
{
  char buf[128];
  sprintf(buf, "%ld", val);
  return string(buf);
}

string to_str(const string& str)
{
  return string(str);
}


//////////////////////////////////////////////////////////////////////////////
/*!
  \brief comme atoi mais lit uniquement les n premiers caracteres
  \return int
*/
int atoin
(
 char* str,
 int nb
 ){
	// CP, v 6.27.1, coverity
	const char	ch	= str [nb];
	str [nb]	= '\0';

	char*	ptr		= str;
	char*	endptr	= ptr;
	long	val		= strtol (ptr, &endptr, 10);
	if (('\0' != *endptr) && (' ' != *endptr))
		throw read_erreur ("Entier attendu et non trouv�.\n");
	if ((val < INT_MIN) || (val > INT_MAX))
		throw range_erreur ("Entier hors domaine.\n");

	str [nb]	= ch;

	return (int)val;
/*
  int res	= 0;
  char ch = str[nb];
  str[nb] = '\0';
  res = atoi(str);
  str[nb] = ch;
  return res;
*/
}


//////////////////////////////////////////////////////////////////////////////
/*!
  \brief comme atof mais lit uniquement les n premiers caracteres
  \return double
*/
double atofn
(
 char* str,
 int nb
 ){
  double res;
  char ch = str[nb];
  str[nb] = '\0';
  res = atof(str);
  str[nb] = ch;
  return res;
}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief comme "fgets" avec remplacement de \n par \0 et gestion exception
  \exception read_erreur
  \exception eof_erreur
*/
char* fgets2
(
 char* buf,  //!< buffer
 int   lon,  //!< longueur du buffer
 File* fd    //!< descripteur de fichier
 ){
  char* res = fgets(buf, lon, fd->fp);
  fd->line_num++;
  if(res==NULL && !feof(fd->fp)) {
    perror("LIMA");
    throw read_erreur(string("Flot de donn�es interrompu\n") +
                     "Erreur detectee a la ligne " + to_str(fd->line_num) +
                     " du fichier.\n");
  }

  if(res==NULL && feof(fd->fp)){
    perror("LIMA");
    throw eof_erreur(string("Fin de fichier rencontr�e pr�maturement\n") +
                     "Erreur detectee a la ligne " + to_str(fd->line_num) +
                     " du fichier.\n");
  }  
  // elimination du retour chariot
  char* fin_ligne = strchr(buf, '\n');
  if(fin_ligne!=NULL)
    *fin_ligne = '\0';

  return res;
}

char* fgets2
(
 char* buf,  //!< buffer
 int   lon,  //!< longueur du buffer
 File& fd    //!< descripteur de fichier
 ){
  return fgets2(buf, lon, &fd);
}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief comme "fprintf" avec gestion exception
  \exception write_erreur
  \exception eof_erreur
*/
int fprintf2
(
 File* fd,      //!< descripteur de fichier
 const char* format,  //!< format d'ecriture
 ...            //!< arguments eventuels
 ){

  int ret;
  va_list arg;
  va_start(arg, format);
  ret = vfprintf(fd->fp, format, arg);
  va_end(arg);
  if (ret == 0) {
    perror("LIMA");
    throw write_erreur(string("Flot de donn�es interrompu\n"));
  }
  return ret;
}

 


END_NAMESPACE_LIMA
