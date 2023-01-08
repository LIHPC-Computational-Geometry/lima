      PROGRAM construction_maillage
c
      IMPLICIT NONE
c
c     Inclusion du fichier de parametre equivalent a un enum en C
      INCLUDE '../include/Lima/param_apif.h'
c  
c     Parametres   
      INTEGER noemax, brasmax, polygomax, polyedmax
      PARAMETER (noemax=40, brasmax=20, polygomax=20, polyedmax= 20)
c
c     Declarations
      INTEGER refmai, ier
      INTEGER nbnoe, nbbras, nbpg, nbpd
      INTEGER nbnoe_nuage1, nbnoe_nuage2
      INTEGER nbbras_ligne1, nbbras_ligne2
      INTEGER nbpg_surface1, nbpg_surface2
      INTEGER nbpd_volume
      DOUBLE PRECISION unit_long
      INTEGER noeori(brasmax), noeext(brasmax)
      INTEGER lmano_pg(4*polygomax), pmano_pg(polygomax)
      INTEGER lmano_pd(8*polyedmax), pmano_pd(polyedmax)
      INTEGER idnoe_nuage1(noemax), idnoe_nuage2(noemax)
      INTEGER idbr_ligne1(2*brasmax), idbr_ligne2(2*brasmax)
      INTEGER idpg_surface1(2*polygomax), idpg_surface2(2*polygomax)
      INTEGER idpd_volume(polyedmax)
      DOUBLE PRECISION x(noemax), y(noemax), z(noemax)
      CHARACTER*10 att_noe(noemax)
      INTEGER att_pol_vol(polyedmax)
      REAL valatt
      DOUBLE PRECISION tab_coor3d(3*noemax)
c
c     Initialisations
      DATA x / 0.0, 1.0, 2.0, 2.0, 2.0, 1.0, 0.0, 0.0, 1.0,
     +         0.0, 1.0, 2.0, 2.0, 2.0, 1.0, 0.0, 0.0, 1.0, 22*0.0 /
      DATA y / 0.0, 0.0, 0.0, 1.0, 2.0, 2.0, 2.0, 1.0, 1.0, 
     +         0.0, 0.0, 0.0, 1.0, 2.0, 2.0, 2.0, 1.0, 1.0, 22*0.0 /
      DATA z / 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
     +         1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 22*0.0 /
c
      DATA noeori / 17, 18, 11, 18, 16*0 /
      DATA noeext / 18, 13, 18, 15, 16*0 /
c
      DATA lmano_pg / 1, 2, 11, 10, 2, 3, 12, 11,
     +                7, 6, 15, 16, 6, 5, 14, 15, 64*0 /
      DATA pmano_pg / 1, 5, 9, 13, 17, 15*0 /
c
      DATA lmano_pd / 1, 2, 9, 8, 10, 11, 18, 17,
     +                2, 3, 4, 9, 11, 12, 13, 18,
     +                8, 9, 6, 7, 17, 18, 15, 16,
     +                9, 4, 5, 6, 18, 13, 14, 15, 128*0 /
      DATA pmano_pd / 1, 9, 17, 25, 33, 15*0 /
c
      DATA idnoe_nuage1 / 1, 2, 3, 4, 5, 6, 7, 8, 9, 31*0 /
      DATA idnoe_nuage2 / 10, 11, 12, 13, 14, 15, 16, 17, 18, 31*0 /
c
      DATA idbr_ligne1 / 1, 2, 38*0 /
      DATA idbr_ligne2 / 3, 4, 38*0 /
c
      DATA idpg_surface1 /  1, 2, 38*0 /
      DATA idpg_surface2 /  3, 4, 38*0 /
c
      DATA idpd_volume / 1, 2, 3, 4, 16*0 /
c
      DATA att_noe / "point1" , "point2" , "point3" , "point4" , 
     +               "point5" , "point6" , "point7" , "point8" , 
     +               "point9" , "point10", "point11", "point12", 
     +               "point13", "point14", "point15", "point16",
     +               "point17", "point18", 22*"_" /
c
      DATA att_pol_vol / 1, 2, 3, 4, 16*0 /
c
      DATA valatt / 1.0 /
c
      DATA tab_coor3d / 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 2.0, 0.0, 0.0,
     +                  2.0, 1.0, 0.0, 2.0, 2.0, 0.0, 1.0, 2.0, 0.0,
     +                  0.0, 2.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0,
     +                  0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 2.0, 0.0, 1.0,
     +                  2.0, 1.0, 1.0, 2.0, 2.0, 1.0, 1.0, 2.0, 1.0,
     +                  0.0, 2.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 
     +                  66*0.0 /
c
      refmai=1
      unit_long=1.0
c
      nbnoe=18
      nbbras=4
      nbpg=4
      nbpd=4
c
c
c
c     LE CORPS DU PROGRAMME
c
c
c     Creation du maillage
      CALL lmcrmi(refmai, "maillage_test", "septembre_2001", unit_long,
     +            CARTESIEN, D3, NORMAL, ier)
c
c     Creation des noeuds
      CALL lm3crno(refmai, x, y, z, nbnoe, ier)
c
c     Creation des bras
      CALL lmcrbr(refmai, noeori, noeext, nbbras, ier)
c
c     Creation des polygones (mailles 2D)
      CALL lmcrma(refmai, lmano_pg, pmano_pg, nbpg, ier)
c
c     Creation des polyedres (mailles 3D)
      CALL lm3crma(refmai, lmano_pd, pmano_pd, nbpd, ier)
c
c     Creation de nuages 
      nbnoe_nuage1 = 9
      CALL lmcrnu(refmai, "bas", idnoe_nuage1, nbnoe_nuage1, ier)
      nbnoe_nuage2 = 9
      CALL lmcrnu(refmai, "haut", idnoe_nuage2, nbnoe_nuage2, ier)
c 
c     Creation de lignes
      nbbras_ligne1 = 2
      CALL lmcrli(refmai, "ligne1", idbr_ligne1, nbbras_ligne1, ier)
      nbbras_ligne2 = 2
      CALL lmcrli(refmai, "ligne2", idbr_ligne2, nbbras_ligne2, ier)
c
c     Creation des surfaces
      nbpg_surface1 = 2
      CALL lmcrsu(refmai, "avant", idpg_surface1, nbpg_surface1, ier)
      nbpg_surface2 = 2
      CALL lmcrsu(refmai, "arriere", idpg_surface2, nbpg_surface2, ier)
c  
c     Creation du volume
      nbpd_volume = 4
      CALL lm3crvo(refmai, "volume_total", idpd_volume, nbpd_volume,
     +             ier)
c  
c     Creation des attributs des noeuds
c      CALL lmcratch(refmai, NOEUD, "TOTO", "numero", att_noe, 1,
c     +              ier)
c
c      CALL lmatch(refmai, NOEUD, "TOTO", "numero", tab_chaine,  nb_elt1,
c     +            ier)
c
c      DO I = 1,nb_elt1 
c         PRINT*,tab_chaine(I),' - '
c      ENDDO
c
      CALL lmcratv3(refmai, NOEUD, "TOTO", "coordonnees", tab_coor3d,
     +              nbnoe, ier)
c
c     Creation des attributs des polyedres du volume
      CALL lmcraten(refmai, POLYEDRE_VOLUME, "VOLUME_TOTAL", "numero", 
     +              att_pol_vol, 4, ier)
c
      CALL lmcratre(refmai, POLYEDRE_VOLUME, "VOLUME_TOTAL", "volume", 
     +              valatt, 1, ier)
c
c     Ecriture du maillage dans un fichier
      CALL lmecmi("maillage_valid.mli", refmai, ier)
c     
      END
