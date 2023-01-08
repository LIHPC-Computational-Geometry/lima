#include <Lima/polygone.h>
#include "LimaP/polygone_it.h"
#include <Lima/noeud.h>
#include <Lima/bras.h>
#include <Lima/polyedre.h>


BEGIN_NAMESPACE_LIMA

Polygone::Polygone(const Noeud& n0,  const Noeud& n1, const Noeud& n2):
  _Handle<_PolygoneInterne>(_PolygoneInterne::create(n0, n1, n2))
{
} 

Polygone::Polygone(const Noeud& n0,  const Noeud& n1, 
		   const Noeud& n2, const Noeud& n3):
  _Handle<_PolygoneInterne>(_PolygoneInterne::create(n0, n1, n2, n3))
{
} 

Polygone::Polygone(const Noeud& n0,  const Noeud& n1, const Noeud& n2,
		   const Noeud& n3, const Noeud& n4):
  _Handle<_PolygoneInterne>(_PolygoneInterne::create(n0, n1, n2, n3, n4))
{
} 

Polygone::Polygone(const Noeud& n0, const Noeud& n1, const Noeud& n2,
		   const Noeud& n3, const Noeud& n4, const Noeud& n5):
  _Handle<_PolygoneInterne>(_PolygoneInterne::create(n0, n1, n2, n3, n4, n5))
{
} 

Polygone::Polygone(size_type n, const Noeud& n0,  
		   const Noeud& n1, const Noeud& n2):
  _Handle<_PolygoneInterne>(_PolygoneInterne::create(n, n0, n1, n2))
{
} 

Polygone::Polygone(size_type n, const Noeud& n0,  const Noeud& n1, 
		   const Noeud& n2, const Noeud& n3):
  _Handle<_PolygoneInterne>(_PolygoneInterne::create(n, n0, n1, n2, n3))
{
} 

Polygone::Polygone(size_type n, const Noeud& n0,  const Noeud& n1, 
		   const Noeud& n2, const Noeud& n3, const Noeud& n4):
  _Handle<_PolygoneInterne>(_PolygoneInterne::create(n, n0, n1, n2, n3, n4))
{
} 

Polygone::Polygone(size_type n, const Noeud& n0,  const Noeud& n1, 
		   const Noeud& n2, const Noeud& n3, const Noeud& n4,
		   const Noeud& n5):
  _Handle<_PolygoneInterne>(_PolygoneInterne::create(n, n0, n1, n2, n3, n4, n5))
{
} 

Polygone::Polygone(_PolygoneInterne* ptr):
  _Handle<_PolygoneInterne>(ptr)
{
}
  
Polygone::Polygone(const Polygone& pg):
  _Handle<_PolygoneInterne>(pg)
{
}  

Polygone::Polygone():
  _Handle<_PolygoneInterne>(NULL)
{
} 

Polygone::~Polygone()
{
} 

// Numero d'identification
size_type Polygone::id() const
{
  return m_ptr->id();
}

// Nombre de noeuds.
size_type Polygone::nb_noeuds() const
{
  return m_ptr->nb_noeuds();
}
  
// Noeud numéro n.
Noeud Polygone::noeud(size_type n) const
{
  return m_ptr->noeud(n);
}
  
// Nombre de bras.
size_type Polygone::nb_bras() const
{
  return m_ptr->nb_bras();
}
  
// Bras numéro n.
Bras Polygone::bras(size_type n) const
{
  return m_ptr->bras(n);
}
  
// Nombre de polyedres.
size_type Polygone::nb_polyedres() const
{
  return m_ptr->nb_polyedres();
}
  
// Polyedre numéro n.
Polyedre Polygone::polyedre(size_type n) const
{
  return m_ptr->polyedre(n);
}

void Polygone::remplacer_noeud (const Noeud& n1, const Noeud& n2)
{
	m_ptr->remplacer_noeud(n1, n2);
}

const Polygone& Polygone::operator = (const Polygone& pg)
{
  _Handle<_PolygoneInterne>::operator = (pg);
  return *this;
}
  
bool Polygone::operator == (const Polygone& pg)
{
  return _Handle<_PolygoneInterne>::operator == (pg);
}

END_NAMESPACE_LIMA
