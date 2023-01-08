#include <Lima/polyedre.h>
#include "LimaP/polyedre_it.h"
#include <Lima/noeud.h>
#include <Lima/bras.h>
#include <Lima/polygone.h>


BEGIN_NAMESPACE_LIMA

int Polyedre::PolyedreNbNode[maxPolyedreType] ={ 4,  // TETRAEDRE
                                                 5,  // PYRAMIDE
                                                 6,  // PRISME
                                                 8,  // HEXAEDRE
                                                 10, // HEPTAEDRE
                                                 12  // OCTOEDRE
                                 };

Polyedre::Polyedre(const Noeud& n0,  const Noeud& n1,
		   const Noeud& n2,  const Noeud& n3):
  _Handle<_PolyedreInterne>(_PolyedreInterne::create(n0, n1, n2, n3))
{
} 

Polyedre::Polyedre(const Noeud& n0,  const Noeud& n1, const Noeud& n2,
		   const Noeud& n3,  const Noeud& n4):
  _Handle<_PolyedreInterne>(_PolyedreInterne::create(n0, n1, n2, n3, n4))
{
} 
  
Polyedre::Polyedre(const Noeud& n0,  const Noeud& n1,
		   const Noeud& n2,  const Noeud& n3,
		   const Noeud& n4,  const Noeud& n5):
  _Handle<_PolyedreInterne>(_PolyedreInterne::create(n0, n1, n2, n3, n4, n5))
{
} 
  
Polyedre::Polyedre(const Noeud& n0,  const Noeud& n1,
		   const Noeud& n2,  const Noeud& n3,
		   const Noeud& n4,  const Noeud& n5,
		   const Noeud& n6,  const Noeud& n7):
  _Handle<_PolyedreInterne>(_PolyedreInterne::create(n0, n1, n2, n3, 
						     n4, n5, n6, n7))
{
} 
  
Polyedre::Polyedre(const Noeud& n0,  const Noeud& n1,
		   const Noeud& n2,  const Noeud& n3,
		   const Noeud& n4,  const Noeud& n5,
		   const Noeud& n6,  const Noeud& n7,
		   const Noeud& n8,  const Noeud& n9):
  _Handle<_PolyedreInterne>(_PolyedreInterne::create(n0, n1, n2, n3, n4, 
						     n5, n6, n7, n8, n9))
{
} 
  
Polyedre::Polyedre(const Noeud& n0,  const Noeud& n1,
		   const Noeud& n2,  const Noeud& n3,
		   const Noeud& n4,  const Noeud& n5,
		   const Noeud& n6,  const Noeud& n7,
		   const Noeud& n8,  const Noeud& n9,
		   const Noeud& n10,  const Noeud& n11):
  _Handle<_PolyedreInterne>(_PolyedreInterne::create(n0, n1, n2, n3, n4, n5,
						     n6, n7, n8, n9, n10, n11))
{
} 
  
Polyedre::Polyedre(size_type n, const Noeud& n0,  const Noeud& n1,
		   const Noeud& n2,  const Noeud& n3):
  _Handle<_PolyedreInterne>(_PolyedreInterne::create(n, n0, n1, n2, n3))
{
} 

Polyedre::Polyedre(size_type n, const Noeud& n0,  const Noeud& n1, 
		   const Noeud& n2, const Noeud& n3,  const Noeud& n4):
  _Handle<_PolyedreInterne>(_PolyedreInterne::create(n, n0, n1, n2, n3, n4))
{
} 
  
Polyedre::Polyedre(size_type n, const Noeud& n0,  const Noeud& n1,
		   const Noeud& n2,  const Noeud& n3,
		   const Noeud& n4,  const Noeud& n5):
  _Handle<_PolyedreInterne>(_PolyedreInterne::create(n, n0, n1, n2, n3, n4, n5))
{
} 
  
Polyedre::Polyedre(size_type n, const Noeud& n0,  const Noeud& n1,
		   const Noeud& n2,  const Noeud& n3,
		   const Noeud& n4,  const Noeud& n5,
		   const Noeud& n6,  const Noeud& n7):
  _Handle<_PolyedreInterne>(_PolyedreInterne::create(n, n0, n1, n2, n3, 
						     n4, n5, n6, n7))
{
} 
  
Polyedre::Polyedre(size_type n, const Noeud& n0,  const Noeud& n1,
		   const Noeud& n2,  const Noeud& n3,
		   const Noeud& n4,  const Noeud& n5,
		   const Noeud& n6,  const Noeud& n7,
		   const Noeud& n8,  const Noeud& n9):
  _Handle<_PolyedreInterne>(_PolyedreInterne::create(n, n0, n1, n2, n3, n4,
						     n5, n6, n7, n8, n9))
{
} 
 
Polyedre::Polyedre(size_type n, const Noeud& n0,  const Noeud& n1,
		   const Noeud& n2,  const Noeud& n3,
		   const Noeud& n4,  const Noeud& n5,
		   const Noeud& n6,  const Noeud& n7,
		   const Noeud& n8,  const Noeud& n9,
		   const Noeud& n10,  const Noeud& n11):
  _Handle<_PolyedreInterne>(_PolyedreInterne::create(n, n0, n1, n2, n3, n4, n5,
						     n6, n7, n8, n9, n10, n11))
{
} 
 
Polyedre::Polyedre(_PolyedreInterne* ptr):
  _Handle<_PolyedreInterne>(ptr)
{
} 
  
Polyedre::Polyedre(const Polyedre& pd):
  _Handle<_PolyedreInterne>(pd)
{
}   
  
Polyedre::Polyedre():
  _Handle<_PolyedreInterne>(NULL)
{
} 

Polyedre::~Polyedre()
{
} 

// Numero d'identification
size_type Polyedre::id() const
{ 
  return m_ptr->id();
}
  
// Type de polyedre
Polyedre::PolyedreType Polyedre::type() const
{
  return m_ptr->type();
}

// Nombre de noeuds.
size_type Polyedre::nb_noeuds() const
{
  return m_ptr->nb_noeuds();
}
  
// Noeud numéro n.
Noeud Polyedre::noeud(size_type n) const
{
  return m_ptr->noeud(n);
}
  
// Nombre de bras.
size_type Polyedre::nb_bras() const
{
  return m_ptr->nb_bras();
}
  
// Bras numéro n.
Bras Polyedre::bras(size_type n) const
{
  return m_ptr->bras(n);
}
  
// Nombre de polygones.
size_type Polyedre::nb_polygones() const
{
  return m_ptr->nb_polygones();
}
  
// Polygone numéro n.
Polygone Polyedre::polygone(size_type n) const
{
  return m_ptr->polygone(n);
}
  
double Polyedre::volume() const
{
  return m_ptr->volume();
}

void Polyedre::remplacer_noeud (const Noeud& n1, const Noeud& n2)
{
	m_ptr->remplacer_noeud(n1, n2);
}

const Polyedre& Polyedre::operator = (const Polyedre& pd)
{
  _Handle<_PolyedreInterne>::operator = (pd);
  return *this;
}
  
bool Polyedre::operator == (const Polyedre& pd)
{
  return _Handle<_PolyedreInterne>::operator == (pd);
}

END_NAMESPACE_LIMA
