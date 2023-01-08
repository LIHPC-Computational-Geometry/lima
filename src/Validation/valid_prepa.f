      PROGRAM prepaf
c
      IMPLICIT NONE
c
c     Inclusion du fichier de parametre equivalent a un enum en C
      INCLUDE '../include/Lima/param_apif.h'
c  
c     Parametres   
      INTEGER pmax
      PARAMETER (pmax=4)
c
c     Declarations
      INTEGER refmai, ier
      INTEGER typcoo, dimens, geomet, struct
      INTEGER nb_prepa
      INTEGER preparation(pmax)
      CHARACTER*20 datmod
      REAL unlosi
c
c     Construction de la preparation
      DATA preparation / LM_POLYGONES_POLYEDRES ,
     +                   LM_RAZ_NOEUDS_POLYEDRES,
     +                   LM_COMPACTE            ,
     +                   LM_ORIENTATION         /
c 
c
c
c
c     LE CORPS DU PROGRAMME
c
c
c     Lecture maillage
c     Le nom du fichier se trouve dans la variable d'environnement FICMAIL
      CALL lmleve('FICMAI', refmai, datmod , unlosi, typcoo, dimens,
     +             geomet, struct, ier)
c
c     Traitement de la preparation
      nb_prepa = 4
      CALL lmpretb(refmai, preparation, nb_prepa, ier)
c
c     Fin lecture maillage
      CALL lmfin(refmai, ier)
c
      END
