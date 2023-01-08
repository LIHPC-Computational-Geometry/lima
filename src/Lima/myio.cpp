/*****************************************************************************\
 *                          U T I L I T A I R E                              *
\*****************************************************************************/


/******************************************************************************
 Fichier : myio.h
 Cree le : Tue Aug 22 15:26:24 MET DST 2000
 Auteur  : JM Bellard
******************************************************************************/

#include <stdio.h>
#include <stdarg.h>


int pout(const char* format, ...){
  int ret;
  va_list arg;
  va_start(arg, format);
  ret = vfprintf(stdout, format, arg);
  va_end(arg);
  return ret;
}


int perr(const char* format, ...){
  int ret;
  va_list arg;
  fprintf(stderr, "\aErreur : ");
  va_start(arg, format);
  ret = vfprintf(stderr, format, arg);
  va_end(arg);
  return ret;
}

