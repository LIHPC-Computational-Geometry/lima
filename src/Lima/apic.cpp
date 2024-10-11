#include <string.h>
#include <stdio.h>

/// Developpements novembre 20001
///
#ifdef __INTERNE_MACHINE_TYPES
#include <machine_types.h>
#endif

#include <Lima/lima.h>
#include "LimaP/config_it.h"
#include <Lima/erreur.h>
#include "LimaP/fonction.h"
#include "LimaP/algorithme.h"
#include "LimaP/maillage_it.h"
#include <Lima/maillage.h>

#if defined(__INTERNE_MALIPP) && defined (__INTERNE_MALIPP_WRITER)
#include <Lima/malipp.h>
#endif	/* __INTERNE_MALIPP */


#include <string.h>


BEGIN_NAMESPACE_LIMA
  
/* Variables pour le tableau de reperage des maillages par leur indice. */
static int_type              QMAI = 0, UMAI = 0;
static _MaillageInterne**    PMAI = NULL;

extern "C"{
void _main();
}
  
  /* Allocaton de la structure Mai initialisation et renvoie de la reference.*/
_MaillageInterne* ouvrir_maillage(int_type* refmai)
{
  int_type       ind;
    
  *refmai = 0;
    
  /* recherche d'une adresse libre dans le tableau PMAI,
   * depart a l'indice 1 */
  for(ind=1; ind<QMAI; ind++){
    if(PMAI[ind]==NULL)
      break;
  }
    
  /* pas d'adresse libre, augmentation de QMAI */
  if(ind>=QMAI){
    int_type q = QMAI+32;
    void* ptr = realloc(PMAI, q*sizeof(_MaillageInterne*));
      
    if(ptr==NULL)
      throw alloc_erreur("Erreur d'allocation\n");
      
    /* mise a NULL des nouvelles adresses */
    PMAI = (_MaillageInterne**)ptr;
    for(int_type j=QMAI; j<q; j++)
      PMAI[j] = NULL;
    QMAI = q;
  }
    
  PMAI[ind] = new _MaillageInterne();
  UMAI++;
  *refmai = ind;
    
  return PMAI[ind];
}
  
/* renvoie l'adresse du maillage correspondant */
_MaillageInterne* trouver_maillage(int_type ref)
{
  if(ref<=0 || ref>=QMAI || !PMAI[ref]){
    throw range_erreur("Reference de maillage invalide");
  }
    
  return  PMAI[ref];
}
  
/* renvoie l'adresse du maillage correspondant et dereference ce maillage */
_MaillageInterne* liberer_maillage (int_type ref)
{
  if(ref<=0 || ref>=QMAI || !PMAI[ref]){
    throw range_erreur("Reference de maillage invalide");
  }

  _MaillageInterne*	maillage	= PMAI [ref];
  PMAI [ref]	= NULL;

  return  maillage;
}
  
/* Fermeture de la structure Mai correspondante
   * Supprime l'adresse du maillage. 	
   * Eventuellement le tableau PMAI. */
void fermer_maillage(int_type refmai)
{
  try{
    _MaillageInterne* maillage = trouver_maillage(refmai);
    delete maillage;
      
    PMAI[refmai] = NULL;
    UMAI--;
      
    if(UMAI==0) {
      free(PMAI);
      PMAI = NULL;
      QMAI = 0;
    }
  }
  catch(...){
    /* pas de maillage, ce n'est pas grave c'etait pour le fermer */
    return;
  }
}

END_NAMESPACE_LIMA

#if !defined(_IBMR2) && !defined(CRAY) && !defined(__hpux)
using namespace Lima;
#else
#if __IBMCPP__ >= 500
using namespace Lima;
#endif  // #if __IBMCPP__ >= 500
#endif	// #if !defined(_IBMR2) && ...


/** Retourne la version de lima++. */
const char* LM_version ( )
{
	return lima_version ( );
}

/** Fonction de lecture d'un maillage.
   * LM_lire_maillage lit un maillage et le conserve en memoire, puis
   * initialise la structure LM_MAILLAGE allouee par l'utilisateur et y
   * renseigne le champ refmai.
   * Si format vaut LM_FF_SUFFIXE et si le suffixe de nom_fichier est
   * .unv, le champ maillage->dimension est prealablement teste. Si sa
   * valeur est 2, le fichier sera considere comme 2D ; sinon il sera
   * considere 3D.
   *
   * @param maillage La structure doit etre allouee par l'utilisateur.
   */
  
int LM_lire_maillage(LM_FORMAT_FICHIER format, const char *nom_fichier,
		     LM_MAILLAGE *maillage)
{
#if defined(__hpux)
  _main();
#endif


  try{
    int_type      ref = 0;
    _MaillageInterne* mai = ouvrir_maillage(&ref);
      
    if((maillage->dimension < D1) || (maillage->dimension > D3))
      maillage->dimension = D3;
    
    mai->dimension(dim_t(maillage->dimension));
    /* initialisation structure */
    memset(maillage, 0, sizeof(LM_MAILLAGE));
    maillage->refmai = ref;
    /* lecture fichier */
    mai->lire(nom_fichier, format_t(format));
    maillage->dimension = mai->dimension();
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    return -1;
  }
  catch(...){
    fputs("Erreur inatendu\n", stderr);    
    return -1;
  }
    
  return 0;
}
  

/* Libere les espaces memoire utilises par la structure LM_MAILLAGE. */
int LM_fermer_maillage(LM_MAILLAGE *maillage)
{
  fermer_maillage(maillage->refmai);
    
  /* initialisation structure */
  memset(maillage, 0, sizeof(LM_MAILLAGE));
    
  return 0;
}
  
static void lire_information(LM_MAILLAGE *maillage)
{
  const _MaillageInterne* mai = trouver_maillage(maillage->refmai);
    
  strncpy(maillage->titre, mai->titre().c_str(), sizeof(maillage->titre)-1);
  strncpy(maillage->date, mai->date().c_str(),  sizeof(maillage->date)-1);
  maillage->unite = mai->unite_longueur();
  maillage->geometrie = LM_GEOMETRIE(mai->type_geometrie());
  maillage->structuration = LM_STRUCTURATION(mai->structuration());    
  maillage->dimension = mai->dimension();
    
  /* nombre d'entités */
  maillage->entite.n.nombre_noeud = mai->nb_noeuds();
  maillage->entite.n.nombre_bras = mai->nb_bras();
  maillage->entite.n.nombre_maille = mai->nb_polygones();
  maillage->entite.n.nombre_polyedre = mai->nb_polyedres();
    
  /* nombre de groupes */
  maillage->objet.n.nombre_nuage = mai->nb_nuages();
  maillage->objet.n.nombre_ligne = mai->nb_lignes();
  maillage->objet.n.nombre_surface = mai->nb_surfaces();
  maillage->objet.n.nombre_volume = mai->nb_volumes();
    
  /* nombre de connexions */
  maillage->entite.n.noeud_bras =
    nombre_connexions(mai->premier_noeud(), mai->dernier_noeud(),
		      _les_bras<_NoeudInterne>());
  maillage->entite.n.noeud_maille =
    nombre_connexions(mai->premier_noeud(), mai->dernier_noeud(),
		      _les_polygones<_NoeudInterne>());
  maillage->entite.n.noeud_polyedre =
    nombre_connexions(mai->premier_noeud(), mai->dernier_noeud(),
		      _les_polyedres<_NoeudInterne>());
    
  maillage->entite.n.bras_noeud =
    nombre_connexions(mai->premier_bras(), mai->dernier_bras(),
		      _les_noeuds<_BrasInterne>());
  maillage->entite.n.bras_maille =
    nombre_connexions(mai->premier_bras(), mai->dernier_bras(),
		      _les_polygones<_BrasInterne>());
  maillage->entite.n.bras_polyedre =
    nombre_connexions(mai->premier_bras(), mai->dernier_bras(),
		      _les_polyedres<_BrasInterne>());
    
  maillage->entite.n.maille_noeud =
    nombre_connexions(mai->premier_polygone(), mai->dernier_polygone(),
		      _les_noeuds<_PolygoneInterne>());
  maillage->entite.n.maille_bras =
    nombre_connexions(mai->premier_polygone(), mai->dernier_polygone(),
		      _les_bras<_PolygoneInterne>());
  maillage->entite.n.maille_polyedre =
    nombre_connexions(mai->premier_polygone(), mai->dernier_polygone(),
		      _les_polyedres<_PolygoneInterne>());
    
  maillage->entite.n.polyedre_noeud =
    nombre_connexions(mai->premier_polyedre(), mai->dernier_polyedre(),
		      _les_noeuds<_PolyedreInterne>());
  maillage->entite.n.polyedre_bras =
    nombre_connexions(mai->premier_polyedre(), mai->dernier_polyedre(),
		      _les_bras<_PolyedreInterne>());
  maillage->entite.n.polyedre_maille =
    nombre_connexions(mai->premier_polyedre(), mai->dernier_polyedre(),
		      _les_polygones<_PolyedreInterne>());
    
  maillage->objet.n.nuage_noeud =
    nombre_connexions(mai->premier_nuage(), mai->dernier_nuage(),
		      _les_noeuds<_NuageInterne>());
    
  maillage->objet.n.ligne_bras =
    nombre_connexions(mai->premier_ligne(), mai->dernier_ligne(), 
		      _les_bras<_LigneInterne>());
    
  maillage->objet.n.surface_maille =
    nombre_connexions(mai->premier_surface(), mai->dernier_surface(), 
		      _les_polygones<_SurfaceInterne>());
    
  maillage->objet.n.volume_polyedre =
    nombre_connexions(mai->premier_volume(), mai->dernier_volume(), 
		      _les_polyedres<_VolumeInterne>());
}

/* Effectue les traitements de calcul selon le niveau de preparation 
   demande. */
/// Developpements novembre 2001
int LM_preparer_maillage(LM_MAILLAGE *maillage, 
                         LM_PREPARATION preparation)
{
  try{
    int_type     refmai = maillage->refmai;
      
    _MaillageInterne* mai = trouver_maillage(refmai);
    mai->preparer(preparation_t(preparation));
      
    /* initialisation structure */
    memset(maillage, 0, sizeof(LM_MAILLAGE));
      
    /* transfert des informations dans les champs de maillage */
    maillage->refmai = refmai;
      
    lire_information(maillage);
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    return -1;
  }
  catch(...){
    fputs("Erreur inatendu\n", stderr);    
    return -1;
  }
    
  return 0;
}

int LM_preparation_parametrable(LM_MAILLAGE *maillage, 
				LM_TYPEMASQUE preparation)
{
  try{
    int_type     refmai = maillage->refmai;
      
    _MaillageInterne* mai = trouver_maillage(refmai);
    mai->preparation_parametrable(preparation, true);
      
    /* initialisation structure */
    memset(maillage, 0, sizeof(LM_MAILLAGE));
      
    /* transfert des informations dans les champs de maillage */
    maillage->refmai = refmai;
      
    lire_information(maillage);
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    return -1;
  }
  catch(...){
    fputs("Erreur inatendu\n", stderr);    
    return -1;
  }
    
  return 0;
}
/* Effectue les traitements de calcul selon le niveau de preparation demande. */
int LM_preparer_maillage2 (LM_MAILLAGE *maillage, LM_PREPARATION preparation,
                           char* message, size_t taille_message)
{
	try
	{
		int_type	refmai = maillage->refmai;
      
		_MaillageInterne*	mai	= trouver_maillage (refmai);
		mai->preparer (preparation_t(preparation));
      
		/* initialisation structure */
		memset (maillage, 0, sizeof (LM_MAILLAGE));
      
		/* transfert des informations dans les champs de maillage */
		maillage->refmai = refmai;
      
		lire_information(maillage);
	}
	catch (const erreur& erreur)
	{
		if (0 != taille_message)
		{
			size_t	longueur = strlen (erreur.what ( )) < taille_message - 1 ?
							   strlen (erreur.what ( )) : taille_message - 1;
			strncpy (message, erreur.what ( ), longueur);
			message [longueur]	= '\0';
		}	// if (0 != taille_message)

		return -1;
	}
	catch(...)
	{
		if (0 != taille_message)
		{
			const char*	errorMsg	= "Erreur inattendue non documentée.";
			size_t	longueur		= strlen (errorMsg) < taille_message - 1 ?
									  strlen (errorMsg) : taille_message - 1;
			strncpy (message, errorMsg, longueur);
			message [longueur]	= '\0';
		}	// if (0 != taille_message)

		return -1;
	}
    
	return 0;
}	// LM_preparer_maillage2

int LM_preparation_parametrable2 (LM_MAILLAGE *maillage, 
           LM_TYPEMASQUE preparation, char* message, size_t taille_message)
{
	try
	{
		int_type     refmai = maillage->refmai;
      
		_MaillageInterne* mai = trouver_maillage (refmai);
		mai->preparation_parametrable (preparation, true);
      
		/* initialisation structure */
		memset (maillage, 0, sizeof (LM_MAILLAGE));
      
		/* transfert des informations dans les champs de maillage */
		maillage->refmai = refmai;
      
		lire_information(maillage);
	}
	catch (const erreur& erreur)
	{
		if (0 != taille_message)
		{
			size_t	longueur = strlen (erreur.what ( )) < taille_message - 1 ?
							   strlen (erreur.what ( )) : taille_message - 1;
			strncpy (message, erreur.what ( ), longueur);
			message [longueur]	= '\0';
		}	// if (0 != taille_message)

		return -1;
	}
	catch(...)
	{
		if (0 != taille_message)
		{
			const char*	errorMsg	= "Erreur inattendue non documentée.";
			size_t	longueur		= strlen (errorMsg) < taille_message - 1 ?
									  strlen (errorMsg) : taille_message - 1;
			strncpy (message, errorMsg, longueur);
			message [longueur]	= '\0';
		}	// if (0 != taille_message)

		return -1;
	}
    
  return 0;
}	// LM_preparation_parametrable2


int LM_precision_distance_noeuds (double precision)
{
	try
	{
		Maillage::precision_distance_noeuds (precision);
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}	// LM_precision_distance_noeuds


int LM_verifier_maillage2 (LM_MAILLAGE *maillage, LM_VC_TYPEMASQUE code, 
                           char* message, size_t taille_message)
{
//	memset (message, taille_message, '\0');
	memset (message, '\0', taille_message);	// CP, v 6.27.1, coverity
	try
	{
		int_type	refmai	= maillage->refmai;
      
		_MaillageInterne*	mai	= trouver_maillage (refmai);
		mai->verification_code ((TypeVerificationCode)code);
	}
	catch (const erreur& erreur)
	{
		if (0 != taille_message)
		{
			size_t	longueur = strlen (erreur.what ( )) < taille_message - 1 ?
							   strlen (erreur.what ( )) : taille_message - 1;
			strncpy (message, erreur.what ( ), longueur);
			message [longueur]	= '\0';
		}	// if (0 != taille_message)

		return -1;
	}
	catch(...)
	{
		if (0 != taille_message)
		{
			const char*	errorMsg	= "Erreur inattendue non documentée.";
			size_t	longueur		= strlen (errorMsg) < taille_message - 1 ?
									  strlen (errorMsg) : taille_message - 1;
			strncpy (message, errorMsg, longueur);
			message [longueur]	= '\0';
		}	// if (0 != taille_message)

		return -1;
	}
    
	return 0;
}	// LM_verifier_maillage2


int LM_verification_parametrable2 (LM_MAILLAGE *maillage, 
                 LM_V_TYPEMASQUE masque, char* message, size_t taille_message)
{
//	memset (message, taille_message, '\0');
	memset (message, '\0', taille_message);	// CP, v 6.27.1, coverity
	try
	{
		int_type	refmai	= maillage->refmai;
      
		_MaillageInterne*	mai	= trouver_maillage (refmai);
		mai->verification_parametrable ((TypeMasqueVerification)masque);
	}
	catch (const erreur& erreur)
	{
		if (0 != taille_message)
		{
			size_t	longueur = strlen (erreur.what ( )) < taille_message - 1 ?
							   strlen (erreur.what ( )) : taille_message - 1;
			strncpy (message, erreur.what ( ), longueur);
			message [longueur]	= '\0';
		}	// if (0 != taille_message)

		return -1;
	}
	catch(...)
	{
		if (0 != taille_message)
		{
			const char*	errorMsg	= "Erreur inattendue non documentée.";
			size_t	longueur		= strlen (errorMsg) < taille_message - 1 ?
									  strlen (errorMsg) : taille_message - 1;
			strncpy (message, errorMsg, longueur);
			message [longueur]	= '\0';
		}	// if (0 != taille_message)

		return -1;
	}
    
	return 0;
}	// LM_verification_parametrable2
  
int LM_reserver_maillage(LM_MAILLAGE* maillage)
{
  /* initialisation structure */
  memset(maillage, 0, sizeof(LM_MAILLAGE));
  maillage->unite = 1;
    
  /* reservation maillage et renseignement champ refmai */
  try{
    int_type      ref = 0;
    ouvrir_maillage(&ref);
    maillage->refmai = ref;
  }
    
  catch(erreur& err){
    fputs(err.what(), stderr);
    return -1;
  }
  catch(...){
    fputs("Erreur inatendu\n", stderr);    
    return -1;
  }
    
  return 0;
}
  
  
int LM_definir_maillage(LM_MAILLAGE *maillage)
{
  try{
    _MaillageInterne* mai =  trouver_maillage(maillage->refmai);
      
    mai->titre(maillage->titre);
    mai->date(maillage->date);
    mai->dimension(dim_t(maillage->dimension));
    mai->unite_longueur(maillage->unite);
    mai->type_geometrie(geometrie_t(maillage->geometrie));
  }
    
  catch(erreur& err){
    fputs(err.what(), stderr);
    return -1;
  }
  catch(...){
    fputs("Erreur inatendu\n", stderr);    
    return -1;
  }
  return 0;
}
  
  
int LM_lire_noeud(LM_MAILLAGE *maillage, LM_NOEUD* coo)
{
  try{
    const _MaillageInterne* mai =  trouver_maillage(maillage->refmai);
    maillage->dimension = mai->dimension();
    maillage->entite.n.nombre_noeud = mai->nb_noeuds();
    for(_MaillageInterne::const_iterator_noeud ptr = mai->premier_noeud();
	ptr != mai->dernier_noeud(); ++ptr, ++coo){
      coo->coordonnee.t[0] = (*ptr)->x();
      coo->coordonnee.t[1] = (*ptr)->y();
      coo->coordonnee.t[2] = (*ptr)->z();
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    return -1;
  }
  catch(...){
    fputs("Erreur inatendu\n", stderr);    
    return -1;
  }
  return 0;
}
  
  
int LM_definir_noeud(LM_MAILLAGE *maillage, LM_NOEUD* coo)
{
  try{
    _MaillageInterne* mai =  trouver_maillage(maillage->refmai);
    mai->dimension(dim_t(maillage->dimension));
    //    mai->invalider_noeuds();
    for(int_type id=1; id<=maillage->entite.n.nombre_noeud; ++id, ++coo){
      if(id<=mai->nb_noeuds()){
	mai->noeud(id-1)->set_x(coo->coordonnee.n.x);
	mai->noeud(id-1)->set_y(coo->coordonnee.n.y);
	mai->noeud(id-1)->set_z(coo->coordonnee.n.z);
      }
      else
	mai->ajouter(_NoeudInterne::create(id, coo->coordonnee.n.x, 
					   coo->coordonnee.n.y,
					   coo->coordonnee.n.z));
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    return -1;
  }
  catch(...){
    fputs("Erreur inatendu\n", stderr);    
    return -1;
  }
  return 0;
}
  

int LM_definir_connexion(LM_MAILLAGE *maillage,
			 LM_TYPE_ENTITE typi, LM_TYPE_ENTITE typj,
			 int_type* pointage, int_type* liste)
{
  if(typi<LM_TE_BRAS || typi>LM_TE_POLYEDRE || typj!=LM_TE_NOEUD){
    fputs("On ne peut definir que les connexions "
	  "des tables de base", stderr);
    return -1;
  }
    
  try{
    int_type id;
    --liste;
      
    _MaillageInterne* mai =  trouver_maillage(maillage->refmai);
    switch(typi){
    case LM_TE_BRAS:
      for(id=1; id<=maillage->entite.n.nombre_bras; id++)
	ajouter_bras(mai, liste+pointage[id-1], liste+pointage[id], 
		     (size_type)id);
      break;
    case LM_TE_MAILLE:
      for(id=1; id<=maillage->entite.n.nombre_maille; id++)
	ajouter_polygone(mai, liste+pointage[id-1], 
			 liste+pointage[id], (size_type)id);
      break;
    case LM_TE_POLYEDRE:
      for(id=1; id<=maillage->entite.n.nombre_polyedre; id++)
	ajouter_polyedre(mai, liste+pointage[id-1], 
			 liste+pointage[id], (size_type)id);
      break;
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    return -1;
  }
  catch(...){
    fputs("Erreur inatendu\n", stderr);    
    return -1;
  }
  return 0;
}
  
int LM_lire_connexion(LM_MAILLAGE *maillage,
		      LM_TYPE_ENTITE typi, LM_TYPE_ENTITE typj,
		      int_type* pointage, int_type* liste)
{
  try{
    const _MaillageInterne* mai =  trouver_maillage(maillage->refmai);
    switch(typi){
    case LM_TE_NOEUD:
      switch(typj){
      case LM_TE_BRAS:
	lire_connexions(pointage, liste, mai->premier_noeud(), 
			mai->dernier_noeud(), _les_bras<_NoeudInterne>());
	break;
      case LM_TE_MAILLE:
	lire_connexions(pointage, liste, mai->premier_noeud(), 
			mai->dernier_noeud(), _les_polygones<_NoeudInterne>());
	break;
      case LM_TE_POLYEDRE:
	lire_connexions(pointage, liste, mai->premier_noeud(), 
			mai->dernier_noeud(), _les_polyedres<_NoeudInterne>());
	break;
      } 
      break;
    case LM_TE_BRAS:
      switch(typj){
      case LM_TE_NOEUD:
	lire_connexions(pointage, liste, mai->premier_bras(), 
			mai->dernier_bras(), _les_noeuds<_BrasInterne>());
	break;
      case LM_TE_MAILLE:
	lire_connexions(pointage, liste, mai->premier_bras(), 
			mai->dernier_bras(), _les_polygones<_BrasInterne>());
	break;
      case LM_TE_POLYEDRE:
	lire_connexions(pointage, liste, mai->premier_bras(), 
			mai->dernier_bras(), _les_polyedres<_BrasInterne>());
	break;
      }
      break;
    case LM_TE_MAILLE:
      switch(typj){
      case LM_TE_NOEUD:
	lire_connexions(pointage, liste, mai->premier_polygone(),
			mai->dernier_polygone(), _les_noeuds<_PolygoneInterne>());
	break;
      case LM_TE_BRAS:
	lire_connexions(pointage, liste, mai->premier_polygone(),
			mai->dernier_polygone(), _les_bras<_PolygoneInterne>());
	break;
      case LM_TE_POLYEDRE:
	lire_connexions(pointage, liste, mai->premier_polygone(),
			mai->dernier_polygone(), _les_polyedres<_PolygoneInterne>());
	break;
      }
      break;
    case LM_TE_POLYEDRE:
      switch(typj){
      case LM_TE_NOEUD:
	lire_connexions(pointage, liste, mai->premier_polyedre(),
			mai->dernier_polyedre(), _les_noeuds<_PolyedreInterne>());
	break;
      case LM_TE_BRAS:
	lire_connexions(pointage, liste, mai->premier_polyedre(),
			mai->dernier_polyedre(), _les_bras<_PolyedreInterne>());
	break;
      case LM_TE_MAILLE:
	lire_connexions(pointage, liste, mai->premier_polyedre(),
			mai->dernier_polyedre(), _les_polygones<_PolyedreInterne>());
	break;
      }
      break;
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    return -1;
  }
  catch(...){
    fputs("Erreur inatendu\n", stderr);    
    return -1;
  }
  return 0;
}
  
  
int LM_lire_objet(LM_MAILLAGE *maillage, LM_OBJET *objet)
{
  try{
    const _MaillageInterne* mai =  trouver_maillage(maillage->refmai);
    switch(objet->type){
    case LM_TO_NUAGE:
      {
	_NuageInterne* nuage;
	if(objet->numero!=0){
	  nuage = mai->nuage(objet->numero-1);
//	  strncpy(objet->nom, nuage->nom().c_str(), sizeof(objet->nom));
// CP, v 6.27.1, coverity :
	  strncpy(objet->nom, nuage->nom().c_str(), 1025 - 1);
		objet->nom[1024]	= '\0';
	}
	else{
//	  _Groupe::formater_nom(objet->nom); MODIF CP
	  nuage = mai->nuage(objet->nom);
	  size_type id=0;
	  while(strncmp(mai->nuage(id++)->nom().c_str(), objet->nom, 
			sizeof(objet->nom))!=0);	  
	  objet->numero = id;
	}
	objet->nombre.n.noeud = nuage->nb_noeuds();
	if(objet->liste.n.noeud!=NULL)
	  lire_elements(objet->liste.n.noeud, 
			nuage->premier_noeud(), nuage->dernier_noeud());
      }
      break;
    case LM_TO_LIGNE:
      {
	_LigneInterne* ligne;
	if(objet->numero!=0){
	  ligne = mai->ligne(objet->numero-1);
//	  strncpy(objet->nom, ligne->nom().c_str(), sizeof(objet->nom));
// CP, v 6.27.1, coverity :
	  strncpy(objet->nom, ligne->nom().c_str(), 1025 - 1);
		objet->nom[1024]	= '\0';
	}
	else{
//	  _Groupe::formater_nom(objet->nom); MODIF CP
	  ligne = mai->ligne(objet->nom);
	  size_type id=0;
	  while(strncmp(mai->ligne(id++)->nom().c_str(), objet->nom, 
			sizeof(objet->nom))!=0);	  
	  objet->numero = id;
	}
	objet->nombre.n.noeud = ligne->nb_noeuds();
	objet->nombre.n.bras = ligne->nb_bras();
	objet->longueur.n.ni = ligne->longueur_i();
	if(objet->liste.n.noeud!=NULL)
	  lire_elements(objet->liste.n.noeud, 
			ligne->premier_noeud(), ligne->dernier_noeud());
	if(objet->liste.n.bras!=NULL)
	  lire_elements(objet->liste.n.bras, 
			ligne->premier_bras(), ligne->dernier_bras());	
	  
	_NuageInterne* contour = ligne->contour();
	if(contour){
	  objet->nombre.n.noeud_contour = contour->nb_noeuds();
	  if(objet->liste.n.noeud_contour!=NULL)
	    lire_elements(objet->liste.n.noeud_contour, 
			  contour->premier_noeud(), contour->dernier_noeud());
	}
	else
	  objet->nombre.n.noeud_contour = 0;
      }
      break;
    case LM_TO_SURFACE:
      {
	_SurfaceInterne* surface;
	if(objet->numero!=0){
	  surface = mai->surface(objet->numero-1);
//	  strncpy(objet->nom, surface->nom().c_str(), sizeof(objet->nom));
// CP, v 6.27.1, coverity :
	  strncpy(objet->nom, surface->nom().c_str(), 1025 - 1);
		objet->nom[1024]	= '\0';
	}
	else{
//	  _Groupe::formater_nom(objet->nom); MODIF CP
	  surface = mai->surface(objet->nom);
	  size_type id=0;
	  while(strncmp(mai->surface(id++)->nom().c_str(), objet->nom, 
			sizeof(objet->nom))!=0);	  
	  objet->numero = id;
	}
	objet->nombre.n.noeud = surface->nb_noeuds();
	objet->nombre.n.bras = surface->nb_bras();
	objet->nombre.n.maille = surface->nb_polygones();
	objet->longueur.n.ni = surface->longueur_i();
	objet->longueur.n.nj = surface->longueur_j();
	if(objet->liste.n.noeud!=NULL)
	  lire_elements(objet->liste.n.noeud, 
			surface->premier_noeud(), surface->dernier_noeud());
	if(objet->liste.n.bras!=NULL)
	  lire_elements(objet->liste.n.bras, 
			surface->premier_bras(), surface->dernier_bras());
	if(objet->liste.n.maille!=NULL)
	  lire_elements(objet->liste.n.maille, 
			surface->premier_polygone(), surface->dernier_polygone());
	  
	_LigneInterne* contour = surface->contour();
	if(contour){
	  objet->nombre.n.noeud_contour = contour->nb_noeuds();
	  objet->nombre.n.bras_contour = contour->nb_bras();
	  if(objet->liste.n.noeud_contour!=NULL)
	    lire_elements(objet->liste.n.noeud_contour, 
			  contour->premier_noeud(), contour->dernier_noeud());
	  if(objet->liste.n.bras_contour!=NULL)
	    lire_elements(objet->liste.n.bras_contour, 
			  contour->premier_bras(), contour->dernier_bras());
	}
	else{
	  objet->nombre.n.noeud_contour = 0;
	  objet->nombre.n.bras_contour = 0;	  
	}
      }
      break;
    case LM_TO_VOLUME:
      {
	_VolumeInterne* volume;
	if(objet->numero!=0){
	  volume = mai->volume(objet->numero-1);
//	  strncpy(objet->nom, volume->nom().c_str(), sizeof(objet->nom));
// CP, v 6.27.1, coverity :
		strncpy(objet->nom, volume->nom().c_str(), 1025 - 1);
		objet->nom[1024]	= '\0';
	}
	else{
//	  _Groupe::formater_nom(objet->nom); MODIF CP
	  volume = mai->volume(objet->nom);
	  size_type id=0;
	  while(strncmp(mai->volume(id++)->nom().c_str(), objet->nom, 
			sizeof(objet->nom))!=0);	  
	  objet->numero = id;
	}
	objet->nombre.n.noeud = volume->nb_noeuds();
	objet->nombre.n.bras = volume->nb_bras();
	objet->nombre.n.maille = volume->nb_polygones();
	objet->nombre.n.polyedre = volume->nb_polyedres();
	if(objet->liste.n.noeud!=NULL)
	  lire_elements(objet->liste.n.noeud, 
			volume->premier_noeud(), volume->dernier_noeud());
	if(objet->liste.n.bras!=NULL)
	  lire_elements(objet->liste.n.bras, 
			volume->premier_bras(), volume->dernier_bras());
	if(objet->liste.n.maille!=NULL)
	  lire_elements(objet->liste.n.maille, 
			volume->premier_polygone(), volume->dernier_polygone());
	if(objet->liste.n.polyedre!=NULL)
	  lire_elements(objet->liste.n.polyedre, 
			volume->premier_polyedre(), volume->dernier_polyedre());
	  
	_SurfaceInterne* contour = volume->contour();
	if(contour){
	  objet->nombre.n.noeud_contour = contour->nb_noeuds();
	  objet->nombre.n.bras_contour = contour->nb_bras();
	  objet->nombre.n.maille_contour = contour->nb_polygones();
	  if(objet->liste.n.noeud_contour!=NULL)
	    lire_elements(objet->liste.n.noeud_contour, 
			  contour->premier_noeud(), contour->dernier_noeud());
	  if(objet->liste.n.bras_contour!=NULL)	  
	    lire_elements(objet->liste.n.bras_contour, 
			  contour->premier_bras(), contour->dernier_bras());
	  if(objet->liste.n.maille_contour!=NULL)
	    lire_elements(objet->liste.n.maille_contour,
			  contour->premier_polygone(), contour->dernier_polygone());
	}
	else{
	  objet->nombre.n.noeud_contour = 0;
	  objet->nombre.n.bras_contour = 0;
	  objet->nombre.n.maille_contour = 0;	  
	}
      }
      break;
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    return -1;
  }
  catch(...){
    fputs("Erreur inatendu\n", stderr);    
    return -1;
  }
  return 0;
}
  
int LM_definir_objet(LM_MAILLAGE *maillage, LM_OBJET *objet)
{
  try{
    _MaillageInterne* mai =  trouver_maillage(maillage->refmai);
    switch(objet->type){
    case LM_TO_NUAGE:
      {
	_NuageInterne* nuage = new _NuageInterne(objet->nom);
	mai->ajouter(nuage);
	for(int_type n=0; n<objet->nombre.n.noeud; n++)
	  nuage->ajouter(mai->noeud(objet->liste.n.noeud[n]-1));
      }
      break;
    case LM_TO_LIGNE:
      {
	_LigneInterne* ligne = new _LigneInterne(objet->nom);
	mai->ajouter(ligne);
	for(int_type b=0; b<objet->nombre.n.bras; b++)
	  ligne->ajouter(mai->bras(objet->liste.n.bras[b]-1));
      }
      break;
    case LM_TO_SURFACE:
      {
	_SurfaceInterne* surface = new _SurfaceInterne(objet->nom);
	mai->ajouter(surface);
	for(int_type m=0; m<objet->nombre.n.maille; m++)
	  surface->ajouter(mai->polygone(objet->liste.n.maille[m]-1));
      }
      break;
    case LM_TO_VOLUME:
      {
	_VolumeInterne* volume = new _VolumeInterne(objet->nom);
	mai->ajouter(volume);
	for(int_type p=0; p<objet->nombre.n.polyedre; p++)
	  volume->ajouter(mai->polyedre(objet->liste.n.polyedre[p]-1));
      }
      break;
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    return -1;
  }
  catch(...){
    fputs("Erreur inatendu\n", stderr);    
    return -1;
  }
  return 0;
}
  
/** Ecrit un maillage prepare et non modifie depuis. */
int LM_ecrire_maillage(LM_MAILLAGE *maillage, LM_FORMAT_FICHIER format,
		       const char *nom_fichier)
{
  try{
    _MaillageInterne* mai = trouver_maillage(maillage->refmai);
      
    /* ecriture fichier */
    mai->ecrire(nom_fichier, format_t(format));
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    return -1;
  }
  catch(...){
    fputs("Erreur inatendu\n", stderr);    
    return -1;
  }
  return 0;
}

#if defined(__INTERNE_MALIPP) && defined (__INTERNE_MALIPP_WRITER)
int LM_compression_zlib_malipp (int comprimer, int niveau)
{
	try
	{
		if (1 == comprimer)
			MaliPPWriter::activer_compression_zlib (niveau);
		else
			MaliPPWriter::desactiver_compression ( );
	}
	catch(erreur& err)
	{
		fputs(err.what ( ), stderr);
		return -1;
	}
	catch(...)
	{
		fputs ("Erreur inattendue\n", stderr);    
		return -1;
	}

	return 0;
}	/* LM_compression_zlib_malipp */
#endif	/* __INTERNE_MALIPP */


