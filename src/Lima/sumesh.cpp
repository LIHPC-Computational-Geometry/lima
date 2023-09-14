#ifdef __INTERNE_LMSUMESH
#ifdef __INTERNE_MACHINE_TYPES
#include <machine_types.h>
#endif

#include <Lima/size.h>
#include "LimaP/maillage_it.h"
#include "LimaP/surface_it.h"

extern "C" {  

  void sumesh(int_type *qn, double *x, double *y, int_type *qmq, 
	      int_type *mqn, int_type *ia, int_type *ni, double *om, 
	      int_type *lf, int_type *lx);
#if defined(_CRAY)
  void R1MESH(double* zz, double* rr, int_type* m, int_type* n, int_type* ia, 
	      int_type* ni, double* om, int_type* lf, int_type* lx, 
	      double* wx, int_type* i);
#else
  void r1mesh_(double* zz, double* rr, int_type* m, int_type* n, int_type* ia, 
	       int_type* ni, double* om, int_type* lf, int_type* lx, 
	       double* wx, int_type* i);
#endif
}

#if !defined(_IBMR2) && !defined(CRAY) && !defined(__hpux)
using namespace Lima;
#else
#if __IBMCPP__ >= 500
using namespace Lima;
#endif	// #if __IBMCPP__ >= 500
#endif	// #if !defined(_IBMR2) && ...

void sumesh(int_type *qn, double *x, double *y, int_type *qmq, 
	    int_type *mqn, int_type *ia, int_type *ni, double *om, 
	    int_type *lf, int_type *lx)
{
#if !defined(linux)
  size_type nd;
  
  _MaillageInterne mai;	// Allocation dans la pile pour destruction automatique
  
  for(nd=0; nd<*qn; ++nd)
    mai.ajouter(_NoeudInterne::create(nd+1, x[nd], y[nd], 0));
  
  _SurfaceInterne* la_surface = new _SurfaceInterne("TOUT");
  mai.ajouter(la_surface);
  
  for(size_type p=0; p<*qmq; p++){
    int_type* id = mqn+(p*4);
    _PolygoneInterne* le_polygone;
    
    if(id[3]){
      _NoeudInterne* n1 = mai.noeud_id(id[0]);
      _NoeudInterne* n2 = mai.noeud_id(id[1]);
      _NoeudInterne* n3 = mai.noeud_id(id[2]);
      _NoeudInterne* n4 = mai.noeud_id(id[3]);
      le_polygone = _PolygoneInterne::create(p+1, n1, n2, n3, n4);
    }
    else{
      _NoeudInterne* n1 = mai.noeud_id(id[0]);
      _NoeudInterne* n2 = mai.noeud_id(id[1]);
      _NoeudInterne* n3 = mai.noeud_id(id[2]);
      le_polygone = _PolygoneInterne::create(p+1, n1, n2, n3);
    }
    
    mai.ajouter(le_polygone);
    la_surface->ajouter(le_polygone);
  }

  mai.calculer_noeuds_bras();
  mai.calculer_noeuds_polygones();
  mai.calculer_polygones_bras();
  mai.calculer_surfaces_noeuds();
  mai.calculer_surfaces_bras();
  la_surface->structurer();
  mai.compacter_numerotation();

  if(la_surface->longueur_i()*la_surface->longueur_j() != la_surface->nb_polygones())
    throw argument_erreur("L'ensemble de mailles transmis n'est pas structure\n");

  double* zz = new double[la_surface->nb_noeuds_grille()];
  double* rr = new double[la_surface->nb_noeuds_grille()];
  if(zz==NULL || rr==NULL) 
    throw alloc_erreur("Probleme d'allocation dans lmsume\n");
  double* wx = NULL;
  if (*ia == 8) {
    /* besoin du tableau de travail wx */
    wx = new double[3*la_surface->nb_noeuds_grille()];
    memset(wx, 0, 3*la_surface->nb_noeuds_grille()*sizeof(double));
      if(wx==NULL) 
	throw alloc_erreur("Probleme d'allocation dans lmsume\n");
   }

  for(nd=0; nd<la_surface->nb_noeuds_grille(); ++nd){
    zz[nd] = la_surface->noeud_grille(nd)->x();
    rr[nd] = la_surface->noeud_grille(nd)->y();
  }
  
  int_type ier = 0;
  int_type m = la_surface->longueur_i()+1;
  int_type n = la_surface->longueur_j()+1;
#if defined(_CRAY)
  R1MESH(zz, rr, &m, &n, ia, ni, om, lf, lx, wx, &ier);
#else
  r1mesh_(zz, rr, &m, &n, ia, ni, om, lf, lx, wx, &ier);
#endif
  if(ier!=0)
    throw argument_erreur("Probleme dans r1mesh\n");
  
  for(nd=0; nd<la_surface->nb_noeuds_grille(); ++nd){
    size_type id = la_surface->noeud_grille(nd)->id();
    x[id-1] = zz[nd];
    y[id-1] = rr[nd];
  }
  
  if(wx) delete[] wx;
  delete[] zz;
  delete[] rr;
#endif	// #if !defined(linux)
}

#endif	// __INTERNE_LMSUMESH
