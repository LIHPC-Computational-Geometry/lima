#include <Lima/volume.h>
#include "LimaP/volume_it.h"
#include <Lima/noeud.h>
#include "LimaP/noeud_it.h"
#include <Lima/bras.h>
#include "LimaP/polygone_it.h"
#include <Lima/polygone.h>
#include "LimaP/bras_it.h"
#include <Lima/surface.h>
#include "LimaP/surface_it.h"
#include "LimaP/using_it.h"


BEGIN_NAMESPACE_LIMA

Volume::Volume(const string& nom):
  _Handle<_VolumeInterne>(new _VolumeInterne(nom))
{
}
  
Volume::Volume(_VolumeInterne* ptr):
  _Handle<_VolumeInterne>(ptr)
{
}
  
Volume::Volume(const Volume& vo):
  _Handle<_VolumeInterne>(vo)
{
}
  
Volume::~Volume()
{
}

Composition Volume::composition ( ) const
{
	return m_ptr->composition ( );
}	// Volume::composition
  
void Volume::ajouter(const Polyedre& pd)
{
  m_ptr->ajouter(pd);
}
  
void Volume::retirer(const Polyedre& pd)
{
  m_ptr->retirer(pd);
}

const string& Volume::nom() const
{
  return m_ptr->nom();
}
  
void Volume::nommer(const string& str)
{
  m_ptr->nommer(str);
}
  
/// Nombre de polyedre.
size_type Volume::nb_polyedres() const
{
  return m_ptr->nb_polyedres();
}

// Polyedre numéro n.
Polyedre Volume::polyedre(size_type n) const
{
  return m_ptr->polyedre(n);
}

// Polyedre d'ID n.
Polyedre Volume::polyedre_id(size_type id) const
{
  return m_ptr->polyedre_id(id);
}

bool Volume::contenir(const Polyedre& pd) const
{
  return m_ptr->contenir(pd);
}

// Liste des attributs des polyèdres.
Liste_attribut& Volume::att_polyedres()
{
  return m_ptr->att_polyedres();
}
 
/// Nombre de polygone.
size_type Volume::nb_polygones() const
{
  return m_ptr->nb_polygones();
}

// Polygone numéro n.
Polygone Volume::polygone(size_type n) const
{
  return m_ptr->polygone(n);
}

// Polygone d'ID n.
Polygone Volume::polygone_id(size_type id) const
{
  return m_ptr->polygone_id(id);
}

bool Volume::contenir(const Polygone& pg) const
{
  return m_ptr->contenir(pg);
}

// Liste des attributs des polygones.
Liste_attribut&    Volume::att_polygones()
{
  return m_ptr->att_polygones();
}
 
/// Nombre de bras.
size_type Volume::nb_bras() const
{
  return m_ptr->nb_bras();
}

// Bras numéro n.
Bras Volume::bras(size_type n) const
{
  return m_ptr->bras(n);
}

// Bras d'ID n.
Bras Volume::bras_id(size_type id) const
{
  return m_ptr->bras_id(id);
}

bool Volume::contenir(const Bras& br) const
{
  return m_ptr->contenir(br);
}

// Liste des attributs des bras.
Liste_attribut&    Volume::att_bras()
{
  return m_ptr->att_bras();
}

/// Nombre de noeuds.
size_type Volume::nb_noeuds() const
{
  return m_ptr->nb_noeuds();
}

// Noeud numéro n.
Noeud Volume::noeud(size_type n) const
{
  return m_ptr->noeud(n);
}

// Noeud d'ID n.
Noeud Volume::noeud_id(size_type id) const
{
  return m_ptr->noeud_id(id);
}

bool Volume::contenir(const Noeud& nd) const
{
  return m_ptr->contenir(nd);
}

// Liste des attributs des noeuds.
Liste_attribut&    Volume::att_noeuds()
{
  return m_ptr->att_noeuds();
}

const Volume& Volume::operator = (const Volume& vo)
{
  _Handle<_VolumeInterne>::operator = (vo);
  return *this;
}
  
bool Volume::operator == (const Volume& vo)
{
  return _Handle<_VolumeInterne>::operator == (vo);
}

Surface Volume::contour() const
{
  return m_ptr->contour();
}

void Volume::init_att()
{
  m_ptr->init_att();
}


END_NAMESPACE_LIMA

