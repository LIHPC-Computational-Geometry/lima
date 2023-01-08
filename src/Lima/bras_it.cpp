#include "LimaP/bras_it.h"
#include "LimaP/noeud_it.h"
#include "LimaP/polygone_it.h"
#include "LimaP/polyedre_it.h"
#include "LimaP/algorithme.h"
#include "LimaP/vecteur.h"

BEGIN_NAMESPACE_LIMA

#ifndef LIMA_MT
size_type _BrasInterne::m_max_id = 0;
#endif	// LIMA_MT


 
_BrasInterne::_BrasInterne(_NoeudInterne* n0, _NoeudInterne* n1)
#ifndef LIMA_MT
	: _ElementFini (++m_max_id), m_noeuds(2)
#else
	: _ElementFini (0), m_noeuds(2)
#endif	// LIMA_MT
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
}	// _ElementFini::_ElementFini


void* _BrasInterne::operator new(size_t)
{
  return ClassAllocateur<_BrasInterne>::allouer();
}
  

void _BrasInterne::operator delete(void* ptr)
{
  ClassAllocateur<_BrasInterne>::liberer(ptr);    
}

 
_BrasInterne::_BrasInterne (size_type n, _NoeudInterne* n0, _NoeudInterne* n1)
	: _ElementFini (n),  m_noeuds(2)
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
#ifndef LIMA_MT
  m_max_id = (m_max_id<n)? n:m_max_id;
#endif	// LIMA_MT
}


 _BrasInterne* 
_BrasInterne::create(_NoeudInterne* n0, _NoeudInterne* n1)
{
  return new _BrasInterne(n0, n1);
}

 _BrasInterne* 
_BrasInterne::create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1)
{
  return new _BrasInterne(n, n0, n1);
}


size_type _BrasInterne::nb_noeuds() const 
{ 
  return m_noeuds.size(); 
}


size_type _BrasInterne::nb_polygones() const 
{ 
  return m_polygones.size(); 
}


size_type _BrasInterne::nb_polyedres() const 
{ 
  return m_polyedres.size(); 
}


_NoeudInterne* _BrasInterne::noeud(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_noeuds())
    throw range_erreur("Bras " + to_str(id()) + ", Indice local de noeud " +
		       to_str(n) + " non valide\n");
#endif
  return m_noeuds[n]; 
}


_PolygoneInterne* _BrasInterne::polygone(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_polygones())
    throw range_erreur("Bras " + to_str(id()) + 
		       ", Indice local de polygone " +
		       to_str(n) + " non valide\n");
#endif
  return m_polygones[n]; 
}


_PolyedreInterne* _BrasInterne::polyedre(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_polyedres())
    throw range_erreur("Bras " + to_str(id()) + 
		       ", Indice local de polyèdre " +
		       to_str(n) + " non valide\n");
#endif
  return m_polyedres[n]; 
}


void _BrasInterne::connecter(_PolygoneInterne* pg)
{
  m_polygones.push_back(pg);
}


void _BrasInterne::connecter(_PolyedreInterne* pd)
{
  m_polyedres.push_back(pd);
}


void _BrasInterne::invalider_polygones()
{
  m_polygones.resize(0);
}


void _BrasInterne::invalider_polyedres()
{
  m_polyedres.resize(0);
}


double _BrasInterne::longueur() const
{
  _Vecteur v(*m_noeuds[0], *m_noeuds[1]);
  return v.norme();
}


void _BrasInterne::retourner()
{
  IN_STD swap(m_noeuds[0], m_noeuds[1]);
}


void _BrasInterne::copier(const _BrasInterne* br)
{
  m_noeuds = br->m_noeuds;
}


_NoeudInterne* _BrasInterne::element_oppose(const _NoeudInterne* nd) const
{
  return element_oppose_(m_noeuds, nd);
}


_PolygoneInterne* _BrasInterne::element_oppose(const _PolygoneInterne* pg) const
{
  return element_oppose_(m_polygones, pg);
}

_NoeudInterne* _BrasInterne::frontiere(const _BrasInterne* b1, const _BrasInterne* b2)
{
  if(b1->noeud(0)==b2->noeud(0))
    return b1->noeud(0); 
  if(b1->noeud(0)==b2->noeud(1))
    return b1->noeud(0);
  if(b1->noeud(1)==b2->noeud(0))
    return b1->noeud(1);
  if(b1->noeud(1)==b2->noeud(1))
    return b1->noeud(1);
  return NULL;
}

bool _BrasInterne::comparer(const _BrasInterne* b1, const _BrasInterne* b2)
{
  if(b1->noeud(0)==b2->noeud(0)){
    if(b1->noeud(1)==b2->noeud(1)) return true;
    return false;
  }
  if(b1->noeud(1)==b2->noeud(0)){
    if(b1->noeud(0)==b2->noeud(1)) return true;
    return false;
  }
  return false;
}


void _BrasInterne::remplacer_noeud (_NoeudInterne* n1, _NoeudInterne* n2)
{
	const size_type	nombre	= m_noeuds.size ( );

	for (size_type i = 0; i < nombre; i++)
		if (m_noeuds [i] == n1)
			m_noeuds [i]	= n2;
}	// _BrasInterne::remplacer_noeud

END_NAMESPACE_LIMA

