#include "LimaP/octoedre_it.h"
#include "LimaP/noeud_it.h"
#include "LimaP/bras_it.h"
#include "LimaP/polygone_it.h"
#include "LimaP/vecteur.h"


BEGIN_NAMESPACE_LIMA

// Indices des noeuds de la face.
size_type _OctoedreInterne::m_faces[8][7] = 
{ 
  {6, 0, 5, 4, 3, 2, 1}, 
  {6, 6, 7, 8, 9,10,11}, 
  {4, 0, 1, 7, 6, 0, 0}, 
  {4, 1, 2, 8, 7, 0, 0}, 
  {4, 2, 3, 9, 8, 0, 0}, 
  {4, 3, 4, 10, 9, 0, 0}, 
  {4, 4, 5, 11, 10, 0, 0}, 
  {4, 5, 0, 6, 11, 0, 0} 
};

// Nombre de noeuds et indices des noeuds de l'arete.
size_type _OctoedreInterne::m_aretes[18][2] = 
{ 
  {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 0}, {6, 7}, {7, 8}, {8, 9},
  {9, 10}, {10,11}, {11, 6}, {0, 6}, {1, 7}, {2, 8}, {3, 9}, {4, 10}, {5, 11}
};

// nombre d'aretes au noeud, indice de l'arete, indice du noeud oppose.
size_type _OctoedreInterne::m_noeud_aretes[12][4][2] =
{ 
  { {3, 0}, {0, 1}, {5,  5}, {12, 6} }, 
  { {3, 0}, {0, 0}, {1,  2}, {13, 7} },
  { {3, 0}, {1, 1}, {2,  3}, {14, 8} }, 
  { {3, 0}, {2, 2}, {3,  4}, {15, 9} }, 
  { {3, 0}, {3, 3}, {4,  5}, {16,10} },
  { {3, 0}, {5, 5}, {5,  0}, {17,11} }, 
  { {3, 0}, {6, 7}, {11,11}, {12, 0} }, 
  { {3, 0}, {6, 6}, {7,  8}, {13, 1} }, 
  { {3, 0}, {7, 7}, {8,  9}, {14, 2} }, 
  { {3, 0}, {8, 8}, {9, 10}, {15, 3} },
  { {3, 0}, {9, 9}, {10,11}, {16, 4} },
  { {3, 0}, {10,10}, {11,6}, {17, 5} }
};
 
// nombre d'aretes au noeud, indice de la face, nombre de noeuds,
// indices des noeud.
size_type _OctoedreInterne::m_noeud_faces[12][7][7] =
{ 
  { {3, 0, 0, 0, 0, 0, 0}, 
    {0, 6, 1, 2, 3, 4, 5}, 
    {2, 4, 1, 7, 6, 0, 0},
    {0, 6, 5, 4, 3, 2, 1},
    {7, 4, 5, 11, 6, 0, 0},
    {2, 4, 6, 7, 1, 0, 0},
    {7, 4, 6, 11, 5, 0, 0},
  },
  { {3, 0, 0, 0, 0, 0, 0}, 
    {0, 6, 0, 5, 4, 3, 2}, 
    {2, 4, 0, 6, 7, 0, 0},
    {0, 6, 2, 3, 4, 5, 0},
    {3, 4, 2, 8, 7, 0, 0},
    {2, 4, 7, 6, 0, 0, 0},
    {3, 4, 7, 8, 2, 0, 0},
  },
  { {3, 0, 0, 0, 0, 0, 0}, 
    {0, 6, 1, 0, 4, 3, 2}, 
    {3, 4, 1, 7, 8, 0, 0},
    {0, 6, 3, 4, 5, 0, 1},
    {4, 4, 3, 9, 8, 0, 0},
    {3, 4, 8, 7, 1, 0, 0},
    {4, 4, 8, 9, 3, 0, 0},
  },
  { {3, 0, 0, 0, 0, 0, 0}, 
    {0, 6, 2, 1, 0, 5, 4}, 
    {4, 4, 2, 8, 9, 0, 0},
    {0, 6, 4, 5, 0, 1, 2},
    {5, 4, 4, 10, 9, 0, 0},
    {4, 4, 9, 8, 2, 0, 0},
    {5, 4, 9, 10, 4, 0, 0},
  },
  { {3, 0, 0, 0, 0, 0, 0}, 
    {0, 6, 3, 2, 1, 0, 5}, 
    {5, 4, 3, 9,10, 0, 0},
    {0, 6, 5, 0, 1, 2, 3},
    {6, 4, 5,11,10, 0, 0},
    {5, 4,10, 9, 3, 0, 0},
    {6, 4,10,11, 5, 0, 0},
  },
  { {3, 0, 0, 0, 0, 0, 0}, 
    {0, 6, 0, 1, 2, 3, 4}, 
    {7, 4, 0, 6,11, 0, 0},
    {0, 6, 4, 3, 2, 1, 0},
    {6, 4, 4,10,11, 0, 0},
    {6, 4,11,10, 4, 0, 0},
    {7, 4,11, 6, 0, 0, 0},
  },
  { {3, 0, 0, 0, 0, 0, 0}, 
    {2, 4, 0, 1, 7, 0, 0}, 
    {7, 4, 0, 5,11, 0, 0},
    {1, 6, 7, 8, 9,10,11},
    {2, 4, 7, 1, 0, 0, 0},
    {1, 6,11,10, 9, 8,11},
    {7, 4,11, 5, 0, 0, 0},
  },
  { {3, 0, 0, 0, 0, 0, 0}, 
    {2, 4, 1, 0, 6, 0, 0}, 
    {3, 4, 1, 2, 8, 0, 0},
    {1, 6, 6,11,10, 9, 8},
    {2, 4, 6, 0, 1, 0, 0},
    {1, 6, 8, 9,10,11, 6},
    {3, 4, 8, 2, 1, 0, 0},
  },
  { {3, 0, 0, 0, 0, 0, 0}, 
    {3, 4, 2, 1, 7, 0, 0}, 
    {4, 4, 2, 3, 9, 0, 0},
    {1, 6, 7, 6,11,10, 9},
    {3, 4, 7, 1, 2, 0, 0},
    {1, 6, 9, 9,10,11, 6},
    {4, 4, 9, 3, 2, 0, 0},
  },
  { {3, 0, 0, 0, 0, 0, 0}, 
    {4, 4, 3, 2, 8, 0, 0}, 
    {5, 4, 3, 4,10, 0, 0},
    {1, 6, 8, 7, 6,11,10},
    {4, 4, 8, 2, 3, 0, 0},
    {1, 6,10,11, 6, 7, 8},
    {5, 4,10, 4, 3, 0, 0},
  },
  { {3, 0, 0, 0, 0, 0, 0}, 
    {5, 4, 4, 3, 9, 0, 0}, 
    {6, 4, 4, 5,11, 0, 0},
    {1, 6, 9, 8, 7, 6,11},
    {6, 4, 9, 3, 4, 0, 0},
    {1, 6,11, 6, 7, 8, 9},
    {5, 4,11, 5, 4, 0, 0},
  },
  { {3, 0, 0, 0, 0, 0, 0}, 
    {6, 4, 5, 4,10, 0, 0}, 
    {7, 4, 5, 0, 6, 0, 0},
    {1, 6, 6, 7, 8, 9,10},
    {7, 4, 6, 0, 5, 0, 0},
    {1, 6,10, 9, 8, 7, 6},
    {6, 4,10, 4, 5, 0, 0},
  }
};

double _OctoedreInterne::volume() const
{
	// CP, version 6.1.1
	// Décomposition en 2 hexaèdres :
	// (0, 1, 2, 3, 6, 7, 8, 9) :
	_Vecteur	v0_2 (*noeud (0), *noeud (2));
	_Vecteur	v0_7 (*noeud (0), *noeud (7));
	_Vecteur	v0_9 (*noeud (0), *noeud (9));
	_Vecteur	v1_8 (*noeud (1), *noeud (8));
	_Vecteur	v3_8 (*noeud (3), *noeud (8));
	_Vecteur	v6_8 (*noeud (6), *noeud (8));
	
	_Vecteur	v1	= v1_8 + v0_9;
	_Vecteur	v2	= v3_8 + v0_7;
	_Vecteur	v3	= v6_8 + v0_2;

	double	h1	= determinant (v1, v3_8, v0_2) + determinant (v0_9, v2, v6_8) +
				  determinant (v1_8, v0_7, v3);

	// (0, 3, 4, 5, 6, 9, 10, 11) :
	_Vecteur	v0_4 (*noeud (0), *noeud (4));
//	_Vecteur	v0_9 (*noeud (0), *noeud (9));
	_Vecteur	v0_11 (*noeud (0), *noeud (11));
	_Vecteur	v3_10 (*noeud (3), *noeud (10));
	_Vecteur	v5_10 (*noeud (5), *noeud (10));
	_Vecteur	v6_10 (*noeud (6), *noeud (10));
	
	_Vecteur	v4	= v3_10 + v0_11;
	_Vecteur	v5	= v5_10 + v0_9;
	_Vecteur	v6	= v6_10 + v0_4;

	double	h2	= determinant(v4, v5_10, v0_4) + determinant(v0_11, v5, v6_10) +
				  determinant(v3_10, v0_9, v6);

	return (h1 + h2) / 12.;
/*
  double v1, v2, v3, res;

  _Point p1, p2;
  
  p1.set_x((noeud(0)->x()+noeud(1)->x()+noeud(2)->x()+
	    noeud(3)->x()+noeud(4)->x()+noeud(5)->x())/6.0);
  p1.set_y((noeud(0)->y()+noeud(1)->y()+noeud(2)->y()+
	    noeud(3)->y()+noeud(4)->y()+noeud(5)->y())/6.0);
  p1.set_z((noeud(0)->z()+noeud(1)->z()+noeud(2)->z()+
	    noeud(3)->z()+noeud(4)->z()+noeud(5)->z())/6.0);
 
  p2.set_x((noeud(6)->x()+noeud(7)->x()+noeud(8)->x()+
	    noeud(9)->x()+noeud(10)->x()+noeud(11)->x())/6.0);
  p2.set_y((noeud(6)->y()+noeud(7)->y()+noeud(8)->y()+
	    noeud(9)->y()+noeud(10)->y()+noeud(11)->y())/6.0);
  p2.set_z((noeud(6)->z()+noeud(7)->z()+noeud(8)->z()+
	    noeud(9)->z()+noeud(10)->z()+noeud(11)->z())/6.0);
  
//    0 -> 0   1 -> 1   2 -> 2   3 -> p1
//    4 -> 6   5 -> 7   6 -> 8   7 -> p2
  v1 = 
    determinant(_Vecteur(*noeud(1), *noeud(8))+_Vecteur(*noeud(0), p2),
		_Vecteur(p1, *noeud(8)),
		_Vecteur(*noeud(0), *noeud(2)));
  v2 = 
    determinant(_Vecteur(*noeud(0), p2),
		_Vecteur(p1, *noeud(8))+_Vecteur(*noeud(0), *noeud(7)),
		_Vecteur(*noeud(6), *noeud(8)));
  v3 = 
    determinant(_Vecteur(*noeud(1), *noeud(8)),
		_Vecteur(*noeud(0), *noeud(7)),
		_Vecteur(*noeud(6), *noeud(8))+_Vecteur(*noeud(0), *noeud(2)));

  res = (v1 + v2 + v3)/12.0;

//    0 -> 2   1 -> 3   2 -> 4   3 -> p1
//    4 -> 8   5 -> 9   6 -> 10   7 -> p2
  v1 = 
    determinant(_Vecteur(*noeud(3), *noeud(10))+_Vecteur(*noeud(2), p2),
		_Vecteur(p1, *noeud(10)),
		_Vecteur(*noeud(2), *noeud(4)));
  v2 = 
    determinant(_Vecteur(*noeud(2), p2),
		_Vecteur(p1, *noeud(10))+_Vecteur(*noeud(2), *noeud(9)),
		_Vecteur(*noeud(8), *noeud(10)));
  v3 = 
    determinant(_Vecteur(*noeud(3), *noeud(10)),
		_Vecteur(*noeud(2), *noeud(9)),
		_Vecteur(*noeud(8), *noeud(10))+_Vecteur(*noeud(2), *noeud(4)));

  res += (v1 + v2 + v3)/12.0;

//    0 -> 4   1 -> 5   2 -> 0   3 -> p1
//    4 -> 10   5 -> 11   6 -> 6   7 -> p2
  v1 = 
    determinant(_Vecteur(*noeud(5), *noeud(6))+_Vecteur(*noeud(4), p2),
		_Vecteur(p1, *noeud(6)),
		_Vecteur(*noeud(4), *noeud(0)));
  v2 = 
    determinant(_Vecteur(*noeud(4), p2),
		_Vecteur(p1, *noeud(6))+_Vecteur(*noeud(4), *noeud(11)),
		_Vecteur(*noeud(4), *noeud(6)));
  v3 = 
    determinant(_Vecteur(*noeud(5), *noeud(6)),
		_Vecteur(*noeud(4), *noeud(11)),
		_Vecteur(*noeud(10), *noeud(6))+_Vecteur(*noeud(4), *noeud(0)));

  res += (v1 + v2 + v3)/12.0;

  return res;
*/
}
  
void _OctoedreInterne::retourner()
{
  IN_STD swap(m_noeuds[1], m_noeuds[5]);
  IN_STD swap(m_noeuds[2], m_noeuds[4]);
  IN_STD swap(m_noeuds[7], m_noeuds[11]);
  IN_STD swap(m_noeuds[8], m_noeuds[10]);
}
  
size_type _OctoedreInterne::nb_faces() const
{
  return NB_FACES;
}
  
size_type _OctoedreInterne::nb_aretes() const
{
  return NB_ARETES;
}
  
_BrasInterne* _OctoedreInterne::extraire_arete(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=NB_ARETES)
    throw range_erreur("Polyedre " + to_str(id()) + 
		       ", Indice local d'arete " +
		       to_str(n) + " non valide\n");
#endif
  return _BrasInterne::create(noeud(m_aretes[n][0]), noeud(m_aretes[n][1]));  
}

_PolygoneInterne* _OctoedreInterne::extraire_face(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=NB_FACES)
    throw range_erreur("Polyedre " + to_str(id()) + 
		       ", Indice local de face " +
		       to_str(n) + " non valide\n");
#endif   
  if(m_faces[n][0] == 4)
    return _PolygoneInterne::create(noeud(m_faces[n][1]), noeud(m_faces[n][2]), 
				    noeud(m_faces[n][3]), noeud(m_faces[n][4]));
  Tableau<_NoeudInterne*> t(6);
  t[0] = noeud(m_faces[n][1]);
  t[1] = noeud(m_faces[n][2]);
  t[2] = noeud(m_faces[n][3]);
  t[3] = noeud(m_faces[n][4]);
  t[4] = noeud(m_faces[n][5]);
  t[5] = noeud(m_faces[n][6]);
  return _PolygoneInterne::create(t);
}
  
int _OctoedreInterne::contenir(const _BrasInterne* br) const
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

int _OctoedreInterne::contenir(const _PolygoneInterne* pg) const
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

bool _OctoedreInterne::comparer(const _PolyedreInterne* p) const
{
  return false;
}

END_NAMESPACE_LIMA

