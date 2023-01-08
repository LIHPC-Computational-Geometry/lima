#ifdef __INTERNE_MALIPP2
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : malipp2_factory.h
 Cree le : 06/06/2019
 Derniere modification le :
 Auteur  : C. Pignerol
******************************************************************************/

/*!
  \file malipp2_factory.h
  \brief Interface d'instanciation de lecteur/écrivain de fichiers
         MALI++2 (.mli2).
*/

#ifndef _MALIPP2_FACTORY_H
#define _MALIPP2_FACTORY_H

#include "reader.h"
#include "writer.h"


BEGIN_NAMESPACE_LIMA


_Reader* createMalipp2Reader (_MaillageInterne* mesh, const std::string& filename, format_t format, size_type num);
_Writer* createMalipp2Writer (_MaillageInterne* mesh, const std::string& filename, format_t format, size_type num);


END_NAMESPACE_LIMA

#endif // _MALIPP2_FACTORY_H
#endif
