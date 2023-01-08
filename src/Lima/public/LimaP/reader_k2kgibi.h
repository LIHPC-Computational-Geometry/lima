#ifdef __INTERNE_GIBI
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_k2kgibi.h
 Cree le : Tue Oct 24 13:18:32 MET DST 2000
 Fin le  : Tue Oct  8 14:48:31 MEST 2002 (C. Pignerol)
 Auteur  : JM Bellard
******************************************************************************/

/*!
  \file reader_k2kgibi.h
  \brief Interface de lecture / ecriture pour les fichiers gibi
*/

#ifndef _READER_K2KGIBI_H
#define _READER_K2KGIBI_H

#include "reader_k2k.h"
#include "writer.h"


BEGIN_NAMESPACE_LIMA

class _ReaderK2KGibi : public _ReaderK2K {

public:
  typedef char char15[15];

  virtual void readStructure();

  virtual void readAll();


  _ReaderK2KGibi(_MaillageInterne* mesh, const IN_STD string& nom_fichier, format_t format=SUFFIXE);
  
  virtual ~_ReaderK2KGibi();



  typedef struct gibi_object_t {
    char name[16];    //!< nom eventuel de l'objet. en fait 8 caracteres (7+\0)
    size_type  typ;   //!< type des elements du groupe 
    size_type  nbe;   //!< nombre d'elements dans le groupe 
    size_type* elm_n; //!< liste des elements [1..] numerotation lima */
    size_type  nbn;   //!< nombre de noeuds (=nbe * nb_noeud_par_elm) */
    size_type* nod;   //!< liste des noeuds [1..]  numerotation gibi */
  } gibi_object_t;
 

private:

  typedef struct tab_gibi_object_t {
    size_type nbr;
    size_type max;
    gibi_object_t* elm;
  } tab_gibi_object_t;

  int gibi_level; //! version du fichier 
  static int TailleBuf;
  char buf[128+1];

  int nb_pt_nomme; // nombre de points nommes dans le fichier
  int nb_obj; // nombre d'objets gibi
  int  nb_obj_nomme; // nombre d'objets gibi nommes


 
  /*! index apres la lecture de la ligne du type :
    \code
    NOMBRE DE POINTS        9
  */
  long idx_coordinate;

  /*! index apres la lecture de la ligne du type :
    \code
    NOMBRE DE POINTS NOMMES       2
   */
  long idx_node_named;
  
  /*! index AVANT la lecture de la ligne du type :
    \code
    QUA4 SOUS OBJETS   0 REFERENCES   4 NB NOEUDS   4 NBELEM    4
  */
  long *idx_object;

  /*! index apres la lecture de la ligne du type :
    \code
    NOMBRE D'OBJETS NOMMES       3
   */
  long idx_object_named;

  k2k_lname_t lnode_named;
  k2k_lobject_t lobject;
  k2k_lname_t lobject_named;

  /*! 
    permet de ne charger que les noeuds utiles
    dimensionne au nombre a m_nb_object[_Reader::NOEUD]
      useful_node[ipt] == "y" --> a charger 
      useful_node[ipt] == "-" --> utile mais ne pas charger, car il l'est deja
      useful_node[ipt] == "n" --> ne pas charger, il n'est pas necessaire
  */
  char* useful_node;

  long idxNodeNamed();
  long idxObject(int num);
  long idxObjectNamed();
  void read_gibi_object(gibi_object_t* obj, File* fp, 
                        int nb_max_obj, int gibi_level);

  void addGroup(const char *name, gibi_object_t* obj);
  void addNuage(_NuageInterne* g, gibi_object_t* obj);
  void addLigne(_LigneInterne* g, gibi_object_t* obj);
  void addSurface(_SurfaceInterne* g, gibi_object_t* obj);
  void addVolume(_VolumeInterne* g, gibi_object_t* obj);

  void addObject
  (
   tab_gibi_object_t object, //!< Liste d'objets
   int nb_obj_nomme,
   char15 *obj_name,
   int* obj_nomme_num
   );

  size_type  objectType(char* buf, k2k_object_t* obj = NULL);

  void readHeader();
  void lire( );
  void readNb();

  void readObjectNamed();
  void readNodeNamed();
  void readListOfName(k2k_lname_t& lname);
  
  void readk2kObject(k2k_object_t* obj);
  
  //! Le nombre de noeuds du maillage
  size_type		m_nb_noeuds;
};

//////////////////////////////////////////////////////////////////////////////
class _WriterK2KGibi : public _Writer {
public :
  _WriterK2KGibi(_MaillageInterne* mesh, 
               const IN_STD string& nom_fichier, 
               format_t format=SUFFIXE);

  virtual ~_WriterK2KGibi();
  void write(format_t format);
  void prepareListes
  (
   k2k_lobject_t* lobject, //!< liste des objets gibi 
   k2k_lname_t *lname,    //!< liste des objets nommes
   int nb_node_named      //!< nombre de noeuds nommes
   );

private:
  void ecrire(File* fd, format_t format=SUFFIXE);
};


END_NAMESPACE_LIMA

#endif // _READER_K2KGIBI_H
#endif
