#include <Lima/surface.h>
#include "LimaP/surface_it.h"
#include <Lima/noeud.h>
#include "LimaP/noeud_it.h"
#include <Lima/bras.h>
#include "LimaP/bras_it.h"
#include <Lima/polygone.h>
#include "LimaP/polygone_it.h"
#include <Lima/ligne.h>
#include "LimaP/ligne_it.h"
#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA

Surface::Surface(const string& nom):
  _Handle<_SurfaceInterne>(new _SurfaceInterne(nom))
{
}
  
Surface::Surface(_SurfaceInterne* ptr):
  _Handle<_SurfaceInterne>(ptr)
{
}
  
Surface::Surface(const Surface& su):
  _Handle<_SurfaceInterne>(su)
{
}
  
Surface::~Surface()
{
}

Composition Surface::composition ( ) const
{
	return m_ptr->composition ( );
}	// Surface::composition
  
void Surface::ajouter(const Polygone& pg)
{
  m_ptr->ajouter(pg);
}
  
void Surface::retirer(const Polygone& pg)
{
  m_ptr->retirer(pg);
}

const string& Surface::nom() const
{
  return m_ptr->nom();
}
  
void Surface::nommer(const string& str)
{
  m_ptr->nommer(str);
}
  
/// Nombre de polygone.
size_type Surface::nb_polygones() const
{
  return m_ptr->nb_polygones();
}

// Polygone numéro n.
Polygone Surface::polygone(size_type n) const
{
  return m_ptr->polygone(n);
}

// Polygone d'ID n.
Polygone Surface::polygone_id(size_type id) const
{
  return m_ptr->polygone_id(id);
}

bool Surface::contenir(const Polygone& pg) const
{
  return m_ptr->contenir(pg);
}

// Liste des attributs des polygones.
Liste_attribut&    Surface::att_polygones()
{
  return m_ptr->att_polygones();
}
 
/// Nombre de bras.
size_type Surface::nb_bras() const
{
  return m_ptr->nb_bras();
}

// Bras numéro n.
Bras Surface::bras(size_type n) const
{
  return m_ptr->bras(n);
}

// Bras d'ID n.
Bras Surface::bras_id(size_type id) const
{
  return m_ptr->bras_id(id);
}

bool Surface::contenir(const Bras& br) const
{
  return m_ptr->contenir(br);
}

// Liste des attributs des bras.
Liste_attribut&    Surface::att_bras()
{
  return m_ptr->att_bras();
}

/// Nombre de noeuds.
size_type Surface::nb_noeuds() const
{
  return m_ptr->nb_noeuds();
}

// Noeud numéro n.
Noeud Surface::noeud(size_type n) const
{
  return m_ptr->noeud(n);
}

// Noeud d'ID n.
Noeud Surface::noeud_id(size_type id) const
{
  return m_ptr->noeud_id(id);
}

bool Surface::contenir(const Noeud& nd) const
{
  return m_ptr->contenir(nd);
}

// Liste des attributs des noeuds.
Liste_attribut&    Surface::att_noeuds()
{
  return m_ptr->att_noeuds();
}

const Surface& Surface::operator = (const Surface& su)
{
  _Handle<_SurfaceInterne>::operator = (su);
  return *this;
}
  
bool Surface::operator == (const Surface& su)
{
  return _Handle<_SurfaceInterne>::operator == (su);
}
  
Ligne Surface::contour() const
{
  return m_ptr->contour();
}

size_type Surface::longueur_i() const
{
  return 0;
  //  return m_ptr->longueur_i();
}

size_type Surface::longueur_j() const
{
  return 0;
  //  return m_ptr->longueur_j();
}

void Surface::init_att()
{
  m_ptr->init_att();
}


END_NAMESPACE_LIMA
