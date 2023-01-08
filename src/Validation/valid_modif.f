      PROGRAM modification_maillage
c
      IMPLICIT NONE
c
c     Inclusion du fichier de parametre equivalent a un enum en C
      INCLUDE '../include/Lima/param_apif.h'
c
c     Declarations
      INTEGER refmai, ier
      INTEGER typcoo, dimens, geomet, struct
      CHARACTER*20 datmod
      REAL unlosi
c
      INTEGER nbmai_2d, idpg
      INTEGER lmano_new_pg(4), pmano_new_pg(2)
c
      INTEGER idpd
c
c     Initialisations
      DATA lmano_new_pg / 2, 3, 4, 9 /
      DATA pmano_new_pg / 1, 5 /
      nbmai_2d = 1
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
c     Ajout du noeud 10 au nuage 'bas'
      CALL lmajno(refmai, "BAS", 10, ier)
c
c     Destruction du nuage 'haut'
      CALL lmdenu(refmai, "HAUT", ier)
c
c     Creation du polygone 2-3-4-9
      CALL lmcrma(refmai, lmano_new_pg, pmano_new_pg, nbmai_2d, ier)
c
c     Ajout de ce polygone dans la surface 'avant'
      idpg = 5
      CALL lmajpg(refmai, "AVANT", idpg, ier)
c
c     Retrait du polygone 1-2-11-10 (id=1) de la surface 'avant'
      idpg = 1
      CALL lmrepg(refmai, "AVANT", idpg, ier)
c
c     Retrait du polyedre 8-9-6-7-17-18-15-16 (id=3) du volume
      idpd = 3
      CALL lm3repd(refmai, "VOLUME_TOTAL", idpd, ier)
c
c     Destruction de ce polyedre
      CALL lm3dema(refmai, idpd, ier)
c
c     Ecriture du maillage dans un fichier
      CALL lmecmi("maillage_modif.mli", refmai, ier)
c     
c     Fin lecture maillage
      CALL lmfin(refmai, ier)
c
      END
