#include <Lima/noeud.h>
#include "LimaP/noeud_it.h"
#include <Lima/bras.h>
#include "LimaP/bras_it.h"
#include <Lima/polygone.h>
#include "LimaP/polygone_it.h"
#include <Lima/polyedre.h>
#include "LimaP/polyedre_it.h"

BEGIN_NAMESPACE_LIMA

Noeud::Noeud(double x, double y, double z):
  _Handle<_NoeudInterne>(_NoeudInterne::create(x, y, z))
{
} 

Noeud::Noeud(size_type n, double x, double y, double z):
  _Handle<_NoeudInterne>(_NoeudInterne::create(n, x, y, z))
{
} 

Noeud::Noeud(_NoeudInterne* ptr):
  _Handle<_NoeudInterne>(ptr)
{
} 

Noeud::Noeud(const Noeud& nd):
  _Handle<_NoeudInterne>(nd)
{
}

Noeud::~Noeud()
{

} 

// Coordonnée X du noeud.
double Noeud::x() const
{
  return m_ptr->x();
}

// Coordonnée Y du noeud. 
double Noeud::y() const
{
  return m_ptr->y();
}

// Coordonnée Z du noeud. 
double Noeud::z() const
{
  return m_ptr->z();
}
  
// Coordonnée X du noeud. 
void Noeud::set_x(double d)
{
  m_ptr->set_x(d);
}
  
// Coordonnée Y du noeud. 
void Noeud::set_y(double d)
{
  m_ptr->set_y(d);
}

// Coordonnée Z du noeud. 
void Noeud::set_z(double d)
{
  m_ptr->set_z(d);
}
  
// Numero d'identification
size_type Noeud::id() const
{
  return m_ptr->id();
}
  
// Nombre de bras.
size_type Noeud::nb_bras() const
{
  return m_ptr->nb_bras();
}

// Bras numéro n.
Bras Noeud::bras(size_type n) const
{
  return m_ptr->bras(n);
}
  
// Nombre de polygones.
size_type Noeud::nb_polygones() const
{
  return m_ptr->nb_polygones();
}
  
// Polygone numéro n.
Polygone Noeud::polygone(size_type n) const
{
  return m_ptr->polygone(n);
}
  
// Nombre de polyedres.
size_type Noeud::nb_polyedres() const
{
  return m_ptr->nb_polyedres();
}

// Polyedre numéro n.
Polyedre Noeud::polyedre(size_type n) const
{
  return m_ptr->polyedre(n);
}

const Noeud& Noeud::operator = (const Noeud& nd)
{
  _Handle<_NoeudInterne>::operator = (nd);
  return *this;
}
  
bool Noeud::operator == (const Noeud& nd)
{
  return _Handle<_NoeudInterne>::operator == (nd);
}

END_NAMESPACE_LIMA
