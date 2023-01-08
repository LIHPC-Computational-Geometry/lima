#ifndef _LIMA_H
#define _LIMA_H


#include "Lima/lima_types.h"

/** @name L'API C
 *
 * Les noms de fonctions commencent tous par LM_ puis sont suivis de
 * lettres en minuscules (tous les mots sont au singulier). La valeur
 * renvoyee est l'indicateur d'erreur (0 si pas d'erreur).
 *
 * Les seules allocations effectuees par l'API sont celles des donnees
 * internes de gestion, utilisees via la valeur du champ refmai de la
 * structure LM_MAILLAGE. Les liberations sont effectuees par
 * LM_fermer_maillage.
 * 
 * @memo Prototypes de fonctions pour l'utilisation de LIMA depuis un source C.
 */

/*@{*/

#ifdef __cplusplus
extern "C" {
#endif

  /**@name Les enums. 
   * @type 
   */
  /*@{*/

  /** Types enum pour l'acces aux fichiers et la preparation 
   */
  enum LM_FORMAT_FICHIER {
    /** Detection du format par le suffixe du nom du fichier.
     * Formats utilises pour la lecture et l'ecriture */
    LM_FF_SUFFIXE = 0, 
#ifdef __INTERNE_MALI
    /** Format Mali (*.unf). Pour la lecture et l'ecriture */
    LM_FF_MALI = 1, 
#endif
#ifdef __INTERNE_IDEAS
    /** Formats Ideas (*.unv). Pour la lecture et l'ecriture */
    LM_FF_IDEAS = 2, 
#endif
#ifdef __INTERNE_GIBI
    /** Formats Gibi (*.gib). Pour la lecture et l'ecriture */
    LM_FF_GIBI = 3, 
    /** Formats Castem (*.cas). Pour la lecture et l'ecriture */
    LM_FF_CASTEM = 4,
#endif
#ifdef __INTERNE_MODULEF
    /** Formats Modulef (*.nop). Pour la lecture et l'ecriture */
    LM_FF_MODULEF = 5,
#endif
#ifdef __INTERNE_IDEAS
    /** Format Ideas 2D. Pour la lecture uniquement */
    LM_FF_IDEAS_2D = 6,
    /** Formats Ideas Version 6. Pour l'ecriture uniquement */
    LM_FF_IDEAS_V6 = 7, 
#endif
#ifdef __INTERNE_GIBI
    /** Formats GIBI entier sur 8 bytes. Pour l'ecriture uniquement */
    LM_FF_GIBI_I8 = 8,
    /** Formats MAZE, pour la lecture uniquement */
#endif
#ifdef __INTERNE_MAZE
    LM_FF_MAZE = 9,
#endif
#ifdef __INTERNE_INGRID
    /** Formats INGRID, pour la lecture uniquement */
    LM_FF_INGRID = 10,
#endif
#ifdef __INTERNE_ICEMCFD
  /** Formats ICEMCFD (*.ice), pour la lecture uniquement. */
   LM_FF_ICEMCFD = 11,
#endif
#ifdef __INTERNE_MALIPP
  /** Format Mali++ avec HDF5 (*.mli, *.mliz). Pour la lecture et l'ecriture. */
   LM_FF_MALIPP = 12,
   LM_FF_MALIPP_Z=13,
#endif
#ifdef __INTERNE_ABAQUS
    /** Format Abaqus (*.inp). Pour la lecture uniquement. */
    LM_FF_ABAQUS = 14,
#endif
#ifdef __INTERNE_MALIPP2
  /** Format Mali++2 avec HDF5 (*.mli2). Pour la lecture et l'ecriture. */
   LM_FF_MALIPP2 = 15,
#endif
    LM_FF_INCONNU
  };  
  typedef enum LM_FORMAT_FICHIER LM_FORMAT_FICHIER;


  /** Niveau de preparation du maillage pour les codes 
   * (verification et traitement) 
   */
  enum LM_PREPARATION {
    /** Preparation minimale. Verification d'integrite uniquement. 
	Seul les donnees de base sont autorise en consultation */
    LM_PR_MINIMALE = 1, 
    /** Preparation maximale. 
	Toute les donnees calculables sont accessible */
    LM_PR_MAXIMALE = 2, 
    /** Preparation pour le code B */
    LM_PR_CODE_B = 3, 
    /** Preparation pour le code A */
    LM_PR_CODE_A = 4,
    /** Preparation pour le mailleur 2D */
    LM_PR_MAILLEUR_2D = 5,
    LM_PR_BASE_3D = 6,
    LM_PR_CODE_T = 7,
    LM_PR_INCONNU
  };
  typedef enum LM_PREPARATION LM_PREPARATION;



#ifdef __INTERNE_MACHINE_TYPES
// EXPURGE_BEGINNING_TAG MACHINE_TYPES
// EXPURGE_COMPLETION_TAG
#else
  typedef unsigned long long int LM_TYPEMASQUE ;
  typedef unsigned long long int LM_V_TYPEMASQUE ;
  typedef unsigned long long int LM_VC_TYPEMASQUE ;
#endif

  /* Les initialisations suivantes permettent d'associer un codage sur 
   * un entier 64 bits a chaque mot-cle. */

  /*
   * LES CODES */
  extern const LM_TYPEMASQUE LM_MINIMALE;

  extern const LM_TYPEMASQUE LM_MAXIMALE;
  extern const LM_TYPEMASQUE LM_CODE_B;
  extern const LM_TYPEMASQUE LM_CODE_A;

  extern const LM_TYPEMASQUE LM_MAILLEUR_2D;

  extern const LM_TYPEMASQUE LM_BASE_3D;

  extern const LM_TYPEMASQUE LM_CODE_T;
  
  extern const LM_TYPEMASQUE LM_PARTITIONNEMENT_3D;
  extern const LM_TYPEMASQUE LM_LISSAGE_3D;
  
  /*
   * LES PREPARATIONS ELEMENTAIRES */

  extern const LM_TYPEMASQUE LM_NOEUDS_BRAS;
  extern const LM_TYPEMASQUE LM_NOEUDS_POLYGONES;
  extern const LM_TYPEMASQUE LM_NOEUDS_POLYEDRES;
  extern const LM_TYPEMASQUE LM_POLYGONES_BRAS;
  extern const LM_TYPEMASQUE LM_BRAS_POLYGONES;
  extern const LM_TYPEMASQUE LM_POLYEDRES_BRAS;
  extern const LM_TYPEMASQUE LM_BRAS_POLYEDRES;
  extern const LM_TYPEMASQUE LM_POLYEDRES_POLYGONES;
  extern const LM_TYPEMASQUE LM_POLYGONES_POLYEDRES;
  
  extern const LM_TYPEMASQUE LM_SUPPRIMER_DOUBLONS_NOEUDS;
  extern const LM_TYPEMASQUE LM_SUPPRIMER_DOUBLONS_BRAS;
  extern const LM_TYPEMASQUE LM_SUPPRIMER_DOUBLONS_POLYGONES;
  extern const LM_TYPEMASQUE LM_SUPPRIMER_DOUBLONS_POLYEDRES;
  
  extern const LM_TYPEMASQUE LM_SUPPRIMER_BRAS_INUTILISES;
  extern const LM_TYPEMASQUE LM_SUPPRIMER_POLYGONES_INUTILISES;

  extern const LM_TYPEMASQUE LM_LIGNES_NOEUDS;
  extern const LM_TYPEMASQUE LM_SURFACES_NOEUDS;
  extern const LM_TYPEMASQUE LM_SURFACES_BRAS;
  extern const LM_TYPEMASQUE LM_VOLUMES_NOEUDS;
  extern const LM_TYPEMASQUE LM_VOLUMES_BRAS;
  extern const LM_TYPEMASQUE LM_VOLUMES_POLYGONES;
  
  extern const LM_TYPEMASQUE LM_RAZ_NOEUDS_BRAS;
  extern const LM_TYPEMASQUE LM_RAZ_NOEUDS_POLYGONES;
  extern const LM_TYPEMASQUE LM_RAZ_NOEUDS_POLYEDRES;
  extern const LM_TYPEMASQUE LM_RAZ_POLYGONES_BRAS;
  extern const LM_TYPEMASQUE LM_RAZ_BRAS_POLYGONES;
  extern const LM_TYPEMASQUE LM_RAZ_POLYEDRES_BRAS;
  extern const LM_TYPEMASQUE LM_RAZ_BRAS_POLYEDRES;
  extern const LM_TYPEMASQUE LM_RAZ_POLYEDRES_POLYGONES;
  extern const LM_TYPEMASQUE LM_RAZ_POLYGONES_POLYEDRES;
  
  extern const LM_TYPEMASQUE LM_CONTOURS_LIGNES;
  extern const LM_TYPEMASQUE LM_CONTOURS_SURFACES;
  extern const LM_TYPEMASQUE LM_CONTOURS_VOLUMES;
  extern const LM_TYPEMASQUE LM_CONTOURS_SURF_NOE;
  extern const LM_TYPEMASQUE LM_CONTOURS_VOL_NOE;
  extern const LM_TYPEMASQUE LM_CONTOURS_VOL_BR;
  
  extern const LM_TYPEMASQUE LM_STRUCTURER_LIGNES;
  extern const LM_TYPEMASQUE LM_STRUCTURER_SURFACES;
  extern const LM_TYPEMASQUE LM_STRUCT_CONTOURS_SURF;
  
  extern const LM_TYPEMASQUE LM_COMPACTE;
  extern const LM_TYPEMASQUE LM_ORIENTATION;
  /** Orienter dans le sens trigonométrique les contours de surface. */
  extern const LM_TYPEMASQUE LM_ORIENT_CONTOURS_SURF;

  /** Convertir les noms de groupes en majuscules. */
  extern const LM_TYPEMASQUE LM_FORMATER_NOMS;

  /* 
   * LES VERIFICATIONS DE CODES
   */
   /*! Vérifie que le maillage est conforme au code B. */
   extern const LM_VC_TYPEMASQUE LM_VC_CODE_B;

  /*
   * LES VERIFICATIONS ELEMENTAIRES */
  /*! Vérifie que les bras sont constitues de deux noeuds. */
  extern const LM_V_TYPEMASQUE LM_V_BRAS;
  /*! Vérifie l'absence de bras de taille nulle. */
  extern const LM_V_TYPEMASQUE LM_V_BRAS_NON_NULS;
  /*! Vérifie que les mailles de gauche ont des identifiants plus petits que
     les mailles de droite des bras. */
  extern const LM_V_TYPEMASQUE LM_V_ORIENTATION_BRAS;
  /*! Vérifie que chaque bras est connecté à un ou deux polygones et
      que, s'il y en a deux, qu'il y en a un de chaque côté. */
  extern const LM_V_TYPEMASQUE LM_V_CONNEXIONS_POLYGONES_BRAS;
  /*! Vérifie que les lignes sont structurées. */
  extern const LM_V_TYPEMASQUE LM_V_STRUCTURER_LIGNES;
  /*! Vérifie que les lignes fermées sont orientées dans le sens
      trigonométrique. */
  extern const LM_V_TYPEMASQUE LM_V_ORIENTATION_LIGNES_FERMEES;
  /*! Vérification que les polygones sont orientés dans le sens 
      trigonométrique. */
  extern const LM_V_TYPEMASQUE LM_V_ORIENTATION_POLYGONES;
  /*! Vérifie que des contours ont été extraits des surfaces. */
  extern const LM_V_TYPEMASQUE LM_V_CONTOURS_SURFACES;
  /*! Vérifie que les contours de surface sont structurés. */
  extern const LM_V_TYPEMASQUE LM_V_STRUCT_CONTOURS_SURF;
  /*! Vérification que les contours des surfaces  sont orientés dans le 
      sens trigonométrique. */
  extern const LM_V_TYPEMASQUE LM_V_ORIENT_CONTOURS_SURF;
  /*! Vérification que le compactage de la numérotation des entités 
      a été effectuée. */
  extern const LM_V_TYPEMASQUE LM_V_COMPACTE;

  /* Attributs du maillage. 
   */
  enum LM_GEOMETRIE {
    LM_GE_NORMAL = 0, 
    LM_GE_AXISYMETRIQUE,
    LM_GE_PERIODIQUE
  };
  typedef enum LM_GEOMETRIE LM_GEOMETRIE;


  /** Structuration des objets. Type correspondant a la
   * dimension du maillage (2D:surfaces, 3D:volumes) ; aucun objet,
   * certains, tous 
   */
  enum LM_STRUCTURATION {
    LM_ST_LIBRE = 0, 
    LM_ST_MIXTE, 
    LM_ST_STRUCTURE
  };
  typedef enum LM_STRUCTURATION LM_STRUCTURATION;


  /** Type des coordonnées. */
  enum LM_TYPE_COORDONNEE {
    LM_TC_CARTESIEN=0,
    LM_TC_CYLINDRIQUE
  };
  typedef enum LM_TYPE_COORDONNEE LM_TYPE_COORDONNEE;


  /** Type des entites. 
   * Identiques pour les types d'entites et d'objets associes ;
   * valeurs de 0 a 3, permettant d'indicer les champs tableaux de type t */
  enum LM_TYPE_ENTITE {
    LM_TE_NOEUD = 0, 
    LM_TE_BRAS, 
    LM_TE_MAILLE, 
    LM_TE_POLYEDRE
  };
  typedef enum LM_TYPE_ENTITE LM_TYPE_ENTITE;


  /** Type des objets. 
   * Identiques pour les types d'entites et d'objets associes ;
   * valeurs de 0 a 3, permettant d'indicer les champs tableaux de type t */
  enum LM_TYPE_OBJET {
    LM_TO_NUAGE = 0, LM_TO_LIGNE, LM_TO_SURFACE, LM_TO_VOLUME
  };
  typedef enum LM_TYPE_OBJET LM_TYPE_OBJET;
  /*@}*/

  /**@name Les structures.
   * @type 
   */
  /*@{*/

  /** Structure maillage */
  struct LM_MAILLAGE {
    int_type        refmai;	/* Valeur utilisee par l'API */

    /* Attributs du maillage */
    char            titre[81], date[21];
    double          unite;	/* Unite de longueur exprimee en SI */
    LM_GEOMETRIE    geometrie;
    LM_STRUCTURATION structuration;
    /* LM_TYPE_COORDONNEE type_coordonnee; inutile, toujours cartesien */

    int_type        dimension;/* 2 ou 3 */

    /** @name entite 
     *	Nombre d'entites et de connexions.
     */
// EXPURGE_BEGINNING_TAG DOC_CONNECTIVITES
// EXPURGE_COMPLETION_TAG
    union {
      /** @name n
       * Acces par le nom de chaque champ */
      struct {
	/** */
	int_type nombre_noeud, noeud_bras, noeud_maille, noeud_polyedre;
	int_type bras_noeud, nombre_bras, bras_maille, bras_polyedre;
	int_type maille_noeud, maille_bras, nombre_maille, maille_polyedre;
	int_type polyedre_noeud, polyedre_bras, polyedre_maille, nombre_polyedre;
      } n;
      /** @name t
       * Acces par tableau a 2 dimensions. t[LM_TE_BRAS][LM_TE_BRAS]
       * est au meme endroit que n.nombre_bras,
       * t[LM_TE_BRAS][LM_TE_NOEUD] est au meme endroit que
       * n.bras_noeud, */
      int_type t[4][4];
    } entite;

    /** Nombre d'objets et de connexions */
    union {
      struct {
	/** Acces par le nom de chaque champ */
	int_type nombre_nuage, nombre_ligne, nombre_surface, nombre_volume;
	int_type nuage_noeud, ligne_bras, surface_maille, volume_polyedre;
      } n;
      /** Acces par tableau a 2 dimensions. 1er vecteur = nombre
       * d'objets, 2e vecteur = nombre de connexions objets-entites */
      int_type t[2][4];
    } objet;
  };
  typedef struct LM_MAILLAGE LM_MAILLAGE;


  /** Structure noeud */
  struct LM_NOEUD {
    /** */
    union {
      /** @name n
       * Acces par le nom de chaque coordonnee */
      struct {
	double x, y, z;
      } n;
      /** Acces par tableau des 3 coordonnees */
      double t[3];
    } coordonnee;
  };               
  typedef struct LM_NOEUD LM_NOEUD;


  /** Structure objet */
  struct LM_OBJET 
  {
    LM_TYPE_OBJET   type;
    /** de 1 a nombre_objet */
    int_type        numero;	
    char            nom[1025];

    /** @name nombre
     * Nombre d'entites et adresse des listes */
    union {
      /** @name n
       * Acces par le nom de chaque champ */
      struct {
	int_type noeud, bras, maille, polyedre;
	int_type noeud_contour, bras_contour, maille_contour;
      } n;
      /** Acces par tableau a 2 dimensions.
       * 1er vecteur = nombre d'entites dans l'objet, 
       * 2e vecteur = nombre d'entites du contour */
      int_type t[2][4];
    } nombre;

    /** @name liste
     * Listes des entites */
    union {
      /** @name n 
       * Acces par le nom de chaque champ */
      struct {
	int_type *noeud, *bras, *maille, *polyedre;
	int_type *noeud_contour, *bras_contour, *maille_contour;
      } n;
      /** Acces par tableau a 2 dimensions.
       * 1er vecteur = liste des entites de l'objet, 
       * 2e vecteur = liste des entites du contour */
      int_type *t[2][4];
    } liste;

    /** @name longueur
     * Nombre d'entites du type de l'objet. 
     * Comptees dans chaque
     * direction, si l'objet est structure */
    union {
      /** @name n
	  Acces par le nom de chaque champ */
      struct {
	int_type ni, nj, nk;
      } n;
      /** Acces par tableau des 3 longueurs */
      int_type t[3];
    } longueur;
  };
  typedef struct LM_OBJET LM_OBJET;

  /*@}*/

  /**@name Les fonctions.
   * @type 
   */
  /*@{*/

  /** Retourne la version de lima++. */
  const char* LM_version ( );

  /** Fonction de lecture d'un maillage. */
  int LM_lire_maillage(LM_FORMAT_FICHIER format, const char *nom_fichier, 
		       LM_MAILLAGE *maillage);


  /** Effectue les traitements de verification et de
   * calcul selon le niveau de preparation demande. 
   * Ensuite les champs de la structure LM_MAILLAGE sont renseignes.
   * FONCTIONS OBSOLETES remplacées par LM_preparer_maillage2 et
   * LM_preparation_parametrable2.
   *
   * @param maillage La structure doit avoir ete remplie par LM_lire_maillage.
   * @param preparation	est le niveau de préparation demandé.
   * @return			0 si aucune erreur est rencontrée, ou une valeur non
   *					nulle en cas d'erreur.
   */

  int LM_preparer_maillage(LM_MAILLAGE *maillage,  
                           LM_PREPARATION preparation) ;

  int LM_preparation_parametrable(LM_MAILLAGE *maillage,  
				  LM_TYPEMASQUE preparation) ;


  /** Effectue les traitements de calcul selon le niveau de preparation demande.
   * Ensuite les champs de la structure LM_MAILLAGE sont renseignes.
   * Les vérifications sont à effectuer à l'aide de la fonction
   * LM_verification_parametrable2.
   *
   * @param maillage La structure doit avoir ete remplie par LM_lire_maillage.
   * @param preparation	est le niveau de préparation demandé.
   * @param	message		permet de récupérer en sortie un listing contenant
   *					les erreurs rençontrées. Ce tableau de caractères
   *					doit être alloué.
   * @param	taille_message	donne la taille du tableau de caractères message.
   * @return			0 si aucune erreur est rencontrée, ou une valeur non
   *					nulle en cas d'erreur.
   */

  int LM_preparer_maillage2 (LM_MAILLAGE *maillage, LM_PREPARATION preparation,
                             char* message, size_t taille_message) ;

  int LM_preparation_parametrable2 (LM_MAILLAGE *maillage, 
              LM_TYPEMASQUE preparation, char* message, size_t taille_message);

	/**
	 * @param			précision utilisée dans le calcul de distance entre deux
	 *					noeuds. Cette valeur est utilisée par la préparation
	 *					élémentaire LM_SUPPRIMER_DOUBLONS_NOEUDS.
	 *					Cette valeur n'est prise en compte que si elle est
	 *					strictement positive.
	 * @return			0 si cette valeur est prise en compte, -1 dans le cas
	 *					contraire.
	 */
	int LM_precision_distance_noeuds (double precision);

  /** Vérifie que le maillage est conforme aux spécifications du code dont
   * l'identifiant est transmis en argument.
   * @param	maillage	est la structure contenant les informations sur le
   *					maillage à vérifier.
   * @param	code		identifie le code avec lequel doit être compatible
   *					le maillage.
   * @param	message		permet de récupérer en sortie un listing contenant
   *					les erreurs rençontrées. Ce tableau de caractères
   *					doit être alloué.
   * @param	taille_message	donne la taille du tableau de caractères message.
   * @return			0 si aucune erreur est rencontrée, ou une valeur non
   *					nulle en cas d'erreur.
   */
  int LM_verifier_maillage2 (LM_MAILLAGE *maillage, LM_VC_TYPEMASQUE code, 
                             char* message, size_t taille_message);

  /** Vérifie que le maillage est conforme aux spécifications demandées
   * dans le masque transmis en argument.
   * @param	maillage	est la structure contenant les informations sur le
   *					maillage à vérifier.
   * @param	masque		est le masque de vérification à appliquer.
   * @param	message		permet de récupérer en sortie un listing contenant
   *					les erreurs rençontrées. Ce tableau de caractères
   *					doit être alloué.
   * @param	taille_message	donne la taille du tableau de caractères message.
   * @return			0 si aucune erreur est rencontrée, ou une valeur non
   *					nulle en cas d'erreur.
   */
  int LM_verification_parametrable2 (LM_MAILLAGE *maillage, 
             LM_V_TYPEMASQUE masque, char* message, size_t taille_message);

  /** Renseigne le tableau noeud alloue par l'utilisateur. 
   * Ce tableau doit contenir au moins nombre_noeud
   * elements (ce champ est verifie dans LM_MAILLAGE). noeud[i] est la
   * description du noeud de numero i+1. C'est le numero du noeud qui est
   * utilise dans les connectivites. 
   */
  int LM_lire_noeud(LM_MAILLAGE *maillage, LM_NOEUD noeud[]);


  /** Renseigne les tableaux pointage et liste alloues par l'utilisateur. 
   * Ces tableaux doivent contenir au
   * moins (respectivement) nombre_typi+1 et typi_typj entiers ; ces
   * valeurs sont verifiees dans la structure LM_MAILLAGE. La numerotation
   * des entites commence a 1 et les tableaux representent la table de
   * connectivite au format fortran : le numero de la premiere entite typj
   * connectee a l'entite typi numero i est liste[pointage[i-1]-1]. Si la
   * preparation n'a pas renseigne typi_typj, la table ne doit pas etre
   * accedee. 
   */
  int LM_lire_connexion(LM_MAILLAGE *maillage, 
			LM_TYPE_ENTITE typi, LM_TYPE_ENTITE typj, 
			int_type* pointage, int_type* liste);


  /** Renseigne une structure LM_OBJET allouee par l'utilisateur.
   * LM_lire_objet recoit l'adresse d'une structure LM_OBJET.
   * Les champs en entree y sont type et numero ou nom,
   * ainsi que les adresses des listes et eventuellement les champs
   * nombre. 
   * Si numero est non nul, nombre_objet est alors verifie dans la
   * structure LM_MAILLAGE, l'objet sera selectionne par son numero dans
   * la liste des objets du type et nom sera renseigne. Si numero vaut 0,
   * l'objet sera selectionne par recherche sur le nom et numero sera
   * renseigne. Si nom ne correspond a aucun objet de la liste, numero est
   * laisse a 0, sans positionnement de l'indicateur d'erreur. 
   * La liste des noms peut etre obtenue en faisant varier numero de 1 a
   * nombre_objet. 
   * Les champs qui ne sont pas pris en entree sont renseignes ou
   * recoivent une valeur par defaut.
   * Si un champ nombre peut etre renseigne par l'API, et si son champ
   * liste correspondant est NULL, alors le champ nombre est renseigne.
   * Mais si le champ liste est non nul, alors le champ nombre n'est pas
   * renseigne mais verifie (c'est la longueur du tableau), et le champ
   * liste est pris pour l'adresse d'un tableau alloue par l'utilisateur,
   * qui sera renseigne par l'API, et contenant au moins le nombre
   * d'elements prevu au champ nombre. Pour extraire une liste, il faut
   * donc faire deux appels, un pour consulter la taille et un autre pour
   * remplir la liste. On pourra allouer le tableau ou verifier sa taille
   * entre les deux appels 
   */
  int LM_lire_objet(LM_MAILLAGE *maillage, LM_OBJET *objet);


  /* Fonctions de definition et d'ecriture d'un maillage 
   * Les fonctions LM_definir... permettent soit de definir un nouveau
   * maillage, soit de modifier un maillage existant. L'exploitation
   * (consultation, ecriture) doit etre precedee de la preparation. 
   */


  /** Reserve en memoire la structure de stockage d'un jeu de donnees maillage.
   * Initialise la structure LM_MAILLAGE
   * allouee par l'utilisateur et y renseigne le champ refmai. 
   */
  int LM_reserver_maillage(LM_MAILLAGE *maillage);


  /** Lit et enregistre la valeur des des differents champs. Les champs sont
   * date, unite, geometrie, dimension, nombre_noeud, nombre_bras,
   * nombre_maille, nombre_polyedre, bras_noeud, maille_noeud,
   * polyedre_noeud, nombre_nuage, nombre_ligne, nombre_surface,
   * nombre_volume, nuage_noeud, ligne_bras, surface_maille,
   * volume_polyedre (ces 4 dernieres valeurs sont necessaires car les
   * objets sont geres en interne par des tables de connectivite) de la
   * structure LM_MAILLAGE. 
   */
  int LM_definir_maillage(LM_MAILLAGE *maillage);


  /** Lit le contenu du tableau noeud[] alloue et renseigne par l'utilisateur. 
   * Ce tableau doit
   * contenir au moins nombre_noeud elements (ce champ est verifie dans
   * LM_MAILLAGE). noeud[i] est la description du noeud de numero i+1.
   * C'est le numero du noeud qui est utilise dans les connectivites. 
   */
  int LM_definir_noeud(LM_MAILLAGE *maillage, LM_NOEUD noeud[]);


  /** Lit le contenu des tableaux
   * pointage et liste alloues par l'utilisateur. Ces tableaux doivent
   * contenir au moins (respectivement) nombre_typi+1 et typi_typj entiers
   * ; ces valeurs sont verifiees dans la structure LM_MAILLAGE. La
   * numerotation des entites commence a un et les tableaux representent
   * la table de connectivite au format fortran : le numero de la premiere
   * entite typj connectee a l'entite typi numero i est
   * liste[pointage[i-1]-1]. Les donnees de connectivite pouvant etre
   * definies sont celles dont les dimensions sont enregistrees par
   * LM_definir_maillage, c'est a dire bras-noeuds, maille-noeuds et
   * polyedre-noeuds. 
   */
  int LM_definir_connexion(LM_MAILLAGE *maillage, 
			   LM_TYPE_ENTITE typi, LM_TYPE_ENTITE typj, 
			   int_type* pointage, int_type* liste);

  /** Lit une structure LM_OBJET allouee
   * par l'utilisateur. Les champs en entree sont type_objet, nom, numero,
   * nombre.t[0][type_objet] et liste.t[0][type_objet]. Pour un maillage
   * et un type d'objet donnes, le premier appel doit se faire avec
   * numero=1 et les appels suivants doivent se faire en incrementant
   * numero (contrainte due au chargement par morceaux des tables de
   * connectivite definissant les objets). La definition ou la
   * modification de taille d'un objet doit etre suivie de la
   * (re)definition de tous les objets du meme type et ayant un numero
   * superieur. 
   */
  int LM_definir_objet(LM_MAILLAGE *maillage, LM_OBJET *objet);


  /** Ecrit un maillage prepare et non modifie depuis. */
  int LM_ecrire_maillage(LM_MAILLAGE *maillage, LM_FORMAT_FICHIER format, 
			   const char *nom_fichier);
  
  /** Libere les espaces memoire utilises par la structure LM_MAILLAGE.
   * Initialise la structure LM_MAILLAGE. 
   */
  int LM_fermer_maillage(LM_MAILLAGE *maillage);

#ifdef __INTERNE_MALIPP
	/** (In)active la compression avec la zlib lors des opérations d'écriture
	 * dans des fichiers au format mali++.
	 * La compression est activée si comprimer vaut 1, inactivée dans le cas
	 * contraire.
	 * Si comprimer vaut 1, le niveau de compression doit être compris entre 0
	 * et 1.
	 * Retourne 0 (succès) ou -1 (échec).
	 */
	int LM_compression_zlib_malipp (int comprimer, int niveau);
#endif	/* __INTERNE_MALIPP */
  /*@}*/

#ifdef __cplusplus
}
#endif

/*@}*/

#endif				/* _LIMA_H  */
