/*****************************************************************************\
 *                          U T I L I T A I R E                              *
\*****************************************************************************/


/******************************************************************************
 Fichier : myio.h
 Cree le : Tue Aug 22 15:26:24 MET DST 2000
 Auteur  : JM Bellard
******************************************************************************/

#ifndef _MYIO_H
#define _MYIO_H

//#ifdef  __cplusplus
//extern "C" {
//#endif


extern int pout(const char* format, ...);

extern int perr(const char* format, ...);


#define SHOWI(a) pout(#a " : %d\n", a);
#define SHOWF(a) pout(#a " : %g\n", a);  

//#ifdef  __cplusplus
//           }
//#endif

#endif /*_MYIO_H */
