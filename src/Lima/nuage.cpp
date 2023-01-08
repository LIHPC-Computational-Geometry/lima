#include <Lima/nuage.h>
#include "LimaP/nuage_it.h"
#include <Lima/noeud.h>
#include "LimaP/noeud_it.h"
#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA

Nuage::Nuage(const string& nom):
  _Handle<_NuageInterne>(new _NuageInterne(nom))
{
}
  
Nuage::Nuage(_NuageInterne* ptr):
  _Handle<_NuageInterne>(ptr)
{
}
  
Nuage::Nuage(const Nuage& nu):
  _Handle<_NuageInterne>(nu)
{
}


Composition Nuage::composition ( ) const
{
	return m_ptr->composition ( );
}	// Nuage::composition

  
Nuage::~Nuage()
{
}
  
void Nuage::ajouter(const Noeud& nd)
{
  m_ptr->ajouter(nd);
}
  
void Nuage::retirer(const Noeud& nd)
{
  m_ptr->retirer(nd);
}

const string& Nuage::nom() const
{
  return m_ptr->nom();
}
  
void Nuage::nommer(const string& str)
{
  m_ptr->nommer(str);
}
  
/// Nombre de noeuds.
size_type Nuage::nb_noeuds() const
{
  return m_ptr->nb_noeuds();
}

// Noeud numéro n.
Noeud Nuage::noeud(size_type n) const
{
  return m_ptr->noeud(n);
}

// Noeud d'ID n.
Noeud Nuage::noeud_id(size_type id) const
{
  return m_ptr->noeud_id(id);
}

bool Nuage::contenir(const Noeud& nd) const
{
  return m_ptr->contenir(nd);
}

// Liste des attributs des noeuds.
Liste_attribut&    Nuage::att_noeuds()
{
  return m_ptr->att_noeuds();
}

const Nuage& Nuage::operator = (const Nuage& nd)
{
  _Handle<_NuageInterne>::operator = (nd);
  return *this;
}
  
bool Nuage::operator == (const Nuage& nd)
{
  return _Handle<_NuageInterne>::operator == (nd);
}

void Nuage::init_att()
{
  m_ptr->init_att();
}

END_NAMESPACE_LIMA
