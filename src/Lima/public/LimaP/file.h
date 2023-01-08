/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/

// ATTENTION : attributs format et mesh_num commentes, a supprimer 
// definitivement (CP le 26/08/02)

/******************************************************************************
 Fichier : file.h
 Cree le : Mon Oct  2 18:07:26 MET DST 2000
 Derniere modification : Wed Apr 30 08:23:22 MEST 2003 (C. Pignerol)
 Auteur  : JM Bellard
******************************************************************************/

/*!
  \file file.h
  \brief Descripteur de fichier
*/

#ifndef _FILE_H
#define _FILE_H

#include <string.h>
#include <stdio.h>
#include <Lima/enum.h>
#include <Lima/erreur.h>
#include "liste_fichier.h"

BEGIN_NAMESPACE_LIMA

//! Descripteur de fichier
class File {

public:
  IN_STD string full_name;   //!< nom complet du fichier
//  format_t format;    //!< format 
  FILE* fp;           //!< fichier
  int id;             //!< identifiant
  int line_num;       //!< numero de ligne courante (sert pour les erreurs)
  bool ascii;         //!< == true si le fichier est ascii
  void* udata;        //!< donnee utilisateur
  bool  writable;     //!< ouverture du fichier en ecriture
//  size_type mesh_num; //!< numero du maillage dans le fichier 

public:

  ///////////////////////////////////////////////////////////////////////
  File (char* name, format_t _format = SUFFIXE, size_type num=1)
  : full_name (IN_STD string(name)), fp (NULL), id (0), ascii (false),
    line_num (0), udata (NULL), writable (false)	// CP, v 6.27.1, coverity
  {
//      full_name = IN_STD string(name);
//      format = _format;
//      fp = NULL; id = 0; ascii = false; line_num = 0; udata = NULL;	
//      mesh_num=num;
  }

  ///////////////////////////////////////////////////////////////////////
  File (const IN_STD string& str, format_t _format = SUFFIXE, size_type num=1)
  : full_name (str), fp (NULL), id (0), ascii (false),
    line_num (0), udata (NULL), writable (false)	// CP, v 6.27.1, coverity
  {
//      full_name = str;
//      format = _format;
//      fp = NULL; id = 0; ascii = false; line_num = 0; udata = NULL;
//      mesh_num=num;
  }

  ///////////////////////////////////////////////////////////////////////
  //! ouvre le fichier
  void open(const char *mode, size_type num=-1) {
      fp = fopen(full_name.c_str(), mode);
      if(fp==NULL)
        throw open_erreur("Impossible d'ouvrir le fichier " + full_name);
  }


  ///////////////////////////////////////////////////////////////////////
  //! ferme le fichier
  void close(){
      if (NULL != fp)
		fclose(fp);
	  fp = NULL;
  }


  ///////////////////////////////////////////////////////////////////////
  void pwhere(){
    fprintf(fp, "Ligne %d du fichier %s\n", line_num, full_name.c_str());
  }

};

END_NAMESPACE_LIMA

#endif /* _FILE_H */
