#include <string.h>
#include <vector>
#include <stdio.h>
// EXPURGE_BEGINNING_TAG INCLUDE_LOCAL_DEPENDENCIES
// EXPURGE_COMPLETION_TAG

#include <Lima/lima.h>
#include <Lima/enum.h>
#include "LimaP/enum_apif.h"
#include "LimaP/attribut_type.h"
#include "LimaP/config_it.h"
#include "LimaP/using_it.h"

#include "LimaP/noeud_it.h"
#include "LimaP/bras_it.h"
#include "LimaP/polygone_it.h"
#include "LimaP/polyedre_it.h"

#include "LimaP/nuage_it.h"
#include "LimaP/ligne_it.h"
#include "LimaP/surface_it.h"
#include "LimaP/volume_it.h"

#include "LimaP/maillage_it.h"
#include <Lima/maillage.h>

#include "LimaP/fonction.h"
#include "LimaP/algorithme.h"

#ifdef __INTERNE_MALIPP
#include <Lima/malipp.h>
#endif	// __INTERNE_MALIPP

/// Developpements novembre 2001
#include <Lima/size.h>
///

#include <Lima/erreur.h>
#include "LimaP/util.h"
#include "LimaP/misc.h"


extern "C" {  

// EXPURGE_BEGINNING_TAG LOCAL_DEPENDENCIES_1
// EXPURGE_COMPLETION_TAG

  void lminnm_(char* _ficmai, int_type* refmai, char* _datmod, 
	       double* unlosi, int_type* typcoo, int_type* dimens, 
	       int_type* geomet, int_type* structure, int_type* ier, 
	       int l_ficmai, int l_datmod);
  
  void lminve_(char* _varfic, int_type* refmai, char* _datmod, 
	       double* unlosi, int_type* typcoo, int_type* dimens, 
	       int_type* geomet, int_type* structure, int_type* ier, 
	       int l_varfic, int l_datmod);

  void lmlenm_(char* _ficmai, int_type* refmai, char* _datmod, 
	       double* unlosi, int_type* typcoo, int_type* dimens, 
	       int_type* geomet, int_type* structure, int_type* ier, 
	       int l_ficmai, int l_datmod);

  void lmleve_(char* _varfic, int_type* refmai, char* _datmod, 
	       double* unlosi, int_type* typcoo, int_type* dimens, 
	       int_type* geomet, int_type* structure, int_type* ier, 
	       int l_varfic, int l_datmod);

  void lmindi_(int_type* refmai, int_type* nbrnoe, int_type* nbrbra,
	       int_type* nbrmai,  int_type* ier);

  void lmnbcn_(int_type* refmai, int_type* maibra, int_type* mainoe,           
	       int_type* branoe, int_type* ier);

  void lmnbnu_(int_type* refmai, int_type* nbrnua, int_type* ier);

  void lmnbli_(int_type* refmai, int_type* nbrlig, int_type* ier);

  void lmnbsu_(int_type* refmai, int_type* nbrsur, int_type* ier);

  void lmnoxy_(int_type* refmai, double* x, double* y, int_type* ier);

  void lmnox4_(int_type* refmai, float* x, float* y, int_type* ier);

  void lmnobr_(int_type* refmai, int_type* lnobr, 
	       int_type* pnobr, int_type* ier);

  void lmnoma_(int_type* refmai, int_type* lnoma, 
	       int_type* pnoma, int_type* ier);

  void lmbrno_(int_type* refmai, int_type* noeori, 
	       int_type* noeext, int_type* ier);

  void lmbrma_(int_type* refmai, int_type* maigau, 
	       int_type* maidro, int_type* ier);

  void lmmano_(int_type* refmai, int_type* lmano, 
	       int_type* pmano, int_type* ier);

  void lmmabr_(int_type* refmai, int_type* lmabr, 
	       int_type* pmabr, int_type* ier);

  void lmnuls_(int_type* refmai, char* _nomnua, 
	       int_type* ier, int l_nomnua);

  void lmnuex_(int_type* refmai, char* _nomnua, int_type* nbrnoe,
	       int_type* ier, int l_nomnua);

  void lmnuno_(int_type* refmai, char* _nomnua, int_type* lnoe,
	       int_type* ier, int l_nomnua);

  void lmlils_(int_type* refmai, char* _nomlig, 
	       int_type* ier, int l_nomlig);

  void lmliex_(int_type* refmai, char* _nomlig, int_type* nbrbra,
	       int_type* nbrnoe, int_type* ier, int l_nomlig);

  void lmlibr_(int_type* refmai, char* _nomlig, int_type* lbra,
	       int_type* ier, int l_nomlig);
  
  void lmlino_(int_type* refmai, char* _nomlig, int_type* lnoe,
	       int_type* ier, int l_nomlig);

  void lmsuls_(int_type* refmai, char* _nomsur, 
	       int_type* ier, int l_nomsur);

  void lmsuex_(int_type* refmai, char* _nomsur, int_type* nbrmai,
	       int_type* lonlig, int_type* nbrbrc, int_type* nbrnoc, 
	       int_type* ier, int l_nomsur);

  void lmsues_(int_type* refmai, char* _nomsur, int_type* nbrmai,
	       int_type* lonlig, int_type* nbrbrs, int_type* nbrbrc,
	       int_type* nbrnos, int_type* nbrnoc, int_type* ier, 
	       int l_nomsur);

  void lmsuma_(int_type* refmai, char* _nomsur, int_type* lmai,
	       int_type* ier, int l_nomsur);

  void lmsubr_(int_type* refmai, char* _nomsur, int_type* lbrasu,
	       int_type* ier, int l_nomsur);

  void lmcobr_(int_type* refmai, char* _nomsur, int_type* lbraco,
	       int_type* ier, int l_nomsur);

  void lmsuno_(int_type* refmai, char* _nomsur, int_type* lnoesu,
	       int_type* ier, int l_nomsur);

  void lmcono_(int_type* refmai, char* _nomsur, int_type* lnoeco,
	       int_type* ier, int l_nomsur);

  void lmfin_(int_type* refmai, int_type* ier);

  void lmmess_(int* iumes);

  void lmsume_(int_type *qn, double *x, double *y, int_type *qmq, 
	       int_type *mqn, int_type *ia, int_type *ni, double *om, 
	       int_type *lf, int_type *lx, int_type *ier);

  void lmcrmi_(int_type* refmai, char* titre, char* _datmod, 
	       double* unlosi, int_type* typcoo, int_type* dimens, 
	       int_type* geomet, int_type* ier, 
	       int l_titre, int l_datmod); 

  void lmcrno_(int_type* refmai, double* x, double* y, int_type* nbnoe,
	       int_type* ier);

  void lmcrno4_(int_type* refmai, float* x, float* y, int_type* nbnoe,
		int_type* ier);

  void lm3crno_(int_type* refmai, double* x, double* y, double* z,
		int_type* nbnoe, int_type* ier);

  void lm3crno4_(int_type* refmai, float* x, float* y, float* z,
		 int_type* nbnoe, int_type* ier);

  void lmcrbr_(int_type* refmai, int_type* noeori, int_type* noeext,
	       int_type* nbbras, int_type* ier);

  void lmcrma_(int_type* refmai, int_type* lmano, int_type* pmano,
	       int_type* nbmai, int_type* ier); 

  void lm3crma_(int_type* refmai, int_type* lmano, int_type* pmano,
		int_type* nbmai, int_type* ier); 

  void lmcrnu_(int_type* refmai, char* nom, int_type* idnoe, int_type* nbnoe,
	       int_type* ier, int l_nom);

  void lmcrli_(int_type* refmai, char* nom, int_type* idbr, int_type* nbbr,
	       int_type* ier, int l_nom); 

  void lmcrsu_(int_type* refmai, char* nom, int_type* idpg, int_type* nbpg,
	       int_type* ier, int l_nom); 

  void lm3crvo_(int_type* refmai, char* nom, int_type* idpd, int_type* nbpd,
		int_type* ier, int l_nom);

  void lmajno_(int_type* refmai, char* nom, int_type* idnoe, int_type* ier,
	       int l_nom);

  void lmreno_(int_type* refmai, char* nom, int_type* idnoe, int_type* ier,
	       int l_nom);

  void lmdeno_(int_type* refmai, int_type* idnoe, int_type* ier);

  void lmdebr_(int_type* refmai, int_type* idbras, int_type* ier);

  void lmdema_(int_type* refmai, int_type* idmai, int_type* ier);

  void lm3dema_(int_type* refmai, int_type* idmai, int_type* ier);

  void lmdenu_(int_type* refmai, char* nom, int_type* ier, int l_nom); 

  void lmdeli_(int_type* refmai, char* nom, int_type* ier, int l_nom); 

  void lmdesu_(int_type* refmai, char* nom, int_type* ier, int l_nom); 

  void lm3devo_(int_type* refmai, char* nom, int_type* ier, int l_nom); 

  void lmajbr_(int_type* refmai, char* nom, int_type* idbr, int_type* ier,
	       int l_nom);

  void lmrebr_(int_type* refmai, char* nom, int_type* idbr, int_type* ier,
	       int l_nom);

  void lmajpg_(int_type* refmai, char* nom, int_type* idpg, int_type* ier,
	       int l_nom);

  void lmrepg_(int_type* refmai, char* nom, int_type* idpg, int_type* ier,
	       int l_nom);

  void lm3ajpd_(int_type* refmai, char* nom, int_type* idpd, int_type* ier,
		int l_nom);

  void lm3repd_(int_type* refmai, char* nom, int_type* idpd, int_type* ier,
		int l_nom);

  void lmecmi_(char* _ficmai, int_type* refmai, int_type* ier, int l_ficmai);

  void lmprepa_(int_type* refmai, int_type* prepa, int_type* ier) ;
  void lmpretb_(int_type* refmai, int_type prepa_tab[], 
		int_type* nb_prepa, int_type* ier) ;

  void lmvertb_ (int_type* refmai, int_type verif_tab[], 
		int_type* nb_verif, int_type* ier) ;

  void lmatls_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
  	       char* _listenom, int_type listetype[], int_type* nbatt,
  	       int_type* ier, int l_nom_groupe, int l_listenom);
  
  void lmaten_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	       char* _nom_att, int_type tab_att[], int_type* nb_elt,
	       int_type* ier, int l_nom_groupe, int l_nom_att);

  void lmatre_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	       char* _nom_att, double tab_att[], int_type* nb_elt,
	       int_type* ier, int l_nom_groupe, int l_nom_att);

  void lmatv2_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	       char* _nom_att, double tab_att[], int_type* nb_elt,
	       int_type* ier, int l_nom_groupe, int l_nom_att);

  void lmatv3_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	       char* _nom_att, double tab_att[], int_type* nb_elt,
	       int_type* ier, int l_nom_groupe, int l_nom_att);

  void lmatch_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	       char* _nom_att, char* _tab_att, int_type* nb_elt,
	       int_type* ier, int l_nom_groupe, int l_nom_att, int l_tab_att);

  void lmcraten_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att, int_type* valtab, int_type* nbatt, int_type* ier,
		 int l_nom_groupe, int l_nom_att);

  void lmcratre_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att, double* valtab, int_type* nbatt, 
		 int_type* ier, int l_nom_groupe, int l_nom_att);

  void lmcratv2_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att, double* valtab, int_type* nbatt, 
		 int_type* ier, int l_nom_groupe, int l_nom_att);

  void lmcratv3_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att, double* valtab, int_type* nbatt, 
		 int_type* ier, int l_nom_groupe, int l_nom_att);

  void lmcratch_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att, char* valtab, int_type* nbatt, int_type* ier,
		 int l_nom_groupe, int l_nom_att, int l_valtab);

  void lmdeat_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att,  int_type* ier, int l_nom_groupe, int l_nom_att);

  void lmmoaten_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att, int_type* valtab, int_type* nbatt, int_type* ier,
		 int l_nom_groupe, int l_nom_att);

  void lmmoatre_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att, double* valtab, int_type* nbatt, int_type* ier,
		 int l_nom_groupe, int l_nom_att);

  void lmmoatv2_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att, double* valtab, int_type* nbatt, int_type* ier,
		 int l_nom_groupe, int l_nom_att);

  void lmmoatv3_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att, double* valtab, int_type* nbatt, int_type* ier,
		 int l_nom_groupe, int l_nom_att);

  void lmmoatch_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att, char* valtab, int_type* nbatt, int_type* ier,
		 int l_nom_groupe, int l_nom_att, int l_valtab);

}



#if !defined(_IBMR2) && !defined(CRAY) && !defined(__hpux)
using namespace Lima;
#else
#if __IBMCPP__ >= 500
using namespace Lima;
#endif	// #if __IBMCPP__ >= 500
#endif  // #if !defined(_IBMR2) && ...

/* lecture des generalites associees au maillage, le maillage etant
 * defini par son nom */
void lminnm_(char* _ficmai, int_type* refmai, char* _datmod, 
	     double* unlosi, int_type* typcoo, int_type* dimens, 
	     int_type* geomet, int_type* structure, int_type* ier, 
	     int l_ficmai, int l_datmod)
{
  try{   
    *ier = 0;
    string_fortran ficmai(_ficmai, l_ficmai);
    
    _MaillageInterne* mai = ouvrir_maillage(refmai);
    mai->dimension(D2);
    mai->lire(ficmai, SUFFIXE);
    mai->preparer(CODE_B);
    mai->verification_code (LM_VC_CODE_B);
#if 0
    mai->preparation_parametrable(LM_SUPPRIMER_DOUBLONS_BRAS|
				  LM_SUPPRIMER_DOUBLONS_POLYGONES|
				  LM_NOEUDS_BRAS|
				  LM_NOEUDS_POLYGONES|
				  LM_BRAS_POLYGONES|
				  LM_POLYGONES_BRAS|
				  LM_LIGNES_NOEUDS|
				  LM_SURFACES_NOEUDS|
				  LM_SURFACES_BRAS|
				  LM_CONTOURS_SURFACES|
				  LM_CONTOURS_LIGNES|
				  LM_CONTOURS_SURF_NOE|
				  LM_STRUCTURER_SURFACES|
				  LM_STRUCTURER_LIGNES|
				  LM_STRUCT_CONTOURS_SURF|
				  LM_COMPACTE|
				  LM_ORIENTATION | LM_FORMATER_NOMS);
#endif
    string_fortran date = mai->date().c_str();
    date.vers_fortran(_datmod, l_datmod);
    *unlosi = mai->unite_longueur();
    *typcoo = mai->type_coordonnee();
    *dimens = 2;
    *geomet = mai->type_geometrie();
    *structure = mai->structuration();
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture des generalites associees au maillage, le maillage etant
 * defini par une variable d'environnement */
void lminve_(char* _varfic, int_type* refmai, char* _datmod, 
	     double* unlosi, int_type* typcoo, int_type* dimens, 
	     int_type* geomet, int_type* structure, int_type* ier,
	     int l_varfic, int l_datmod)
{
  *ier = 0;
  string_fortran varfic(_varfic, l_varfic);
 
  char* ficmai=getenv(varfic);
  if(ficmai==NULL){
    fprintf(stderr, "Variable d'environnement %s pas trouve", 
	    (char*)varfic);
    *ier = -1;
    return;
  }
  lminnm_(ficmai, refmai, _datmod, unlosi, typcoo, dimens, geomet,
	  structure, ier, strlen(ficmai), l_datmod);
}

/* Lecture des generalites associees au maillage sans appel de la 
 * fonction preparer
 * Le maillage etant defini par son nom */
void lmlenm_(char* _ficmai, int_type* refmai, char* _datmod, 
	     double* unlosi, int_type* typcoo, int_type* dimens, 
	     int_type* geomet, int_type* structure, int_type* ier, 
	     int l_ficmai, int l_datmod)
{
  try{   
    *ier = 0;
    string_fortran ficmai(_ficmai, l_ficmai);
    
    _MaillageInterne* mai = ouvrir_maillage(refmai);

    mai->lire(ficmai, SUFFIXE);
    mai->formater_noms ( );		// CP, version 5.4.4
    mai->preparer(MINIMALE);
    string_fortran date = mai->date().c_str();
    date.vers_fortran(_datmod, l_datmod);
    *unlosi = mai->unite_longueur();
    *typcoo = mai->type_coordonnee();
    *dimens = mai->dimension();
    *geomet = mai->type_geometrie();
    *structure = mai->structuration();
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* Lecture des generalites associees au maillage sans appel de la 
 * fonction preparer
 * Le maillage etant defini par une variable d'environnement */
void lmleve_(char* _varfic, int_type* refmai, char* _datmod, 
	     double* unlosi, int_type* typcoo, int_type* dimens, 
	     int_type* geomet, int_type* structure, int_type* ier,
	     int l_varfic, int l_datmod)
{
  *ier = 0;
  string_fortran varfic(_varfic, l_varfic);
 
  char* ficmai=getenv(varfic);
  if(ficmai==NULL){
    fprintf(stderr, "Variable d'environnement %s pas trouve", 
	    (char*)varfic);
    *ier = -1;
    return;
  }
  lmlenm_(ficmai, refmai, _datmod, unlosi, typcoo, dimens, geomet,
	  structure, ier, strlen(ficmai), l_datmod);
}

/* lecture du nombre d'entites de base du maillage */
void lmindi_(int_type* refmai, int_type* nbrnoe, int_type* nbrbra,
	     int_type* nbrmai,  int_type* ier)
{
  try{
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);

    *nbrnoe = mai->nb_noeuds();
    *nbrbra = mai->nb_bras();
    *nbrmai = mai->nb_polygones();
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture du nombre de connexions entre les entites du maillage */
void lmnbcn_(int_type* refmai, int_type* maibra, int_type* mainoe,
	     int_type* branoe,  int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    *maibra = 
      nombre_connexions(mai->premier_polygone(), mai->dernier_polygone(),
			_les_bras<_PolygoneInterne>());
    *mainoe = 
      nombre_connexions(mai->premier_polygone(), mai->dernier_polygone(),
			_les_noeuds<_PolygoneInterne>());
    *branoe = 
      nombre_connexions(mai->premier_bras(), mai->dernier_bras(),
			_les_noeuds<_BrasInterne>());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_NomBre_NUage */
void lmnbnu_(int_type* refmai, int_type* nbrnua, int_type* ier)
{
  try{
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);

    *nbrnua = mai->nb_nuages();
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_NomBre_LIgne */
void lmnbli_(int_type* refmai, int_type* nbrlig, int_type* ier)
{
  try{
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);

    *nbrlig = mai->nb_lignes();
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_NomBre_SUrface */
void lmnbsu_(int_type* refmai, int_type* nbrsur, int_type* ier)
{
  try{
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);

    *nbrsur = mai->nb_surfaces();
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_NOeud */
void lmnoxy_(int_type* refmai, double* x, double* y, int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    for(_MaillageInterne::const_iterator_noeud ptr = mai->premier_noeud();
	ptr != mai->dernier_noeud(); ++ptr, ++x, ++y){
      *x = (*ptr)->x();
      *y = (*ptr)->y();
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_NOeud, coordonnees en simple precision */
void lmnox4_(int_type* refmai, float* x, float* y, int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

     for(_MaillageInterne::const_iterator_noeud ptr = mai->premier_noeud();
	ptr != mai->dernier_noeud(); ++ptr, ++x, ++y){
      *x = (*ptr)->x();
      *y = (*ptr)->y();
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_NOeud_BRas */
void lmnobr_(int_type* refmai, int_type* lnobr, 
	     int_type* pnobr, int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    lire_connexions(pnobr, lnobr, mai->premier_noeud(), 
		    mai->dernier_noeud(), _les_bras<_NoeudInterne>());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_NOeud_MAille */
void lmnoma_(int_type* refmai, int_type* lnoma, 
	     int_type* pnoma, int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    lire_connexions(pnoma, lnoma, mai->premier_noeud(), 
		    mai->dernier_noeud(), _les_polygones<_NoeudInterne>());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_BRas_NOeud */
void lmbrno_(int_type* refmai, int_type* noeori, 
	     int_type* noeext, int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    for(_MaillageInterne::const_iterator_bras b = mai->premier_bras(); 
	b != mai->dernier_bras(); ++b){
      *noeori++ = (*b)->noeud(0)->id();
      *noeext++ = (*b)->noeud(1)->id();
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_BRas_MAille */
void lmbrma_(int_type* refmai, int_type* maigau, 
	     int_type* maidro, int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    for(_MaillageInterne::const_iterator_bras b = mai->premier_bras(); 
	b != mai->dernier_bras(); ++b, ++maigau, ++maidro){
      switch((*b)->nb_polygones()){
      case 0:
	*maigau = 0;
	*maidro = 0;
	break;
      case 1:
	*maigau = (*b)->polygone(0)->id();
	*maidro = 0;
	break;
      case 2:      
	*maigau = (*b)->polygone(0)->id();
	*maidro = (*b)->polygone(1)->id();
	break;
      }
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_MAille_NOeud */
void lmmano_(int_type* refmai, int_type* lmano, 
	     int_type* pmano, int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    lire_connexions(pmano, lmano,  mai->premier_polygone(),
		    mai->dernier_polygone(), _les_noeuds<_PolygoneInterne>());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
  *ier = 0;
}

/* lecture_MAille_BRas */
void lmmabr_(int_type* refmai, int_type* lmabr, 
	     int_type* pmabr, int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);
    lire_connexions(pmabr, lmabr, mai->premier_polygone(),
		    mai->dernier_polygone(), _les_bras<_PolygoneInterne>());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* LiSte_des_NUages */
void lmnuls_(int_type* refmai, char* _nomnua, int_type* ier, int l_nomnua)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    for(_MaillageInterne::const_iterator_nuage n = mai->premier_nuage(); 
	n != mai->dernier_nuage(); ++n, _nomnua += l_nomnua){
      string_fortran nom = (*n)->nom().c_str();
      nom.vers_fortran(_nomnua, l_nomnua);
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
  *ier = 0;
}

/* EXiste_NUage */
void lmnuex_(int_type* refmai, char* _nomnua, int_type* nbrnoe,
	     int_type* ier, int l_nomnua)
{
  try{
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomnua(_nomnua, l_nomnua);
    _Groupe::formater_nom(nomnua);
    _NuageInterne* nuage = mai->nuage(nomnua);
    *nbrnoe = nuage->nb_noeuds(); 
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_NUage */
void lmnuno_(int_type* refmai, char* _nomnua, int_type* lnoe,
	     int_type* ier, int l_nomnua)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomnua(_nomnua, l_nomnua);
    _Groupe::formater_nom(nomnua);
    _NuageInterne* nuage = mai->nuage(nomnua);
    lire_elements(lnoe, nuage->premier_noeud(), nuage->dernier_noeud());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* LiSte_des_LIgnes */
void lmlils_(int_type* refmai, char* _nomlig, 
	     int_type* ier, int l_nomlig)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);
    
    for(_MaillageInterne::const_iterator_ligne l = mai->premier_ligne(); 
	l != mai->dernier_ligne(); ++l, _nomlig+=l_nomlig){
      string_fortran nom = (*l)->nom().c_str();
      nom.vers_fortran(_nomlig, l_nomlig);
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* EXiste_LIgne */
void lmliex_(int_type* refmai, char* _nomlig, int_type* nbrbra,
	     int_type* nbrnoe, int_type* ier, int l_nomlig)
{
  try{
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomlig(_nomlig, l_nomlig);
    _Groupe::formater_nom(nomlig);	
    _LigneInterne* ligne = mai->ligne(nomlig);
    *nbrbra = ligne->nb_bras(); 
    *nbrnoe = ligne->nb_noeuds(); 
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_LIgne_BRas */
void lmlibr_(int_type* refmai, char* _nomlig, int_type* lbra,
	     int_type* ier, int l_nomlig)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomlig(_nomlig, l_nomlig);
    _Groupe::formater_nom(nomlig);
    _LigneInterne* ligne = mai->ligne(nomlig);
    lire_elements(lbra, ligne->premier_bras(), ligne->dernier_bras());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_LIgne_NOeud */
void lmlino_(int_type* refmai, char* _nomlig, int_type* lnoe,
	     int_type* ier, int l_nomlig)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomlig(_nomlig, l_nomlig);
    _Groupe::formater_nom(nomlig);
    _LigneInterne* ligne = mai->ligne(nomlig);
    lire_elements(lnoe, ligne->premier_noeud(), ligne->dernier_noeud());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* LiSte_des_SUrfaces */
void lmsuls_(int_type* refmai, char* _nomsur, 
	     int_type* ier, int l_nomsur)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);
    
    for( _MaillageInterne::const_iterator_surface s = mai->premier_surface();
	s != mai->dernier_surface(); ++s, _nomsur+=l_nomsur){
      string_fortran nom = (*s)->nom().c_str();
      nom.vers_fortran(_nomsur, l_nomsur);
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* EXiste_SUrface (version 1) */
void lmsuex_(int_type* refmai, char* _nomsur, int_type* nbrmai,
	     int_type* lonlig, int_type* nbrbrc, int_type* nbrnoc, 
	     int_type* ier, int l_nomsur)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomsur(_nomsur, l_nomsur);
    _Groupe::formater_nom(nomsur);
    _SurfaceInterne* surface = mai->surface(nomsur);
    *nbrmai = surface->nb_polygones(); 
    *lonlig = surface->longueur_i();
    if(surface->contour()!=NULL){
      *nbrbrc = surface->contour()->nb_bras(); 
      *nbrnoc = surface->contour()->nb_noeuds(); 
    }   
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* EXiste_SUrface, bras-noeuds de la Surface (a partir version 2) */
void lmsues_(int_type* refmai, char* _nomsur, int_type* nbrmai,
	     int_type* lonlig, int_type* nbrbrs, int_type* nbrbrc,
	     int_type* nbrnos, int_type* nbrnoc, int_type* ier, 
	     int l_nomsur)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomsur(_nomsur, l_nomsur);
    _Groupe::formater_nom(nomsur);
    _SurfaceInterne* surface = mai->surface(nomsur);
    *nbrmai = surface->nb_polygones(); 
    *lonlig = surface->longueur_i();
    *nbrbrs = surface->nb_bras(); 
    *nbrnos = surface->nb_noeuds(); 
    if(surface->contour()!=NULL){
      *nbrbrc = surface->contour()->nb_bras(); 
      *nbrnoc = surface->contour()->nb_noeuds(); 
    }   
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_SUrface_MAille */
void lmsuma_(int_type* refmai, char* _nomsur, int_type* lmai,
	     int_type* ier, int l_nomsur)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomsur(_nomsur, l_nomsur);
    _Groupe::formater_nom(nomsur);
    _SurfaceInterne* surface = mai->surface(nomsur);
    lire_elements(lmai, surface->premier_polygone(), surface->dernier_polygone());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_SUrface_BRas */
void lmsubr_(int_type* refmai, char* _nomsur, int_type* lbrasu,
	     int_type* ier, int l_nomsur)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomsur(_nomsur, l_nomsur);
    _Groupe::formater_nom(nomsur);
    _SurfaceInterne* surface = mai->surface(nomsur);
    lire_elements(lbrasu, surface->premier_bras(), surface->dernier_bras());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_COntour_surface_BRas */
void lmcobr_(int_type* refmai, char* _nomsur, int_type* lbraco,
	     int_type* ier, int l_nomsur)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomsur(_nomsur, l_nomsur);
    _Groupe::formater_nom(nomsur);
    _SurfaceInterne* surface = mai->surface(nomsur);
    _LigneInterne* contour= surface->contour();
    if(contour!=NULL)
      lire_elements(lbraco, contour->premier_bras(), contour->dernier_bras());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_SUrface_NOeud */
void lmsuno_(int_type* refmai, char* _nomsur, int_type* lnoesu,
	     int_type* ier, int l_nomsur)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomsur(_nomsur, l_nomsur);
    _Groupe::formater_nom(nomsur);
    _SurfaceInterne* surface = mai->surface(nomsur);
    lire_elements(lnoesu, surface->premier_noeud(), surface->dernier_noeud());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* lecture_COntour_surface_NOeud */
void lmcono_(int_type* refmai, char* _nomsur, int_type* lnoeco,
	     int_type* ier, int l_nomsur)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomsur(_nomsur, l_nomsur);
    _Groupe::formater_nom(nomsur);
    _SurfaceInterne* surface = mai->surface(nomsur);
    _LigneInterne* contour = surface->contour();
    if(contour!=NULL)
      lire_elements(lnoeco, contour->premier_noeud(), contour->dernier_noeud());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }  
}

/* FIN_lecture_maillage */
void lmfin_(int_type* refmai, int_type* ier)
{
  *ier = 0; 
  fermer_maillage(*refmai);
  *refmai = 0;
}

/* anciennement 'modification numero unite de sortie des MESSages', ne
 * fait rien, presente pour la compatibilite avec la version 2 */
void lmmess_(int *)
{
}

// EXPURGE_BEGINNING_TAG USE_LOCAL_DEPENDENCIES
// EXPURGE_COMPLETION_TAG

/* CReation d'un Maillage */
void lmcrmi_(int_type* refmai, char* _titre, char* _datmod, 
	     double* unlosi, int_type* typcoo, int_type* dimens, 
	     int_type* geomet, int_type* ier, 
	     int l_titre, int l_datmod)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = ouvrir_maillage(refmai);
    
    string_fortran titre(_titre, l_titre);
    mai->titre(titre);
    string_fortran datmod(_datmod, l_datmod);
    mai->date(datmod);
    mai->unite_longueur(*unlosi);
    mai->type_coordonnee(coordonnee_t(*typcoo));
    mai->dimension(dim_t(*dimens));
    mai->type_geometrie(geometrie_t(*geomet));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }    
}

/* CReation de NOeuds en 1D ou 2D */
void lmcrno_(int_type* refmai, double* x, double* y, int_type* nbnoe,
	     int_type* ier)
{
  try{
    size_type iter;
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);
    switch(mai->dimension()){
    case D1:
      for (iter = 0 ; iter < *nbnoe ; iter++)
	{
	  mai->ajouter(_NoeudInterne::create(iter+1, x[iter]));
	}
      break;
    case D2:
      for (iter = 0 ; iter < *nbnoe ; iter++)
	{
	  mai->ajouter(_NoeudInterne::create(iter+1, x[iter], y[iter])); 
	}
      break;
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }  
}

/* CReation de NOeuds en 1D ou 2D (simple precision Fortran)*/
void lmcrno4_(int_type* refmai, float* x, float* y, int_type* nbnoe,
	      int_type* ier)
{
  try{
    size_type iter;
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);
    switch(mai->dimension()){
    case D1:
      for ( iter = 0 ; iter < *nbnoe ; iter++)
	{
	  mai->ajouter(_NoeudInterne::create(iter+1, (double)x[iter]));

	}
      break;
    case D2:
      for ( iter = 0 ; iter < *nbnoe ; iter++)
	{
	  // La fonction 'create' de _NoeudInterne ne prend en compte
	  // que des doubles.
	  mai->ajouter(_NoeudInterne::create(iter+1, (double)x[iter], (double)y[iter])); 
	}
      break;
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }  
}

/* CReation de NOeuds en 3D */
void lm3crno_(int_type* refmai, double* x, double* y, double* z,
		int_type* nbnoe, int_type* ier)
{
  try{
    int iter;
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);
    
    if (mai->dimension() == D3){
      for ( iter = 0 ; iter < *nbnoe ; iter++){
	mai->ajouter(_NoeudInterne::create(iter+1, x[iter], y[iter], z[iter]));
      }
    }
    else{
      fputs("Configuration 3D requise \n", stderr);
      *ier = -1;
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }  
}

/* CReation de NOeuds en 3D (simple precision Fortran)*/
void lm3crno4_(int_type* refmai, float* x, float* y, float* z,
		 int_type* nbnoe, int_type* ier)
{
  try{
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);
    
    if (mai->dimension() == D3){
      for (int iter = 0 ; iter < *nbnoe ; iter++){
	// La fonction 'create' de _NoeudInterne ne prend en compte
	// que des doubles.
	mai->ajouter(_NoeudInterne::create(iter+1, (double)x[iter], (double)y[iter], (double)z[iter]));
      }
    }
    else{
      fputs("Configuration 3D requise \n", stderr);
      *ier = -1;
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }  
}

/* CReation de BRas */
void lmcrbr_(int_type* refmai, int_type* noeori, int_type* noeext,
	     int_type* nbbras, int_type* ier)
{
  try{
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);

    for (int iter = 0 ; iter < *nbbras ; iter++)
      {
	_NoeudInterne* n1 = mai->noeud_id(noeori[iter]);
	_NoeudInterne* n2 = mai->noeud_id(noeext[iter]);
	//
	mai->ajouter(_BrasInterne::create(iter+1, n1, n2));
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* CReation de MAilles 2D (polygones) */
void lmcrma_(int_type* refmai, int_type* lmano, int_type* pmano,
	     int_type* nbmai, int_type* ier)
{
  try{
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);

    for (int iter = 0 ; iter < *nbmai ; iter++)
      {
	// Calcul du nbre de noeuds definissant la maille
	int_type nbnoe = 0;
	nbnoe = pmano[iter+1] - pmano[iter];

	// REMARQUE : on soustrait 1 au indice donne par pmano de facon
	// a transformer l'indice fortran en indice C

	// Les noeuds communs aux differentes formes
	_NoeudInterne* n1 = mai->noeud_id(lmano[pmano[iter]-1]);
	_NoeudInterne* n2 = mai->noeud_id(lmano[pmano[iter]]);
	_NoeudInterne* n3 = mai->noeud_id(lmano[pmano[iter]+1]);

	// Les triangles
	if (nbnoe == 3)
	  {
	    mai->ajouter(_PolygoneInterne::create(iter+1, n1, n2, n3));
	  }
	// Les quadrangles
	else if (nbnoe == 4)
	  {
	    _NoeudInterne* n4 = mai->noeud_id(lmano[pmano[iter]+2]);
	    //
	    mai->ajouter(_PolygoneInterne::create(iter+1, n1, n2, n3, n4));
	  }
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }  
}

/* CReation de MAilles 3D (polyedres) */
void lm3crma_(int_type* refmai, int_type* lmano, int_type* pmano,
	      int_type* nbmai, int_type* ier)
{
  try{
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);

    for (int iter = 0 ; iter < *nbmai ; iter++)
      {
	// Calcul du nbre de noeuds definissant la maille
	int_type nbnoe = 0;
	nbnoe = pmano[iter+1] - pmano[iter];

	// REMARQUE : on soustrait 1 au indice donne par pmano de facon
	// a transformer l'indice fortran en indice C

	// Les noeuds communs aux differentes formes
	_NoeudInterne* n1 = mai->noeud_id(lmano[pmano[iter]-1]);
	_NoeudInterne* n2 = mai->noeud_id(lmano[pmano[iter]]);
	_NoeudInterne* n3 = mai->noeud_id(lmano[pmano[iter]+1]);
	_NoeudInterne* n4 = mai->noeud_id(lmano[pmano[iter]+2]);

	// Les tetraedres
	if (nbnoe == 4)
	  {
	    mai->ajouter(_PolyedreInterne::create(iter+1, n1, n2, n3, n4));
	  }
	// Les pyramides
	else if (nbnoe == 5)
	  {
	    _NoeudInterne* n5 = mai->noeud_id(lmano[pmano[iter]+3]);
	    //
	    mai->ajouter(_PolygoneInterne::create(iter+1, n1, n2, n3, n4, n5));
	  }
	// Les prismes
	else if (nbnoe == 6)
	  {
	    _NoeudInterne* n5 = mai->noeud_id(lmano[pmano[iter]+3]);
	    _NoeudInterne* n6 = mai->noeud_id(lmano[pmano[iter]+4]);
	    //
	    mai->ajouter(_PolyedreInterne::create(iter+1, n1, n2, n3, n4, n5, n6));
	  }
	// Les hexaedres
	else if (nbnoe == 8)
	  {
	    _NoeudInterne* n5 = mai->noeud_id(lmano[pmano[iter]+3]);
	    _NoeudInterne* n6 = mai->noeud_id(lmano[pmano[iter]+4]);
	    _NoeudInterne* n7 = mai->noeud_id(lmano[pmano[iter]+5]);
	    _NoeudInterne* n8 = mai->noeud_id(lmano[pmano[iter]+6]);
	    //
	    mai->ajouter(_PolyedreInterne::create(iter+1, n1, n2, n3, n4, n5, n6, n7, n8));
	  }
	// Les heptaedres
	else if (nbnoe == 10)
	  {
	    _NoeudInterne* n5  = mai->noeud_id(lmano[pmano[iter]+3]);
	    _NoeudInterne* n6  = mai->noeud_id(lmano[pmano[iter]+4]);
	    _NoeudInterne* n7  = mai->noeud_id(lmano[pmano[iter]+5]);
	    _NoeudInterne* n8  = mai->noeud_id(lmano[pmano[iter]+6]);
	    _NoeudInterne* n9  = mai->noeud_id(lmano[pmano[iter]+7]);
	    _NoeudInterne* n10 = mai->noeud_id(lmano[pmano[iter]+8]);
	    //
	    mai->ajouter(_PolyedreInterne::create(iter+1, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10));
	  }
	// Les octaedres
	else if (nbnoe == 12)
	  {
	    _NoeudInterne* n5  = mai->noeud_id(lmano[pmano[iter]+3]);
	    _NoeudInterne* n6  = mai->noeud_id(lmano[pmano[iter]+4]);
	    _NoeudInterne* n7  = mai->noeud_id(lmano[pmano[iter]+5]);
	    _NoeudInterne* n8  = mai->noeud_id(lmano[pmano[iter]+6]);
	    _NoeudInterne* n9  = mai->noeud_id(lmano[pmano[iter]+7]);
	    _NoeudInterne* n10 = mai->noeud_id(lmano[pmano[iter]+8]);
	    _NoeudInterne* n11 = mai->noeud_id(lmano[pmano[iter]+9]);
	    _NoeudInterne* n12 = mai->noeud_id(lmano[pmano[iter]+10]);
	    //
	    mai->ajouter(_PolyedreInterne::create(iter+1, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12));
	  }
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }  
}

/* CReation d'un NUage */
void lmcrnu_(int_type* refmai, char* nom, int_type* idnoe, int_type* nbnoe,
	     int_type* ier, int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);

    // Nombre de nuages du maillage
    int nbnu = mai->nb_nuages();

    // Creation du nuage
    _NuageInterne* nu = new _NuageInterne(nom_groupe);
    mai->ajouter(nu);
    nu->ordre(nbnu);     // La numerotation partant de 0

    // Ajout des noeuds au nuage
    for (int iter = 0 ; iter < *nbnoe ; iter++)
      {
	//	mai->nuage(nom_groupe)->ajouter(mai->noeud_id(idnoe[iter]));
	mai->nuage_ordre(nbnu)->ajouter(mai->noeud_id(idnoe[iter]));
      }
    nu->nb_elements(*nbnoe);   
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* CReation d'une LIgne */
void lmcrli_(int_type* refmai, char* nom, int_type* idbr, int_type* nbbr,
	     int_type* ier, int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);

    // Nombre des lignes du maillage
    int nbli = mai->nb_lignes();

    // Creation de la ligne
    _LigneInterne* li = new _LigneInterne(nom_groupe);
    mai->ajouter(li);
    li->ordre(nbli);     // La numerotation partant de 0

    // Ajout des bras a la ligne
    for (int iter = 0 ; iter < *nbbr ; iter ++)
      {
	mai->ligne(nbli)->ajouter(mai->bras_id(idbr[iter]));
      }
    li->nb_elements(*nbbr);   
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* CReation d'une SUrface */
void lmcrsu_(int_type* refmai, char* nom, int_type* idpg, int_type* nbpg,
	     int_type* ier, int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);

    // Nombre des surfaces du maillage
    int nbsu = mai->nb_surfaces();

    // Creation de la surface
    _SurfaceInterne* su = new _SurfaceInterne(nom_groupe);
    mai->ajouter(su);
    su->ordre(nbsu);     // La numerotation partant de 0

    // Ajout des polygones a la surface
    for (int iter = 0 ; iter < *nbpg ; iter ++)
      {
	mai->surface(nbsu)->ajouter(mai->polygone_id(idpg[iter]));
      }
    su->nb_elements(*nbpg);   
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* CReation d'un VOlume */
void lm3crvo_(int_type* refmai, char* nom, int_type* idpd, int_type* nbpd,
	      int_type* ier, int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);

    // Nombre des volumes du maillage
    int nbvo = mai->nb_volumes();

    // Creation du volume
    _VolumeInterne* vo = new _VolumeInterne(nom_groupe);
    mai->ajouter(vo);
    vo->ordre(nbvo);     // La numerotation partant de 0

    // Ajout des polyedres au volume
    for (int iter = 0 ; iter < *nbpd ; iter ++)
      {
	mai->volume(nbvo







)->ajouter(mai->polyedre_id(idpd[iter]));
      }
    vo->nb_elements(*nbpd);   
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* DEstruction d'un NOeud 2D/3D */
void lmdeno_(int_type* refmai, int_type* idnoe, int_type* ier)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Le noeud a detruire est defini par son Id
    mai->retirer(mai->noeud_id(*idnoe));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* DEstruction d'un BRas */
void lmdebr_(int_type* refmai, int_type* idbras, int_type* ier)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Le bras a detruire est defini par son Id
    mai->retirer(mai->bras_id(*idbras));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* DEstruction d'une MAille 2D (polygone) */
void lmdema_(int_type* refmai, int_type* idmai, int_type* ier)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Le polygone a detruire est defini par son Id
    mai->retirer(mai->polygone_id(*idmai));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* DEstruction d'une MAille 3D (polyedre) */
void lm3dema_(int_type* refmai, int_type* idmai, int_type* ier)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Le polyedre a detruire est defini par son Id
    mai->retirer(mai->polyedre_id(*idmai));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* DEstruction d'un NUage */
void lmdenu_(int_type* refmai, char* nom, int_type* ier, int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);
    //
    mai->retirer(mai->nuage(nom_groupe));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* DEstruction d'une LIgne */
void lmdeli_(int_type* refmai, char* nom, int_type* ier, int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);
    //
    mai->retirer(mai->ligne(nom_groupe));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* DEstruction d'une SUrface */
void lmdesu_(int_type* refmai, char* nom, int_type* ier, int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);
    //
    mai->retirer(mai->surface(nom_groupe));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
} 

/* DEstruction d'un VOlume */
void lm3devo_(int_type* refmai, char* nom, int_type* ier, int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);
    //
    mai->retirer(mai->volume(nom_groupe));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* AJout d'un NOeud dans un nuage */
void lmajno_(int_type* refmai, char* nom, int_type* idnoe, int_type* ier,
	     int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);
    //
    mai->nuage(nom_groupe)->ajouter(mai->noeud_id(*idnoe));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* Retrait d'un NOeud dans un nuage */
void lmreno_(int_type* refmai, char* nom, int_type* idnoe, int_type* ier,
	     int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);
    //
    mai->nuage(nom_groupe)->retirer(mai->noeud_id(*idnoe));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* AJout d'un BRas dans une ligne */
void lmajbr_(int_type* refmai, char* nom, int_type* idbr, int_type* ier,
	     int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);
    //
    mai->ligne(nom_groupe)->ajouter(mai->bras_id(*idbr));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* Retrait d'un BRas dans une ligne */
void lmrebr_(int_type* refmai, char* nom, int_type* idbr, int_type* ier,
	     int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);
    //
    mai->ligne(nom_groupe)->retirer(mai->bras_id(*idbr));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* AJout d'un PolyGone dans une surface */
void lmajpg_(int_type* refmai, char* nom, int_type* idpg, int_type* ier,
	     int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);
    //
    mai->surface(nom_groupe)->ajouter(mai->polygone_id(*idpg));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* Retrait d'un PolyGone dans une surface */
void lmrepg_(int_type* refmai, char* nom, int_type* idpg, int_type* ier,
	     int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);
    //
    mai->surface(nom_groupe)->retirer(mai->polygone_id(*idpg));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* AJout d'un PolyeDre dans un volume */
void lm3ajpd_(int_type* refmai, char* nom, int_type* idpd, int_type* ier,
	      int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);
    //
    mai->volume(nom_groupe)->ajouter(mai->polyedre_id(*idpd));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* Retrait d'un PolyeDre dans un volume */
void lm3repd_(int_type* refmai, char* nom, int_type* idpd, int_type* ier,
	      int l_nom)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(nom, l_nom);
    //
    mai->volume(nom_groupe)->retirer(mai->polyedre_id(*idpd));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* ECriture du MaIllage (en memoire) dans un fichier */
void lmecmi_(char* _ficmai, int_type* refmai, int_type* ier, int l_ficmai)
{
  try{   
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);
    
    string_fortran ficmai(_ficmai, l_ficmai);
    mai->ecrire(ficmai);
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}  

/* Fonction de transformation de char* en int_type (prototype)
 * Elle est appelee par lmatls */
int_type char2enum(char* chaine_car);

/* lecture de la LiSte des noms des ATtributs et de leurs types */
void lmatls_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	     char* _listenom, int_type listetype[], int_type* nbatt,
	     int_type* ier, int l_nom_groupe, int l_listenom)
{
  try{
    *ier = 0;
    vector<string>::iterator iter;

    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo de la chaine "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    
    // Definition du vecteur intermediaire qui recoit la liste des attributs
    vector<string> tmp_liste_att;

    // Construction de la liste des noms des attributs
    tmp_liste_att=mai->get_pt_attache(*pt_attach, nom_groupe).liste();

    int i = 0;
    for (iter = tmp_liste_att.begin() ; 
	 iter != tmp_liste_att.end() ; 
	 iter++ , i++)
      {
	listetype[i] = mai->get_pt_attache(*pt_attach, nom_groupe).element(iter->c_str()).type();
      }


    /* Longueur du vecteur de nom d'attributs */
    *nbatt = tmp_liste_att.size();

    /* Ecriture des noms des attributs dans _listenom en format
     * d'un tableau de chaines de caracteres Fortran */
    int indice = 0;
    
    for (iter = tmp_liste_att.begin() ; 
	 iter != tmp_liste_att.end() ; iter++)
      {
	char* tmp_chaine	= new char[256];
	strncpy(tmp_chaine, (char*)iter->c_str(), l_listenom);

	for (int jter = 0 ; jter < l_listenom ; jter++)
	  {
	    _listenom[indice] = tmp_chaine[jter];
	    indice++;
	  }
	
	delete [] tmp_chaine;	// CP, v 6.27.1, coverity
      }
	
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* lecture d'un ATtribut ENtier */
void lmaten_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	     char* _nom_att, int_type tab_att[], int_type* nb_elt,
	     int_type* ier, int l_nom_groupe, int l_nom_att)
{
  try{
    *ier = 0;

    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // 
    *nb_elt = mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).nb_elements();
    //
    // Variable intermediaire
    int tmp_var = 0;
    for (int iter = 0 ; iter < *nb_elt ; iter++)
      {
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).get_val(iter,tmp_var);
	tab_att[iter] = tmp_var;
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }

}

/* lecture d'un ATtribut REel */
void lmatre_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	     char* _nom_att, double tab_att[], int_type* nb_elt,
	     int_type* ier, int l_nom_groupe, int l_nom_att)
{
  try{
    *ier = 0;

    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // 
    *nb_elt = mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).nb_elements();
    //
    // Variable intermediaire
    double tmp_var = 0.;
    for (int iter = 0 ; iter < *nb_elt ; iter++)
      {
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).get_val(iter,tmp_var);
	tab_att[iter] = tmp_var;
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }

}

/* lecture d'un ATtribut Vecteur 2D */
void lmatv2_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	     char* _nom_att, double tab_att[], int_type* nb_elt,
	     int_type* ier, int l_nom_groupe, int l_nom_att)
{
  try{
    *ier = 0;

    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // 
    *nb_elt = mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).nb_elements();
    //
    // Variable intermediaire
    Vect2d tmp_var;
    for (int iter = 0 ; iter < *nb_elt ; iter++)
      {
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).get_val(iter,tmp_var);
	tab_att[2*iter] = tmp_var.x();
	tab_att[2*iter+1] = tmp_var.y();
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }

}

/* lecture d'un ATtribut Vecteur 3D */
void lmatv3_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	     char* _nom_att, double tab_att[], int_type* nb_elt,
	     int_type* ier, int l_nom_groupe, int l_nom_att)
{

  try{
    *ier = 0;

    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // 
    *nb_elt = mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).nb_elements();
    //
    // Variable intermediaire
    Vect3d tmp_var;
    for (int iter = 0 ; iter < *nb_elt ; iter++)
      {
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).get_val(iter,tmp_var);
	tab_att[3*iter]   = tmp_var.x();
	tab_att[3*iter+1] = tmp_var.y();
	tab_att[3*iter+2] = tmp_var.z();
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }

}

/* lecture d'un ATtribut CHaine de caracteres */
void lmatch_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	       char* _nom_att, char* _tab_att, int_type* nb_elt,
	       int_type* ier, int l_nom_groupe, int l_nom_att, int l_tab_att)
{
  try{
    *ier = 0;

    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // 
    *nb_elt = mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).nb_elements();
    //
    int   indice  = 0;
    for (int iter = 0 ; iter < *nb_elt ; iter++)
      {
	// Variables intermediaires
//	char* tmp_var1;
//	char* tmp_var2;
//	tmp_var1 = new char[256];
//	tmp_var2 = new char[256];
    string	tmp_var1;

	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).get_val(iter,tmp_var1);
    size_t	length	= tmp_var1.length ( ) < l_tab_att ? 
                      tmp_var1.length ( ) : l_tab_att;
//	strncpy(tmp_var2, tmp_var1, l_tab_att);

	for (int jter = 0 ; jter < l_tab_att ; jter++)
	  {
		if (jter < length)
			_tab_att[indice] = tmp_var1[jter];
		else
			_tab_att[indice] = '\0';
	    indice++;
	  }
	  
//	delete tmp_var1;
//	delete tmp_var2;
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }

}

/* Fonction d'initialisation des attributs (prototype)
 * Elle appelle la bonne fonction d'initialisation selon le pt d'attache */
void Init_attribut(_MaillageInterne* mai, att_loc  pt_attach, string nom_groupe);

/* CReation d'un ATtribut ENtier */
void lmcraten_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	       char* _nom_att, int_type* valtab, int_type* nbatt, int_type* ier,
	       int l_nom_groupe, int l_nom_att)
{
  try{
    *ier = 0;
    _Attribut* att;
    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // Creation et initialisation de l'attribut a ajouter
    att = new _AttributInterne<int>(nom_att);
    Init_attribut(mai, *pt_attach, nom_groupe);
    mai->get_pt_attache(*pt_attach, nom_groupe).attacher(att);

    // Valorisation de l'attribut
    if (*nbatt == 1)
      {
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(int(valtab[0]));
      }
    else if (*nbatt == mai->get_pt_attache(*pt_attach, nom_groupe).max_val())
      {
	for (int iter = 0 ; iter < *nbatt ; iter++)
	  {
	    mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(iter,int(valtab[iter]));
	  }
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* CReation d'un ATtribut REel */
void lmcratre_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	       char* _nom_att, double* valtab, int_type* nbatt, int_type* ier,
	       int l_nom_groupe, int l_nom_att)
{
  try{
    *ier = 0;
    _Attribut* att;
    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // Creation et initialisation de l'attribut a ajouter
    att = new _AttributInterne<double>(nom_att);
    Init_attribut(mai, *pt_attach, nom_groupe);
    mai->get_pt_attache(*pt_attach, nom_groupe).attacher(att);

    // Valorisation de l'attribut
    if (*nbatt == 1)
      {
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(valtab[0]);
      }
    else if (*nbatt == mai->get_pt_attache(*pt_attach, nom_groupe).max_val())
      {
	for (int iter = 0 ; iter < *nbatt ; iter++)
	  {
	    mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(iter,valtab[iter]);
	  }
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* CReation d'un ATtribut Vecteur 2D */
void lmcratv2_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att, double* valtab, int_type* nbatt, 
		 int_type* ier, int l_nom_groupe, int l_nom_att)
{
  try{
    *ier = 0;
    Vect2d vec2;
    _Attribut* att;
    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // Creation et initialisation de l'attribut a ajouter
    att = new _AttributInterne<Vect2d>(nom_att);
    Init_attribut(mai, *pt_attach, nom_groupe);
    mai->get_pt_attache(*pt_attach, nom_groupe).attacher(att);

    // Valorisation de l'attribut
    if (*nbatt == 1)
      {
	vec2.set_x(valtab[0]);
	vec2.set_y(valtab[1]);
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(vec2);
      }
    else if (*nbatt == mai->get_pt_attache(*pt_attach, nom_groupe).max_val())
      {
	for (int iter = 0 ; iter < *nbatt ; iter++)
	  {
	    vec2.set_x(valtab[2*iter]);
	    vec2.set_y(valtab[2*iter+1]);
	    mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(iter,vec2);
	  }
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* CReation d'un ATtribut Vecteur 3D */
void lmcratv3_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att, double* valtab, int_type* nbatt, 
		 int_type* ier, int l_nom_groupe, int l_nom_att)
{
  try{
    *ier = 0;
    Vect3d vec3;
    _Attribut* att;
    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // Creation et initialisation de l'attribut a ajouter
    att = new _AttributInterne<Vect3d>(nom_att);
    Init_attribut(mai, *pt_attach, nom_groupe);
    mai->get_pt_attache(*pt_attach, nom_groupe).attacher(att);

    // Valorisation de l'attribut
    if (*nbatt == 1)
      {
	vec3.set_x(valtab[0]);
	vec3.set_y(valtab[1]);
	vec3.set_z(valtab[2]);
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(vec3);
      }
    else if (*nbatt == mai->get_pt_attache(*pt_attach, nom_groupe).max_val())
      {
	for (int iter = 0 ; iter < *nbatt ; iter++)
	  {
	    vec3.set_x(valtab[3*iter]);
	    vec3.set_y(valtab[3*iter+1]);
	    vec3.set_z(valtab[3*iter+2]);
	    mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(iter,vec3);
	  }
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* CReation d'un ATtribut CHaine de caracteres */
void lmcratch_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	       char* _nom_att, char* valtab, int_type* nbatt, int_type* ier,
	       int l_nom_groupe, int l_nom_att, int l_valtab)
{
  try{
    *ier = 0;
    _Attribut* att;
    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // Creation et initialisation de l'attribut a ajouter
    att = new _AttributInterne<string>(nom_att);
    Init_attribut(mai, *pt_attach, nom_groupe);
    mai->get_pt_attache(*pt_attach, nom_groupe).attacher(att);

    // Valorisation de l'attribut
    char* tmp_var	= 0;	// CP, v 6.27.1
    if (*nbatt == 1)
      {
	tmp_var = new char[l_valtab+1];
	strncpy(tmp_var, valtab, l_valtab);
	tmp_var[l_valtab]='\0';
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(tmp_var);
	delete [] tmp_var;	// CP, v 6.27.1, coverity

//	char* retour	= new char[50];
	string	retour;
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).get_val(retour);
      }
    else if (*nbatt == mai->get_pt_attache(*pt_attach, nom_groupe).max_val())
      {
	for (int iter = 0 ; iter < *nbatt ; iter++)
	  {
	    tmp_var = new char[l_valtab+1];
	    strncpy(tmp_var, (valtab + iter*l_valtab), l_valtab);
	    tmp_var[l_valtab]='\0';
	    mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(iter,tmp_var);
	    delete [] tmp_var;	// CP, v 6.27.1, coverity

//	    char* retour= new char[50];
		string	retour;
	    mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).get_val(iter,retour);
	  }
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* suppression (DElete) d'un ATtribut */
void lmdeat_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	     char* _nom_att, int_type* ier, int l_nom_groupe, int l_nom_att)
{
  try{
    *ier = 0;
    _Attribut* att;
    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    att = mai->get_pt_attache(*pt_attach, nom_groupe).detacher(nom_att);
    delete att;
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* MOdification d'un ATtribut ENtier */
void lmmoaten_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	       char* _nom_att, int_type* valtab, int_type* nbatt, int_type* ier,
	       int l_nom_groupe, int l_nom_att)
{
  try{
    *ier = 0;

    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // Modification
    if (*nbatt == 1)
      {
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(int(valtab[0]));
      }
    else if (*nbatt == mai->get_pt_attache(*pt_attach, nom_groupe).max_val())
      {
	for (int iter = 0 ; iter < *nbatt ; iter++)
	  {
	    mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(iter,int(valtab[iter]));
	  }
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* MOdification d'un ATtribut REel */
void lmmoatre_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	       char* _nom_att, double* valtab, int_type* nbatt, int_type* ier,
	       int l_nom_groupe, int l_nom_att)
{
  try{
    *ier = 0;

    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // Modification
    if (*nbatt == 1)
      {
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(valtab[0]);
      }
    else if (*nbatt == mai->get_pt_attache(*pt_attach, nom_groupe).max_val())
      {
	for (int iter = 0 ; iter < *nbatt ; iter++)
	  {
	    mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(iter,valtab[iter]);
	  }
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* MOdification d'un ATtribut Vecteur 2D */
void lmmoatv2_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	       char* _nom_att, double* valtab, int_type* nbatt, int_type* ier,
	       int l_nom_groupe, int l_nom_att)
{
  try{
    *ier = 0;
    Vect2d vec2;
    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // Modification
    if (*nbatt == 1)
      {
	vec2.set_x(valtab[0]);
	vec2.set_y(valtab[1]);
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(vec2);
      }
    else if (*nbatt == mai->get_pt_attache(*pt_attach, nom_groupe).max_val())
      {
	for (int iter = 0 ; iter < *nbatt ; iter++)
	  {
	    vec2.set_x(valtab[2*iter]);
	    vec2.set_y(valtab[2*iter+1]);
	    mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(iter,vec2);
	  }
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* MOdification d'un ATtribut Vecteur 3D */
void lmmoatv3_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
	       char* _nom_att, double* valtab, int_type* nbatt, int_type* ier,
	       int l_nom_groupe, int l_nom_att)
{
  try{
    *ier = 0;
    Vect3d vec3;
    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // Modification
    if (*nbatt == 1)
      {
	vec3.set_x(valtab[0]);
	vec3.set_y(valtab[1]);
	vec3.set_z(valtab[2]);
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(vec3);
      }
    else if (*nbatt == mai->get_pt_attache(*pt_attach, nom_groupe).max_val())
      {
	for (int iter = 0 ; iter < *nbatt ; iter++)
	  {
	    vec3.set_x(valtab[2*iter]);
	    vec3.set_y(valtab[2*iter+1]);
	    vec3.set_z(valtab[2*iter+2]);
	    mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(iter,vec3);
	  }
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* MOdification d'un ATtribut CHaine de caracteres */
void lmmoatch_(int_type* refmai, att_loc* pt_attach, char* _nom_groupe,
		 char* _nom_att, char* valtab, int_type* nbatt, int_type* ier,
		 int l_nom_groupe, int l_nom_att, int l_valtab)
{
  try{
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);
 
    // Transfo des chaines "Fortran"
    string_fortran nom_groupe(_nom_groupe, l_nom_groupe);
    string_fortran nom_att(_nom_att, l_nom_att);

    // Modification
    char* tmp_var	= 0;	// CP, v 6.27.1
    if (*nbatt == 1)
      {
        tmp_var = new char[l_valtab+1];
	strncpy(tmp_var, valtab, l_valtab);
	tmp_var[l_valtab]='\0';
	mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(tmp_var);
	delete [] tmp_var;	// CP, v 6.27.1, coverity
      }
    else if (*nbatt == mai->get_pt_attache(*pt_attach, nom_groupe).max_val())
      {
	for (int iter = 0 ; iter < *nbatt ; iter++)
	  {
            tmp_var = new char[l_valtab+1];
	    strncpy(tmp_var, (valtab + iter*l_valtab), l_valtab);
	    tmp_var[l_valtab]='\0';
	    mai->get_pt_attache(*pt_attach, nom_groupe).element(nom_att).set_val(iter,tmp_var);
	    delete [] tmp_var;	// CP, v 6.27.1, coverity
	  }
      }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* PREPAration d'un maillage */
void lmprepa_(int_type* refmai, int_type* prepa, int_type* ier)
{
  try{
    *ier = 0;
    
    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Appel de la fonction 'preparer' de _MaillageInterne
    mai->preparer(preparation_t(*prepa));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* PREPAration d'un maillage */
void lmpretb_(int_type* refmai, int_type prepa_tab[], int_type* nb_prepa, 
	      int_type* ier)
{
  try{
    *ier = 0;

    _MaillageInterne* mai = trouver_maillage(*refmai);

    // Initialisation de la preparation de type TypeMasque
    TypeMasque prepa = 0 ;

    for(int iter = 0 ; iter < *nb_prepa ; iter++){
      TypeMasque tmp = 1LL<<prepa_tab[iter];

      prepa = prepa | tmp ;
    }
    
    // Appel de la fonction 'preparer' de _MaillageInterne
    mai->preparation_parametrable(prepa, true);
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inattendue\n", stderr);    
    *ier = -1;
  }
}

/* VERification d'un maillage */
void lmvertb_ (int_type* refmai, int_type verif_tab[], int_type* nb_verif, 
	           int_type* ier)
{
	try
	{
		*ier	= 0;

		_MaillageInterne*	mai = trouver_maillage (*refmai);

		// Initialisation de la preparation de type TypeMasque
		TypeMasqueVerification	verif	= 0 ;

		for (int iter = 0 ; iter < *nb_verif ; iter++)
		{
			TypeMasqueVerification tmp = 1LL << verif_tab [iter];

			verif |= tmp ;
		}
    
		// Appel de la fonction 'verification_parametrable' de _MaillageInterne
		mai->verification_parametrable (verif);
	}
	catch (const erreur& err)
	{
		fputs (err.what ( ), stderr);
		*ier	= -1;
	}
	catch (...)
	{
		fputs("Erreur inattendue\n", stderr);    
		*ier	= -1;
	}
}	// lmvertb_
  
/* Fonction d'initialisation des attributs */
void Init_attribut(_MaillageInterne* mai, att_loc pt_attach, string nom_groupe)
{
  switch(pt_attach)
    {
    case MAILLAGE:
    case NOEUD:
    case BRAS:
    case POLYGONE:
    case POLYEDRE:      
    case NUAGE:
    case LIGNE:
    case SURFACE:
    case VOLUME:
      mai->init_att();
      break;
      
    case NOEUD_NUAGE:
      mai->nuage(nom_groupe)->init_att();
      break;
      
    case NOEUD_LIGNE:
    case BRAS_LIGNE:
      mai->ligne(nom_groupe)->init_att();
      break;
      
    case NOEUD_SURFACE:
    case BRAS_SURFACE:
    case POLYGONE_SURFACE:
      mai->surface(nom_groupe)->init_att();
      break;
      
    case NOEUD_VOLUME:
    case BRAS_VOLUME:
    case POLYGONE_VOLUME:
    case POLYEDRE_VOLUME:
      mai->volume(nom_groupe)->init_att();
      break;
       
    default:
      fputs("Erreur concernant le point d'attache d'un attribut \n", stderr);     
      break;
    }
}


#ifdef __INTERNE_MALIPP
void lmczmli_ (int_type* comprimer, int_type* niveau, int_type* ier)
{
	try
	{
		*ier	= 0;
		if (1 == *comprimer)
			MaliPPWriter::activer_compression_zlib (*niveau);
		else
			MaliPPWriter::desactiver_compression ( );
	}
	catch(erreur& err)
	{
		fputs(err.what ( ), stderr);
		*ier	= -1;
	}
	catch(...)
	{
		fputs ("Erreur inattendue\n", stderr);    
		*ier	= -1;
	}
}	/* lmczmli_ */
#endif	/* __INTERNE_MALIPP */


void lmpdno_ (double* precision, int_type* ier)
{
	try
	{
		*ier	= 0;
		Maillage::precision_distance_noeuds (*precision);
	}
	catch(erreur& err)
	{
		fputs(err.what ( ), stderr);
		*ier	= -1;
	}
	catch(...)
	{
		fputs ("Erreur inattendue\n", stderr);    
		*ier	= -1;
	}
}	/* lmpdno_ */



#if defined(_CRAY)

#include <fortran.h>

extern "C" {  

  void LMINNM(_fcd _ficmai, int_type* refmai, _fcd _datmod, 
	      double* unlosi, int_type* typcoo, int_type* dimens, 
	      int_type* geomet, int_type* structure, int_type* ier)
  {
    lminnm_(_fcdtocp(_ficmai), refmai, _fcdtocp(_datmod), 
	    unlosi, typcoo, dimens, geomet, structure, ier, 
	    _fcdlen(_ficmai), _fcdlen(_datmod));
  }
  
  void LMINVE(_fcd _varfic, int_type* refmai, _fcd _datmod, 
	      double* unlosi, int_type* typcoo, int_type* dimens, 
	      int_type* geomet, int_type* structure, int_type* ier)
  {
    lminve_(_fcdtocp(_varfic), refmai, _fcdtocp(_datmod), unlosi, typcoo, 
	  dimens, geomet, structure, ier, _fcdlen(_varfic), _fcdlen(_datmod));
  }
  
  void LMLENM(_fcd _ficmai, int_type* refmai, _fcd _datmod, 
	      double* unlosi, int_type* typcoo, int_type* dimens, 
	      int_type* geomet, int_type* structure, int_type* ier)
  {
    lmlenm_(_fcdtocp(_ficmai), refmai, _fcdtocp(_datmod), 
	    unlosi, typcoo, dimens, geomet, structure, ier, 
	    _fcdlen(_ficmai), _fcdlen(_datmod));
  }
  
  void LMLEVE(_fcd _varfic, int_type* refmai, _fcd _datmod, 
	      double* unlosi, int_type* typcoo, int_type* dimens, 
	      int_type* geomet, int_type* structure, int_type* ier)
  {
    lmleve_(_fcdtocp(_varfic), refmai, _fcdtocp(_datmod), unlosi, typcoo, 
	  dimens, geomet, structure, ier, _fcdlen(_varfic), _fcdlen(_datmod));
  }
   
  void LMINDI(int_type* refmai, int_type* nbrnoe, int_type* nbrbra,
	      int_type* nbrmai, int_type* ier)
  {
    lmindi_(refmai, nbrnoe, nbrbra, nbrmai, ier); 
  }
  
  void LMNBCN(int_type* refmai, int_type* maibra, int_type* mainoe,           
	      int_type* branoe, int_type* ier)
  {
    lmnbcn_(refmai, maibra, mainoe, branoe, ier);
  }
  
  void LMNBNU(int_type* refmai, int_type* nbrnua, int_type* ier)
  {
    lmnbnu_(refmai, nbrnua, ier); 
  }
  
  void LMNBLI(int_type* refmai, int_type* nbrlig, int_type* ier)
  {
    lmnbli_(refmai, nbrlig, ier);
  }
  
  void LMNBSU(int_type* refmai, int_type* nbrsur, int_type* ier)
  {
    lmnbsu_(refmai, nbrsur, ier);
  }
  
  void LMNOXY(int_type* refmai, double* x, double* y, int_type* ier)
  {
    lmnoxy_(refmai, x, y, ier);
  }
  
  void LMNOX4(int_type* refmai, float* x, float* y, int_type* ier)
  {
    lmnox4_(refmai, x, y, ier);
  }
  
  
  void LMNOBR(int_type* refmai, int_type* lnobr, 
	      int_type* pnobr, int_type* ier)
  {
    lmnobr_(refmai, lnobr, pnobr, ier);
  }
  
  void LMNOMA(int_type* refmai, int_type* lnoma, 
	      int_type* pnoma, int_type* ier)
  {
    lmnoma_(refmai, lnoma, pnoma, ier);
  }
  
  void LMBRNO(int_type* refmai, int_type* noeori, 
	      int_type* noeext, int_type* ier)
  {
    lmbrno_(refmai, noeori, noeext, ier);
  }
  
  void LMBRMA(int_type* refmai, int_type* maigau, 
	      int_type* maidro, int_type* ier)
  {
    lmbrma_(refmai, maigau, maidro, ier);
  }
  
  void LMMANO(int_type* refmai, int_type* lmano, 
	      int_type* pmano, int_type* ier)
  {
    lmmano_(refmai, lmano, pmano, ier);
  }
  
  void LMMABR(int_type* refmai, int_type* lmabr, 
	      int_type* pmabr, int_type* ier)
  {
    lmmabr_(refmai, lmabr, pmabr, ier);
  }
  
  void LMNULS(int_type* refmai, _fcd _nomnua, int_type* ier)
  {
    lmnuls_(refmai, _fcdtocp(_nomnua), ier, _fcdlen(_nomnua));
  }
  
  void LMNUEX(int_type* refmai, _fcd _nomnua, int_type* nbrnoe,
	      int_type* ier)
  {
    lmnuex_(refmai, _fcdtocp(_nomnua), nbrnoe, ier, _fcdlen(_nomnua)); 
  }
  
  void LMNUNO(int_type* refmai, _fcd _nomnua, int_type* lnoe,
	      int_type* ier)
  {
    lmnuno_(refmai, _fcdtocp(_nomnua), lnoe, ier, _fcdlen(_nomnua));
  }
  
  void LMLILS(int_type* refmai, _fcd _nomlig, 
	      int_type* ier)
  {
    lmlils_(refmai, _fcdtocp(_nomlig), ier, _fcdlen(_nomlig));
  }

  void LMLIEX(int_type* refmai, _fcd _nomlig, int_type* nbrbra,
	      int_type* nbrnoe, int_type* ier)
  {
    lmliex_(refmai, _fcdtocp(_nomlig), nbrbra, nbrnoe, ier, _fcdlen(_nomlig));
  }
  
  void LMLIBR(int_type* refmai, _fcd _nomlig, int_type* lbra,
	      int_type* ier)
  {
    lmlibr_(refmai, _fcdtocp(_nomlig), lbra, ier, _fcdlen(_nomlig));
  }
  
  void LMLINO(int_type* refmai, _fcd _nomlig, int_type* lnoe,
	      int_type* ier)
  {
    lmlino_(refmai, _fcdtocp(_nomlig), lnoe, ier, _fcdlen(_nomlig));
  }
  
  void LMSULS(int_type* refmai, _fcd _nomsur, 
	      int_type* ier)
  {
    lmsuls_(refmai, _fcdtocp(_nomsur), ier, _fcdlen(_nomsur));
  }
  
  void LMSUEX(int_type* refmai, _fcd _nomsur, int_type* nbrmai,
	      int_type* lonlig, int_type* nbrbrc, int_type* nbrnoc, 
	      int_type* ier)
  {
    lmsuex_(refmai, _fcdtocp(_nomsur), nbrmai, lonlig, nbrbrc, nbrnoc, 
	    ier, _fcdlen(_nomsur));
  }
  
  void LMSUES(int_type* refmai, _fcd _nomsur, int_type* nbrmai,
	      int_type* lonlig, int_type* nbrbrs, int_type* nbrbrc,
	      int_type* nbrnos, int_type* nbrnoc, int_type* ier)
  {
    lmsues_(refmai, _fcdtocp(_nomsur), nbrmai, lonlig, nbrbrs, nbrbrc,
	    nbrnos, nbrnoc, ier, _fcdlen(_nomsur));
    
  }
  
  void LMSUMA(int_type* refmai, _fcd _nomsur, int_type* lmai,
	      int_type* ier)
  {
    lmsuma_(refmai, _fcdtocp(_nomsur), lmai, ier, _fcdlen(_nomsur));
  }
  
  void LMSUBR(int_type* refmai, _fcd _nomsur, int_type* lbrasu,
	      int_type* ier)
  {
    lmsubr_(refmai, _fcdtocp(_nomsur), lbrasu, ier, _fcdlen(_nomsur));
  }
  
  void LMCOBR(int_type* refmai, _fcd _nomsur, int_type* lbraco,
	      int_type* ier)
  {
    lmcobr_(refmai, _fcdtocp(_nomsur), lbraco, ier, _fcdlen(_nomsur)); 
  }
  
  void LMSUNO(int_type* refmai, _fcd _nomsur, int_type* lnoesu,
	      int_type* ier)
  {
    lmsuno_(refmai, _fcdtocp(_nomsur), lnoesu, ier, _fcdlen(_nomsur));
  }
  
  void LMCONO(int_type* refmai, _fcd _nomsur, int_type* lnoeco,
	      int_type* ier)
  {
    lmcono_(refmai, _fcdtocp(_nomsur), lnoeco, ier, _fcdlen(_nomsur));
  }
  
  void LMFIN(int_type* refmai, int_type* ier)
  {
    lmfin_(refmai, ier);
  }
  
  void LMMESS(int* iumes)
  {
    lmmess_(iumes);
  }

// EXPURGE_BEGINNING_TAG USE_LOCAL_DEPENDENCIES_2
// EXPURGE_COMPLETION_TAG
  
  void LMCRMI(int_type *refmai, _fcd titre, _fcd _datmod, double *unlosi,
	      int_type *typcoo, int_type *dimens, int_type *geomet,
	      int_type *ier)
  {
    lmcrmi_(refmai, _fcdtocp(titre), _fcdtocp(_datmod), unlosi, typcoo,
	    dimens, geomet, ier, _fcdlen(titre), _fcdlen(_datmod));
  }
  
  void LMCRNO(int_type* refmai, double* x, double* y, int_type* nbnoe,
	      int_type* ier)
  {
    lmcrno_(refmai, x, y, nbnoe, ier);
  }
  
  void LMCRNO4(int_type* refmai, float* x, float* y, int_type* nbnoe,
	       int_type* ier)
  {
    lmcrno4_(refmai, x, y, nbnoe,ier);
  }
  
  void LM3CRNO(int_type* refmai, double* x, double* y, double* z,
	       int_type* nbnoe, int_type* ier)
  {
    lm3crno_(refmai, x, y, z, nbnoe, ier);
  }
  
  void LM3CRNO4(int_type* refmai, float* x, float* y, float* z,
		int_type* nbnoe, int_type* ier)
  {
    lm3crno4_(refmai, x, y, z, nbnoe,ier);
  }
  
  void LMCRBR(int_type* refmai, int_type* noeori, int_type* noeext,
	      int_type* nbbras, int_type* ier)
  {
    lmcrbr_(refmai, noeori, noeext, nbbras, ier);
  }
  
  void LMCRMA(int_type* refmai, int_type* lmano, int_type* pmano,
	      int_type* nbmai, int_type* ier)
  {
    lmcrma_(refmai, lmano, pmano, nbmai, ier);
  }
  
  void LM3CRMA(int_type* refmai, int_type* lmano, int_type* pmano,
	       int_type* nbmai, int_type* ier)
  {
    lm3crma_(refmai, lmano, pmano, nbmai, ier); 
  }

  void LMCRNU(int_type* refmai, _fcd nom, int_type* idnoe, int_type* nbnoe,
	       int_type* ier)
  { 
    lmcrnu_(refmai, _fcdtocp(nom), idnoe, nbnoe, ier, _fcdlen(nom)); 
  }

  void LMCRLI(int_type* refmai, _fcd nom, int_type* idbr, int_type* nbbr,
	      int_type* ier)
  { 
    lmcrli_(refmai, _fcdtocp(nom), idbr, nbbr, ier, _fcdlen(nom)); 
  }

  void LMCRSU(int_type* refmai, _fcd nom, int_type* idpg, int_type* nbpg,
	      int_type* ier)
  { 
    lmcrli_(refmai, _fcdtocp(nom), idpg, nbpg, ier, _fcdlen(nom)); 
  }

  void LM3CRVO(int_type* refmai, _fcd nom, int_type* idpd, int_type* nbpd,
	       int_type* ier)
  { 
    lm3crvo_(refmai, _fcdtocp(nom), idpd, nbpd, ier, _fcdlen(nom)); 
  }

  void LMDENO(int_type* refmai, int_type* idnoe, int_type* ier)  
  {
    lmdeno_(refmai, idnoe, ier);  
  }

  void LMDEBR(int_type* refmai, int_type* idbras, int_type* ier)
  {
    lmdebr_(refmai, idbras, ier);
  }

  void LMDEMA(int_type* refmai, int_type* idmai, int_type* ier)
  {
    lmdema_(refmai, idmai, ier);
  }

  void LM3DEMA(int_type* refmai, int_type* idmai, int_type* ier)
  {
    lm3dema_(refmai, idmai, ier);
  }

  void LMDENU(int_type* refmai, _fcd nom, int_type* ier)
  {
    lmdenu_(refmai, _fcdtocp(nom), ier, _fcdlen(nom)); 
  }

  void LMDELI(int_type* refmai, _fcd nom, int_type* ier)
  {
    lmdeli_(refmai, _fcdtocp(nom), ier, _fcdlen(nom)); 
  }

  void LMDESU(int_type* refmai, _fcd nom, int_type* ier)
  {
    lmdesu_(refmai, _fcdtocp(nom), ier, _fcdlen(nom)); 
  }

  void LM3DEVO(int_type* refmai, _fcd nom, int_type* ier)
  {
    lm3devo_(refmai, _fcdtocp(nom), ier, _fcdlen(nom)); 
  }

  void LMAJNO(int_type* refmai, _fcd nom, int_type* idnoe, int_type* ier)
  {
    lmajno_(refmai, _fcdtocp(nom), idnoe, ier, _fcdlen(nom));
  }

  void LMRENO(int_type* refmai, _fcd nom, int_type* idnoe, int_type* ier)
  {
    lmreno_(refmai, _fcdtocp(nom), idnoe, ier, _fcdlen(nom));
  }

  void LMAJBR(int_type* refmai, _fcd nom, int_type* idbr, int_type* ier)
  {
    lmajbr_(refmai, _fcdtocp(nom), idbr, ier, _fcdlen(nom));
  }

  void LMREBR(int_type* refmai, _fcd nom, int_type* idbr, int_type* ier)
  {
    lmrebr_(refmai, _fcdtocp(nom), idbr, ier, _fcdlen(nom));
  }

  void LMAJPG(int_type* refmai, _fcd nom, int_type* idpg, int_type* ier)
  {
    lmajpg_(refmai, _fcdtocp(nom), idpg, ier, _fcdlen(nom));
  }

  void LMREPG(int_type* refmai, _fcd nom, int_type* idpg, int_type* ier)
  {
    lmrepg_(refmai, _fcdtocp(nom), idpg, ier, _fcdlen(nom));
  }

  void LM3AJPD(int_type* refmai, _fcd nom, int_type* idpd, int_type* ier)
  {
    lm3ajpd_(refmai, _fcdtocp(nom), idpd, ier, _fcdlen(nom));
  }

  void LM3REPD(int_type* refmai, _fcd nom, int_type* idpd, int_type* ier)
  {
    lm3repd_(refmai, _fcdtocp(nom), idpd, ier, _fcdlen(nom));
  }

  void LMECMI(_fcd(_ficmai), int_type* refmai, int_type* ier)
  {
    lmecmi_(_fcdtocp(_ficmai), refmai, ier, _fcdlen(_ficmai));
  }
  
  void LMATLS(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
	      _fcd(_listenom), int_type listetype[], int_type* nbatt,
	      int_type* ier)
  {
    lmatls_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_listenom),
	    listetype, nbatt, ier, _fcdlen(_nom_groupe), _fcdlen(_listenom));
  }
  
  void LMATEN(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
	      _fcd(_nom_att), int tab_att[], int_type* nb_elt,
	      int_type* ier)
  {
    lmaten_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	    tab_att, nb_elt, ier, _fcdlen(_nom_groupe), _fcdlen(_nom_att));
  }
  
  void LMATRE(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
	      _fcd(_nom_att), double tab_att[], int_type* nb_elt,
	      int_type* ier)
  {
    lmatre_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	    tab_att, nb_elt, ier, _fcdlen(_nom_groupe), _fcdlen(_nom_att));
  }
  
  void LMATV2(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
	      _fcd(_nom_att), double tab_att[], int_type* nb_elt,
	      int_type* ier)
  {
    lmatv2_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	    tab_att, nb_elt, ier, _fcdlen(_nom_groupe), _fcdlen(_nom_att));
  }
  
  void LMATV3(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
	      _fcd(_nom_att), double tab_att[], int_type* nb_elt,
	      int_type* ier)
  {
    lmatv3_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	    tab_att, nb_elt, ier, _fcdlen(_nom_groupe), _fcdlen(_nom_att));
  }
  
  void LMATCH(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
	      _fcd(_nom_att), _fcd(_tab_att), int_type* nb_elt,
	      int_type* ier)
  {
    lmatch_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	    _fcdtocp(_tab_att), nb_elt, ier, _fcdlen(_nom_groupe), 
	    _fcdlen(_nom_att), _fcdlen(_tab_att));
  }

  void LMCRATEN(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
		_fcd(_nom_att), int_type* valtab, int_type* nbatt, int_type* ier)
  {	
    lmcraten_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	      valtab, nbatt, ier, _fcdlen(_nom_groupe), _fcdlen(_nom_att));
  }

  void LMCRATRE(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
		_fcd(_nom_att), double* valtab, int_type* nbatt, int_type* ier)
  {
    lmcratre_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	      valtab, nbatt, ier, _fcdlen(_nom_groupe), _fcdlen(_nom_att));
  }

  void LMCRATV2(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
		_fcd(_nom_att), double* valtab, int_type* nbatt, int_type* ier)
  {
    lmcratv2_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	      valtab, nbatt, ier, _fcdlen(_nom_groupe), _fcdlen(_nom_att));
  }

  void LMCRATV3(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
		_fcd(_nom_att), double* valtab, int_type* nbatt, int_type* ier)
  {
    lmcratv3_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	      valtab, nbatt, ier, _fcdlen(_nom_groupe), _fcdlen(_nom_att));
  }

  void LMCRATCH(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
		_fcd(_nom_att), _fcd(valtab), int_type* nbatt, int_type* ier)
  {
    lmcratch_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	      _fcdtocp(valtab), nbatt, ier, _fcdlen(_nom_groupe),
	      _fcdlen(_nom_att), _fcdlen(valtab));
  }

  void LMDEAT(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
	      _fcd(_nom_att), int_type* ier)
  {
    lmdeat_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	    ier, _fcdlen(_nom_groupe), _fcdlen(_nom_att));
  }

  void LMMOATEN(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
		_fcd(_nom_att), int_type* valtab, int_type* nbatt, int_type* ier)
  {
    lmmoaten_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	      valtab, nbatt, ier, _fcdlen(_nom_groupe), _fcdlen(_nom_att));
  }

  void LMMOATRE(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
		_fcd(_nom_att), double* valtab, int_type* nbatt, int_type* ier)
  {
    lmmoatre_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	      valtab, nbatt, ier, _fcdlen(_nom_groupe), _fcdlen(_nom_att));
  }

  void LMMOATV2(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
		_fcd(_nom_att), double* valtab, int_type* nbatt, int_type* ier)
  {
    lmmoatv2_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	      valtab, nbatt, ier, _fcdlen(_nom_groupe), _fcdlen(_nom_att));
  }

  void LMMOATV3(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
		_fcd(_nom_att), double* valtab, int_type* nbatt, int_type* ier)
  {
    lmmoatv3_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	      valtab, nbatt, ier, _fcdlen(_nom_groupe), _fcdlen(_nom_att));
  }

  void LMMOATCH(int_type* refmai, att_loc* pt_attach, _fcd(_nom_groupe),
		_fcd(_nom_att), _fcd(valtab), int_type* nbatt, int_type* ier)
  {
    lmmoatch_(refmai, pt_attach, _fcdtocp(_nom_groupe), _fcdtocp(_nom_att),
	      _fcdtocp(valtab), nbatt, ier, _fcdlen(_nom_groupe),
	      _fcdlen(_nom_att), _fcdlen(valtab));
  }

  void LMPREPA(int_type* refmai, int_type* prepa, int_type* ier)
  {
    lmprepa_(refmai, prepa, ier) ;
  }

  void LMPRETB(int_type* refmai, int_type* prepa_tab, 
	       int_type* nb_prepa, int_type* ier)
  {
    lmpretb_(refmai, prepa_tab, nb_prepa, ier) ;
  }

  void LMVERTB (int_type* refmai, int_type* verif_tab, 
	            int_type* nb_verif, int_type* ier)
  {
    lmvertb_ (refmai, verif_tab, nb_verif, ier) ;
  }
}

#endif

