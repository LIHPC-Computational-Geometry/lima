#ifdef __INTERNE_INGRID
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_ingrid.h
 Cree le : Wed Dec 20 14:35:58 MET 2000
 Derniere modification le : Mon Dec 16 14:37:10 MET 2002 (C. Pignerol)
 Auteur  : JM Bellard
******************************************************************************/

/*!
  \file reader_ingrid.h
  \brief Interface de lecture / ecriture pour les fichiers ingrid
*/

#ifndef _READER_INGRID_H
#define _READER_INGRID_H

#include "reader.h"


BEGIN_NAMESPACE_LIMA

class _ReaderIngrid : public _Reader {

public:
  virtual void readStructure();

  virtual void readAll();


  _ReaderIngrid(_MaillageInterne* mesh, const IN_STD string& nom_fichier, format_t format=SUFFIXE);
  
  virtual ~_ReaderIngrid();

private:

  /// Lecture du fichier.
  void          lire( );

  //! Lecture d'une ligne.
  void lire_ligne(FILE* fp, char* buf);

  typedef char nom_t[41];

  //! Lecture des caracteristiques du maillage.
  void          lire_control_cards(FILE* fp, size_t* nb_mat, 
                                   size_t* nb_pt, size_t* nb_el,
                                   size_t* nb_dt, size_t* nb_sur, int* format);
  //! Lecture des materiaux.
  void          lire_material_cards(FILE* fp, char* nom);
  //! Lecture des coordonnees des noeuds.
  void          lire_point_cards(FILE* fp, size_t nb_pt);
  //! Lecture des mailles.
  void          lire_element_cards(FILE* fp, size_t nb_el, size_t nb_mat, nom_t* noms);
  //! Lecture des surfaces.
  void          lire_sliding_interface(FILE* fp, size_t nb_sur);
  //! Lecture des points.
  void          lire_detonation_points(FILE* fp, size_t nb_dt);

  //! Retourne un nouveau nom de matériau.
  IN_STD string		creer_nom_materiau ( );

  //! La ligne en cours de lecture.
  size_t		_n_ligne;

  //! Le numéro du dernier matériau dont le nom a été créé.
  size_t		_dernierMatNum;
};

END_NAMESPACE_LIMA

#endif // _READER_INGRID_H
#endif
