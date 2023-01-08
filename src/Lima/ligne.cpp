#include <Lima/ligne.h>
#include "LimaP/ligne_it.h"
#include <Lima/noeud.h>
#include "LimaP/noeud_it.h"
#include <Lima/bras.h>
#include "LimaP/bras_it.h"
#include <Lima/nuage.h>
#include "LimaP/nuage_it.h"
#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA

Ligne::Ligne(const string& nom):
  _Handle<_LigneInterne>(new _LigneInterne(nom))
{
}
  
Ligne::Ligne(_LigneInterne* ptr):
  _Handle<_LigneInterne>(ptr)
{
}
  
Ligne::Ligne(const Ligne& li):
  _Handle<_LigneInterne>(li)
{
}
  
Ligne::~Ligne()
{
}

Composition Ligne::composition ( ) const
{
	return m_ptr->composition ( );
}	// Ligne::composition
  
void Ligne::ajouter(const Bras& br)
{
  m_ptr->ajouter(br);
}
  
void Ligne::retirer(const Bras& br)
{
  m_ptr->retirer(br);
}

const string& Ligne::nom() const
{
  return m_ptr->nom();
}
  
void Ligne::nommer(const string& str)
{
  m_ptr->nommer(str);
}
  
/// Nombre de bras.
size_type Ligne::nb_bras() const
{
  return m_ptr->nb_bras();
}

// Bras numéro n.
Bras Ligne::bras(size_type n) const
{
  return m_ptr->bras(n);
}

// Bras d'ID n.
Bras Ligne::bras_id(size_type id) const
{
  return m_ptr->bras_id(id);
}

bool Ligne::contenir(const Bras& br) const
{
  return m_ptr->contenir(br);
}

// Liste des attributs des bras.
Liste_attribut&    Ligne::att_bras()
{
  return m_ptr->att_bras();
}

/// Nombre de noeuds.
size_type Ligne::nb_noeuds() const
{
  return m_ptr->nb_noeuds();
}

// Noeud numéro n.
Noeud Ligne::noeud(size_type n) const
{
  return m_ptr->noeud(n);
}

// Noeud d'ID n.
Noeud Ligne::noeud_id(size_type id) const
{
  return m_ptr->noeud_id(id);
}

bool Ligne::contenir(const Noeud& nd) const
{
  return m_ptr->contenir(nd);
}

// Liste des attributs des noeuds.
Liste_attribut&    Ligne::att_noeuds()
{
  return m_ptr->att_noeuds();
}

Nuage Ligne::contour() const
{
  return m_ptr->contour();
}

size_type Ligne::longueur_i() const
{
  return 0;
  //  return m_ptr->longueur_i();
}
  
const Ligne& Ligne::operator = (const Ligne& li)
{
  _Handle<_LigneInterne>::operator = (li);
  return *this;
}
  
bool Ligne::operator == (const Ligne& li)
{
  return _Handle<_LigneInterne>::operator == (li);
}

void Ligne::init_att()
{
  m_ptr->init_att();
}

END_NAMESPACE_LIMA

