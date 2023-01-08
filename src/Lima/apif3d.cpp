#include <string.h>
#include <stdio.h>
// EXPURGE_BEGINNING_TAG LOCAL_DEPENDENCIES
// EXPURGE_COMPLETION_TAG

#include <Lima/lima.h>

#include "LimaP/noeud_it.h"
#include "LimaP/bras_it.h"
#include "LimaP/polygone_it.h"
#include "LimaP/polyedre_it.h"

#include "LimaP/nuage_it.h"
#include "LimaP/ligne_it.h"
#include "LimaP/surface_it.h"
#include "LimaP/volume_it.h"

#include "LimaP/maillage_it.h"

#include "LimaP/fonction.h"
#include "LimaP/algorithme.h"
#include <Lima/erreur.h>
#include "LimaP/util.h"
#include "LimaP/misc.h"

#define MAXFAC 6
#define MAXNOE 8

extern "C" {  

  void lm3init_(char* _ficmai, int_type* refmai, int_type* nbrnoe,
		int_type* nbrfce, int_type* nbrmai,  int_type* ier,
		int l_ficmai);

  void lm3xyz_(int_type* refmai, double* x, double* y, double* z,
	       int_type* ier);

  void lm3fcemai_(int_type* refmai, int_type* mayg, int_type* mayd, 
		  int_type* ier);

  void lm3nbsurf_(int_type* refmai, int_type* nbsurf, int_type* ier);

  void lm3lstsurf_(int_type* refmai, char* _nomsur, 
		   int_type* ier, int l_nomsur);

  void lm3surfdim_(int_type* refmai, char* _nomsur, int_type* nbrfce,
		   int_type* ier, int l_nomsur);

  void lm3surnumf_(int_type* refmai, char* _nomsur, int_type* numf,
		   int_type* ier, int l_nomsur);
  
  void lm3mvoi_(int_type* refmai, int_type* mvoi, int_type* ier);
  
  void lm3facz_(int_type* refmai, int_type* facz, int_type* ier);

  void lm3istrigo_(int_type* refmai, int_type* istrigo, int_type* ier);

  void lm3noeu_(int_type* refmai, int_type* noeu, int_type* ier);
}

#if !defined(_IBMR2) && !defined(CRAY) && !defined(__hpux)
using namespace Lima;
#else
#if __IBMCPP__ >= 500
using namespace Lima;
#endif	// #if __IBMCPP__ >= 500
#endif	// #if !defined(_IBMR2) && ...

/* lecture des generalites associees au maillage, le maillage etant
 * defini par son nom */
void lm3init_(char* _ficmai, int_type* refmai, int_type* nbrnoe,
	      int_type* nbrfce, int_type* nbrmai,  int_type* ier, 
	      int l_ficmai)
{
  try{   
    *ier = 0;
    string_fortran ficmai(_ficmai, l_ficmai);
    
    _MaillageInterne* mai = ouvrir_maillage(refmai);
    mai->lire(ficmai, SUFFIXE);
    mai->formater_noms ( );		// CP, version 5.4.4
    mai->preparer(CODE_T);

    *nbrnoe = mai->nb_noeuds();
    *nbrfce = mai->nb_polygones();
    *nbrmai = mai->nb_polyedres();
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inatendue\n", stderr);    
    *ier = -1;
  }    
}

void lm3xyz_(int_type* refmai, double* x, double* y, double* z,
	     int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    // indice 0 pointe dans le vide
    *x++ = 0;
    *y++ = 0;
    *z++ = 0;

    for(_MaillageInterne::const_iterator_noeud ptr = mai->premier_noeud();
	ptr != mai->dernier_noeud(); ++ptr, ++x, ++y, ++z){
      *x = (*ptr)->x();
      *y = (*ptr)->y();
      *z = (*ptr)->z();
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inatendue\n", stderr);    
    *ier = -1;
  }    
}

void lm3fcemai_(int_type* refmai, int_type* mayg, int_type* mayd, 
		int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    for(_MaillageInterne::const_iterator_polygone p = mai->premier_polygone(); 
	p != mai->dernier_polygone(); ++p, ++mayg, ++mayd){
      switch((*p)->nb_polyedres()){
      case 0:
	*mayg = 0;
	*mayd = 0;
	break;
      case 1:
	*mayg = (*p)->polyedre(0)->id();
	*mayd = 0;
	break;
      case 2:      
	*mayg = (*p)->polyedre(0)->id();
	*mayd = (*p)->polyedre(1)->id();
	break;
      }
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inatendue\n", stderr);    
    *ier = -1;
  }    
}

void lm3nbsurf_(int_type* refmai, int_type* nbsurf, int_type* ier)
{
  try{
    *ier = 0; 
    _MaillageInterne* mai = trouver_maillage(*refmai);

    *nbsurf = mai->nb_surfaces();
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inatendue\n", stderr);    
    *ier = -1;
  }    
}

void lm3lstsurf_(int_type* refmai, char* _nomsur, 
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
    fputs("Erreur inatendue\n", stderr);    
    *ier = -1;
  }    
}
  
void lm3surfdim_(int_type* refmai, char* _nomsur, int_type* nbrfce,
		 int_type* ier, int l_nomsur)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomsur(_nomsur, l_nomsur);
    _SurfaceInterne* surface = mai->surface(nomsur);
    *nbrfce = surface->nb_polygones(); 
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inatendue\n", stderr);    
    *ier = -1;
  }    
}

void lm3surnumf_(int_type* refmai, char* _nomsur, int_type* numf,
		 int_type* ier, int l_nomsur)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);

    string_fortran nomsur(_nomsur, l_nomsur);
    _SurfaceInterne* surface = mai->surface(nomsur);
    lire_elements(numf, surface->premier_polygone(), surface->dernier_polygone());
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inatendue\n", stderr);    
    *ier = -1;
  }    
}

void lm3mvoi_(int_type* refmai, int_type* mvoi, int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);
    memset(mvoi, 0, sizeof(int_type)*MAXFAC*(mai->nb_polyedres()+1));
    size_type nb_mai = mai->nb_polyedres();
    for(size_type i=0; i<nb_mai; ++i){
      _PolyedreInterne* p = mai->polyedre(i);
      for(size_type j=0; j<p->nb_polygones(); ++j){
	_PolygoneInterne* f = p->polygone(j);
	if(f->nb_polyedres() == 2){
	  if(f->polyedre(0)!=p)
	    *(mvoi+j*(nb_mai+1)+i+1) = f->polyedre(0)->id();
	  else
	    *(mvoi+j*(nb_mai+1)+i+1) = f->polyedre(1)->id();
	}
      }
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inatendue\n", stderr);    
    *ier = -1;
  }    
}

void lm3facz_(int_type* refmai, int_type* facz, int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);
    memset(facz, 0, sizeof(int_type)*MAXFAC*(mai->nb_polyedres()+1));
    size_type nb_mai = mai->nb_polyedres();
    for(size_type i=0; i<nb_mai; ++i){
      _PolyedreInterne* p = mai->polyedre(i);
      for(size_type j=0; j<p->nb_polygones(); ++j){
	*(facz+j*(nb_mai+1)+i+1) = p->polygone(j)->id();
      }
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inatendue\n", stderr);    
    *ier = -1;
  }    
}

void lm3istrigo_(int_type* refmai, int_type* istrigo, int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);
    memset(istrigo, 0, sizeof(int_type)*MAXFAC*(mai->nb_polyedres()+1));
    size_type nb_mai = mai->nb_polyedres();
    for(size_type i=0; i<nb_mai; ++i){
      _PolyedreInterne* p = mai->polyedre(i);
      for(size_type j=0; j<p->nb_polygones(); ++j){
	_PolygoneInterne* f = p->polygone(j);
	*(istrigo+j*(nb_mai+1)+i+1) = (f->polyedre(0)==p);
      }
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inatendue\n", stderr);    
    *ier = -1;
  }    
}

void lm3noeu_(int_type* refmai, int_type* noeu, int_type* ier)
{
  try{
    *ier = 0; 
    const _MaillageInterne* mai = trouver_maillage(*refmai);
    memset(noeu, 0, sizeof(int_type)*MAXNOE*(mai->nb_polyedres()+1));
    size_type nb_mai = mai->nb_polyedres();
    for(size_type i=0; i<nb_mai; ++i){
      _PolyedreInterne* p = mai->polyedre(i);
      for(size_type j=0; j<p->nb_noeuds(); ++j){
	*(noeu+j*(nb_mai+1)+i+1) = p->noeud(j)->id();
      }
    }
  }
  catch(erreur& err){
    fputs(err.what(), stderr);
    *ier = -1;
  }
  catch(...){
    fputs("Erreur inatendue\n", stderr);    
    *ier = -1;
  }    
}

