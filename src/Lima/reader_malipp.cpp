#ifdef __INTERNE_MALIPP
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_malipp.C
 Cree le :
 Derniere modification le : Wed Sep 10 09:55:17 MEST 2003 (C. Pignerol)
 Auteur  : O. Bertrand
******************************************************************************/

/*!
  \file reader_malipp.C
  \brief Interface de lecture / ecriture pour les fichiers MALI++ (.mli/.mliz)
*/

/* REMARQUES :
   -----------

   - Cette version prend en compte le fait qu'un maillage puisse etre
   ecrase par un autre plus petit. Par exemple, un attribut est associe a chaque
   dataset donnant le nombre d'element a prendre en compte.

   - ATTENTION : pour les bras et mailles on ne sauvegarde pas les identifiants
   des noeuds, mais les identifiants -1 afin de faire une lecture optimisee
   par indexation dans le cas d'un compactage de la numerotation, ce qui
   concerne pratiquement tous les cas. Il en va de meme pour les nuages,
   lignes, surfaces et volumes.
*/

/* CP : ID_TYPE -> H5T_NATIVE_UINT32 : voir hdf5_size.h */

#include "LimaP/myio.h"
#include "LimaP/misc.h"
#include "LimaP/MutableString.h"
#include "LimaP/hdf_helper.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "LimaP/maillage_it.h"
#include "LimaP/reader_malipp.h"
#include "LimaP/liste_fichier.h"
#include "LimaP/hdf5_size.h"
#include "LimaP/attribut_type.h"

#include <iostream>
#ifndef HDF5_1_4_3
#include "LimaP/using_it.h"
#endif	// HDF5_1_4_3

#if defined(_IBMR2) || defined(__GNUC__) || defined(CRAY) || defined(INCLUDE_TEMPLATE)
#include "LimaP/attribut_type_t.h"
#endif

//#ifndef H5_NO_NAMESPACE
//using namespace H5;
//#endif  // H5_NO_NAMESPACE
using namespace HDF145;


BEGIN_NAMESPACE_LIMA


//////////////////////////////////////////////////////////////////////////////
/* ! Retourne la taille d'un bloc pour les opérations de lecture/écriture.
     count est le nombre de données à lire ou écrire, elementSize leur
     taille en octets. */
static size_t computeChunkSize (size_t count, size_t elementSize);


//////////////////////////////////////////////////////////////////////////////
//! Nom du champ contenant le nom d'un attribut dans la structure HDF.
const string	HDFAttributeDescriptorType::m_name ("name");

//////////////////////////////////////////////////////////////////////////////
//! Nom du champ contenant le type d'un attribut dans la structure HDF.
const string	HDFAttributeDescriptorType::m_type ("type");

//////////////////////////////////////////////////////////////////////////////
/*! Nom du champ contenant le nombre d'éléments d' d'un attribut dans la
    structure HDF. */
const string	HDFAttributeDescriptorType::m_size ("size");


//////////////////////////////////////////////////////////////////////////////
/*! Ouvre en lecture le fichier nom_fichier et ouvre le groupe correspondant au
   maillage num. */
_ReaderMalipp::_ReaderMalipp (_MaillageInterne* mesh, const string& nom_fichier,
                              format_t format, size_type num)
	: _Reader (mesh, nom_fichier, format, num),
	  m_fileName (nom_fichier), m_meshNum (num), m_reader (nom_fichier, num)
{
	if(!_ListeFichier::donneInstance (m_fileName, m_meshNum, "rt"))
		throw read_erreur("Le maillage " + to_str (m_meshNum) + " du fichier "+
                          m_fileName + " est deja ouvert en lecture \n");

	m_mai->titre (m_reader.titre ( ));
	m_mai->date (m_reader.date ( ));
	m_mai->dimension (m_reader.dimension ( ));
	m_mai->type_geometrie (m_reader.geometrie ( ));
	m_mai->type_coordonnee (m_reader.systeme_coordonnees ( ));
	m_mai->unite_angle (m_reader.unite_angle ( ));
	m_mai->unite_longueur (m_reader.unite_longueur ( ));
}	// _ReaderMalipp::_ReaderMalipp


//////////////////////////////////////////////////////////////////////////////
//! Ferme le fichier HDF.
_ReaderMalipp::~_ReaderMalipp ( )
{
	_ListeFichier::libereInstance (m_fileName, m_meshNum, "rt");
}	// _ReaderMalipp::~_ReaderMalipp


//////////////////////////////////////////////////////////////////////////////
//! Charge l'ensemble du maillage si cela n'a pas déjà été fait.
void _ReaderMalipp::readAll ( )
{
	try
	{
		m_mai->debut_lecture (m_fileName, m_meshNum);
		lire (true);
		m_mai->fin_lecture (m_fileName, m_meshNum);
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
}	// _ReaderMalipp::readAll

//////////////////////////////////////////////////////////////////////////////
void _ReaderMalipp::readStructure ( )
{
	try
	{
		m_mai->debut_lecture (m_fileName, m_meshNum);
		lire (false);
		m_mai->fin_lecture (m_fileName, m_meshNum);
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
}	// _ReaderMalipp::readStructure


//////////////////////////////////////////////////////////////////////////////
//! Lecture d'un fichier Malipp
void _ReaderMalipp::lire (bool loadData)
{
	bool			throwExc	= false;
	MutableString	errorMsg;
	errorMsg << "Impossible de lire le fichier " << m_fileName << " : ";

	try
	{
		readNode (loadData);
		readEdge (loadData);
		readCell2D (loadData);
		readCell3D (loadData);
		readNodeset (loadData);
		readLine (loadData);
		readSurface (loadData);
		readVolume (loadData);

		if (false == loadData)
			return;

		// Lecture des attributs des entites de base
		m_mai->init_att();
		bool	cancel	= false;
		m_mai->donnees_lues (m_fileName, m_meshNum,
		                     ReaderCallback::ATTR_MAILLAGE, "", 0., cancel);
		if (true == cancel) throw _Reader::LectureAnnulee ( );
		readMeshAttr ( );
	}
	catch (const Exception& hdfExc)
	{
		throwExc	= true;
		errorMsg << hdfExc.getDetailMsg ( );
	}
	catch (const read_erreur& limaExc)
	{
		throwExc	= true;
		errorMsg << limaExc.what ( );
	}
	catch (const exception& exc)
	{
		throwExc	= true;
		errorMsg << exc.what ( );
	}
	catch (const _Reader::LectureAnnulee&)
	{
		throw;
	}
	catch (...)
	{
		throwExc	= true;
		errorMsg << "erreur non renseignee.";
	}

	if (true == throwExc)
		throw read_erreur (errorMsg.str ( ));
}	// _ReaderMalipp::lire


//////////////////////////////////////////////////////////////////////////////
//! Lecture des donnees sur les noeuds
void _ReaderMalipp::readNode (bool loadData)
{
	bool	cancel	= false;
	m_mai->donnees_lues (m_fileName, m_meshNum, ReaderCallback::NOEUD, "", 0.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );

	bool			throwError	= false;
	MutableString	errorMsg;
	errorMsg << "Lecture des noeuds impossible : ";

	Composition	composition	= m_reader.composition_noeuds ( );
	size_type	nodeNumber	= composition.nb_elements;
	m_mai->composition_noeuds (composition);

	if ((0 == nodeNumber) || (false == loadData))
		return;
	composition.renseigne	= true;
	composition.nb_elements	= 0;
	//	m_mai->composition_noeuds (composition);	// v 6.6.0
	Composition	c;	// v 6.6.0
	c.renseigne	= true;	// v 6.6.0
	m_mai->composition_noeuds (c);	// v 6.6.0

	// Lecture des noeuds :
	const size_t	chunkSize	= computeChunkSize(nodeNumber, sizeof (double));
	double*		coords_buffer	= m_reader.allouer_tampon_coords (chunkSize);
	size_type*	id_buffer		= m_reader.allouer_tampon_ids (chunkSize);
	size_type	nodeCount		= 0;
	try
	{
		if ((0 == coords_buffer) || (0 == id_buffer))
		{
			delete [] coords_buffer;
			delete [] id_buffer;
			throw read_erreur ("Echec d'une allocation mémoire.");
		}	// if ((0 == coords_buffer) || (0 == id_buffer))

		for (size_t pos = 0; pos < nodeNumber; )
		{
			const size_type	count	=
				m_reader.lire_noeuds (pos, chunkSize, id_buffer, coords_buffer);

			for (size_type n = 0; n < count; n++, nodeCount++)
			{
				switch (m_reader.dimension ( ))
				{
					case D1	:
						m_mai->ajouter (_NoeudInterne::create (
									id_buffer [n], coords_buffer [n], 0., 0.));
						break;
					case D2	:
						m_mai->ajouter (
							_NoeudInterne::create (id_buffer [n],
								coords_buffer [2*n], coords_buffer [2*n+1],0.));
						break;
					case D3	:
						m_mai->ajouter (
							_NoeudInterne::create (id_buffer [n],
								coords_buffer [3*n], coords_buffer [3*n + 1],
								coords_buffer [3*n + 2]));
						break;
				}	// switch (m_mai->dimension ( ))
			}	// for (size_type n = 0; n < count; n++, nodeCount++)

			pos	+= count;
			double	percent	= ((double)pos) * 100. / nodeNumber;
			m_mai->donnees_lues (m_fileName, m_meshNum, ReaderCallback::NOEUD,
			                     "", percent, cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
		}	// for (size_t pos = 0; pos < nodeNumber; )
	}
	catch (const erreur& err)
	{
		throwError	= true;
		errorMsg << err.what ( );
	}
	catch (const exception& exc)
	{
		throwError	= true;
		errorMsg << exc.what ( );
	}
	catch (const _Reader::LectureAnnulee&)
	{	// But du jeu : sortir de la boucle et faire les desallocations.
	}
	catch (...)
	{
		throwError	= true;
		errorMsg << "erreur non renseignee.";
	}

	delete [] coords_buffer;
	delete [] id_buffer;

	if (true == throwError)
		throw read_erreur (errorMsg.str ( ));
}	// _ReaderMalipp::readNode

//////////////////////////////////////////////////////////////////////////////
//! Lecture des donnees sur les bras
void _ReaderMalipp::readEdge (bool loadData)
{
	bool	cancel	= false;
	m_mai->donnees_lues (m_fileName, m_meshNum, ReaderCallback::BRAS, "", 0.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );

	bool			throwError	= false;
	MutableString	errorMsg;
	errorMsg << "Lecture des bras impossible : ";

	Composition		composition		= m_reader.composition_bras ( );
	const size_type	edgeNumber		= composition.nb_elements;
	m_mai->composition_bras (composition);

	if ((0 == edgeNumber) || (false == loadData))
		return;
	composition.renseigne	= true;
	composition.nb_elements	= 0;
	//	m_mai->composition_bras (composition);	// v 6.6.0
	Composition	c;	// v 6.6.0
	c.renseigne	= true;	// v 6.6.0
	m_mai->composition_bras (c);	// v 6.6.0

	// Lecture des ids des noeuds des bras :
	const size_t	chunkSize	=
						computeChunkSize(edgeNumber, 2 * sizeof(id_type));
	size_type*		ids			= m_reader.allouer_tampon_bras (chunkSize);
	try
	{
		if (0 == ids)
			throw read_erreur ("Echec d'une allocation mémoire.");

		for (size_type pos = 0; pos * chunkSize < edgeNumber; pos++)
		{
			const size_type	count	=
					m_reader.lire_bras (pos * chunkSize, chunkSize, ids);
			size_type*	ptr		= ids;

			// Ajout des bras au maillage :
			for (size_type ielm = 0; ielm < count; ielm++)
			{
				size_type	id	= *ptr;	ptr++;
				size_type	id1	= *ptr;	ptr++;
				size_type	id2	= *ptr;	ptr++;
				m_mai->ajouter (_BrasInterne::create (
					id, m_mai->noeud_id (id1), m_mai->noeud_id (id2)));
			}

			double	percent	= (pos * chunkSize + count) * 100. / edgeNumber;
			m_mai->donnees_lues (m_fileName, m_meshNum, ReaderCallback::BRAS,
			                     "", percent, cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
		}	// for (size_t pos = 0; pos * chunkSize < edgeNumber; pos++)
	}
	catch (const erreur& err)
	{
		throwError	= true;
		errorMsg << err.what ( );
	}
	catch (const exception& exc)
	{
		throwError	= true;
		errorMsg << exc.what ( );
	}
	catch (const _Reader::LectureAnnulee&)
	{	// Sortir du for + faire les delete
	}
	catch (...)
	{
		throwError	= true;
		errorMsg << "erreur non renseignee.";
	}

	delete [] ids;

	if (true == throwError)
		throw read_erreur (errorMsg.str ( ));
}	// _ReaderMalipp::readEdge


//////////////////////////////////////////////////////////////////////////////
//! Lecture des donnees sur les polygones
void _ReaderMalipp::readCell2D (bool loadData)
{
	bool	cancel	= false;
	m_mai->donnees_lues (m_fileName, m_meshNum, ReaderCallback::POLYGONE, "",
	                     0., cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );

	bool			throwError	= false;
	MutableString	errorMsg;
	errorMsg << "Lecture des polygones impossible : ";

	Composition		composition	= m_reader.composition_polygones ( );
	const size_type	cellNumber	= composition.nb_elements;
	m_mai->composition_polygones (composition);

	if ((0 == cellNumber) || (false == loadData))
		return;
	composition.renseigne	= true;
	composition.nb_elements	= 0;
	//	m_mai->composition_polygones (composition);	// v 6.6.0
	Composition	c;	// v 6.6.0
	c.renseigne	= true;	// v 6.6.0
	m_mai->composition_polygones (c);	// v 6.6.0

	// Lecture des ids des noeuds des polygones et des polygones :
	const size_t	chunkSize	=
				computeChunkSize(cellNumber, sizeof(object_type));
	size_type*		ids			= m_reader.allouer_tampon_polygones (chunkSize);
	try
	{
		if (0 == ids)
			throw read_erreur ("Echec d'une allocation mémoire.");

		for (size_type pos = 0; pos * chunkSize < cellNumber; pos++)
		{
			const size_type	count	=
				m_reader.lire_polygones (pos * chunkSize, chunkSize, ids);
			size_type*	ptr		= ids;

			// Ajout des polygones au maillage :
			for (size_type ielm = 0; ielm < count; ielm++)
			{
				size_type	id	= *ptr;		ptr++;
				const size_type	nodeCount	= *ptr;
				switch (nodeCount)
				{
					case 3	:
						m_mai->ajouter (
								_PolygoneInterne::create (id,
								m_mai->noeud_id (ptr [1]),
								m_mai->noeud_id (ptr [2]),
								m_mai->noeud_id (ptr [3])));
						break;
					case 4	:
						m_mai->ajouter (
								_PolygoneInterne::create (id,
								m_mai->noeud_id (ptr [1]),
								m_mai->noeud_id (ptr [2]),
								m_mai->noeud_id (ptr [3]),
								m_mai->noeud_id (ptr [4])));
						break;
					case 5	:
						m_mai->ajouter (
								_PolygoneInterne::create (id,
								m_mai->noeud_id (ptr [1]),
								m_mai->noeud_id (ptr [2]),
								m_mai->noeud_id (ptr [3]),
								m_mai->noeud_id (ptr [4]),
								m_mai->noeud_id (ptr [5])));
						break;
					case 6	:
							m_mai->ajouter(
								_PolygoneInterne::create (id,
								m_mai->noeud_id (ptr [1]),
								m_mai->noeud_id (ptr [2]),
								m_mai->noeud_id (ptr [3]),
								m_mai->noeud_id (ptr [4]),
								m_mai->noeud_id (ptr [5]),
								m_mai->noeud_id (ptr [6])));
						break;
					default	:
						throw read_erreur ("Erreur interne.");
				}	// switch (nodeCount)
				ptr	+= nodeCount + 1;
			}	// for (size_type ielm = 0; ielm < count; ielm++)

			double	percent	= (pos * chunkSize + count) * 100. / cellNumber;;
			m_mai->donnees_lues (m_fileName, m_meshNum,
			                     ReaderCallback::POLYGONE, "", percent, cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
		}	// for (size_type pos = 0; pos * chunkSize < cellNumber; pos++)
	}
	catch (const erreur& err)
	{
		throwError	= true;
		errorMsg << err.what ( );
	}
	catch (const exception& exc)
	{
		throwError	= true;
		errorMsg << exc.what ( );
	}
	catch (const _Reader::LectureAnnulee&)
	{	// Sortir du for + desallocations
	}
	catch (...)
	{
		throwError	= true;
		errorMsg << "erreur non renseignee.";
	}

	delete [] ids;

	if (true == throwError)
		throw read_erreur (errorMsg.str ( ));
}


//////////////////////////////////////////////////////////////////////////////
//! Lecture des donnees sur les polyedres
void _ReaderMalipp::readCell3D (bool loadData)
{
	bool	cancel	= false;
	m_mai->donnees_lues (m_fileName, m_meshNum, ReaderCallback::POLYEDRE, "",
	                     0., cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );

	bool			throwError	= false;
	MutableString	errorMsg;
	errorMsg << "Lecture des polyedres impossible : ";

	Composition	composition	= m_reader.composition_polyedres ( );
	id_type		cellNumber	= composition.nb_elements;
	m_mai->composition_polyedres (composition);

	if ((0 == cellNumber) || (false == loadData))
		return;
	composition.renseigne	= true;
	composition.nb_elements	= 0;
//	m_mai->composition_polyedres (composition);	// v 6.6.0
	Composition	c;	// v 6.6.0
	c.renseigne	= true;	// v 6.6.0
	m_mai->composition_polyedres (c);	// v 6.6.0

	const size_t	chunkSize	=
						computeChunkSize(cellNumber, sizeof(object_type));
	size_type*		ids			= m_reader.allouer_tampon_polyedres (chunkSize);
	try
	{
		if (0 == ids)
			throw read_erreur ("Echec d'une allocation mémoire.");

		for (size_t pos = 0; pos * chunkSize < cellNumber; pos++)
		{
			const size_type	count		=
					m_reader.lire_polyedres (pos * chunkSize, chunkSize, ids);
			size_type*	ptr			= ids;

			// Ajout des polyedres au maillage :
			for (size_type ielm = 0; ielm < count; ielm++)
			{
				size_type		id			= *ptr;		ptr++;
				const size_type	nodeCount	= *ptr;
				switch (nodeCount)
				{
					case 4	:
						m_mai->ajouter (
								_PolyedreInterne::create (id,
									m_mai->noeud_id (ptr [1]),
									m_mai->noeud_id (ptr [2]),
									m_mai->noeud_id (ptr [3]),
									m_mai->noeud_id (ptr [4])));
						break;
					case 5	:
						m_mai->ajouter (
								_PolyedreInterne::create (id,
									m_mai->noeud_id (ptr [1]),
									m_mai->noeud_id (ptr [2]),
									m_mai->noeud_id (ptr [3]),
									m_mai->noeud_id (ptr [4]),
									m_mai->noeud_id (ptr [5])));
						break;
					case 6	:
						m_mai->ajouter (
								_PolyedreInterne::create (id,
									m_mai->noeud_id (ptr [1]),
									m_mai->noeud_id (ptr [2]),
									m_mai->noeud_id (ptr [3]),
									m_mai->noeud_id (ptr [4]),
									m_mai->noeud_id (ptr [5]),
									m_mai->noeud_id (ptr [6])));
						break;
					case 8:
						m_mai->ajouter (
								_PolyedreInterne::create (id,
									m_mai->noeud_id (ptr [1]),
									m_mai->noeud_id (ptr [2]),
									m_mai->noeud_id (ptr [3]),
									m_mai->noeud_id (ptr [4]),
									m_mai->noeud_id (ptr [5]),
									m_mai->noeud_id (ptr [6]),
									m_mai->noeud_id (ptr [7]),
									m_mai->noeud_id (ptr [8])));
						break;
					case 10:
						m_mai->ajouter (
								_PolyedreInterne::create (id,
									m_mai->noeud_id (ptr [1]),
									m_mai->noeud_id (ptr [2]),
									m_mai->noeud_id (ptr [3]),
									m_mai->noeud_id (ptr [4]),
									m_mai->noeud_id (ptr [5]),
									m_mai->noeud_id (ptr [6]),
									m_mai->noeud_id (ptr [7]),
									m_mai->noeud_id (ptr [8]),
									m_mai->noeud_id (ptr [9]),
									m_mai->noeud_id (ptr [10])));
						break;
					case 12:
						m_mai->ajouter (
								_PolyedreInterne::create (id,
									m_mai->noeud_id (ptr [1]),
									m_mai->noeud_id (ptr [2]),
									m_mai->noeud_id (ptr [3]),
									m_mai->noeud_id (ptr [4]),
									m_mai->noeud_id (ptr [5]),
									m_mai->noeud_id (ptr [6]),
									m_mai->noeud_id (ptr [7]),
									m_mai->noeud_id (ptr [8]),
									m_mai->noeud_id (ptr [9]),
									m_mai->noeud_id (ptr [10]),
									m_mai->noeud_id (ptr [11]),
									m_mai->noeud_id (ptr [12])));
						break;
					default	:
						throw read_erreur ("Erreur interne.");
				}	// switch (nodeCount)
				ptr	+= nodeCount + 1;
			}	// for (size_type ielm = 0; ielm < count; ielm++)

			double	percent	= (pos * chunkSize + count) * 100. / cellNumber;
			m_mai->donnees_lues (m_fileName, m_meshNum,
			                     ReaderCallback::POLYEDRE, "", percent, cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
		}	// for (size_t pos = 0; pos * chunkSize < cellNumber; pos++)
	}
	catch (const erreur& err)
	{
		throwError	= true;
		errorMsg << err.what ( );
	}
	catch (const exception& exc)
	{
		throwError	= true;
		errorMsg << exc.what ( );
	}
	catch (const _Reader::LectureAnnulee&)
	{	// Sortir du for + desallocations
	}
	catch (...)
	{
		throwError	= true;
		errorMsg << "erreur non renseignee.";
	}

	delete [] ids;

	if (true == throwError)
		throw read_erreur (errorMsg.str ( ));
}	// _ReaderMalipp::readCell3D


//////////////////////////////////////////////////////////////////////////////
//! Lecture des donnees sur les nuages du maillage
void _ReaderMalipp::readNodeset (bool loadData)
{
	bool			throwError	= false;
	MutableString	errorMsg;
	errorMsg << "Lecture des nuages impossible : ";

	vector<string>	clouds;
	size_type		ordre	= 0;
	size_type*		ids		= 0;
	m_reader.liste_nuages (clouds);
	try
	{
		for (vector<string>::const_iterator it = clouds.begin ( );
		     it != clouds.end ( ); it++, ordre++)
		{
			bool	cancel	= false;
			MaliPPReader::NuageReader	reader	= m_reader.nuage (*it);

			m_mai->donnees_lues (m_fileName, m_meshNum, ReaderCallback::NUAGE,
			                     *it, 0., cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );

			// Lecture du nombre de noeuds du nuage :
			Composition	composition	= reader.composition ( );
			size_type	nodeNumber	= composition.nb_elements;

			// Creation du nuage et ajout au maillage :
			_NuageInterne*	nuage	= new _NuageInterne (*it);
			m_mai->ajouter (nuage);
			nuage->composition (composition);

			if (false == loadData)
			{
				nuage->ordre (ordre);
				nuage->nb_elements (nodeNumber);
				continue;
			}
			composition.reinitialise ( );	// v 6.38.0
			composition.renseigne	= true;
//			composition.nb_elements	= 0;
			nuage->composition (composition);

			// Ajout des noeuds au nuage :
			const size_type	chunkSize	=
						computeChunkSize(nodeNumber, sizeof(size_type));
			ids			= reader.allouer_tampon_ids (chunkSize);
			if (0 == ids)
				throw read_erreur ("Echec d'une allocation mémoire.");
			for (size_t pos = 0; pos * chunkSize < nodeNumber; pos++)
			{
				size_type	count	=
					reader.lire_noeuds_ids (pos * chunkSize, chunkSize, ids);
				size_type*	ptr	= ids;

				for (size_type ielm = 0; ielm < count; ielm++)
				{
					nuage->ajouter (m_mai->noeud_id (*ptr));
					ptr++;
				}
				double	percent	=
					100. * (double)(pos*chunkSize + count) / (double)nodeNumber;
				m_mai->donnees_lues (m_fileName, m_meshNum,
					ReaderCallback::NUAGE, nuage->nom ( ), percent, cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (size_t pos = 0; pos * chunkSize < nodeNumber; pos++)

			nuage->ordre (ordre);
			nuage->nb_elements (nodeNumber);
			delete [] ids;	ids	= 0;

			// Lecture des attributs :
			nuage->init_att ( );
			vector<HDFAttributeDescriptor> attributs;
			reader.lire_liste_attributs (MaliPPReader::NOEUDS, attributs);
			size_t	count	= 1;
			vector<HDFAttributeDescriptor>::const_iterator	ita;
			for (ita = attributs.begin ( ); ita != attributs.end ( );
			     ita++, count++)
			{
				m_mai->donnees_lues (m_fileName, m_meshNum,
	                    ReaderCallback::ATTR_NOEUD, nuage->nom ( ), 0., cancel);
				readGroupAttr (
					reader, MaliPPReader::NOEUDS, nuage->att_noeuds ( ), *ita);
				double	percent	= count * 100. / attributs.size ( );
				m_mai->donnees_lues (m_fileName, m_meshNum,
						ReaderCallback::ATTR_NOEUD, nuage->nom ( ), percent,
						cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (ita = attributs.begin ( ); ...
		}	// for (size_type grp = 0; grp < nodeNumber; grp++)
	}
	catch (const erreur& err)
	{
		throwError	= true;
		errorMsg << err.what ( );
	}
	catch (const exception& exc)
	{
		throwError	= true;
		errorMsg << exc.what ( );
	}
	catch (const _Reader::LectureAnnulee&)
	{	// Sortir du for + desallocations
	}
	catch (...)
	{
		throwError	= true;
		errorMsg << "erreur non renseignee.";
	}
	delete [] ids;

	if (true == throwError)
		throw read_erreur (errorMsg.str ( ));
}	// _ReaderMalipp::readNodeset

//////////////////////////////////////////////////////////////////////////////
//! Lecture des donnees sur les lignes du maillage
void _ReaderMalipp::readLine (bool loadData)
{
	bool			throwError	= false;
	MutableString	errorMsg;
	errorMsg << "Lecture des lignes impossible : ";

	vector<string>	lines;
	size_type		ordre	= 0;
	size_type*		ids		= 0;
	m_reader.liste_lignes (lines);
	try
	{
		for (vector<string>::const_iterator it = lines.begin ( );
		     it != lines.end ( ); it++, ordre++)
		{
			bool	cancel	= false;
			MaliPPReader::LigneReader	reader	= m_reader.ligne (*it);

			m_mai->donnees_lues (m_fileName, m_meshNum, ReaderCallback::LIGNE,
			                     *it, 0., cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );

			// Lecture du nombre de bras de la ligne :
			Composition	composition	= reader.composition ( );
			size_type	edgeNumber	= composition.nb_elements;

			// Creation de la ligne et ajout au maillage :
			_LigneInterne*	ligne	= new _LigneInterne (*it);
			m_mai->ajouter (ligne);
			ligne->composition (composition);

			if (false == loadData)
			{
				ligne->ordre (ordre);
				ligne->nb_elements (edgeNumber);
				continue;
			}
			composition.reinitialise ( );	// v 6.38.0
			composition.renseigne	= true;
//			composition.nb_elements	= 0;
			ligne->composition (composition);

			// Ajout des bras a la ligne :
			const size_type	chunkSize	=
						computeChunkSize(edgeNumber, sizeof(size_type));
			ids			= reader.allouer_tampon_ids (chunkSize);
			if (0 == ids)
				throw read_erreur ("Echec d'une allocation mémoire.");
			for (size_t pos = 0; pos * chunkSize < edgeNumber; pos++)
			{
				size_type	count	=
					reader.lire_bras_ids (pos * chunkSize, chunkSize, ids);
				size_type*	ptr	= ids;

				for (size_type ielm = 0; ielm < count; ielm++)
				{
					ligne->ajouter (m_mai->bras_id (*ptr));
					ptr++;
				}
				double	percent	=
					100. * (double)(pos*chunkSize + count) / (double)edgeNumber;
				m_mai->donnees_lues (m_fileName, m_meshNum,
					ReaderCallback::LIGNE, ligne->nom ( ), percent, cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (size_t pos = 0; pos * chunkSize < edgeNumber; pos++)

			ligne->ordre (ordre);
			ligne->nb_elements (edgeNumber);
			delete [] ids;	ids	= 0;

			// Lecture des attributs :
			// Les bras :
			ligne->init_att ( );
			vector<HDFAttributeDescriptor> attributs;
			reader.lire_liste_attributs (MaliPPReader::NOEUDS, attributs);
			size_t	count	= 1;
			vector<HDFAttributeDescriptor>::const_iterator	ita;
			for (ita = attributs.begin ( ); ita != attributs.end ( );
			     ita++, count++)
			{
				m_mai->donnees_lues (m_fileName, m_meshNum,
	                    ReaderCallback::ATTR_NOEUD, ligne->nom ( ), 0., cancel);
				readGroupAttr (
					reader, MaliPPReader::NOEUDS, ligne->att_noeuds ( ), *ita);
				double	percent	= count * 100. / attributs.size ( );
				m_mai->donnees_lues (m_fileName, m_meshNum,
						ReaderCallback::ATTR_NOEUD, ligne->nom ( ), percent,
						cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (ita = attributs.begin ( ); ...
			attributs.clear ( );
			// Les noeuds :
			reader.lire_liste_attributs (MaliPPReader::BRAS, attributs);
			for (ita = attributs.begin ( ); ita != attributs.end ( );
			     ita++, count++)
			{
				m_mai->donnees_lues (m_fileName, m_meshNum,
	                    ReaderCallback::ATTR_BRAS, ligne->nom ( ), 0., cancel);
				readGroupAttr (
					reader, MaliPPReader::BRAS, ligne->att_bras ( ), *ita);
				double	percent	= count * 100. / attributs.size ( );
				m_mai->donnees_lues (m_fileName, m_meshNum,
						ReaderCallback::ATTR_BRAS, ligne->nom ( ), percent,
						cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (ita = attributs.begin ( ); ...
		}	// for (size_type grp = 0; grp < edgeNumber; grp++)
	}
	catch (const erreur& err)
	{
		throwError	= true;
		errorMsg << err.what ( );
	}
	catch (const exception& exc)
	{
		throwError	= true;
		errorMsg << exc.what ( );
	}
	catch (const _Reader::LectureAnnulee&)
	{	// Sortir du for + desallocations
	}
	catch (...)
	{
		throwError	= true;
		errorMsg << "erreur non renseignee.";
	}
	delete [] ids;

	if (true == throwError)
		throw read_erreur (errorMsg.str ( ));
}	// _ReaderMalipp::readLine

//////////////////////////////////////////////////////////////////////////////
//! Lecture des donnees sur les surfaces du maillage
void _ReaderMalipp::readSurface (bool loadData)
{
	bool			throwError	= false;
	MutableString	errorMsg;
	errorMsg << "Lecture des surfaces impossible : ";

	vector<string>	surfaces;
	size_type		ordre	= 0;
	size_type*		ids		= 0;
	m_reader.liste_surfaces (surfaces);
	try
	{
		for (vector<string>::const_iterator it = surfaces.begin ( );
		     it != surfaces.end ( ); it++, ordre++)
		{
			bool	cancel	= false;
			MaliPPReader::SurfaceReader	reader	= m_reader.surface (*it);

			m_mai->donnees_lues (m_fileName, m_meshNum, ReaderCallback::SURFACE,
			                     *it, 0., cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );

			// Lecture du nombre de mailles de la surface :
			Composition	composition	= reader.composition ( );
			size_type	cellNumber	= composition.nb_elements;

			// Creation de la surface et ajout au maillage :
			_SurfaceInterne*	surface	= new _SurfaceInterne (*it);
			m_mai->ajouter (surface);
			surface->composition (composition);

			if (false == loadData)
			{
				surface->ordre (ordre);
				surface->nb_elements (cellNumber);
				continue;
			}
			composition.reinitialise ( );	// v 6.38.0
			composition.renseigne	= true;
//			composition.nb_elements	= 0;
			surface->composition (composition);

			// Ajout des mailles a la surface :
			const size_type	chunkSize	=
						computeChunkSize(cellNumber, sizeof(size_type));
			ids			= reader.allouer_tampon_ids (chunkSize);
			if (0 == ids)
				throw read_erreur ("Echec d'une allocation mémoire.");
			for (size_t pos = 0; pos * chunkSize < cellNumber; pos++)
			{
				size_type	count	=
					reader.lire_mailles_ids (pos * chunkSize, chunkSize, ids);
				size_type*	ptr	= ids;

				for (size_type ielm = 0; ielm < count; ielm++)
				{
					surface->ajouter (m_mai->polygone_id (*ptr));
					ptr++;
				}
				double	percent	=
					100. * (double)(pos*chunkSize + count) / (double)cellNumber;
				m_mai->donnees_lues (m_fileName, m_meshNum,
					ReaderCallback::SURFACE, surface->nom ( ), percent, cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (size_t pos = 0; pos * chunkSize < cellNumber; pos++)

			surface->ordre (ordre);
			surface->nb_elements (cellNumber);
			delete [] ids;	ids	= 0;

			// Lecture des attributs :
			// Les noeuds :
			surface->init_att ( );
			vector<HDFAttributeDescriptor> attributs;
			reader.lire_liste_attributs (MaliPPReader::NOEUDS, attributs);
			size_t	count	= 1;
			vector<HDFAttributeDescriptor>::const_iterator	ita;
			for (ita = attributs.begin ( ); ita != attributs.end ( );
			     ita++, count++)
			{
				m_mai->donnees_lues (m_fileName, m_meshNum,
                    ReaderCallback::ATTR_NOEUD, surface->nom ( ), 0., cancel);
				readGroupAttr (
					reader, MaliPPReader::NOEUDS, surface->att_noeuds( ), *ita);
				double	percent	= count * 100. / attributs.size ( );
				m_mai->donnees_lues (m_fileName, m_meshNum,
						ReaderCallback::ATTR_NOEUD, surface->nom ( ), percent,
						cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (ita = attributs.begin ( ); ...
			attributs.clear ( );
			// Les bras :
			reader.lire_liste_attributs (MaliPPReader::BRAS, attributs);
			for (ita = attributs.begin ( ); ita != attributs.end ( );
			     ita++, count++)
			{
				m_mai->donnees_lues (m_fileName, m_meshNum,
	                    ReaderCallback::ATTR_BRAS, surface->nom( ), 0., cancel);
				readGroupAttr (
					reader, MaliPPReader::BRAS, surface->att_bras ( ), *ita);
				double	percent	= count * 100. / attributs.size ( );
				m_mai->donnees_lues (m_fileName, m_meshNum,
						ReaderCallback::ATTR_BRAS, surface->nom ( ), percent,
						cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (ita = attributs.begin ( ); ...
			// Les polygones :
			reader.lire_liste_attributs (MaliPPReader::POLYGONES, attributs);
			for (ita = attributs.begin ( ); ita != attributs.end ( );
			     ita++, count++)
			{
				m_mai->donnees_lues (m_fileName, m_meshNum,
                    ReaderCallback::ATTR_POLYGONE, surface->nom( ), 0., cancel);
				readGroupAttr (
					reader, MaliPPReader::POLYGONES, surface->att_polygones ( ),
					*ita);
				double	percent	= count * 100. / attributs.size ( );
				m_mai->donnees_lues (m_fileName, m_meshNum,
						ReaderCallback::ATTR_POLYGONE, surface->nom( ), percent,
						cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (ita = attributs.begin ( ); ...
		}	// for (size_type grp = 0; grp < cellNumber; grp++)
	}
	catch (const erreur& err)
	{
		throwError	= true;
		errorMsg << err.what ( );
	}
	catch (const exception& exc)
	{
		throwError	= true;
		errorMsg << exc.what ( );
	}
	catch (const _Reader::LectureAnnulee&)
	{	// Sortir du for + desallocations
	}
	catch (...)
	{
		throwError	= true;
		errorMsg << "erreur non renseignee.";
	}
	delete [] ids;

	if (true == throwError)
		throw read_erreur (errorMsg.str ( ));
}	// _ReaderMalipp::readSurface

//////////////////////////////////////////////////////////////////////////////
//! Lecture des donnees sur les volumes du maillage
void _ReaderMalipp::readVolume (bool loadData)
{
	bool			throwError	= false;
	MutableString	errorMsg;
	errorMsg << "Lecture des volumes impossible : ";

	vector<string>	volumes;
	size_type		ordre	= 0;
	size_type*		ids		= 0;
	m_reader.liste_volumes (volumes);
	try
	{
		for (vector<string>::const_iterator it = volumes.begin ( );
		     it != volumes.end ( ); it++, ordre++)
		{
			bool	cancel	= false;
			MaliPPReader::VolumeReader	reader	= m_reader.volume (*it);

			m_mai->donnees_lues (m_fileName, m_meshNum, ReaderCallback::VOLUME,
			                     *it, 0., cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );

			// Lecture du nombre de noeuds du volume :
			Composition	composition	= reader.composition ( );
			size_type	cellNumber	= composition.nb_elements;

			// Creation du volume et ajout au maillage :
			_VolumeInterne*	volume	= new _VolumeInterne (*it);
			m_mai->ajouter (volume);
			volume->composition (composition);

			if (false == loadData)
			{
				volume->ordre (ordre);
				volume->nb_elements (cellNumber);
				continue;
			}
			composition.reinitialise ( );	// v 6.38.0
			composition.renseigne	= true;
//			composition.nb_elements	= 0;
			volume->composition (composition);

			// Ajout des noeuds au volume :
			const size_type	chunkSize	=
						computeChunkSize(cellNumber, sizeof(size_type));
			ids			= reader.allouer_tampon_ids (chunkSize);
			if (0 == ids)
				throw read_erreur ("Echec d'une allocation mémoire.");
			for (size_t pos = 0; pos * chunkSize < cellNumber; pos++)
			{
				size_type	count	=
					reader.lire_mailles_ids (pos * chunkSize, chunkSize, ids);
				size_type*	ptr	= ids;

				for (size_type ielm = 0; ielm < count; ielm++)
				{
					volume->ajouter (m_mai->polyedre_id (*ptr));
					ptr++;
				}
				double	percent	=
					100. * (double)(pos*chunkSize + count) / (double)cellNumber;
				m_mai->donnees_lues (m_fileName, m_meshNum,
					ReaderCallback::VOLUME, volume->nom ( ), percent, cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (size_t pos = 0; pos * chunkSize < cellNumber; pos++)

			volume->ordre (ordre);
			volume->nb_elements (cellNumber);
			delete [] ids;	ids	= 0;

			// Lecture des attributs :
			// Les noeuds :
			volume->init_att ( );
			vector<HDFAttributeDescriptor> attributs;
			reader.lire_liste_attributs (MaliPPReader::NOEUDS, attributs);
			size_t	count	= 1;
			vector<HDFAttributeDescriptor>::const_iterator	ita;
			for (ita = attributs.begin ( ); ita != attributs.end ( );
			     ita++, count++)
			{
				m_mai->donnees_lues (m_fileName, m_meshNum,
                    ReaderCallback::ATTR_NOEUD, volume->nom ( ), 0., cancel);
				readGroupAttr (
					reader, MaliPPReader::NOEUDS, volume->att_noeuds ( ), *ita);
				double	percent	= count * 100. / attributs.size ( );
				m_mai->donnees_lues (m_fileName, m_meshNum,
						ReaderCallback::ATTR_NOEUD, volume->nom ( ), percent,
						cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (ita = attributs.begin ( ); ...
			attributs.clear ( );
			// Les bras :
			reader.lire_liste_attributs (MaliPPReader::BRAS, attributs);
			for (ita = attributs.begin ( ); ita != attributs.end ( );
			     ita++, count++)
			{
				m_mai->donnees_lues (m_fileName, m_meshNum,
	                    ReaderCallback::ATTR_BRAS, volume->nom( ), 0., cancel);
				readGroupAttr (
					reader, MaliPPReader::BRAS, volume->att_bras ( ), *ita);
				double	percent	= count * 100. / attributs.size ( );
				m_mai->donnees_lues (m_fileName, m_meshNum,
						ReaderCallback::ATTR_BRAS, volume->nom ( ), percent,
						cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (ita = attributs.begin ( ); ...
			// Les polygones :
			reader.lire_liste_attributs (MaliPPReader::POLYGONES, attributs);
			for (ita = attributs.begin ( ); ita != attributs.end ( );
			     ita++, count++)
			{
				m_mai->donnees_lues (m_fileName, m_meshNum,
                    ReaderCallback::ATTR_POLYGONE, volume->nom( ), 0., cancel);
				readGroupAttr (
					reader, MaliPPReader::POLYGONES, volume->att_polygones ( ),
					*ita);
				double	percent	= count * 100. / attributs.size ( );
				m_mai->donnees_lues (m_fileName, m_meshNum,
						ReaderCallback::ATTR_POLYGONE, volume->nom( ), percent,
						cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (ita = attributs.begin ( ); ...
			// Les polyedres :
			reader.lire_liste_attributs (MaliPPReader::POLYEDRES, attributs);
			for (ita = attributs.begin ( ); ita != attributs.end ( );
			     ita++, count++)
			{
				m_mai->donnees_lues (m_fileName, m_meshNum,
                    ReaderCallback::ATTR_POLYEDRE, volume->nom( ), 0., cancel);
				readGroupAttr (
					reader, MaliPPReader::POLYEDRES, volume->att_polyedres ( ),
					*ita);
				double	percent	= count * 100. / attributs.size ( );
				m_mai->donnees_lues (m_fileName, m_meshNum,
						ReaderCallback::ATTR_POLYEDRE, volume->nom( ), percent,
						cancel);
				if (true == cancel) throw _Reader::LectureAnnulee ( );
			}	// for (ita = attributs.begin ( ); ...
		}	// for (size_type grp = 0; grp < cellNumber; grp++)
	}
	catch (const erreur& err)
	{
		throwError	= true;
		errorMsg << err.what ( );
	}
	catch (const exception& exc)
	{
		throwError	= true;
		errorMsg << exc.what ( );
	}
	catch (const _Reader::LectureAnnulee&)
	{	// Sortir du for + desallocations
	}
	catch (...)
	{
		throwError	= true;
		errorMsg << "erreur non renseignee.";
	}
	delete [] ids;

	if (true == throwError)
		throw read_erreur (errorMsg.str ( ));
}	// _ReaderMalipp::readVolume


//////////////////////////////////////////////////////////////////////////////
void _ReaderMalipp::readMeshAttr ( )
{
	bool			throwError	= false, cancel = false;
	MutableString	errorMsg;
	errorMsg << "Lecture des attributs du maillage impossible : ";

	try
	{
		// Les attributs du maillage :
		m_mai->donnees_lues (m_fileName, m_meshNum,
		                     ReaderCallback::ATTR_MAILLAGE, "", 0., cancel);
		vector<HDFAttributeDescriptor>	attributs;
		m_reader.lire_liste_attributs (MaliPPReader::MAILLAGE, attributs);
		size_t	count	= 1;
		vector<HDFAttributeDescriptor>::const_iterator	it;
		for (it = attributs.begin ( ); it != attributs.end ( ); it++,count++)
		{
			readAttr (MaliPPReader::MAILLAGE, m_mai->att_maillage ( ), *it);
			double	percent	= count * 100. / attributs.size ( );
			m_mai->donnees_lues (m_fileName, m_meshNum,
		                    ReaderCallback::ATTR_MAILLAGE, "", percent, cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
		}	// for (vector<HDFAttributeDescriptor>::const_iterator it =

		// Les attributs des noeuds du maillage :
		m_mai->donnees_lues (m_fileName, m_meshNum,
		                     ReaderCallback::ATTR_NOEUD, "", 0., cancel);
		attributs.clear ( );
		m_reader.lire_liste_attributs (MaliPPReader::NOEUDS, attributs);
		count	= 1;
		for (it = attributs.begin ( ); it != attributs.end ( ); it++,count++)
		{
			readAttr (MaliPPReader::NOEUDS, m_mai->att_noeuds ( ), *it);
			double	percent	= count * 100. / attributs.size ( );
			m_mai->donnees_lues (m_fileName, m_meshNum,
		                    ReaderCallback::ATTR_NOEUD, "", percent, cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
		}	// for (it = attributs.begin ( ); ...

		// Les attributs des bras du maillage :
		m_mai->donnees_lues (m_fileName, m_meshNum,
		                     ReaderCallback::ATTR_BRAS, "", 0., cancel);
		attributs.clear ( );
		m_reader.lire_liste_attributs (MaliPPReader::BRAS, attributs);
		count	= 1;
		for (it = attributs.begin ( ); it != attributs.end ( ); it++,count++)
		{
			readAttr (MaliPPReader::BRAS, m_mai->att_bras ( ), *it);
			double	percent	= count * 100. / attributs.size ( );
			m_mai->donnees_lues (m_fileName, m_meshNum,
		                    ReaderCallback::ATTR_BRAS, "", percent, cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
		}	// for (it = attributs.begin ( ); ...

		// Les attributs des polygones du maillage :
		m_mai->donnees_lues (m_fileName, m_meshNum,
		                     ReaderCallback::ATTR_POLYGONE, "", 0., cancel);
		attributs.clear ( );
		m_reader.lire_liste_attributs (MaliPPReader::POLYGONES, attributs);
		count	= 1;
		for (it = attributs.begin ( ); it != attributs.end ( ); it++,count++)
		{
			readAttr (MaliPPReader::POLYGONES, m_mai->att_polygones ( ), *it);
			double	percent	= count * 100. / attributs.size ( );
			m_mai->donnees_lues (m_fileName, m_meshNum,
		                    ReaderCallback::ATTR_POLYGONE, "", percent, cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
		}	// for (it = attributs.begin ( ); ...

		// Les attributs des polyedres du maillage :
		m_mai->donnees_lues (m_fileName, m_meshNum,
		                     ReaderCallback::ATTR_POLYEDRE, "", 0., cancel);
		attributs.clear ( );
		m_reader.lire_liste_attributs (MaliPPReader::POLYEDRES, attributs);
		count	= 1;
		for (it = attributs.begin ( ); it != attributs.end ( ); it++,count++)
		{
			readAttr (MaliPPReader::POLYEDRES, m_mai->att_polyedres ( ), *it);
			double	percent	= count * 100. / attributs.size ( );
			m_mai->donnees_lues (m_fileName, m_meshNum,
		                    ReaderCallback::ATTR_POLYEDRE, "", percent, cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
		}	// for (it = attributs.begin ( ); ...

		// Les attributs des nuages du maillage :
		m_mai->donnees_lues (m_fileName, m_meshNum,
		                     ReaderCallback::ATTR_NUAGE, "", 0., cancel);
		attributs.clear ( );
		m_reader.lire_liste_attributs (MaliPPReader::NUAGES, attributs);
		count	= 1;
		for (it = attributs.begin ( ); it != attributs.end ( ); it++,count++)
		{
			readAttr (MaliPPReader::NUAGES, m_mai->att_nuages ( ), *it);
			double	percent	= count * 100. / attributs.size ( );
			m_mai->donnees_lues (m_fileName, m_meshNum,
		                    ReaderCallback::ATTR_NUAGE, "", percent, cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
		}	// for (it = attributs.begin ( ); ...

		// Les attributs des lignes du maillage :
		m_mai->donnees_lues (m_fileName, m_meshNum,
		                     ReaderCallback::ATTR_LIGNE, "", 0., cancel);
		attributs.clear ( );
		m_reader.lire_liste_attributs (MaliPPReader::LIGNES, attributs);
		count	= 1;
		for (it = attributs.begin ( ); it != attributs.end ( ); it++,count++)
		{
			readAttr (MaliPPReader::LIGNES, m_mai->att_lignes ( ), *it);
			double	percent	= count * 100. / attributs.size ( );
			m_mai->donnees_lues (m_fileName, m_meshNum,
		                    ReaderCallback::ATTR_LIGNE, "", percent, cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
		}	// for (it = attributs.begin ( ); ...

		// Les attributs des surface du maillage :
		m_mai->donnees_lues (m_fileName, m_meshNum,
		                     ReaderCallback::ATTR_SURFACE, "", 0., cancel);
		attributs.clear ( );
		m_reader.lire_liste_attributs (MaliPPReader::SURFACES, attributs);
		count	= 1;
		for (it = attributs.begin ( ); it != attributs.end ( ); it++,count++)
		{
			readAttr (MaliPPReader::SURFACES, m_mai->att_surfaces ( ), *it);
			double	percent	= count * 100. / attributs.size ( );
			m_mai->donnees_lues (m_fileName, m_meshNum,
		                    ReaderCallback::ATTR_SURFACE, "", percent, cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
		}	// for (it = attributs.begin ( ); ...

		// Les attributs des volumes du maillage :
		m_mai->donnees_lues (m_fileName, m_meshNum,
		                     ReaderCallback::ATTR_VOLUME, "", 0., cancel);
		attributs.clear ( );
		m_reader.lire_liste_attributs (MaliPPReader::VOLUMES, attributs);
		count	= 1;
		for (it = attributs.begin ( ); it != attributs.end ( ); it++,count++)
		{
			readAttr (MaliPPReader::VOLUMES, m_mai->att_volumes ( ), *it);
			double	percent	= count * 100. / attributs.size ( );
			m_mai->donnees_lues (m_fileName, m_meshNum,
		                    ReaderCallback::ATTR_VOLUME, "", percent, cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
		}	// for (it = attributs.begin ( ); ...
	}
	catch (const erreur& err)
	{
		throwError	= true;
		errorMsg << err.what ( );
	}
	catch (const exception& exc)
	{
		throwError	= true;
		errorMsg << exc.what ( );
	}
	catch (const _Reader::LectureAnnulee&)
	{	// Sortir du for + desallocations
	}
	catch (...)
	{
		throwError	= true;
		errorMsg << "erreur non renseignee.";
	}

	if (true == throwError)
		throw read_erreur (errorMsg.str ( ));
}	// _ReaderMalipp::readMeshAttr


//////////////////////////////////////////////////////////////////////////////
/*! Lecture d'un attribut. Cet attribut est ajouté à la liste transmise
    en argument. Le nom et le type de l'attribut sont donnés en argument.
*/
void _ReaderMalipp::readAttr (
					MaliPPReader::ENTITE entite, Liste_attribut& liste,
					const HDFAttributeDescriptor& attrDesc)
{
	size_t		chunkSize	= 0,	first	= 0, dataCount	= 0;
	size_t		k			= 0,	ielm	= 0;
	_Attribut*	attribute	= 0;
	int_4*		ibuf		= 0;
	double*		dbuf		= 0;

	try
	{
		switch (attrDesc.m_type)
		{
			case Attribut::ENTIER	:
			{
				chunkSize	= computeChunkSize (attrDesc.m_size, sizeof(int_4));
				ibuf		= new int_4 [chunkSize];
				_AttributInterne<int>*	attr	=
								new _AttributInterne<int> (attrDesc.m_name);
				if ((0 == ibuf) || (0 == attr))
					throw read_erreur ("Echec d'une allocation mémoire.");
				attribute	= attr;
				liste.attacher (attribute);	// Maintenant, sinon plante.
				for (size_t pos = 0; pos * chunkSize < attrDesc.m_size; pos++)
				{
					first		= pos * chunkSize;
					dataCount	= first + chunkSize >= attrDesc.m_size ?
								  attrDesc.m_size - first : chunkSize;
					m_reader.lire_attr_entier (
									entite, attrDesc, first, dataCount, ibuf);

					if (1 == attrDesc.m_size)
						attribute->set_val (ibuf [0]);
					else
						for (k = 0, ielm = first; k < dataCount; k++,ielm++)
							attribute->set_val (ielm, ibuf [k]);
				}	// for (size_t pos = 0; pos * chunkSize < attrDesc.m_size;
			}	// case Attribut::ENTIER
			break;
			case Attribut::REEL		:
			{
				chunkSize	= computeChunkSize (attrDesc.m_size,sizeof(double));
				dbuf		= new double [chunkSize];
				_AttributInterne<double>*	attr	=
								new _AttributInterne<double> (attrDesc.m_name);
				if ((0 == dbuf) || (0 == attr))
					throw read_erreur ("Echec d'une allocation mémoire.");
				attribute	= attr;
				liste.attacher (attribute);
				for (size_t pos = 0; pos * chunkSize < attrDesc.m_size; pos++)
				{
					first		= pos * chunkSize;
					dataCount	= first + chunkSize >= attrDesc.m_size ?
								  attrDesc.m_size - first : chunkSize;
					m_reader.lire_attr_reel (
									entite, attrDesc, first, dataCount, dbuf);

					if (1 == attrDesc.m_size)
						attribute->set_val (dbuf [0]);
					else
						for (k = 0, ielm = first; k < dataCount; k++,ielm++)
							attribute->set_val (ielm, dbuf [k]);
				}	// for (size_t pos = 0; pos * chunkSize < attrDesc.m_size;
			}	// case Attribut::REEL
			break;
			case Attribut::CHAINE	:
			{
				size_type	length	=
							m_reader.longueur_chaines(entite, attrDesc);
				chunkSize		= computeChunkSize (attrDesc.m_size, length);
				_AttributInterne<string>*	attr	=
								new _AttributInterne<string> (attrDesc.m_name);
				attribute	= attr;
				liste.attacher (attribute);
				if (0 == attr)
					throw read_erreur ("Echec d'une allocation mémoire.");
				vector<string>	chaines (chunkSize);
				for (size_t pos = 0; pos * chunkSize < attrDesc.m_size; pos++)
				{
					first		= pos * chunkSize;
					dataCount	= first + chunkSize >= attrDesc.m_size ?
								  attrDesc.m_size - first : chunkSize;
					m_reader.lire_attr_chaine (
								entite, attrDesc, first, dataCount, chaines);

					if (1 == attrDesc.m_size)
						attribute->set_val (chaines [0]);
					else
					{
						vector<string>::const_iterator	it	= chaines.begin ( );
						for (k = 0, ielm = first; k < dataCount;
						     k++, ielm++, it++)
							attribute->set_val (ielm, *it);
					}
				}	// for (size_t pos = 0; pos * chunkSize < attrDesc.m_size; pos++)
			}	// case Attribut::CHAINE
			break;
			case Attribut::VECT2D	:
			{
				chunkSize	=
						computeChunkSize (attrDesc.m_size, 2 * sizeof(double));
				dbuf		= new double [2 * chunkSize];
				_AttributInterne<Vect2d>*	attr	=
							new _AttributInterne<Vect2d> (attrDesc.m_name);
				if ((0 == dbuf) || (0 == attr))
					throw read_erreur ("Echec d'une allocation mémoire.");
				attribute	= attr;
				liste.attacher (attribute);
				for (size_t pos = 0; pos * chunkSize < attrDesc.m_size; pos++)
				{
					first		= pos * chunkSize;
					dataCount	= first + chunkSize >= attrDesc.m_size ?
								  attrDesc.m_size - first : chunkSize;
					m_reader.lire_attr_vect_2d (
									entite, attrDesc, first, dataCount, dbuf);

					if (1 == attrDesc.m_size)
						attribute->set_val (Vect2d (dbuf [0], dbuf [1]));
					else
						for (k = 0, ielm = first; k < dataCount; k++,ielm++)
							attribute->set_val (
								ielm, Vect2d (dbuf [2 * k], dbuf [2 * k + 1]));
				}	// for (size_t pos = 0; pos * chunkSize < attrDesc.m_size;
			}	// case Attribut::VECT2D
			break;
			case Attribut::VECT3D	:
			{
				chunkSize	=
						computeChunkSize (attrDesc.m_size, 3 * sizeof(double));
				dbuf		= new double [3 * chunkSize];
				_AttributInterne<Vect3d>*	attr	=
							new _AttributInterne<Vect3d> (attrDesc.m_name);
				if ((0 == dbuf) || (0 == attr))
					throw read_erreur ("Echec d'une allocation mémoire.");
				attribute	= attr;
				liste.attacher (attribute);
				for (size_t pos = 0; pos * chunkSize < attrDesc.m_size; pos++)
				{
					first		= pos * chunkSize;
					dataCount	= first + chunkSize >= attrDesc.m_size ?
								  attrDesc.m_size - first : chunkSize;
					m_reader.lire_attr_vect_3d (
									entite, attrDesc, first, dataCount, dbuf);

					if (1 == attrDesc.m_size)
						attribute->set_val (Vect3d (dbuf[0], dbuf[1], dbuf[2]));
					else
						for (k = 0, ielm = first; k < dataCount; k++,ielm++)
							attribute->set_val (ielm,
								Vect3d (dbuf[3*k], dbuf[3*k+1], dbuf[3*k+2]));
				}	// for (size_t pos = 0; pos * chunkSize < attrDesc.m_size;
			}	// case Attribut::VECT3D
			break;
		}	// switch (attrDesc.m_type)
	}
	catch (...)
	{
		delete attribute;
		delete [] ibuf;
		delete [] dbuf;
		throw;
	}

	delete [] ibuf;
	delete [] dbuf;
}	// _ReaderMalipp::readAttr


//////////////////////////////////////////////////////////////////////////////
/*! Lecture d'un attribut. Cet attribut est ajouté à la liste transmise
    en argument. Le nom et le type de l'attribut sont donnés en argument.
*/
void _ReaderMalipp::readGroupAttr (
		MaliPPReader::GroupeReader& reader, MaliPPReader::ENTITE entite,
		Liste_attribut& liste, const HDFAttributeDescriptor& attrDesc)
{
	size_t		chunkSize	= 0,	first	= 0, dataCount	= 0;
	size_t		k			= 0,	ielm	= 0;
	_Attribut*	attribute	= 0;
	int_4*		ibuf		= 0;
	double*		dbuf		= 0;

	try
	{
		switch (attrDesc.m_type)
		{
			case Attribut::ENTIER	:
			{
				chunkSize	= computeChunkSize (attrDesc.m_size, sizeof(int_4));
				ibuf		= new int_4 [chunkSize];
				_AttributInterne<int>*	attr	=
								new _AttributInterne<int> (attrDesc.m_name);
				if ((0 == ibuf) || (0 == attr))
					throw read_erreur ("Echec d'une allocation mémoire.");
				attribute	= attr;
				liste.attacher (attribute);	// Maintenant, sinon plante.
				for (size_t pos = 0; pos * chunkSize < attrDesc.m_size; pos++)
				{
					first		= pos * chunkSize;
					dataCount	= first + chunkSize >= attrDesc.m_size ?
								  attrDesc.m_size - first : chunkSize;
					reader.lire_attr_entier (
									entite, attrDesc, first, dataCount, ibuf);

					if (1 == attrDesc.m_size)
						attribute->set_val (ibuf [0]);
					else
						for (k = 0, ielm = first; k < dataCount; k++,ielm++)
							attribute->set_val (ielm, ibuf [k]);
				}	// for (size_t pos = 0; pos * chunkSize < attrDesc.m_size;
			}	// case Attribut::ENTIER
			break;
			case Attribut::REEL		:
			{
				chunkSize	= computeChunkSize (attrDesc.m_size,sizeof(double));
				dbuf		= new double [chunkSize];
				_AttributInterne<double>*	attr	=
								new _AttributInterne<double> (attrDesc.m_name);
				if ((0 == dbuf) || (0 == attr))
					throw read_erreur ("Echec d'une allocation mémoire.");
				attribute	= attr;
				liste.attacher (attribute);
				for (size_t pos = 0; pos * chunkSize < attrDesc.m_size; pos++)
				{
					first		= pos * chunkSize;
					dataCount	= first + chunkSize >= attrDesc.m_size ?
								  attrDesc.m_size - first : chunkSize;
					reader.lire_attr_reel (
									entite, attrDesc, first, dataCount, dbuf);

					if (1 == attrDesc.m_size)
						attribute->set_val (dbuf [0]);
					else
						for (k = 0, ielm = first; k < dataCount; k++,ielm++)
							attribute->set_val (ielm, dbuf [k]);
				}	// for (size_t pos = 0; pos * chunkSize < attrDesc.m_size;
			}	// case Attribut::REEL
			break;
			case Attribut::CHAINE	:
			{
				size_type	length	= reader.longueur_chaines(entite, attrDesc);
				chunkSize	= computeChunkSize (attrDesc.m_size, length);
				_AttributInterne<string>*	attr	=
								new _AttributInterne<string> (attrDesc.m_name);
				attribute	= attr;
				liste.attacher (attribute);
				if (0 == attr)
					throw read_erreur ("Echec d'une allocation mémoire.");
				vector<string>	chaines (chunkSize);
				for (size_t pos = 0; pos * chunkSize < attrDesc.m_size; pos++)
				{
					first		= pos * chunkSize;
					dataCount	= first + chunkSize >= attrDesc.m_size ?
								  attrDesc.m_size - first : chunkSize;
					reader.lire_attr_chaine (
								entite, attrDesc, first, dataCount, chaines);

					if (1 == attrDesc.m_size)
						attribute->set_val (chaines [0]);
					else
					{
						vector<string>::const_iterator	it	= chaines.begin ( );
						for (k = 0, ielm = first; k < dataCount;
						     k++, ielm++, it++)
							attribute->set_val (ielm, *it);
					}
				}	// for (size_t pos = 0; pos * chunkSize < attrDesc.m_size; pos++)
			}	// case Attribut::CHAINE
			break;
			case Attribut::VECT2D	:
			{
				chunkSize	=
						computeChunkSize (attrDesc.m_size, 2 * sizeof(double));
				dbuf		= new double [2 * chunkSize];
				_AttributInterne<Vect2d>*	attr	=
							new _AttributInterne<Vect2d> (attrDesc.m_name);
				if ((0 == dbuf) || (0 == attr))
					throw read_erreur ("Echec d'une allocation mémoire.");
				attribute	= attr;
				liste.attacher (attribute);
				for (size_t pos = 0; pos * chunkSize < attrDesc.m_size; pos++)
				{
					first		= pos * chunkSize;
					dataCount	= first + chunkSize >= attrDesc.m_size ?
								  attrDesc.m_size - first : chunkSize;
					reader.lire_attr_vect_2d (
									entite, attrDesc, first, dataCount, dbuf);

					if (1 == attrDesc.m_size)
						attribute->set_val (Vect2d (dbuf [0], dbuf [1]));
					else
						for (k = 0, ielm = first; k < dataCount; k++,ielm++)
							attribute->set_val (
								ielm, Vect2d (dbuf [2 * k], dbuf [2 * k + 1]));
				}	// for (size_t pos = 0; pos * chunkSize < attrDesc.m_size;
			}	// case Attribut::VECT2D
			break;
			case Attribut::VECT3D	:
			{
				chunkSize	=
						computeChunkSize (attrDesc.m_size, 3 * sizeof(double));
				dbuf		= new double [3 * chunkSize];
				_AttributInterne<Vect3d>*	attr	=
							new _AttributInterne<Vect3d> (attrDesc.m_name);
				if ((0 == dbuf) || (0 == attr))
					throw read_erreur ("Echec d'une allocation mémoire.");
				attribute	= attr;
				liste.attacher (attribute);
				for (size_t pos = 0; pos * chunkSize < attrDesc.m_size; pos++)
				{
					first		= pos * chunkSize;
					dataCount	= first + chunkSize >= attrDesc.m_size ?
								  attrDesc.m_size - first : chunkSize;
					reader.lire_attr_vect_3d (
									entite, attrDesc, first, dataCount, dbuf);

					if (1 == attrDesc.m_size)
						attribute->set_val (Vect3d (dbuf[0], dbuf[1], dbuf[2]));
					else
						for (k = 0, ielm = first; k < dataCount; k++,ielm++)
							attribute->set_val (ielm,
								Vect3d (dbuf[3*k], dbuf[3*k+1], dbuf[3*k+2]));
				}	// for (size_t pos = 0; pos * chunkSize < attrDesc.m_size;
			}	// case Attribut::VECT3D
			break;
		}	// switch (attrDesc.m_type)
	}
	catch (...)
	{
		delete attribute;
		delete [] ibuf;
		delete [] dbuf;
		throw;
	}

	delete [] ibuf;
	delete [] dbuf;
}	// _ReaderMalipp::readGroupAttr


//////////////////////////////////////////////////////////////////////////////
//! Ecriture de l'indice des elements dans le champ udata
void  _WriterMalipp::setDataToIndice ( )
{
	size_type i;

	// Pour les noeuds
	for( i=0;i<m_mai->nb_noeuds();i++)
	{
		id_type* j=new id_type(i);
		m_mai->noeud(i)->set_udata(j);
	}
	// Pour les bras
	for( i=0;i<m_mai->nb_bras();i++)
	{
		id_type* j=new id_type(i);
		m_mai->bras(i)->set_udata(j);
	}
	// Pour les polygones
	for( i=0;i<m_mai->nb_polygones();i++)
	{
		id_type* j=new id_type(i);
		m_mai->polygone(i)->set_udata(j);
	}
	// Pour les polyedres
	for( i=0;i<m_mai->nb_polyedres();i++)
	{
		id_type* j=new id_type(i);
		m_mai->polyedre(i)->set_udata(j);
	}
}	// _WriterMalipp::setDataToIndice

//////////////////////////////////////////////////////////////////////////////
//! Mise a NULL du champ udata
void  _WriterMalipp::setDataToNull ( )
{
	size_type i;

	// Pour les noeuds
	for( i=0;i<m_mai->nb_noeuds();i++)
	{
		delete (id_type*)m_mai->noeud(i)->udata();
		m_mai->noeud(i)->set_udata(NULL);
	}
	// Pour les bras
	for( i=0;i<m_mai->nb_bras();i++)
	{
		delete (id_type*)m_mai->bras(i)->udata();
		m_mai->bras(i)->set_udata(NULL);
	}
	// Pour les polygones
	for( i=0;i<m_mai->nb_polygones();i++)
	{
		delete (id_type*)m_mai->polygone(i)->udata();
		m_mai->polygone(i)->set_udata(NULL);
	}
	// Pour les polyedres
	for( i=0;i<m_mai->nb_polyedres();i++)
	{
		delete (id_type*)m_mai->polyedre(i)->udata();
		m_mai->polyedre(i)->set_udata(NULL);
	}
}	// _WriterMalipp::setDataToNull

//////////////////////////////////////////////////////////////////////////////
//! Ecriture d'un fichier Malipp
void _WriterMalipp::ecrire ( )
{
	bool			throwExc	= false;
	MutableString	errorMsg;

	// Lima 6.0.0 : On verifie que deux groupes de meme type n'ont pas le meme
	// nom.
	size_type	i	= 0, j = 0;
	if (2 <= m_mai->nb_nuages ( ))
		for (i = 0; i < m_mai->nb_nuages ( ); i++)
			for (j = i + 1; j < m_mai->nb_nuages ( ); j++)
				if (m_mai->nuage (i)->nom ( ) == m_mai->nuage (j)->nom ( ))
				{
					errorMsg << "Deux nuages ont pour nom "
					         << m_mai->nuage (i)->nom ( );
					throw write_erreur (errorMsg.str ( ));
				}
	if (2 <= m_mai->nb_lignes ( ))
		for (i = 0; i < m_mai->nb_lignes ( ); i++)
			for (j = i + 1; j < m_mai->nb_lignes ( ); j++)
				if (m_mai->ligne (i)->nom ( ) == m_mai->ligne (j)->nom ( ))
				{
					errorMsg << "Deux lignes ont pour nom "
					         << m_mai->ligne (i)->nom ( );
					throw write_erreur (errorMsg.str ( ));
				}
	if (2 <= m_mai->nb_surfaces ( ))
		for (i = 0; i < m_mai->nb_surfaces ( ); i++)
			for (j = i + 1; j < m_mai->nb_surfaces ( ); j++)
				if (m_mai->surface (i)->nom ( ) == m_mai->surface (j)->nom ( ))
				{
					errorMsg << "Deux surfaces ont pour nom "
					         << m_mai->surface (i)->nom ( );
					throw write_erreur (errorMsg.str ( ));
				}
	if (2 <= m_mai->nb_volumes ( ))
		for (i = 0; i < m_mai->nb_volumes ( ); i++)
			for (j = i + 1; j < m_mai->nb_volumes ( ); j++)
				if (m_mai->volume (i)->nom ( ) == m_mai->volume (j)->nom ( ))
				{
					errorMsg << "Deux volumes ont pour nom "
					         << m_mai->volume (i)->nom ( );
					throw write_erreur (errorMsg.str ( ));
				}

	if(true == _ListeFichier::donneInstance (m_fileName, m_meshNum, "rw"))
	{
		bool	useZlib		= false;
		int		zlibLevel	= 0;

		MaliPPWriter::utilisation_zlib (useZlib, zlibLevel);
		try
		{
			// Cette fonction couvre la totalite des operations d'ecriture. On
			// inactive donc ici, pour toutes les operations d'ecriture,
			// l'affichage des messages d'erreurs HDF dans le terminal.
			// Ces affichages peuvent etre reactives en compilant avec
			// l'option -DNDEBUG_HDF.
			HDFErrorHandlerManager	errorHandlerManager;

			// Ouverture en ecriture du fichier, creation si necessaire :
//			try
//			{
				// Contournement de bogue HDF5 1.4.3 ? Plante dans certains cas,
				// pour une raison inconnue, avec g++ pour compilateur sous
				// Sun.
				struct stat	stats;
				int			notexists	= stat (m_fileName.c_str ( ), &stats);
				if (0 == notexists)
					m_hdfFile.reset (new H5File (m_fileName, H5F_ACC_RDWR));
				else
					m_hdfFile.reset (new H5File (m_fileName, H5F_ACC_EXCL));
//			}
//			catch (const Exception& exc)
//			{	// Le fichier n'existe probablement pas : on le cree.
//				m_hdfFile.reset (new H5File (m_fileName, H5F_ACC_EXCL));
//			}

#ifndef HDF5_1_4_3
			if (true == m_forceZlib)
				MaliPPWriter::activer_compression_zlib ( );
#endif	// HDF5_1_4_3

			// On met a jour l'entete du fichier :
			createMainHeader ( );

			// Le fichier est ouvert : ouvrir en ecriture le maillage demande,
			// en le creant si necessaire :
			createMesh ( );
			// Ecriture de l'entete du maillage :
			writeMeshHeader ( );

			// Ecriture du maillage :
//			setDataToIndice ( );	CP, version 5.4.3
			writeAttr (&m_mai->att_maillage(),m_meshGroup, DATA_MESH_GROUP_NAME);

			writeNode ( );
			writeAttr (&m_mai->att_noeuds(),m_meshGroup,DATA_NODE_GROUP_NAME);

			writeEdge ( );
			writeAttr(&m_mai->att_bras(),m_meshGroup,DATA_EDGE_GROUP_NAME);

			writeCell2D ( );
			writeAttr(&m_mai->att_polygones(),m_meshGroup,DATA_CELL_2D_GROUP_NAME);

			writeCell3D ( );
			writeAttr(&m_mai->att_polyedres(),m_meshGroup,DATA_CELL_3D_GROUP_NAME);

			writeNodeset ( );
			writeAttr(&m_mai->att_nuages(),m_meshGroup,DATA_NODESET_GROUP_NAME);

			writeLine ( );
			writeAttr(&m_mai->att_lignes(),m_meshGroup,DATA_LINE_GROUP_NAME);

			writeSurface ( );
			writeAttr(&m_mai->att_surfaces(),m_meshGroup,DATA_SURFACE_GROUP_NAME);

			writeVolume ( );
			writeAttr(&m_mai->att_volumes(),m_meshGroup,DATA_VOLUME_GROUP_NAME);
		}
		catch (const Exception& hdfExc)
		{
			throwExc	= true;
			errorMsg << "Impossible d'ouvrir le fichier " << m_fileName
			         << " en ecriture :\n" << hdfExc.getDetailMsg ( );
		}
		catch (const exception& exc)
		{
			throwExc	= true;
			errorMsg << "Impossible d'ouvrir le fichier " << m_fileName
			         << " en ecriture :\n" << exc.what ( );
		}
		catch (...)
		{
			throwExc	= true;
			errorMsg << "Impossible d'ouvrir le fichier " << m_fileName
			         << " en ecriture : erreur non documentee.";
		}
#ifndef HDF5_1_4_3
		if (false == useZlib)
			MaliPPWriter::desactiver_compression ( );
#endif	// HDF5_1_4_3

		// Un peu de menage :
//  		setDataToNull ( );	CP, version 5.4.3
		m_meshGroup	= Group ( );
		m_hdfFile.reset (0);
		_ListeFichier::libereInstance (m_fileName, m_meshNum, "rw");

		if (true == throwExc)
			throw write_erreur (errorMsg.str ( ));
	}
	else
		throw write_erreur ("Le maillage " + to_str (m_meshNum) +
		                    " du fichier " + m_fileName + " est deja ouvert\n");
}	// _WriterMalipp::ecrire

//////////////////////////////////////////////////////////////////////////////
//! Creation (si nécessaire) du header d'un fichier HDF5
void _WriterMalipp::createMainHeader ( )
{
#ifndef DISABLE_MLI_WARNING
	// Avertissement écrit en bleu puis restauration couleur par défaut.
	cerr << "\033[34m" << "ATTENTION : le format mli est obsolète. Nous recommandons d'utiliser à la place le format mli2 qui reprend les mêmes caractéristiques." << "\033[39m" << endl;
#endif	// DISABLE_MLI_WARNING
	hsize_t strDims [1];
	hsize_t strMaxDims [1];

	Group	header	=
			HDFHelper::openOrCreateGroup (*m_hdfFile, FILE_HEADER_GROUP_NAME);

	// Titre : ne pas l'ecraser si pre-existant.
	try
	{
		header.openDataSet (TITLE_FIELD_NAME);
	}
	catch (const Exception&)
	{
		const string	title ("Titre du fichier vide");
		DataSet			titleDataSet	=
			HDFHelper::openOrCreateStringDataSet (header, TITLE_FIELD_NAME,
		   	                                   title.length ( ));
		titleDataSet.write (title.c_str ( ), titleDataSet.getDataType ( ));
	}

	// Version du format de fichier et de lima++ :
	writeVersionsInfos (header);

	// Auteur : on l'ecrase pour y mettre ce qui devrait etre le nom de
	// l'utilisateur actuel.
	const string	author (getUserName ( ));
	Attribute		authorAttribute	=
		HDFHelper::openOrCreateStringAttribute (header, AUTHOR_FIELD_NAME,
		                                        ATTRIBUT_SIZE);
	HDFHelper::writeStringAttribute (authorAttribute, author);

	// Commentaire : ne pas l'ecraser si pre-existant.
	try
	{
		header.openDataSet (COMMENT_FIELD_NAME);
	}
	catch (const Exception&)
	{
		const string	comment ("Commentaire vide");
		DataSet			commentDataSet	=
			HDFHelper::openOrCreateStringDataSet (header, COMMENT_FIELD_NAME,
			                                      comment.length( ));
		commentDataSet.write(comment.c_str ( ), commentDataSet.getDataType ( ));
	}

	// Nombre de maillages : ne pas l'ecraser si pre-existant
	try
	{
		header.openAttribute (NB_MESHES_FIELD_NAME);
	}
	catch (const Exception&)
	{
		Attribute	nbMeshesAttribute	=
			HDFHelper::openOrCreateAttribute (header, NB_MESHES_FIELD_NAME,
			                                  PredType::NATIVE_INT);
		const int		nbMeshes = 0;
		nbMeshesAttribute.write (PredType::NATIVE_INT, &nbMeshes);
	}
}	// _WriterMalipp::createMainHeader


//////////////////////////////////////////////////////////////////////////////
/*! Ecriture des informations relatives aux versions (Lima/Format mli/.mliz).
    root est l'endroit où sont écrites ces informations. */
void _WriterMalipp::writeVersionsInfos (H5Object& root)
{
	// Version du format de fichier : mise a jour obligatoire.
	Attribute		fileVersionAttribute	=
		HDFHelper::openOrCreateStringAttribute (root, FILE_VERSION_FIELD_NAME,
		                                        FILE_VERSION_MAX_LENGTH);
	HDFHelper::writeStringAttribute (fileVersionAttribute, FILE_VERSION);

	// Version de lima++ : mise a jour obligatoire.
	Attribute		limaVersionAttribute	=
		HDFHelper::openOrCreateStringAttribute (root, LIMA_VERSION_FIELD_NAME,
		                                        LIMA_VERSION_MAX_LENGTH);
	HDFHelper::writeStringAttribute (limaVersionAttribute, LIMA_VERSION);
}	// _WriterMalipp::writeVersionsInfos


//////////////////////////////////////////////////////////////////////////////
/*! Incremente le nombre de maillage d'un fichier. L'attribut identifie par
   NB_MESHES_FIELD_NAME doit préalablement exister et m_fileName être
   ouvert. */
void _WriterMalipp::nbMeshInc ( )
{
	Group		header				=
					m_hdfFile->openGroup (FILE_HEADER_GROUP_NAME);
	int			nbMeshes 			= 0;
	Attribute	nbMeshesAttribute	=
					header.openAttribute (NB_MESHES_FIELD_NAME);
	nbMeshesAttribute.read (PredType::NATIVE_INT, &nbMeshes);
	nbMeshes++;
	nbMeshesAttribute.write (PredType::NATIVE_INT, &nbMeshes);
}	// _WriterMalipp::nbMeshInc

//////////////////////////////////////////////////////////////////////////////
//! Ecriture de commentaires sur le fichier
/* =========================================================================
void _WriterMalipp::setComment (hid_t root,const string& comment)
{
  hid_t   hfp_h;
  hid_t   dataset;
  herr_t  ret;

  hfp_h = H5Gopen(root,"header");

  dataset = H5Dopen(hfp_h,"comment");
  ret = H5Dwrite(dataset, H5T_NATIVE_CHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           comment.c_str());
  H5Dclose(dataset);
  H5Gclose(hfp_h);
}	// _WriterMalipp::setComment
========================================================================= */

//////////////////////////////////////////////////////////////////////////////
//! Ecriture du header d'un maillage. Création si nécessaire.
void _WriterMalipp::writeMeshHeader ( )
{
	Group	header	=
			HDFHelper::openOrCreateGroup (m_meshGroup, MESH_HEADER_GROUP_NAME);

	// Version du format de fichier et de lima++ :
	writeVersionsInfos (header);

	// Titre du maillage :
	Attribute	meshTitleAttribute	=
		HDFHelper::openOrCreateStringAttribute (header, MESH_TITLE_FIELD_NAME,
		                                        ATTRIBUT_SIZE);
	HDFHelper::writeStringAttribute (meshTitleAttribute, m_mai->titre ( ));

	// Date du maillage :
	Attribute	meshDateAttribute	=
		HDFHelper::openOrCreateStringAttribute (header, MESH_DATE_FIELD_NAME,
		                                        ATTRIBUT_SIZE);
	HDFHelper::writeStringAttribute (meshDateAttribute, m_mai->date ( ));

	// Dimension du maillage :
	Attribute	meshDimAttribute	=
			HDFHelper::openOrCreateAttribute (header, MESH_DIMENSION_FIELD_NAME,
			                                  PredType::NATIVE_INT);
	int	dimension	= m_mai->dimension ( );
	meshDimAttribute.write (PredType::NATIVE_INT, &dimension);

	// Type de geometrie du maillage :
	Attribute	meshGeomAttribute	=
			HDFHelper::openOrCreateAttribute (header, MESH_GEOMETRY_FIELD_NAME,
			                                  PredType::NATIVE_INT);
	int	geometry	= m_mai->type_geometrie ( );
	meshGeomAttribute.write (PredType::NATIVE_INT, &geometry);

	// System de coordonnees du maillage :
	Attribute	meshCoordSysAttribute	=
		HDFHelper::openOrCreateAttribute (header, MESH_COORD_SYSTEM_FIELD_NAME,
		                                  PredType::NATIVE_INT);
	int	coordSys	= m_mai->type_coordonnee ( );
	meshCoordSysAttribute.write (PredType::NATIVE_INT, &coordSys);

	// Unite d'angle :
	Attribute	meshAngleUnitAttribute	=
			HDFHelper::openOrCreateAttribute (
				header, MESH_ANGLE_UNIT_FIELD_NAME, PredType::NATIVE_DOUBLE);
	double	angleUnit	= m_mai->unite_angle ( );
	meshAngleUnitAttribute.write (PredType::NATIVE_DOUBLE, &angleUnit);

	// Unite de longueur :
	Attribute	meshLengthUnitAttribute	=
		HDFHelper::openOrCreateAttribute (header, MESH_LENGTH_UNIT_FIELD_NAME,
		                                  PredType::NATIVE_DOUBLE);
	double	lengthUnit	= m_mai->unite_longueur ( );
	meshLengthUnitAttribute.write (PredType::NATIVE_DOUBLE, &lengthUnit);
}	// _WriterMalipp::writeMeshHeader

//////////////////////////////////////////////////////////////////////////////
//! Creation du squelette d'un maillage .mli
void _WriterMalipp::createMeshDataGroup ( )
{
	// Creation du DataGroup data_mesh
	HDFHelper::openOrCreateGroup (m_meshGroup, DATA_MESH_GROUP_NAME);

	// Creation des DataGroups node
	HDFHelper::openOrCreateGroup (m_meshGroup, NODE_GROUP_NAME);
	HDFHelper::openOrCreateGroup (m_meshGroup, DATA_NODE_GROUP_NAME);

	// Creation des DataGroups edge
	HDFHelper::openOrCreateGroup (m_meshGroup, EDGE_GROUP_NAME);
	HDFHelper::openOrCreateGroup (m_meshGroup, DATA_EDGE_GROUP_NAME);

	// Creation des DataGroups cell
	HDFHelper::openOrCreateGroup (m_meshGroup, CELL_2D_GROUP_NAME);
	HDFHelper::openOrCreateGroup (m_meshGroup, DATA_CELL_2D_GROUP_NAME);
	HDFHelper::openOrCreateGroup (m_meshGroup, CELL_3D_GROUP_NAME);
	HDFHelper::openOrCreateGroup (m_meshGroup, DATA_CELL_3D_GROUP_NAME);

	// Creation des DataGroups nodeset
	HDFHelper::openOrCreateGroup (m_meshGroup, NODESET_GROUP_NAME);
	HDFHelper::openOrCreateGroup (m_meshGroup, DATA_NODESET_GROUP_NAME);

	// Creation des DataGroups line
	HDFHelper::openOrCreateGroup (m_meshGroup, LINE_GROUP_NAME);
	HDFHelper::openOrCreateGroup (m_meshGroup, DATA_LINE_GROUP_NAME);

	// Creation des DataGroups surface
	HDFHelper::openOrCreateGroup (m_meshGroup, SURFACE_GROUP_NAME);
	HDFHelper::openOrCreateGroup (m_meshGroup, DATA_SURFACE_GROUP_NAME);

	// Creation des DataGroups volume
	HDFHelper::openOrCreateGroup (m_meshGroup, VOLUME_GROUP_NAME);
	HDFHelper::openOrCreateGroup (m_meshGroup, DATA_VOLUME_GROUP_NAME);
}	// _WriterMalipp::createMeshDataGroup

//////////////////////////////////////////////////////////////////////////////
//! Creation d'un maillage
void _WriterMalipp::createMesh ( )
{
	string	path	= MESH_GROUP_PREFIX + to_str (m_meshNum);

	// Il faudra incrementer le nombre de maillage du fichier si celui-ci
	// n'exite pas encore. Cette operation sera effectuee a la fin de
	// cette fonction si celle-ci aboutie.
	bool	incrementMeshCount	= false;
	try
	{
		m_hdfFile->openGroup (path);
	}
	catch (const Exception&)
	{
		incrementMeshCount	= true;
	}

	// Creation du DataGroup meshx
	m_meshGroup	= HDFHelper::openOrCreateGroup (*m_hdfFile, path);

	// Creation (si necessaire) du header du maillage
	writeMeshHeader ( );

	// Creation (si necessaire) de l'arborescence du maillage
	createMeshDataGroup ( );

	if (true == incrementMeshCount)
		nbMeshInc ( );
}	// _WriterMalipp::createMesh

//////////////////////////////////////////////////////////////////////////////
//! Ecriture des donnees sur les noeuds
void _WriterMalipp::writeNode ( )
{
	Group		nodeGroup 		= m_meshGroup.openGroup (NODE_GROUP_NAME);
	// Le nombre de noeuds :
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			nodeGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	const id_type	nodeNumber	= (id_type)m_mai->nb_noeuds ( );
	sizeAttribute.write (PredType::NATIVE_UINT32, &nodeNumber);
	writeComposition (m_mai->composition_noeuds ( ),
	                  NODE_COMPOSITION_DATASET_NAME, nodeGroup);

	if (0 == nodeNumber)
		return;

	size_t		chunkSize	= computeChunkSize (nodeNumber, sizeof (double));
	double*		buf			= new double [chunkSize];
	size_type	ielm		= 0, i = 0;
	// Les identifiants sont ils contigus ? Recherche faite une unique fois ! :
	int			contiguous	= 1;
	for (ielm = 0; ielm < nodeNumber - 1; ielm++)
		if (m_mai->noeud (ielm)->id ( ) + 1 != m_mai->noeud (ielm+1)->id ( ))
		{
			contiguous	= 0;
			break;
		}
	// Pour ecrire les ids :
	id_type*	ibuf		= new id_type [1 == contiguous ? 1 : chunkSize];

	// Ecriture des noeuds :
	for (size_t pos = 0; pos * chunkSize < nodeNumber; pos++)
	{
		size_t		first		= pos * chunkSize;
		size_t		dataCount	= first + chunkSize >= nodeNumber ?
								  nodeNumber - first : chunkSize;
		switch (m_mai->dimension ( ))
		{
			case D3	:
			{
				DataSet	zCoordDataSet	= HDFHelper::openOrCreateDataSet1D (
						nodeGroup, DATASET_COORD_Z_NAME, nodeNumber,
						PredType::NATIVE_DOUBLE, true, NODE_CHUNK_SIZE);
				for (ielm = first, i = 0; ielm < first + dataCount; ielm++, i++)
					buf [i] = m_mai->noeud (ielm)->z ( );
				HDFHelper::writeDoubleDataSet1D (zCoordDataSet, buf, dataCount,
				                                 first);
			}
			case D2	:
			{
				DataSet	yCoordDataSet	= HDFHelper::openOrCreateDataSet1D (
						nodeGroup, DATASET_COORD_Y_NAME, nodeNumber,
						PredType::NATIVE_DOUBLE, true, NODE_CHUNK_SIZE);
				for (ielm = first, i = 0; ielm < first + dataCount; ielm++, i++)
					buf [i] = m_mai->noeud (ielm)->y ( );
				HDFHelper::writeDoubleDataSet1D (yCoordDataSet, buf, dataCount,
				                                 first);
			}
			case D1 :
			{
				DataSet	xCoordDataSet	= HDFHelper::openOrCreateDataSet1D (
						nodeGroup, DATASET_COORD_X_NAME, nodeNumber,
						PredType::NATIVE_DOUBLE, true, NODE_CHUNK_SIZE);
				for (ielm = first, i = 0; ielm < first + dataCount; ielm++, i++)
					buf [i] = m_mai->noeud (ielm)->x ( );
				HDFHelper::writeDoubleDataSet1D (xCoordDataSet, buf, dataCount,
				                                 first);
			}
			break;
			default	:
   	       throw write_erreur (
							"Ecriture des noeuds impossible : erreur interne.");
		}	// switch (m_mai->dimension ( ))

		// Sauvegarde des identifiants :
		if (0 == pos)
		{
			Attribute	contiguousAttribut	= HDFHelper::openOrCreateAttribute (
					nodeGroup, CONTIGUOUS_IDS_ATTR_NAME, PredType::NATIVE_INT);
			contiguousAttribut.write (PredType::NATIVE_INT, &contiguous);
		}	// if (0 == pos)

		// Ecriture des identifiants :
		// Le dataset contenant les ids :
		DataSet	idsDataSet	= HDFHelper::openOrCreateDataSet1D (
				nodeGroup, DATASET_IDS_NAME,
				(hsize_t)(1 == contiguous ? 1 : nodeNumber),
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		if (1 == contiguous)
		{
			if (0 == pos)
			{
				ibuf [0]	= (id_type)(m_mai->noeud (0)->id ( ));
				HDFHelper::writeUIntDataSet1D (idsDataSet, ibuf, 1, 0);
			}	// if (0 == pos)
		}	// if ((1 == contiguous) && (0 == pos))
		else
		{
			for (ielm = first, i = 0; ielm < first + dataCount; ielm++, i++)
				ibuf [i] = (id_type)(m_mai->noeud (ielm)->id ( ));
			HDFHelper::writeUIntDataSet1D (idsDataSet, ibuf, dataCount, first);
		}	// else if (1 == contiguous)
	}	// for (size_t pos = 0; pos * chunkSize < nodeNumber; pos++)
	delete [] buf;
	delete [] ibuf;
}	// _WriterMalipp::writeNode

//////////////////////////////////////////////////////////////////////////////
//! Ecriture des donnees sur les bras
void _WriterMalipp::writeEdge ( )
{
	Group		edgeGroup 		= m_meshGroup.openGroup (EDGE_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			edgeGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	const id_type	edgeNumber	= (id_type)m_mai->nb_bras ( );
	sizeAttribute.write (PredType::NATIVE_UINT32, &edgeNumber);
	writeComposition (m_mai->composition_bras ( ),
	                  EDGE_COMPOSITION_DATASET_NAME, edgeGroup);

	if (0 == edgeNumber)
		return;

	size_t		chunkSize	= computeChunkSize(edgeNumber, 2 * sizeof(id_type));
	id_type*	ibuf		= 0;
	size_type	ielm		= 0, i = 0;
	// Sauvegarde des identifiants : les identifiants sont ils contigus ?
	int			contiguous	= 1;
	for (ielm = 0; ielm < edgeNumber - 1; ielm++)
		if (m_mai->bras (ielm)->id ( ) + 1 != m_mai->bras (ielm + 1)->id ( ))
		{
			contiguous	= 0;
			break;
		}
	Attribute	contiguousAttribut	= HDFHelper::openOrCreateAttribute (
					edgeGroup, CONTIGUOUS_IDS_ATTR_NAME, PredType::NATIVE_INT);
	contiguousAttribut.write (PredType::NATIVE_INT, &contiguous);

	// Ecriture des bras :
	for (size_t pos = 0; pos * chunkSize < edgeNumber; pos++)
	{
		size_t		first		= pos * chunkSize;
		size_t		dataCount	= first + chunkSize >= edgeNumber ?
								  edgeNumber - first : chunkSize;
		size_type	nodeNumber	= 2 * dataCount;
		size_type	indice		= first;
		if (0 == ibuf)
			ibuf	= new id_type [nodeNumber];
		for (ielm = 0, i = first; ielm < (size_type)dataCount; ielm++, i++)
		{
			ibuf [2*ielm]	=
					(id_type)(m_mai->bras (i)->noeud (0)->id ( ) - 1);
			ibuf [2*ielm+1]	=
					(id_type)(m_mai->bras (i)->noeud (1)->id ( ) - 1);
		}	// for (ielm = 0, i = first; ielm < edgeNumber; ielm++, i++)
		// La liste des noeuds :
		DataSet	nodeIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
				edgeGroup, DATASET_IDS_LIST_NAME, 2 * edgeNumber,
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		HDFHelper::writeUIntDataSet1D (nodeIdsDataSet, ibuf, nodeNumber,
		                               2 * first);

		// Ecriture des identifiants :
		size_type	count	= 0 != contiguous ? 1 : edgeNumber;
		DataSet	edgeIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
				edgeGroup, DATASET_IDS_NAME,
				(hsize_t)(0 != contiguous ? 1 : edgeNumber),
				PredType::NATIVE_UINT32, true, EDGE_CHUNK_SIZE);
		if (1 == contiguous)
		{
			if (0 == pos)
			{
				ibuf [0]	= (id_type)(m_mai->bras (0)->id ( ));
				HDFHelper::writeUIntDataSet1D (edgeIdsDataSet, ibuf, 1, 0);
			}	// if (0 == pos)
		}	// if (1 == contiguous)
		else
		{
			for (ielm = first, i = 0; ielm < first + dataCount; ielm++, i++)
				ibuf [i]	= (id_type)(m_mai->bras (ielm)->id ( ));
			HDFHelper::writeUIntDataSet1D (edgeIdsDataSet, ibuf, dataCount,
			                               first);
		}	// else if (1 == contiguous)
	}	// for (size_t pos = 0; pos * chunkSize < edgeNumber; pos++)

	delete [] ibuf;
}	// _WriterMalipp::writeEdge

//////////////////////////////////////////////////////////////////////////////
//! Ecriture des donnees sur les mailles 2D
void _WriterMalipp::writeCell2D ( )
{
	Group		cell2DGroup 	= m_meshGroup.openGroup (CELL_2D_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			cell2DGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	const id_type	cellNumber	= (id_type)m_mai->nb_polygones ( );
	sizeAttribute.write (PredType::NATIVE_UINT32, &cellNumber);
	writeComposition (m_mai->composition_polygones ( ),
	                  CELL2D_COMPOSITION_DATASET_NAME,cell2DGroup);

	if (0 == cellNumber)
		return;

	// chunkSize : pb, on ne connait pas encore le type des mailles et donc
	// le nombre de noeuds par maille ... Fixe arbitrairement a 6.
	size_t			chunkSize	=
						computeChunkSize (cellNumber, 6 * sizeof (id_type));
	size_type		ielm		= 0, i = 0, totalNodeNumber = 0;
	object_type*	ibuf 		= 0;
	id_type*		uibuf		= 0;
	// Sauvegarde des identifiants : les identifiants sont ils contigus ?
	int			contiguous	= 1;
	for (ielm = 0; ielm < cellNumber - 1; ielm++)
		if (m_mai->polygone(ielm)->id( ) + 1 != m_mai->polygone(ielm+1)->id( ))
		{
			contiguous	= 0;
			break;
		}
	Attribute	contiguousAttribut	= HDFHelper::openOrCreateAttribute (
					cell2DGroup, CONTIGUOUS_IDS_ATTR_NAME,PredType::NATIVE_INT);
	contiguousAttribut.write (PredType::NATIVE_INT, &contiguous);

	// Ecriture des mailles 2D :
	for (size_t pos = 0; pos * chunkSize < cellNumber; pos++)
	{
		size_t		first		= pos * chunkSize;
		size_t		dataCount	= first + chunkSize >= cellNumber ?
								  cellNumber - first : chunkSize;

		// Ecriture des types de maille 2D (nombre de noeuds) :
		if (0 == ibuf)
			ibuf	= new object_type [chunkSize];
		size_type		nodeNumber	= 0;
		for (ielm = 0, i = first; ielm < (size_type)dataCount; ielm++, i++)
		{
			ibuf [ielm]	= (object_type)(m_mai->polygone (i)->nb_noeuds ( ));
			nodeNumber	+= ibuf [ielm];
		}
		// Nombre de noeuds actuellement recenses :
		totalNodeNumber	+= nodeNumber;
		DataSet	cellTypesDataSet	= HDFHelper::openOrCreateDataSet1D (
				cell2DGroup, DATASET_TYPES_NAME, cellNumber,
				PredType::NATIVE_INT32, true, CELL2D_CHUNK_SIZE);
		HDFHelper::writeIntDataSet1D (cellTypesDataSet, ibuf, dataCount, first);

		// Ecriture des ids des noeuds :
		uibuf	= new id_type [nodeNumber];
  		size_type	indice	= 0;
		for (ielm = first; ielm < (size_type)(first + dataCount); ielm++)
   		 	for (i = 0; i < m_mai->polygone (ielm)->nb_noeuds( ); i++)
				uibuf [indice++] =
						(id_type)(m_mai->polygone (ielm)->noeud(i)->id ( ) - 1);
		// On fait la mise a jour du nombre de noeuds total :
		DataSet	nodeIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
			cell2DGroup, DATASET_IDS_LIST_NAME, totalNodeNumber,
			PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		HDFHelper::writeUIntDataSet1D (nodeIdsDataSet, uibuf, nodeNumber,
		                               totalNodeNumber - nodeNumber);
		delete [] uibuf;	uibuf	= 0;

		// Ecriture des identifiants :
		size_type	count	= 0 != contiguous ? 1 : cellNumber;
		DataSet	cellIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
			cell2DGroup, DATASET_IDS_NAME, (hsize_t)count,
			PredType::NATIVE_UINT32, true, CELL2D_CHUNK_SIZE);
		if (1 == contiguous)
		{
			if (0 == pos)
			{
				uibuf		= new id_type [1];
				uibuf [0]	= (id_type)(m_mai->polygone (0)->id ( ));
				HDFHelper::writeUIntDataSet1D (cellIdsDataSet, uibuf, 1, 0);
			}	// // if (0 == pos)
		}	// if (1 == contiguous)
		else
		{
			uibuf	= new id_type [dataCount];
			for (ielm = first, i = 0; ielm < first + dataCount; ielm++, i++)
				uibuf [i]	= (id_type)(m_mai->polygone (ielm)->id ( ));
			HDFHelper::writeUIntDataSet1D (cellIdsDataSet, uibuf, dataCount,
			                               first);
		}	// else if (1 == contiguous)
		delete [] uibuf;	uibuf	= 0;
	}	// for (size_t pos = 0; pos * chunkSize < cellNumber; pos++)
	delete [] ibuf;		ibuf	= 0;
}	// _WriterMalipp::writeCell2D

//////////////////////////////////////////////////////////////////////////////
//! Ecriture des donnees sur les mailles 3D
void _WriterMalipp::writeCell3D ( )
{
	Group		cell3DGroup 	= m_meshGroup.openGroup (CELL_3D_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			cell3DGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	const id_type	cellNumber	= (id_type)m_mai->nb_polyedres ( );
	sizeAttribute.write (PredType::NATIVE_UINT32, &cellNumber);
	writeComposition (m_mai->composition_polyedres ( ),
	                  CELL3D_COMPOSITION_DATASET_NAME,cell3DGroup);

	if (0 == cellNumber)
		return;

	// Ecriture des types de maille 3D (TETRAEDRE, PYRAMIDE, PRISME, ...) :
	// chunkSize : pb, on ne connait pas encore le type des mailles et donc
	// le nombre de noeuds par maille ... Fixe arbitrairement a 12.
	size_t			chunkSize	=
						computeChunkSize (cellNumber, 12 * sizeof (id_type));
	size_type		ielm		= 0, i = 0;
	object_type*	ibuf		= 0;
	id_type*		uibuf		= 0;
	size_type		totalNodeNumber = 0;

	// Sauvegarde des identifiants : les identifiants sont ils contigus ?
	int			contiguous	= 1;
	for (ielm = 0; ielm < cellNumber - 1; ielm++)
		if (m_mai->polyedre(ielm)->id( ) + 1 != m_mai->polyedre(ielm+1)->id( ))
		{
			contiguous	= 0;
			break;
		}
	Attribute	contiguousAttribut	= HDFHelper::openOrCreateAttribute (
					cell3DGroup, CONTIGUOUS_IDS_ATTR_NAME,PredType::NATIVE_INT);
	contiguousAttribut.write (PredType::NATIVE_INT, &contiguous);

	// Ecriture des mailles 3D :
	for (size_t pos = 0; pos * chunkSize < cellNumber; pos++)
	{
		size_t	first		= pos * chunkSize;
		size_t	dataCount	= first + chunkSize >= cellNumber ?
							  cellNumber - first : chunkSize;
		size_t	nodeNumber	= 0;

		// Ecriture des types de mailles 3D :
		if (0 == ibuf)
			ibuf	= new object_type [chunkSize];
		for (ielm = 0, i = first; ielm < (size_type)dataCount; ielm++, i++)
		{
			ibuf [ielm]	= (object_type)(m_mai->polyedre (i)->type ( ));
			nodeNumber	+= Polyedre::PolyedreNbNode [ibuf [ielm]];
		}
		// Nombre de noeuds actuellement recenses :
		totalNodeNumber	+= nodeNumber;
		DataSet	cellTypesDataSet	= HDFHelper::openOrCreateDataSet1D (
				cell3DGroup, DATASET_TYPES_NAME, cellNumber,
				PredType::NATIVE_INT32, true, CELL3D_CHUNK_SIZE);
		HDFHelper::writeIntDataSet1D (cellTypesDataSet, ibuf, dataCount, first);

		// Ecriture des ids des noeuds :
		uibuf	= new id_type [nodeNumber];
  		size_type	indice	= 0;
		for (ielm = first; ielm < (size_type)(first + dataCount); ielm++)
   		 	for (i = 0; i < m_mai->polyedre (ielm)->nb_noeuds ( ); i++)
				uibuf [indice++] =
					(id_type)(m_mai->polyedre (ielm)->noeud (i)->id ( ) - 1);
		// On fait la mise a jour du nombre de noeuds total :
		DataSet	nodeIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
			cell3DGroup, DATASET_IDS_LIST_NAME, totalNodeNumber,
			PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		HDFHelper::writeUIntDataSet1D (nodeIdsDataSet, uibuf, nodeNumber,
		                               totalNodeNumber - nodeNumber);
		delete [] uibuf;	uibuf	= 0;

		// Ecriture des identifiants :
		size_type	count	= 0 != contiguous ? 1 : cellNumber;
		DataSet	cellIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
			cell3DGroup, DATASET_IDS_NAME, (hsize_t)count,
			PredType::NATIVE_UINT32, true, CELL3D_CHUNK_SIZE);
		if (1 == contiguous)
		{
			if (0 == pos)
			{
				uibuf		= new id_type [1];
				uibuf [0]	= (id_type)(m_mai->polyedre (0)->id ( ));
				HDFHelper::writeUIntDataSet1D (cellIdsDataSet, uibuf, 1, 0);
			}	// if (0 == pos)
		}	// if (1 == contiguous)
		else
		{
			uibuf	= new id_type [dataCount];
			for (ielm = first, i = 0; ielm < first + dataCount; ielm++, i++)
				uibuf [i]	= (id_type)(m_mai->polyedre (ielm)->id ( ));
			HDFHelper::writeUIntDataSet1D (cellIdsDataSet, uibuf, dataCount,
			                               first);
		}	// else if (1 == contiguous)
		delete [] uibuf;	uibuf	= 0;
	}	// for (size_t pos = 0; pos * chunkSize < cellNumber; pos++)
	delete [] ibuf;		ibuf	= 0;
}	// _WriterMalipp::writeCell3D

//////////////////////////////////////////////////////////////////////////////
//! Ecriture des donnees sur les nuages du maillage
void _WriterMalipp::writeNodeset ( )
{
	Group		nodeSetGroup 	= m_meshGroup.openGroup (NODESET_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			nodeSetGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	const id_type	nodeSetNumber	= (id_type)m_mai->nb_nuages ( );
	sizeAttribute.write (PredType::NATIVE_UINT32, &nodeSetNumber);

	// On boucle sur les nuages du maillage :
	for (size_type grp = 0; grp < nodeSetNumber; grp++)
	{
		_NuageInterne*	nuage	= m_mai->nuage (grp);

		// Creation de son groupe :
		string	nodeSetName	= SUBGROUP_PREFIX + to_str (grp);
		Group	nodeSetNGroup	=
					HDFHelper::openOrCreateGroup (nodeSetGroup, nodeSetName);

		// Ecriture du nom du nuage :
		Attribute	nameAttribute	=
			HDFHelper::openOrCreateStringAttribute (
						nodeSetNGroup, NAME_ATTR_NAME, ATTRIBUT_SIZE);
		HDFHelper::writeStringAttribute (nameAttribute, nuage->nom( ));

		writeComposition (nuage->composition ( ), NODE_COMPOSITION_DATASET_NAME,
		                  nodeSetNGroup);

		// Ecriture du nombre de noeuds :
		Attribute	sizeNAttribute	= HDFHelper::openOrCreateAttribute (
				nodeSetNGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		const id_type	nodeNumber	= (id_type)nuage->nb_noeuds ( );
		sizeNAttribute.write (PredType::NATIVE_UINT32, &nodeNumber);

		// Ecriture des identifiants des noeuds :
		DataSet		nodeIdsDataSet	=
			HDFHelper::openOrCreateDataSet1D (
				nodeSetNGroup, DATASET_IDS_LIST_NAME, nodeNumber,
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		size_t		chunkSize	= computeChunkSize(nodeNumber, sizeof(id_type));
		id_type*	uibuf = 0;
		for (size_t pos = 0; pos * chunkSize < nodeNumber; pos++)
		{
			size_t	first		= pos * chunkSize;
			size_t	dataCount	= first + chunkSize >= nodeNumber ?
								  nodeNumber - first : chunkSize;
			if (0 == uibuf)
				uibuf	= new id_type [dataCount];
			for (size_type i = 0, ielm = first; i < (size_type)dataCount;
			     i++, ielm++)
				uibuf [i]	= (id_type) (nuage->noeud(ielm)->id ( ) - 1);
			HDFHelper::writeUIntDataSet1D (nodeIdsDataSet, uibuf, dataCount,
			                               first);
		}	// for (size_t pos = 0; pos * chunkSize < nodeNumber; pos++)
		delete [] uibuf;

		// Ecriture des attributs du nuage :
		writeAttr (&nuage->att_noeuds ( ), nodeSetNGroup, DATA_NODE_GROUP_NAME);
	}	// for (size_type grp = 0; grp < nodeSetNumber; grp++)
}	// _WriterMalipp::writeNodeset

//////////////////////////////////////////////////////////////////////////////
//! Ecriture des donnees sur les lignes du maillage
void _WriterMalipp::writeLine ( )
{
	Group		lineGroup 	= m_meshGroup.openGroup (LINE_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			lineGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	const id_type	lineNumber	= (id_type)m_mai->nb_lignes ( );
	sizeAttribute.write (PredType::NATIVE_UINT32, &lineNumber);

	// On boucle sur les lignes du maillage :
	for (size_type grp = 0; grp < lineNumber; grp++)
	{
		_LigneInterne*	ligne	= m_mai->ligne (grp);

		// Creation de son groupe :
		string	lineName	= SUBGROUP_PREFIX + to_str (grp);
		Group	lineNGroup	= HDFHelper::openOrCreateGroup(lineGroup, lineName);

		// Ecriture du nom de la ligne :
		Attribute	nameAttribute	=
			HDFHelper::openOrCreateStringAttribute (
						lineNGroup, NAME_ATTR_NAME, ATTRIBUT_SIZE);
		HDFHelper::writeStringAttribute (nameAttribute, ligne->nom( ));

		// Ecriture du nombre de bras :
		Attribute	sizeNAttribute	= HDFHelper::openOrCreateAttribute (
				lineNGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		const id_type	edgeNumber	= (id_type)ligne->nb_bras ( );
		sizeNAttribute.write (PredType::NATIVE_UINT32, &edgeNumber);

		writeComposition (ligne->composition ( ), EDGE_COMPOSITION_DATASET_NAME,
		                  lineNGroup);

		// Ecriture des identifiants des bras :
		DataSet		edgeIdsDataSet	=
			HDFHelper::openOrCreateDataSet1D (
				lineNGroup, DATASET_IDS_LIST_NAME, edgeNumber,
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		size_t		chunkSize	= computeChunkSize(edgeNumber, sizeof(id_type));
		id_type*	uibuf = 0;
		for (size_t pos = 0; pos * chunkSize < edgeNumber; pos++)
		{
			size_t	first		= pos * chunkSize;
			size_t	dataCount	= first + chunkSize >= edgeNumber ?
								  edgeNumber - first : chunkSize;
			if (0 == uibuf)
				uibuf	= new id_type [dataCount];
			for (size_type i = 0, ielm = first; i < (size_type)dataCount;
			     i++, ielm++)
				uibuf [i]	= (id_type) (ligne->bras (ielm)->id ( ) - 1);
			HDFHelper::writeUIntDataSet1D (edgeIdsDataSet, uibuf, dataCount,
			                               first);
		}	// for (size_t pos = 0; pos * chunkSize < edgeNumber; pos++)
		delete [] uibuf;

		// Ecriture des attributs de la ligne :
		writeAttr (&ligne->att_noeuds ( ), lineNGroup, DATA_NODE_GROUP_NAME);
		writeAttr (&ligne->att_bras ( ), lineNGroup, DATA_EDGE_GROUP_NAME);
	}	// for (size_type grp = 0; grp < lineNumber; grp++)
}	// _WriterMalipp::writeLine

//////////////////////////////////////////////////////////////////////////////
//! Ecriture des donnees sur les surfaces du maillage
void _WriterMalipp::writeSurface ( )
{
	Group		surfaceGroup 	= m_meshGroup.openGroup (SURFACE_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			surfaceGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	const id_type	surfaceNumber	= (id_type)m_mai->nb_surfaces ( );
	sizeAttribute.write (PredType::NATIVE_UINT32, &surfaceNumber);

	// On boucle sur les surfaces du maillage :
	for (size_type grp = 0; grp < surfaceNumber; grp++)
	{
		_SurfaceInterne*	surface	= m_mai->surface (grp);

		// Creation de son groupe :
		string	surfaceName		= SUBGROUP_PREFIX + to_str (grp);
		Group	surfaceNGroup	=
					HDFHelper::openOrCreateGroup (surfaceGroup, surfaceName);

		// Ecriture du nom de la surface :
		Attribute	nameAttribute	=
			HDFHelper::openOrCreateStringAttribute (
						surfaceNGroup, NAME_ATTR_NAME, ATTRIBUT_SIZE);
		HDFHelper::writeStringAttribute (nameAttribute, surface->nom( ));

		// Ecriture du nombre de polygones :
		Attribute	sizeNAttribute	= HDFHelper::openOrCreateAttribute (
				surfaceNGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		const id_type	polygoneNumber	= (id_type)surface->nb_polygones ( );
		sizeNAttribute.write (PredType::NATIVE_UINT32, &polygoneNumber);

		writeComposition (surface->composition ( ),
		                  CELL2D_COMPOSITION_DATASET_NAME, surfaceNGroup);

		// Ecriture des identifiants des polygones :
		DataSet		polygoneIdsDataSet	=
			HDFHelper::openOrCreateDataSet1D (
				surfaceNGroup, DATASET_IDS_LIST_NAME, polygoneNumber,
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		size_t		chunkSize	=
						computeChunkSize (polygoneNumber, sizeof(id_type));
		id_type*	uibuf = 0;
		for (size_t pos = 0; pos * chunkSize < polygoneNumber; pos++)
		{
			size_t	first		= pos * chunkSize;
			size_t	dataCount	= first + chunkSize >= polygoneNumber ?
								  polygoneNumber - first : chunkSize;
			if (0 == uibuf)
				uibuf	= new id_type [dataCount];
			for (size_type i = 0, ielm = first; i < (size_type)dataCount;
			     i++, ielm++)
				uibuf [i]	= (id_type) (surface->polygone(ielm)->id ( ) - 1);
			HDFHelper::writeUIntDataSet1D (polygoneIdsDataSet, uibuf, dataCount,
			                               first);
		}	// for (size_t pos = 0; pos * chunkSize < polygoneNumber; pos++)
		delete [] uibuf;

		// Ecriture des attributs de la ligne :
		writeAttr (&surface->att_noeuds ( ),surfaceNGroup,DATA_NODE_GROUP_NAME);
		writeAttr (&surface->att_bras ( ), surfaceNGroup, DATA_EDGE_GROUP_NAME);
		writeAttr (&surface->att_polygones ( ), surfaceNGroup,
		           DATA_CELL_2D_GROUP_NAME);
	}	// for (size_type grp = 0; grp < surfaceNumber; grp++)
}	// _WriterMalipp::writeSurface

//////////////////////////////////////////////////////////////////////////////
//! Ecriture des donnees sur les volumes du maillage
void _WriterMalipp::writeVolume ( )
{
	Group		volumeGroup 	= m_meshGroup.openGroup (VOLUME_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			volumeGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	const id_type	volumeNumber	= (id_type)m_mai->nb_volumes ( );
	sizeAttribute.write (PredType::NATIVE_UINT32, &volumeNumber);

	// On boucle sur les volumes du maillage :
	for (size_type grp = 0; grp < volumeNumber; grp++)
	{
		_VolumeInterne*	volume	= m_mai->volume (grp);

		// Creation de son groupe :
		string	volumeName		= SUBGROUP_PREFIX + to_str (grp);
		Group	volumeNGroup	=
					HDFHelper::openOrCreateGroup (volumeGroup, volumeName);

		// Ecriture du nom du volume :
		Attribute	nameAttribute	=
			HDFHelper::openOrCreateStringAttribute (
						volumeNGroup, NAME_ATTR_NAME, ATTRIBUT_SIZE);
		HDFHelper::writeStringAttribute (nameAttribute, volume->nom( ));

		// Ecriture du nombre de polyedres :
		Attribute	sizeNAttribute	= HDFHelper::openOrCreateAttribute (
				volumeNGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		const id_type	polyedreNumber	= (id_type)volume->nb_polyedres ( );
		sizeNAttribute.write (PredType::NATIVE_UINT32, &polyedreNumber);

		writeComposition (volume->composition ( ),
		                  CELL3D_COMPOSITION_DATASET_NAME, volumeNGroup);

		// Ecriture des identifiants des polyedres :
		DataSet		polyedreIdsDataSet	=
			HDFHelper::openOrCreateDataSet1D (
				volumeNGroup, DATASET_IDS_LIST_NAME, polyedreNumber,
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		size_t		chunkSize	=
						computeChunkSize(polyedreNumber, sizeof(id_type));
		id_type*	uibuf		= 0;
		for (size_t pos = 0; pos * chunkSize < polyedreNumber; pos++)
		{
			size_t	first		= pos * chunkSize;
			size_t	dataCount	= first + chunkSize >= polyedreNumber ?
								  polyedreNumber - first : chunkSize;
			if (0 == uibuf)
				uibuf	= new id_type [dataCount];
			for (size_type i = 0, ielm = first; i < (size_type)dataCount;
			     i++, ielm++)
				uibuf [i]	= (id_type) (volume->polyedre (ielm)->id ( ) - 1);
			HDFHelper::writeUIntDataSet1D (polyedreIdsDataSet, uibuf,dataCount,
			                               first);
		}	// for (size_t pos = 0; pos * chunkSize < polyedreNumber; pos++)
		delete [] uibuf;

		// Ecriture des attributs de la ligne :
		writeAttr (&volume->att_noeuds ( ),volumeNGroup,DATA_NODE_GROUP_NAME);
		writeAttr (&volume->att_bras ( ), volumeNGroup, DATA_EDGE_GROUP_NAME);
		writeAttr (&volume->att_polygones ( ), volumeNGroup,
		           DATA_CELL_2D_GROUP_NAME);
		writeAttr (&volume->att_polyedres ( ), volumeNGroup,
		           DATA_CELL_3D_GROUP_NAME);
	}	// for (size_type grp = 0; grp < volumeNumber; grp++)
}	// _WriterMalipp::writeVolume


//////////////////////////////////////////////////////////////////////////////
/*! Ecriture des attributs d'une entite du maillage dans le groupe name.
	Détruit préalablement l'ensemble des données du groupe.
*/
void _WriterMalipp::writeAttr (Liste_attribut* listeAttr, CommonFG& root,
                               const string& name)
{
	HDFHelper::removeGroup (root, name);
	Group	attrGroup	= HDFHelper::openOrCreateGroup (root, name);

	// Boucle sur les attributs : on ecrit d'abord le nombre d'attributs. S'il
	// est non nul, on ecrit les attributs.
	vector<string>  listenom		= listeAttr->liste ( );
	Attribute		sizeAttribute	= HDFHelper::openOrCreateAttribute (
						attrGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	const id_type	attrCount		= (id_type)listenom.size ( );
	sizeAttribute.write (PredType::NATIVE_UINT32, &attrCount);
	if (0 == attrCount)
		return;

	size_t			count		= 0;
	// Pour ecrire la liste des attributs :
	HDFAttributeDescriptor*	attrDescList	=
						new HDFAttributeDescriptor [listenom.size ( )];

	for (vector<string>::iterator iv = listenom.begin ( );
	     listenom.end ( ) != iv; iv++, count++)
	{
		Attribut::TYPE	type_att	= listeAttr->element(iv->c_str ( )).type( );
		size_type		nb_elements	=
							listeAttr->element (iv->c_str( )).nb_elements ( );
		Attribut		attr 		= listeAttr->element (iv->c_str ( ));
		strncpy (attrDescList [count].m_name, iv->c_str ( ), ATTRIBUT_SIZE);
		attrDescList [count].m_name [ATTRIBUT_SIZE - 1]	= '\0';
		attrDescList [count].m_type	= (int_2)type_att;
		attrDescList [count].m_size	= (id_type)nb_elements;

		// Ecriture par blocs :
		size_type	i = 0, ielm = 0;
		switch (type_att)
		{
			case Attribut::ENTIER		:
			{
				size_t		chunkSize	=
							computeChunkSize (nb_elements, sizeof (int_4));
				DataSet	dataSet	=
					HDFHelper::openOrCreateDataSet1D (
						attrGroup, *iv, nb_elements, PredType::NATIVE_INT32,
						false);
      			int_4*	ibuf	= 0;
				for (size_t pos = 0; pos * chunkSize < nb_elements; pos++)
				{
					size_t	first		= pos * chunkSize;
					size_t	dataCount	= first + chunkSize >= nb_elements ?
										  nb_elements - first : chunkSize;
					if (0 == ibuf)
						ibuf	= new int_4 [dataCount];
					for (i = 0, ielm = first; i < dataCount ; i++, ielm++)
						attr.get_val (ielm, ibuf [i]);
					try
					{
						HDFHelper::writeIntDataSet1D (dataSet, ibuf, dataCount,
						                              first);
					}
					catch (...)
					{
						delete [] ibuf;
						throw;
					}
				}	// for (size_t pos = 0; pos * chunkSize < nb_elements; ...
				delete [] ibuf;
			}
			break;	// case Attribut::ENTIER
			case Attribut::REEL			:
			{
				size_t		chunkSize	=
							computeChunkSize (nb_elements, sizeof (double));
				DataSet	dataSet	=
					HDFHelper::openOrCreateDataSet1D (
						attrGroup, *iv, nb_elements, PredType::NATIVE_DOUBLE,
						false);
      			double*	dbuf	= 0;
				for (size_t pos = 0; pos * chunkSize < nb_elements; pos++)
				{
					size_t	first		= pos * chunkSize;
					size_t	dataCount	= first + chunkSize >= nb_elements ?
										  nb_elements - first : chunkSize;
					if (0 == dbuf)
						dbuf	= new double [dataCount];
					for (i = 0, ielm = first; i < dataCount ; i++, ielm++)
        				attr.get_val (ielm, dbuf [i]);
					try
					{
						HDFHelper::writeDoubleDataSet1D (
									dataSet, dbuf, dataCount, first);
					}
					catch (...)
					{
						delete [] dbuf;
						throw;
					}
				}	// for (size_t pos = 0; pos * chunkSize < nb_elements; ...
				delete [] dbuf;
			}
			break;	// case Attribut::REEL
			case Attribut::CHAINE		:
			{
				// Recherche de la taille max des chaines :
				size_type	maxSize	= 0;
				string		str;
				size_type	i	= 0;
				for (i = 0; i < nb_elements ; i++)
				{
					attr.get_val (i, str);
					maxSize	= maxSize < str.length( ) ? str.length( ) : maxSize;
				}
				size_t	chunkSize	= computeChunkSize (nb_elements, maxSize);
				// Les chaines sont recopiees de facon contigues dans un
				// meme tableau ou elles auront toutes la meme taille :
				StrType		type (PredType::C_S1);
				type.setSize (maxSize);
				DataSet	dataSet	= HDFHelper::openOrCreateDataSet1D (
								attrGroup, *iv, nb_elements, type, false);
				char*	sbuf	= 0;
				for (size_t pos = 0; pos * chunkSize < nb_elements; pos++)
				{
					size_t	first		= pos * chunkSize;
					size_t	dataCount	= first + chunkSize >= nb_elements ?
										  nb_elements - first : chunkSize;
					if (0 == sbuf)
						sbuf	= new char [dataCount * maxSize];
					char*	ptr	= sbuf;
					memset (sbuf, '\0', dataCount * maxSize);
					for (i = 0, ielm = first; i < dataCount ;
					     i++, ielm++, ptr += maxSize)
					{
						attr.get_val (ielm, str);
						strncpy (ptr, str.c_str ( ), maxSize);
					}
					try
					{
						HDFHelper::writeDataSet1D (
										dataSet, sbuf, dataCount, first);
					}
					catch (...)
					{
						delete [] sbuf;
						throw;
					}
				}	// for (size_t pos = 0; pos * chunkSize < nb_elements; ...
				delete [] sbuf;
			}
			break;	// case Attribut::CHAINE
			case Attribut::VECT2D		:
			{
				size_t		chunkSize	=
							computeChunkSize (nb_elements, 2 * sizeof (double));
				DataSet	dataSet	=
					HDFHelper::openOrCreateDataSet2D (
						attrGroup, *iv, nb_elements, PredType::NATIVE_DOUBLE,
						false);
      			double*	dbuf	= 0;

				for (size_t pos = 0; pos * chunkSize < nb_elements; pos++)
				{
					size_t	first		= pos * chunkSize;
					size_t	dataCount	= first + chunkSize >= nb_elements ?
										  nb_elements - first : chunkSize;
					if (0 == dbuf)
						dbuf	= new double [2 * dataCount];
					try
					{
						for (i = 0, ielm = first; i < dataCount ; i++, ielm++)
						{
							Vect2d	vec2d;
							attr.get_val (ielm, vec2d);
							dbuf [2*i]		= vec2d.x ( );
							dbuf [2*i + 1]	= vec2d.y ( );
						}	// for (size_type i = 0; i < dataCount ; i++)

						HDFHelper::writeDoubleDataSet2D (
									dataSet, dbuf, dataCount, first);
					}
					catch (...)
					{
						delete [] dbuf;
						throw;
					}
				}	// for (size_t pos = 0; pos * chunkSize < nb_elements; ...
				delete [] dbuf;
			}
			break;	// case Attribut::VECT2D
			case Attribut::VECT3D		:
			{
				size_t		chunkSize	=
							computeChunkSize (nb_elements, 3 * sizeof (double));
				DataSet	dataSet	=
					HDFHelper::openOrCreateDataSet3D (
						attrGroup, *iv, nb_elements, PredType::NATIVE_DOUBLE,
						false);
      			double*	dbuf	= 0;

				for (size_t pos = 0; pos * chunkSize < nb_elements; pos++)
				{
					size_t	first		= pos * chunkSize;
					size_t	dataCount	= first + chunkSize >= nb_elements ?
										  nb_elements - first : chunkSize;
					if (0 == dbuf)
						dbuf	= new double [3 * dataCount];
					try
					{
						for (i = 0, ielm = first; i < dataCount ; i++, ielm++)
						{
							Vect3d	vec3d;
							attr.get_val (ielm, vec3d);
							dbuf [3*i]		= vec3d.x ( );
							dbuf [3*i + 1]	= vec3d.y ( );
							dbuf [3*i + 2]	= vec3d.z ( );
						}	// for (size_type i = 0; i < dataCount ; i++)

						HDFHelper::writeDoubleDataSet3D (
									dataSet, dbuf, dataCount, first);
					}
					catch (...)
					{
						delete [] dbuf;
						throw;
					}
				}	// for (size_t pos = 0; pos * chunkSize < nb_elements; ...
				delete [] dbuf;
			}
			break;	// case Attribut::VECT3D
			default						:
				throw write_erreur ("Ecriture de l'attribut " + *iv +
					" impossible : type " + to_str (type_att) + " inconnu.");
		}	// switch (type_att)
	}	// for (vector<string>::iterator iv = listenom.begin ( ); ...

	// On ecrit la liste des attributs :
	HDFAttributeDescriptorType	attrDescrType;
	DataSet		attrListDataSet	= HDFHelper::openOrCreateDataSet1D (
					attrGroup, DATASET_ATTR_LIST_NAME, listenom.size ( ),
					attrDescrType, false);
	HDFHelper::writeDataSet1D (attrListDataSet, attrDescList, listenom.size( ));
	delete [] attrDescList;
}	// _WriterMalipp::writeAttr

//////////////////////////////////////////////////////////////////////////////
_WriterMalipp::_WriterMalipp (_MaillageInterne* mesh, const string& nom_fichier,
                              format_t format, size_type num, bool forceZlib)
	: _Writer (mesh, nom_fichier, format),
	  m_fileName (nom_fichier), m_meshNum (num), m_hdfFile ( ), m_meshGroup ( ),
	  m_forceZlib (forceZlib)
{
}	// _WriterMalipp::_WriterMalipp

//////////////////////////////////////////////////////////////////////////////
_WriterMalipp::~_WriterMalipp ( )
{
}	// _WriterMalipp::~_WriterMalipp

//////////////////////////////////////////////////////////////////////////////
void _WriterMalipp::write (format_t format)
{
	if ((MALIPP != format) && (MALIPP_Z != format))
		throw write_erreur ("Format invalide pour ce type de writer.");

	ecrire ( );
}	// _WriterMalipp::write


//////////////////////////////////////////////////////////////////////////////
void _WriterMalipp::writeComposition (const Composition& composition,
                                      const string& name, CommonFG& root)
{
	// Le groupe contenant la composition du groupe de donnees :
	Group		compositionGroup	= HDFHelper::openOrCreateGroup (root, name);

	// Le nombre d'elements du groupe :
	Attribute	sizeAttribute		=
		HDFHelper::openOrCreateAttribute (compositionGroup, SIZE_ATTR_NAME,
		                                  PredType::NATIVE_UINT32);
	const id_type	number	= composition.nb_elements;
	sizeAttribute.write (PredType::NATIVE_UINT32, &number);

	// Le detail de la composition du groupe :
	Attribute	detailedAttribute	=
		HDFHelper::openOrCreateAttribute (compositionGroup, DETAILED_ATTR_NAME,
		                                  PredType::NATIVE_HBOOL);
	hbool_t		detailed	= false == composition.renseigne ? 0 : 1;
	detailedAttribute.write (PredType::NATIVE_HBOOL, &detailed);
	hsize_t		dims [1];
	dims [0]	= MAX_NOEUDS;
	id_type	detail [MAX_NOEUDS];
	for (hsize_t i = 0; i < MAX_NOEUDS; i++)
		detail [i]	= composition.detail [i];
	Attribute	compositionAttr	=
		HDFHelper::openOrCreateArrayAttribute (compositionGroup,
				COMPOSITION_ATTR_NAME, PredType::NATIVE_UINT32, 1, dims);
	compositionAttr.write (compositionAttr.getDataType ( ), detail);
}	// _WriterMalipp::writeComposition


static size_t computeChunkSize (size_t count, size_t elementSize)
{
// Pour forcer la creation de blocs lors de tests :
//	return count < 10 ? count : count / 10;

	// Taille a partir de laquelle on procede par blocs pour les operations
	// de lecture/ecriture (correspond a 100.000 mailles, ou les tableaux
	// x, y, z contiennent des double)
	static const size_t	threshold		= 3 * sizeof (double) * 100000;
	if ((count * elementSize) <= threshold)
		return count;

	// On accepte ici d'allouer, au plus, des blocs de taille egale a
	// count / (chunkDivider * elementSize) octets.
	const size_t	chunkDivider	= 10;

	return count / (chunkDivider * elementSize);
}	// computeChunkSize

END_NAMESPACE_LIMA

#endif
