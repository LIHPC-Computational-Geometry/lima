      PROGRAM relecture_maillage_en_fortran
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
      INTEGER typcoo, dimens, geomet, struct
      CHARACTER*20 datmod
      REAL unlosi
c
      CHARACTER*20 listenom1(10), listenom2(10)
      INTEGER listetype1(10), listetype2(10)
c
      INTEGER A, B, C, I, J, AB, BC
c
      INTEGER nbatt1, nbatt2, nb_triplet, nb_ent, nb_reel
      INTEGER nbatt_noe, nbatt_pol
c
      INTEGER tab_ent(polyedmax)
      REAL    tab_reel(polyedmax)
      DOUBLE PRECISION tab_coor3d(3*noemax)
      CHARACTER*10 tab_ch_noe(noemax)
      CHARACTER*10 tab_ch_pol(polyedmax)
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
c     Lecture de la liste des attributs associes aux noeuds
      CALL lmatls(refmai, NOEUD, "toto", listenom1,
     +            listetype1, nbatt1, ier)
c
      IF (nbatt1.EQ.1) THEN
         PRINT*,'Il n''y a qu''un seul attribut associe aux noeuds.'
         PRINT*,'Cet attribut est : ', listenom1(1)
      ELSE IF (nbatt1.GT.1) THEN
         PRINT*,'Il y a ', nbatt1 ,' attributs associes aux noeuds.'
         PRINT*,'Ces attributs sont :'
         DO I = 1,nbatt1
            PRINT*,'--> L''attribut ', I ,' est : ', listenom1(I)
         ENDDO
      ENDIF
c
c     Lecture de l'attribut de type chaine de caracteres associe
c     aux noeuds
c     Cet attribut est : 'numero'
      CALL lmatch(refmai, NOEUD, "toto", "numero", 
     +            tab_ch_noe, nbatt_noe, ier)
c
      PRINT*,'--> L''attribut de type chaine : ''numero'' ...'
      IF (nbatt_noe.EQ.1) THEN
         PRINT*,'... est a valeur constante : ', tab_ch_noe(1)
      ELSE IF (nbatt_noe.GT.1) THEN
         PRINT*,'... possede ', nbatt_noe ,' valeurs qui sont :'
         DO AB = 1,nbatt_noe
            PRINT*,tab_ch_noe(AB)
         ENDDO
      ENDIF
c
c     Lecture de l'attribut de type vecteur3d associe aux noeuds
c     Cet attribut est : 'coordonnees'
      CALL lmatv3(refmai, NOEUD, "toto", "coordonnees", 
     +            tab_coor3d, nb_triplet, ier)
c
      PRINT*,'--> L''attribut de type vecteur3d : ''coordonnees'' ...'
      IF (nb_triplet.EQ.1) THEN
         PRINT*,'... est a valeur constante : [', tab_coor3d(1) ,' ; ',
     +        tab_coor3d(2) ,' ; ', tab_coor3d(3) ,']'
      ELSE IF (nb_triplet.GT.1) THEN
         PRINT*,'... possede ', nb_triplet ,' valeurs qui sont :'
         DO A = 1,nb_triplet
            PRINT*,'[', tab_coor3d((3*A)-2) ,' ; ', tab_coor3d((3*A)-1)
     +           ,' ; ', tab_coor3d(3*A) ,']'
         ENDDO
      ENDIF
c
c
c     Lecture de la liste des attributs associes aux polyedres du volume
      CALL lmatls(refmai, POLYEDRE_VOLUME, "VOLUME_TOTAL", listenom2,
     +            listetype2, nbatt2, ier)
c
      IF (nbatt2.EQ.1) THEN
         PRINT*,'Il n''y a qu''un seul attribut associe aux polyedres du
     +        volume.'
         PRINT*,'Cet attribut est : ', listenom2(1)
      ELSE IF (nbatt2.GT.1) THEN
         PRINT*,'Il y a ', nbatt2 ,' attributs associes aux polyedres du
     +        volume.'
         PRINT*,'Ces attributs sont :'
         DO J = 1,nbatt2
            PRINT*,'--> L''attribut ', J ,' est : ', listenom2(J)
         ENDDO
      ENDIF
c
c     Lecture de l'attribut de type entier associe aux polyedres du volume
c     Cet attribut est : 'numero'
      CALL lmaten(refmai, POLYEDRE_VOLUME, "VOLUME_TOTAL", "numero", 
     +            tab_ent, nb_ent, ier)
c
      PRINT*,'--> L''attribut de type entier: ''numero'' ...'
      IF (nb_ent.EQ.1) THEN
         PRINT*,'... est a valeur constante : ', tab_ent(1)
      ELSE IF (nb_ent.GT.1) THEN
         PRINT*,'... possede ', nb_ent,' valeurs qui sont :'
         DO B = 1,nb_ent
            PRINT*,' *** ', tab_ent(B)
         ENDDO
      ENDIF
c
c     Lecture de l'attribut de type reel associe aux polyedres du volume
c     Cet attribut est : 'volume'
      CALL lmatre(refmai, POLYEDRE_VOLUME, "VOLUME_TOTAL", "volume", 
     +            tab_reel, nb_reel, ier)
c
      PRINT*,'--> L''attribut de type reel: ''volume'' ...'
      IF (nb_reel.EQ.1) THEN
         PRINT*,'... est a valeur constante : ', tab_reel(1)
      ELSE IF (nb_reel.GT.1) THEN
         PRINT*,'... possede ', nb_reel,' valeurs qui sont :'
         DO C = 1,nb_reel
            PRINT*,' *** ', tab_reel(C)
         ENDDO
      ENDIF
c
c     Lecture de l'attribut de type chaine de caracteres associe
c     aux polyedres du volume
c     Cet attribut est : 'type_elem'
      CALL lmatch(refmai, POLYEDRE_VOLUME, "VOLUME_TOTAL", "type_elem", 
     +            tab_ch_pol, nbatt_pol, ier)
c
      PRINT*,'--> L''attribut de type chaine : ''type_elem'' ...'
      IF (nbatt_pol.EQ.1) THEN
         PRINT*,'... est a valeur constante : ', tab_ch_pol(1)
      ELSE IF (nbatt_pol.GT.1) THEN
         PRINT*,'... possede ', nbatt_pol ,' valeurs qui sont :'
         DO BC = 1,nbatt_pol
            PRINT*,tab_ch_pol(BC)
         ENDDO
      ENDIF
c
c     Fin lecture maillage
      CALL lmfin(refmai, ier)
c
      END
