/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader.h
 Cree le : Tue Oct 24 13:18:32 MET DST 2000
 Auteur  : JM Bellard
******************************************************************************/

/*!
  \file reader.h
  \brief Interface de lecture / ecriture

  vocabulaire et prefixe des methodes :
  - "read" pour lire qq chose et la fonction retourne la valeur
  - "load" pour lire qq chose et chargement en memoire fait par la methode

*/

#ifndef _WRITER_H
#define _WRITER_H


#include <Lima/enum.h>
#include <Lima/erreur.h>
#include "file.h"
#include "misc.h"



BEGIN_NAMESPACE_LIMA

class _MaillageInterne;

//! \class Classe d'interface pour l'écriture des fichiers
class _Writer {

public:

  static _Writer* create(_MaillageInterne* mesh, 
                     const IN_STD string& nom_fichier, 
                     format_t format=SUFFIXE,
                     size_type num=1);

  virtual void write(format_t format) = 0;

  virtual ~_Writer();

protected:
  _Writer(_MaillageInterne* mesh,
          const IN_STD string& fichier, 
          format_t format=SUFFIXE);

  //! reference sur le maillage
  _MaillageInterne* m_mai;

  //! Fichier du _Writer
  File m_file;
 
};

END_NAMESPACE_LIMA

#endif // WRITER
