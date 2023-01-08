#ifdef __INTERNE_MAZE
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_maze.h
 Cree le : Wed Dec 20 14:35:58 MET 2000
 Auteur  : JM Bellard
******************************************************************************/

/*!
  \file reader_maze.h
  \brief Interface de lecture / ecriture pour les fichiers maze
*/

#ifndef _READER_MAZE_H
#define _READER_MAZE_H

#include "reader.h"


BEGIN_NAMESPACE_LIMA

class _ReaderMaze : public _Reader {

public:
  virtual void readStructure();

  virtual void readAll();


  _ReaderMaze(_MaillageInterne* mesh, const IN_STD string& nom_fichier, format_t format=SUFFIXE);
  
  virtual ~_ReaderMaze();

private:
  //! Lecture du fichier.
  void          lire( );

  typedef char nom_t[41];

  //! Lecture d'une ligne.
  void lire_ligne(FILE* fp, char* buf);

  //! Lecture des caracteristiques du maillage.
  void          lire_control_cards(FILE* fp, size_t* nb_mat, 
                                   size_t* nb_pt, size_t* nb_el, 
                                   size_t* nb_ligne, size_t* nb_nc);
  //! Lecture des materiaux.
  void          lire_material_cards(FILE* fp, char* nom);
  //! Lecture des coordonnees des noeuds.
  void          lire_point_cards(FILE* fp, size_type nb_pt);
  //! Lecture des mailles.
  void          lire_element_cards(FILE* fp, size_type nb_el, size_type nb_mat, nom_t* noms);
  //! Lecture des lignes de chargement
  void          lire_boundary_condition_card(FILE* fp, size_type nb_ligne);
  //! Lecture des contraintes nodales
  void          lire_nodal_constraint_card(FILE* fp, size_type nb_nc);

};

END_NAMESPACE_LIMA

#endif // _READER_MAZE_H
#endif
