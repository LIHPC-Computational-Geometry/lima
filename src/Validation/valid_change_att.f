      PROGRAM modification_attribut
c
      IMPLICIT NONE
c
c     Inclusion du fichier de parametre equivalent a un enum en C
      INCLUDE '../include/Lima/param_apif.h'
c
c     Parametres   
      INTEGER noemax, polyedmax
      PARAMETER (noemax=40, polyedmax=20)
c
c     Declarations
      INTEGER refmai, ier
      INTEGER typcoo, dimens, geomet, struct
      CHARACTER*20 datmod
      REAL unlosi
c
      INTEGER new_att_pol_vol(polyedmax)
      CHARACTER*10 new_att_pol_ch(polyedmax)
      CHARACTER*10 new_att_noe(noemax)
c
c     Initialisations
      DATA new_att_pol_vol / 1, 19*0 /  
c
      DATA new_att_pol_ch / "P1", "P2", "P3", "P4", 16*"_" / 
c
      DATA new_att_noe / "point", 39*"_" /
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
c     Modification de l'attribut de type chaine de caracteres associe aux
c     noeuds du maillage
      CALL lmmoatch(refmai, NOEUD, "TOTOL", "numero", new_att_noe,
     +              1, ier)
c
c     Suppression de l'attribut 'volume' associe aux polyedres du volume
      CALL lmdeat(refmai, POLYEDRE_VOLUME, "VOLUME_TOTAL", "volume",
     +            ier)
c    
c     Modification de l'attribut de type entier associe aux polyedres
c     du volume
      CALL lmmoaten(refmai, POLYEDRE_VOLUME, "VOLUME_TOTAL", "numero",
     +              new_att_pol_vol, 1, ier)
c
c     Modification de l'attribut de type chaine de caracteres associe aux polyedres
c     du volume
      CALL lmmoatch(refmai, POLYEDRE_VOLUME, "VOLUME_TOTAL",
     +              "type_elem", new_att_pol_ch, 3, ier)
c
c     Ecriture du maillage dans un fichier
      CALL lmecmi("maillage_change.mli", refmai, ier)
c     
c     Fin lecture maillage
      CALL lmfin(refmai, ier)
c
      END
