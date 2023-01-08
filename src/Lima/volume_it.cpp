#include "LimaP/volume_it.h"
#include <Lima/surface.h>
#include "LimaP/surface_it.h"
#include "LimaP/fonction.h"
#include "LimaP/algorithme.h"
#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA
  
_VolumeInterne::~_VolumeInterne()
{
  if(m_contour){
    m_contour->ref_moins();
    if(m_contour->orphelin()) 
      delete m_contour;
  }
}



_VolumeInterne::_VolumeInterne(const IN_STD string& str):
  _Groupe(str), m_contour(0),
  m_liste_att_noeuds(),
  m_liste_att_bras(),
  m_liste_att_polygones(),
  m_liste_att_polyedres()
{
}


_SurfaceInterne* _VolumeInterne::contour() const
{
  return m_contour;
}


size_type _VolumeInterne::nb_noeuds() const
{
  return m_noeuds.nb_elements();
}


size_type _VolumeInterne::nb_bras() const
{
  return m_bras.nb_elements();
}


size_type _VolumeInterne::nb_polygones() const
{
  return m_polygones.nb_elements();
}


size_type _VolumeInterne::nb_polyedres() const
{
  return m_polyedres.nb_elements();
}


size_type _VolumeInterne::nb_elm() const
{
  return m_polyedres.nb_elements();
}


_NoeudInterne* _VolumeInterne::noeud(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_noeuds())
    throw range_erreur("Volume " + nom() + 
		       ", Indice local de noeud " +
		       to_str(n) + " non valide\n");
#endif
  return m_noeuds.element(n);
}


_BrasInterne* _VolumeInterne::bras(size_type n) const
{
 #ifdef RANGE_CHECKING
  if(n>=nb_bras())
    throw range_erreur("Volume " + nom() + 
		       ", Indice local de bras " +
		       to_str(n) + " non valide\n");
#endif
 return m_bras.element(n);
}


_PolygoneInterne* _VolumeInterne::polygone(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_polygones())
    throw range_erreur("Volume " + nom() + 
		       ", Indice local de polygone " +
		       to_str(n) + " non valide\n");
#endif
  return m_polygones.element(n);
}


_PolyedreInterne* _VolumeInterne::polyedre(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_polyedres())
    throw range_erreur("Volume " + nom() + 
		       ", Indice local de polyedre " +
		       to_str(n) + " non valide\n");
#endif
  return m_polyedres.element(n);
}


_PolyedreInterne* _VolumeInterne::elm(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_polyedres())
    throw range_erreur("Volume " + nom() + 
		       ", Indice local de polyedre " +
		       to_str(n) + " non valide\n");
#endif
  return m_polyedres.element(n);
}


_NoeudInterne* _VolumeInterne::noeud_id(size_type n) const
{
  return m_noeuds.element_id(n);
}


_BrasInterne* _VolumeInterne::bras_id(size_type n) const
{
  return m_bras.element_id(n);
}


_PolygoneInterne* _VolumeInterne::polygone_id(size_type n) const
{
  return m_polygones.element_id(n);
}


_PolyedreInterne* _VolumeInterne::polyedre_id(size_type n) const
{
  return m_polyedres.element_id(n);
}


void _VolumeInterne::reserver_noeuds(size_type n)
{
  m_noeuds.reserver(n);
}


void _VolumeInterne::reserver_bras(size_type n)
{
  m_bras.reserver(n);
}


void _VolumeInterne::reserver_polygones(size_type n)
{
  m_polygones.reserver(n);
}


void _VolumeInterne::reserver(size_type n)
{
  m_polyedres.reserver(n);
}
  

Liste_attribut&    _VolumeInterne::att_polyedres()
{
  return m_liste_att_polyedres;
}

/*! 
  Le polyèdre et le volume doivent au préalable avoir été attachés au
  même maillage (_ElementMaillage::attacher).
*/

void _VolumeInterne::ajouter(_PolyedreInterne* pd)
{
  if(maillage()!=pd->maillage())
    throw range_erreur("Volume " + nom() + ", et Polyedre " + to_str(pd->id()) + 
		       " n'appartiennent pas au meme maillage\n");    
  att_polyedres().augmenter();
  m_polyedres.ajouter(pd);
  m_composition.nb_elements++;
  m_composition.detail [pd->nb_noeuds ( )]++;
}


void _VolumeInterne::retirer(_PolyedreInterne* pd)
{
  att_polyedres().retirer(m_polyedres.indice(pd));
  m_polyedres.retirer(pd);
  m_composition.nb_elements--;
  m_composition.detail [pd->nb_noeuds ( )]--;
}
 


bool _VolumeInterne::contenir(_PolyedreInterne* pd) const
{
  return m_polyedres.contenir(pd);
}


bool _VolumeInterne::contenir(_PolygoneInterne* pg) const
{
  return m_polygones.contenir(pg);
}


bool _VolumeInterne::contenir(_BrasInterne* br) const
{
  return m_bras.contenir(br);
}


bool _VolumeInterne::contenir(_NoeudInterne* nd) const
{
  return m_noeuds.contenir(nd);
}


Liste_attribut&    _VolumeInterne::att_noeuds()
{
  return m_liste_att_noeuds;
}


Liste_attribut&    _VolumeInterne::att_bras()
{
  return m_liste_att_bras;
}


Liste_attribut&    _VolumeInterne::att_polygones()
{
  return m_liste_att_polygones;
}


void               _VolumeInterne::init_att_noeuds()
{
  att_noeuds().set_max_val(nb_noeuds());
}


void               _VolumeInterne::init_att_bras()
{
  att_bras().set_max_val(nb_bras());
}


void               _VolumeInterne::init_att_polygones()
{
  att_polygones().set_max_val(nb_polygones());
}


void               _VolumeInterne::init_att_polyedres()
{
  att_polyedres().set_max_val(nb_polyedres());
}


void               _VolumeInterne::init_att()
{
  init_att_noeuds();
  init_att_bras();
  init_att_polygones();
  init_att_polyedres();
}



void _VolumeInterne::extraire_noeuds()
{
	if (0 == nb_polyedres ( ))	// CP version 5.8.0
		return;

  extraire_elements(premier_polyedre(), dernier_polyedre(),
		    _les_noeuds<_PolyedreInterne>(), m_noeuds);
}

void _VolumeInterne::extraire_bras()
{
	if (0 == nb_polyedres ( ))	// CP version 5.8.0
		return;

  extraire_elements(premier_polyedre(), dernier_polyedre(),
		    _les_bras<_PolyedreInterne>(), m_bras);
}

void _VolumeInterne::extraire_polygones()
{
	if (0 == nb_polyedres ( ))	// CP version 5.8.0
		return;

  extraire_elements(premier_polyedre(), dernier_polyedre(),
		    _les_polygones<_PolyedreInterne>(), m_polygones);
}

void _VolumeInterne::extraire_contour()
{
  if(m_contour && m_contour->orphelin()) delete m_contour;
  m_contour = new _SurfaceInterne("CONTOUR_"+nom());
  m_contour->attacher(maillage());
  m_contour->ref_plus();
  
	if (0 == nb_polyedres ( ))	// CP version 5.8.0
		return;
  _extraire_contour(premier_polyedre(), dernier_polyedre(),
		    _les_polygones<_PolyedreInterne>(), m_contour);
}

END_NAMESPACE_LIMA
