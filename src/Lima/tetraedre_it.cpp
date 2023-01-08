#include "LimaP/tetraedre_it.h"
#include "LimaP/noeud_it.h"
#include "LimaP/bras_it.h"
#include "LimaP/polygone_it.h"
#include "LimaP/vecteur.h"


BEGIN_NAMESPACE_LIMA

// Indices des noeuds de l'arete.
size_type _TetraedreInterne::m_faces[4][4] = 
{ 
  {3, 0, 2, 1}, 
  {3, 0, 3, 2}, 
  {3, 0, 1, 3}, 
  {3, 1, 2, 3} 
};

// Nombre de noeuds et indices des noeuds de la face.
size_type _TetraedreInterne::m_aretes[6][2] = 
{ 
  {0, 1}, {1, 2}, {2, 0}, {0, 3}, {1, 3}, {2, 3} 
};

// nombre d'aretes au noeud, indice de l'arete, indice du noeud oppose.
size_type _TetraedreInterne::m_noeud_aretes[4][4][2] =
{ 
  { {3, 0}, {0, 1}, {2, 2}, {3, 3} }, 
  { {3, 0}, {0, 0}, {1, 2}, {4, 3} },
  { {3, 0}, {1, 1}, {2, 0}, {5, 3} }, 
  { {3, 0}, {3, 0}, {4, 1}, {5, 2} } 
};
 
// nombre d'aretes au noeud, indice de la face, nombre de noeuds,
// indices des noeud.
size_type _TetraedreInterne::m_noeud_faces[4][7][4] =
{ 
  { {3, 0, 0, 0}, 
    {0, 3, 1, 2}, 
    {2, 3, 1, 3},
    {0, 3, 2, 1},
    {1, 3, 2, 3},
    {2, 3, 3, 1},
    {1, 3, 3, 2}
  },
  { {3, 0, 0, 0}, 
    {0, 3, 0, 2}, 
    {2, 3, 0, 3},
    {0, 3, 2, 0},
    {3, 3, 2, 3},
    {2, 3, 3, 0},
    {3, 3, 3, 2}
  },
  { {3, 0, 0, 0}, 
    {0, 3, 0, 1}, 
    {1, 3, 0, 3},
    {0, 3, 1, 0},
    {3, 3, 1, 3},
    {1, 3, 3, 0},
    {3, 3, 3, 1},
  },
  { {3, 0, 0, 0}, 
    {1, 3, 0, 2}, 
    {2, 3, 0, 1},
    {2, 3, 1, 0},
    {3, 3, 1, 2},
    {1, 3, 2, 0},
    {3, 3, 2, 1}
  }
};

double _TetraedreInterne::volume() const
{
  return determinant(_Vecteur(*noeud(0), *noeud(1)),
		     _Vecteur(*noeud(0), *noeud(2)),
		     _Vecteur(*noeud(0), *noeud(3)))/6.0;
}
  
void _TetraedreInterne::retourner()
{
  /* inversion du bord 12 et conservation face 1 */
  IN_STD swap(m_noeuds[0], m_noeuds[1]);
}
  
size_type _TetraedreInterne::nb_faces() const
{
  return NB_FACES;
}
  
size_type _TetraedreInterne::nb_aretes() const
{
  return NB_ARETES;
}
  
_BrasInterne* _TetraedreInterne::extraire_arete(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=NB_ARETES)
    throw range_erreur("Tetraedre " + to_str(id()) + 
		       ", Indice local d'arete " +
		       to_str(n) + " non valide\n");
#endif
  return _BrasInterne::create(noeud(m_aretes[n][0]), noeud(m_aretes[n][1]));  
}

_PolygoneInterne* _TetraedreInterne::extraire_face(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=NB_FACES)
    throw range_erreur("Tetraedre " + to_str(id()) + 
		       ", Indice local de face " +
		       to_str(n) + " non valide\n");
#endif   
  return _PolygoneInterne::create(noeud(m_faces[n][1]), noeud(m_faces[n][2]), 
				  noeud(m_faces[n][3]));
}
  
int _TetraedreInterne::contenir(const _BrasInterne* br) const
{
  for(int i=0; i<NB_NOEUDS; ++i){
    // recherche de l'accroche noeud origine du bras, noeud du maillage
    if(br->noeud(0) == noeud(i)){
      for(int a=1; a<=m_noeud_aretes[i][0][0]; ++a){
	// comparaison noeud extremite du bras, noeud extremites
	// des aretes partant du noeud du maillage.
	if(br->noeud(1) == noeud(m_noeud_aretes[i][a][1]))
	  return m_noeud_aretes[i][a][0];
      }
      return -1;
    }
  }
  return -1;
}

int _TetraedreInterne::contenir(const _PolygoneInterne* pg) const
{
  if(pg->nb_noeuds()!=3) return -1;

  for(int i=0; i<NB_NOEUDS; ++i){
    // recherche de l'accroche noeud origine du bras, noeud du maillage
    if(pg->noeud(0) == noeud(i)){
     for(int a=1; a<=m_noeud_faces[i][0][0]; ++a){
       // comparaison noeud extremite du premier bras du polygone, 
       // noeud extremites des aretes partant du noeud du maillage.
       if(pg->noeud(1) == noeud(m_noeud_faces[i][2*a][2])){
	 if(pg->noeud(2) == noeud(m_noeud_faces[i][2*a][3]))
	   return m_noeud_faces[i][2*a][0];
	 if(pg->noeud(2) == noeud(m_noeud_faces[i][2*a-1][3]))
	   return m_noeud_faces[i][2*a-1][0];
	 return -1;
       }
     }
     return -1;      
    }
  }
  return -1;
}
  
size_type _TetraedreInterne::POS[24][4] =
{
  {0, 1, 2, 3},
  {0, 1, 3, 2},
  {0, 2, 1, 3},
  {0, 2, 3, 1},
  {0, 3, 1, 2},
  {0, 3, 2, 1},
  {1, 0, 2, 3},
  {1, 0, 3, 2},
  {1, 2, 0, 3},
  {1, 2, 3, 0},
  {1, 3, 0, 2},
  {1, 3, 2, 0},
  {2, 0, 1, 3},
  {2, 0, 3, 1},
  {2, 1, 0, 3},
  {2, 1, 3, 0},
  {2, 3, 0, 1},
  {2, 3, 1, 0},
  {3, 0, 1, 2},
  {3, 0, 2, 1},
  {3, 1, 0, 2},
  {3, 1, 2, 0},
  {3, 2, 0, 1},
  {3, 2, 1, 0}
};

bool _TetraedreInterne::comparer(const _PolyedreInterne* p) const
{
  if(nb_noeuds()!=p->nb_noeuds()) return 0;
  
  for(int i=0; i<4; ++i){
    if(noeud(0)==p->noeud(POS[6*i][0])){
      for(int j=0; j<3; ++j){
	if(noeud(1)==p->noeud(POS[6*i+2*j][1])){
	  for(int k=0; k<2; ++k){
	    if(noeud(2)==p->noeud(POS[6*i+2*j+k][2])){
	      if(noeud(3)==p->noeud(POS[6*i+2*j+k][3]))
		return true;
	      return false;
	    }
	  }
	  return false;
	}
      }
      return false;
    }
  }
  return false;
}

END_NAMESPACE_LIMA

