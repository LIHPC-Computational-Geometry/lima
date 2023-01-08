#include "LimaP/polygone_it.h"
#include "LimaP/triangle_it.h"
#include "LimaP/quadrangle_it.h"
#include "LimaP/noeud_it.h"
#include "LimaP/bras_it.h"
#include <Lima/erreur.h>
#include "LimaP/algorithme.h"
#include "LimaP/vecteur.h"


BEGIN_NAMESPACE_LIMA

#ifndef LIMA_MT
size_type _PolygoneInterne::m_max_id = 0;
#endif	// LIMA_MT



 
_PolygoneInterne::_PolygoneInterne (
					_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2)
#ifndef LIMA_MT
	: _ElementFini (++m_max_id), m_noeuds (3)
#else
	: _ElementFini (0), m_noeuds (3)
#endif	// LIMA_MT
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
}	// _PolygoneInterne::_PolygoneInterne

 
_PolygoneInterne::_PolygoneInterne (
		size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2)
	: _ElementFini (n),  m_noeuds (3)
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
#ifndef LIMA_MT
  m_max_id = (m_max_id<n)? n:m_max_id;
#endif	// LIMA_MT
}	// _PolygoneInterne::_PolygoneInterne

 
_PolygoneInterne::_PolygoneInterne (
	_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2, _NoeudInterne* n3)
#ifndef LIMA_MT
	: _ElementFini (++m_max_id), m_noeuds (4)
#else
	: _ElementFini (0), m_noeuds (4)
#endif	// LIMA_MT
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
}	// _PolygoneInterne::_PolygoneInterne

 
_PolygoneInterne::_PolygoneInterne (
		size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
		_NoeudInterne* n3)
	: _ElementFini (n),  m_noeuds (4)
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
#ifndef LIMA_MT
  m_max_id = (m_max_id<n)? n:m_max_id;
#endif	// LIMA_MT
}	// _PolygoneInterne::_PolygoneInterne

 
_PolygoneInterne::_PolygoneInterne (const Tableau<_NoeudInterne*>& nds)
#ifndef LIMA_MT
	: _ElementFini (++m_max_id), m_noeuds (nds)
#else
	: _ElementFini (0), m_noeuds (nds)
#endif	// LIMA_MT
{
}	// _PolygoneInterne::_PolygoneInterne

 
_PolygoneInterne::_PolygoneInterne (
							size_type n, const Tableau<_NoeudInterne*>& nds)
	: _ElementFini (n), m_noeuds (nds)
{
#ifndef LIMA_MT
  m_max_id = (m_max_id<n)? n:m_max_id;
#endif	// LIMA_MT
}	// _PolygoneInterne::_PolygoneInterne


void* _PolygoneInterne::operator new(size_t)
{
  return ClassAllocateur<_PolygoneInterne>::allouer();
}
  

void _PolygoneInterne::operator delete(void* ptr)
{
  ClassAllocateur<_PolygoneInterne>::liberer(ptr);    
}

 _PolygoneInterne* 
_PolygoneInterne::create(const Tableau<_NoeudInterne*>& nds)
{
  return new _PolygoneInterne(nds);
}

 _PolygoneInterne* 
_PolygoneInterne::create(size_type n, const Tableau<_NoeudInterne*>& nds)
{
  return new _PolygoneInterne(n, nds);
}


size_type _PolygoneInterne::nb_noeuds() const 
{ 
  return m_noeuds.size(); 
}


size_type _PolygoneInterne::nb_bras() const 
{ 
  return m_bras.size(); 
}


size_type _PolygoneInterne::nb_polyedres() const 
{ 
  return m_polyedres.size(); 
}


_NoeudInterne* _PolygoneInterne::noeud(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_noeuds())
    throw range_erreur("Polygone " + to_str(id()) + 
		       ", Indice local de noeud " +
		       to_str(n) + " non valide\n");
#endif
  return m_noeuds[n]; 
}


_BrasInterne* _PolygoneInterne::bras(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_bras())
    throw range_erreur("Polygone " + to_str(id()) + 
		       ", Indice local de bras " +
		       to_str(n) + " non valide\n");
#endif
  return m_bras[n]; 
}


_PolyedreInterne* _PolygoneInterne::polyedre(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_polyedres())
    throw range_erreur("Polygone " + to_str(id()) + 
		       ", Indice local de polyèdre " +
		       to_str(n) + " non valide\n");
#endif
  return m_polyedres[n]; 
}


size_type _PolygoneInterne::nb_aretes() const
{
  return nb_noeuds();
}


void _PolygoneInterne::reserver_bras()
{
  m_bras.resize(nb_aretes());
}


void _PolygoneInterne::connecter(_BrasInterne* br, size_type pos)
{
  m_bras[pos] = br;
}


void _PolygoneInterne::connecter(_PolyedreInterne* pd)
{
  m_polyedres.push_back(pd);
}


void _PolygoneInterne::invalider_polyedres()
{
  m_polyedres.resize(0);
}


void _PolygoneInterne::invalider_bras()
{
  m_bras.resize(0);
}


void _PolygoneInterne::copier(const _PolygoneInterne* pg)
{
  m_noeuds = pg->m_noeuds;
}


_BrasInterne* _PolygoneInterne::element_oppose(const _BrasInterne* br) const
{
  return element_oppose_(m_bras, br);
}


_BrasInterne* _PolygoneInterne::bras_suivant(const _BrasInterne* br) const
{
  return element_suivant(m_bras, br);
}


_BrasInterne* _PolygoneInterne::bras_precedent(const _BrasInterne* br) const
{
  return element_precedent(m_bras, br);
}


_PolygoneInterne* 
_PolygoneInterne::create(_NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2)
{
  return new _TriangleInterne(n0, n1, n2);
}

_PolygoneInterne* 
_PolygoneInterne::create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2)
{
  return new _TriangleInterne(n, n0, n1, n2);
}

_PolygoneInterne* 
_PolygoneInterne::create(_NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3)
{
  return new _QuadrangleInterne(n0, n1, n2, n3);
}

_PolygoneInterne* 
_PolygoneInterne::create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3)
{
  return new _QuadrangleInterne(n, n0, n1, n2, n3);
}

_PolygoneInterne* 
_PolygoneInterne::create(_NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3,
			 _NoeudInterne* n4)
{
  Tableau<_NoeudInterne*> t(5);
  t[0] = n0;
  t[1] = n1;
  t[2] = n2;
  t[3] = n3;
  t[4] = n4;
  return _PolygoneInterne::create(t);
}

_PolygoneInterne* 
_PolygoneInterne::create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3,
			 _NoeudInterne* n4)
{
  Tableau<_NoeudInterne*> t(5);
  t[0] = n0;
  t[1] = n1;
  t[2] = n2;
  t[3] = n3;
  t[4] = n4;
  return _PolygoneInterne::create(n, t);
}

_PolygoneInterne* 
_PolygoneInterne::create(_NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3,
			 _NoeudInterne* n4, _NoeudInterne* n5)
{
  Tableau<_NoeudInterne*> t(6);
  t[0] = n0;
  t[1] = n1;
  t[2] = n2;
  t[3] = n3;
  t[4] = n4;
  t[5] = n5;
  return _PolygoneInterne::create(t);
}

_PolygoneInterne* 
_PolygoneInterne::create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3,
			 _NoeudInterne* n4, _NoeudInterne* n5)
{
  Tableau<_NoeudInterne*> t(6);
  t[0] = n0;
  t[1] = n1;
  t[2] = n2;
  t[3] = n3;
  t[4] = n4;
  t[5] = n5;
  return _PolygoneInterne::create(n, t);
}

/*
  Calcul de la surface d'un polygone quelconque par developpement
  de la formule de Green.
*/
double _PolygoneInterne::surface2D() const
{
	double surface	= 0.;
    
	// CP, version 6.1.1. Spécialisation pentagones/hexagones :
	const size_type	nbNoeuds	= nb_noeuds ( );

	if (4 <= nbNoeuds)
	{	// Triangularisation barycentrique :
		double		x	= 0., y = 0., z = 0.;
		size_type	i	= 0;
		for (i = 0; i < nbNoeuds; i++)
		{
			_NoeudInterne*	n	= noeud (i);
			x	+= n->x ( );
			y	+= n->y ( );
			z	+= n->z ( );
		}
		_Point	barycentre (x / nbNoeuds, y / nbNoeuds, z / nbNoeuds);

		for (i = 0; i < nbNoeuds - 1; i++)
			surface	+= (_Vecteur (*noeud (i), *(noeud (i + 1))) ^
					   _Vecteur (*noeud (i), barycentre)).norme ( );
		surface	+= (_Vecteur (*noeud (nbNoeuds - 1), *(noeud (0))) ^
			   _Vecteur (*noeud (0), barycentre)).norme ( );

		surface /= 2.;
	}
	else	// Développement de la formule de Green :
	{
		for(size_type nd = 0; nd < nbNoeuds; ++nd)
		{
			const _NoeudInterne* noe1 = noeud(nd);
			const _NoeudInterne* noe2 = noeud ((nd+1) % nb_noeuds( )); 
			surface += noe1->x ( )*noe2->y ( ) - noe1->y ( )*noe2->x ( );
		}
	}
/*
  for(size_type nd=0; nd<nb_noeuds(); ++nd){
    const _NoeudInterne* noe1 = noeud(nd);
    const _NoeudInterne* noe2 = noeud((nd+1) % nb_noeuds()); 
    surface += noe1->x()*noe2->y() - noe1->y()*noe2->x();
  }
*/
	return surface;
}
  
void _PolygoneInterne::retourner()
{
  /* inversion du sens de parcours 
   * et decalage pour compatibilite avec ancienne version */
  for(size_type nd=1; nd<=nb_noeuds()/2; nd++){
    IN_STD swap(m_noeuds[nd], m_noeuds[(nb_noeuds()+1-nd)%nb_noeuds()]);
  }
}
  
_BrasInterne* _PolygoneInterne::extraire_arete(size_type n) const
{ 
#ifdef RANGE_CHECKING
  if(n>=nb_aretes())
    throw range_erreur("Polygone " + to_str(id()) + 
		       ", Indice local d'arete " +
		       to_str(n) + " non valide\n");
#endif
  return _BrasInterne::create(noeud(n), noeud((n+1)%nb_noeuds()));  
}
  
int _PolygoneInterne::contenir(const _BrasInterne* br) const
{
  for(int p=0; p<nb_noeuds(); p++){
    if(noeud(p)==br->noeud(0)){
      if(noeud((p+1)%nb_noeuds())==br->noeud(1))
	return p;      
      if(noeud((p+nb_noeuds()-1)%nb_noeuds())==br->noeud(1))
	return (p+nb_noeuds()-1)%nb_noeuds();
      return -1;
    }
  }
  return -1;
}

bool _PolygoneInterne::comparer(const _PolygoneInterne* p1, 
				const _PolygoneInterne* p2)
{
  if(p1->nb_noeuds()!=p2->nb_noeuds()) return 0;
    
  for(size_type n1=0; n1<p1->nb_noeuds(); ++n1){
    if(p1->noeud(0)==p2->noeud(n1)){
      if(p1->noeud(1)==p2->noeud((n1+1)%p1->nb_noeuds())){
	for(size_type n2=2; n2<p1->nb_noeuds(); ++n2){
	  if(p1->noeud(n2)!=p2->noeud((n1+n2)%p1->nb_noeuds())) return 0;
	}
	return 1;
      }
      else if(p1->noeud(1) == 
	      p2->noeud((n1+p1->nb_noeuds()-1)%p1->nb_noeuds())){
	for(size_type n2=2; n2<p1->nb_noeuds(); ++n2){
	  if(p1->noeud(n2) != 
	     p2->noeud((n1-n2+p1->nb_noeuds())%p1->nb_noeuds()))
	    return 0;
	}
	return 1;
      }
    }
  }
  return 0;
}


void _PolygoneInterne::remplacer_noeud (_NoeudInterne* n1, _NoeudInterne* n2)
{
	const size_type	nombre	= m_noeuds.size ( );

	for (size_type i = 0; i < nombre; i++)
		if (m_noeuds [i] == n1)
			m_noeuds [i]	= n2;
}	// _PolygoneInterne::remplacer_noeud

END_NAMESPACE_LIMA

