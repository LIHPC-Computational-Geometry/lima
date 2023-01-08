/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : validateur.h
 Cree le : Fri Jun 14 13:29:50 MEST 2002
 Auteur  : C Pignerol
******************************************************************************/

/*!
  \file validateur.h
  \brief Utilitaire de vérification de maillage.

*/

#ifndef _VALIDATEUR_H
#define _VALIDATEUR_H


#include "maillage_it.h"
#include "noeud_it.h"
#include "bras_it.h"
#include "polygone_it.h"
#include "ligne_it.h"
#include "surface_it.h"
#include "polyedre_it.h"


BEGIN_NAMESPACE_LIMA



//! Classe de vérification de maillage.
class _Validateur {


public:


  /*! 
    Vérifie que le maillage ç en premier argument est conforme aux
    spécifications données par le masque recu en second argument.
    En cas de non conformité(s) une exception de de type Lima::erreur et
	récapitulant les erreurs est levée.
  */
  static void verification_parametrable (const _MaillageInterne& mail, 
                                         LM_V_TYPEMASQUE masque);


  private :


  /*! Vérifie que la numérotation de l'ensemble des noeuds du maillage est
      effectuée. Le cas échéant, une exception de type Lima::erreur.
    \param	mail	est le maillage dont la numérotation des noeuds est
                    vérifiée.
   */
  static void verifier_compactage_numerotation_noeuds (
		const _MaillageInterne& mail);

   /*! Retourne un pointeur sur le polygone situé à gauche du 
       bras, ou lève une exception si ce bras n'a pas de polygone gauche. Un 
       polygone est considéré comme étant à gauche du bras si l'un 
       de ses bras est identique à celui évalué, y compris son 
       orientation.
    \param	bras	est le bras dont on recherche le polygone gauche.
    */
  static const _PolygoneInterne* polygone_gauche (const _BrasInterne& bras);

   /*! Retourne un pointeur sur le polygone situé à droite du 
       bras, ou lève une exception si ce bras n'a pas de polygone droit.
    \param	bras	est le bras dont on recherche le polygone droit.
    */
  static const _PolygoneInterne* polygone_droit (const _BrasInterne& bras);

  /*! Vérifie que le bras a deux noeuds. Lève, le
      cas échéant, une exception de type Lima::erreur renseignant 
      l'invalidité de ce bras.
    \param	bras	est le bras à vérifier.
   */
  static void verifier_bras (const _BrasInterne& bras);

  /*! Vérifie que la taille du bras n'est pas nulle. Lève, le
      cas échéant, une exception de type Lima::erreur.
    \param	bras	est le bras à vérifier.
   */
  static void verifier_bras_non_nul (const _BrasInterne& bras);

  /*! Vérifie, si le bras est connecté à deux polygones, que celui de 
      gauche a un identifiant plus petit que celui de droite. Lève, le cas 
      échéant, une exception de type Lima::erreur.
    \param	bras	est le bras dont l'orientation est testée.
   */
  static void verifier_orientation_bras (const _BrasInterne& bras);

  /*! Vérifie que le bras est connecté a une ou deux mailles et qu'il
      a au plus une maille de chaque côté (droit/gauche).
      Lève le cas échéant, une exception de type Lima::erreur 
      renseignant l'erreur rencontrée.
    \param	bras	est le bras à vérifier.
   */
  static void verifier_connexions_polygones_bras (const _BrasInterne& bras);

  /*! Vérifie que les bras du maillage ont deux noeuds. Lève, le
      cas échéant, une exception de type Lima::erreur renseignant les bras
      invalides.
      Cette fonction correspond au masque LM_V_BRAS.
    \param	mail	est le maillage dont les bras sont vérifiés.
   */
  static void verifier_bras (const _MaillageInterne& mail);

  /*! Vérifie qu'aucun des bras du maillage n'a de taille nulle. Lève, le
      cas échéant, une exception de type Lima::erreur renseignant les bras
      de taille nulle. 
      Cette fonction correspond au masque LM_V_BRAS_NON_NULS.
    \param	mail	est le maillage dont les bras sont vérifiés.
   */
  static void verifier_bras_non_nuls (const _MaillageInterne& mail);

  /*! Vérifie que les polygones de gauche ont des identifiants plus petits 
      que les polygones de droite pour chaque bras. Lève, le cas 
      échéant, une exception de type Lima::erreur renseignant les bras 
      mal orientés. 
      Cette fonction correspond au masque LM_V_ORIENTATION_BRAS.
    \param	mail	est le maillage dont l'orientation des bras est 
                    vérifiée.
   */
  static void verifier_orientations_bras (const _MaillageInterne& mail);

  /*! Vérifie que chaque bras est connecté a un ou deux polygones et que
      chaque bras a au plus un polygone de chaque côté (droit/gauche).
      Lève le cas échéant, une exception de type Lima::erreur 
      renseignant les bras invalides.
      Cette fonction correspond au masque LM_V_CONNEXIONS_POLYGONES_BRAS.
    \param	mail	est le maillage dont les bras sont vérifiés.
   */
  static void verifier_connexions_polygones_bras (const _MaillageInterne& mail);

  /*! Vérifie que la numérotation de l'ensemble des bras du maillage est 
      effectuée. Le cas échéant, une exception de type Lima::erreur.
    \param	mail	est le maillage dont la numérotation des bras est
                    vérifiée.
   */
  static void verifier_compactage_numerotation_bras (
		const _MaillageInterne& mail);

  /*! Vérifie que la ligne est structurée. Une ligne structurée a n ou
      n - 1 bras, où n est le nombre de noeuds. Les bras sont ordonnés de
      manière à être contigus. Les noeuds des bras ne sont pas
      ordonnés.
      Lève, le cas échéant, une exception de type Lima::erreur 
      renseignant l'erreur rencontrée.
    \param	ligne	est la ligne à vérifier.
   */
  static void verifier_structuration_ligne (const _LigneInterne& ligne);

  /*! Vérifie que la ligne est orientée dans le sens trigonométrique.
      Le sens d'orientation est obtenu à par développement de la formule 
      de Green et prend tout son sens dans le cas d'une ligne fermée.
      Lève, le cas échéant, une exception de type Lima::erreur .
    \param	ligne	est la ligne à vérifier.
   */
  static void verifier_orientation_ligne (const _LigneInterne& ligne);

  /*! Vérifie que les lignes du maillage sont structurées. Lève, le
      cas échéant, une exception de type Lima::erreur renseignant les 
      lignes non structurées.
      Cette fonction correspond au masque LM_V_STRUCTURER_LIGNES.
    \param	mail	est le maillage dont les lignes sont vérifiées.
   */
  static void verifier_structuration_lignes (const _MaillageInterne& mail);

  /*! Vérifie que les lignes fermées du maillage sont orientées dans
      le sens trigonométrique. Lève, le cas échéant, une exception 
      de type Lima::erreur renseignant les lignes mal orientées.
      Cette fonction correspond au masque LM_V_ORIENTATION_LIGNES_FERMEES.
    \param	mail	est le maillage dont les lignes sont vérifiées.
   */
  static void verifier_orientation_lignes_fermees (const _MaillageInterne& mail);

  /*! Vérifie que le polygone est orienté dans le sens
      trigonométrique. Lève, le cas échéant, une exception de type 
      Lima::erreur.
    \param	mail	est le maillage dont l'orientation des mailles est 
                    vérifiée.
   */
  static void verifier_orientation_polygone (const _PolygoneInterne& polygone);

  /*! Vérifie que les polygones du maillage sont orientées dans le sens
      trigonométrique. Lève, le cas échéant, une exception de type 
      Lima::erreur renseignant les polygones mal orientées.
      Cette fonction correspond au masque LM_V_ORIENTATION_POLYGONES.
    \param	mail	est le maillage dont l'orientation des polygones est 
                    vérifiée.
   */
  static void verifier_orientation_polygones (const _MaillageInterne& mail);

  /*! Vérifie que la numérotation de l'ensemble des polygones du maillage
      est effectuée. Le cas échéant, une exception de type 
      Lima::erreur.
    \param	mail	est le maillage dont la numérotation des polygones est
                    vérifiée.
   */
  static void verifier_compactage_numerotation_polygones (
		const _MaillageInterne& mail);

  /*! Vérifie que les contours des surfaces du maillage sont extraits. 
      Lève, le cas échéant, une exception de type Lima::erreur 
      renseignant les surfaces dont les contours n'ont pas été extraits.
      Cette fonction correspond au masque LM_V_CONTOURS_SURFACES.
    \param	mail	est le maillage dont les contours de surfaces sont 
                    vérifiés.
   */
  static void verifier_extraction_contours_surfaces (
		const _MaillageInterne& mail);

  /*! Vérifie que les contours des surfaces du maillage sont structurées.
      Lève, le cas échéant, une exception de type Lima::erreur 
      renseignant les contours non structurés.
      Cette fonction correspond au masque LM_V_STRUCT_CONTOURS_SURF.
    \param	mail	est le maillage dont les contours des surfaces sont 
                    vérifiés.
   */
  static void verifier_structuration_contours_surfaces (
		const _MaillageInterne& mail);

  /*! Vérifie que les contours des surfaces du maillage sont orientés dans
      le sens trigonométrique. Lève, le cas échéant, une exception 
      de type Lima::erreur renseignant les contours mal orientés.
      Cette fonction correspond au masque LM_V_ORIENT_CONTOURS_SURF.
    \param	mail	est le maillage dont l'orientation des contours est 
                    vérifié.
   */
  static void verifier_orientation_contours_surfaces (
		const _MaillageInterne& mail);

  /*! Vérifie que la numérotation de l'ensemble des polyedres du maillage
      est effectuée. Le cas échéant, une exception de type 
      Lima::erreur.
    \param	mail	est le maillage dont la numérotation des polyedres est
                    vérifiée.
   */
  static void verifier_compactage_numerotation_polyedres (
		const _MaillageInterne& mail);

  /*! Vérifie que la numérotation de l'ensemble des types d'entités du
      maillage est effectuée. Le cas échéant, une exception de type 
      Lima::erreur renseignant les types d'entités dont la numérotation 
      n'est pas compactée.
      Cette fonction correspond au masque LM_V_COMPACTE.
    \param	mail	est le maillage dont la numérotation des entités est
                    vérifiée.
   */
  static void verifier_compactage_numerotation (const _MaillageInterne& mail);
};	// class _Validateur




END_NAMESPACE_LIMA

#endif // _VALIDATEUR_H
