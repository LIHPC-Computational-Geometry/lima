#include "LimaP/noeud_it.h"
#include "LimaP/algorithme.h"

BEGIN_NAMESPACE_LIMA

#ifndef LIMA_MT
size_type _NoeudInterne::m_max_id = 0;
#endif	// LIMA_MT

 
_NoeudInterne::_NoeudInterne (double x, double y, double z)
#ifndef LIMA_MT
	: _ElementFini (++m_max_id), _Point (x, y, z)
#else
	: _ElementFini (0), _Point(x, y, z)
#endif	// LIMA_MT
{
}	// _NoeudInterne::_NoeudInterne

 
_NoeudInterne::_NoeudInterne (size_type n, double x, double y, double z)
	: _ElementFini (n), _Point (x, y, z)
{
#ifndef LIMA_MT
  m_max_id = (m_max_id<n)? n:m_max_id;
#endif	// LIMA_MT
}	// _NoeudInterne::_NoeudInterne


void* _NoeudInterne::operator new(size_t)
{
  return ClassAllocateur<_NoeudInterne>::allouer();
}
  

void _NoeudInterne::operator delete(void* ptr)
{
  ClassAllocateur<_NoeudInterne>::liberer(ptr);    
}

 _NoeudInterne* 
_NoeudInterne::create(double x, double y, double z)
{
  return new _NoeudInterne(x, y, z);
}

 _NoeudInterne* 
_NoeudInterne::create(size_type n, double x, double y, double z)
{
  return new _NoeudInterne(n, x, y, z);
}


size_type _NoeudInterne::nb_noeuds() const 
{ 
  return 1; 
}


size_type _NoeudInterne::nb_bras() const 
{ 
  return m_bras.size(); 
}


size_type _NoeudInterne::nb_polygones() const 
{ 
  return m_polygones.size(); 
}


size_type _NoeudInterne::nb_polyedres() const 
{ 
  return m_polyedres.size(); 
}


_BrasInterne* _NoeudInterne::bras(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_bras())
    throw range_erreur("Noeud " + to_str(id()) + ", Indice local de bras " +
		       to_str(n) + " non valide\n");
#endif
  return m_bras[n]; 
}


_PolygoneInterne* _NoeudInterne::polygone(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_polygones())
    throw range_erreur("Noeud " + to_str(id()) + 
		       ", Indice local de polygone " +
		       to_str(n) + " non valide\n");
#endif
  return m_polygones[n]; 
}


_PolyedreInterne* _NoeudInterne::polyedre(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_polyedres())
    throw range_erreur("Noeud " + to_str(id()) + 
		       ", Indice local de polyèdre " +
		       to_str(n) + " non valide\n");
#endif
  return m_polyedres[n]; 
}


void _NoeudInterne::connecter(_BrasInterne* br)
{
  m_bras.push_back(br);
}


void _NoeudInterne::connecter(_PolygoneInterne* pg)
{
  m_polygones.push_back(pg);
}


void _NoeudInterne::connecter(_PolyedreInterne* pd)
{
  m_polyedres.push_back(pd);
}


void _NoeudInterne::invalider_bras()
{
  m_bras.resize(0);
}


void _NoeudInterne::invalider_polygones()
{
  m_polygones.resize(0);
}


void _NoeudInterne::invalider_polyedres()
{
  m_polyedres.resize(0);
}


_BrasInterne* _NoeudInterne::element_oppose(const _BrasInterne* br) const
{
  return element_oppose_(m_bras, br);
}

END_NAMESPACE_LIMA

