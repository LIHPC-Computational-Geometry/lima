#ifdef __INTERNE_GIBI
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_k2k.h
 Cree le : Tue Oct 24 13:18:32 MET DST 2000
 Auteur  : JM Bellard
******************************************************************************/

/*!
  \file reader_k2k.h
  \brief Interface de lecture / ecriture pour les fichiers de sauvegarde et de sortie castem
*/

#ifndef _READER_K2K_H
#define _READER_K2K_H

#include "myio.h"
#include "reader.h"
#include "maillage_it.h"

#include "gibicastem.h"

BEGIN_NAMESPACE_LIMA

class _ReaderK2K : public _Reader {

public:

  _ReaderK2K(_MaillageInterne* mesh, const IN_STD string& nom_fichier, format_t format=SUFFIXE) : _Reader(mesh, nom_fichier, format){};
  virtual ~_ReaderK2K(){};





  ////////////////////////////////////////////////////////////////////////////
  // objet au sens GIBI et CASTEM 2000
  enum {GROUPE = 0,
        NOEUD, BRAS,
        TRIANGLE, QUADRANGLE,
        TETRAEDRE, PRISME, CUBE,
        NB_MAX_ELM_TYPE}; 



  /*!
    \brief retourne le nombre de noeuds d'un objet gibi de type \a type
  */
  static int k2k_NbNode
  (
   int type
   ){
    const int elm_type_nb_node[NB_MAX_ELM_TYPE] = {-1, 1, 2, 3, 4, 4, 6, 8};
#ifdef RANGE_CHECKING
    if (type > GROUPE && type < NB_MAX_ELM_TYPE)
      return elm_type_nb_node[type];
    perr("out of range in : k2k_NbNode. Type = %d Max = %d\n", type, (NB_MAX_ELM_TYPE - 1));
    return 0;
#else
    return elm_type_nb_node[type];
#endif
  };
  
  //! \return La chaine de caractere servant a identifier le type d'objet
  static const char* elmTypeName(int type) {
    const char *elm_type_name[NB_MAX_ELM_TYPE]= {
      "    ", "POI1", "SEG2", "TRI3", "QUA4", "TET4", "PRI6", "CUB8"};
#ifdef RANGE_CHECKING
    if (type >= 0 && type < NB_MAX_ELM_TYPE)
      return elm_type_name[type];
    perr("out of range in : elmTypeName\n");
    return elm_type_name[0];
#else
      return elm_type_name[type];
#endif
  };

  //! \return La dimension des elements (1D, 2D, 3D)
  static dim_t elmTypeDimension(int type) {
    dim_t elm_type_dim[NB_MAX_ELM_TYPE] = {D1, D1, D1, D2, D2, D3, D3, D3};
#ifdef RANGE_CHECKING
    if (type >= 0 && type < NB_MAX_ELM_TYPE)
      return elm_type_dim[type];
    perr("out of range in : elmTypeDimension. Type = %d Max = %d\n", type, (NB_MAX_ELM_TYPE - 1));
    return elm_type_dim[0];
#else
    return elm_type_dim[type];
#endif
  };
};
  

END_NAMESPACE_LIMA

#endif // _READER_K2K_H
#endif
