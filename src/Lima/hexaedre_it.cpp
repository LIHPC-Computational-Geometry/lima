#include "LimaP/hexaedre_it.h"
#include "LimaP/noeud_it.h"
#include "LimaP/bras_it.h"
#include "LimaP/polygone_it.h"
#include "LimaP/vecteur.h"


BEGIN_NAMESPACE_LIMA

// Indices des noeuds de l'arete.
size_type _HexaedreInterne::m_faces[6][5] = 
{ 
  {4, 0, 3, 2, 1}, 
  {4, 0, 4, 7, 3}, 
  {4, 0, 1, 5, 4}, 
  {4, 4, 5, 6, 7}, 
  {4, 1, 2, 6, 5}, 
  {4, 2, 3, 7, 6} 
};

// Nombre de noeuds et indices des noeuds de la face.
size_type _HexaedreInterne::m_aretes[12][2] = 
{ 
  {0, 1}, {1, 2}, {2, 3}, {3, 0}, {0, 4}, {1, 5}, 
  {2, 6}, {3, 7}, {4, 5}, {5, 6}, {6, 7}, {7, 4}
};

// nombre d'aretes au noeud, indice de l'arete, indice du noeud oppose.
size_type _HexaedreInterne::m_noeud_aretes[8][4][2] =
{ 
  { {3, 0}, {0, 1}, {3,  3}, {4,  4} }, 
  { {3, 0}, {0, 0}, {1,  2}, {5,  5} },
  { {3, 0}, {1, 1}, {2,  3}, {6,  6} }, 
  { {3, 0}, {2, 2}, {3,  0}, {7,  7} }, 
  { {3, 0}, {4, 0}, {8,  5}, {11, 7} },
  { {3, 0}, {5, 1}, {8,  4}, {9,  6} }, 
  { {3, 0}, {6, 2}, {9,  5}, {10, 7} },
  { {3, 0}, {7, 3}, {10, 6}, {11, 4} }
};
 
// nombre d'aretes au noeud, indice de la face, nombre de noeuds,
// indices des noeud.
size_type _HexaedreInterne::m_noeud_faces[8][7][5] =
{ 
  { {3, 0, 0, 0, 0}, 
    {0, 4, 1, 2, 3}, 
    {2, 4, 1, 5, 4},
    {0, 4, 3, 2, 1},
    {1, 4, 3, 7, 4},
    {1, 4, 4, 7, 3},
    {2, 4, 4, 5, 1},
  },
  { {3, 0, 0, 0, 0}, 
    {0, 4, 0, 3, 2}, 
    {2, 4, 0, 4, 5},
    {0, 4, 2, 3, 0},
    {4, 4, 2, 6, 5},
    {2, 4, 5, 4, 0},
    {4, 4, 5, 6, 2},
  },
  { {3, 0, 0, 0, 0}, 
    {0, 4, 1, 0, 3}, 
    {4, 4, 1, 5, 6},
    {0, 4, 3, 0, 1},
    {5, 4, 3, 7, 6},
    {4, 4, 6, 5, 1},
    {5, 4, 6, 7, 3},
  },
  { {3, 0, 0, 0, 0}, 
    {0, 4, 0, 1, 2}, 
    {1, 4, 0, 4, 7},
    {0, 4, 2, 1, 0},
    {5, 4, 2, 6, 7},
    {1, 4, 7, 4, 0},
    {5, 4, 7, 6, 2},
  },
  { {3, 0, 0, 0, 0}, 
    {1, 4, 0, 3, 7}, 
    {2, 4, 0, 1, 5},
    {2, 4, 5, 1, 0},
    {3, 4, 5, 6, 7},
    {1, 4, 7, 3, 0},
    {3, 4, 7, 6, 5},
  },
  { {3, 0, 0, 0, 0}, 
    {2, 4, 1, 0, 4}, 
    {4, 4, 1, 2, 6},
    {2, 4, 4, 0, 1},
    {3, 4, 4, 7, 6},
    {3, 4, 6, 7, 4},
    {4, 4, 6, 2, 1},
  },
  { {3, 0, 0, 0, 0}, 
    {4, 4, 2, 1, 5}, 
    {5, 4, 2, 3, 7},
    {3, 4, 5, 4, 7},
    {4, 4, 5, 1, 2},
    {3, 4, 7, 4, 5},
    {5, 4, 7, 3, 2},
  },
  { {3, 0, 0, 0, 0}, 
    {1, 4, 3, 0, 4}, 
    {5, 4, 3, 2, 6},
    {1, 4, 4, 0, 3},
    {3, 4, 4, 5, 6},
    {3, 4, 6, 5, 4},
    {5, 4, 6, 2, 3},
  }
};

double _HexaedreInterne::volume() const
{

  double v1 = 
    determinant(_Vecteur(*noeud(1), *noeud(6))+_Vecteur(*noeud(0), *noeud(7)),
		_Vecteur(*noeud(3), *noeud(6)),
		_Vecteur(*noeud(0), *noeud(2)));
  double v2 = 
    determinant(_Vecteur(*noeud(0), *noeud(7)),
		_Vecteur(*noeud(3), *noeud(6))+_Vecteur(*noeud(0), *noeud(5)),
		_Vecteur(*noeud(4), *noeud(6)));
  double v3 = 
    determinant(_Vecteur(*noeud(1), *noeud(6)),
		_Vecteur(*noeud(0), *noeud(5)),
		_Vecteur(*noeud(4), *noeud(6))+_Vecteur(*noeud(0), *noeud(2)));

  double res = (v1 + v2 + v3)/12.0;

  return res;
}
  
void _HexaedreInterne::retourner()
{
  /* inversion du bord 12 et conservation face 1 */
  IN_STD swap(m_noeuds[0], m_noeuds[1]);
  IN_STD swap(m_noeuds[3], m_noeuds[2]);
  IN_STD swap(m_noeuds[4], m_noeuds[5]);
  IN_STD swap(m_noeuds[7], m_noeuds[6]);
}
  
size_type _HexaedreInterne::nb_faces() const
{
  return NB_FACES;
}
  
size_type _HexaedreInterne::nb_aretes() const
{
  return NB_ARETES;
}
  
_BrasInterne* _HexaedreInterne::extraire_arete(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=NB_ARETES)
    throw range_erreur("Polyedre " + to_str(id()) + 
		       ", Indice local d'arete " +
		       to_str(n) + " non valide\n");
#endif
  return _BrasInterne::create(noeud(m_aretes[n][0]), noeud(m_aretes[n][1]));  
}

_PolygoneInterne* _HexaedreInterne::extraire_face(size_type n) const
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
  
int _HexaedreInterne::contenir(const _BrasInterne* br) const
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

int _HexaedreInterne::contenir(const _PolygoneInterne* pg) const
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

size_type _HexaedreInterne::POS[48][8] =
{
  {0, 1, 2, 3, 4, 5, 6, 7},
  {0, 1, 5, 4, 3, 2, 6, 7},
  {0, 3, 2, 1, 4, 7, 6, 5},
  {0, 3, 7, 4, 1, 2, 6, 5},
  {0, 4, 5, 1, 3, 7, 6, 2},
  {0, 4, 7, 3, 1, 5, 6, 2},
  {1, 0, 3, 2, 5, 4, 7, 6},
  {1, 0, 4, 5, 2, 3, 7, 6},
  {1, 2, 3, 0, 5, 6, 7, 4},
  {1, 2, 6, 5, 0, 3, 7, 4},
  {1, 5, 4, 0, 2, 6, 7, 3},
  {1, 5, 6, 2, 0, 4, 7, 3},
  {2, 1, 0, 3, 6, 5, 4, 7},
  {2, 1, 5, 6, 3, 0, 4, 7},
  {2, 3, 0, 1, 6, 7, 4, 5},
  {2, 3, 7, 6, 1, 0, 4, 5},
  {2, 6, 5, 1, 3, 7, 4, 0},
  {2, 6, 7, 3, 1, 5, 4, 0},
  {3, 0, 1, 2, 7, 4, 5, 6},
  {3, 0, 4, 7, 2, 1, 5, 6},
  {3, 2, 1, 0, 7, 6, 5, 4},
  {3, 2, 6, 7, 0, 1, 5, 4},
  {3, 7, 4, 0, 2, 6, 5, 1},
  {3, 7, 6, 2, 0, 4, 5, 1},
  {4, 0, 1, 5, 7, 3, 2, 6},
  {4, 0, 3, 7, 5, 1, 2, 6},
  {4, 5, 1, 0, 7, 6, 2, 3},
  {4, 5, 6, 7, 0, 1, 2, 3},
  {4, 7, 3, 0, 5, 6, 2, 1},
  {4, 7, 6, 5, 0, 3, 2, 1},
  {5, 1, 0, 4, 6, 2, 3, 7},
  {5, 1, 2, 6, 4, 0, 3, 7},
  {5, 4, 0, 1, 6, 7, 3, 2},
  {5, 4, 7, 6, 1, 0, 3, 2},
  {5, 6, 2, 1, 4, 7, 3, 0},
  {5, 6, 7, 4, 1, 2, 3, 0},
  {6, 2, 1, 5, 7, 3, 0, 4},
  {6, 2, 3, 7, 5, 1, 0, 4},
  {6, 5, 1, 2, 7, 4, 0, 3},
  {6, 5, 4, 7, 2, 1, 0, 3},
  {6, 7, 3, 2, 5, 4, 0, 1},
  {6, 7, 4, 5, 2, 3, 0, 1},
  {7, 3, 0, 4, 6, 2, 1, 5},
  {7, 3, 2, 6, 4, 0, 1, 5},
  {7, 4, 0, 3, 6, 5, 1, 2},
  {7, 4, 5, 6, 3, 0, 1, 2},
  {7, 6, 2, 3, 4, 5, 1, 0},
  {7, 6, 5, 4, 3, 2, 1, 0}
};


bool _HexaedreInterne::comparer(const _PolyedreInterne* p) const
{
  if(nb_noeuds()!=p->nb_noeuds()) return 0;
  
  for(int i=0; i<8; ++i){
    if(noeud(0)==p->noeud(POS[6*i][0])){
      for(int j=0; j<3; ++j){
	if(noeud(1)==p->noeud(POS[6*i+2*j][1])){
	  for(int k=0; k<2; ++k){
	    if(noeud(2)==p->noeud(POS[6*i+2*j+k][2])){
	      if(noeud(3)!=p->noeud(POS[6*i+2*j+k][3]))
		return false;
	      if(noeud(4)!=p->noeud(POS[6*i+2*j+k][4]))
		return false;
	      if(noeud(5)!=p->noeud(POS[6*i+2*j+k][5]))
		return false;
	      if(noeud(6)!=p->noeud(POS[6*i+2*j+k][6]))
		return false;
	      if(noeud(7)!=p->noeud(POS[6*i+2*j+k][7]))
		return false;
	      return true;
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

