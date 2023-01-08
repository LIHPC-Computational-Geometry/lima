#ifdef __INTERNE_MODULEF
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/

/******************************************************************************
 Fichier : io_fortran.h
 Cree le : Fri Jul  7 14:41:21 MET DST 2000
 Auteur  : JM Bellard
******************************************************************************/

#include <string.h>

#if !defined(_CRAY)

extern "C" {
  void findfreeul_(int*, int*);
  void openf_(char*, int*, int*, int);
  void closef_(int*, int*);
  void writeint_(int*, int*, int*, int*);
  void readint_(int*, int*, int*, int*);
}

inline void findfreeul(int* iu, int* ier)
{
  findfreeul_(iu, ier);
}

inline void openf(char*fic, int* iu, int* ier)
{
  openf_(fic, iu, ier, strlen(fic));
}

inline void closef(int* iu, int* ier)
{
  closef_(iu, ier);
}

inline void writeint(int* t, int* l, int* iu, int* ier)
{
  writeint_(t,l,iu,ier);
}

inline void readint(int* t, int* l, int* iu, int* ier)
{
  readint_(t,l,iu,ier);
}


#endif

#if defined(_CRAY)

#include <fortran.h>

extern "C" {  

  void FINDFREEUL(int*, int*);
  void OPENF(_fcd, int*, int*);
  void CLOSEF(int*, int*);
  void WRITEINT(int*, int*, int*, int*);
  void READINT(int*, int*, int*, int*);

}

inline void findfreeul(int* iu, int* ier)
{
  FINDFREEUL(iu, ier);
}

inline void openf(char* fic, int* iu, int* ier)
{
  OPENF(_cptofcd(fic, strlen(fic)), iu, ier);
}

inline void closef(int* iu, int* ier)
{
  CLOSEF(iu, ier);
}

inline void writeint(int* t, int* l, int* iu, int* ier)
{
  WRITEINT(t,l,iu,ier);
}

inline void readint(int* t, int* l, int* iu, int* ier)
{
  READINT(t,l,iu,ier);
}

#endif

#endif
