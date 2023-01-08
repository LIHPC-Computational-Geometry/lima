c
c      Les points d attache possibles d un attribut
c
       INTEGER MAILLAGE
       INTEGER NOEUD, BRAS, POLYGONE, POLYEDRE
       INTEGER NUAGE, LIGNE, SURFACE, VOLUME
       INTEGER NOEUD_NUAGE, NOEUD_LIGNE, NOEUD_SURFACE, NOEUD_VOLUME
       INTEGER BRAS_LIGNE, BRAS_SURFACE, BRAS_VOLUME
       INTEGER POLYGONE_SURFACE, POLYGONE_VOLUME
       INTEGER POLYEDRE_VOLUME
c
       PARAMETER (MAILLAGE=0)
       PARAMETER (NOEUD=1, BRAS=2, POLYGONE=3, POLYEDRE=4)
       PARAMETER (NUAGE=5, LIGNE=6, SURFACE=7, VOLUME=8)
       PARAMETER (NOEUD_NUAGE=9, NOEUD_LIGNE=10, NOEUD_SURFACE=11)
       PARAMETER (NOEUD_VOLUME=12)
       PARAMETER (BRAS_LIGNE=13, BRAS_SURFACE=14, BRAS_VOLUME=15)
       PARAMETER (POLYGONE_SURFACE=16, POLYGONE_VOLUME=17)
       PARAMETER (POLYEDRE_VOLUME=18)
c
c
c
c
c
c      Les types d attribut possibles
c
       INTEGER ATT_ENTIER, ATT_REEL, ATT_VECT2D, ATT_VECT3D, ATT_CHAINE
c
       PARAMETER (ATT_ENTIER=0, ATT_REEL=1)
       PARAMETER (ATT_VECT2D=2, ATT_VECT3D=3)
       PARAMETER (ATT_CHAINE=4)
c
c
c
c 
c
c /// Developpements novembre 2001
c      Les types de preparation possibles (version enum_C)
c
       INTEGER LM_PR_MINIMALE, LM_PR_MAXIMALE
       INTEGER LM_PR_BASE_3D   , LM_PR_PARTITIONNEMENT_3D, LM_PR_LISSAGE_3D
       INTEGER LM_PR_CODE_B  , LM_PR_CODE_A
       INTEGER LM_PR_MAILLEUR_2D, LM_PR_CODE_T
c
c
c
c
c
c      Les types de preparation possibles (version TypeMasque)
c
c      LES CODES
c
       INTEGER LM_MINIMALE, LM_MAXIMALE
       INTEGER LM_BASE_3D , LM_PARTITIONNEMENT_3D, LM_LISSAGE_3D
       INTEGER LM_CODE_B  , LM_CODE_A
       INTEGER LM_MAILLEUR_2D, LM_CODE_T
c
       PARAMETER (LM_MINIMALE                = 0)  
       PARAMETER (LM_MAXIMALE                = 1)
       PARAMETER (LM_BASE_3D                 = 5)    
       PARAMETER (LM_PARTITIONNEMENT_3D      = 7)
       PARAMETER (LM_LISSAGE_3D              = 8)
       PARAMETER (LM_CODE_B                  = 2)  
       PARAMETER (LM_CODE_A                  = 3) 
       PARAMETER (LM_MAILLEUR_2D             = 4) 
       PARAMETER (LM_CODE_T                  = 6)
c
c      LES PREPARATIONS ELEMENTAIRES
c
       INTEGER LM_NOEUDS_BRAS, LM_NOEUDS_POLYGONES, LM_NOEUDS_POLYEDRES
       INTEGER LM_POLYGONES_BRAS, LM_BRAS_POLYGONES
       INTEGER LM_POLYEDRES_BRAS, LM_BRAS_POLYEDRES
       INTEGER LM_POLYEDRES_POLYGONES, LM_POLYGONES_POLYEDRES
       INTEGER LM_SUPPRIMER_DOUBLONS_NOEUDS
       INTEGER LM_SUPPRIMER_DOUBLONS_BRAS
       INTEGER LM_SUPPRIMER_DOUBLONS_POLYGONES
       INTEGER LM_SUPPRIMER_DOUBLONS_POLYEDRES
       INTEGER LM_LIGNES_NOEUDS
       INTEGER LM_SURFACES_NOEUDS, LM_SURFACES_BRAS
       INTEGER LM_VOLUMES_NOEUDS, LM_VOLUMES_BRAS, LM_VOLUMES_POLYGONES
       INTEGER LM_RAZ_NOEUDS_BRAS, LM_RAZ_NOEUDS_POLYGONES
       INTEGER LM_RAZ_NOEUDS_POLYEDRES
       INTEGER LM_RAZ_POLYGONES_BRAS, LM_RAZ_BRAS_POLYGONES
       INTEGER LM_RAZ_POLYEDRES_BRAS, LM_RAZ_BRAS_POLYEDRES
       INTEGER LM_RAZ_POLYEDRES_POLYGONES, LM_RAZ_POLYGONES_POLYEDRES
       INTEGER LM_CONTOURS_LIGNES, LM_CONTOURS_SURFACES
       INTEGER LM_CONTOURS_VOLUMES
       INTEGER LM_CONTOURS_SURF_NOE
       INTEGER LM_CONTOURS_VOL_NOE, LM_CONTOURS_VOL_BR
       INTEGER LM_STRUCTURER_LIGNES, LM_STRUCTURER_SURFACES
       INTEGER LM_STRUCT_CONTOURS_SURF
       INTEGER LM_COMPACTE, LM_ORIENTATION
       INTEGER LM_ORIENT_CONTOURS_SURF, LM_FORMATER_NOMS
C
       PARAMETER (LM_NOEUDS_BRAS             = 9) 
       PARAMETER (LM_NOEUDS_POLYGONES        = 10)
       PARAMETER (LM_NOEUDS_POLYEDRES        = 11)
       PARAMETER (LM_POLYGONES_BRAS          = 12)
       PARAMETER (LM_BRAS_POLYGONES          = 13)
       PARAMETER (LM_POLYEDRES_BRAS          = 14)
       PARAMETER (LM_BRAS_POLYEDRES          = 15)
       PARAMETER (LM_POLYEDRES_POLYGONES     = 16)
       PARAMETER (LM_POLYGONES_POLYEDRES     = 17)
       PARAMETER (LM_SUPPRIMER_DOUBLONS_NOEUDS    = 49)
       PARAMETER (LM_SUPPRIMER_DOUBLONS_BRAS      = 18)
       PARAMETER (LM_SUPPRIMER_DOUBLONS_POLYGONES = 19)
       PARAMETER (LM_SUPPRIMER_DOUBLONS_POLYEDRES = 20)
       PARAMETER (LM_LIGNES_NOEUDS           = 21)
       PARAMETER (LM_SURFACES_NOEUDS         = 22)
       PARAMETER (LM_SURFACES_BRAS           = 23)
       PARAMETER (LM_VOLUMES_NOEUDS          = 24)
       PARAMETER (LM_VOLUMES_BRAS            = 25)
       PARAMETER (LM_VOLUMES_POLYGONES       = 26)
       PARAMETER (LM_RAZ_NOEUDS_BRAS         = 27)
       PARAMETER (LM_RAZ_NOEUDS_POLYGONES    = 28)
       PARAMETER (LM_RAZ_NOEUDS_POLYEDRES    = 29)
       PARAMETER (LM_RAZ_POLYGONES_BRAS      = 30)
       PARAMETER (LM_RAZ_BRAS_POLYGONES      = 31)
       PARAMETER (LM_RAZ_POLYEDRES_BRAS      = 32)
       PARAMETER (LM_RAZ_BRAS_POLYEDRES      = 33)
       PARAMETER (LM_RAZ_POLYEDRES_POLYGONES = 34)
       PARAMETER (LM_RAZ_POLYGONES_POLYEDRES = 35)
       PARAMETER (LM_CONTOURS_LIGNES         = 36)
       PARAMETER (LM_CONTOURS_SURFACES       = 37)
       PARAMETER (LM_CONTOURS_VOLUMES        = 38)
       PARAMETER (LM_CONTOURS_SURF_NOE       = 39)
       PARAMETER (LM_CONTOURS_VOL_NOE        = 40)
       PARAMETER (LM_CONTOURS_VOL_BR         = 41)
       PARAMETER (LM_STRUCTURER_LIGNES       = 42)
       PARAMETER (LM_STRUCTURER_SURFACES     = 43)
       PARAMETER (LM_STRUCT_CONTOURS_SURF    = 44)
       PARAMETER (LM_COMPACTE                = 45)
       PARAMETER (LM_ORIENTATION             = 46)
       PARAMETER (LM_ORIENT_CONTOURS_SURF    = 47)
       PARAMETER (LM_FORMATER_NOMS           = 48)
c
c      LES VERIFICATIONS ELEMENTAIRES
c
       INTEGER LM_V_BRAS
       INTEGER LM_V_BRAS_NON_NULS
       INTEGER LM_V_ORIENTATION_BRAS
       INTEGER LM_V_CONNEXIONS_POLYGONES_BRAS
       INTEGER LM_V_STRUCTURER_LIGNES
       INTEGER LM_V_ORIENTATION_LIGNES_FERMEES
       INTEGER LM_V_ORIENTATION_POLYGONES
       INTEGER LM_V_CONTOURS_SURFACES
       INTEGER LM_V_STRUCT_CONTOURS_SURF
       INTEGER LM_V_ORIENT_CONTOURS_SURF 
       INTEGER LM_V_COMPACTE
C
       PARAMETER (LM_V_BRAS                       = 0) 
       PARAMETER (LM_V_BRAS_NON_NULS              = 1)
       PARAMETER (LM_V_ORIENTATION_BRAS           = 2)
       PARAMETER (LM_V_CONNEXIONS_POLYGONES_BRAS  = 3)
       PARAMETER (LM_V_STRUCTURER_LIGNES          = 4)
       PARAMETER (LM_V_ORIENTATION_LIGNES_FERMEES = 5)
       PARAMETER (LM_V_ORIENTATION_POLYGONES      = 6)
       PARAMETER (LM_V_CONTOURS_SURFACES          = 7)
       PARAMETER (LM_V_STRUCT_CONTOURS_SURF       = 8)
       PARAMETER (LM_V_ORIENT_CONTOURS_SURF       = 9)
       PARAMETER (LM_V_COMPACTE                   = 10)
C
c
c
c
c
c      Les types de preparation possibles (version TypeMasque)
c
c      LES CODES
c
c       INTEGER MINIMALE, MAXIMALE
c       INTEGER BASE_3D , PARTITIONNEMENT_3D, LISSAGE_3D
c       INTEGER CODE_B  , CODE_A
c       INTEGER MAILLEUR_2D, CODE_T
c
c       PARAMETER (MINIMALE=1, MAXIMALE=2)
c       PARAMETER (BASE_3D=6 , PARTITIONNEMENT_3D=8, LISSAGE_3D=9)
c       PARAMETER (CODE_B=3  , CODE_A=4) 
c       PARAMETER (MAILLEUR_2D=5, CODE_T=7)
c
c

c
c
c
c

c      Les types de dimension possibles
c
       INTEGER D1, D2, D3
c
       PARAMETER (D1=1, D2=2, D3=3)
c
c
c
c
c
c      Les types de geometrie possibles
c
       INTEGER NORMAL, AXISYMETRIQUE, PERIODIQUE
c
       PARAMETER (NORMAL=0, AXISYMETRIQUE=1, PERIODIQUE=2)
c
c
c
c
c
c      Les types de coordonnee possibles
c
       INTEGER CARTESIEN, CYLINDRIQUE
c
       PARAMETER (CARTESIEN=0, CYLINDRIQUE=1)
c
c
c
c
c
c      Les types de structure possibles
c
       INTEGER LIBRE, MIXTE, STRUCTURE
c
       PARAMETER (LIBRE=0, MIXTE=1, STRUCTURE=2)
