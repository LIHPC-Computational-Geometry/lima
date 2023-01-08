#ifdef __INTERNE_IDEAS
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_ideas.h
 Cree le : Wed Dec 20 14:35:58 MET 2000
 Auteur  : JM Bellard
******************************************************************************/

/*!
  \file reader_ideas.h
  \brief Interface de lecture / ecriture pour les fichiers ideas
*/

#ifndef _READER_IDEAS_H
#define _READER_IDEAS_H

#include "reader.h"
#include "writer.h"


BEGIN_NAMESPACE_LIMA

class _ReaderIdeas : public _Reader {

public:
  virtual void readStructure();

  virtual void readAll();


  _ReaderIdeas(_MaillageInterne* mesh, const IN_STD string& nom_fichier, format_t format=SUFFIXE);
  
  virtual ~_ReaderIdeas();

private:
  //! Lecture d'un fichier ideas.
  void          lire( );
  enum Typ { NOE = 0, BRA, PGN, PDR};

  //! Recherche de toutes les occurances de D et remplace par E.
  void remplace_D_par_E(char* str);
  
  //! Lecture d'une ligne du fichier et stockage dans le buffer.
  void lire_ligne(FILE* fp, char* buf);

  //! Recherche le debut d'un nouveau dataset et retourne son numero.
  int debut_dataset(FILE* fp);

  //! Recherche la fin du dataset.
  void fin_dataset(FILE* fp);

  //! Lecture titre et date.
  void          lire_dataset_titre(FILE* fp);
  //! Lecture unite de longueur.
  void          lire_dataset_unite(FILE* fp);
  //! Lecture des coordonnees des noeuds.
  void          lire_dataset_noeuds(FILE* fp);
  //! Lecture des elements finis.
  void          lire_dataset_elements(FILE* fp, int dataset, 
				      IN_STD vector<Typ>& type);
  //! Lecture des groupes.
  void          lire_dataset_groupes(FILE* fp, int dataset, 
				     IN_STD vector<Typ>& type);


};

//////////////////////////////////////////////////////////////////////////////
class _WriterIdeas : public _Writer {
public :
  _WriterIdeas(_MaillageInterne* mesh, 
               const IN_STD string& nom_fichier, 
               format_t format=SUFFIXE);

  virtual ~_WriterIdeas();
  void write(format_t format);
private:
  void ecrire(File* fd);
  void ecrire_ligne(FILE* fp, const char* buf);
};


END_NAMESPACE_LIMA

#endif // _READER_IDEAS_H
#endif
