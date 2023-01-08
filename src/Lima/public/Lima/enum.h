#ifndef _ENUM_H
#define _ENUM_H


#include "config.h"
#include "size.h"
#include "lima.h"

#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

//! Dimension.
enum dim_t {D1=1, D2, D3};
//! Type de geometrie.
enum geometrie_t {NORMAL, AXISYMETRIQUE, PERIODIQUE};
//! Type des coordonnées.
enum coordonnee_t {CARTESIEN, CYLINDRIQUE};

//! Structuration globale, locale ou aucune.
enum structuration_t {LIBRE, MIXTE, STRUCTURE};
  
enum format_t {
  /** Detection du format par le suffixe du nom du fichier.
#ifdef __INTERNE_MALI
   * .unf=LM_FF_MALI,
#endif
#ifdef __INTERNE_IDEAS
   * .unv=LM_FF_IDEAS,
#endif
#ifdef __INTERNE_ABAQUS
   * .inp=LM_FF_ABAQUS,
#endif
#ifdef __INTERNE_GIBI
   * .gib=LM_FF_GIBI,
   * .cas=LM_FF_CASTEM,
#endif
#ifdef __INTERNE_MODULEF
   * .nop=LM_FF_MODULEF
#endif
#ifdef __INTERNE_MAZE
   * .maz=LM_FF_MAZE
#endif
#ifdef __INTERNE_INGRID
   * .ing=LM_FF_INGRID
#endif
#ifdef __INTERNE_ICEMCFD
   * .ice=LM_FF_ICEMCFD, .uns=LM_FF_ICEMCFD
#endif
#ifdef __INTERNE_MALIPP
   * .mli=LM_FF_MALIPP, .mliz=LM_FF_MALIPP_Z
#endif
#ifdef __INTERNE_MALIPP2
   * .mli2=LM_FF_MALIPP2
#endif
   */
  SUFFIXE = 0,
#ifdef __INTERNE_MALI
  /// Format Mali. Pour la lecture et l'ecriture.
  MALI = 1,
#endif
#ifdef __INTERNE_IDEAS
  /// Formats Ideas. Pour la lecture et l'ecriture.
  IDEAS = 2,
#endif
#ifdef __INTERNE_GIBI
  /// Formats Gibi. Pour la lecture et l'ecriture.
  GIBI = 3,
  /// Formats Castem. Pour la lecture et l'ecriture.
  CASTEM = 4,
#endif
#ifdef __INTERNE_MODULEF
  /// Formats Modulef. Pour la lecture et l'ecriture.
  MODULEF = 5,
#endif
#ifdef __INTERNE_IDEAS
  /// Format Ideas 2D. Pour la lecture uniquement.
  IDEAS_2D = 6,
  /// Formats Ideas Version 6. Pour l'ecriture uniquement.
  IDEAS_V6 = 7,
#endif
#ifdef __INTERNE_GIBI
  /// Formats GIBI entier sur 8 bytes. Pour l'ecriture uniquement.
  GIBI_I8 = 8,
#endif
#ifdef __INTERNE_MAZE
  /// Formats MAZE, pour la lecture uniquement.
  MAZE = 9,
#endif
#ifdef __INTERNE_INGRID
  /// Formats INGRID, pour la lecture uniquement.
  INGRID = 10,
#endif
#ifdef __INTERNE_ICEMCFD
  /// Formats ICEMCFD, pour la lecture uniquement.
  ICEMCFD = 11,
#endif
#ifdef __INTERNE_MALIPP
  /// Format Mali++ avec HDF5. Pour la lecture et l'ecriture.
  MALIPP = 12,
  MALIPP_Z=13,
#endif
#ifdef __INTERNE_ABAQUS
  /// Format Abaqus. Pour la lecture uniquement.
  ABAQUS = 14,
#endif
#ifdef __INTERNE_MALIPP2
  MALIPP2 = 15,
#endif
  INCONNU
};
  
/** Niveau de preparation du maillage pour les codes
   * (verification et traitement)
   */
enum preparation_t {
  /** Preparation minimale. Verification d'integrite uniquement.
      Seul les donnees de base sont autorise en consultation */
  MINIMALE=1,
  /** Preparation maximale.
      Toute les donnees calculables sont accessible */
  MAXIMALE=2,
  /// Preparation pour le code B.
  CODE_B=3,
  /// Preparation pour les code A.
  CODE_A=4,
  /// Preparation pour le mailleur 2D.
  MAILLEUR_2D=5,
  BASE_3D=6,
  CODE_T=7,
  PARTITIONNEMENT_3D=8,
  LISSAGE_3D=9
};

typedef LM_TYPEMASQUE		TypeMasque;
typedef LM_V_TYPEMASQUE		TypeMasqueVerification;
typedef LM_VC_TYPEMASQUE	TypeVerificationCode;

// Liste des formats de lecture disponibles
extern const char** liste_format_lecture();

// Liste des formats d'ecriture disponibles
extern const char** liste_format_ecriture();

extern const char* lima_version();

#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif
