#ifdef __INTERNE_MALIPP2
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : malipp2_factory.cpp
 Cree le : 06/06/2019
 Derniere modification le :
 Auteur  : C. Pignerol
******************************************************************************/

/*!
  \file malipp2_factory.C
  \brief Interface d'instanciation de lecteur/écrivain de fichiers
         MALI++2 (.mli2).
*/

/* REMARQUES :
   -----------

   L'objectif est ici d'isoler l'API de chaque format de fichier reposant sur
   HDF afin d'éviter des interférences au niveau des fichiers HDF inclus ...
*/

#include "LimaP/reader_malipp2.h"


using namespace std;


BEGIN_NAMESPACE_LIMA


_Reader* createMalipp2Reader (
	_MaillageInterne* mesh, const string& nom_fichier, format_t format, size_type num)
{
	return new _ReaderMalipp2 (mesh, nom_fichier, format, num);
}


_Writer* createMalipp2Writer (
	_MaillageInterne* mesh, const string& nom_fichier, format_t format, size_type num)
{
	return new _WriterMalipp2 (mesh, nom_fichier, format, num);
}


END_NAMESPACE_LIMA

#endif	// __INTERNE_MALIPP2
