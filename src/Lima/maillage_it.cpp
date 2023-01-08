#include <Lima/config.h>
#include "LimaP/maillage_it.h"
#include "LimaP/MutableString.h"
#include "LimaP/validateur.h"
#include "LimaP/fonction.h"
#include "LimaP/algorithme.h"
#include "LimaP/using_it.h"

#include "LimaP/myio.h"

#include "LimaP/reader.h"
#include "LimaP/writer.h"

#include <Lima/lima.h>

#include <assert.h>
#include <memory>

#ifdef DEBUG
#include <iostream>
#endif

const LM_TYPEMASQUE LM_MINIMALE                 = 1LL ;
const LM_TYPEMASQUE LM_MAXIMALE                 = 1LL<<1LL ;
const LM_TYPEMASQUE LM_CODE_B                   = 1LL<<2LL ;
const LM_TYPEMASQUE LM_CODE_A                   = 1LL<<3LL ;
const LM_TYPEMASQUE LM_MAILLEUR_2D              = 1LL<<4LL ;
const LM_TYPEMASQUE LM_BASE_3D                  = 1LL<<5LL ;
const LM_TYPEMASQUE LM_CODE_T                   = 1LL<<6LL ;
const LM_TYPEMASQUE LM_PARTITIONNEMENT_3D       = 1LL<<7LL ;
const LM_TYPEMASQUE LM_LISSAGE_3D               = 1LL<<8LL ;
const LM_TYPEMASQUE LM_NOEUDS_BRAS              = 1LL<<9LL ;
const LM_TYPEMASQUE LM_NOEUDS_POLYGONES         = 1LL<<10LL ;
const LM_TYPEMASQUE LM_NOEUDS_POLYEDRES         = 1LL<<11LL ;
const LM_TYPEMASQUE LM_POLYGONES_BRAS           = 1LL<<12LL ;
const LM_TYPEMASQUE LM_BRAS_POLYGONES           = 1LL<<13LL ;
const LM_TYPEMASQUE LM_POLYEDRES_BRAS           = 1LL<<14LL ;
const LM_TYPEMASQUE LM_BRAS_POLYEDRES           = 1LL<<15LL ;
const LM_TYPEMASQUE LM_POLYEDRES_POLYGONES      = 1LL<<16LL ;
const LM_TYPEMASQUE LM_POLYGONES_POLYEDRES      = 1LL<<17LL ;
const LM_TYPEMASQUE LM_SUPPRIMER_DOUBLONS_NOEUDS= 1LL<<49LL ;
const LM_TYPEMASQUE LM_SUPPRIMER_DOUBLONS_BRAS  = 1LL<<18LL ;
const LM_TYPEMASQUE LM_SUPPRIMER_DOUBLONS_POLYGONES = 1LL<<19LL ;
const LM_TYPEMASQUE LM_SUPPRIMER_DOUBLONS_POLYEDRES = 1LL<<20LL ;
const LM_TYPEMASQUE LM_LIGNES_NOEUDS            = 1LL<<21LL ;
const LM_TYPEMASQUE LM_SURFACES_NOEUDS          = 1LL<<22LL ;
const LM_TYPEMASQUE LM_SURFACES_BRAS            = 1LL<<23LL ;
const LM_TYPEMASQUE LM_VOLUMES_NOEUDS           = 1LL<<24LL ;
const LM_TYPEMASQUE LM_VOLUMES_BRAS             = 1LL<<25LL ;
const LM_TYPEMASQUE LM_VOLUMES_POLYGONES        = 1LL<<26LL ;
const LM_TYPEMASQUE LM_RAZ_NOEUDS_BRAS          = 1LL<<27LL ;
const LM_TYPEMASQUE LM_RAZ_NOEUDS_POLYGONES     = 1LL<<28LL ;
const LM_TYPEMASQUE LM_RAZ_NOEUDS_POLYEDRES     = 1LL<<29LL ;
const LM_TYPEMASQUE LM_RAZ_POLYGONES_BRAS       = 1LL<<30LL ;
const LM_TYPEMASQUE LM_RAZ_BRAS_POLYGONES       = 1LL<<31LL ;
const LM_TYPEMASQUE LM_RAZ_POLYEDRES_BRAS       = 1LL<<32LL ;
const LM_TYPEMASQUE LM_RAZ_BRAS_POLYEDRES       = 1LL<<33LL ;
const LM_TYPEMASQUE LM_RAZ_POLYEDRES_POLYGONES  = 1LL<<34LL ;
const LM_TYPEMASQUE LM_RAZ_POLYGONES_POLYEDRES  = 1LL<<35LL ;
const LM_TYPEMASQUE LM_CONTOURS_LIGNES          = 1LL<<36LL ;
const LM_TYPEMASQUE LM_CONTOURS_SURFACES        = 1LL<<37LL ;
const LM_TYPEMASQUE LM_CONTOURS_VOLUMES         = 1LL<<38LL ;
const LM_TYPEMASQUE LM_CONTOURS_SURF_NOE        = 1LL<<39LL ;
const LM_TYPEMASQUE LM_CONTOURS_VOL_NOE         = 1LL<<40LL ;
const LM_TYPEMASQUE LM_CONTOURS_VOL_BR          = 1LL<<41LL ;
const LM_TYPEMASQUE LM_STRUCTURER_LIGNES        = 1LL<<42LL ;
const LM_TYPEMASQUE LM_STRUCTURER_SURFACES      = 1LL<<43LL ;
const LM_TYPEMASQUE LM_STRUCT_CONTOURS_SURF     = 1LL<<44LL ;
const LM_TYPEMASQUE LM_COMPACTE                 = 1LL<<45LL ;
const LM_TYPEMASQUE LM_ORIENTATION              = 1LL<<46LL ;
const LM_TYPEMASQUE LM_ORIENT_CONTOURS_SURF     = 1LL<<47LL ;
const LM_TYPEMASQUE LM_FORMATER_NOMS            = 1LL<<48LL ;
const LM_TYPEMASQUE LM_SUPPRIMER_BRAS_INUTILISES   = 1LL<<49LL ;
const LM_TYPEMASQUE LM_SUPPRIMER_POLYGONES_INUTILISES = 1LL<<50LL ;

const LM_VC_TYPEMASQUE LM_VC_CODE_B                = 1LL;

const LM_V_TYPEMASQUE LM_V_BRAS                         = 1LL;
const LM_V_TYPEMASQUE LM_V_BRAS_NON_NULS                = 1LL<<1LL ;
const LM_V_TYPEMASQUE LM_V_ORIENTATION_BRAS             = 1LL<<2LL ;
const LM_V_TYPEMASQUE LM_V_CONNEXIONS_POLYGONES_BRAS    = 1LL<<3LL ;
const LM_V_TYPEMASQUE LM_V_STRUCTURER_LIGNES            = 1LL<<4LL ;
const LM_V_TYPEMASQUE LM_V_ORIENTATION_LIGNES_FERMEES   = 1LL<<5LL ;
const LM_V_TYPEMASQUE LM_V_ORIENTATION_POLYGONES        = 1LL<<6LL ;
const LM_V_TYPEMASQUE LM_V_CONTOURS_SURFACES            = 1LL<<7LL ;
const LM_V_TYPEMASQUE LM_V_STRUCT_CONTOURS_SURF         = 1LL<<8LL ;
const LM_V_TYPEMASQUE LM_V_ORIENT_CONTOURS_SURF         = 1LL<<9LL ;
const LM_V_TYPEMASQUE LM_V_COMPACTE                     = 1LL<<10LL ;


// Le masque de vérification des préparations CODE_B :
const LM_V_TYPEMASQUE	MASQUE_VERIFICATION_CODE_B	=
									LM_V_BRAS                           |
									LM_V_BRAS_NON_NULS                  |
									LM_V_ORIENTATION_BRAS               |
									LM_V_CONNEXIONS_POLYGONES_BRAS      |
									LM_V_STRUCTURER_LIGNES              |
									LM_V_ORIENTATION_LIGNES_FERMEES     |
									LM_V_ORIENTATION_POLYGONES          |
									LM_V_CONTOURS_SURFACES              |
									LM_V_STRUCT_CONTOURS_SURF           |
									LM_V_ORIENT_CONTOURS_SURF           |
									LM_V_COMPACTE;


BEGIN_NAMESPACE_LIMA

const char* lima_version()
{
	return LIMA_API_VERSION;
}

_MaillageInterne::_MaillageInterne(const string& str):
	m_composition_noeuds ( ), m_composition_bras ( ),
	m_composition_polygones ( ), m_composition_polyedres ( ),
	m_titre(str), m_contour3D(0),m_contour2D(0) , m_etat_prepa (0)
#ifdef LIMA_MT
	,
	m_noeuds_max_id (0), m_bras_max_id (0), m_polygones_max_id (0),
	m_polyedres_max_id (0)
#endif	// LIMA_MT
// CP version 5.4.3 : initialisation de m_etat_prepa a 0
{
  m_composition_noeuds.renseigne	= true;
  m_composition_bras.renseigne		= true;
  m_composition_polygones.renseigne	= true;
  m_composition_polyedres.renseigne	= true;
  m_dim = D3;
  init();
}

_MaillageInterne::~_MaillageInterne()
{
  if(m_contour3D && m_contour3D->orphelin())
    delete m_contour3D;
  if(m_contour2D && m_contour2D->orphelin())
    delete m_contour2D;

  vider();
}

void _MaillageInterne::init()
{
  m_struct = LIBRE;
  m_geom = NORMAL;
  m_tycoo = CARTESIEN;
  m_unilo = 1.;
  m_unia = 1.;
#ifdef LIMA_MT
	m_noeuds_max_id		= 0;
	m_bras_max_id		= 0;
	m_polygones_max_id	= 0;
	m_polyedres_max_id	= 0;
#endif	// LIMA_MT
}

void _MaillageInterne::vider()
{
  m_liste_att_noeuds.vider();
  for(iterator_noeud nd = premier_noeud(); nd != dernier_noeud(); ++nd){
    (*nd)->ref_moins();
    if((*nd)->orphelin()) delete (*nd);
  }
  m_noeuds.vider();

  m_liste_att_bras.vider();
  for(iterator_bras br = premier_bras(); br != dernier_bras(); ++br){
    (*br)->ref_moins();
    if((*br)->orphelin()) delete (*br);
  }
  m_bras.vider();

  m_liste_att_polygones.vider();
  for(iterator_polygone pg = premier_polygone();
      pg != dernier_polygone(); ++pg){
    (*pg)->ref_moins();
    if((*pg)->orphelin()) delete (*pg);
  }
  m_polygones.vider();

  m_liste_att_polyedres.vider();
  for(iterator_polyedre pd = premier_polyedre();
      pd != dernier_polyedre(); ++pd){
    (*pd)->ref_moins();
    if((*pd)->orphelin()) delete (*pd);
  }
  m_polyedres.vider();

  m_liste_att_nuages.vider();
  for(iterator_nuage nu = premier_nuage();
      nu != dernier_nuage(); ++nu){
    (*nu)->att_noeuds().vider();
    (*nu)->ref_moins();
    if((*nu)->orphelin()) delete (*nu);
  }
  m_nuages.vider();

  m_liste_att_lignes.vider();
  for(iterator_ligne li = premier_ligne();
      li != dernier_ligne(); ++li){
    (*li)->att_noeuds().vider();
    (*li)->att_bras().vider();
    (*li)->ref_moins();
    if((*li)->orphelin()) delete (*li);
  }
  m_lignes.vider();

  m_liste_att_surfaces.vider();
  for(iterator_surface su = premier_surface();
      su != dernier_surface(); ++su){
    (*su)->att_noeuds().vider();
    (*su)->att_bras().vider();
    (*su)->att_polygones().vider();
    (*su)->ref_moins();
    if((*su)->orphelin()) delete (*su);
  }
  m_surfaces.vider();

  m_liste_att_volumes.vider();
  for(iterator_volume vo = premier_volume();
      vo != dernier_volume(); ++vo){
    (*vo)->att_noeuds().vider();
    (*vo)->att_bras().vider();
    (*vo)->att_polygones().vider();
    (*vo)->att_polyedres().vider();
    (*vo)->ref_moins();
    if((*vo)->orphelin()) delete (*vo);
  }
  m_volumes.vider();

  init();

	Composition	composition;
	composition.renseigne	= true;
	m_composition_noeuds	= composition;
	m_composition_bras		= composition;
	m_composition_polygones	= composition;
	m_composition_polyedres	= composition;
}


// Dimension du maillage.
void _MaillageInterne::dimension(dim_t dim)
{
  if(dim<D1 || dim>D3)
    throw argument_erreur("Dimension incorrect");

  m_dim = dim;
}

// Unité de longueur du maillage.
void _MaillageInterne::unite_longueur(double unilo)
{
  if(unilo<=0)
    throw argument_erreur("unite longeur negative ou nulle");

  m_unilo = unilo;
}

// Unité d'angle du maillage.
void _MaillageInterne::unite_angle(double unia)
{
  if(unia<=0)
    throw argument_erreur("unite d'angle negative ou nulle");

  m_unia = unia;
}

// Type de géométrie du maillage.
void _MaillageInterne::type_geometrie(geometrie_t geom)
{
  if(geom<NORMAL || geom>PERIODIQUE)
    throw argument_erreur("Type geometrie incorrecte");

  m_geom = geom;
}

// Type de coordonnée du maillage.
void _MaillageInterne::type_coordonnee(coordonnee_t tycoo)
{
  if(tycoo<CARTESIEN || tycoo>CYLINDRIQUE)
    throw argument_erreur("type de coordonnees non valide");

  m_tycoo = tycoo;
}

// Titre du maillage.
void _MaillageInterne::titre(const string& titre)
{
  m_titre = titre;
  string::size_type pos = m_titre.find_last_not_of(' ');

#if defined(CRAY)
 if(pos !=  NPOS)
    m_titre.remove(pos+1);
#else
 if(pos !=  string::npos)
    m_titre.erase(pos+1);
#endif
}

// Date du maillage.
void _MaillageInterne::date(const string& date)
{
  m_date = date;
}

void _MaillageInterne::ajouter(_PolyedreInterne* pd)
{
#ifdef LIMA_MT
	if (0 == pd->id ( ))
		pd->set_id (++m_polyedres_max_id);
	else
		if (pd->id ( ) > m_polyedres_max_id)
			m_polyedres_max_id	= pd->id ( );
#endif	// LIMA_MT
  for(_PolyedreInterne::iterator_noeud n = pd->premier_noeud();
      n != pd->dernier_noeud(); ++n){
    if(!contenir(*n))
      throw range_erreur("Le polyedre " + to_str(pd->id()) + " reference "
			 "le noeud " + to_str((*n)->id()) +
			 " qui n'appartiennent pas au maillage " +
			 titre() + "\n");
  }
//  m_polyedres.ajouter(pd);	// v 6.6.0
	if (true == m_polyedres.ajouter (pd))	// v 6.6.0
	{
		att_polyedres().augmenter();
		pd->attacher(this);
		pd->ref_plus();
		m_composition_polyedres.nb_elements++;
		m_composition_polyedres.detail [pd->nb_noeuds ( )]++;
	}	// v 6.6.0
}

void _MaillageInterne::ajouter(_PolygoneInterne* pg)
{
#ifdef LIMA_MT
	if (0 == pg->id ( ))
		pg->set_id (++m_polygones_max_id);
	else
		if (pg->id ( ) > m_polygones_max_id)
			m_polygones_max_id	= pg->id ( );
#endif	// LIMA_MT
  for(_PolygoneInterne::iterator_noeud n = pg->premier_noeud();
      n != pg->dernier_noeud(); ++n){
    if(!contenir(*n))
      throw range_erreur("Le polygone " + to_str(pg->id()) + " reference "
			 "le noeud " + to_str((*n)->id()) +
			 " qui n'appartiennent pas au maillage " +
			 titre() + "\n");
  }
//  m_polygones.ajouter(pg);	// v 6.6.0
	if (true == m_polygones.ajouter (pg))	// v 6.6.0
	{
		att_polygones().augmenter();
		pg->attacher(this);
		pg->ref_plus();
		m_composition_polygones.nb_elements++;
		m_composition_polygones.detail [pg->nb_noeuds ( )]++;
	}	// v 6.6.0
}

void _MaillageInterne::ajouter(_BrasInterne* br)
{
#ifdef LIMA_MT
	if (0 == br->id ( ))
		br->set_id (++m_bras_max_id);
	else
		if (br->id ( ) > m_bras_max_id)
			m_bras_max_id	= br->id ( );
#endif	// LIMA_MT
  for(_BrasInterne::iterator_noeud n = br->premier_noeud();
      n != br->dernier_noeud(); ++n){
    if(!contenir(*n))
      throw range_erreur("Le bras " + to_str(br->id()) + " reference "
			 "le noeud " + to_str((*n)->id()) +
			 " qui n'appartiennent pas au maillage " +
			 titre() + "\n");
  }
//  m_bras.ajouter(br);	// v 6.6.0
	if (true == m_bras.ajouter (br))	// v 6.6.0
	{
		att_bras().augmenter();
		br->attacher(this);
		br->ref_plus();
		m_composition_bras.nb_elements++;
		m_composition_bras.detail [br->nb_noeuds ( )]++;
	}	// v 6.6.0
}

void _MaillageInterne::ajouter(_NoeudInterne* nd)
{
#ifdef LIMA_MT
	if (0 == nd->id ( ))
		nd->set_id (++m_noeuds_max_id);
	else
		if (nd->id ( ) > m_noeuds_max_id)
			m_noeuds_max_id	= nd->id ( );
#endif	// LIMA_MT
//  m_noeuds.ajouter(nd);	// v 6.6.0
	if (true == m_noeuds.ajouter (nd))	// v 6.6.0
	{
		att_noeuds().augmenter();
		nd->attacher(this);
		nd->ref_plus();
		m_composition_noeuds.nb_elements++;
		m_composition_noeuds.detail [0]++;	
	}	// v 6.6.0
}

void _MaillageInterne::ajouter(_NuageInterne* nu)
{
  if (nu->ordre() == -1)
    nu->ordre(nb_nuages());
  att_nuages().augmenter();
  m_nuages.ajouter(nu);
  nu->attacher(this);
  nu->ref_plus();
}

void _MaillageInterne::ajouter(_LigneInterne* li)
{
  if (li->ordre() == -1)
    li->ordre(nb_lignes());
  att_lignes().augmenter();
  m_lignes.ajouter(li);
  li->attacher(this);
  li->ref_plus();
}

void _MaillageInterne::ajouter(_SurfaceInterne* su)
{
  if (su->ordre() == -1)
    su->ordre(nb_surfaces());
  att_surfaces().augmenter();
  m_surfaces.ajouter(su);
  su->attacher(this);
  su->ref_plus();
}

void _MaillageInterne::ajouter(_VolumeInterne* vo)
{
  if (vo->ordre() == -1)
    vo->ordre(nb_volumes());
  att_volumes().augmenter();
  m_volumes.ajouter(vo);
  vo->attacher(this);
  vo->ref_plus();
}

void _MaillageInterne::retirer(_NoeudInterne* nd)
{
//  m_noeuds.retirer(nd);	// v 6.6.0
	if (true == m_noeuds.retirer (nd))	// v 6.6.0
	{
		att_noeuds().retirer(m_noeuds.indice(nd->id()));
		nd->detacher();
		nd->ref_moins();
		m_composition_noeuds.nb_elements--;
		m_composition_noeuds.detail [0]--;
	}	// v 6.6.0
  if(nd->orphelin()) delete nd;
}

void _MaillageInterne::retirer(_BrasInterne* br)
{
//  m_bras.retirer(br);	// v 6.6.0
	if (true == m_bras.retirer (br))	// v 6.6.0
	{
		att_bras().retirer(m_bras.indice(br->id()));
		br->detacher();
		br->ref_moins();
		m_composition_bras.nb_elements--;
		m_composition_bras.detail [br->nb_noeuds ( )]--;
	}	// v 6.6.0
  if(br->orphelin()) delete br;
}

void _MaillageInterne::retirer(_PolygoneInterne* pg)
{
//  m_polygones.retirer(pg);	// v 6.6.0
	if (true == m_polygones.retirer (pg))	// v 6.6.0
	{
		att_polygones().retirer(m_polygones.indice(pg->id()));
		pg->detacher();
		pg->ref_moins();
		m_composition_polygones.nb_elements--;
		m_composition_polygones.detail [pg->nb_noeuds ( )]--;
	}	// v 6.6.0
  if(pg->orphelin()) delete pg;
}

void _MaillageInterne::retirer(_PolyedreInterne* pd)
{
//  m_polyedres.retirer(pd);	// v 6.6.0
	if (true == m_polyedres.retirer (pd))	// v 6.6.0
	{
		att_polyedres().retirer(m_polyedres.indice(pd->id()));
		m_polyedres.retirer(pd);
		pd->detacher();
		pd->ref_moins();
		m_composition_polyedres.nb_elements--;
		m_composition_polyedres.detail [pd->nb_noeuds ( )]--;
	}	// v 6.6.0
  if(pd->orphelin()) delete pd;
}

void _MaillageInterne::retirer(_NuageInterne* nu)
{
  att_nuages().retirer(m_nuages.indice(nu));
  m_nuages.retirer(nu);
  nu->detacher();
  nu->ref_moins();
  if(nu->orphelin()) delete nu;
}

void _MaillageInterne::retirer(_LigneInterne* li)
{
  att_lignes().retirer(m_lignes.indice(li));
  m_lignes.retirer(li);
  li->detacher();
  li->ref_moins();
  if(li->orphelin()) delete li;
}

void _MaillageInterne::retirer(_SurfaceInterne* su)
{
  att_surfaces().retirer(m_surfaces.indice(su));
  m_surfaces.retirer(su);
  su->detacher();
  su->ref_moins();
  if(su->orphelin()) delete su;
}

void _MaillageInterne::retirer(_VolumeInterne* vo)
{
  att_volumes().retirer(m_volumes.indice(vo));
  m_volumes.retirer(vo);
  vo->detacher();
  vo->ref_moins();
  if(vo->orphelin()) delete vo;
}

void _MaillageInterne::deplacer (size_type id, double x, double y, double z)
{
	_NoeudInterne*	noeud	= noeud_id (id);
	noeud->set_x (x);
	noeud->set_y (y);
	noeud->set_z (z);
}	// _MaillageInterne::deplacer

void _MaillageInterne::preparer(preparation_t preparation)
{
  switch(preparation){
  case MINIMALE:
    compacter_numerotation();
    break;
  case MAXIMALE:
    calculer_noeuds_polygones();
    m_etat_prepa	|= noe_pg;			// CP, version 5.4.3
    calculer_noeuds_polyedres();		// sera invalide
    calculer_polyedres_polygones();
    m_etat_prepa	|= pd_pg;			// CP, version 5.4.3
    invalider_noeuds_polyedres();
    calculer_polygones_polyedres();
    m_etat_prepa	|= pg_pd;			// CP, version 5.4.3
    calculer_contours_volumes();
    m_etat_prepa	|= cont_vol;			// CP, version 5.4.3
    compacter_numerotation();
    break;
  case MAILLEUR_2D:
    if(dimension()>D2)
      throw argument_erreur("Dimension maillage 3D incompatible avec la "
			    "preparation Mailleur 2D\n");
    calculer_noeuds_bras();
    calculer_noeuds_polygones();
    calculer_polygones_bras();
    invalider_noeuds_bras();
    invalider_noeuds_polygones();
    invalider_polygones_bras();

    compacter_numerotation();
    break;

  case CODE_A:
    calculer_noeuds_bras();				// sera invalide
    supprimer_doublon_bras();
    invalider_noeuds_bras();
    calculer_noeuds_polygones();		// sera invalide
    supprimer_doublon_polygones();
    invalider_noeuds_polygones();
    calculer_noeuds_polyedres();		// sera invalide
    supprimer_doublon_polyedres();
    invalider_noeuds_polyedres();
    compacter_numerotation();
    // orientation_positive();
    calculer_lignes_noeuds();
    m_etat_prepa	|= li_noe;			// CP, version 5.4.3
    calculer_surfaces_noeuds();
    m_etat_prepa	|= su_noe;			// CP, version 5.4.3
    calculer_volumes_noeuds();
    m_etat_prepa	|= vo_noe;			// CP, version 5.4.3
    break;
  case CODE_B:
    if(dimension()>D2)
      throw argument_erreur("Dimension maillage 3D incompatible avec la "
			    "preparation Code B\n");
    formater_noms ( );					// CP, version 5.4.4
    calculer_noeuds_bras();
    supprimer_doublon_bras();
    invalider_noeuds_bras();			// sera invalide
    calculer_noeuds_polygones();
    supprimer_doublon_polygones();
    invalider_noeuds_polygones();			// sera invalide

    orientation_positive();
    calculer_noeuds_bras();					// sera invalide
    calculer_noeuds_polygones();
    m_etat_prepa	|= noe_pg;			// CP, version 5.4.3
    calculer_polygones_bras();
    m_etat_prepa	|= pg_br;			// CP, version 5.4.3
    compacter_numerotation();

    calculer_lignes_noeuds();
    m_etat_prepa	|= li_noe;			// CP, version 5.4.3
    calculer_structuration_lignes();
    calculer_contours_lignes();
    m_etat_prepa	|= cont_lig;		// CP, version 5.4.3

    calculer_surfaces_bras();
    m_etat_prepa	|= su_br;			// CP, version 5.4.3
    calculer_surfaces_noeuds();
    m_etat_prepa	|= su_noe;			// CP, version 5.4.3
    calculer_contours_surfaces();
    m_etat_prepa	|= cont_sur;		// CP, version 5.4.3
    calculer_contours_surfaces_noeuds();
    m_etat_prepa	|= cont_sur_noe;	// CP, version 5.4.3
    calculer_structuration_surfaces();
    calculer_structuration_contours_surfaces();
    calculer_orientation_contours_surfaces ( );

    invalider_noeuds_bras();
    calculer_noeuds_bras();
    m_etat_prepa	|= noe_br;			// CP, version 5.4.3
    invalider_bras_polygones();
    calculer_bras_polygones();
    m_etat_prepa	|= br_pg;			// CP, version 5.4.3

    compacter_numerotation();
    {
      size_type nb=0;
      for(size_type s=0; s<nb_surfaces(); ++s){
	if(surface(s)->longueur_i()!=0) ++nb;
      }
      m_struct = LIBRE;
      if(nb) m_struct = MIXTE;
      if(nb==nb_surfaces()) m_struct = STRUCTURE;
    }

    break;

  case CODE_T:
    orientation_positive();

    calculer_noeuds_polygones();		// sera invalide
    supprimer_doublon_polygones();
    invalider_noeuds_polygones();
    calculer_noeuds_polygones();
    m_etat_prepa	|= noe_pg;			// CP, version 5.4.3
    calculer_noeuds_polyedres();
    m_etat_prepa	|= noe_pd;			// CP, version 5.4.3
    calculer_polyedres_polygones();
    m_etat_prepa	|= pd_pg;			// CP, version 5.4.3
    calculer_polygones_polyedres();
    m_etat_prepa	|= pg_pd;			// CP, version 5.4.3
    calculer_lignes_noeuds();
    m_etat_prepa	|= li_noe;			// CP, version 5.4.3
    calculer_surfaces_noeuds();
    m_etat_prepa	|= su_noe;			// CP, version 5.4.3
    calculer_volumes_noeuds();
    m_etat_prepa	|= vo_noe;			// CP, version 5.4.3
    compacter_numerotation();
    break;

  case BASE_3D:
    calculer_noeuds_bras();				// sera invalide
    supprimer_doublon_bras();
    invalider_noeuds_bras();
    calculer_noeuds_polygones();		// sera invalide
    supprimer_doublon_polygones();
    invalider_noeuds_polygones();
    calculer_noeuds_polyedres();		// sera invalide
    supprimer_doublon_polyedres();
    invalider_noeuds_polyedres();
    compacter_numerotation();
    orientation_positive();
    break;

  case PARTITIONNEMENT_3D:
    calculer_noeuds_polygones();		// sera invalide
    calculer_noeuds_polyedres();		// sera invalide
    calculer_polyedres_polygones();
    m_etat_prepa	|= pd_pg;			// CP, version 5.4.3
    invalider_noeuds_polygones();
    invalider_noeuds_polyedres();
    calculer_polygones_polyedres();
    m_etat_prepa	|= pg_pd;			// CP, version 5.4.3
    compacter_numerotation();
    orientation_positive();
    break;
  case LISSAGE_3D:
    calculer_noeuds_polygones();
    m_etat_prepa	|= noe_pg;			// CP, version 5.4.3
    calculer_noeuds_polyedres();
    m_etat_prepa	|= noe_pd;			// CP, version 5.4.3
    calculer_polyedres_polygones();
    m_etat_prepa	|= pd_pg;			// CP, version 5.4.3
    calculer_polygones_polyedres();
    m_etat_prepa	|= pg_pd;			// CP, version 5.4.3
    calculer_contours_volumes();
    m_etat_prepa	|= cont_vol;		// CP, version 5.4.3
    calculer_volumes_noeuds();
    m_etat_prepa	|= vo_noe;			// CP, version 5.4.3
    compacter_numerotation();
    orientation_positive();
    break;
  }
}

void _MaillageInterne::preparation_parametrable(TypeMasque preparation,
						bool init)
{
  // Initialisation de m_etat_prepa
/* CP, version 5.4.3. On garde les infos sur d'eventuelles preparation
   anterieures. Evite des doublons dans les tables de connectivites
   (invalider_X_Y n'etait pas appele.
  if (init == true) {
    m_etat_prepa = 0 ;
  } */

  /*
   *   TRAITEMENT DES PREPARATIONS PAR MOTS-CLES "CODE"
   */
  if ((preparation & LM_MINIMALE) != 0)
    {
      compacter_numerotation() ;
    }

  if ((preparation & LM_MAXIMALE) != 0)
    {
      calculer_noeuds_polygones();
      m_etat_prepa	|= noe_pg;			// CP, version 5.4.3
      calculer_noeuds_polyedres();		// sera invalide
      calculer_polyedres_polygones();
      m_etat_prepa	|= pd_pg;			// CP, version 5.4.3
      invalider_noeuds_polyedres();
      calculer_polygones_polyedres();
      m_etat_prepa	|= pg_pd;			// CP, version 5.4.3
      calculer_contours_volumes();
      m_etat_prepa	|= cont_vol;		// CP, version 5.4.3
      compacter_numerotation();
    }

  if ((preparation & LM_MAILLEUR_2D) != 0)
    {
      if(dimension()>D2)
        throw argument_erreur("Dimension maillage 3D incompatible avec la "
                              "preparation Mailleur 2D\n") ;
      calculer_noeuds_bras() ;		// sera invalide
      calculer_noeuds_polygones() ;	// sera invalide
      calculer_polygones_bras() ;	// sera invalide
      invalider_noeuds_bras() ;
      invalider_noeuds_polygones() ;
      invalider_polygones_bras() ;

      compacter_numerotation() ;
    }


  if ((preparation & LM_CODE_A) != 0)
    {
      calculer_noeuds_bras() ;		// sera invalide
      supprimer_doublon_bras() ;
      invalider_noeuds_bras() ;
      calculer_noeuds_polygones() ;	// sera invalide
      supprimer_doublon_polygones();
      invalider_noeuds_polygones() ;
      calculer_noeuds_polyedres() ;	// sera invalide
      supprimer_doublon_polyedres();
      invalider_noeuds_polyedres() ;
      compacter_numerotation() ;
      calculer_lignes_noeuds() ;
      m_etat_prepa	|= li_noe;			// CP, version 5.4.3
      calculer_surfaces_noeuds() ;
      m_etat_prepa	|= su_noe;			// CP, version 5.4.3
      calculer_volumes_noeuds() ;
      m_etat_prepa	|= vo_noe;			// CP, version 5.4.3
    }

  if ((preparation & LM_CODE_B) != 0)
    {
      if(dimension()>D2)
        throw argument_erreur("Dimension maillage 3D incompatible avec la "
                              "preparation Code B\n") ;
      formater_noms ( );				// CP, version 5.4.4
      calculer_noeuds_bras() ;			// sera invalide
      supprimer_doublon_bras() ;
      invalider_noeuds_bras() ;
      calculer_noeuds_polygones() ;		// sera invalide
      supprimer_doublon_polygones() ;
      invalider_noeuds_polygones() ;

      orientation_positive() ;
      calculer_noeuds_bras() ;			// sera invalide
      calculer_noeuds_polygones() ;
      m_etat_prepa	|= noe_pg;			// CP, version 5.4.3
      calculer_polygones_bras() ;
      m_etat_prepa	|= pg_br;			// CP, version 5.4.3
      compacter_numerotation() ;

      calculer_lignes_noeuds() ;
      m_etat_prepa	|= li_noe;			// CP, version 5.4.3
      calculer_structuration_lignes() ;
      calculer_contours_lignes() ;
      m_etat_prepa	|= cont_lig;		// CP, version 5.4.3

      calculer_surfaces_bras() ;
      m_etat_prepa	|= su_br;			// CP, version 5.4.3
      calculer_surfaces_noeuds() ;
      m_etat_prepa	|= su_noe;			// CP, version 5.4.3
      calculer_contours_surfaces() ;
      m_etat_prepa	|= cont_sur;		// CP, version 5.4.3
      calculer_contours_surfaces_noeuds() ;
      m_etat_prepa	|= cont_sur_noe;			// CP, version 5.4.3
      calculer_structuration_surfaces() ;
      calculer_structuration_contours_surfaces() ;
      calculer_orientation_contours_surfaces ( );

      invalider_noeuds_bras() ;
      calculer_noeuds_bras() ;
      m_etat_prepa	|= noe_br;			// CP, version 5.4.3
      invalider_bras_polygones() ;
      calculer_bras_polygones() ;
      m_etat_prepa	|= br_pg;			// CP, version 5.4.3

      compacter_numerotation() ;
      {
        size_type nb=0 ;
        for(size_type s=0; s<nb_surfaces(); ++s){
          if(surface(s)->longueur_i()!=0) ++nb ;
        }
        m_struct = LIBRE ;
        if(nb) m_struct = MIXTE ;
        if(nb==nb_surfaces()) m_struct = STRUCTURE ;
      }
    }

  if ((preparation & LM_CODE_T) != 0)
    {
      orientation_positive();

      calculer_noeuds_polygones();		// sera invalide
      supprimer_doublon_polygones();
      invalider_noeuds_polygones();
      calculer_noeuds_polygones();
      m_etat_prepa	|= noe_pg;			// CP, version 5.4.3
      calculer_noeuds_polyedres();
      m_etat_prepa	|= noe_pd;			// CP, version 5.4.3
      calculer_polyedres_polygones();
      m_etat_prepa	|= pd_pg;			// CP, version 5.4.3
      calculer_polygones_polyedres();
      m_etat_prepa	|= pg_pd;			// CP, version 5.4.3
      calculer_lignes_noeuds();
      m_etat_prepa	|= li_noe;			// CP, version 5.4.3
      calculer_surfaces_noeuds();
      m_etat_prepa	|= su_noe;			// CP, version 5.4.3
      calculer_volumes_noeuds();
      m_etat_prepa	|= vo_noe;			// CP, version 5.4.3
      compacter_numerotation();
    }

  if ((preparation & LM_BASE_3D) != 0)
    {
      calculer_noeuds_bras() ; 			// sera invalide
      supprimer_doublon_bras() ;
      invalider_noeuds_bras() ;
      calculer_noeuds_polygones() ;		// sera invalide
      supprimer_doublon_polygones() ;
      invalider_noeuds_polygones() ;
      calculer_noeuds_polyedres() ;		// sera invalide
      supprimer_doublon_polyedres() ;
      invalider_noeuds_polyedres() ;
      compacter_numerotation() ;
      orientation_positive() ;
    }

  if ((preparation & LM_PARTITIONNEMENT_3D) != 0)
    {
      calculer_noeuds_polygones() ;		// sera invalide
      calculer_noeuds_polyedres() ;		// sera invalide
      calculer_polyedres_polygones() ;	// sera invalide
      invalider_noeuds_polygones() ;
      invalider_noeuds_polyedres() ;
      calculer_polygones_polyedres() ;
      m_etat_prepa	|= pg_pd;			// CP, version 5.4.3
      compacter_numerotation() ;
      orientation_positive() ;
    }

  if ((preparation & LM_LISSAGE_3D) != 0)
    {
      calculer_noeuds_polygones() ;
      m_etat_prepa	|= noe_pg;			// CP, version 5.4.3
      calculer_noeuds_polyedres() ;
      m_etat_prepa	|= noe_pd;			// CP, version 5.4.3
      calculer_polyedres_polygones() ;
      m_etat_prepa	|= pd_pg;			// CP, version 5.4.3
      calculer_polygones_polyedres() ;
      m_etat_prepa	|= pg_pd;			// CP, version 5.4.3
      calculer_contours_volumes() ;
      m_etat_prepa	|= cont_vol;		// CP, version 5.4.3
      calculer_volumes_noeuds() ;
      m_etat_prepa	|= vo_noe;			// CP, version 5.4.3
      compacter_numerotation() ;
      orientation_positive() ;
    }


  /*
   *   TRAITEMENT DES PREPARATIONS ELEMENTAIRES
   */

  ///
  if ((preparation & LM_FORMATER_NOMS) != 0)
     formater_noms ( );

	if (0 != (preparation & LM_SUPPRIMER_DOUBLONS_NOEUDS))
	{
	  //_afficheMaillage();

	  if (0 == (m_etat_prepa & noe_br))
	    preparation_parametrable (LM_NOEUDS_BRAS);
	  if (0 == (m_etat_prepa & noe_pg))
	    preparation_parametrable (LM_NOEUDS_POLYGONES) ;
	  if (0 == (m_etat_prepa & noe_pd))
	    preparation_parametrable (LM_NOEUDS_POLYEDRES) ;

	  // Suppression des doublons de noeuds :
	  supprimer_doublon_noeuds ( );

	  preparation_parametrable (LM_RAZ_NOEUDS_BRAS) ;
	  preparation_parametrable (LM_RAZ_NOEUDS_POLYGONES) ;
	  preparation_parametrable (LM_RAZ_NOEUDS_POLYEDRES) ;

	  //_afficheMaillage();

	}	// if (0 != (preparation & LM_SUPPRIMER_DOUBLONS_NOEUDS))

  if ((preparation & LM_SUPPRIMER_DOUBLONS_BRAS) != 0) {
    dim_t dim = dimension() ;
    if ((dim == D1) || (dim == D2) || (dim == D3)) {
      // Test d'existence de la table noeuds/bras
      if ((m_etat_prepa & noe_br) == 0) {
        preparation_parametrable(LM_NOEUDS_BRAS) ;
      }

      // Suppression des doublons bras
      supprimer_doublon_bras() ;
      preparation_parametrable(LM_RAZ_NOEUDS_BRAS) ;
    }
  }

  if ((preparation & LM_SUPPRIMER_DOUBLONS_POLYGONES) != 0) {
    dim_t dim = dimension() ;
    if ((dim == D2) || (dim == D3)) {
      // Test d'existence de la table noeuds/polygones
      if ((m_etat_prepa & noe_pg) == 0) {
        preparation_parametrable(LM_NOEUDS_POLYGONES) ;
      }

      // Suppression des doublons polygones
      supprimer_doublon_polygones() ;
      preparation_parametrable(LM_RAZ_NOEUDS_POLYGONES) ;
    }
  }

  if ((preparation & LM_SUPPRIMER_DOUBLONS_POLYEDRES) != 0) {
    dim_t dim = dimension() ;
    if (dim == D3) {
      // Test d'existence de la table noeuds/polyedres
      if ((m_etat_prepa & noe_pd) == 0) {
        preparation_parametrable(LM_NOEUDS_POLYEDRES) ;
      }

      // Suppression des doublons polyedres
      supprimer_doublon_polyedres() ;
      preparation_parametrable(LM_RAZ_NOEUDS_POLYEDRES) ;
    }
  }

  if ((preparation & LM_SUPPRIMER_BRAS_INUTILISES) != 0) {
	  dim_t dim = dimension() ;
	  if ((dim == D1) || (dim == D2) || (dim == D3)) {
		  // Suppression des bras inutilisés (ceux qui ne sont référencés par aucune ligne)
		  supprimer_bras_inutilises() ;
	  }
  }

  if ((preparation & LM_SUPPRIMER_POLYGONES_INUTILISES) != 0) {
	  dim_t dim = dimension() ;
	  if ((dim == D2) || (dim == D3)) {
		  // Suppression des polygones inutilisés (ceux qui ne sont référencés par aucune surface)
		  supprimer_polygones_inutilises() ;
	  }
  }

  if ((preparation & LM_ORIENTATION) != 0) {
    orientation_positive() ;
  }

  if ((preparation & LM_POLYGONES_BRAS) != 0) {
    // Test d'existence de la table noeuds/bras
    if ((m_etat_prepa & noe_br) == 0) {
      preparation_parametrable(LM_NOEUDS_BRAS) ;
    }
    // Test d'existence de la table noeuds/polygones
    if ((m_etat_prepa & noe_pg) == 0) {
      preparation_parametrable(LM_NOEUDS_POLYGONES) ;
    }

    // Calcul de la table directe polygones/bras
    calculer_polygones_bras() ;
    m_etat_prepa = m_etat_prepa | pg_br ;

    // RAZ de la table noeuds/bras devenue non-valide
    preparation_parametrable(LM_RAZ_NOEUDS_BRAS) ;
  }

  if ((preparation & LM_POLYEDRES_BRAS) != 0) {
    // Test d'existence de la table noeuds/bras
    if ((m_etat_prepa & noe_br) == 0) {
      preparation_parametrable(LM_NOEUDS_BRAS) ;
    }
    // Test d'existence de la table noeuds/polyedres
    if ((m_etat_prepa & noe_pd) == 0) {
      preparation_parametrable(LM_NOEUDS_POLYEDRES) ;
    }

    // Calcul de la table directe polyedres/bras
    calculer_polyedres_bras() ;
    m_etat_prepa = m_etat_prepa | pd_br ;

    // RAZ de la table noeuds/bras devenue non-valide
    preparation_parametrable(LM_RAZ_NOEUDS_BRAS) ;
  }

  if ((preparation & LM_POLYEDRES_POLYGONES) != 0) {
    // Test d'existence de la table noeuds/polygones
    if ((m_etat_prepa & noe_pg) == 0) {
      preparation_parametrable(LM_NOEUDS_POLYGONES) ;
    }
    // Test d'existence de la table noeuds/polyedres
    if ((m_etat_prepa & noe_pd) == 0) {
      preparation_parametrable(LM_NOEUDS_POLYEDRES) ;
    }

    // Calcul de la table directe polyedres/polygones
    calculer_polyedres_polygones() ;
    m_etat_prepa = m_etat_prepa | pd_pg ;

    // RAZ de la table noeuds/polygones devenue non-valide
    preparation_parametrable(LM_RAZ_NOEUDS_POLYGONES) ;
  }

  if ((preparation & LM_LIGNES_NOEUDS) != 0) {
    // Calcul de la table directe lignes/noeuds
    calculer_lignes_noeuds() ;
    m_etat_prepa = m_etat_prepa | li_noe ;
  }

  if ((preparation & LM_SURFACES_NOEUDS) != 0) {
    // Calcul de la table directe surfaces/noeuds
    calculer_surfaces_noeuds() ;
    m_etat_prepa = m_etat_prepa | su_noe ;
  }

  if ((preparation & LM_SURFACES_BRAS) != 0) {
    // Test d'existence de la table polygones/bras
    if ((m_etat_prepa & pg_br) == 0 ) {
      preparation_parametrable(LM_POLYGONES_BRAS) ;
    }

    // Calcul de la table directe surfaces/bras
    calculer_surfaces_bras() ;
    m_etat_prepa = m_etat_prepa | su_br ;
  }

  if ((preparation & LM_VOLUMES_NOEUDS) != 0) {
    // Calcul de la table directe volumes/noeuds
    calculer_volumes_noeuds() ;
    m_etat_prepa = m_etat_prepa | vo_noe ;
  }

  if ((preparation & LM_VOLUMES_BRAS) != 0) {
    // Test d'existence de la table polyedres/bras
    if ((m_etat_prepa & pd_br) == 0 ) {
      preparation_parametrable(LM_POLYEDRES_BRAS) ;
    }

    // Calcul de la table directe volumes/bras
    calculer_volumes_bras() ;
    m_etat_prepa = m_etat_prepa | vo_br ;
  }

  if ((preparation & LM_VOLUMES_POLYGONES) != 0) {
    // Test d'existence de la table polyedres/polygones
    if ((m_etat_prepa & pd_pg) == 0 ) {
      preparation_parametrable(LM_POLYEDRES_POLYGONES) ;
    }

    // Calcul de la table directe volumes/polygones
    calculer_volumes_polygones() ;
    m_etat_prepa = m_etat_prepa | vo_pg ;
  }

  if ((preparation & LM_CONTOURS_LIGNES) != 0) {
    if ((m_etat_prepa & li_noe) == 0 ) {
      preparation_parametrable(LM_LIGNES_NOEUDS) ;
    }
    calculer_contours_lignes() ;
    m_etat_prepa = m_etat_prepa | cont_lig ;
  }

  if ((preparation & LM_CONTOURS_SURFACES) != 0) {
    if ((m_etat_prepa & su_br) == 0) {
      preparation_parametrable(LM_SURFACES_BRAS) ;
    }
    calculer_contours_surfaces() ;
    m_etat_prepa = m_etat_prepa | cont_sur ;
  }

  if ((preparation & LM_CONTOURS_VOLUMES) != 0) {
    if ((m_etat_prepa & vo_pg) == 0) {
      preparation_parametrable(LM_POLYEDRES_POLYGONES) ;
    }
    calculer_contours_volumes() ;
    m_etat_prepa = m_etat_prepa | cont_vol ;
  }

  if ((preparation & LM_CONTOURS_SURF_NOE) != 0) {
    // Test d'existence des contours des surfaces
    if ((m_etat_prepa & cont_sur) == 0) {
      preparation_parametrable(LM_CONTOURS_SURFACES) ;
    }

    // Calcul des contours des surfaces en terme de noeuds
    calculer_contours_surfaces_noeuds() ;
    m_etat_prepa = m_etat_prepa | cont_sur_noe ;
  }

  if ((preparation & LM_CONTOURS_VOL_NOE) != 0) {
    // Test d'existence des contours des volumes
    if ((m_etat_prepa & cont_vol) == 0) {
      preparation_parametrable(LM_CONTOURS_VOLUMES) ;
    }

    // Calcul des contours des volumes en terme de noeuds
    calculer_contours_volumes_noeuds() ;
  }

  if ((preparation & LM_CONTOURS_VOL_BR) != 0) {
    // Test d'existence des contours des volumes
    if ((m_etat_prepa & cont_vol) == 0) {
      preparation_parametrable(LM_CONTOURS_VOLUMES) ;
    }

    // Calcul des contours des volumes en terme de bras
    calculer_contours_volumes_bras() ;
  }

  if ((preparation & LM_STRUCTURER_LIGNES) != 0) {
    // Test d'existence de la table lignes/noeuds
    if ((m_etat_prepa & li_noe) == 0) {
      preparation_parametrable(LM_LIGNES_NOEUDS) ;
    }

    // Structuration des lignes
    calculer_structuration_lignes() ;
    preparation_parametrable(LM_RAZ_NOEUDS_BRAS) ;
  }

  if ((preparation & LM_STRUCTURER_SURFACES) != 0) {
    // Test d'existence de la table surfaces/bras
    if ((m_etat_prepa & su_br) == 0) {
      preparation_parametrable(LM_SURFACES_BRAS) ;
    }

    // Structuration des des surfaces
    calculer_structuration_surfaces() ;
    preparation_parametrable(LM_RAZ_BRAS_POLYGONES) ;

    size_type nb=0;
    for(size_type s=0; s<nb_surfaces(); ++s){
      if(surface(s)->longueur_i()!=0) ++nb;
    }
    m_struct = LIBRE;
    if(nb) m_struct = MIXTE;
    if(nb==nb_surfaces()) m_struct = STRUCTURE;
  }

  if ((preparation & LM_STRUCT_CONTOURS_SURF) != 0) {
    // Test d'existence des contours des surfaces
    if ((m_etat_prepa & cont_sur) == 0) {
      preparation_parametrable(LM_CONTOURS_SURFACES) ;
    }

    if ((m_etat_prepa & cont_sur_noe) == 0) {
      preparation_parametrable(LM_CONTOURS_SURF_NOE) ;
    }

    // Structuration des contours des surfaces
    calculer_structuration_contours_surfaces() ;
    preparation_parametrable(LM_RAZ_NOEUDS_BRAS) ;
  }

  if ((preparation & LM_ORIENT_CONTOURS_SURF) != 0)
  {
    if (!(preparation & LM_STRUCT_CONTOURS_SURF))
      preparation_parametrable (LM_STRUCT_CONTOURS_SURF);
    calculer_orientation_contours_surfaces ( );
  }	// if ((preparation & LM_ORIENT_CONTOURS_SURF) != 0)

  if ((preparation & LM_FORMATER_NOMS) != 0)
    formater_noms ( );

  if ((preparation & LM_NOEUDS_BRAS) != 0) {
    // Calcul de la table inverse noeuds/bras
    if((m_etat_prepa & noe_br) == 0)
      calculer_noeuds_bras() ;
    m_etat_prepa = m_etat_prepa | noe_br ;
  }

  if ((preparation & LM_NOEUDS_POLYGONES) != 0) {
    // Calcul de la table inverse noeuds/polygones
    if((m_etat_prepa & noe_pg) == 0)
      calculer_noeuds_polygones() ;
    m_etat_prepa = m_etat_prepa | noe_pg ;
  }

  if ((preparation & LM_BRAS_POLYGONES) != 0) {
    // Test d'existence de la table polygones/bras
    if ((m_etat_prepa & pg_br) == 0) {
      preparation_parametrable(LM_POLYGONES_BRAS) ;
    }

    // Calcul de la table inverse bras/polygones
    if((m_etat_prepa & br_pg) == 0)
      calculer_bras_polygones() ;
    m_etat_prepa = m_etat_prepa | br_pg ;
  }

  if ((preparation & LM_NOEUDS_POLYEDRES) != 0) {
    // Calcul de la table inverse noeuds/polyedres
    if((m_etat_prepa & noe_pd) == 0)
      calculer_noeuds_polyedres() ;
    m_etat_prepa = m_etat_prepa | noe_pd ;
  }

  if ((preparation & LM_BRAS_POLYEDRES) != 0) {
    // Test d'existence de la table polyedres/bras
    if ((m_etat_prepa & pd_br) == 0) {
      preparation_parametrable(LM_POLYEDRES_BRAS) ;
    }

    // Calcul de la table inverse bras/polyedres
    if((m_etat_prepa & br_pd) == 0)
      calculer_bras_polyedres() ;
    m_etat_prepa = m_etat_prepa | br_pd ;
  }

  if ((preparation & LM_POLYGONES_POLYEDRES) != 0) {
    // Test d'existence de la table polyedres/polygones
    if ((m_etat_prepa & pd_pg) == 0) {
      preparation_parametrable(LM_POLYEDRES_POLYGONES) ;
    }

    // Calcul de la table inverse polygones/polyedres
    if((m_etat_prepa & pg_pd) == 0)
      calculer_polygones_polyedres() ;
    m_etat_prepa = m_etat_prepa | pg_pd ;
  }

  if ((preparation & LM_RAZ_NOEUDS_BRAS) != 0) {
    // RAZ de la table noeuds/bras
    invalider_noeuds_bras() ;
    if(m_etat_prepa & noe_br)
      m_etat_prepa = m_etat_prepa ^ noe_br ;
  }

  if ((preparation & LM_RAZ_NOEUDS_POLYGONES) != 0) {
    // RAZ de la table noeuds/polygones
    invalider_noeuds_polygones() ;
    if(m_etat_prepa & noe_pg)
      m_etat_prepa = m_etat_prepa ^ noe_pg ;
  }

  if ((preparation & LM_RAZ_NOEUDS_POLYEDRES) != 0) {
    // RAZ de la table noeuds/polyedres
    invalider_noeuds_polyedres() ;
    if(m_etat_prepa & noe_pd)
      m_etat_prepa = m_etat_prepa ^ noe_pd ;
  }

  if ((preparation & LM_RAZ_POLYGONES_BRAS) != 0) {
    // RAZ de la table polygones/bras
    invalider_polygones_bras() ;
    if(m_etat_prepa & pg_br)
      m_etat_prepa = m_etat_prepa ^ pg_br ;
  }

  if ((preparation & LM_RAZ_BRAS_POLYGONES) != 0) {
    // RAZ de la table bras/polygones
    invalider_bras_polygones() ;
    if(m_etat_prepa & br_pg)
      m_etat_prepa = m_etat_prepa ^ br_pg ;
  }

  if ((preparation & LM_RAZ_POLYEDRES_BRAS) != 0) {
    // RAZ de la table polyedres/bras
    invalider_polyedres_bras() ;
    if(m_etat_prepa & pd_br)
      m_etat_prepa = m_etat_prepa ^ pd_br ;
  }

  if ((preparation & LM_RAZ_BRAS_POLYEDRES) != 0) {
    // RAZ de la table bras/polyedres
    invalider_bras_polyedres() ;
    if(m_etat_prepa & br_pd)
      m_etat_prepa = m_etat_prepa ^ br_pd ;
  }

  if ((preparation & LM_RAZ_POLYEDRES_POLYGONES) != 0) {
    // RAZ de la table polyedres/polygones
    invalider_polyedres_polygones() ;
    if(m_etat_prepa & pd_pg)
      m_etat_prepa = m_etat_prepa ^ pd_pg ;
  }

  if ((preparation & LM_RAZ_POLYGONES_POLYEDRES) != 0) {
    // RAZ de la table polygones/polyedres
    invalider_polygones_polyedres() ;
    if(m_etat_prepa & pg_pd)
      m_etat_prepa = m_etat_prepa ^ pg_pd ;
  }

  if ((preparation & LM_COMPACTE) != 0) {
    compacter_numerotation() ;
  }

}



void _MaillageInterne::verification_code (TypeVerificationCode type) const
{
	// Verification de la dimension du maillage :
	if (LM_VC_CODE_B == type)
	{
		if (D2 != dimension ( ))
			throw erreur ("Le maillage n'est pas de dimension 2.");
	}	// if (LM_VC_CODE_B == type)

	// Autres verifications du maillage :
	if (LM_VC_CODE_B == type)
		verification_parametrable (MASQUE_VERIFICATION_CODE_B);
	else
	{
		MutableString	message;
		message << "Masque de verification inconnu : " << (unsigned long)type
		        << ".";
			throw erreur (message.str ( ));
	}
}	// _MaillageInterne::verification_code


void _MaillageInterne::verification_parametrable (TypeMasqueVerification masque)
	const
{
	// On delegue la verification a la classe _Validateur :
	_Validateur::verification_parametrable (*this, (LM_V_TYPEMASQUE)masque);
}	// _MaillageInterne::verification_parametrable


void _MaillageInterne::formater_noms ( )
{
	for (iterator_nuage nu = premier_nuage ( ); nu != dernier_nuage ( ); nu++)
		(*nu)->formater_nom ( );
	for (iterator_ligne li = premier_ligne ( ); li != dernier_ligne ( ); li++)
		(*li)->formater_nom ( );
	for (iterator_surface su = premier_surface ( ); su != dernier_surface ( );
	     su++)
		(*su)->formater_nom ( );
	for (iterator_volume vo = premier_volume ( ); vo != dernier_volume ( );
	     vo++)
		(*vo)->formater_nom ( );
}	// void _MaillageInterne::formater_noms ( )


void _MaillageInterne::compacter_numerotation()
{
  size_type id;

  id = 1;
  for(iterator_noeud nd = premier_noeud(); nd != dernier_noeud(); ++nd)
    (*nd)->set_id(id++);
  id = 1;
  for(iterator_bras br = premier_bras(); br != dernier_bras(); ++br)
    (*br)->set_id(id++);
  id = 1;
  for(iterator_polygone pg = premier_polygone();
      pg != dernier_polygone(); ++pg)
    (*pg)->set_id(id++);
  id = 1;
  for(iterator_polyedre pd = premier_polyedre();
      pd != dernier_polyedre(); ++pd)
    (*pd)->set_id(id++);
}

void _MaillageInterne::calculer_noeuds_bras()
{
  invalider_noeuds_bras ( );	// CP, version 5.4.3
  connexions_inverses(premier_bras(), dernier_bras(),
		      _les_noeuds<_BrasInterne>());
}

void _MaillageInterne::calculer_noeuds_polygones()
{
  invalider_noeuds_polygones ( );	// CP, version 5.4.3
  connexions_inverses(premier_polygone(), dernier_polygone(),
		      _les_noeuds<_PolygoneInterne>());
}

void _MaillageInterne::calculer_noeuds_polyedres()
{
  invalider_noeuds_polyedres ( );	// CP, version 5.4.3
  connexions_inverses(premier_polyedre(), dernier_polyedre(),
		      _les_noeuds<_PolyedreInterne>());
}

void _MaillageInterne::calculer_bras_polygones()
{
  invalider_bras_polygones ( );	// CP, version 5.4.3
  connexions_inverses(premier_polygone(), dernier_polygone(),
		      _les_bras<_PolygoneInterne>());
}

void _MaillageInterne::calculer_bras_polyedres()
{
  invalider_bras_polyedres ( );	// CP, version 5.4.3
  connexions_inverses(premier_polyedre(), dernier_polyedre(),
		      _les_bras<_PolyedreInterne>());
}

void _MaillageInterne::calculer_polygones_polyedres()
{
  invalider_polygones_polyedres ( );	// CP, version 5.4.3
  connexions_inverses(premier_polyedre(), dernier_polyedre(),
		      _les_polygones<_PolyedreInterne>());
}

void _MaillageInterne::invalider_noeuds_bras()
{
  for_each(premier_noeud(), dernier_noeud(),
	   _invalider_bras<_NoeudInterne>());
}

void _MaillageInterne::invalider_noeuds_polygones()
{
  for_each(premier_noeud(), dernier_noeud(),
	   _invalider_polygones<_NoeudInterne>());
}

void _MaillageInterne::invalider_noeuds_polyedres()
{
  for_each(premier_noeud(), dernier_noeud(),
	   _invalider_polyedres<_NoeudInterne>());
}

void _MaillageInterne::invalider_bras_polygones()
{
  for_each(premier_bras(), dernier_bras(),
	   _invalider_polygones<_BrasInterne>());
}

void _MaillageInterne::invalider_polygones_bras()
{
  for_each(premier_polygone(), dernier_polygone(),
	   _invalider_bras<_PolygoneInterne>());
}

void _MaillageInterne::invalider_polyedres_bras()
{
  for_each(premier_polyedre(), dernier_polyedre(),
	   _invalider_bras<_PolyedreInterne>());
}

void _MaillageInterne::invalider_bras_polyedres()
{
  for_each(premier_bras(), dernier_bras(),
	   _invalider_polyedres<_BrasInterne>());
}

void _MaillageInterne::invalider_polyedres_polygones()
{
  for_each(premier_polyedre(), dernier_polyedre(),
	   _invalider_polygones<_PolyedreInterne>());
}

void _MaillageInterne::invalider_polygones_polyedres()
{
  for_each(premier_polygone(), dernier_polygone(),
	   _invalider_polyedres<_PolygoneInterne>());
}

void _MaillageInterne::calculer_lignes_noeuds()
{
  for_each(premier_ligne(), dernier_ligne(),
	   _extraire_noeuds<_LigneInterne>());
}

void _MaillageInterne::calculer_contours_lignes()
{
  for_each(premier_ligne(), dernier_ligne(),
	   _extraire_contour_<_LigneInterne>());
}

void _MaillageInterne::calculer_structuration_lignes()
{
  for_each(premier_ligne(), dernier_ligne(),
	   _structurer<_LigneInterne>());
}

void _MaillageInterne::calculer_surfaces_noeuds()
{
  for_each(premier_surface(), dernier_surface(),
	   _extraire_noeuds<_SurfaceInterne>());
}

void _MaillageInterne::calculer_surfaces_bras()
{
  for_each(premier_surface(), dernier_surface(),
	   _extraire_bras<_SurfaceInterne>());
}

void _MaillageInterne::calculer_contours_surfaces()
{
  for_each(premier_surface(), dernier_surface(),
	   _extraire_contour_<_SurfaceInterne>());
}

void _MaillageInterne::calculer_contours_surfaces_noeuds()
{
  for_each(premier_surface(), dernier_surface(),
	   _extraire_noeuds_contour<_SurfaceInterne>());
}

void _MaillageInterne::calculer_structuration_surfaces()
{
  for_each(premier_surface(), dernier_surface(),
	   _structurer<_SurfaceInterne>());
}

void _MaillageInterne::calculer_structuration_contours_surfaces()
{
    for_each(premier_surface(), dernier_surface(),
	   _structurer_contour<_SurfaceInterne>());
}

void _MaillageInterne::calculer_orientation_contours_surfaces ( )
{
	for_each (premier_surface ( ), dernier_surface ( ),
		_orienter_contour<_SurfaceInterne> ( ));
}

void _MaillageInterne::calculer_volumes_noeuds()
{
  for_each(premier_volume(), dernier_volume(),
	   _extraire_noeuds<_VolumeInterne>());
}

void _MaillageInterne::calculer_volumes_bras()
{
  for_each(premier_volume(), dernier_volume(),
	   _extraire_bras<_VolumeInterne>());
}

void _MaillageInterne::calculer_volumes_polygones()
{
  for_each(premier_volume(), dernier_volume(),
	   _extraire_polygones<_VolumeInterne>());
}

void _MaillageInterne::calculer_contours_volumes()
{
  for_each(premier_volume(), dernier_volume(),
	   _extraire_contour_<_VolumeInterne>());
}

void _MaillageInterne::calculer_contours_volumes_noeuds()
{
  for_each(premier_volume(), dernier_volume(),
	   _extraire_noeuds_contour<_VolumeInterne>());
}

void _MaillageInterne::calculer_contours_volumes_bras()
{
  for_each(premier_volume(), dernier_volume(),
	   _extraire_bras_contour<_VolumeInterne>());
}

void _MaillageInterne::orientation_positive()
{
  switch(dimension()){
  case D1:
    orienter(premier_bras(), dernier_bras(), &_BrasInterne::longueur);
    break;
  case D2:
    orienter(premier_polygone(), dernier_polygone(),
	     &_PolygoneInterne::surface2D);
    break;
  case D3:
    orienter(premier_polyedre(), dernier_polyedre(),
	     &_PolyedreInterne::volume);
    break;
  }
}

void _MaillageInterne::calculer_polygones_bras()
{
  bool invalider = false;

  for(iterator_polygone e = premier_polygone();
      e != dernier_polygone(); ++e)
    (*e)->reserver_bras();

  for(iterator_polygone pg = premier_polygone();
      pg != dernier_polygone(); ++pg) {
    _PolygoneInterne* le_polygone = *pg;

    for(size_type ct=0; ct<le_polygone->nb_aretes(); ++ct){

      /* test si le coté n'est pas déjà traité */
      if(ct>=le_polygone->nb_bras() || le_polygone->bras(ct)==NULL){
	_BrasInterne* le_cote = le_polygone->extraire_arete(ct);
#ifdef LIMA_MT
	// Version 6.5.0 :
	// Maintient de compatibilité avec les versions antérieures ou égales à la
	// version 6.3.3 : on affecte un id à ce bras, quite à créer un trou
	// dans la numérotation s'il existe déjà (en tant que bras avec ces
	// noeuds) car il va être détruit par ce même algo.
	if (0 == le_cote->id ( ))	// Ce doit être le cas
		le_cote->set_id (++m_bras_max_id);
#endif	// LIMA_MT

	// Recherche si le bras existe déjà

	// noeud connecte au minimum de bras
	_NoeudInterne* le_noeud = le_cote->noeud(0);
	size_type nd;
	for(nd=1; nd<le_cote->nb_noeuds(); ++nd){
	  if(le_cote->noeud(nd)->nb_bras()<le_noeud->nb_bras())
	    le_noeud = le_cote->noeud(nd);
	}

	_BrasInterne* le_bras = NULL;
	for(size_type br=0; br<le_noeud->nb_bras(); ++br){
	  if(_BrasInterne::comparer(le_noeud->bras(br), le_cote)){
	    le_bras = le_noeud->bras(br);
	    /* reecriture des noeuds du bras pour orientation
	     * en accord avec le polygone de plus petit numero */
	    le_bras->copier(le_cote);
	  }
	}
	if(le_bras==NULL){
	  invalider = true;
	  ajouter(le_cote);
	  le_bras = le_cote;
	}

	le_polygone->connecter(le_bras, ct);

	// recherche si le cote borde une autre maille

	// noeud connecte au minimum de maille
	le_noeud = le_cote->noeud(0);
	for(nd=1; nd<le_cote->nb_noeuds(); ++nd){
	  if(le_cote->noeud(nd)->nb_polygones()<le_noeud->nb_polygones())
	    le_noeud = le_cote->noeud(nd);
	}

	/* boucle sur les mailles voisines lies au noeud */
	for(size_type vs=0; vs<le_noeud->nb_polygones(); ++vs){
	  _PolygoneInterne* le_voisin = le_noeud->polygone(vs);
	  /* test si la maille voisine située apres la maille */
	  if(le_voisin->id()>le_polygone->id()){
	    int position = le_voisin->contenir(le_bras);
	    if(position!=-1)
	      le_voisin->connecter(le_bras, position);
	  }
	}
	if(le_cote->orphelin()) delete le_cote;
      }
    }
  }
  if(invalider==true){
    invalider_noeuds_bras();
    calculer_noeuds_bras();
  }
}

void _MaillageInterne::calculer_polyedres_bras()
{
  bool invalider = false;

  for(iterator_polyedre e = premier_polyedre();
      e != dernier_polyedre(); ++e)
    (*e)->reserver_bras();

  for(iterator_polyedre pd = premier_polyedre();
      pd != dernier_polyedre(); ++pd) {
    _PolyedreInterne* le_polyedre = *pd;

    for(size_type ct=0; ct<le_polyedre->nb_aretes(); ++ct){

      /* test si le coté n'est pas déjà traité */
      if(ct>=le_polyedre->nb_bras() || le_polyedre->bras(ct)==NULL){
	_BrasInterne* le_cote = le_polyedre->extraire_arete(ct);
#ifdef LIMA_MT
	// Version 6.5.0 :
	// Maintient de compatibilité avec les versions antérieures ou égales à la
	// version 6.3.3 : on affecte un id à ce bras, quite à créer un trou
	// dans la numérotation s'il existe déjà (en tant que bras avec ces
	// noeuds) car il va être détruit par ce même algo.
	if (0 == le_cote->id ( ))	// Ce doit être le cas
		le_cote->set_id (++m_bras_max_id);
#endif	// LIMA_MT

	// Recherche si le bras existe déjà

	// noeud connecte au minimum de bras
	_NoeudInterne* le_noeud = le_cote->noeud(0);
	size_type nd;
	for(nd=1; nd<le_cote->nb_noeuds(); ++nd){
	  if(le_cote->noeud(nd)->nb_bras()<le_noeud->nb_bras())
	    le_noeud = le_cote->noeud(nd);
	}

	_BrasInterne* le_bras = NULL;
	for(size_type br=0; br<le_noeud->nb_bras(); ++br){
	  if(_BrasInterne::comparer(le_noeud->bras(br), le_cote)){
	    le_bras = le_noeud->bras(br);
	    /* reecriture des noeuds du bras pour orientation
	     * en accord avec le polygone de plus petit numero */
	    le_bras->copier(le_cote);
	  }
	}
	if(le_bras==NULL){
	  invalider = true;
	  ajouter(le_cote);
	  le_bras = le_cote;
	}

	le_polyedre->connecter(le_bras, ct);

	// recherche si le cote borde une autre maille

	// noeud connecte au minimum de maille
	le_noeud = le_cote->noeud(0);
	for(nd=1; nd<le_cote->nb_noeuds(); ++nd){
	  if(le_cote->noeud(nd)->nb_polyedres()<le_noeud->nb_polyedres())
	    le_noeud = le_cote->noeud(nd);
	}

	/* boucle sur les mailles voisines lies au noeud */
	for(size_type vs=0; vs<le_noeud->nb_polyedres(); ++vs){
	  _PolyedreInterne* le_voisin = le_noeud->polyedre(vs);
	  /* test si la maille voisine située apres la maille */
	  if(le_voisin->id()>le_polyedre->id()){
	    int position = le_voisin->contenir(le_bras);
	    if(position!=-1)
	      le_voisin->connecter(le_bras, position);
	  }
	}
	if(le_cote->orphelin()) delete le_cote;
      }
    }
  }
  if(invalider==true){
    invalider_noeuds_bras();
    calculer_noeuds_bras();
  }
}

void _MaillageInterne::calculer_polyedres_polygones()
{
  bool invalider = false;

  for(iterator_polyedre e = premier_polyedre();
      e != dernier_polyedre(); ++e)
    (*e)->reserver_polygones();

  for(iterator_polyedre pd = premier_polyedre();
      pd != dernier_polyedre(); ++pd) {
    _PolyedreInterne* le_polyedre = *pd;

    for(size_type ct=0; ct<le_polyedre->nb_faces(); ++ct){

      /* test si le coté n'est pas déjà traité */
      if(ct>=le_polyedre->nb_polygones() || le_polyedre->polygone(ct)==NULL){
	_PolygoneInterne* le_cote = le_polyedre->extraire_face(ct);
#ifdef LIMA_MT
	// Version 6.5.0 :
	// Maintient de compatibilité avec les versions antérieures ou égales à la
	// version 6.3.3 : on affecte un id à ce polygones, quite à créer un trou
	// dans la numérotation s'il existe déjà (en tant que polygone avec ces
	// noeuds) car il va être détruit par ce même algo.
	if (0 == le_cote->id ( ))	// Ce doit être le cas
		le_cote->set_id (++m_polygones_max_id);
#endif	// LIMA_MT

	// Recherche si le polygone existe déjà

	// noeud connecte au minimum de polygone
	_NoeudInterne* le_noeud = le_cote->noeud(0);
	size_type nd;
	for(nd=1; nd<le_cote->nb_noeuds(); ++nd){
	  if(le_cote->noeud(nd)->nb_polygones()<le_noeud->nb_polygones())
	    le_noeud = le_cote->noeud(nd);
	}

	_PolygoneInterne* le_polygone = NULL;
	for(size_type pg=0; pg<le_noeud->nb_polygones(); ++pg){
	  if(_PolygoneInterne::comparer(le_noeud->polygone(pg), le_cote)){
	    le_polygone = le_noeud->polygone(pg);
	    /* reecriture des noeuds du polygone pour orientation
	     * en accord avec le polygone de plus petit numero */
	    le_polygone->copier(le_cote);
	  }
	}
	if(le_polygone==NULL){
	  invalider = true;
	  ajouter(le_cote);
	  le_polygone = le_cote;
	}

	le_polyedre->connecter(le_polygone, ct);

	// recherche si le cote borde une autre maille

	// noeud connecte au minimum de maille
	le_noeud = le_cote->noeud(0);
	for(nd=1; nd<le_cote->nb_noeuds(); ++nd){
	  if(le_cote->noeud(nd)->nb_polyedres()<le_noeud->nb_polyedres())
	    le_noeud = le_cote->noeud(nd);
	}

	/* boucle sur les mailles voisines lies au noeud */
	for(size_type vs=0; vs<le_noeud->nb_polyedres(); ++vs){
	  _PolyedreInterne* le_voisin = le_noeud->polyedre(vs);
	  /* test si la maille voisine située apres la maille */
	  if(le_voisin->id()>le_polyedre->id()){
	    int position = le_voisin->contenir(le_polygone);
	    if(position!=-1)
	      le_voisin->connecter(le_polygone, position);
	  }
	}
	if(le_cote->orphelin()) delete le_cote;
      }
    }
  }

  if(invalider==true){
    invalider_noeuds_polygones();
    calculer_noeuds_polygones();
  }
}


void _MaillageInterne::supprimer_doublon_noeuds ( )
{

  if (2 > nb_noeuds ( ))
    return;

  if (dimension()==D2) {
    // on utilise la version optimisée spécialisée pour le 2D
    supprimer_doublon_noeuds_2D ();
    return;
  } else if (dimension()==D3) {
    // on utilise la version optimisée spécialisée pour le 3D
    supprimer_doublon_noeuds_3D ();
    return;
  }

  // version initiale, non optimisée

  size_t	i		= 0;
  size_t	nb_n	= nb_noeuds ( );
  bool*	a_supprimer	= new bool [nb_n + 1];
  memset (a_supprimer, (int)false, (nb_n + 1) * sizeof (bool));
  iterator_noeud	it1	= dernier_noeud ( );
  it1--;
  for( ; it1 != premier_noeud ( ); it1--)
    {
      iterator_noeud	it2	= it1;
      it2--;

      for ( ; ; it2--)
	{
	  if (**it1 == **it2)
	    {
	      _NoeudInterne*	n1	= *it1;
	      _NoeudInterne*	n2	= *it2;
	      remplacer_noeud (n1, n2);
	      a_supprimer [n1->id ( )]	= true;
	    }	// if (*it1 == *it2)

	  if (premier_noeud ( ) == it2)
	    break;
	}	// for ( ; ; it2--)
    }	// for( ; it1 != premier_noeud ( ); it1--)
  for (i = nb_n; i > 0; i--)
    if (true == a_supprimer [i])
      retirer (noeud_id (i));
  delete [] a_supprimer;
} // _MaillageInterne::supprimer_doublon_noeuds


void _MaillageInterne::supprimer_doublon_noeuds_2D ( )
{
  //std::cout << "_MaillageInterne::supprimer_doublon_noeuds_2D()\n";

  size_t	nb_n	= nb_noeuds ( );

  // recherche de la boite englobante
  double xmin = (*premier_noeud ( ))->x();
  double xmax = (*premier_noeud ( ))->x();
  double ymin = (*premier_noeud ( ))->y();
  double ymax = (*premier_noeud ( ))->y();
  uint   max_id = (*premier_noeud ( ))->id();

  for( iterator_noeud it1 = premier_noeud ( ); it1 != dernier_noeud(); ++it1) {
    _NoeudInterne* n1 = *it1;
    double   x  = n1->x(), y = n1->y();
    uint     id = n1->id();
    if (x<xmin) xmin = x;
    if (x>xmax) xmax = x;
    if (y<ymin) ymin = y;
    if (y>ymax) ymax = y;
    if (id>max_id) max_id = id;
  }

  uint nb_boites = (uint)sqrt((double)nb_n)+1; // nombre de discrétisations dans chaques directions

  //std::cout << " nb_boites = "<< nb_boites << std::endl;

  double prec = sqrt(_Point::epsilon2);

  //std::cout << " prec = "<< prec << std::endl;

  double delta_x = (xmax - xmin) / (double) nb_boites + prec;
  double delta_y = (ymax - ymin) / (double) nb_boites + prec;

  xmin -= delta_x;
  ymin -= delta_y;
  nb_boites += 2;

  // tableau de décompostion spaciale, les noeuds y son regroupés par rectangle
  vector<_NoeudInterne*> * tableau = new vector<_NoeudInterne*>[nb_boites*nb_boites];

  // on met dans chacune des cases des pointeurs sur les objets qui s'y trouvent
  for( iterator_noeud it1 = premier_noeud ( ); it1 != dernier_noeud(); ++it1) {
    _NoeudInterne* obi = *it1;

    uint id_x = (uint)((obi->x() - xmin)/delta_x);
    uint id_y = (uint)((obi->y() - ymin)/delta_y);

    tableau[id_x+nb_boites*id_y].push_back(obi);
  }

  // on se donne un tableau de correspondance entre un noeud et son remplaceant s'il y a lieu
  // attention, les id ne sont pas forcément sans trou, aussi utilise-t-on le max_id
  bool* a_supprimer = new bool [max_id + 1];
  memset (a_supprimer, (int)false, (max_id + 1) * sizeof (bool));

  // grosse boucle de recherche
  //std::cout <<"boucle sur les objets\n";
  // on fait le calcul de distance pour les objets dans les cases proches (9 cases)
  for( iterator_noeud it1 = premier_noeud ( ); it1 != dernier_noeud(); ++it1) {
    _NoeudInterne* obi = *it1;

    bool ob_ref_to_fusion = false;
    uint id_x = (uint)((obi->x() - xmin)/delta_x);
    uint id_y = (uint)((obi->y() - ymin)/delta_y);

    uint id_x_min = (id_x>0?id_x-1:id_x); // les erreurs numériques peuvent nous ramemener à 0
    uint id_y_min = (id_y>0?id_y-1:id_y);
    uint id_x_max = id_x+1;
    uint id_y_max = id_y+1;

    // boucles sur les 9 cases
    for (uint id_x_i=id_x_min; id_x_i<=id_x_max; id_x_i++)
      for (uint id_y_i=id_y_min; id_y_i<=id_y_max; id_y_i++){
	vector<_NoeudInterne*> & une_case = tableau[id_x_i+nb_boites*id_y_i];
	// boucle sur les objets dans cette case
	for (vector<_NoeudInterne*>::iterator itab = une_case.begin(); itab != une_case.end(); ++itab ){
	  _NoeudInterne* n2 = *itab;
	  if (obi->id() < n2->id() && *obi == *n2) {
	    //std::cout << n2->id()<<" est remplacé par "<< obi->id()<<std::endl;
	    a_supprimer[n2->id()] = true;
	    remplacer_noeud (n2, obi);

	  } // end if (obi->id() < n2->id() && *obi == *n2)
	} // end for itab = une_case.begin()
      }// end for id_y_i=id_y_min ...
  } // end for it1 = premier_noeud ( ) ...

  delete[] tableau;

  // phase de suppression des noeuds
  //std::cout <<"suppression des noeuds\n";
  iterator_noeud it1 = dernier_noeud ( );
  it1--;
  for( ; it1 != premier_noeud(); --it1) {
    _NoeudInterne* n1 = *it1;
    if (a_supprimer[n1->id()] == true)
      retirer(n1);
  } // end for it1 = dernier_noeud ( ) ...

  delete [] a_supprimer;

  //  _afficheMaillage();

} // _MaillageInterne::supprimer_doublon_noeuds_2D


void _MaillageInterne::supprimer_doublon_noeuds_3D ( )
{
  //std::cout << "_MaillageInterne::supprimer_doublon_noeuds_3D()\n";

  size_t	nb_n	= nb_noeuds ( );

  // recherche de la boite englobante
  double xmin = (*premier_noeud ( ))->x();
  double xmax = (*premier_noeud ( ))->x();
  double ymin = (*premier_noeud ( ))->y();
  double ymax = (*premier_noeud ( ))->y();
  double zmin = (*premier_noeud ( ))->z();
  double zmax = (*premier_noeud ( ))->z();
  uint   max_id = (*premier_noeud ( ))->id();

  for( iterator_noeud it1 = premier_noeud ( ); it1 != dernier_noeud(); ++it1) {
    _NoeudInterne* n1 = *it1;
    double   x  = n1->x(), y = n1->y(), z = n1->z();
    uint     id = n1->id();
    if (x<xmin) xmin = x;
    if (x>xmax) xmax = x;
    if (y<ymin) ymin = y;
    if (y>ymax) ymax = y;
    if (z<zmin) zmin = z;
    if (z>zmax) zmax = z;
    if (id>max_id) max_id = id;
  }

  uint nb_boites = (uint)pow((double)nb_n,1.0/3.0)+1; // nombre de discrétisations dans chaques directions

  //std::cout << " nb_boites = "<< nb_boites << std::endl;

  double prec = sqrt(_Point::epsilon2);

  //std::cout << " prec = "<< prec << std::endl;

  double delta_x = (xmax - xmin) / (double) nb_boites + prec;
  double delta_y = (ymax - ymin) / (double) nb_boites + prec;
  double delta_z = (zmax - zmin) / (double) nb_boites + prec;

  xmin -= delta_x;
  ymin -= delta_y;
  zmin -= delta_z;
  nb_boites += 2;

  // tableau de décompostion spaciale, les noeuds y son regroupés par rectangle
  vector<_NoeudInterne*> * tableau = new vector<_NoeudInterne*>[nb_boites*nb_boites*nb_boites];

  // on met dans chacune des cases des pointeurs sur les objets qui s'y trouvent
  for( iterator_noeud it1 = premier_noeud ( ); it1 != dernier_noeud(); ++it1) {
    _NoeudInterne* obi = *it1;

    uint id_x = (uint)((obi->x() - xmin)/delta_x);
    uint id_y = (uint)((obi->y() - ymin)/delta_y);
    uint id_z = (uint)((obi->z() - zmin)/delta_z);

    tableau[id_x+nb_boites*id_y+nb_boites*nb_boites*id_z].push_back(obi);
  }

  // on se donne un tableau de correspondance entre un noeud et son remplaceant s'il y a lieu
  // attention, les id ne sont pas forcément sans trou, aussi utilise-t-on le max_id
  bool* a_supprimer = new bool [max_id + 1];
  memset (a_supprimer, (int)false, (max_id + 1) * sizeof (bool));

  // grosse boucle de recherche
  //std::cout <<"boucle sur les objets\n";
  // on fait le calcul de distance pour les objets dans les cases proches (27 cases)
  for( iterator_noeud it1 = premier_noeud ( ); it1 != dernier_noeud(); ++it1) {
    _NoeudInterne* obi = *it1;

    bool ob_ref_to_fusion = false;
    uint id_x = (uint)((obi->x() - xmin)/delta_x);
    uint id_y = (uint)((obi->y() - ymin)/delta_y);
    uint id_z = (uint)((obi->z() - zmin)/delta_z);

    uint id_x_min = (id_x>0?id_x-1:id_x); // les erreurs numériques peuvent nous ramemener à 0
    uint id_y_min = (id_y>0?id_y-1:id_y);
    uint id_z_min = (id_y>0?id_z-1:id_z);
    uint id_x_max = id_x+1;
    uint id_y_max = id_y+1;
    uint id_z_max = id_z+1;

    // boucles sur les 27 cases
    for (uint id_x_i=id_x_min; id_x_i<=id_x_max; id_x_i++)
      for (uint id_y_i=id_y_min; id_y_i<=id_y_max; id_y_i++)
	for (uint id_z_i=id_z_min; id_z_i<=id_z_max; id_z_i++){
	  vector<_NoeudInterne*> & une_case = tableau[id_x_i+nb_boites*id_y_i+nb_boites*nb_boites*id_z_i];
	  // boucle sur les objets dans cette case
	  for (vector<_NoeudInterne*>::iterator itab = une_case.begin(); itab != une_case.end(); ++itab ){
	    _NoeudInterne* n2 = *itab;
	    if (obi->id() < n2->id() && *obi == *n2) {
	      //std::cout << n2->id()<<" est remplacé par "<< obi->id()<<std::endl;
	      a_supprimer[n2->id()] = true;
	      remplacer_noeud (n2, obi);

	    } // end if (obi->id() < n2->id() && *obi == *n2)
	  } // end for itab = une_case.begin()
	}// end for id_z_i=id_z_min ...
  } // end for it1 = premier_noeud ( ) ...

  delete[] tableau;

  // phase de suppression des noeuds
  //std::cout <<"suppression des noeuds\n";
  iterator_noeud it1 = dernier_noeud ( );
  it1--;
  for( ; it1 != premier_noeud(); --it1) {
    _NoeudInterne* n1 = *it1;
    if (a_supprimer[n1->id()] == true)
      retirer(n1);
  } // end for it1 = dernier_noeud ( ) ...

  delete [] a_supprimer;

  //  _afficheMaillage();

} // _MaillageInterne::supprimer_doublon_noeuds_3D


#ifdef DEBUG
void _MaillageInterne::_afficheMaillage()
{
  std::cout << " Les noeuds : (id, x, y, z)\n";
  for( iterator_noeud it1 = premier_noeud ( ); it1 != dernier_noeud(); ++it1) {
    _NoeudInterne* obi = *it1;
    std::cout<<obi->id()<<", "<<obi->x()<<", "<<obi->y()<<", "<<obi->z()<<std::endl;
  }

  std::cout << " Les polygones : (id, nd1->id, nd2->id ...)\n";
  for( iterator_polygone it1 = premier_polygone ( ); it1 != dernier_polygone(); ++it1) {
    _PolygoneInterne* po = *it1;
    std::cout<<po->id();

    for( _PolygoneInterne::iterator_noeud it2 = po->premier_noeud ( ); it2 != po->dernier_noeud(); ++it2) {
      _NoeudInterne* nd = *it2;
      std::cout<<", "<<nd->id();
    }
    std::cout<<std::endl;
  }

  std::cout << " Les polyedres : (id, nd1->id, nd2->id ...)\n";
  for( iterator_polyedre it1 = premier_polyedre ( ); it1 != dernier_polyedre(); ++it1) {
    _PolyedreInterne* pe = *it1;
    std::cout<<pe->id();

    for( _PolyedreInterne::iterator_noeud it2 = pe->premier_noeud ( ); it2 != pe->dernier_noeud(); ++it2) {
      _NoeudInterne* nd = *it2;
      std::cout<<", "<<nd->id();
    }
    std::cout<<std::endl;
  }
} // _MaillageInterne::_afficheMaillage()
#endif

void _MaillageInterne::supprimer_doublon_bras()
{
  bool invalider = false;

  for(iterator_bras e = premier_bras();
      e != dernier_bras(); ++e)
    (*e)->set_udata(NULL);

  for(iterator_bras br = premier_bras();
      br != dernier_bras(); ++br) {
    _BrasInterne* le_bras = *br;

    /* test si le bras n'est pas déjà traité */
    if(le_bras->udata() != NULL)
      continue;

    // noeud connecte au minimum de maille
    _NoeudInterne* le_noeud = le_bras->noeud(0);
    for(size_type nd=1; nd<le_bras->nb_noeuds(); ++nd){
      if(le_bras->noeud(nd)->nb_bras()<le_noeud->nb_bras())
	le_noeud = le_bras->noeud(nd);
    }

    /* boucle sur les mailles voisines lies au noeud */
    for(size_type vs=0; vs<le_noeud->nb_bras(); ++vs){
      _BrasInterne* le_voisin = le_noeud->bras(vs);
      /* test si la maille voisine située apres la maille */
      if(le_voisin->id()>le_bras->id()){
	if(_BrasInterne::comparer(le_bras, le_voisin)){
	  le_voisin->set_udata(le_bras);
	  invalider = true;
	}
      }
    }
  }

  for(iterator_ligne li = premier_ligne();
      li != dernier_ligne(); ++li) {
    for(iterator_bras br = (*li)->premier_bras();
	br != (*li)->dernier_bras(); ++br) {
      if((*br)->udata() != NULL)
	*br = (_BrasInterne*)(*br)->udata();
    }
  }

  if(invalider==true){
    Ensemble<_BrasInterne> new_bras;
    for(iterator_bras br = premier_bras();
	br != dernier_bras(); ++br){
      if((*br)->udata()!=NULL){
        (*br)->detacher();
	(*br)->ref_moins();
    m_composition_bras.nb_elements--;
    m_composition_bras.detail [(*br)->nb_noeuds ( )]--;
	if((*br)->orphelin()) delete (*br);
      }
      else
	new_bras.ajouter(*br);
    }
    m_bras = new_bras;
  }
}

void _MaillageInterne::supprimer_doublon_polygones()
{
  bool invalider = false;

  for(iterator_polygone e = premier_polygone();
      e != dernier_polygone(); ++e)
    (*e)->set_udata(NULL);

  for(iterator_polygone pg = premier_polygone();
      pg != dernier_polygone(); ++pg) {
    _PolygoneInterne* le_polygone = *pg;

    /* test si le polygone n'est pas déjà traité */
    if(le_polygone->udata() != NULL)
      continue;

    // noeud connecte au minimum de maille
    _NoeudInterne* le_noeud = le_polygone->noeud(0);
    for(size_type nd=1; nd<le_polygone->nb_noeuds(); ++nd){
      if(le_polygone->noeud(nd)->nb_polygones()<le_noeud->nb_polygones())
	le_noeud = le_polygone->noeud(nd);
    }

    /* boucle sur les mailles voisines lies au noeud */
    for(size_type vs=0; vs<le_noeud->nb_polygones(); ++vs){
      _PolygoneInterne* le_voisin = le_noeud->polygone(vs);
      /* test si la maille voisine située apres la maille */
      if(le_voisin->id()>le_polygone->id()){
	if(_PolygoneInterne::comparer(le_polygone, le_voisin)){
	  le_voisin->set_udata(le_polygone);
	  invalider = true;
	}
      }
    }
  }

  for(iterator_surface su = premier_surface();
      su != dernier_surface(); ++su) {
    for(iterator_polygone pg = (*su)->premier_polygone();
	pg != (*su)->dernier_polygone(); ++pg) {
      if((*pg)->udata() != NULL)
	*pg = (_PolygoneInterne*)(*pg)->udata();
    }
  }

  if(invalider==true){
    Ensemble<_PolygoneInterne> new_polygones;
    for(iterator_polygone pg = premier_polygone();
	pg != dernier_polygone(); ++pg){
      if((*pg)->udata()!=NULL){
        (*pg)->detacher();
	(*pg)->ref_moins();
    m_composition_polygones.nb_elements--;
    m_composition_polygones.detail [(*pg)->nb_noeuds ( )]--;
	if((*pg)->orphelin()) delete (*pg);
      }
      else
	new_polygones.ajouter(*pg);
    }
    m_polygones = new_polygones;
  }
}

void _MaillageInterne::supprimer_doublon_polyedres()
{
  bool invalider = false;

  for(iterator_polyedre e = premier_polyedre();
      e != dernier_polyedre(); ++e)
    (*e)->set_udata(NULL);

  for(iterator_polyedre pd = premier_polyedre();
      pd != dernier_polyedre(); ++pd) {
    _PolyedreInterne* le_polyedre = *pd;

    /* test si le polyedre n'est pas déjà traité */
    if(le_polyedre->udata() != NULL)
      continue;

    // noeud connecte au minimum de maille
    _NoeudInterne* le_noeud = le_polyedre->noeud(0);
    for(size_type nd=1; nd<le_polyedre->nb_noeuds(); ++nd){
      if(le_polyedre->noeud(nd)->nb_polyedres()<le_noeud->nb_polyedres())
	le_noeud = le_polyedre->noeud(nd);
    }

    /* boucle sur les mailles voisines lies au noeud */
    for(size_type vs=0; vs<le_noeud->nb_polyedres(); ++vs){
      _PolyedreInterne* le_voisin = le_noeud->polyedre(vs);
      /* test si la maille voisine située apres la maille */
      if(le_voisin->id()>le_polyedre->id()){
	if(_PolyedreInterne::comparer(le_polyedre, le_voisin)){
	  le_voisin->set_udata(le_polyedre);
	  invalider = true;
	}
      }
    }
  }

  for(iterator_volume vo = premier_volume();
      vo != dernier_volume(); ++vo) {
    for(iterator_polyedre pd = (*vo)->premier_polyedre();
	pd != (*vo)->dernier_polyedre(); ++pd) {
      if((*pd)->udata() != NULL)
	*pd = (_PolyedreInterne*)(*pd)->udata();
    }
  }

  if(invalider==true){
    Ensemble<_PolyedreInterne> new_polyedres;
    for(iterator_polyedre pd = premier_polyedre();
	pd != dernier_polyedre(); ++pd){
      if((*pd)->udata()!=NULL){
        (*pd)->detacher();
	(*pd)->ref_moins();
    m_composition_polyedres.nb_elements--;
    m_composition_polyedres.detail [(*pd)->nb_noeuds ( )]--;
	if((*pd)->orphelin()) delete (*pd);
      }
      else
	new_polyedres.ajouter(*pd);
    }
    m_polyedres = new_polyedres;
  }
}

void _MaillageInterne::supprimer_bras_inutilises()
{
	// initialise à 0 les udata par plus de sécurité
	for(iterator_bras e = premier_bras();
			e != dernier_bras(); ++e)
		(*e)->set_udata(NULL);

	// marque (via udata) les bras référencés par une ligne
	for(iterator_ligne li = premier_ligne();
			li != dernier_ligne(); ++li) {
		for(iterator_bras br = (*li)->premier_bras(); br != (*li)->dernier_bras(); ++br) {
			(*br)->set_udata((void*)1);
		}
	}

	// prépare la suppression des bras (déréférencement), constitution de la Liste avec ceux à conserver
	Ensemble<_BrasInterne> new_bras;
	for(iterator_bras br = premier_bras(); br != dernier_bras(); ++br){
		if((*br)->udata()==NULL){
			(*br)->detacher();
			(*br)->ref_moins();
			m_composition_bras.nb_elements--;
			m_composition_bras.detail [(*br)->nb_noeuds ( )]--;
			if((*br)->orphelin())
				delete (*br);
			else
				throw erreur ("Un bras normallement libéré est encore utilisé (par qui ?).");
		}
		else
			new_bras.ajouter(*br);
	}
	m_bras = new_bras;

	// remet les udata à 0
	for(iterator_bras e = premier_bras();
			e != dernier_bras(); ++e)
		(*e)->set_udata(NULL);
}

void _MaillageInterne::supprimer_polygones_inutilises()
{
	// initialise à 0 les udata par plus de sécurité
	for(iterator_polygone e = premier_polygone();
			e != dernier_polygone(); ++e)
		(*e)->set_udata(NULL);

	// marque (via udata) les polygones référencés par une surface
	for(iterator_surface su = premier_surface();
			su != dernier_surface(); ++su) {
		for(iterator_polygone pg = (*su)->premier_polygone();
				pg != (*su)->dernier_polygone(); ++pg) {
			(*pg)->set_udata((void*)1);
		}
	}

	// prépare la suppression des polygones (déréférencement), constitution de la Liste avec ceux à conserver
	Ensemble<_PolygoneInterne> new_polygones;
	for(iterator_polygone pg = premier_polygone();
			pg != dernier_polygone(); ++pg){
		if((*pg)->udata()==NULL){
			(*pg)->detacher();
			(*pg)->ref_moins();
			m_composition_polygones.nb_elements--;
			m_composition_polygones.detail [(*pg)->nb_noeuds ( )]--;
			if((*pg)->orphelin())
				delete (*pg);
			else
				throw erreur ("Un polygone normallement libéré est encore utilisé (par qui ?).");
		}
		else
			new_polygones.ajouter(*pg);
	}
	m_polygones = new_polygones;

	// remet les udata à 0
	for(iterator_polygone e = premier_polygone();
			e != dernier_polygone(); ++e)
		(*e)->set_udata(NULL);
}
//////////////////////////////////////////////////////////////////////////////
void _MaillageInterne::lire_structure (
					const string& fichier, format_t format, size_type num)
{
	vider();

	try
	{
		auto_ptr<_Reader>	reader (_Reader::create (this,fichier,format,num));
		if (0 == reader.get ( ))
			throw read_erreur("Impossible d'initialiser le module de lecture\n");

		reader->readStructure ( );
	}
	catch (erreur& err)
	{
		vider ( );
		throw read_erreur ("Erreur lors de la lecture du fichier " +
		                   fichier + "\n" + err.what ( ));
	}
}	// _MaillageInterne::lire_structure

//////////////////////////////////////////////////////////////////////////////
void _MaillageInterne::lire(const string& fichier, format_t format, size_type num)
{
	vider();

	try
	{
		auto_ptr<_Reader>	reader (_Reader::create (this,fichier,format,num));
		if (0 == reader.get ( ))
			throw read_erreur("Impossible d'initialiser le module de lecture\n");

		reader->readAll ( );
	}
	catch (erreur& err)
	{
		vider ( );
		throw read_erreur ("Erreur lors de la lecture du fichier " +
		                   fichier + "\n" + err.what ( ));
	}
}	// _MaillageInterne::lire

//////////////////////////////////////////////////////////////////////////////
void _MaillageInterne::ecrire(const string& fichier,
			      format_t format, size_type num, bool compacter)
{
  /*
  _Writer output_file(this, fichier, format);

  try{
    if (true == compacter)
        compacter_numerotation();
    output_file.writeAll();
  }
  */

  _Writer* output_file = _Writer::create(this, fichier, format, num);
  try{
    if (true == compacter)
        compacter_numerotation();
    format = _Reader::detectFormat(fichier);
    output_file->write(format);
  }
  catch(erreur& err){
    delete output_file;
    throw write_erreur("Erreur lors de l'ecriture du fichier " +
		       fichier + "\n" + err.what());
  }

  delete output_file;
}

Liste_attribut&  _MaillageInterne::get_pt_attache(att_loc pt_attach,
						  string  nom_groupe)
{
  switch(pt_attach)
      {
      case MAILLAGE:
	return this->att_maillage();
	break;

      case NOEUD:
	return this->att_noeuds();
	break;

      case BRAS:
	return this->att_bras();
	break;

      case POLYGONE:
	return this->att_polygones();
	break;

      case POLYEDRE:
	return this->att_polyedres();
	break;

      case NUAGE:
	return this->att_nuages();
	break;

      case LIGNE:
	return this->att_lignes();
	break;

      case SURFACE:
	return this->att_surfaces();
	break;

      case VOLUME:
	return this->att_volumes();
	break;

      case NOEUD_NUAGE:
	return this->nuage(nom_groupe)->att_noeuds();
	break;

      case NOEUD_LIGNE:
	return this->ligne(nom_groupe)->att_noeuds();
	break;

      case NOEUD_SURFACE:
	return this->surface(nom_groupe)->att_noeuds();
	break;

      case NOEUD_VOLUME:
	return this->volume(nom_groupe)->att_noeuds();
	break;

      case BRAS_LIGNE:
	return this->ligne(nom_groupe)->att_bras();
	break;

      case BRAS_SURFACE:
	return this->surface(nom_groupe)->att_bras();
	break;

      case BRAS_VOLUME:
	return this->volume(nom_groupe)->att_bras();
	break;

      case POLYGONE_SURFACE:
	return this->surface(nom_groupe)->att_polygones();
	break;

      case POLYGONE_VOLUME:
	return this->volume(nom_groupe)->att_polygones();
	break;

      case POLYEDRE_VOLUME:
	return this->volume(nom_groupe)->att_polyedres();
	break;

      default:
	fputs("Erreur concernant le point d'attache d'un attribut \n", stderr);
	break;
      }
}


void _MaillageInterne::ajouter_reader_callback (ReaderCallback* rc)
{
	if (0 != rc)
		m_readers_callback.push_back (rc);
}	// _MaillageInterne::ajouter_reader_callback


void _MaillageInterne::enlever_reader_callback (ReaderCallback* rc)
{
	if (0 != rc)
		for (vector<ReaderCallback*>::iterator it = m_readers_callback.begin( );
		     m_readers_callback.end ( ) != it; it++)
		{
			if (*it == rc)
			{
				m_readers_callback.erase (it);
				return;
			}
		}
}	// _MaillageInterne::enlever_reader_callback


void _MaillageInterne::debut_lecture (const string& file, size_type num)
{
	for (vector<ReaderCallback*>::iterator it = m_readers_callback.begin ( );
	     m_readers_callback.end ( ) != it; it++)
		(*it)->starting (file, num);
}	// _MaillageInterne::debut_lecture


void _MaillageInterne::fin_lecture (const string& file, size_type num)
{
	for (vector<ReaderCallback*>::iterator it = m_readers_callback.begin ( );
	     m_readers_callback.end ( ) != it; it++)
		(*it)->atCompletion (file, num);
}	// _MaillageInterne::fin_lecture


void _MaillageInterne::donnees_lues (
	const string& file, size_type num, ReaderCallback::Type type,
	const string& nom, double pourcent, bool& arreter)
{
	for (vector<ReaderCallback*>::iterator it = m_readers_callback.begin ( );
	     m_readers_callback.end ( ) != it; it++)
	{
		(*it)->dataRead (file, num, type, nom, pourcent, arreter);
		if (true == arreter)
			break;
	}	// for (vector<ReaderCallback*>::iterator ...
}	// _MaillageInterne::donnees_lues


void _MaillageInterne::remplacer_noeud (_NoeudInterne* n1, _NoeudInterne* n2)
{
	assert ((0 != n1) && (0 != n2));

	// MAJ des nuages : on met également à jour les connectivités. Un noeud
	// peut être présent en de multiples exemplaires, et se voir remplacé à
	// plusieurs reprises : les mailles doivent être informées de ces
	// remplacements successifs.
	for (iterator_nuage nu = premier_nuage ( ); nu != dernier_nuage ( ); nu++)
	{
		if (true == (*nu)->contenir (n1))
		{
			(*nu)->retirer (n1);
			if (false == (*nu)->contenir (n2))
				(*nu)->ajouter (n2);
		}	// if (true == (*nu)->contenir (n1))
	}	// for (iterator_nuage nu = premier_nuage ( ); ...

	// MAJ des éléments :
	for(_NoeudInterne::iterator_bras br = n1->premier_bras ( );
	    br != n1->dernier_bras ( ); br++)
	{
		(*br)->remplacer_noeud (n1, n2);
		n2->connecter (*br);
	}	// for(_NoeudInterne::iterator_bras br = n1->premier_bras ( );
	for(_NoeudInterne::iterator_polygone pg = n1->premier_polygone ( );
	    pg != n1->dernier_polygone ( ); pg++)
	{
		(*pg)->remplacer_noeud (n1, n2);
		n2->connecter (*pg);
	}	// for(_NoeudInterne::iterator_polygone pg = n1->premier_polygone ( );
	for(_NoeudInterne::iterator_polyedre pe = n1->premier_polyedre ( );
	    pe != n1->dernier_polyedre ( ); pe++)
	{
		(*pe)->remplacer_noeud (n1, n2);
		n2->connecter (*pe);
	}	// for(_NoeudInterne::iterator_polyedre pe = n1->premier_polyedre ( );
}	// _MaillageInterne::remplacer_noeud

END_NAMESPACE_LIMA
