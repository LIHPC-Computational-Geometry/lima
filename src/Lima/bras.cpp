#include <Lima/bras.h>
#include "LimaP/bras_it.h"
#include <Lima/noeud.h>
#include <Lima/polygone.h>
#include <Lima/polyedre.h>


BEGIN_NAMESPACE_LIMA

Bras::Bras(const Noeud& n0,  const Noeud& n1):
  _Handle<_BrasInterne>(_BrasInterne::create(n0, n1))
{
} 

Bras::Bras(size_type n, const Noeud& n0,  const Noeud& n1):
  _Handle<_BrasInterne>(_BrasInterne::create(n, n0, n1))
{
} 

Bras::Bras(_BrasInterne* ptr):
  _Handle<_BrasInterne>(ptr)
{
}

Bras::Bras(const Bras& br):
  _Handle<_BrasInterne>(br)
{
}

Bras::Bras():
  _Handle<_BrasInterne>(NULL)
{
} 

Bras::~Bras()
{
} 

// Numero d'identification
size_type Bras::id() const
{
  return m_ptr->id();
}

// Nombre de noeuds.
size_type Bras::nb_noeuds() const
{
  return m_ptr->nb_noeuds();
}

// Noeud numéro n.
Noeud Bras::noeud(size_type n) const
{
  return m_ptr->noeud(n);
}
  
// Nombre de polygones.
size_type  Bras::nb_polygones() const
{
  return m_ptr->nb_polygones();
}

// Polygone numéro n.
Polygone Bras::polygone(size_type n) const
{
  return m_ptr->polygone(n);
}

// Nombre de polyedres.
size_type Bras::nb_polyedres() const
{
  return m_ptr->nb_polyedres();
}

// Polyedre numéro n.
Polyedre Bras::polyedre(size_type n) const
{
  return m_ptr->polyedre(n);
}

void Bras::remplacer_noeud (const Noeud& n1, const Noeud& n2)
{
	m_ptr->remplacer_noeud(n1, n2);
}

const Bras& Bras::operator = (const Bras& br)
{
  _Handle<_BrasInterne>::operator = (br);
    return *this;
}

bool Bras::operator == (const Bras& br)
{
    return _Handle<_BrasInterne>::operator == (br);
}

END_NAMESPACE_LIMA
