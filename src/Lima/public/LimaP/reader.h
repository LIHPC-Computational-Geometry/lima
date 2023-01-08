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

*/

#ifndef _READER_H
#define _READER_H


#include <Lima/enum.h>
#include <Lima/erreur.h>
#include "file.h"
#include "filecpp.h"
#include "misc.h"
#include <Lima/liste_attribut.h>
//#include "iostream.h"


BEGIN_NAMESPACE_LIMA

class _MaillageInterne;


//! Classe d'interface pour la lecture des fichiers
class _Reader {


public:


  /*! 
    \brief Lecture de la structure du maillage et des informations générales.
  */
  virtual void readStructure() = 0;


  /*! \brief Lecture (et chargement en mémoire) de la totalité du maillage */
  virtual void readAll() = 0;

  virtual ~_Reader(){
    m_file.close();
  };

  static _Reader* create(_MaillageInterne* mesh, 
                     const IN_STD string& nom_fichier, 
                     format_t format=SUFFIXE,
                     size_type num=1);

  /*!
    \brief Détecte le format par le suffixe du nom du fichier.
    \a nom_fichier nom du fichier
  */
  static format_t detectFormat(const IN_STD string& nom_fichier);

protected :

  _Reader(_MaillageInterne* mesh, 
          const IN_STD string& nom_fichier, 
          format_t format=SUFFIXE,
          size_type num=1);


	class LectureAnnulee
	{
		public :

		LectureAnnulee ( );
		LectureAnnulee (const LectureAnnulee&);
		LectureAnnulee& operator = (const LectureAnnulee&);
		virtual ~LectureAnnulee ( );
	};	// class LectureAnnulee


  //! reference sur le maillage
  _MaillageInterne* m_mai;

  //! Fichier du _Reader
  File m_file;
  FileCpp m_filecpp;
};




END_NAMESPACE_LIMA

#endif // _READER_H
