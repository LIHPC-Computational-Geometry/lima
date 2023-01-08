#include <Lima/maillage.h>
#include "LimaP/maillage_it.h"
#include <Lima/noeud.h>
#include "LimaP/noeud_it.h"
#include <Lima/bras.h>
#include "LimaP/bras_it.h"
#include <Lima/polygone.h>
#include "LimaP/polygone_it.h"
#include <Lima/polyedre.h>
#include "LimaP/polyedre_it.h"
#include <Lima/nuage.h>
#include "LimaP/nuage_it.h"
#include <Lima/ligne.h>
#include "LimaP/ligne_it.h"
#include <Lima/surface.h>
#include "LimaP/surface_it.h"
#include <Lima/volume.h>
#include "LimaP/volume_it.h"
#include "LimaP/handle_t.h"
#include "LimaP/using_it.h"


BEGIN_NAMESPACE_LIMA

#if defined(__GNUC__) || defined(__IBMCPP__) || defined(__ICC)
// __INTEL_COMPILER : icpc predefini egalement __GNUC__ ...
template class _Handle<_MaillageInterne>;
template class _Handle<_NoeudInterne>;
template class _Handle<_BrasInterne>;
template class _Handle<_PolygoneInterne>;
template class _Handle<_PolyedreInterne>;
template class _Handle<_NuageInterne>;
template class _Handle<_LigneInterne>;
template class _Handle<_SurfaceInterne>;
template class _Handle<_VolumeInterne>;
#endif	// #if defined(__GNUC__) ...


extern _MaillageInterne* liberer_maillage (int_type);


Maillage maillage_C_vers_CPP (int_type ref)
{
	return Maillage (liberer_maillage (ref));
}	// maillage_C_vers_CPP


// Maillage vide.
Maillage::Maillage(const string& str):
  _Handle<_MaillageInterne>(new _MaillageInterne(str))
{
}

Maillage::Maillage (_MaillageInterne* maillage)
	: _Handle<_MaillageInterne> (maillage)
{
}

// Destructeur
Maillage::~Maillage()
{
}

Maillage::Maillage(const Maillage& mai):
  _Handle<_MaillageInterne>(mai)
{
}

const Maillage& Maillage::operator = (const Maillage& mai)
{
  _Handle<_MaillageInterne>::operator = (mai);
  return *this;
}

Composition Maillage::composition_noeuds ( ) const
{
	return m_ptr->composition_noeuds ( );
}	// Maillage::composition_noeuds

Composition Maillage::composition_bras ( ) const
{
	return m_ptr->composition_bras ( );
}	// Maillage::composition_bras

Composition Maillage::composition_polygones ( ) const
{
	return m_ptr->composition_polygones ( );
}	// Maillage::composition_polygones

Composition Maillage::composition_polyedres ( ) const
{
	return m_ptr->composition_polyedres ( );
}	// Maillage::composition_polyedres

void Maillage::vider()
{
  m_ptr->vider();
}

// Dimension du maillage, 1D, 2D ou 3D.
dim_t Maillage::dimension() const
{
  return m_ptr->dimension();
}

/* Coefficient multiplicatif à appliquer sur les coordonnées
   *  pour obtenir des métres */
double Maillage::unite_longueur() const
{
  return m_ptr->unite_longueur();
}

/* Coefficient multiplicatif à appliquer sur les angles
   *  pour obtenir des radians */
double Maillage::unite_angle() const
{
  return m_ptr->unite_angle();
}

// Retourne le type de coordonnées.
coordonnee_t Maillage::type_coordonnee() const
{
  return m_ptr->type_coordonnee();
}

// Retourne le type de géométrie.
geometrie_t Maillage::type_geometrie() const
{
  return m_ptr->type_geometrie();
}

// Retourne le titre du maillage.
const string& Maillage::titre() const
{
  return m_ptr->titre();
}

// Retourne la date de création du maillage.
const string& Maillage::date() const
{
  return m_ptr->date();
}

// Dimension du maillage.
void Maillage::dimension(dim_t dim)
{
  m_ptr->dimension(dim);
}

// Unité de longueur du maillage.
void Maillage::unite_longueur(double unilo)
{
  m_ptr->unite_longueur(unilo);
}

// Unité d'angle du maillage.
void Maillage::unite_angle(double unia)
{
  m_ptr->unite_angle(unia);
}

// Type de géométrie du maillage.
void Maillage::type_geometrie(geometrie_t geom)
{
  m_ptr->type_geometrie(geom);
}

// Type de coordonnée du maillage.
void Maillage::type_coordonnee(coordonnee_t tycoo)
{
  m_ptr->type_coordonnee(tycoo);
}

// Titre du maillage.
void Maillage::titre(const string& str)
{
  m_ptr->titre(str);
}

// Date du maillage.
void Maillage::date(const string& str)
{
  m_ptr->date(str);
}

// Liste des attributs du maillage.
Liste_attribut&    Maillage::att_maillage()
{
  return m_ptr->att_maillage();
}

/// Nombre de noeuds.
size_type Maillage::nb_noeuds() const
{
  return m_ptr->nb_noeuds();
}

// Noeud numéro n.
Noeud Maillage::noeud(size_type n) const
{
  return m_ptr->noeud(n);
}

// Noeud d'ID n.
Noeud Maillage::noeud_id(size_type id) const
{
  return m_ptr->noeud_id(id);
}

bool Maillage::contenir(const Noeud& nd) const
{
  return m_ptr->contenir(nd);
}

void Maillage::ajouter(const Noeud& nd)
{
  m_ptr->ajouter(nd);
}

void Maillage::retirer(const Noeud& nd)
{
  m_ptr->retirer(nd);
}

void Maillage::deplacer (size_type id, double x, double y, double z)
{
	m_ptr->deplacer (id, x, y, z);
}	// Maillage::deplacer

// Liste des attributs des noeuds.
Liste_attribut&    Maillage::att_noeuds()
{
  return m_ptr->att_noeuds();
}

/// Nombre de bras.
size_type Maillage::nb_bras() const
{
  return m_ptr->nb_bras();
}

// Bras numéro n.
Bras Maillage::bras(size_type n) const
{
  return m_ptr->bras(n);
}

// Bras d'ID n.
Bras Maillage::bras_id(size_type id) const
{
  return m_ptr->bras_id(id);
}

bool Maillage::contenir(const Bras& br) const
{
  return m_ptr->contenir(br);
}

void Maillage::ajouter(const Bras& br)
{
  m_ptr->ajouter(br);
}

void Maillage::retirer(const Bras& br)
{
  m_ptr->retirer(br);
}

// Liste des attributs des bras.
Liste_attribut&    Maillage::att_bras()
{
  return m_ptr->att_bras();
}

/// Nombre de polygone.
size_type Maillage::nb_polygones() const
{
  return m_ptr->nb_polygones();
}

// Polygone numéro n.
Polygone Maillage::polygone(size_type n) const
{
  return m_ptr->polygone(n);
}

// Polygone d'ID n.
Polygone Maillage::polygone_id(size_type id) const
{
  return m_ptr->polygone_id(id);
}

bool Maillage::contenir(const Polygone& pg) const
{
  return m_ptr->contenir(pg);
}

void Maillage::ajouter(const Polygone& pg)
{
  m_ptr->ajouter(pg);
}

void Maillage::retirer(const Polygone& pg)
{
  m_ptr->retirer(pg);
}

// Liste des attributs des polygones.
Liste_attribut&    Maillage::att_polygones()
{
  return m_ptr->att_polygones();
}

/// Nombre de polyedre.
size_type Maillage::nb_polyedres() const
{
  return m_ptr->nb_polyedres();
}

// Polyedre numéro n.
Polyedre Maillage::polyedre(size_type n) const
{
  return m_ptr->polyedre(n);
}

// Polyedre d'ID n.
Polyedre Maillage::polyedre_id(size_type id) const
{
  return m_ptr->polyedre_id(id);
}

bool Maillage::contenir(const Polyedre& pd) const
{
  return m_ptr->contenir(pd);
}

void Maillage::ajouter(const Polyedre& pd)
{
  m_ptr->ajouter(pd);
}

void Maillage::retirer(const Polyedre& pd)
{
  m_ptr->retirer(pd);
}

// Liste des attributs des polyèdres.
Liste_attribut&    Maillage::att_polyedres()
{
  return m_ptr->att_polyedres();
}

/// Nombre de nuage.
size_type Maillage::nb_nuages() const
{
  return m_ptr->nb_nuages();
}

// Nuage numéro n.
Nuage Maillage::nuage(size_type n) const
{
  return m_ptr->nuage(n);
}

// Nuage de nom str
Nuage Maillage::nuage(const string& str) const
{
  return m_ptr->nuage(str);
}

bool Maillage::contenir(const Nuage& nu) const
{
  return m_ptr->contenir(nu);
}

void Maillage::ajouter(const Nuage& nu)
{
  m_ptr->ajouter(nu);
}

void Maillage::retirer(const Nuage& nu)
{
  m_ptr->retirer(nu);
}

// Liste des attributs des nuages.
Liste_attribut&    Maillage::att_nuages()
{
  return m_ptr->att_nuages();
}

/// Nombre de ligne.
size_type Maillage::nb_lignes() const
{
  return m_ptr->nb_lignes();
}

// Ligne numéro n.
Ligne Maillage::ligne(size_type n) const
{
  return m_ptr->ligne(n);
}

// Ligne d'ID n.
Ligne Maillage::ligne(const string& str) const
{
  return m_ptr->ligne(str);
}

bool Maillage::contenir(const Ligne& li) const
{
  return m_ptr->contenir(li);
}

void Maillage::ajouter(const Ligne& li)
{
  m_ptr->ajouter(li);
}

void Maillage::retirer(const Ligne& li)
{
  m_ptr->retirer(li);
}

// Liste des attributs des lignes.
Liste_attribut&    Maillage::att_lignes()
{
  return m_ptr->att_lignes();
}

/// Nombre de surface.
size_type Maillage::nb_surfaces() const
{
  return m_ptr->nb_surfaces();
}

// Surface numéro n.
Surface Maillage::surface(size_type n) const
{
  return m_ptr->surface(n);
}

// Surface d'ID n.
Surface Maillage::surface(const string& str) const
{
  return m_ptr->surface(str);
}

bool Maillage::contenir(const Surface& su) const
{
  return m_ptr->contenir(su);
}

void Maillage::ajouter(const Surface& su)
{
  m_ptr->ajouter(su);
}

void Maillage::retirer(const Surface& su)
{
  m_ptr->retirer(su);
}

// Liste des attributs des surfaces.
Liste_attribut&    Maillage::att_surfaces()
{
  return m_ptr->att_surfaces();
}

/// Nombre de volume.
size_type Maillage::nb_volumes() const
{
  return m_ptr->nb_volumes();
}

// Volume numéro n.
Volume Maillage::volume(size_type n) const
{
  return m_ptr->volume(n);
}

// Volume d'ID n.
Volume Maillage::volume(const string& str) const
{
  return m_ptr->volume(str);
}

bool Maillage::contenir(const Volume& vo) const
{
  return m_ptr->contenir(vo);
}

void Maillage::ajouter(const Volume& vo)
{
  m_ptr->ajouter(vo);
}

void Maillage::retirer(const Volume& vo)
{
  m_ptr->retirer(vo);
}

// Liste des attributs des volumes.
Liste_attribut&    Maillage::att_volumes()
{
  return m_ptr->att_volumes();
}


void Maillage::lire_structure (
					const string& nom_fichier, format_t format, size_type num)
{
	m_ptr->lire_structure (nom_fichier, format, num);
}	// Maillage::lire_structure


void Maillage::lire(const string& nom_fichier, format_t format, size_type num)
{
  m_ptr->lire(nom_fichier, format, num);
}

void Maillage::ecrire(
	const string& nom_fichier, format_t format, size_type num, bool compacter)
{
  m_ptr->ecrire(nom_fichier, format, num, compacter);
}

void Maillage::preparer(preparation_t preparation)
{
  m_ptr->preparer(preparation);
}

void Maillage::preparation_parametrable(TypeMasque preparation)
{
  /* On passe le booleen TRUE pour initialiser le membre
   * m_etat_prepa de _MaillageInterne */
  m_ptr->preparation_parametrable(preparation, true);
}

void Maillage::precision_distance_noeuds (double precision)
{
	if (precision <= 0)
		throw range_erreur ("La précision dans le calcul de distance entre deux noeuds doit être strictement positive.");

	_Point::epsilon2	= precision * precision;
}	// Maillage::precision_distance_noeuds


void Maillage::verification_code (TypeVerificationCode type) const
{
	m_ptr->verification_code (type);
}	// Maillage::verification_code


void Maillage::verification_parametrable (TypeMasqueVerification masque) const
{
	m_ptr->verification_parametrable (masque);
}	// Maillage::verification_parametrable

void Maillage::init_att()
{
  m_ptr->init_att();
}


void Maillage::ajouter_reader_callback (ReaderCallback* rc)
{
	m_ptr->ajouter_reader_callback (rc);
}	// Maillage::ajouter_reader_callback


void Maillage::enlever_reader_callback (ReaderCallback* rc)
{
	m_ptr->enlever_reader_callback (rc);
}	// Maillage::enlever_reader_callback


END_NAMESPACE_LIMA
