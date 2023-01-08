#ifndef _UTIL_H
#define _UTIL_H

#include <Lima/lima.h>
#include "LimaP/config_it.h"

BEGIN_NAMESPACE_LIMA
  
class _MaillageInterne;
  
  /* Allocaton de la structure Mai initialisation et renvoie de la reference.*/
_MaillageInterne* ouvrir_maillage(int_type* refmai);
  
/* renvoie l'adresse du maillage correspondant */
_MaillageInterne* trouver_maillage(int_type ref);
  
/* Supprime l'adresse du maillage. 	
   * Eventuellement le tableau PMAI. */
void fermer_maillage(int_type refmai);

END_NAMESPACE_LIMA

#endif
