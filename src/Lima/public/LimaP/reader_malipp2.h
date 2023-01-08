#ifdef __INTERNE_MALIPP2
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_malipp2.h
 Cree le : 05/06/2019
 Derniere modification le :
 Auteur  : C. Pignerol
******************************************************************************/

/*!
  \file reader_malipp2.h
  \brief Interface de lecture / ecriture pour les fichiers MALI++2 (.mli2)
*/

#ifndef _READER_MALIPP2_H
#define _READER_MALIPP2_H

#include "reader.h"
#include "writer.h"
#include "malipp_common.h"
#include <Lima/malipp2.h>
#include "hdf5_size.h"

#define IN_H5 H5::

BEGIN_NAMESPACE_LIMA


class _ReaderMalipp2 : public _Reader {

public:

  virtual void readStructure();

  //! Lecture d'un attribut du maillage
  virtual void readAttr (MaliPPReader2::ENTITE,
					Liste_attribut& liste, const HDFAttributeDescriptor2& ad);
  //! Lecture d'un attribut d'un groupe du maillage
  virtual void readGroupAttr (
					MaliPPReader2::GroupeReader& reader, MaliPPReader2::ENTITE,
					Liste_attribut& liste, const HDFAttributeDescriptor2& ad);
  virtual void readAll();


  _ReaderMalipp2(_MaillageInterne* mesh, const IN_STD string& nom_fichier, 
                format_t format=SUFFIXE, size_type num=1);
  
  virtual ~_ReaderMalipp2();


private:
  //! Lecture d'un fichier Malipp.
  void          lire (bool loadData);

  //! Lecture des donnees sur les noeuds
  void          readNode (bool loadData);
  //! Lecture des donnees sur les bras
  void          readEdge (bool loadData);
  //! Lecture des donnees sur les polygones
  void          readCell2D (bool loadData);
  //! Lecture des donnees sur les polyedres
  void          readCell3D (bool loadData);
  //! Lecture des donnees sur les nuages du maillage
  void          readNodeset (bool loadData);
  //! Lecture des donnees sur les lignes du maillage
  void          readLine (bool loadData);
  //! Lecture des donnees sur les surfaces du maillage
  void          readSurface (bool loadData);
  //! Lecture des donnees sur les volumes du maillage
  void          readVolume (bool loadData);
  //! Lecture des attributs du maillage.
  void			readMeshAttr ( );

	//! Le nom du fichier.
	IN_STD string						m_fileName;

	//! Le numéro du maillage.
	size_type							m_meshNum;

	//! Le lecteur du fichier.
	MaliPPReader2						m_reader;
};

//////////////////////////////////////////////////////////////////////////////
class _WriterMalipp2 : public _Writer {
public :
  _WriterMalipp2(_MaillageInterne* mesh, 
               const IN_STD string& nom_fichier, 
               format_t format=SUFFIXE, size_type num=1);

  virtual ~_WriterMalipp2 ( );
  void write (format_t format);

private:
  void ecrire ( );

  /////////// HDF5
  //! Creation du header d'un fichier
  void         createMainHeader ( );
  //! Ecriture des informations relatives aux versions (Lima/Format mli)
  void         writeVersionsInfos (IN_H5 H5Object&);
  //! incremente le nombre de maillage du fichier.
  // Rem : les identifiants ne sont pas forcement successifs.
  void         nbMeshInc ( );
  //! Ecriture de commentaires sur le fichier
//  void         setComment(hid_t root,const IN_STD string& comment);
  //! Ecriture de l'indice des elements dans le champ udata
  void         setDataToIndice ( );
  //! Mise a NULL du champ udata
  void         setDataToNull();
  //! Creation du header d'un maillage
  void         writeMeshHeader ( );
  //! Creation d'un maillage
  void         createMesh ( );
  //! Creation du squelette d'un maillage .mli
  void         createMeshDataGroup ( );
  //! Ecriture des donnees sur les noeuds
  void         writeNode ( );
  //! Ecriture des attributs des noeuds du maillage
  void         writeAttr (Liste_attribut* liste, IN_H5 Group& root,
                          const IN_STD string& name);
  //! Ecriture des donnees sur les bras
  void         writeEdge ( );
  //! Ecriture des donnees sur les polygones
  void         writeCell2D ( );
  //! Ecriture des donnees sur les polyedres
  void         writeCell3D ( );
  //! Ecriture des donnees sur les nuages du maillage
  void         writeNodeset ( );
  //! Ecriture des donnees sur les lignes du maillage
  void         writeLine ( );
  //! Ecriture des donnees sur les surfaces du maillage
  void         writeSurface ( );
  //! Ecriture des donnees sur les volumes du maillage
  void         writeVolume ( );
  //! Ecriture de la composition d'un groupe de3 données.
  void         writeComposition (const Composition& composition, 
                                 const IN_STD string& name, 
                                 IN_H5 Group& root);

	//! Le nom du fichier.
	IN_STD string						m_fileName;

	//! Le numéro du maillage.
	size_type							m_meshNum;

	//! Le groupe contenant le maillage.
	IN_H5 Group							m_meshGroup;

	//! Le fichier HDF contenant le maillage.
	IN_STD auto_ptr<IN_H5 H5File>		m_hdfFile;
};


END_NAMESPACE_LIMA

#endif // _READER_MALIPP2_H
#endif
