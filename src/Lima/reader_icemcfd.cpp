#ifdef __INTERNE_ICEMCFD
#include <domain.h>
#include <element.h>

#include <time.h>
#include <values.h>

#include "LimaP/reader_icemcfd.h"

#include "LimaP/maillage_it.h"
#include "LimaP/noeud_it.h"
#include "LimaP/bras_it.h"
#include "LimaP/polygone_it.h"
#include "LimaP/polyedre_it.h"

#include "LimaP/nuage_it.h"
#include "LimaP/ligne_it.h"
#include "LimaP/surface_it.h"
#include "LimaP/volume_it.h"

#include <Lima/erreur.h>
#include "LimaP/fonction.h"
#include "LimaP/algorithme.h"
#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA
  
//////////////////////////////////////////////////////////////////////////////
_ReaderIcemcfd::_ReaderIcemcfd(_MaillageInterne* mesh, const string& nom_fichier, format_t format) : _Reader(mesh, nom_fichier, format) {
  
  m_file.ascii = true;
  m_file.open("r");
}


//////////////////////////////////////////////////////////////////////////////
_ReaderIcemcfd::~_ReaderIcemcfd(){
  m_file.close();
}


//////////////////////////////////////////////////////////////////////////////
void _ReaderIcemcfd::readAll()
{
	try
	{
		m_mai->debut_lecture (m_file.full_name, 0);
		lire (true);
		m_mai->fin_lecture (m_file.full_name, 0);
	}
	catch (const _Reader::LectureAnnulee&)
	{
		m_mai->vider ( );
		throw read_erreur ("Lecture annulée par l'application.");
	}
	catch (...)
	{
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////////
void _ReaderIcemcfd::readStructure() 
{
	try
	{
		m_mai->debut_lecture (m_file.full_name, 0);
		lire (false);
		m_mai->fin_lecture (m_file.full_name, 0);
	}
	catch (const _Reader::LectureAnnulee&)
	{
		m_mai->vider ( );
		throw read_erreur ("Lecture annulée par l'application.");
	}
	catch (...)
	{
		throw;
	}
}


void _ReaderIcemcfd::erreur()
{
  throw read_erreur(to_str(df_get_error()) + "\n");
}

// Lecture d'un fichier ICEM
void _ReaderIcemcfd::lire(bool loadData)
{
// Domaine non structure : n noeuds numerotes de maniere interne de 0 a n-1.
// Chaque noeud est compose de 3 coordonnees (reels double precision) et d'un
// nombre (unicite non verifiee).
// Les elements sont numerotes de n a n + m - 1, ou m est le nombre d'elements.
// Un element est compose d'un nombre, de l'id de ses proprietes (pid), et de la
// table de connectivite avec les noeuds. Le pid est associe a un groupe 
// d'elements qui ne sont pas necessairement de meme type. A chaque domaine
// de type BY_PID correspond un pid. Le pid le plus faible semble commencer a
// 1. Puis les pid semble se suivre de maniere incrementale. On a donc au plus
// autant de pid que de domaines. sachant que le premier indice commence a 1.
// Les elements consecutifs de meme type forme une section.

// ATTENTION : Lima++ 5.7. Le pid peut commencer a 0.

	int type, nnodes, nsections, n_domain;
	int i, j;
	size_t	edgeId = 0, polygoneId = 0, polyedreId = 0;		// Lima 5.5
  
	// Ouverture
	m_file.id = df_open ((char*)m_file.full_name.c_str(), MODE_READ, 
	                  UNSTRUCTURED_DOMAIN);

	// Dimension 3 pour les fichiers icemcfd
	m_mai->dimension(D3);
	int		gmtDate	= -1;
	if (0 > df_date (m_file.id, &gmtDate))
		erreur ( );
	time_t	longDate	= (time_t)gmtDate;
	struct tm*	date	= gmtime (&longDate);
	m_mai->date (asctime (date));

	type = -1;
	if (0 > df_type (m_file.id, &type)) 
		erreur ( );
	if(type != UNSTRUCTURED_DOMAIN)
		throw read_erreur("Seul le type UNSTRUCTURED_DOMAIN est supporte \n");

	// Lecture des noeuds :
	Composition	composition;
	if (0 > df_n_nodes (m_file.id, &nnodes)) 
		erreur ( );
	composition.nb_elements	= nnodes;
	m_mai->composition_noeuds (composition);
	bool	cancel	= false;
	if (false != loadData)
	{
		composition.nb_elements	= 0;	// MAJ par m_mai->ajouter (noeud)
		m_mai->composition_noeuds (composition);
		m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "", 0.,
		                     cancel);
		if (true == cancel) throw _Reader::LectureAnnulee ( );
		double*	pnt = new double [3 * nnodes];
		if (0 > df_unstruct_read_nodes (m_file.id, 0, nnodes, pnt)) 
			erreur ( );
		for (i = 0; i < nnodes; ++i)
// CP : Lima++ 5.5.0 : correction de bogue
			m_mai->ajouter (
				_NoeudInterne::create (i + 1, pnt [3*i], pnt [3*i+1], pnt [3*i+2]));
//				_NoeudInterne::create (i, pnt [3*i], pnt [3*i+1], pnt [3*i+2]));
		delete [] pnt;	pnt	= 0;
		m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "", 
		                     100., cancel);
		if (true == cancel) throw _Reader::LectureAnnulee ( );
	}	// if (false != loadData)

	// Lecture des domaines :
	if (0 > df_n_domain (m_file.id, &n_domain)) 
		erreur ( );
//printf ("%s %d NB SUBDOMAINS = %d\n", __FILE__, __LINE__, n_domain);

	// Lima++ 5.9.3 : Recherche des pid min et max pour allocation de
	// noms_grp. En effet, pid_max peut etre strictement superieur a n_domain.
	// On prend le plus grand des deux.
	int			pid_min	= MAXINT, pid_max		= -1;
	for (i = 0; i < n_domain; ++i)
	{
		type			= -1;
		if (0 > df_domain_type (m_file.id, i, &type)) 
			erreur ( );

		if (BY_PID == type)
		{
			// On recupere le pid du domaine :
			int	pid	= -1;
			if (0 > df_get_pid_of_domain (m_file.id, i, &pid))
				erreur ( );
			pid_max	= pid_max > pid ? pid_max : pid;
			pid_min	= pid_min < pid ? pid_min : pid;
		}	// if (BY_PID == type)
	}	// for (i = 0; i < n_domain; ++i)
	int	noms_grp_size	= pid_max > n_domain ? pid_max + 1 : n_domain + 1;

	// On referencera les noms de groupes par leur pid (1er indice : 1) :
//	int			pid_min	= MAXINT, pid_max		= -1;
//	char**		noms_grp	= new char* [n_domain + 1];
	char**		noms_grp	= new char* [noms_grp_size];
//	for (i = 0; i <= n_domain; i++)
	for (i = 0; i < noms_grp_size; i++)
		noms_grp [i]	= 0;
	// On compte ici les sous-domaines reellement charges :
	int	pidCount	= 0;	// CP DEBUG lima 5.5
 
	for (i = 0; i < n_domain; ++i)
	{
		type			= -1;
		if (0 > df_domain_type (m_file.id, i, &type)) 
			erreur ( );

		// Les domaines de type BY_PID sont ceux realises par l'operateur
		// sous ICEM CFD. Les autres sont ceux generes automatiquement par 
		// l'outil. Ils sont neanmoins conserves car ils contiennent des 
		// informations utiles sur la structure.
		// Ces domaines definissent des lignes, nuages, surfaces, volumes.
		if (BY_PID == type)
		{
			// On recupere le pid du domaine :
			int	pid	= -1;
			if (0 > df_get_pid_of_domain (m_file.id, i, &pid))
				erreur ( );
			pid_max	= pid_max > pid ? pid_max : pid;
			pid_min	= pid_min < pid ? pid_min : pid;
//			if (0 > df_domain_name (m_file.id, i, (const char**)(noms_grp + pid))) 
//				erreur ( );
// CP : Lima++ 5.5.0 : correction de bogue. pids non forcement contigus, et ne
// commencent pas forcement a 1 pour ceux charges.
// CP : Lima++ 5.7.0. Les pids peuvent commencer a 0 ! Si le 1er pid est nul, on
// le laisse a zero, sinon on commence a 1.

			if (0 != pid)	// CP : Lima++ 5.7.0
				pidCount++;
			// CP, Version 5.9.2 :
//			if (0 > df_domain_name (m_file.id, i, (const char**)(noms_grp + pidCount))) 
			if (0 > df_domain_name (m_file.id, i, (const char**)(noms_grp + pid))) 
				erreur ( );
		}	// if (BY_PID == type)
	}	// for (j = 0, i = 0; i < n_domain; ++i)
  
	// Sections
	if (0 > df_n_sections (m_file.id, &nsections)) 
		erreur ( );
	// Attention : un domaine peut contenir des elements de dimensions 
	// differentes (ex : des bras et des polygones) => on peut avoir plusieurs
	// groupes de dimensions differentes et de meme nom (ex : nuage et surface
	// de meme nom, cf. methodes ajouter).
	for (i = 0; i < nsections; ++i)
	{
		int	start, end, type;

		if (0 > df_section_info (m_file.id, i, &start, &end, &type)) 
			erreur ( );

		int 	nelements	= end - start + 1;
		int		npe			= df_nodes_per_element (type);
		// La composition en noeuds ne semble pas toujours tres exacte.
		// Le nombre de noeuds lus initialement est pas contre lui OK.
		if (NODE != type)
			if (false == loadData)	// sinon MAJ par m_mai->ajouter (entite)
				updateComposition (type, nelements);
		if (0 == npe)
			erreur ( );
		int*	data		= new int [nelements * (npe+1)];

		if (0 > df_read_elements (m_file.id, start, nelements, data)) 
			erreur ( );

		if (false == loadData)
		{	// Ne pas lire les donnees, juste reconstituer la structure du
			// maillage.
			for (j = 0; j < nelements; ++j)
			{
				const int	pos		= j * (npe + 1);
				const int	grId	= data [pos];
				if ((grId < pid_min) || (grId > pid_max))
					continue;	// Usage interne icemcfd

				ajouterGroupe (type, noms_grp [grId]);
			}	// for (j = 0; j < nelements; ++j)

			delete [] data;
			continue;
		}	// if (false == loadData)

		switch (type)
		{
			case NODE :
				for (j = 0; j < nelements; ++j)
				{
					const int		pos		= j * (npe + 1);
					const int		grId	= data [pos];
					if ((grId < pid_min) || (grId > pid_max))
						continue;	// Usage interne icemcfd
					_NoeudInterne*	n	= m_mai->noeud (data [pos + 1]);
					ajouter (n, noms_grp [grId]);
				}	// for (j = 0; j < nelements; ++j)
				break;
			case BAR_2 :
				for (j = 0; j < nelements; ++j)
				{
					const int		pos		= j * (npe + 1);
					const int		grId	= data [pos];
					_NoeudInterne*	n1 = m_mai->noeud (data [pos + 1]);
					_NoeudInterne*	n2 = m_mai->noeud (data [pos + 2]);
					_BrasInterne*	br = 
									_BrasInterne::create (++edgeId, n1, n2);
					m_mai->ajouter (br);
					if ((grId >= pid_min) && (grId <= pid_max))
						ajouter (br, noms_grp [grId]);
				}	// for (j = 0; j < nelements; ++j)
				break;
			case TRI_3 :
				for (j = 0; j < nelements; ++j)
				{
					const int		pos		= j * (npe + 1);
					const int		grId	= data [pos];
					_NoeudInterne*		n1 = m_mai->noeud (data [pos + 1]);
					_NoeudInterne*		n2 = m_mai->noeud (data [pos + 2]);
					_NoeudInterne*		n3 = m_mai->noeud (data [pos + 3]);
					_PolygoneInterne*	pg = 
							_PolygoneInterne::create (++polygoneId, n1, n2, n3);
					m_mai->ajouter (pg);
					if ((grId >= pid_min) && (grId <= pid_max))
						ajouter (pg, noms_grp [grId]);
				}	// for (j = 0; j < nelements; ++j)
				break;
			case QUAD_4:
				for (j = 0; j < nelements; ++j)
				{
					const int		pos		= j * (npe + 1);
					const int		grId	= data [pos];
					_NoeudInterne*		n1 = m_mai->noeud (data [pos + 1]);
					_NoeudInterne*		n2 = m_mai->noeud (data [pos + 2]);
					_NoeudInterne*		n3 = m_mai->noeud (data [pos + 3]);
					_NoeudInterne*		n4 = m_mai->noeud (data [pos + 4]);
					_PolygoneInterne*	pg = 
						_PolygoneInterne::create (++polygoneId, n1, n2, n3, n4);
					m_mai->ajouter (pg);
					if ((grId >= pid_min) && (grId <= pid_max))
						ajouter (pg, noms_grp [grId]);
				}	// for (j = 0; j < nelements; ++j)
				break;
			case TETRA_4 :
				for (j = 0; j < nelements; ++j)
				{
					const int		pos		= j * (npe + 1);
					const int		grId	= data [pos];
					_NoeudInterne*	n1 = m_mai->noeud (data[pos + 1]);
					_NoeudInterne*	n2 = m_mai->noeud (data[pos + 2]);
					_NoeudInterne*	n3 = m_mai->noeud (data[pos + 3]);
					_NoeudInterne*	n4 = m_mai->noeud (data[pos + 4]);
					_PolyedreInterne* pd = 
						_PolyedreInterne::create (++polyedreId, n1, n2, n3, n4);
					m_mai->ajouter (pd);
					if ((grId >= pid_min) && (grId <= pid_max))
						ajouter (pd, noms_grp [grId]);
				}	// for (j = 0; j < nelements; ++j)
				break;
			case PYRA_5 :
				for (j = 0; j < nelements; ++j)
				{
					const int		pos		= j * (npe + 1);
					const int		grId	= data [pos];
					_NoeudInterne*	n1 = m_mai->noeud (data [pos + 1]);
					_NoeudInterne*	n2 = m_mai->noeud (data [pos + 2]);
					_NoeudInterne*	n3 = m_mai->noeud (data [pos + 3]);
					_NoeudInterne*	n4 = m_mai->noeud (data [pos + 4]);
					_NoeudInterne*	n5 = m_mai->noeud (data [pos + 5]);
					_PolyedreInterne* pd = 
					_PolyedreInterne::create (++polyedreId, n1, n2, n3, n4, n5);
					m_mai->ajouter (pd);
					if ((grId >= pid_min) && (grId <= pid_max))
						ajouter (pd, noms_grp [grId]);
				}	// for (j = 0; j < nelements; ++j)
				break;
			case PENTA_6 :
				for (j = 0; j < nelements; ++j)
				{
					const int		pos		= j * (npe + 1);
					const int		grId	= data [pos];
					_NoeudInterne*	n1 = m_mai->noeud (data [pos + 1]);
					_NoeudInterne*	n2 = m_mai->noeud (data [pos + 2]);
					_NoeudInterne*	n3 = m_mai->noeud (data [pos + 3]);
					_NoeudInterne*	n4 = m_mai->noeud (data [pos + 4]);
					_NoeudInterne*	n5 = m_mai->noeud (data [pos + 5]);
					_NoeudInterne*	n6 = m_mai->noeud (data [pos + 6]);
					_PolyedreInterne* pd = 
				_PolyedreInterne::create (++polyedreId, n1, n2, n3, n4, n5, n6);
					m_mai->ajouter (pd);
					if ((grId >= pid_min) && (grId <= pid_max))
						ajouter (pd, noms_grp [grId]);
				}	// for (j = 0; j < nelements; ++j)
				break;
			case HEXA_8 :
				for (j = 0; j < nelements; ++j)
				{
					const int		pos		= j * (npe + 1);
					const int		grId	= data [pos];
					_NoeudInterne*	n1 = m_mai->noeud (data [pos + 1]);
					_NoeudInterne*	n2 = m_mai->noeud (data [pos + 2]);
					_NoeudInterne*	n3 = m_mai->noeud (data [pos + 3]);
					_NoeudInterne*	n4 = m_mai->noeud (data [pos + 4]);
					_NoeudInterne*	n5 = m_mai->noeud (data [pos + 5]);
					_NoeudInterne*	n6 = m_mai->noeud (data [pos + 6]);
					_NoeudInterne*	n7 = m_mai->noeud (data [pos + 7]);
					_NoeudInterne*	n8 = m_mai->noeud (data [pos + 8]);
					_PolyedreInterne* pd = 
		_PolyedreInterne::create(++polyedreId, n1, n2, n3, n4, n5, n6, n7, n8);
					m_mai->ajouter (pd);
					if ((grId >= pid_min) && (grId <= pid_max))
						ajouter (pd, noms_grp [grId]);
				}	// for (j = 0; j < nelements; ++j)
				break;
			default :
				throw read_erreur("Type d'element non supporte \n");
		}	// switch (type)
    
		delete [] data;
		m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "", 
		                     100. * ((double)i + 1.) / nsections, cancel);
		if (true == cancel) throw _Reader::LectureAnnulee ( );
	}	// for (i = 0; i < nsections; ++i)

	if (0 > df_close (m_file.id)) 
		erreur ( );
	m_file.id	= 0;

	delete [] noms_grp;
}	// _ReaderIcemcfd::lire


void _ReaderIcemcfd::ajouter (_NoeudInterne* noeud, const string& nom_nuage)
{
	if (0 == noeud)
		return;

	_NuageInterne*	nu = 0;
	try
	{
		nu	= m_mai->nuage (nom_nuage);
	}
	catch (...)
	{
		nu	= new _NuageInterne (nom_nuage);
		m_mai->ajouter (nu);
	}

	nu->ajouter (noeud);
}	// _ReaderIcemcfd::ajouter (_NoeudInterne* noeud, const string& nom_nuage)


void _ReaderIcemcfd::ajouter (_BrasInterne* bras, const string& nom_ligne)
{
	if (0 == bras)
		return;

	_LigneInterne*	li	= 0;
	try
	{
		li	= m_mai->ligne (nom_ligne);
	}
	catch (...)
	{
		li	= new _LigneInterne (nom_ligne);
		m_mai->ajouter (li);
	}

	li->ajouter(bras);
}	// _ReaderIcemcfd::ajouter (_BrasInterne* bras, const string& nom_ligne)


void _ReaderIcemcfd::ajouter (_PolygoneInterne* polygone, 
                              const string& nom_surface)
{
	if (0 == polygone)
		return;

	_SurfaceInterne*	su	= 0;
	try
	{
		su	= m_mai->surface (nom_surface);
	}
	catch (...)
	{
		su	= new _SurfaceInterne (nom_surface);
		m_mai->ajouter (su);
	}

	su->ajouter(polygone);
}	// _ReaderIcemcfd::ajouter (_PolygoneInterne* polygone, ...


void _ReaderIcemcfd::ajouter (_PolyedreInterne* polyedre, 
                              const string& nom_volume)
{
	if (0 == polyedre)
		return;

	_VolumeInterne*	v	= 0;
	try
	{
		v	= m_mai->volume (nom_volume);
	}
	catch (...)
	{
		v	= new _VolumeInterne (nom_volume);
		m_mai->ajouter (v);
	}

	v->ajouter(polyedre);
}	// _ReaderIcemcfd::ajouter (_PolyedreInterne* polyedre, ...


void _ReaderIcemcfd::updateComposition (int type, int nb)
{
	int		npe			= df_nodes_per_element (type);

	switch (type)
	{
		case	NODE	:
		{
			Composition	nodes	= m_mai->composition_noeuds ( );
			nodes.nb_elements	+= nb;
			nodes.detail [0]	+= nb;
			nodes.renseigne		= true;
			m_mai->composition_noeuds (nodes);
			break;
		}
		case	BAR_2	:
		case	BAR_3	:
		{
			Composition	edges	= m_mai->composition_bras ( );
			edges.nb_elements	+= nb;
			edges.detail [npe]	+= nb;
			edges.renseigne		= true;
			m_mai->composition_bras (edges);
			break;
		}
		case	TRI_3		:
		case	TRI_3_X		:
		case	TRI_6		:
		case	TRI_6_X		:
		case	QUAD_4		:
		case	QUAD_4_X	:
		case	QUAD_8		:
		case	QUAD_8_X	:
		{
			Composition	polygons	= m_mai->composition_polygones ( );
			polygons.nb_elements	+= nb;
			polygons.detail [npe]	+= nb;
			polygons.renseigne		= true;
			m_mai->composition_polygones (polygons);
			break;
		}
		case	TETRA_4		:
		case	TETRA_10	:
		case	HEXA_8		:
		case	HEXA_20		:
		case	HEXA_27		:
		case	PENTA_6		:
		case	PENTA_15	:
		case	PENTA_18	:
		case	PYRA_5		:
		case	PYRA_13		:
		case	PYRA_14		:
		{
			Composition	polyedrons	= m_mai->composition_polyedres ( );
			polyedrons.nb_elements	+= nb;
			polyedrons.detail [npe]	+= nb;
			polyedrons.renseigne		= true;
			m_mai->composition_polyedres (polyedrons);
			break;
		}
		default	:
			throw read_erreur ("_ReaderIcemcfd::updateComposition : type non supporté.");
	}	// switch (type)
}	// _ReaderIcemcfd::updateComposition


void _ReaderIcemcfd::ajouterGroupe (int type, const string& name)
{
	switch (type)
	{
		case	NODE	:
			try
			{
				m_mai->nuage (name);
			}
			catch (...)
			{
				_NuageInterne*	nodeset	= new _NuageInterne (name);
				m_mai->ajouter (nodeset);
			}
			break;
		case	BAR_2	:
		case	BAR_3	:
			try
			{
				m_mai->ligne (name);
			}
			catch (...)
			{
				_LigneInterne*	line	= new _LigneInterne (name);
				m_mai->ajouter (line);
			}
			break;
			break;
		case	TRI_3		:
		case	TRI_3_X		:
		case	TRI_6		:
		case	TRI_6_X		:
		case	QUAD_4		:
		case	QUAD_4_X	:
		case	QUAD_8		:
		case	QUAD_8_X	:
			try
			{
				m_mai->surface (name);
			}
			catch (...)
			{
				_SurfaceInterne*	surface	= new _SurfaceInterne (name);
				m_mai->ajouter (surface);
			}
			break;
		case	TETRA_4		:
		case	TETRA_10	:
		case	HEXA_8		:
		case	HEXA_20		:
		case	HEXA_27		:
		case	PENTA_6		:
		case	PENTA_15	:
		case	PENTA_18	:
		case	PYRA_5		:
		case	PYRA_13		:
		case	PYRA_14		:
			try
			{
				m_mai->volume (name);
			}
			catch (...)
			{
				_VolumeInterne*	volume	= new _VolumeInterne (name);
				m_mai->ajouter (volume);
			}
			break;
		default	:
			throw read_erreur ("_ReaderIcemcfd::ajouterGroupe : type non supporté.");
	}	// switch (type)
}	// _ReaderIcemcfd::ajouterGroupe


END_NAMESPACE_LIMA

#endif
