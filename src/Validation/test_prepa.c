#include <string.h>
#include <stdio.h>

#include "lima.h"


int main(int argc, char** argv)
{
  int ier ;
  LM_MAILLAGE mai ;
  LM_TYPEMASQUE prepa ;

  prepa = LM_POLYGONES_POLYEDRES | LM_RAZ_NOEUDS_POLYEDRES | LM_COMPACTE | LM_ORIENTATION;

  if(argc<2){
    fprintf(stderr, "Il manque le nom du fichier en argument\n") ;
    exit(-1) ;
  }

  ier = LM_lire_maillage(LM_FF_SUFFIXE, argv[1], &mai) ; 

  if(ier != 0)
    goto erreur ;
  printf("\nMaillage lu \n") ;


  ier = LM_preparer_maillage(&mai, prepa) ;

  if(ier != 0)
    goto erreur ;
  printf("\nMaillage lu : preparation OK\n\n") ;

  return 0 ;


 erreur:
  LM_fermer_maillage(&mai) ;
  return ier ;
}

