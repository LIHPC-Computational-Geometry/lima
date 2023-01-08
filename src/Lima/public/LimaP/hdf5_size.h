/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : hdf5_size.h
 Cree le : 13-06-01
 Auteur  : O. Bertrand
******************************************************************************/

/*!
  \file hdf5_size.h
  \brief Definition des tailles des objets d'un fichier MALIPP
*/

#ifndef _HDF5_SIZE_H
#define _HDF5_SIZE_H

BEGIN_NAMESPACE_LIMA


const int NODE_CHUNK_SIZE   = 1024;
const int EDGE_CHUNK_SIZE   = 1024;
const int CELL2D_CHUNK_SIZE = 1024;
const int CELL3D_CHUNK_SIZE = 1024;

/* CP : Suppression de cette indirection qui pose probleme sur Bauxite
(SGI Irix 6.5) avec les versions 1.4.2-patch1 et 1.4.3 de HDF en utilisant
l'API C (cc a l'edition des liens).
const int ID_TYPE  = H5T_NATIVE_UINT;
const int IND_TYPE = H5T_NATIVE_UINT;
*/

END_NAMESPACE_LIMA

#endif /* _HDF5_SIZE_H */
