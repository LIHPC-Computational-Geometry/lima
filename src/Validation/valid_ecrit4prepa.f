      PROGRAM construction_maillage
c
      IMPLICIT NONE
c
c     Inclusion du fichier de parametre equivalent a un enum en C
      INCLUDE '../src/param_apif.h'
c  
c     Parametres   
      INTEGER noemax, brasmax, polygomax, polyedmax
      PARAMETER (noemax=40, brasmax=20, polygomax=20, polyedmax= 20)
c
c     Declarations
      INTEGER refmai, ier
      INTEGER nbnoe, nbbras, nbpg, nbpd
      INTEGER nb_for
      DOUBLE PRECISION unit_long
      INTEGER noeori(brasmax), noeext(brasmax)
      INTEGER lmano_pg(4*polygomax), pmano_pg(polygomax)
      INTEGER lmano_pd(8*polyedmax), pmano_pd(polyedmax)
      DOUBLE PRECISION x(noemax), y(noemax), z(noemax)
c
c     Initialisations
c
      DATA x / 0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 0.0, 1.0, 2.0,
     +         0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 22*0.0 /
      DATA y / 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 
     +         0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 22*0.0 /
      DATA z / 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
     +         1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 22*0.0 /     
c
      DATA noeori / 11, 13, 4, 9, 16*0 /
      DATA noeext / 14, 16, 7, 6, 16*0 /
c
      DATA lmano_pg / 2 ,  1, 10, 13, 5,  8, 15, 14,
     +                17, 18, 15, 14, 4, 11, 12,  7, 64*0 /
      DATA pmano_pg / 1, 5, 9, 13, 17, 15*0 /
c
      DATA lmano_pd / 1, 2, 5, 4, 10, 13, 14, 11,
     +                2, 3, 6, 5, 13, 16, 17, 14,
     +                7, 8, 5, 4, 12, 15, 14, 11, 136*0 /
      DATA pmano_pd / 1, 9, 17, 25, 16*0 /
c
      refmai=1
      unit_long=1.0
c
      nbnoe=18
      nbbras=4
      nbpg=4
      nbpd=3
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
c     Ecriture du maillage dans un fichier
      CALL lmecmi("maillage_test.unf", refmai, ier)
c     
      END
