#include "LimaP/prisme_it.h"
#include "LimaP/noeud_it.h"
#include "LimaP/bras_it.h"
#include "LimaP/polygone_it.h"
#include "LimaP/vecteur.h"


BEGIN_NAMESPACE_LIMA

// Indices des noeuds de l'arete.
size_type _PrismeInterne::m_faces[5][5] = 
{ 
  {3, 0, 2, 1, 0}, 
  {4, 0, 3, 5, 2}, 
  {4, 0, 1, 4, 3}, 
  {3, 3, 4, 5, 0}, 
  {4, 1, 2, 5, 4} 
};

// Nombre de noeuds et indices des noeuds de la face.
size_type _PrismeInterne::m_aretes[9][2] = 
{ 
  {0, 1}, {1, 2}, {2, 0}, {0, 3}, {1, 4}, {2, 5}, {3, 4}, {4, 5}, {5, 3}
};

// nombre d'aretes au noeud, indice de l'arete, indice du noeud oppose.
size_type _PrismeInterne::m_noeud_aretes[6][4][2] =
{ 
  { {3, 0}, {0, 1}, {2, 2}, {3, 3} }, 
  { {3, 0}, {0, 0}, {1, 2}, {4, 4} },
  { {3, 0}, {1, 1}, {2, 0}, {5, 5} }, 
  { {3, 0}, {3, 0}, {6, 4}, {8, 5} }, 
  { {3, 0}, {4, 1}, {6, 3}, {7, 5} },
  { {3, 0}, {5, 2}, {7, 4}, {8, 3} }
};
 
// nombre d'aretes au noeud, indice de la face, nombre de noeuds,
// indices des noeud.
size_type _PrismeInterne::m_noeud_faces[6][7][5] =
{ 
  { {3, 0, 0, 0, 0}, 
    {0, 3, 1, 2, 0}, 
    {2, 4, 1, 4, 3},
    {0, 3, 2, 1, 0},
    {1, 4, 2, 5, 3},
    {1, 4, 3, 5, 2},
    {2, 4, 3, 4, 1},
  },
  { {3, 0, 0, 0, 0}, 
    {0, 3, 0, 2, 0}, 
    {2, 4, 0, 3, 4},
    {0, 3, 2, 0, 0},
    {4, 4, 2, 5, 4},
    {2, 4, 4, 3, 0},
    {4, 4, 4, 5, 2},
  },
  { {3, 0, 0, 0, 0}, 
    {0, 3, 0, 1, 0}, 
    {1, 4, 0, 3, 5},
    {0, 3, 1, 0, 0},
    {4, 4, 1, 4, 5},
    {1, 4, 5, 3, 0},
    {4, 4, 5, 4, 1},
  },
  { {3, 0, 0, 0, 0}, 
    {1, 4, 0, 2, 5}, 
    {2, 4, 0, 1, 4},
    {2, 4, 4, 1, 0},
    {3, 3, 4, 5, 0},
    {1, 3, 5, 2, 0},
    {3, 3, 5, 4, 0},
  },
  { {3, 0, 0, 0, 0}, 
    {2, 4, 1, 0, 3}, 
    {4, 4, 1, 2, 5},
    {2, 4, 3, 0, 1},
    {3, 3, 3, 5, 0},
    {3, 3, 5, 3, 0},
    {4, 4, 5, 2, 1},
  },
  { {3, 0, 0, 0, 0}, 
    {1, 4, 2, 0, 3}, 
    {4, 4, 2, 1, 4},
    {1, 4, 3, 0, 2},
    {3, 3, 3, 4, 0},
    {3, 3, 4, 3, 0},
    {4, 3, 4, 1, 2},
  }
};

double _PrismeInterne::volume() const
{
	// CP, version 6.1.1
	_Vecteur	v0_2 (*noeud (0), *noeud (2));
	_Vecteur	v0_4 (*noeud (0), *noeud (4));
	_Vecteur	v0_5 (*noeud (0), *noeud (5));
	_Vecteur	v1_5 (*noeud (1), *noeud (5));
	_Vecteur	v2_5 (*noeud (2), *noeud (5));
	_Vecteur	v3_5 (*noeud (3), *noeud (5));

	_Vecteur	v1	= v1_5 + v0_5;
	_Vecteur	v2	= v2_5 + v0_4;
	_Vecteur	v3	= v3_5 + v0_2;
	return (determinant (v1, v2_5, v0_2) + determinant (v0_5, v2, v3_5) +
	        determinant (v1_5, v0_4, v3)) / 12.;
/*
  return 
    determinant(_Vecteur(*noeud(0), *noeud(1)),
		_Vecteur(*noeud(0), *noeud(2)),
		_Vecteur(*noeud(0), *noeud(3))) +
    determinant(_Vecteur(*noeud(1), *noeud(2)),
		_Vecteur(*noeud(1), *noeud(3)),
		_Vecteur(*noeud(1), *noeud(4))) +
    determinant(_Vecteur(*noeud(2), *noeud(3)),
		_Vecteur(*noeud(2), *noeud(4)),
		_Vecteur(*noeud(2), *noeud(5))) ;
*/
}
  
void _PrismeInterne::retourner()
{
  /* inversion du bord 12 et conservation face 1 */
  IN_STD swap(m_noeuds[0], m_noeuds[1]);
  IN_STD swap(m_noeuds[3], m_noeuds[4]);
}
  
size_type _PrismeInterne::nb_faces() const
{
  return NB_FACES;
}
  
size_type _PrismeInterne::nb_aretes() const
{
  return NB_ARETES;
}
  
_BrasInterne* _PrismeInterne::extraire_arete(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=NB_ARETES)
    throw range_erreur("Polyedre " + to_str(id()) + 
		       ", Indice local d'arete " +
		       to_str(n) + " non valide\n");
#endif
  return _BrasInterne::create(noeud(m_aretes[n][0]), noeud(m_aretes[n][1]));  
}

_PolygoneInterne* _PrismeInterne::extraire_face(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=NB_FACES)
    throw range_erreur("Polyedre " + to_str(id()) + 
		       ", Indice local de face " +
		       to_str(n) + " non valide\n");
#endif   
  if(m_faces[n][0] == 3)
    return _PolygoneInterne::create(noeud(m_faces[n][1]), 
				    noeud(m_faces[n][2]), 
				    noeud(m_faces[n][3]));

  return _PolygoneInterne::create(noeud(m_faces[n][1]), noeud(m_faces[n][2]), 
				  noeud(m_faces[n][3]),noeud(m_faces[n][4]));
}
  
int _PrismeInterne::contenir(const _BrasInterne* br) const
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

int _PrismeInterne::contenir(const _PolygoneInterne* pg) const
{
  for(int i=0; i<NB_NOEUDS; ++i){
    // recherche de l'accroche noeud origine du bras, noeud du maillage
    if(pg->noeud(0) == noeud(i)){
     for(int a=1; a<=m_noeud_faces[i][0][0]; ++a){
       // comparaison noeud extremite du premier bras du polygone, 
       // noeud extremites des aretes partant du noeud du maillage.
       if(pg->noeud(1) == noeud(m_noeud_faces[i][2*a][2])){
	 if(pg->nb_noeuds() ==  m_noeud_faces[i][2*a][1] &&
	    pg->noeud(2) == noeud(m_noeud_faces[i][2*a][3])){
	   if(m_noeud_faces[i][2*a][1] == 3)
	     return m_noeud_faces[i][2*a][0];
	   else{
	     if(pg->noeud(3) == noeud(m_noeud_faces[i][2*a][4]))
	       return m_noeud_faces[i][2*a][0];
	   }
	   return -1;
	 }
	 if(pg->nb_noeuds() ==  m_noeud_faces[i][2*a-1][1] &&
	    pg->noeud(2) == noeud(m_noeud_faces[i][2*a-1][3])){
	   if(m_noeud_faces[i][2*a-1][1] == 3)
	     return m_noeud_faces[i][2*a-1][0];
	   else{
	     if(pg->noeud(3) == noeud(m_noeud_faces[i][2*a-1][4]))
	       return m_noeud_faces[i][2*a-1][0];
	   }
	   return -1;
	 }
	 return -1;
       }
     }
     return -1;      
    }
  }
  return -1;
}

size_type _PrismeInterne::POS[12][6] =
{
  {0, 1, 2, 3, 4, 5},
  {0, 2, 1, 3, 5, 4},
  {1, 2, 0, 4, 5, 3},
  {1, 0, 2, 4, 3, 5},
  {2, 0, 1, 5, 3, 4},
  {2, 1, 0, 5, 4, 3},
  {3, 4, 5, 0, 1, 2},
  {3, 5, 4, 0, 2, 1},
  {4, 5, 3, 1, 2, 0},
  {4, 3, 5, 1, 0, 2},
  {5, 3, 4, 2, 0, 1},
  {5, 4, 3, 2, 1, 0}
};

bool _PrismeInterne::comparer(const _PolyedreInterne* p) const
{
  if(nb_noeuds()!=p->nb_noeuds()) return 0;

  for(int i=0; i<6; ++i){
    if(noeud(0)==p->noeud(POS[2*i][0])){
      for(int j=0; j<2; ++j){
	if(noeud(1)==p->noeud(POS[2*i+j][1])){
	  if(noeud(2)!=p->noeud(POS[2*i+j][2]))
	    return false;
	  if(noeud(3)!=p->noeud(POS[2*i+j][3]))
	    return false;	    
	  if(noeud(4)!=p->noeud(POS[2*i+j][4]))
	    return false;	    
	  if(noeud(5)!=p->noeud(POS[2*i+j][5]))
	    return false;	    
	  return true;
	}
      }
      return false;
    }
  }
  return false;
}

END_NAMESPACE_LIMA

