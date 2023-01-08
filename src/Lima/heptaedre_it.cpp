#include "LimaP/heptaedre_it.h"
#include "LimaP/noeud_it.h"
#include "LimaP/bras_it.h"
#include "LimaP/polygone_it.h"
#include "LimaP/vecteur.h"


BEGIN_NAMESPACE_LIMA

// Indices des noeuds de la face.
size_type _HeptaedreInterne::m_faces[7][6] = 
{ 
  {5, 0, 4, 3, 2, 1}, 
  {5, 5, 6, 7, 8, 9}, 
  {4, 0, 1, 6, 5, 0}, 
  {4, 1, 2, 7, 6, 0}, 
  {4, 2, 3, 8, 7, 0}, 
  {4, 3, 4, 9, 8, 0}, 
  {4, 4, 0, 5, 9, 0} 
};

// Nombre de noeuds et indices des noeuds de l'arete.
size_type _HeptaedreInterne::m_aretes[15][2] = 
{ 
  {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {5, 6}, {6, 7}, {7, 8},
  {8, 9}, {9, 5}, {0, 5}, {1, 6}, {2, 7}, {3, 8}, {4, 9}
};

// nombre d'aretes au noeud, indice de l'arete, indice du noeud oppose.
size_type _HeptaedreInterne::m_noeud_aretes[10][4][2] =
{ 
  { {3, 0}, {0, 1}, {4,  4}, {10, 5} }, 
  { {3, 0}, {0, 0}, {1,  2}, {11, 6} },
  { {3, 0}, {1, 1}, {2,  3}, {12, 7} }, 
  { {3, 0}, {2, 2}, {3,  4}, {13, 8} }, 
  { {3, 0}, {3, 3}, {4,  0}, {14, 9} },
  { {3, 0}, {5, 6}, {9,  9}, {10, 0} }, 
  { {3, 0}, {5, 5}, {6,  7}, {11, 1} }, 
  { {3, 0}, {6, 6}, {7,  8}, {12, 2} }, 
  { {3, 0}, {7, 7}, {8,  9}, {13, 3} },
  { {3, 0}, {8, 8}, {9,  5}, {14, 4} }
};
 
// nombre d'aretes au noeud, indice de la face, nombre de noeuds,
// indices des noeud.
size_type _HeptaedreInterne::m_noeud_faces[10][7][6] =
{ 
  { {3, 0, 0, 0, 0, 0}, 
    {0, 5, 1, 2, 3, 4}, 
    {2, 4, 1, 6, 5, 0},
    {0, 5, 4, 3, 2, 1},
    {6, 4, 4, 9, 5, 0},
    {2, 4, 5, 6, 1, 0},
    {6, 4, 5, 9, 4, 0},
  },
  { {3, 0, 0, 0, 0, 0}, 
    {0, 5, 0, 4, 3, 2}, 
    {2, 4, 0, 5, 6, 0},
    {0, 5, 2, 3, 4, 0},
    {3, 4, 2, 7, 6, 0},
    {2, 4, 6, 5, 0, 0},
    {3, 4, 6, 7, 2, 0},
  },
  { {3, 0, 0, 0, 0, 0}, 
    {0, 5, 1, 0, 4, 3}, 
    {3, 4, 1, 6, 7, 0},
    {0, 5, 3, 4, 0, 1},
    {4, 4, 3, 8, 7, 0},
    {3, 4, 7, 6, 1, 0},
    {4, 4, 7, 8, 3, 0},
  },
  { {3, 0, 0, 0, 0, 0}, 
    {0, 5, 2, 1, 0, 4}, 
    {4, 4, 2, 7, 8, 0},
    {0, 5, 4, 0, 1, 2},
    {5, 4, 4, 9, 8, 0},
    {4, 4, 8, 7, 2, 0},
    {5, 4, 8, 9, 4, 0},
  },
  { {3, 0, 0, 0, 0, 0}, 
    {0, 5, 0, 1, 2, 3}, 
    {6, 4, 0, 5, 9, 0},
    {0, 5, 3, 2, 1, 0},
    {5, 4, 3, 8, 9, 0},
    {5, 4, 9, 8, 3, 0},
    {6, 4, 9, 5, 0, 0},
  },
  { {3, 0, 0, 0, 0, 0}, 
    {2, 4, 0, 1, 6, 0}, 
    {6, 4, 0, 4, 9, 0},
    {1, 5, 6, 7, 8, 9},
    {2, 4, 6, 1, 0, 0},
    {1, 5, 9, 8, 7, 6},
    {6, 4, 9, 4, 0, 0},
  },
  { {3, 0, 0, 0, 0, 0}, 
    {2, 4, 1, 0, 5, 0}, 
    {3, 4, 1, 2, 7, 0},
    {1, 5, 5, 9, 8, 7},
    {2, 4, 5, 0, 1, 0},
    {1, 5, 7, 8, 9, 5},
    {3, 4, 7, 2, 1, 0},
  },
  { {3, 0, 0, 0, 0, 0}, 
    {3, 4, 2, 1, 6, 0}, 
    {4, 4, 2, 3, 8, 0},
    {1, 5, 6, 5, 9, 8},
    {3, 4, 6, 1, 2, 0},
    {1, 5, 8, 9, 5, 6},
    {4, 4, 8, 3, 2, 0},
  },
  { {3, 0, 0, 0, 0, 0}, 
    {4, 4, 3, 2, 7, 0}, 
    {5, 4, 3, 4, 9, 0},
    {1, 5, 7, 6, 5, 9},
    {4, 4, 7, 2, 3, 0},
    {1, 5, 9, 5, 6, 7},
    {5, 4, 9, 4, 3, 0},
  },
  { {3, 0, 0, 0, 0, 0}, 
    {5, 4, 4, 3, 8, 0}, 
    {6, 4, 4, 0, 5, 0},
    {1, 5, 5, 6, 7, 8},
    {6, 4, 5, 0, 4, 0},
    {1, 5, 8, 7, 6, 5},
    {5, 4, 8, 3, 4, 0},
  }
};

double _HeptaedreInterne::volume() const
{	// CP, version 6.1.1
	// Décomposition en hexaèdre + prisme à base triangulaire.
	// (0, 1, 2, 4, 5, 6, 7, 9)
	_Vecteur	v0_2 (*noeud (0), *noeud (2));
	_Vecteur	v0_6 (*noeud (0), *noeud (6));
	_Vecteur	v0_9 (*noeud (0), *noeud (9));
	_Vecteur	v1_7 (*noeud (1), *noeud (7));
	_Vecteur	v4_7 (*noeud (4), *noeud (7));
	_Vecteur	v5_7 (*noeud (5), *noeud (7));

	_Vecteur	v1	= v1_7 + v0_9;
	_Vecteur	v2	= v4_7 + v0_6;
	_Vecteur	v3	= v5_7 + v0_2;
	double	vh	= determinant (v1, v4_7, v0_2) + determinant (v0_9, v2, v5_7) +
				  determinant (v1_7, v0_6, v3);

	// (2, 3, 4, 7, 8, 9)
	_Vecteur	v2_4 (*noeud (2), *noeud (4));
	_Vecteur	v2_8 (*noeud (2), *noeud (8));
	_Vecteur	v2_9 (*noeud (2), *noeud (9));
	_Vecteur	v3_9 (*noeud (3), *noeud (9));
	_Vecteur	v4_9 (*noeud (4), *noeud (9));
	_Vecteur	v7_9 (*noeud (7), *noeud (9));

	_Vecteur	v4	= v3_9 + v2_9;
	_Vecteur	v5	= v4_9 + v2_8;
	_Vecteur	v6	= v7_9 + v2_4;
	double	vp	= determinant (v4, v4_9, v2_4) + determinant (v2_9, v5, v7_9) +
	        determinant (v3_9, v2_8, v6);

	return (vh + vp) / 12.;
/*
  double res = 0;

  _Point p1, p2;
  
  p1.set_x((noeud(0)->x()+noeud(1)->x()+noeud(2)->x()+
	    noeud(3)->x()+noeud(4)->x())/5.0);
  p1.set_y((noeud(0)->y()+noeud(1)->y()+noeud(2)->y()+
	    noeud(3)->y()+noeud(4)->y())/5.0);
  p1.set_z((noeud(0)->z()+noeud(1)->z()+noeud(2)->z()+
	    noeud(3)->z()+noeud(4)->z())/5.0);
 
  p2.set_x((noeud(5)->x()+noeud(6)->x()+noeud(7)->x()+
	    noeud(8)->x()+noeud(9)->x())/5.0);
  p2.set_y((noeud(5)->y()+noeud(6)->y()+noeud(7)->y()+
	    noeud(8)->y()+noeud(9)->y())/5.0);
  p2.set_z((noeud(5)->z()+noeud(6)->z()+noeud(7)->z()+
	    noeud(8)->z()+noeud(9)->z())/5.0);
 
  for(int i=0; i<5; ++i){
    res += determinant(_Vecteur(*noeud(i), *noeud((i+1)%5)),
		       _Vecteur(*noeud(i), p1),
		       _Vecteur(*noeud(i), *noeud(i+5))) +
      determinant(_Vecteur(*noeud((i+1)%5), p1),
		  _Vecteur(*noeud((i+1)%5), *noeud(i+5)),
		  _Vecteur(*noeud((i+1)%5), *noeud(5+(i+1)%5))) +
      determinant(_Vecteur(p1, *noeud(3)),
		  _Vecteur(p1, *noeud(5+(i+1)%5)),
		  _Vecteur(p1, p2)) ;
  }
  
  return res;
*/
}
  
void _HeptaedreInterne::retourner()
{
  IN_STD swap(m_noeuds[4], m_noeuds[1]);
  IN_STD swap(m_noeuds[2], m_noeuds[3]);
  IN_STD swap(m_noeuds[6], m_noeuds[9]);
  IN_STD swap(m_noeuds[7], m_noeuds[8]);
}
  
size_type _HeptaedreInterne::nb_faces() const
{
  return NB_FACES;
}
  
size_type _HeptaedreInterne::nb_aretes() const
{
  return NB_ARETES;
}
  
_BrasInterne* _HeptaedreInterne::extraire_arete(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=NB_ARETES)
    throw range_erreur("Polyedre " + to_str(id()) + 
		       ", Indice local d'arete " +
		       to_str(n) + " non valide\n");
#endif
  return _BrasInterne::create(noeud(m_aretes[n][0]), noeud(m_aretes[n][1]));  
}

_PolygoneInterne* _HeptaedreInterne::extraire_face(size_type n) const
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
  Tableau<_NoeudInterne*> t(5);
  t[0] = noeud(m_faces[n][1]);
  t[1] = noeud(m_faces[n][2]);
  t[2] = noeud(m_faces[n][3]);
  t[3] = noeud(m_faces[n][4]);
  t[4] = noeud(m_faces[n][5]);
  return _PolygoneInterne::create(t);
}
  
int _HeptaedreInterne::contenir(const _BrasInterne* br) const
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

int _HeptaedreInterne::contenir(const _PolygoneInterne* pg) const
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

bool _HeptaedreInterne::comparer(const _PolyedreInterne* p) const
{
  return false;
}

END_NAMESPACE_LIMA

