#ifdef __INTERNE_GIBI
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_k2kcastem.h
 Cree le : Wed Dec 20 14:35:58 MET 2000
 Auteur  : JM Bellard
******************************************************************************/

/*!
  \file reader_k2kcastem.h
  \brief Interface de lecture / ecriture pour les fichiers castem
*/

#ifndef _READER_K2KCASTEM_H
#define _READER_K2KCASTEM_H

#include "reader_k2k.h"
#include "reader_k2kgibi.h"
#include "writer.h"

BEGIN_NAMESPACE_LIMA

#if !defined(INT_4)
#define INT_4 int
#endif     

#if !defined(FLOAT_4)
#define FLOAT_4 float
#endif     


class _ReaderK2KCastem : public _ReaderK2K {

public:
  virtual void readStructure();

  virtual void readAll();


  _ReaderK2KCastem(_MaillageInterne* mesh, const IN_STD string& nom_fichier, format_t format=SUFFIXE);
  
  virtual ~_ReaderK2KCastem();

  //! Descripteur 4 : information generale sur le maillage
  typedef struct k_info_mesh_t{
	k_info_mesh_t ( );	// CP, v 6.27.1, coverity
    INT_4 NIVEAU;
    INT_4 IERR;
    INT_4 JDIM;
    FLOAT_4 DENSITE;
  }k_info_mesh_t;

  //! Descripteur 2 : information sur une pile
  typedef struct k_info_pile_t{
	k_info_pile_t ( );	// CP, v 6.27.1, coverity
    INT_4 IPIL;
    INT_4 NOBJN;
    INT_4 NOBJ;
  }k_info_pile_t;

  //
  typedef struct k_objn_t {
	k_objn_t ( );	// CP, v 6.27.1, coverity
    int nbr;
    char* NOM;
    INT_4*  IP;
  }k_objn_t;

  typedef struct k_lnode_t {
	k_lnode_t ( );	// CP, v 6.27.1, coverity
    int nbr;
    int *id;
    union {
      double* dir[3];// coo.d[dirx][in]
      struct {
        double* x;
        double* y;
        double* z;
      }n;
    } coo;
  } k_lnode_t;

  typedef struct k_meleme_t {
	k_meleme_t ( );	// CP, v 6.27.1, coverity
    INT_4 ITYPEL;
    INT_4 NBSOUS;
    INT_4 NBREF;
    INT_4 NBNN;
    INT_4 NBELEM;
    INT_4* LISOUS;
    INT_4* LISREF;
    INT_4* ICOLOR;
    INT_4* NUM;
  } k_meleme_t;

  typedef struct k_pile1_t{
	k_pile1_t ( );	// CP, v 6.27.1, coverity
    INT_4 nbr;
    k2k_object_t* elm;
  }k_pile1_t;

  typedef struct k_pile32_t {
	k_pile32_t ( );	// CP, v 6.27.1, coverity
    INT_4 nbr;
    INT_4 *pt2nod;
  }k_pile32_t;


private:

  int readInt(File* fd, int nbval, int nbchar, int bynb, int*tab);
  int readInt(File* fd, int nbval, int nbchar, int bynb, size_type*tab);

  void lire( );
  
  int readNextEnr(File* fd);
  void readInfoMesh(File* fd, k_info_mesh_t& info);
  void readInfoPile(File* fd, int level, k_info_pile_t& pile);
  void readPileObjectNamed(File* fd, k2k_lname_t& lname);
  void readNode(File* fd, int dim, k2k_lnode_t& lnode);
  void readPile33(File* fd, int dim, k2k_lnode_t& lnode);
  void readPile1(File* fd, k2k_lobject_t& pile);
  int ObjectType(int id, int nbn);
  void readObject(File* fd, k2k_object_t* obj);
  void addNode(k2k_lnode_t& lnode, k2k_lname_t& node_name,
               k_pile32_t&  pile32,k2k_lname_t& pile32_name);
  void prepareObject(k2k_lobject_t& object,
                     k_pile32_t&  pile32);
  void addGroup(char *name, k2k_object_t* obj);
  void addNuage(_NuageInterne* g, k2k_object_t* obj);
  void addLigne(_LigneInterne* g, k2k_object_t* obj);
  void addSurface(_SurfaceInterne* g, k2k_object_t* obj);
  void addVolume(_VolumeInterne* g, k2k_object_t* obj);
  void addObject(k2k_lobject_t object, k2k_lname_t object_name);
  void readPile32(File* fd, k_pile32_t& pile);


};


//////////////////////////////////////////////////////////////////////////////
class _WriterK2KCastem : public _Writer {
public :
  _WriterK2KCastem(_MaillageInterne* mesh, 
               const IN_STD string& nom_fichier, 
               format_t format=SUFFIXE);

  virtual ~_WriterK2KCastem();
  void write(format_t format);
private:
  void ecrire(File* fd);
  void writePileObjectNamed(File* fd, k2k_lname_t& lname);


};

END_NAMESPACE_LIMA

#endif // _READER_K2KCASTEM_H
#endif
