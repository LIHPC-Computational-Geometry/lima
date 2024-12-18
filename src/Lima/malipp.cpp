#ifdef __INTERNE_MALIPP
/*****************************************************************************\

 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : malipp.C
 Cree le : 04/01/2005
 Derniere modification le :
 Auteur : C. Pignerol
******************************************************************************/

/**
 * @file	malipp.C
 * @brief	Classe de lecture de fichiers mali++ (.mli).
 */
#include <Lima/malipp.h>
#include <Lima/erreur.h>
#include "LimaP/MutableString.h"
#include "LimaP/hdf_helper.h"
// Rem : FILE_HEADER_GROUP_NAME et autres sont laisses dans reader_malipp.
// Ces informations n'ont pas a etre connues de l'exterieur.
#include "LimaP/reader_malipp.h"
#include <Lima/noeud.h>
#include <Lima/bras.h>
#include <Lima/polygone.h>
#include <Lima/polyedre.h>

#include <assert.h>
#include <unistd.h>
#include <iostream>
#include "LimaP/using_it.h"

#include <sys/stat.h>
#include <pwd.h>

using namespace std;

//#ifndef H5_NO_NAMESPACE
//using namespace H5;
//#endif  // H5_NO_NAMESPACE
using namespace HDF145;


#define BEGIN_TRY_CATCH_BLOCK(message)                                     \
	MutableString		errorMsg;                                          \
	bool				throwExc	= false;                               \
	errorMsg << #message;                                                  \
	try                                                                    \
	{

#define COMPLETE_TRY_CATCH_BLOCK                                           \
	}                                                                      \
	catch (const Exception& hdfExc)                                        \
	{                                                                      \
		throwExc	= true;                                                \
		errorMsg << "\n" << hdfExc.getDetailMsg ( );                       \
	}                                                                      \
	catch (const exception& exc)                                           \
	{                                                                      \
		throwExc	= true;                                                \
		errorMsg << exc.what ( );                                          \
	}                                                                      \
	catch (const erreur& limaExc)                                          \
	{                                                                      \
		throwExc	= true;                                                \
		errorMsg << limaExc.what ( );                                      \
	}                                                                      \
	catch (...)                                                            \
	{                                                                      \
		throwExc	= true;                                                \
		errorMsg << "erreur non documentee.";                              \
	}


BEGIN_NAMESPACE_LIMA


// ===========================================================================
//                    LA STRUCTURE HDFAttributeDescriptor
// ===========================================================================

HDFAttributeDescriptor::HDFAttributeDescriptor ( )
{
	memset (m_name, '\0', ATTRIBUT_SIZE);
	m_type	= -1;
	m_size	= 0;
}	// HDFAttributeDescriptor::HDFAttributeDescriptor


HDFAttributeDescriptor::HDFAttributeDescriptor (const HDFAttributeDescriptor& ad)
{
	for (size_type i = 0; i < ATTRIBUT_SIZE; i++)
		m_name [i]	= ad.m_name [i];
	m_type	= ad.m_type;
	m_size	= ad.m_size;
}	// HDFAttributeDescriptor::HDFAttributeDescriptor


HDFAttributeDescriptor& HDFAttributeDescriptor::operator = (const HDFAttributeDescriptor& ad)
{
	if (&ad != this)
	{
		for (size_type i = 0; i < ATTRIBUT_SIZE; i++)
			m_name [i]	= ad.m_name [i];
		m_type	= ad.m_type;
		m_size	= ad.m_size;
	}

	return *this;
}	// HDFAttributeDescriptor::operator =


// ===========================================================================
//                    LA CLASSE MaliPPReader::GroupeReader
// ===========================================================================


MaliPPReader::GroupeReader::GroupeReader (
		MaliPPReader* reader, const string& nom, const Group& group, 
		const string& composition)
	: m_reader (reader), m_nom (nom), m_groupe_hdf (group), m_composition ( )
{
	// Lecture de la composition :
	id_type		number			= 0;
	Attribute	sizeNAttribute	= m_groupe_hdf.openAttribute (SIZE_ATTR_NAME);
	sizeNAttribute.read (PredType::NATIVE_UINT32, &number);
	m_composition.nb_elements	= number;
	MaliPPReader::lire_composition (
			m_groupe_hdf, composition, m_reader->version_malipp_maillage ( ), 
			m_composition);
}	// GroupeReader::GroupeReader


MaliPPReader::GroupeReader::GroupeReader (const GroupeReader& reader)
	: m_reader (reader.reader ( )), m_nom (reader.nom ( )), 
	  m_groupe_hdf (reader.groupe_hdf ( )), 
	  m_composition (reader.composition ( ))
{
}	// GroupeReader::GroupeReader


MaliPPReader::GroupeReader& MaliPPReader::GroupeReader::operator = (const GroupeReader& reader)
{
	if (&reader != this)
	{
		m_reader				= reader.reader ( );
		m_nom					= reader.nom ( );
		m_groupe_hdf			= reader.groupe_hdf ( );
		m_composition			= reader.composition ( );
	}	// if (&reader != this)

	return *this;
}	// GroupeReader::GroupeReader


MaliPPReader::GroupeReader::~GroupeReader ( )
{
}	// GroupeReader::~GroupeReader


const string& MaliPPReader::GroupeReader::nom ( ) const
{
	return m_nom;
}	// GroupeReader::nom


const Composition& MaliPPReader::GroupeReader::composition ( ) const
{
	return m_composition;
}	// GroupeReader::composition


void MaliPPReader::GroupeReader::lire_liste_attributs (
		MaliPPReader::ENTITE entite, vector<HDFAttributeDescriptor>& liste)
{
	Group	group	= groupe_attribut (entite);
	MaliPPReader::lire_liste_attributs (group, liste);
//	MaliPPReader::lire_liste_attributs (groupe_attribut (entite), liste);
}	// GroupeReader::lire_liste_attributs


void MaliPPReader::GroupeReader::lire_attr_entier (
		MaliPPReader::ENTITE entite, const HDFAttributeDescriptor& desc,
		size_type premier, size_type nombre, int_4*& tableau)
{
	Group	group	= groupe_attribut (entite);
	MaliPPReader::lire_attr_entier (group, desc, premier, nombre, tableau);
}	// GroupeReader::lire_attr_entier


void MaliPPReader::GroupeReader::lire_attr_reel (
		MaliPPReader::ENTITE entite, const HDFAttributeDescriptor& desc,
		size_type premier, size_type nombre, double*& tableau)
{
	Group	group	= groupe_attribut (entite);
	MaliPPReader::lire_attr_reel (group, desc, premier, nombre, tableau);
}	// GroupeReader::lire_attr_reel


void MaliPPReader::GroupeReader::lire_attr_vect_2d (
		MaliPPReader::ENTITE entite, const HDFAttributeDescriptor& desc,
		size_type premier, size_type nombre, double*& tableau)
{
	Group	group	= groupe_attribut (entite);
	MaliPPReader::lire_attr_vect_2d (group, desc, premier, nombre, tableau);
}	// GroupeReader::lire_attr_vect_2d


void MaliPPReader::GroupeReader::lire_attr_vect_3d (
		MaliPPReader::ENTITE entite, const HDFAttributeDescriptor& desc,
		size_type premier, size_type nombre, double*& tableau)
{
	Group	group	= groupe_attribut (entite);
	MaliPPReader::lire_attr_vect_3d (group, desc, premier, nombre, tableau);
}	// GroupeReader::lire_attr_vect_3d


size_type MaliPPReader::GroupeReader::longueur_chaines (
						ENTITE entite, const HDFAttributeDescriptor& desc)
{
	Group	group	= groupe_attribut (entite);
	return MaliPPReader::longueur_chaines (group, desc);
}	// MaliPPReader::longueur_chaines


void MaliPPReader::GroupeReader::lire_attr_chaine (
		MaliPPReader::ENTITE entite, const HDFAttributeDescriptor& desc,
		size_type premier, size_type nombre, vector<IN_STD string>& chaines)
{
	Group	group	= groupe_attribut (entite);
	MaliPPReader::lire_attr_chaine (group, desc, premier, nombre, chaines);
}	// GroupeReader::lire_attr_chaine


Group& MaliPPReader::GroupeReader::groupe_hdf ( )
{
	return m_groupe_hdf;
}	// GroupeReader::groupe_hdf


const Group& MaliPPReader::GroupeReader::groupe_hdf ( ) const
{
	return m_groupe_hdf;
}	// GroupeReader::groupe_hdf


Group MaliPPReader::GroupeReader::groupe_attribut (ENTITE entite)
{
	evaluer_entite (entite);

	switch (entite)
	{
		case NOEUDS	:
			return groupe_hdf ( ).openGroup (DATA_NODE_GROUP_NAME);
		case BRAS	:
			return groupe_hdf ( ).openGroup (DATA_EDGE_GROUP_NAME);
		case POLYGONES	:
			return groupe_hdf ( ).openGroup (DATA_CELL_2D_GROUP_NAME);
		case POLYEDRES	:
			return groupe_hdf ( ).openGroup (DATA_CELL_3D_GROUP_NAME);
	}

	throw read_erreur ("Type d'entité de rattachement inconnu.");
}	// GroupeReader::groupe_attribut


// ===========================================================================
//                    LA CLASSE MaliPPReader::NuageReader
// ===========================================================================


MaliPPReader::NuageReader::NuageReader (
			MaliPPReader* reader, const string& nom, const Group& group)
	: GroupeReader (reader, nom, group, NODE_COMPOSITION_DATASET_NAME)
{
}	// NuageReader::NuageReader


MaliPPReader::NuageReader::NuageReader (const MaliPPReader::NuageReader& reader)
	: GroupeReader (reader)
{
}	// NuageReader::NuageReader


MaliPPReader::NuageReader& MaliPPReader::NuageReader::operator = (
										const MaliPPReader::NuageReader& reader)
{
	if (& reader != this)
		MaliPPReader::GroupeReader::operator = (reader);

	return *this;
}	// NuageReader::operator =


MaliPPReader::NuageReader::~NuageReader ( )
{
}	// NuageReader::~NuageReader


size_type* MaliPPReader::NuageReader::allouer_tampon_ids (size_type nb_elements)
{
	return new size_type [nb_elements];
}	// NuageReader::allouer_tampon_ids


size_type MaliPPReader::NuageReader::lire_noeuds_ids (
				size_type premier, size_type nombre, size_type*& tableau)
{
	DataSet	xCoordDataSet, yCoordDataSet, zCoordDataSet;
	
	const Composition&	comp	= composition ( );
	size_t				count		= premier + nombre < comp.nb_elements ?
									  nombre : comp.nb_elements - premier;
	if (0 == count)
		return 0;

	id_type*	nodeIds			= new id_type [count];
	DataSet		nodeIdsDataSet	= 
						groupe_hdf ( ).openDataSet(DATASET_IDS_LIST_NAME);
	HDFHelper::readUIntDataSet1D (nodeIdsDataSet, nodeIds, count, premier);
	for (size_t i = 0; i < count; i++)
		tableau [i]	= nodeIds [i] + 1;
	delete [] nodeIds;

	return count;
}	// NuageReader::lire_noeuds_ids


void MaliPPReader::NuageReader::evaluer_entite (MaliPPReader::ENTITE entite)
{
	if (MaliPPReader::NOEUDS != entite)
		throw read_erreur ("Entite invalide pour un nuage");
}	// NuageReader::evaluer_entite


// ===========================================================================
//                    LA CLASSE MaliPPReader::LigneReader
// ===========================================================================


MaliPPReader::LigneReader::LigneReader (
		MaliPPReader* reader, const string& nom, const Group& group)
	: GroupeReader (reader, nom, group, EDGE_COMPOSITION_DATASET_NAME)
{
}	// LigneReader::LigneReader


MaliPPReader::LigneReader::LigneReader (const MaliPPReader::LigneReader& reader)
	: GroupeReader (reader)
{
}	// LigneReader::LigneReader


MaliPPReader::LigneReader& MaliPPReader::LigneReader::operator = (
									const MaliPPReader::LigneReader& reader)
{
	if (& reader != this)
		MaliPPReader::GroupeReader::operator = (reader);

	return *this;
}	// LigneReader::operator =


MaliPPReader::LigneReader::~LigneReader ( )
{
}	// LigneReader::~LigneReader


size_type* MaliPPReader::LigneReader::allouer_tampon_ids (size_type nb_elements)
{
	return new size_type [nb_elements];
}	// LigneReader::allouer_tampon_ids


size_type MaliPPReader::LigneReader::lire_bras_ids (
					size_type premier, size_type nombre, size_type*& tableau)
{
	const Composition&	comp	= composition ( );
	size_t				count	= premier + nombre < comp.nb_elements ?
								  nombre : comp.nb_elements - premier;
	if (0 == count)
		return 0;

	id_type*	edgeIds			= new id_type [count];
	DataSet		edgeIdsDataSet	= 
					groupe_hdf ( ).openDataSet(DATASET_IDS_LIST_NAME);
	HDFHelper::readUIntDataSet1D (edgeIdsDataSet, edgeIds, count, premier);
	for (size_t i = 0; i < count; i++)
		tableau [i]	= edgeIds [i] + 1;
	delete [] edgeIds;

	return count;
}	// LigneReader::lire_bras_ids


void MaliPPReader::LigneReader::evaluer_entite (MaliPPReader::ENTITE entite)
{
	if ((MaliPPReader::NOEUDS != entite) && (MaliPPReader::BRAS != entite))
		throw read_erreur ("Entite invalide pour une ligne");
}	// LigneReader::evaluer_entite


// ===========================================================================
//                    LA CLASSE MaliPPReader::SurfaceReader
// ===========================================================================


MaliPPReader::SurfaceReader::SurfaceReader (
			MaliPPReader* reader, const string& nom, const Group& group)
	: GroupeReader (reader, nom, group, CELL2D_COMPOSITION_DATASET_NAME)
{
}	// SurfaceReader::SurfaceReader


MaliPPReader::SurfaceReader::SurfaceReader (
									const MaliPPReader::SurfaceReader& reader)
	: GroupeReader (reader)
{
}	// SurfaceReader::SurfaceReader


MaliPPReader::SurfaceReader& MaliPPReader::SurfaceReader::operator = (
									const MaliPPReader::SurfaceReader& reader)
{
	if (& reader != this)
		MaliPPReader::GroupeReader::operator = (reader);

	return *this;
}	// SurfaceReader::operator =


MaliPPReader::SurfaceReader::~SurfaceReader ( )
{
}	// SurfaceReader::~SurfaceReader


size_type* MaliPPReader::SurfaceReader::allouer_tampon_ids (size_type nb_elements)
{
	return new size_type [nb_elements];
}	// SurfaceReader::allouer_tampon_ids


size_type MaliPPReader::SurfaceReader::lire_mailles_ids (
					size_type premier, size_type nombre, size_type*& tableau)
{
	const Composition&	comp	= composition ( );
	size_t				count	= premier + nombre < comp.nb_elements ?
								  nombre : comp.nb_elements - premier;
	if (0 == count)
		return 0;

	id_type*			cellIds		= new id_type [count];
	DataSet	cellIdsDataSet	= groupe_hdf ( ).openDataSet(DATASET_IDS_LIST_NAME);
	HDFHelper::readUIntDataSet1D (cellIdsDataSet, cellIds, count, premier);
	for (size_t i = 0; i < count; i++)
		tableau [i]	= cellIds [i] + 1;
	delete [] cellIds;

	return count;
}	// SurfaceReader::lire_mailles_ids


void MaliPPReader::SurfaceReader::evaluer_entite (MaliPPReader::ENTITE entite)
{
	if ((MaliPPReader::NOEUDS != entite) && (MaliPPReader::BRAS != entite) &&
	    (MaliPPReader::POLYGONES != entite))
		throw read_erreur ("Entite invalide pour une surface");
}	// SurfaceReader::evaluer_entite

// ===========================================================================
//                    LA CLASSE MaliPPReader::VolumeReader
// ===========================================================================


MaliPPReader::VolumeReader::VolumeReader (
				MaliPPReader* reader, const string& nom, const Group& group)
	: GroupeReader (reader, nom, group, CELL3D_COMPOSITION_DATASET_NAME)
{
}	// VolumeReader::VolumeReader


MaliPPReader::VolumeReader::VolumeReader (
								const MaliPPReader::VolumeReader& reader)
	: GroupeReader (reader)
{
}	// VolumeReader::VolumeReader


MaliPPReader::VolumeReader& MaliPPReader::VolumeReader::operator = (
									const MaliPPReader::VolumeReader& reader)
{
	if (& reader != this)
		MaliPPReader::GroupeReader::operator = (reader);

	return *this;
}	// VolumeReader::operator =


MaliPPReader::VolumeReader::~VolumeReader ( )
{
}	// VolumeReader::~VolumeReader


size_type* MaliPPReader::VolumeReader::allouer_tampon_ids (size_type nb_elements)
{
	return new size_type [nb_elements];
}	// VolumeReader::allouer_tampon_ids


size_type MaliPPReader::VolumeReader::lire_mailles_ids (
					size_type premier, size_type nombre, size_type*& tableau)
{
	const Composition&	comp	= composition ( );
	size_t				count		= premier + nombre < comp.nb_elements ?
									  nombre : comp.nb_elements - premier;
	if (0 == count)
		return 0;

	id_type*			cellIds		= new id_type [count];
	DataSet	cellIdsDataSet	= groupe_hdf ( ).openDataSet(DATASET_IDS_LIST_NAME);
	HDFHelper::readUIntDataSet1D (cellIdsDataSet, cellIds, count, premier);
	for (size_t i = 0; i < count; i++)
		tableau [i]	= cellIds [i] + 1;
	delete [] cellIds;

	return count;
}	// VolumeReader::lire_mailles_ids


void MaliPPReader::VolumeReader::evaluer_entite (MaliPPReader::ENTITE entite)
{
	if ((MaliPPReader::NOEUDS != entite) && (MaliPPReader::BRAS != entite) &&
	    (MaliPPReader::POLYGONES != entite) && 
	    (MaliPPReader::POLYEDRES != entite))
		throw read_erreur ("Entite invalide pour un volume");
}	// VolumeReader::evaluer_entite


// ===========================================================================
//                          LA CLASSE MaliPPReader
// ===========================================================================


MaliPPReader::MaliPPReader (const string& nom_fichier, size_type num)
	: m_nom_fichier (nom_fichier), m_numero_maillage (num),
	  m_lima_version_fichier ("0.0.0"), m_mli_version_fichier ("0.0.0"),
	  m_lima_version_maillage ("0.0.0"), m_mli_version_maillage ("0.0.0"),
	  m_titre ( ), m_date ( ), m_dimension (D1), m_geometrie (NORMAL), 
	  m_systeme_coords (CARTESIEN),
	  m_unite_angle (0.), m_unite_longueur (0.),
	  m_composition_noeuds ( ), m_composition_bras ( ),
	  m_composition_polygones ( ), m_composition_polyedres ( ),
	  m_fichier_hdf ( ), m_maillage_hdf ( ),
	  m_indice_ids_noeuds_polygone (0), m_pos_ids_noeuds_polygone (0),
	  m_indice_ids_noeuds_polyedre (0), m_pos_ids_noeuds_polyedre (0)
{
	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	BEGIN_TRY_CATCH_BLOCK("Impossible de lire le fichier ")
	errorMsg << m_nom_fichier << " : ";

	// Ouverture du fichier :
	m_fichier_hdf.reset (new H5File (m_nom_fichier, H5F_ACC_RDONLY));

	// Lecture de l'entete du fichier :
	Group	header	= m_fichier_hdf->openGroup (FILE_HEADER_GROUP_NAME);
	string	version_lima	= HDFHelper::readStringAttribute (header, LIMA_VERSION_FIELD_NAME);
	if (0 == version_lima.compare (0, 5, "Lima."))
		m_lima_version_fichier	= Version (version_lima.substr (5));
	else
		m_lima_version_fichier	= Version (version_lima);
	m_mli_version_fichier	= Version (HDFHelper::readStringAttribute (header, FILE_VERSION_FIELD_NAME));

	if (m_mli_version_fichier <= Version ("1.1.0"))
	{	// On est ici en HDF >= 1.8.3. Les fichiers dont mli version <= 1.1.0
		// ont été générés en HDF 1.4.3 => on les transforme temporairement en
		// HDF 1.8.3 pour pouvoir les relire après car la compatibilité
		// ascendante 1.4 -> 1.8 n'a pas été maintenue par HDF.

		// En faire de même pour la sauvegarde.
	}	// if (m_mli_version_fichier <= Version ("1.1.0"))

	COMPLETE_TRY_CATCH_BLOCK

	if (true == throwExc)
		throw read_erreur (errorMsg.str ( ));

	lire_entete_maillage ( );
	lire_composition_maillage ( );
}	// MaliPPReader::MaliPPReader


MaliPPReader::MaliPPReader (const MaliPPReader&)
	: m_nom_fichier ( ), m_numero_maillage (0),
	  m_lima_version_fichier ("0.0.0"), m_mli_version_fichier ("0.0.0"),
	  m_lima_version_maillage ("0.0.0"), m_mli_version_maillage ("0.0.0"),
	  m_titre ( ), m_date ( ), m_dimension (D1), m_geometrie (NORMAL), 
	  m_systeme_coords (CARTESIEN),
	  m_unite_angle (0.), m_unite_longueur (0.),
	  m_composition_noeuds ( ), m_composition_bras ( ),
	  m_composition_polygones ( ), m_composition_polyedres ( ),
	  m_indice_ids_noeuds_polygone (0), m_pos_ids_noeuds_polygone (0),
	  m_indice_ids_noeuds_polyedre (0), m_pos_ids_noeuds_polyedre (0)
{
}	// MaliPPReader::MaliPPReader


MaliPPReader& MaliPPReader::operator = (const MaliPPReader&)
{
	return *this;
}	// MaliPPReader::operator =


MaliPPReader::~MaliPPReader ( )
{
	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_fichier_hdf.reset (0);
}	// MaliPPReader::~MaliPPReader


Version MaliPPReader::lireVersionHDF ( )
{
	unsigned int	major = 0, minor = 0, release = 0;
	MutableString	version;
	H5Library::getLibVersion (major, minor, release);
	version << major << "." << minor << "." << release;

	return Version (version);
}	// MaliPPReader::lireVersionHDF


void MaliPPReader::lire_composition (
	CommonFG& root, const string& nom, const Version& version, 
	Composition& composition)
{
	composition.renseigne	= false;

	if (version < Version ("1.1.0"))
		return;

	// Le nombre d'elements du groupe :
	Group		compositionGroup	= root.openGroup (nom);
	Attribute	sizeAttribute		= 
						compositionGroup.openAttribute (SIZE_ATTR_NAME);
	id_type		number				= 0;
	sizeAttribute.read (PredType::NATIVE_UINT32, &number);
	composition.nb_elements = number;

	// Le detail de la composition du groupe :
	Attribute	detailedAttribute	=
					compositionGroup.openAttribute (DETAILED_ATTR_NAME);
	hbool_t		detailed (0);
	detailedAttribute.read (PredType::NATIVE_HBOOL, &detailed);
	composition.renseigne	= 0 == detailed ? false : true;
	hsize_t		dims [1];
	dims [0]	= MAX_NOEUDS;
	id_type 	detail [MAX_NOEUDS];
	hsize_t		i	= 0;
	for (i = 0; i < MAX_NOEUDS; i++)
		detail [i]	= composition.detail [i]	= 0;
	if (false == composition.renseigne)
		return;

#ifdef HDF5_1_4_3
	ArrayType	dataType (PredType::NATIVE_UINT32.getId ( ), 1, dims, NULL);
#else	// => HDF5 > 1.4.3
	ArrayType	dataType (PredType::NATIVE_UINT32, 1, dims);
#endif	// HDF5_1_4_3

	Attribute	compositionAttr	=
					compositionGroup.openAttribute (COMPOSITION_ATTR_NAME);
	compositionAttr.read (dataType, detail);
	for (i = 0; i < MAX_NOEUDS; i++)
		composition.detail [i]	= detail [i];
}	// MaliPPReader::lire_composition


void MaliPPReader::lire_liste_attributs (
					Group& group, vector<HDFAttributeDescriptor>& liste)
{
	liste.clear ( );
	Attribute	sizeAttribute	= group.openAttribute (SIZE_ATTR_NAME);
	id_type		attrCount		= 0;
	sizeAttribute.read (PredType::NATIVE_UINT32, &attrCount);
	if (0 == attrCount)
		return;

	// On recupere la liste des attributs (noms/types) :
	DataSet	attrDescDataSet	= group.openDataSet(DATASET_ATTR_LIST_NAME);
	HDFAttributeDescriptorType	attrDescrType;
	HDFAttributeDescriptor*		attrDescriptors	=
									new HDFAttributeDescriptor [attrCount];

	try
	{
		attrDescDataSet.read (attrDescriptors, attrDescrType);

		for (size_t i = 0; i < attrCount; i++)
			liste.push_back (attrDescriptors[i]);
		delete [] attrDescriptors;
	}
	catch (...)
	{
		delete [] attrDescriptors;
		throw;
	}
}	// MaliPPReader::lire_liste_attributs


void MaliPPReader::lire_attr_entier (
					Group& group, const HDFAttributeDescriptor& desc, 
					size_type premier, size_type nombre, int_4*& tableau)
{
	DataSet	dataSet	= group.openDataSet (desc.m_name);
	HDFHelper::readIntDataSet1D (dataSet, tableau, nombre, premier);
}	// MaliPPReader::lire_attr_entier


void MaliPPReader::lire_attr_reel (
					Group& group, const HDFAttributeDescriptor& desc, 
					size_type premier, size_type nombre, double*& tableau)
{
	DataSet	dataSet	= group.openDataSet (desc.m_name);
	HDFHelper::readDoubleDataSet1D (dataSet, tableau, nombre, premier);
}	// MaliPPReader::lire_attr_reel


void MaliPPReader::lire_attr_vect_2d (
					Group& group, const HDFAttributeDescriptor& desc, 
					size_type premier, size_type nombre, double*& tableau)
{
	DataSet	dataSet	= group.openDataSet (desc.m_name);
	HDFHelper::readDoubleDataSet2D (dataSet, tableau, nombre, premier);
}	// MaliPPReader::lire_attr_vect_2d


void MaliPPReader::lire_attr_vect_3d (
					Group& group, const HDFAttributeDescriptor& desc, 
					size_type premier, size_type nombre, double*& tableau)
{
	DataSet	dataSet	= group.openDataSet (desc.m_name);
	HDFHelper::readDoubleDataSet3D (dataSet, tableau, nombre, premier);
}	// MaliPPReader::lire_attr_vect_3d


size_type MaliPPReader::longueur_chaines (
				Group& group, const HDFAttributeDescriptor& desc)
{
	DataSet		dataSet	= group.openDataSet (desc.m_name);
	DataType	strType	= dataSet.getDataType ( );

	return strType.getSize ( );
}	// MaliPPReader::longueur_chaines


void MaliPPReader::lire_attr_chaine (
				Group& group, const HDFAttributeDescriptor& desc, 
				size_type premier, size_type nombre, vector<string>& chaines)
{
	DataSet	dataSet	= group.openDataSet (desc.m_name);
	HDFHelper::readStringDataSet (dataSet, chaines, nombre, premier);
}	// MaliPPReader::lire_attr_chaine


size_type* MaliPPReader::allouer_tampon_ids (size_type nb_elements)
{
	return new size_type [nb_elements];
}	// MaliPPReader::allouer_tampon_ids


double* MaliPPReader::allouer_tampon_coords (size_type nb_elements)
{
	switch (dimension ( ))
	{
		case D1	: return new double [nb_elements];
		case D2	: return new double [2 * nb_elements];
		case D3	: return new double [3 * nb_elements];
	}	// switch (dimension ( ))
}	// MaliPPReader::allouer_tampon_coords


size_type MaliPPReader::lire_noeuds (
		size_type premier, size_type nombre, size_type*& ids, double*& coords)
{
	const Composition&	comp	= composition_noeuds ( );
	size_t				count	= premier + nombre < comp.nb_elements ?
								  nombre : comp.nb_elements - premier;
	if (0 == count)
		return 0;

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	id_type*	hdf_ids	= 0;
	double		*xBuffer	= 0, *yBuffer	= 0, *zBuffer	= 0;

	BEGIN_TRY_CATCH_BLOCK("Lecture des noeuds impossible : ")

	// Ouverture du groupe contenant les noeuds du maillage :
	Group		nodeGroup	= m_maillage_hdf.openGroup (NODE_GROUP_NAME);

	// Lecture des noeuds :
	const dim_t	dim	= dimension ( );	// CP, v 6.27.1, coverity
//	switch (dimension ( ))
	switch (dim)	// CP, v 6.27.1, coverity
	{
		case	D3	:
		{
			DataSet	zCoordDataSet	= 
							nodeGroup.openDataSet (DATASET_COORD_Z_NAME);
			zBuffer		= new double [count];
			HDFHelper::readDoubleDataSet1D (
									zCoordDataSet, zBuffer, count, premier);
		}
		case	D2	:
		{
			DataSet	yCoordDataSet	= 
							nodeGroup.openDataSet (DATASET_COORD_Y_NAME);
			yBuffer		= new double [count];
			HDFHelper::readDoubleDataSet1D (
									yCoordDataSet, yBuffer, count, premier);
		}
		case	D1	:
		{
			DataSet	xCoordDataSet	= 
							nodeGroup.openDataSet (DATASET_COORD_X_NAME);
			xBuffer		= new double [count];
			HDFHelper::readDoubleDataSet1D (
									xCoordDataSet, xBuffer, count, premier);
		}
		break;
		default		: 
			throw read_erreur ("Erreur interne en MaliPPReader::lire_noeuds.");
	}	// switch (dimension ( ))
	size_t	i = 0;
	switch (dim)	// CP, v 6.27.1, coverity
	{
		case	D1	:
		{
			for (size_t c = 0; c < count; c++)
				coords [i++]	= xBuffer [c];
		}
		break;
		case	D2	:
		{
			for (size_t c = 0; c < count; c++)
			{
				coords [i++]	= xBuffer [c];
				coords [i++]	= yBuffer [c];
			}
		}
		break;
		case	D3	:
		{
			for (size_t c = 0; c < count; c++)
			{
				coords [i++]	= xBuffer [c];
				coords [i++]	= yBuffer [c];
				coords [i++]	= zBuffer [c];
			}
		}
		break;
	}	// switch (dimension ( ))

	// Lecture ou non des identifiants :
	// Les identifiants sont ils contigus ?
	Attribute	contiguousAttribut	=
					nodeGroup.openAttribute (CONTIGUOUS_IDS_ATTR_NAME);
	int			contiguous	= 0;
	DataSet		idsDataSet	= nodeGroup.openDataSet (DATASET_IDS_NAME);
	contiguousAttribut.read (PredType::NATIVE_INT, &contiguous);
	hdf_ids		= new id_type [1 == contiguous ? 1 : count];
	if (1 == contiguous)
	{
		// Lecture du premier id :
		HDFHelper::readUIntDataSet1D (idsDataSet, hdf_ids, 1, 0);
		size_type	id	= hdf_ids [0] + premier;
		for (i = 0; i < count; i++)
			ids [i]	= id++;
	}
	else
	{
		DataSet		idsDataSet	= nodeGroup.openDataSet (DATASET_IDS_NAME);
		HDFHelper::readUIntDataSet1D (idsDataSet, hdf_ids, count, premier);
		for (i = 0; i < count; i++)
			ids [i]	= hdf_ids [i];
	}	// else if (1 == contiguous)

	COMPLETE_TRY_CATCH_BLOCK

	delete [] xBuffer;	xBuffer	= 0;
	delete [] yBuffer;	yBuffer	= 0;
	delete [] zBuffer;	zBuffer	= 0;
	delete [] hdf_ids;	hdf_ids	= 0;

	if (true == throwExc)
		throw read_erreur (errorMsg.str ( ));

	return count;
}	// MaliPPReader::lire_noeuds


size_type* MaliPPReader::allouer_tampon_bras (size_type nb_elements)
{
	return new size_type [3 * nb_elements];
}	// MaliPPReader::allouer_tampon_bras


size_type MaliPPReader::lire_bras (
				size_type premier, size_type nombre, size_type*& tableau)
{
	const Composition&	comp	= composition_bras ( );
	const size_type		count	= premier + nombre < comp.nb_elements ?
								  nombre : comp.nb_elements - premier;
	if (0 == count)
		return 0;

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	id_type*		nodeIds		= 0;
	id_type*		ids			= 0;
	BEGIN_TRY_CATCH_BLOCK("Lecture des bras impossible : ")

	// Ouverture du groupe contenant les polygones du maillage :
	Group		edgeGroup	= m_maillage_hdf.openGroup (EDGE_GROUP_NAME);

	size_type	n = 0, ielm	= 0;

	DataSet	nodeIdsDataSet	= edgeGroup.openDataSet (DATASET_IDS_LIST_NAME);
	nodeIds	= new id_type [2 * count];
	HDFHelper::readUIntDataSet1D (nodeIdsDataSet, nodeIds, 2 * count,
	                              2 * premier);

	// Lecture des identifiants : eviter de tester sans arret si 1 == contiguous
	Attribute	contiguousAttribut	= 
					edgeGroup.openAttribute (CONTIGUOUS_IDS_ATTR_NAME);
	int			contiguous			= 0;
	contiguousAttribut.read (PredType::NATIVE_INT, &contiguous);
	DataSet			idsDataSet		= edgeGroup.openDataSet (DATASET_IDS_NAME);
	if (1 == contiguous)
	{
		ids	= new id_type [1];
		HDFHelper::readUIntDataSet1D (idsDataSet, ids, 1);
		size_type	id	= ids [0] + premier;
		size_t		pos	= 0, nodePos = 0;
		for (size_t i = 0; i < count; i++)
		{
			tableau [pos++]	= id++;
			tableau [pos++]	= nodeIds [2 * i] + 1;
			tableau [pos++]	= nodeIds [2 * i + 1] + 1;
		}	// for (size_t i = 0; i < count; i++)
	}
	else
	{
		ids	= new id_type [count];
		HDFHelper::readUIntDataSet1D (idsDataSet, ids, count, premier);
		size_t		pos	= 0, nodePos = 0;
		for (size_t i = 0; i < count; i++)
		{
			tableau [pos++]	= ids [i];
			tableau [pos++]	= nodeIds [2 * i] + 1;
			tableau [pos++]	= nodeIds [2 * i + 1] + 1;
		}	// for (size_t i = 0; i < count; i++)
	}	// else if (1 == contiguous)

	COMPLETE_TRY_CATCH_BLOCK

	delete [] nodeIds;		nodeIds		= 0;
	delete [] ids;			ids			= 0;

	if (true == throwExc)
		throw read_erreur (errorMsg.str ( ));

	return count;
}	// MaliPPReader::lire_bras


size_type MaliPPReader::nb_polygones_noeuds ( )
{
	if (0 == m_composition_polygones.nb_elements)
		return 0;

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	BEGIN_TRY_CATCH_BLOCK("Lecture des polygones impossible : ")

	// Ouverture du groupe contenant les polygones du maillage :
	Group		cellGroup	= m_maillage_hdf.openGroup (CELL_2D_GROUP_NAME);

	// Ouverture du groupe contenant les ids des noeuds :
	DataSet	nodeIdsDataSet	= cellGroup.openDataSet (DATASET_IDS_LIST_NAME);
	DataSpace	space	= nodeIdsDataSet.getSpace ( );

	return	space.getSimpleExtentNpoints ( );

	COMPLETE_TRY_CATCH_BLOCK

	throw read_erreur (errorMsg.str ( ));
}	// MaliPPReader::nb_polygones_noeuds


size_type* MaliPPReader::allouer_tampon_polygones (size_type nb_elements)
{
	const Composition&	comp	= composition_polygones ( );

	size_type	nb_noeuds	= 6;
	if (true == comp.renseigne)
	{
		for (size_type i = 0; i < MAX_NOEUDS; i++)
			if (0 != comp.detail [i])
				nb_noeuds	= i;
		nb_noeuds	= 3 > nb_noeuds ? 3 : nb_noeuds;
	}	// if (true == comp.renseigne)

	return new size_type [(nb_noeuds + 2) * nb_elements];
}	// MaliPPReader::allouer_tampon_polygones


size_type MaliPPReader::lire_polygones (
				size_type premier, size_type nombre, size_type*& tableau)
{
	const Composition&	comp	= composition_polygones ( );
	const size_type		count	= premier + nombre < comp.nb_elements ?
								  nombre : comp.nb_elements - premier;
	if (0 == count)
		return 0;

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	// Les types de polygones :
	object_type*	cellTypes	= new object_type [count];
	id_type*		nodeIds		= 0;
	id_type*		ids			= 0;
	BEGIN_TRY_CATCH_BLOCK("Lecture des polygones impossible : ")

	// Ouverture du groupe contenant les polygones du maillage :
	Group		cellGroup	= m_maillage_hdf.openGroup (CELL_2D_GROUP_NAME);

	// Pour la lecture des types de polygones :
	DataSet	cellTypesDataSet	= cellGroup.openDataSet(DATASET_TYPES_NAME);

	// Lecture des ids des noeuds : point faible du format car il faut tout
	// lire et analyser avant d'atteindre la maille "premier".
	// Combien de noeuds precedent la lecture ?
	size_type	n = 0, ielm	= 0;
	size_type	nodeIdsPos	= 
					position_ids_noeuds_polygone (premier, cellTypes, count);

	// Lecture des types de polygones :
	HDFHelper::readIntDataSet1D (cellTypesDataSet, cellTypes, count, premier);
	size_t	nodeNumber	= 0;
	for (ielm = 0, nodeNumber = 0; ielm < (size_type)count; ielm++)
		nodeNumber	+= cellTypes [ielm];

	DataSet	nodeIdsDataSet	= cellGroup.openDataSet (DATASET_IDS_LIST_NAME);
	nodeIds	= new id_type [nodeNumber];
	HDFHelper::readUIntDataSet1D (nodeIdsDataSet, nodeIds, nodeNumber,
	                              nodeIdsPos);
	m_pos_ids_noeuds_polygone		+= nodeNumber;
	m_indice_ids_noeuds_polygone	= premier + count;

	// Lecture des identifiants : eviter de tester sans arret si 1 == contiguous
	Attribute	contiguousAttribut	= 
					cellGroup.openAttribute (CONTIGUOUS_IDS_ATTR_NAME);
	int			contiguous			= 0;
	contiguousAttribut.read (PredType::NATIVE_INT, &contiguous);
	DataSet			idsDataSet		= cellGroup.openDataSet (DATASET_IDS_NAME);
	if (1 == contiguous)
	{
		ids	= new id_type [1];
		HDFHelper::readUIntDataSet1D (idsDataSet, ids, 1);
		size_type	id	= ids [0] + premier;
		size_t		pos	= 0, nodePos = 0;
		for (size_t i = 0; i < count; i++)
		{
			tableau [pos++]	= id++;
			const size_t	nbNodes	= tableau [pos++]	= cellTypes [i];
			for (size_t n = 0; n < nbNodes; n++)
				tableau [pos++]	= nodeIds [nodePos++] + 1;
		}	// for (size_t i = 0; i < count; i++)
	}
	else
	{
		ids	= new id_type [count];
		HDFHelper::readUIntDataSet1D (idsDataSet, ids, count, premier);
		size_t		pos	= 0, nodePos = 0;
		for (size_t i = 0; i < count; i++)
		{
			tableau [pos++]	= ids [i];
			tableau [pos++]	= cellTypes [i];
			const size_t	nbNodes	= tableau [pos - 1];
			for (size_t n = 0; n < nbNodes; n++)
				tableau [pos++]	= nodeIds [nodePos++] + 1;
		}	// for (size_t i = 0; i < count; i++)
	}	// else if (1 == contiguous)

	COMPLETE_TRY_CATCH_BLOCK

	delete [] cellTypes;	cellTypes	= 0;
	delete [] nodeIds;		nodeIds		= 0;
	delete [] ids;			ids			= 0;

	if (true == throwExc)
		throw read_erreur (errorMsg.str ( ));

	return count;
}	// MaliPPReader::lire_polygones


size_type MaliPPReader::nb_polyedres_noeuds ( )
{
	if (0 == m_composition_polyedres.nb_elements)
		return 0;

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	BEGIN_TRY_CATCH_BLOCK("Lecture des polyedres impossible : ")

	// Ouverture du groupe contenant les polyedres du maillage :
	Group		cellGroup	= m_maillage_hdf.openGroup (CELL_3D_GROUP_NAME);

	// Ouverture du groupe contenant les ids des noeuds :
	DataSet	nodeIdsDataSet	= cellGroup.openDataSet (DATASET_IDS_LIST_NAME);
	DataSpace	space	= nodeIdsDataSet.getSpace ( );

	return	space.getSimpleExtentNpoints ( );

	COMPLETE_TRY_CATCH_BLOCK

	throw read_erreur (errorMsg.str ( ));
}	// MaliPPReader::nb_polyedres_noeuds


size_type* MaliPPReader::allouer_tampon_polyedres (size_type nb_elements)
{
	const Composition&	comp	= composition_polyedres ( );

	size_type	nb_noeuds	= 12;
	if (true == comp.renseigne)
	{
		for (size_type i = 0; i < MAX_NOEUDS; i++)
			if (0 != comp.detail [i])
				nb_noeuds	= i;
		nb_noeuds	= 4 > nb_noeuds ? 4 : nb_noeuds;
	}	// if (true == comp.renseigne)

	return new size_type [(nb_noeuds + 2) * nb_elements];
}	// MaliPPReader::allouer_tampon_polyedres


size_type MaliPPReader::lire_polyedres (
				size_type premier, size_type nombre, size_type*& tableau)
{
	const Composition&	comp	= composition_polyedres ( );
	const size_type		count	= premier + nombre < comp.nb_elements ?
								  nombre : comp.nb_elements - premier;
	if (0 == count)
		return 0;

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	// Les types de polyedres :
	object_type*	cellTypes	= new object_type [count];
	id_type*		nodeIds		= 0;
	id_type*		ids			= 0;
	BEGIN_TRY_CATCH_BLOCK("Lecture des polyedres impossible : ")

	// Ouverture du groupe contenant les polyedres du maillage :
	Group		cellGroup	= m_maillage_hdf.openGroup (CELL_3D_GROUP_NAME);

	// Pour la lecture des types de polyedres :
	DataSet	cellTypesDataSet	= cellGroup.openDataSet(DATASET_TYPES_NAME);

	// Lecture des ids des noeuds : point faible du format car il faut tout
	// lire et analyser avant d'atteindre la maille "premier".
	// Combien de noeuds precedent la lecture ?
	size_type	n = 0, ielm	= 0;
	size_type	nodeIdsPos	= 
					position_ids_noeuds_polyedre (premier, cellTypes, count);

	// Lecture des types de polyedres :
	HDFHelper::readIntDataSet1D (cellTypesDataSet, cellTypes, count, premier);
	size_t	nodeNumber	= 0;
	for (ielm = 0, nodeNumber = 0; ielm < (size_type)count; ielm++)
		nodeNumber	+= Polyedre::PolyedreNbNode [cellTypes [ielm]];

	DataSet	nodeIdsDataSet	= cellGroup.openDataSet (DATASET_IDS_LIST_NAME);
	nodeIds	= new id_type [nodeNumber];
	HDFHelper::readUIntDataSet1D (nodeIdsDataSet, nodeIds, nodeNumber,
	                              nodeIdsPos);
	m_pos_ids_noeuds_polyedre		+= nodeNumber;
	m_indice_ids_noeuds_polyedre	= premier + count;

	// Lecture des identifiants : eviter de tester sans arret si 1 == contiguous
	Attribute	contiguousAttribut	= 
					cellGroup.openAttribute (CONTIGUOUS_IDS_ATTR_NAME);
	int			contiguous			= 0;
	contiguousAttribut.read (PredType::NATIVE_INT, &contiguous);
	DataSet			idsDataSet		= cellGroup.openDataSet (DATASET_IDS_NAME);
	if (1 == contiguous)
	{
		ids	= new id_type [1];
		HDFHelper::readUIntDataSet1D (idsDataSet, ids, 1);
		size_type	id	= ids [0] + premier;
		size_t		pos	= 0, nodePos = 0;
		for (size_t i = 0; i < count; i++)
		{
			tableau [pos++]	= id++;
			const size_t	nbNodes	= tableau [pos++]	= 
								Polyedre::PolyedreNbNode [cellTypes [i]];
			for (size_t n = 0; n < nbNodes; n++)
				tableau [pos++]	= nodeIds [nodePos++] + 1;
		}	// for (size_t i = 0; i < count; i++)
	}
	else
	{
		ids	= new id_type [count];
		HDFHelper::readUIntDataSet1D (idsDataSet, ids, count, premier);
		size_t		pos	= 0, nodePos = 0;
		for (size_t i = 0; i < count; i++)
		{
			tableau [pos++]	= ids [i];
			tableau [pos++]	= Polyedre::PolyedreNbNode [cellTypes [i]];
			const size_t	nbNodes	= tableau [pos - 1];
			for (size_t n = 0; n < nbNodes; n++)
				tableau [pos++]	= nodeIds [nodePos++] + 1;
		}	// for (size_t i = 0; i < count; i++)
	}	// else if (1 == contiguous)

	COMPLETE_TRY_CATCH_BLOCK

	delete [] cellTypes;	cellTypes	= 0;
	delete [] nodeIds;		nodeIds		= 0;
	delete [] ids;			ids			= 0;

	if (true == throwExc)
		throw read_erreur (errorMsg.str ( ));

	return count;
}	// MaliPPReader::lire_polyedres


void MaliPPReader::liste_nuages (vector<string>& liste)
{
	liste.clear ( );

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	BEGIN_TRY_CATCH_BLOCK("Lecture des nuages impossible : ")

	Group	nodeSetGroup	= m_maillage_hdf.openGroup (NODESET_GROUP_NAME);
	// On lit le nombre de nuages qui peut etre inferieur au nombre de 
	// nuages stockes :
	id_type		nodeSetNumber	= 0;
	Attribute	sizeAttribute	= 
						nodeSetGroup.openAttribute (SIZE_ATTR_NAME);
	sizeAttribute.read (PredType::NATIVE_UINT32, &nodeSetNumber);
	if (0 == nodeSetNumber)
		return;

	// On boucle sur les nuages du maillage :
	for (size_type grp = 0; grp < nodeSetNumber; grp++)
	{
		// Ouverture de son groupe :
		string	nodeSetName		= SUBGROUP_PREFIX + to_str (grp);
		Group	nodeSetNGroup	= nodeSetGroup.openGroup (nodeSetName);

		// Lecture du nom :
		string	name			= 
				HDFHelper::readStringAttribute (nodeSetNGroup, NAME_ATTR_NAME);
		liste.push_back (name);
	}	// for (size_type grp = 0; grp < nodeSetNumber; grp++)

	COMPLETE_TRY_CATCH_BLOCK

	if (true == throwExc)
		throw read_erreur (errorMsg.str ( ));
}	// MaliPPReader::liste_nuages


void MaliPPReader::liste_lignes (vector<string>& liste)
{
	liste.clear ( );

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	BEGIN_TRY_CATCH_BLOCK("Lecture des lignes impossible : ")

	Group	lines	= m_maillage_hdf.openGroup (LINE_GROUP_NAME);
	// On lit le nombre de lignes qui peut etre inferieur au nombre de 
	// lignes stockes :
	id_type		lineNumber		= 0;
	Attribute	sizeAttribute	= lines.openAttribute (SIZE_ATTR_NAME);
	sizeAttribute.read (PredType::NATIVE_UINT32, &lineNumber);
	if (0 == lineNumber)
		return;

	// On boucle sur les lignes du maillage :
	for (size_type grp = 0; grp < lineNumber; grp++)
	{
		// Ouverture de son groupe :
		string	lineName	= SUBGROUP_PREFIX + to_str (grp);
		Group	lineNGroup	= lines.openGroup (lineName);

		// Lecture du nom :
		string	name			= 
				HDFHelper::readStringAttribute (lineNGroup, NAME_ATTR_NAME);
		liste.push_back (name);
	}	// for (size_type grp = 0; grp < lineNumber; grp++)
	COMPLETE_TRY_CATCH_BLOCK

	if (true == throwExc)
		throw read_erreur (errorMsg.str ( ));
}	// MaliPPReader::liste_lignes


void MaliPPReader::liste_surfaces (vector<string>& liste)
{
	liste.clear ( );

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	BEGIN_TRY_CATCH_BLOCK("Lecture des surfaces impossible : ")

	Group	surfaceGroup	= m_maillage_hdf.openGroup (SURFACE_GROUP_NAME);
	// On lit le nombre de surfaces qui peut etre inferieur au nombre de 
	// surfaces stockes :
	id_type		surfaceNumber	= 0;
	Attribute	sizeAttribute	= 
							surfaceGroup.openAttribute (SIZE_ATTR_NAME);
	sizeAttribute.read (PredType::NATIVE_UINT32, &surfaceNumber);
	if (0 == surfaceNumber)
		return;

	// On boucle sur les surfaces du maillage :
	for (size_type grp = 0; grp < surfaceNumber; grp++)
	{
		// Ouverture de son groupe :
		string	surfaceName		= SUBGROUP_PREFIX + to_str (grp);
		Group	surfaceNGroup	= surfaceGroup.openGroup (surfaceName);

		// Lecture du nom :
		string	name			= 
				HDFHelper::readStringAttribute (surfaceNGroup, NAME_ATTR_NAME);
		liste.push_back (name);
	}	// for (size_type grp = 0; grp < surfaceNumber; grp++)

	COMPLETE_TRY_CATCH_BLOCK

	if (true == throwExc)
		throw read_erreur (errorMsg.str ( ));
}	// MaliPPReader::liste_surfaces


void MaliPPReader::liste_volumes (vector<string>& liste)
{
	liste.clear ( );

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	BEGIN_TRY_CATCH_BLOCK("Lecture des volumes impossible : ")

	Group	volumeGroup	= m_maillage_hdf.openGroup (VOLUME_GROUP_NAME);
	// On lit le nombre de volumes qui peut etre inferieur au nombre de 
	// volumes stockes :
	id_type		volumeNumber	= 0;
	Attribute	sizeAttribute	= 
							volumeGroup.openAttribute (SIZE_ATTR_NAME);
	sizeAttribute.read (PredType::NATIVE_UINT32, &volumeNumber);
	if (0 == volumeNumber)
		return;

	// On boucle sur les volumes du maillage :
	for (size_type grp = 0; grp < volumeNumber; grp++)
	{
		// Ouverture de son groupe :
		string	volumeName		= SUBGROUP_PREFIX + to_str (grp);
		Group	volumeNGroup	= volumeGroup.openGroup (volumeName);

		// Lecture du nom :
		string	name			= 
				HDFHelper::readStringAttribute (volumeNGroup, NAME_ATTR_NAME);
		liste.push_back (name);
	}	// for (size_type grp = 0; grp < volumeNumber; grp++)

	COMPLETE_TRY_CATCH_BLOCK

	if (true == throwExc)
		throw read_erreur (errorMsg.str ( ));
}	// MaliPPReader::liste_volumes


void MaliPPReader::lire_liste_attributs (
					ENTITE entite, vector<HDFAttributeDescriptor>& liste)
{
	Group	group	= groupe_attribut (entite);
	lire_liste_attributs (group, liste);
}	// MaliPPReader::lire_liste_attributs


void MaliPPReader::lire_attr_entier (
					ENTITE entite, const HDFAttributeDescriptor& desc, 
					size_type premier, size_type nombre, int_4*& tableau)
{
	Group	group	= groupe_attribut (entite);
	lire_attr_entier (group, desc, premier, nombre, tableau);
}	// MaliPPReader::lire_attr_entier


void MaliPPReader::lire_attr_reel (
					ENTITE entite, const HDFAttributeDescriptor& desc, 
					size_type premier, size_type nombre, double*& tableau)
{
	Group	group	= groupe_attribut (entite);
	lire_attr_reel (group, desc, premier, nombre, tableau);
}	// MaliPPReader::lire_attr_reel


void MaliPPReader::lire_attr_vect_2d (
					ENTITE entite, const HDFAttributeDescriptor& desc, 
					size_type premier, size_type nombre, double*& tableau)
{
	Group	group	= groupe_attribut (entite);
	lire_attr_vect_2d (group, desc, premier, nombre, tableau);
}	// MaliPPReader::lire_attr_vect_2d


void MaliPPReader::lire_attr_vect_3d (
					ENTITE entite, const HDFAttributeDescriptor& desc, 
					size_type premier, size_type nombre, double*& tableau)
{
	Group	group	= groupe_attribut (entite);
	lire_attr_vect_3d (group, desc, premier, nombre, tableau);
}	// MaliPPReader::lire_attr_vect_3d


size_type MaliPPReader::longueur_chaines (
						ENTITE entite, const HDFAttributeDescriptor& desc)
{
	Group	group	= groupe_attribut (entite);
	return longueur_chaines (group, desc);
}	// MaliPPReader::longueur_chaines


void MaliPPReader::lire_attr_chaine (
					ENTITE entite, const HDFAttributeDescriptor& desc, 
					size_type premier, size_type nombre, 
					vector<string>& chaines)
{
	chaines.clear ( );
	Group	group	= groupe_attribut (entite);
	lire_attr_chaine (group, desc, premier, nombre, chaines);
}	// MaliPPReader::lire_attr_chaine


MaliPPReader::NuageReader MaliPPReader::nuage (const string& nom)
{
	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	MutableString	errorMsg;
	errorMsg << "Accès impossible au nuage " << nom << " : ";

	try
	{
		// Ouverture du groupe contenant les nuages :
		Group	nodeSetGroup	= m_maillage_hdf.openGroup (NODESET_GROUP_NAME);

		// Recherche du nuage :
		id_type		cloudNumber	= 0;
		Attribute	sizeAttribute	= 
							nodeSetGroup.openAttribute(SIZE_ATTR_NAME);
		sizeAttribute.read (PredType::NATIVE_UINT32, &cloudNumber);

		for (size_type grp = 0; grp < (size_type)cloudNumber; grp++)
		{
			// Ouverture de son groupe :
			string	cloudName	= SUBGROUP_PREFIX + to_str (grp);
			Group	group		= nodeSetGroup.openGroup (cloudName);

			// Lecture du nom du nuage :
			string	name	= 
					HDFHelper::readStringAttribute(group, NAME_ATTR_NAME);

			if (nom == name)
				return MaliPPReader::NuageReader (this, nom, group);
		}	// for (size_type grp = 0; grp < (size_type)cloudNumber; grp++)

		errorMsg << "ce nuage n'appartient pas au maillage.";
	}
	catch (const Exception& hdfExc)
	{
		errorMsg << "\n" << hdfExc.getDetailMsg ( );
	}
	catch (const exception& exc)
	{
		errorMsg << exc.what ( );
	}
	catch (const erreur& limaExc)
	{
		errorMsg << limaExc.what ( );
	}
	catch (...)
	{
		errorMsg << "erreur non documentee.";
	}

	throw read_erreur (errorMsg.str ( ));
}	// MaliPPReader::nuage


MaliPPReader::LigneReader MaliPPReader::ligne (const string& nom)
{
	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	MutableString		errorMsg;
	errorMsg << "Accès impossible à la ligne " << nom << " : ";

	try
	{
		// Ouverture du groupe contenant les lignes :
		Group	lineGroup	= m_maillage_hdf.openGroup (LINE_GROUP_NAME);

		// Recherche de la ligne :
		id_type		lineNumber	= 0;
		Attribute	sizeAttribute	= lineGroup.openAttribute(SIZE_ATTR_NAME);
		sizeAttribute.read (PredType::NATIVE_UINT32, &lineNumber);

		for (size_type grp = 0; grp < (size_type)lineNumber; grp++)
		{
			// Ouverture de son groupe :
			string	lineName	= SUBGROUP_PREFIX + to_str (grp);
			Group	group		= lineGroup.openGroup (lineName);

			// Lecture du nom du ligne :
			string	name	= 
						HDFHelper::readStringAttribute (group, NAME_ATTR_NAME);

			if (nom == name)
				return MaliPPReader::LigneReader (this, nom, group);
		}	// for (size_type grp = 0; grp < (size_type)lineNumber; grp++)

		errorMsg << "cette ligne n'appartient pas au maillage.";
	}
	catch (const Exception& hdfExc)
	{
		errorMsg << "\n" << hdfExc.getDetailMsg ( );
	}
	catch (const exception& exc)
	{
		errorMsg << exc.what ( );
	}
	catch (const erreur& limaExc)
	{
		errorMsg << limaExc.what ( );
	}
	catch (...)
	{
		errorMsg << "erreur non documentee.";
	}

	throw read_erreur (errorMsg.str ( ));
}	// MaliPPReader::ligne


MaliPPReader::SurfaceReader MaliPPReader::surface (const string& nom)
{
	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	MutableString		errorMsg;
	errorMsg << "Accès impossible à la surface " << nom << " : ";

	try
	{
		// Ouverture du groupe contenant les surfaces :
		Group	surfaceGroup	= m_maillage_hdf.openGroup (SURFACE_GROUP_NAME);

		// Recherche de la surface :
		id_type		surfaceNumber	= 0;
		Attribute	sizeAttribute	= surfaceGroup.openAttribute(SIZE_ATTR_NAME);
		sizeAttribute.read (PredType::NATIVE_UINT32, &surfaceNumber);

		for (size_type grp = 0; grp < (size_type)surfaceNumber; grp++)
		{
			// Ouverture de son groupe :
			string	surfaceName	= SUBGROUP_PREFIX + to_str (grp);
			Group	group		= surfaceGroup.openGroup (surfaceName);

			// Lecture du nom du surface :
			string	name	= 
						HDFHelper::readStringAttribute (group, NAME_ATTR_NAME);

			if (nom == name)
				return MaliPPReader::SurfaceReader (this, nom, group);
		}	// for (size_type grp = 0; grp < (size_type)surfaceNumber; grp++)

		errorMsg << "cette surface n'appartient pas au maillage.";
	}
	catch (const Exception& hdfExc)
	{
		errorMsg << "\n" << hdfExc.getDetailMsg ( );
	}
	catch (const exception& exc)
	{
		errorMsg << exc.what ( );
	}
	catch (const erreur& limaExc)
	{
		errorMsg << limaExc.what ( );
	}
	catch (...)
	{
		errorMsg << "erreur non documentee.";
	}

	throw read_erreur (errorMsg.str ( ));
}	// MaliPPReader::surface


MaliPPReader::VolumeReader MaliPPReader::volume (const string& nom)
{
	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	MutableString		errorMsg;
	errorMsg << "Accès impossible au volume " << nom << " : ";

	try
	{
		// Ouverture du groupe contenant les volumes :
		Group	volumeGroup	= m_maillage_hdf.openGroup (VOLUME_GROUP_NAME);

		// Recherche du volume :
		id_type		volumeNumber	= 0;
		Attribute	sizeAttribute	= volumeGroup.openAttribute(SIZE_ATTR_NAME);
		sizeAttribute.read (PredType::NATIVE_UINT32, &volumeNumber);

		for (size_type grp = 0; grp < (size_type)volumeNumber; grp++)
		{
			// Ouverture de son groupe :
			string	volumeName	= SUBGROUP_PREFIX + to_str (grp);
			Group	group		= volumeGroup.openGroup (volumeName);

			// Lecture du nom du volume :
			string	name	= 
						HDFHelper::readStringAttribute (group, NAME_ATTR_NAME);

			if (nom == name)
				return MaliPPReader::VolumeReader (this, nom, group);
		}	// for (size_type grp = 0; grp < (size_type)volumeNumber; grp++)

		errorMsg << "ce volume n'appartient pas au maillage.";
	}
	catch (const Exception& hdfExc)
	{
		errorMsg << "\n" << hdfExc.getDetailMsg ( );
	}
	catch (const exception& exc)
	{
		errorMsg << exc.what ( );
	}
	catch (const erreur& limaExc)
	{
		errorMsg << limaExc.what ( );
	}
	catch (...)
	{
		errorMsg << "erreur non documentee.";
	}

	throw read_erreur (errorMsg.str ( ));
}	// MaliPPReader::volume


size_type MaliPPReader::position_ids_noeuds_polygone (
					size_type indice, object_type* tampon, size_type taille)
{
	if (indice == m_indice_ids_noeuds_polygone)
		return m_pos_ids_noeuds_polygone;

	// Lecture des ids des noeuds : point faible du format car il faut tout
	// lire et analyser avant d'atteindre la maille "indice".
	// Combien de noeuds precedent la lecture ?
	Group	cellGroup			= m_maillage_hdf.openGroup (CELL_2D_GROUP_NAME);
	DataSet	cellTypesDataSet	= cellGroup.openDataSet (DATASET_TYPES_NAME);
	size_type	n = 0, ielm	= 0;
	size_type	nodeIdsPos	= m_pos_ids_noeuds_polygone - 1;
	if (m_indice_ids_noeuds_polygone > indice)
	{
		nodeIdsPos	= -1;
		m_pos_ids_noeuds_polygone		= 0;
		m_indice_ids_noeuds_polygone	= 0;
	}
	for (n = 0; m_indice_ids_noeuds_polygone + (n + 1) * taille <= indice;  n++)
	{
		HDFHelper::readIntDataSet1D (cellTypesDataSet, tampon, taille,
		                             m_indice_ids_noeuds_polygone + n * taille);
		for (ielm = 0; ielm < (size_type)taille; ielm++)
			nodeIdsPos	+= tampon [ielm];
	}
	const size_type	last	= m_indice_ids_noeuds_polygone + n * taille;
	if ((last != indice - 1) && ((size_type)-1 != last))
	{
		HDFHelper::readIntDataSet1D (
			cellTypesDataSet, tampon, indice - last, last + 1);
		for (ielm = 0; ielm < (size_type)(indice - last); ielm++)
			nodeIdsPos	+= tampon [ielm];
	}
	m_indice_ids_noeuds_polygone	= indice;
	m_pos_ids_noeuds_polygone		= nodeIdsPos + 1;

	return nodeIdsPos + 1;
}	// MaliPPReader::position_ids_noeuds_polygone


size_type MaliPPReader::position_ids_noeuds_polyedre (
					size_type indice, object_type* tampon, size_type taille)
{
	if (indice == m_indice_ids_noeuds_polyedre)
		return m_pos_ids_noeuds_polyedre;

	// Lecture des ids des noeuds : point faible du format car il faut tout
	// lire et analyser avant d'atteindre la maille "indice".
	// Combien de noeuds precedent la lecture ?
	Group	cellGroup			= m_maillage_hdf.openGroup (CELL_3D_GROUP_NAME);
	DataSet	cellTypesDataSet	= cellGroup.openDataSet (DATASET_TYPES_NAME);
	size_type	n = 0, ielm	= 0;
	size_type	nodeIdsPos	= m_pos_ids_noeuds_polyedre - 1;
	if (m_indice_ids_noeuds_polyedre > indice)
	{
		nodeIdsPos	= -1;
		m_pos_ids_noeuds_polyedre		= 0;
		m_indice_ids_noeuds_polyedre	= 0;
	}
	for (n = 0; m_indice_ids_noeuds_polyedre + (n + 1) * taille <= indice;  n++)
	{
		HDFHelper::readIntDataSet1D (cellTypesDataSet, tampon, taille,
		                             m_indice_ids_noeuds_polyedre + n * taille);
		for (ielm = 0; ielm < (size_type)taille; ielm++)
			nodeIdsPos	+= Polyedre::PolyedreNbNode [tampon [ielm]];
	}
	const size_type	last	= m_indice_ids_noeuds_polyedre + n * taille;
	if ((last != indice - 1) && ((size_type)-1 != last))
	{
		HDFHelper::readIntDataSet1D (
			cellTypesDataSet, tampon, indice - last, last + 1);
		for (ielm = 0; ielm < (size_type)(indice - last); ielm++)
			nodeIdsPos	+= Polyedre::PolyedreNbNode [tampon [ielm]];
	}
	m_indice_ids_noeuds_polyedre	= indice;
	m_pos_ids_noeuds_polyedre		= nodeIdsPos + 1;

	return nodeIdsPos + 1;
}	// MaliPPReader::position_ids_noeuds_polyedre


Group MaliPPReader::groupe_attribut (ENTITE entite)
{
	switch (entite)
	{
		case MAILLAGE	:
			return m_maillage_hdf.openGroup (DATA_MESH_GROUP_NAME);
		case NOEUDS	:
			return m_maillage_hdf.openGroup (DATA_NODE_GROUP_NAME);
		case BRAS	:
			return m_maillage_hdf.openGroup (DATA_EDGE_GROUP_NAME);
		case POLYGONES	:
			return m_maillage_hdf.openGroup (DATA_CELL_2D_GROUP_NAME);
		case POLYEDRES	:
			return m_maillage_hdf.openGroup (DATA_CELL_3D_GROUP_NAME);
		case NUAGES		:
			return m_maillage_hdf.openGroup (DATA_NODESET_GROUP_NAME);
		case LIGNES		:
			return m_maillage_hdf.openGroup (DATA_LINE_GROUP_NAME);
		case SURFACES	:
			return m_maillage_hdf.openGroup (DATA_SURFACE_GROUP_NAME);
		case VOLUMES	:
			return m_maillage_hdf.openGroup (DATA_VOLUME_GROUP_NAME);
	}

	throw read_erreur ("Type d'entité de rattachement inconnu.");
}	// MaliPPReader::groupe_attribut


void MaliPPReader::lire_entete_maillage ( )
{
#ifndef DISABLE_MLI_WARNING
	// Avertissement écrit en bleu puis restauration couleur par défaut.
	cerr << "\033[34m" << "ATTENTION : le format mli est obsolète. Nous recommandons d'utiliser à la place le format mli2 qui reprend les mêmes caractéristiques." << "\033[39m" << endl;
#endif	// DISABLE_MLI_WARNING
	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	BEGIN_TRY_CATCH_BLOCK("Impossible de lire le maillage ")
	errorMsg << m_numero_maillage << " du fichier " << m_nom_fichier << " : ";

	// Ouverture du groupe contenant le maillage et lecture de son
	// entete :
	const string	path	= MESH_GROUP_PREFIX + to_str(m_numero_maillage);
	m_maillage_hdf	= m_fichier_hdf->openGroup (path);

	// Lecture de l'entete du maillage :
	Group	header	= m_maillage_hdf.openGroup (MESH_HEADER_GROUP_NAME);
	string	version_lima	= HDFHelper::readStringAttribute (
											header, LIMA_VERSION_FIELD_NAME);
	if (0 == version_lima.compare (0, 5, "Lima."))
		m_lima_version_maillage	= Version (version_lima.substr (5));
	else
		m_lima_version_maillage	= Version (version_lima);
	m_mli_version_maillage	= Version (HDFHelper::readStringAttribute (
										header, FILE_VERSION_FIELD_NAME));

	// Titre, date :
	m_titre		= HDFHelper::readStringAttribute(header, MESH_TITLE_FIELD_NAME);
	m_date		= HDFHelper::readStringAttribute (header, MESH_DATE_FIELD_NAME);

	// Dimension :
	int		dimension	= 0;
	Attribute	meshDimAttribute	=
						header.openAttribute (MESH_DIMENSION_FIELD_NAME);
	meshDimAttribute.read (PredType::NATIVE_INT, &dimension);
	m_dimension	= (dim_t)dimension;

	// Type de geometrie :
	int		geometry	= 0;
	Attribute	meshGeomAttribute	=
						header.openAttribute (MESH_GEOMETRY_FIELD_NAME);
	meshGeomAttribute.read (PredType::NATIVE_INT, &geometry);
	m_geometrie	= (geometrie_t)geometry;

	// Systeme de coordonnees :
	int		coordSys	= 0;
	Attribute	meshCoordSysAttribute	=
						header.openAttribute (MESH_COORD_SYSTEM_FIELD_NAME);
	meshCoordSysAttribute.read (PredType::NATIVE_INT, &coordSys);
	m_systeme_coords	= (coordonnee_t)coordSys;

	// Unite d'angle :
	Attribute	meshAngleUnitAttribute	=
						header.openAttribute (MESH_ANGLE_UNIT_FIELD_NAME);
	meshAngleUnitAttribute.read (PredType::NATIVE_DOUBLE, &m_unite_angle);

	// Unite de longueur :
	Attribute	meshLengthUnitAttribute	=
						header.openAttribute (MESH_LENGTH_UNIT_FIELD_NAME);
	meshLengthUnitAttribute.read (
							PredType::NATIVE_DOUBLE, &m_unite_longueur);

	COMPLETE_TRY_CATCH_BLOCK

	if (true == throwExc)
		throw erreur (errorMsg.str ( ));
}	// MaliPPReader::lire_entete_maillage


void MaliPPReader::lire_composition_maillage ( )
{
	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	BEGIN_TRY_CATCH_BLOCK("Lecture des noeuds impossible : ")

	// Ouverture du groupe contenant les noeuds du maillage :
	Group		nodeGroup	= m_maillage_hdf.openGroup (NODE_GROUP_NAME);
	id_type		nodeNumber	= 0;
	Attribute	sizeAttribute	= nodeGroup.openAttribute (SIZE_ATTR_NAME);
	sizeAttribute.read (PredType::NATIVE_UINT32, &nodeNumber);
	m_composition_noeuds.nb_elements	= nodeNumber;
	lire_composition (nodeGroup, NODE_COMPOSITION_DATASET_NAME, 
	                  m_mli_version_maillage, m_composition_noeuds);

	// Ouverture du groupe contenant les bras du maillage :
	Group		edgeGroup	= m_maillage_hdf.openGroup (EDGE_GROUP_NAME);
	id_type		edgeNumber	= 0;
	sizeAttribute	= edgeGroup.openAttribute (SIZE_ATTR_NAME);
	sizeAttribute.read (PredType::NATIVE_UINT32, &edgeNumber);
	m_composition_bras.nb_elements	= edgeNumber;
	lire_composition (edgeGroup, EDGE_COMPOSITION_DATASET_NAME,
	                  m_mli_version_maillage, m_composition_bras);

	// Ouverture du groupe contenant les polygones du maillage :
	Group		cell2DGroup	= m_maillage_hdf.openGroup (CELL_2D_GROUP_NAME);
	id_type		cell2DNumber= 0;
	sizeAttribute	= cell2DGroup.openAttribute (SIZE_ATTR_NAME);
	sizeAttribute.read (PredType::NATIVE_UINT32, &cell2DNumber);
	m_composition_polygones.nb_elements	= cell2DNumber;
	lire_composition (cell2DGroup, CELL2D_COMPOSITION_DATASET_NAME,
	                  m_mli_version_maillage, m_composition_polygones);

	// Ouverture du groupe contenant les polyedres du maillage :
	Group		cell3DGroup	= m_maillage_hdf.openGroup (CELL_3D_GROUP_NAME);
	id_type		cell3DNumber	= 0;
	sizeAttribute	= cell3DGroup.openAttribute (SIZE_ATTR_NAME);
	sizeAttribute.read (PredType::NATIVE_UINT32, &cell3DNumber);
	m_composition_polyedres.nb_elements	= cell3DNumber;
	lire_composition (cell3DGroup, CELL3D_COMPOSITION_DATASET_NAME,
	                  m_mli_version_maillage, m_composition_polyedres);

	COMPLETE_TRY_CATCH_BLOCK

	if (true == throwExc)
		throw erreur (errorMsg.str ( ));
}	// MaliPPReader::lire_composition_maillage



// ===========================================================================
//                            LA CLASSE MaliPPWriter
// ===========================================================================

#ifdef __INTERNE_MALIPP_WRITER

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


MaliPPWriter::MaliPPWriter (const IN_STD string& nom_fichier, size_type num)
: m_fileName(nom_fichier), m_meshNum(num),
	m_dim(D3),
	m_unilo(1.),
	m_unia(1.),
	m_geom(NORMAL),
	m_tycoo(CARTESIEN),
	m_titre(""),
	m_date(""),
	m_nodesIsContiguous(false),
	m_nodeIndexCurrent(0),
	m_nbNodes(0),
	m_edgesIsContiguous(false),
	m_edgeIndexCurrent(0),
	m_nbEdges(0),
	m_facesIsContiguous(false),
	m_faceIndexCurrent(0),
	m_nbFaces(0),
	m_nbNodesPerFaceIndexCurrent(0),
	m_regionsIsContiguous(false),
	m_regionIndexCurrent(0),
	m_nbRegions(0),
	m_nbNodesPerRegionIndexCurrent(0)
{

}	// MaliPPWriter::MaliPPWriter

void MaliPPWriter::beginWrite ( )
{
	bool			throwExc	= false;
	MutableString	errorMsg;

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
		createMainHeader ();

		// Le fichier est ouvert : ouvrir en ecriture le maillage demande,
		// en le creant si necessaire :
		createMesh ( );

		// Ecriture de l'entete du maillage :
		writeMeshHeader ( );

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

	if(throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}	// MaliPPWriter::beginWrite


void MaliPPWriter::dimension(dim_t dim)
{
	m_dim = dim;
}  // MaliPPWriter::dimension

void MaliPPWriter::unite_longueur(double unilo)
{
	m_unilo = unilo;
}  // MaliPPWriter::unite_longueur

void MaliPPWriter::unite_angle(double unia)
{
	m_unia = unia;
}  // MaliPPWriter::unite_angle

void MaliPPWriter::type_geometrie(geometrie_t geom)
{
	m_geom = geom;
}  // MaliPPWriter::type_geometrie

void MaliPPWriter::type_coordonnee(coordonnee_t tycoo)
{
	m_tycoo = tycoo;
}  // MaliPPWriter::type_coordonnee

void MaliPPWriter::titre(const IN_STD string& titre)
{
	m_titre = titre;
}  // MaliPPWriter::titre

void MaliPPWriter::date(const IN_STD string& date)
{
	m_date = date;
}  // MaliPPWriter::date


void MaliPPWriter::createMainHeader ( )
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::createMainHeader ")
	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

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

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}

}  // MaliPPWriter::createMainHeader


void MaliPPWriter::writeVersionsInfos (H5Object& root)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeVersionsInfos ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

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

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeVersionsInfos


//! Ecriture du header d'un maillage. Création si nécessaire.
void MaliPPWriter::writeMeshHeader ( )
{
#ifndef DISABLE_MLI_WARNING
	// Avertissement écrit en bleu puis restauration couleur par défaut.
	cerr << "\033[34m" << "ATTENTION : le format mli est obsolète. Nous recommandons d'utiliser à la place le format mli2 qui reprend les mêmes caractéristiques." << "\033[39m" << endl;
#endif	// DISABLE_MLI_WARNING

	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeMeshHeader ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	Group	header	=
			HDFHelper::openOrCreateGroup (m_meshGroup, MESH_HEADER_GROUP_NAME);

	// Version du format de fichier et de lima++ :
	writeVersionsInfos (header);

	// Titre du maillage :
	Attribute	meshTitleAttribute	=
		HDFHelper::openOrCreateStringAttribute (header, MESH_TITLE_FIELD_NAME,
		                                        ATTRIBUT_SIZE);
	HDFHelper::writeStringAttribute (meshTitleAttribute, m_titre);

	// Date du maillage :
	Attribute	meshDateAttribute	=
		HDFHelper::openOrCreateStringAttribute (header, MESH_DATE_FIELD_NAME,
		                                        ATTRIBUT_SIZE);
	HDFHelper::writeStringAttribute (meshDateAttribute, m_date);

	// Dimension du maillage :
	Attribute	meshDimAttribute	=
			HDFHelper::openOrCreateAttribute (header, MESH_DIMENSION_FIELD_NAME,
			                                  PredType::NATIVE_INT);
	int	dimension	= m_dim;
	meshDimAttribute.write (PredType::NATIVE_INT, &dimension);

	// Type de geometrie du maillage :
	Attribute	meshGeomAttribute	=
			HDFHelper::openOrCreateAttribute (header, MESH_GEOMETRY_FIELD_NAME,
			                                  PredType::NATIVE_INT);
	int	geometry	= m_geom;
	meshGeomAttribute.write (PredType::NATIVE_INT, &geometry);

	// System de coordonnees du maillage :
	Attribute	meshCoordSysAttribute	=
		HDFHelper::openOrCreateAttribute (header, MESH_COORD_SYSTEM_FIELD_NAME,
		                                  PredType::NATIVE_INT);
	int	coordSys	= m_tycoo;
	meshCoordSysAttribute.write (PredType::NATIVE_INT, &coordSys);

	// Unite d'angle :
	Attribute	meshAngleUnitAttribute	=
			HDFHelper::openOrCreateAttribute (
				header, MESH_ANGLE_UNIT_FIELD_NAME, PredType::NATIVE_DOUBLE);
	double	angleUnit	= m_unia;
	meshAngleUnitAttribute.write (PredType::NATIVE_DOUBLE, &angleUnit);

	// Unite de longueur :
	Attribute	meshLengthUnitAttribute	=
		HDFHelper::openOrCreateAttribute (header, MESH_LENGTH_UNIT_FIELD_NAME,
		                                  PredType::NATIVE_DOUBLE);
	double	lengthUnit	= m_unilo;
	meshLengthUnitAttribute.write (PredType::NATIVE_DOUBLE, &lengthUnit);

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}	// MaliPPWriter::writeMeshHeader


void MaliPPWriter::createMesh ( )
{
	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

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

	writeMeshAttributes ();

	if (true == incrementMeshCount)
		nbMeshInc ( );

}  // MaliPPWriter::createMesh


void MaliPPWriter::close ( )
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::close ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	writeComposition(m_compoNode,NODE_COMPOSITION_DATASET_NAME, m_nodeGroup);
	writeComposition(m_compoEdge,EDGE_COMPOSITION_DATASET_NAME, m_edgeGroup);
	writeComposition(m_compoFace,CELL2D_COMPOSITION_DATASET_NAME, m_faceGroup);
	writeComposition(m_compoRegion,CELL3D_COMPOSITION_DATASET_NAME, m_regionGroup);

	for(id_type igrp=0; igrp<m_nbFaceSet; igrp++) {
		writeComposition (m_faceSetCompo[igrp], CELL2D_COMPOSITION_DATASET_NAME, m_faceSetGroups[igrp]);
	}
	for(id_type igrp=0; igrp<m_nbRegionSet; igrp++) {
		writeComposition (m_regionSetCompo[igrp], CELL3D_COMPOSITION_DATASET_NAME, m_regionSetGroups[igrp]);
	}

	m_meshGroup	= Group ( );
	m_hdfFile.reset (0);

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}	// MaliPPWriter::close


void MaliPPWriter::createMeshDataGroup ( )
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::createMeshDataGroup ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

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

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}	// MaliPPWriter::createMeshDataGroup


void MaliPPWriter::writeMeshAttributes ( )
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeMeshAttributes ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbMeshAttributes = 0;

	m_meshAttributesGroup 	= m_meshGroup.openGroup (DATA_MESH_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_meshAttributesGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbMeshAttributes);

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeMeshAttributes


/*! Incremente le nombre de maillage d'un fichier. L'attribut identifie par
   NB_MESHES_FIELD_NAME doit préalablement exister et m_fileName être
   ouvert. */
void MaliPPWriter::nbMeshInc ( )
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::nbMeshInc ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	Group		header				=
					m_hdfFile->openGroup (FILE_HEADER_GROUP_NAME);
	int			nbMeshes 			= 0;
	Attribute	nbMeshesAttribute	=
					header.openAttribute (NB_MESHES_FIELD_NAME);
	nbMeshesAttribute.read (PredType::NATIVE_INT, &nbMeshes);
	nbMeshes++;
	nbMeshesAttribute.write (PredType::NATIVE_INT, &nbMeshes);

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}	// MaliPPWriter::nbMeshInc


void MaliPPWriter::writeNodesInfo (bool isContiguous, id_type nbNodes, id_type firstID)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeNodesInfo ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbNodes = nbNodes;

	m_nodeGroup 		= m_meshGroup.openGroup (NODE_GROUP_NAME);
	// Le nombre de noeuds :
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_nodeGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);

	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbNodes);

	if(m_nbNodes>0) {
		Attribute	contiguousAttribut	= HDFHelper::openOrCreateAttribute (
				m_nodeGroup, CONTIGUOUS_IDS_ATTR_NAME, PredType::NATIVE_INT);

		m_nodesIsContiguous = isContiguous;
		int contiguous =  (m_nodesIsContiguous ? 1 : 0);

		contiguousAttribut.write (PredType::NATIVE_INT, &contiguous);

		m_nodeIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
				m_nodeGroup, DATASET_IDS_NAME,
				(hsize_t)(m_nodesIsContiguous ? 1 : m_nbNodes),
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		if(m_nodesIsContiguous) {
			HDFHelper::writeUIntDataSet1D (m_nodeIdsDataSet, &firstID, 1, 0);
		}

		switch (m_dim)
		{
		case D3	:
		{
			m_nodeZCoordDataSet	= HDFHelper::openOrCreateDataSet1D (
					m_nodeGroup, DATASET_COORD_Z_NAME, m_nbNodes,
					PredType::NATIVE_DOUBLE, true, NODE_CHUNK_SIZE);
		}
		case D2	:
		{
			m_nodeYCoordDataSet	= HDFHelper::openOrCreateDataSet1D (
					m_nodeGroup, DATASET_COORD_Y_NAME, m_nbNodes,
					PredType::NATIVE_DOUBLE, true, NODE_CHUNK_SIZE);
		}
		case D1 :
		{
			m_nodeXCoordDataSet	= HDFHelper::openOrCreateDataSet1D (
					m_nodeGroup, DATASET_COORD_X_NAME, m_nbNodes,
					PredType::NATIVE_DOUBLE, true, NODE_CHUNK_SIZE);
		}
		break;
		default	:
			throw write_erreur (
					"Ecriture des noeuds impossible : erreur interne.");
		}	// switch (m_dim)

	}

	m_compoNode.renseigne = 1;
	m_compoNode.nb_elements = m_nbNodes;
	m_compoNode.detail[0] = m_nbNodes;

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}	// MaliPPWriter::writeNodesInfo


void MaliPPWriter::writeNodes (id_type nodesChunkSize, double* coordsX, double* coordsY, double* coordsZ, id_type* ids)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeNodes ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	switch (m_dim)
	{
	case D3	:
	{
		HDFHelper::writeDoubleDataSet1D (m_nodeZCoordDataSet, coordsZ, nodesChunkSize,
				m_nodeIndexCurrent);
	}
	case D2	:
	{
		HDFHelper::writeDoubleDataSet1D (m_nodeYCoordDataSet, coordsY, nodesChunkSize,
				m_nodeIndexCurrent);
	}
	case D1 :
	{
		HDFHelper::writeDoubleDataSet1D (m_nodeXCoordDataSet, coordsX, nodesChunkSize,
				m_nodeIndexCurrent);
	}
	break;
	default	:
		throw write_erreur (
				"Ecriture des noeuds impossible : erreur interne.");
	}	// switch (m_dim)

	if(!m_nodesIsContiguous) {
		HDFHelper::writeUIntDataSet1D (m_nodeIdsDataSet, ids, nodesChunkSize, m_nodeIndexCurrent);
	}

	m_nodeIndexCurrent += nodesChunkSize;

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeNodes


void MaliPPWriter::writeNodeSetInfo (id_type nbSet, std::vector<std::string> setNames, std::vector<id_type> setSizes)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeNodeSetInfo ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbNodeSet = nbSet;
	m_nodeSetSizes = setSizes;
	m_nodeSetIndexCurrent.clear();
	m_nodeSetIndexCurrent.resize(m_nbNodeSet,0);

	m_nodeSetGroup 	= m_meshGroup.openGroup (NODESET_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_nodeSetGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbNodeSet);

	for(id_type i=0; i<m_nbNodeSet; i++) {
		m_nodeSetNames[setNames[i]] = i;

		// Creation de son groupe :
		string	basicGroupName		= SUBGROUP_PREFIX + to_str (i);
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_nodeSetGroup, basicGroupName);

		// Ecriture du nom du nuage :
		Attribute	nameAttribute	=
				HDFHelper::openOrCreateStringAttribute (
						cloudGroup, NAME_ATTR_NAME, ATTRIBUT_SIZE);
		HDFHelper::writeStringAttribute (nameAttribute, setNames[i]);

		Composition compoCloud;
		compoCloud.renseigne = 1;
		compoCloud.nb_elements = m_nodeSetSizes[i];
		compoCloud.detail[0] = m_nodeSetSizes[i];
		writeComposition (compoCloud, NODE_COMPOSITION_DATASET_NAME, cloudGroup);

		// Ecriture du nombre de noeuds :
		Attribute	sizeNAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		sizeNAttribute.write (PredType::NATIVE_UINT32, &m_nodeSetSizes[i]);

		// Ecriture des identifiants des noeuds :
		DataSet		nodeSetIdsDataSet	=
				HDFHelper::openOrCreateDataSet1D (
						cloudGroup, DATASET_IDS_LIST_NAME, m_nodeSetSizes[i],
						PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		m_nodeSetIdsDataSet.push_back(nodeSetIdsDataSet);

		m_nodeSetGroups.push_back(cloudGroup);
	}

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeNodeSetInfo


void MaliPPWriter::writeNodeSetData (std::string name, id_type chunkSize, id_type* ids)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeNodeSetData ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	id_type setId = m_nodeSetNames[name];

	// what is written in the mali file is nodesIds -1
	id_type* ids_tmp = new id_type[chunkSize];
	for(id_type i=0; i<chunkSize; i++) {
		ids_tmp[i] = ids[i]-1;
	}

	HDFHelper::writeUIntDataSet1D (m_nodeSetIdsDataSet[setId], ids_tmp, chunkSize,
			m_nodeSetIndexCurrent[setId]);
	delete[] ids_tmp;

	m_nodeSetIndexCurrent[setId] += chunkSize;

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeNodeSetData


void MaliPPWriter::writeNodeAttributes ( )
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeNodeAttributes ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbNodeAttributes = 0;

	m_nodeAttributesGroup 	= m_meshGroup.openGroup (DATA_NODE_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_nodeAttributesGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbNodeAttributes);

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeNodeAttributes


void MaliPPWriter::writeNodeSetsAttributes ( )
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeNodeSetsAttributes ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbNodeSetsAttributes = 0;

	m_nodeSetsAttributesGroup 	= m_meshGroup.openGroup (DATA_NODESET_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_nodeSetsAttributesGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbNodeSetsAttributes);

	for(id_type igrp=0; igrp<m_nbNodeSet; igrp++) {
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_nodeSetGroups[igrp], DATA_NODE_GROUP_NAME);
		Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		id_type size = 0;
		sizeAttribute.write (PredType::NATIVE_UINT32, &size);
	}

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeNodeSetsAttributes


void MaliPPWriter::writeEdgesInfo (bool isContiguous, id_type nbEdges, id_type firstID)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeEdgesInfo ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbEdges = nbEdges;

	m_edgeGroup 		= m_meshGroup.openGroup (EDGE_GROUP_NAME);
	// Le nombre d'arêtes :
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_edgeGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);

	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbEdges);

	if(m_nbEdges>0) {
		Attribute	contiguousAttribut	= HDFHelper::openOrCreateAttribute (
				m_edgeGroup, CONTIGUOUS_IDS_ATTR_NAME, PredType::NATIVE_INT);

		m_edgesIsContiguous = isContiguous;
		int contiguous =  (m_edgesIsContiguous ? 1 : 0);

		contiguousAttribut.write (PredType::NATIVE_INT, &contiguous);

		m_edgeIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
				m_edgeGroup, DATASET_IDS_NAME,
				(hsize_t)(m_edgesIsContiguous ? 1 : m_nbEdges),
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		if(m_edgesIsContiguous) {
			HDFHelper::writeUIntDataSet1D (m_edgeIdsDataSet, &firstID, 1, 0);
		}

		// La liste des noeuds :
		m_edge2nodeIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
				m_edgeGroup, DATASET_IDS_LIST_NAME, 2 * m_nbEdges,
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
	}

	m_compoEdge.renseigne = 1;
	m_compoEdge.nb_elements = m_nbEdges;
	m_compoEdge.detail[2] = m_nbEdges;

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}	// MaliPPWriter::writeEdgesInfo


void MaliPPWriter::writeEdges (id_type edgesChunkSize, id_type* edge2nodeIDs, id_type* ids)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeEdges ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	// what is written in the mali file is nodesIds -1
	id_type* edge2nodeIDs_tmp = new id_type[2*edgesChunkSize];
	for(id_type i=0; i<2*edgesChunkSize; i++) {
		edge2nodeIDs_tmp[i] = edge2nodeIDs[i]-1;
	}
	HDFHelper::writeUIntDataSet1D (m_edge2nodeIdsDataSet, edge2nodeIDs_tmp, 2*edgesChunkSize, 2 * m_edgeIndexCurrent);
	delete[] edge2nodeIDs_tmp;

	if(!m_edgesIsContiguous) {
			HDFHelper::writeUIntDataSet1D (m_edgeIdsDataSet, ids, edgesChunkSize, m_edgeIndexCurrent);
		}

	m_edgeIndexCurrent += edgesChunkSize;

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeEdges


void MaliPPWriter::writeEdgeSetInfo (id_type nbSet, std::vector<std::string> setNames, std::vector<id_type> setSizes)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeEdgeSetInfo ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbEdgeSet = nbSet;
	m_edgeSetSizes = setSizes;
	m_edgeSetIndexCurrent.clear();
	m_edgeSetIndexCurrent.resize(m_nbEdgeSet,0);

	m_edgeSetGroup 	= m_meshGroup.openGroup (LINE_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_edgeSetGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbEdgeSet);

	for(id_type i=0; i<m_nbEdgeSet; i++) {
		m_edgeSetNames[setNames[i]] = i;

		// Creation de son groupe :
		string	basicGroupName		= SUBGROUP_PREFIX + to_str (i);
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_edgeSetGroup, basicGroupName);

		// Ecriture du nom du nuage :
		Attribute	nameAttribute	=
				HDFHelper::openOrCreateStringAttribute (
						cloudGroup, NAME_ATTR_NAME, ATTRIBUT_SIZE);
		HDFHelper::writeStringAttribute (nameAttribute, setNames[i]);

		Composition compoLine;
		compoLine.renseigne = 1;
		compoLine.nb_elements = m_edgeSetSizes[i];
		compoLine.detail[2] = m_edgeSetSizes[i];
		writeComposition (compoLine, EDGE_COMPOSITION_DATASET_NAME, cloudGroup);

		// Ecriture du nombre de noeuds :
		Attribute	sizeNAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		sizeNAttribute.write (PredType::NATIVE_UINT32, &m_edgeSetSizes[i]);

		// Ecriture des identifiants des noeuds :
		DataSet		edgeSetIdsDataSet	=
				HDFHelper::openOrCreateDataSet1D (
						cloudGroup, DATASET_IDS_LIST_NAME, m_edgeSetSizes[i],
						PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		m_edgeSetIdsDataSet.push_back(edgeSetIdsDataSet);

		m_edgeSetGroups.push_back(cloudGroup);
	}

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeEdgeSetInfo


void MaliPPWriter::writeEdgeSetData (std::string name, id_type chunkSize, id_type* ids)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeEdgeSetData ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	id_type setId = m_edgeSetNames[name];

	// what is written in the mali file is nodesIds -1
	id_type* ids_tmp = new id_type[chunkSize];
	for(id_type i=0; i<chunkSize; i++) {
		ids_tmp[i] = ids[i]-1;
	}

	HDFHelper::writeUIntDataSet1D (m_edgeSetIdsDataSet[setId], ids_tmp, chunkSize,
			m_edgeSetIndexCurrent[setId]);
	delete[] ids_tmp;

	m_edgeSetIndexCurrent[setId] += chunkSize;

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeEdgeSetData


void MaliPPWriter::writeEdgeAttributes ( )
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeEdgeAttributes ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbEdgeAttributes = 0;

	m_edgeAttributesGroup 	= m_meshGroup.openGroup (DATA_EDGE_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_edgeAttributesGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbEdgeAttributes);

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeEdgeAttributes


void MaliPPWriter::writeEdgeSetsAttributes ( )
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeEdgeSetsAttributes ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbEdgeSetsAttributes = 0;

	m_edgeSetsAttributesGroup 	= m_meshGroup.openGroup (DATA_LINE_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_edgeSetsAttributesGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbEdgeSetsAttributes);

	for(id_type igrp=0; igrp<m_nbEdgeSet; igrp++) {
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_edgeSetGroups[igrp], DATA_EDGE_GROUP_NAME);
		Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		id_type size = 0;
		sizeAttribute.write (PredType::NATIVE_UINT32, &size);
	}

	for(id_type igrp=0; igrp<m_nbEdgeSet; igrp++) {
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_edgeSetGroups[igrp], DATA_NODE_GROUP_NAME);
		Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		id_type size = 0;
		sizeAttribute.write (PredType::NATIVE_UINT32, &size);
	}

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeEdgeSetsAttributes


void MaliPPWriter::writeFacesInfo (bool isContiguous, id_type nbFaces, id_type firstID)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeFacesInfo ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbFaces = nbFaces;

	m_faceGroup 		= m_meshGroup.openGroup (CELL_2D_GROUP_NAME);
	// Le nombre d'arêtes :
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_faceGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);

	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbFaces);

	if(m_nbFaces>0) {
		Attribute	contiguousAttribut	= HDFHelper::openOrCreateAttribute (
				m_faceGroup, CONTIGUOUS_IDS_ATTR_NAME, PredType::NATIVE_INT);

		m_facesIsContiguous = isContiguous;
		int contiguous =  (m_facesIsContiguous ? 1 : 0);

		contiguousAttribut.write (PredType::NATIVE_INT, &contiguous);

		m_faceIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
				m_faceGroup, DATASET_IDS_NAME,
				(hsize_t)(m_facesIsContiguous ? 1 : m_nbFaces),
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		if(m_facesIsContiguous) {
			HDFHelper::writeUIntDataSet1D (m_faceIdsDataSet, &firstID, 1, 0);
		}

		// La liste des noeuds :
		// on prend 3*nombre de faces parce que l'on ne connait pas a priori
		// la taille totale du tableau face2nodeIDs
		m_face2nodeIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
				m_faceGroup, DATASET_IDS_LIST_NAME, 3 * m_nbFaces,
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);

		m_nbNodesPerFaceDataSet	= HDFHelper::openOrCreateDataSet1D (
						m_faceGroup, DATASET_TYPES_NAME, m_nbFaces,
						PredType::NATIVE_INT32, true, CELL2D_CHUNK_SIZE);

	}

	m_compoFace.renseigne = 1;
	m_compoFace.nb_elements = m_nbFaces;

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}	// MaliPPWriter::writeFacesInfo


void MaliPPWriter::writeFaces (id_type facesChunkSize, id_type* face2nodeIDs, id_type* nbNodesPerFace, id_type* ids)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeFaces ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	HDFHelper::writeUIntDataSet1D (m_nbNodesPerFaceDataSet, nbNodesPerFace, facesChunkSize, m_faceIndexCurrent);

	if(!m_facesIsContiguous) {
		HDFHelper::writeUIntDataSet1D (m_faceIdsDataSet, ids, facesChunkSize, m_faceIndexCurrent);
	}

	size_t nbNodesTot = 0;
	for(id_type iFace=0; iFace<facesChunkSize; iFace++) {
		nbNodesTot += nbNodesPerFace[iFace];
		m_compoFace.detail[nbNodesPerFace[iFace]]++;
	}

	// check that the allocated size is sufficient;
	// 3*m_nbFaces is the initial allocated size
	if(m_nbNodesPerFaceIndexCurrent + nbNodesTot > 3*m_nbFaces) {
		m_face2nodeIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
				m_faceGroup, DATASET_IDS_LIST_NAME, m_nbNodesPerFaceIndexCurrent + nbNodesTot,
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
	}
	// what is written in the mali file is nodesIds -1
	id_type* face2nodeIDs_tmp = new id_type[nbNodesTot];
	for(id_type i=0; i<nbNodesTot; i++) {
		face2nodeIDs_tmp[i] = face2nodeIDs[i]-1;
	}
	HDFHelper::writeUIntDataSet1D (m_face2nodeIdsDataSet, face2nodeIDs_tmp, nbNodesTot, m_nbNodesPerFaceIndexCurrent);
	delete[] face2nodeIDs_tmp;

	m_faceIndexCurrent += facesChunkSize;
	m_nbNodesPerFaceIndexCurrent += nbNodesTot;

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeFaces


void MaliPPWriter::writeFaceSetInfo (id_type nbSet, std::vector<std::string> setNames, std::vector<id_type> setSizes)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeFaceSetInfo ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbFaceSet = nbSet;
	m_faceSetSizes = setSizes;
	m_faceSetIndexCurrent.clear();
	m_faceSetIndexCurrent.resize(m_nbFaceSet,0);

	m_faceSetGroup 	= m_meshGroup.openGroup (SURFACE_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_faceSetGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbFaceSet);

	for(id_type i=0; i<m_nbFaceSet; i++) {
		m_faceSetNames[setNames[i]] = i;

		// Creation de son groupe :
		string	basicGroupName		= SUBGROUP_PREFIX + to_str (i);
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_faceSetGroup, basicGroupName);

		// Ecriture du nom du nuage :
		Attribute	nameAttribute	=
				HDFHelper::openOrCreateStringAttribute (
						cloudGroup, NAME_ATTR_NAME, ATTRIBUT_SIZE);
		HDFHelper::writeStringAttribute (nameAttribute, setNames[i]);

		Composition compoSurf;
		compoSurf.renseigne = 1;
		compoSurf.nb_elements = m_faceSetSizes[i];
		m_faceSetCompo.push_back(compoSurf);

//		writeComposition (compoSurf, CELL2D_COMPOSITION_DATASET_NAME, cloudGroup);

		// Ecriture du nombre de noeuds :
		Attribute	sizeNAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		sizeNAttribute.write (PredType::NATIVE_UINT32, &m_faceSetSizes[i]);

		// Ecriture des identifiants des noeuds :
		DataSet		faceSetIdsDataSet	=
				HDFHelper::openOrCreateDataSet1D (
						cloudGroup, DATASET_IDS_LIST_NAME, m_faceSetSizes[i],
						PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		m_faceSetIdsDataSet.push_back(faceSetIdsDataSet);

		m_faceSetGroups.push_back(cloudGroup);
	}

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeFaceSetInfo


void MaliPPWriter::writeFaceSetData (std::string name, id_type chunkSize, id_type* ids)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeFaceSetData ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	id_type setId = m_faceSetNames[name];

	// what is written in the mali file is nodesIds -1
	id_type* ids_tmp = new id_type[chunkSize];
	for(id_type i=0; i<chunkSize; i++) {
		ids_tmp[i] = ids[i]-1;
	}

	HDFHelper::writeUIntDataSet1D (m_faceSetIdsDataSet[setId], ids_tmp, chunkSize,
			m_faceSetIndexCurrent[setId]);
	delete[] ids_tmp;

	m_faceSetIndexCurrent[setId] += chunkSize;

	m_faceSetCompo[setId].renseigne = 0;

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeFaceSetData


void MaliPPWriter::writeFaceSetData (std::string name, id_type chunkSize, id_type* ids, id_type* nbNodesPerFace)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeFaceSetData ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	id_type setId = m_faceSetNames[name];

	// what is written in the mali file is nodesIds -1
	id_type* ids_tmp = new id_type[chunkSize];
	for(id_type i=0; i<chunkSize; i++) {
		ids_tmp[i] = ids[i]-1;
	}

	HDFHelper::writeUIntDataSet1D (m_faceSetIdsDataSet[setId], ids_tmp, chunkSize,
			m_faceSetIndexCurrent[setId]);
	delete[] ids_tmp;

	m_faceSetIndexCurrent[setId] += chunkSize;

	for(id_type i=0; i<chunkSize; i++) {
		m_faceSetCompo[setId].detail[nbNodesPerFace[i]]++;
	}

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeFaceSetData


void MaliPPWriter::writeFaceAttributes ( )
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeFaceAttributes ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbFaceAttributes = 0;

	m_faceAttributesGroup 	= m_meshGroup.openGroup (DATA_CELL_2D_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_faceAttributesGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbFaceAttributes);

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeFaceAttributes


void MaliPPWriter::writeFaceSetsAttributes ( )
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeFaceSetsAttributes ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbFaceSetsAttributes = 0;

	m_faceSetsAttributesGroup 	= m_meshGroup.openGroup (DATA_SURFACE_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_faceSetsAttributesGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbFaceSetsAttributes);

	for(id_type igrp=0; igrp<m_nbFaceSet; igrp++) {
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_faceSetGroups[igrp], DATA_CELL_2D_GROUP_NAME);
		Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		id_type size = 0;
		sizeAttribute.write (PredType::NATIVE_UINT32, &size);
	}

	for(id_type igrp=0; igrp<m_nbFaceSet; igrp++) {
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_faceSetGroups[igrp], DATA_EDGE_GROUP_NAME);
		Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		id_type size = 0;
		sizeAttribute.write (PredType::NATIVE_UINT32, &size);
	}

	for(id_type igrp=0; igrp<m_nbFaceSet; igrp++) {
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_faceSetGroups[igrp], DATA_NODE_GROUP_NAME);
		Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		id_type size = 0;
		sizeAttribute.write (PredType::NATIVE_UINT32, &size);
	}

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeFaceSetsAttributes


void MaliPPWriter::writeRegionsInfo (bool isContiguous, id_type nbRegions, id_type firstID)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeRegionsInfo ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbRegions = nbRegions;

	m_regionGroup 		= m_meshGroup.openGroup (CELL_3D_GROUP_NAME);
	// Le nombre d'arêtes :
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_regionGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);

	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbRegions);

	if(m_nbRegions>0) {
		Attribute	contiguousAttribut	= HDFHelper::openOrCreateAttribute (
				m_regionGroup, CONTIGUOUS_IDS_ATTR_NAME, PredType::NATIVE_INT);

		m_regionsIsContiguous = isContiguous;
		int contiguous =  (m_regionsIsContiguous ? 1 : 0);

		contiguousAttribut.write (PredType::NATIVE_INT, &contiguous);

		m_regionIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
				m_regionGroup, DATASET_IDS_NAME,
				(hsize_t)(m_regionsIsContiguous ? 1 : m_nbRegions),
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		if(m_regionsIsContiguous) {
			HDFHelper::writeUIntDataSet1D (m_regionIdsDataSet, &firstID, 1, 0);
		}

		// La liste des noeuds :
		// on prend 4*nombre de regions parce que l'on ne connait pas a priori
		// la taille totale du tableau region2nodeIDs
		m_region2nodeIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
				m_regionGroup, DATASET_IDS_LIST_NAME, 4 * m_nbRegions,
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);

		m_nbNodesPerRegionDataSet	= HDFHelper::openOrCreateDataSet1D (
								m_regionGroup, DATASET_TYPES_NAME, m_nbRegions,
								PredType::NATIVE_INT32, true, CELL2D_CHUNK_SIZE);
	}

	m_compoRegion.renseigne = 1;
	m_compoRegion.nb_elements = m_nbRegions;

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}	// MaliPPWriter::writeRegionsInfo


void MaliPPWriter::writeRegions (id_type regionsChunkSize, id_type* region2nodeIDs, Lima::Polyedre::PolyedreType* regionTypes, id_type* ids)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeRegions ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	HDFHelper::writeUIntDataSet1D (m_nbNodesPerRegionDataSet, (id_type*)regionTypes, regionsChunkSize, m_regionIndexCurrent);

	if(!m_regionsIsContiguous) {
		HDFHelper::writeUIntDataSet1D (m_regionIdsDataSet, ids, regionsChunkSize, m_regionIndexCurrent);
	}

	size_t nbNodesTot = 0;
	for(id_type iRegion=0; iRegion<regionsChunkSize; iRegion++) {
//		nbNodesTot += regionTypes[iRegion];
//		id_type nbNodes =
//		switch(regionTypes[iRegion]) {
//		case Lima::Polyedre::TETRAEDRE :
			nbNodesTot += Lima::Polyedre::PolyedreNbNode[regionTypes[iRegion]];
//			break;
//		case Lima::Polyedre::PYRAMIDE :

//		}
//		m_compoRegion.detail[regionTypes[iRegion]]++;
			m_compoRegion.detail[Lima::Polyedre::PolyedreNbNode[regionTypes[iRegion]]]++;
	}

	// check that the allocated size is sufficient;
	// 3*m_nbRegions is the initial allocated size
	if(m_nbNodesPerRegionIndexCurrent + nbNodesTot > 4*m_nbRegions) {
		m_region2nodeIdsDataSet	= HDFHelper::openOrCreateDataSet1D (
				m_regionGroup, DATASET_IDS_LIST_NAME, m_nbNodesPerRegionIndexCurrent + nbNodesTot,
				PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
	}
	// what is written in the mali file is nodesIds -1
	id_type* region2nodeIDs_tmp = new id_type[nbNodesTot];
	for(id_type i=0; i<nbNodesTot; i++) {
		region2nodeIDs_tmp[i] = region2nodeIDs[i]-1;
	}
	HDFHelper::writeUIntDataSet1D (m_region2nodeIdsDataSet, region2nodeIDs_tmp, nbNodesTot, m_nbNodesPerRegionIndexCurrent);
	delete[] region2nodeIDs_tmp;

	m_regionIndexCurrent += regionsChunkSize;
	m_nbNodesPerRegionIndexCurrent += nbNodesTot;

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeRegions


void MaliPPWriter::writeRegionSetInfo (id_type nbSet, std::vector<std::string> setNames, std::vector<id_type> setSizes)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeRegionSetInfo ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbRegionSet = nbSet;
	m_regionSetSizes = setSizes;
	m_regionSetIndexCurrent.clear();
	m_regionSetIndexCurrent.resize(m_nbRegionSet,0);

	m_regionSetGroup 	= m_meshGroup.openGroup (VOLUME_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_regionSetGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbRegionSet);

	for(id_type i=0; i<m_nbRegionSet; i++) {
		m_regionSetNames[setNames[i]] = i;

		// Creation de son groupe :
		string	basicGroupName		= SUBGROUP_PREFIX + to_str (i);
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_regionSetGroup, basicGroupName);

		// Ecriture du nom du nuage :
		Attribute	nameAttribute	=
				HDFHelper::openOrCreateStringAttribute (
						cloudGroup, NAME_ATTR_NAME, ATTRIBUT_SIZE);
		HDFHelper::writeStringAttribute (nameAttribute, setNames[i]);

		Composition compoVol;
		compoVol.renseigne = 1;
		compoVol.nb_elements = m_regionSetSizes[i];
		m_regionSetCompo.push_back(compoVol);
//		writeComposition (compoVol, CELL3D_COMPOSITION_DATASET_NAME, cloudGroup);

		// Ecriture du nombre de noeuds :
		Attribute	sizeNAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		sizeNAttribute.write (PredType::NATIVE_UINT32, &m_regionSetSizes[i]);

		// Ecriture des identifiants des noeuds :
		DataSet		regionSetIdsDataSet	=
				HDFHelper::openOrCreateDataSet1D (
						cloudGroup, DATASET_IDS_LIST_NAME, m_regionSetSizes[i],
						PredType::NATIVE_UINT32, true, NODE_CHUNK_SIZE);
		m_regionSetIdsDataSet.push_back(regionSetIdsDataSet);

		m_regionSetGroups.push_back(cloudGroup);
	}

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeRegionSetInfo


void MaliPPWriter::writeRegionSetData (std::string name, id_type chunkSize, id_type* ids)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeRegionSetData ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	id_type setId = m_regionSetNames[name];

	// what is written in the mali file is nodesIds -1
	id_type* ids_tmp = new id_type[chunkSize];
	for(id_type i=0; i<chunkSize; i++) {
		ids_tmp[i] = ids[i]-1;
	}

	HDFHelper::writeUIntDataSet1D (m_regionSetIdsDataSet[setId], ids_tmp, chunkSize,
			m_regionSetIndexCurrent[setId]);
	delete[] ids_tmp;

	m_regionSetIndexCurrent[setId] += chunkSize;

	m_regionSetCompo[setId].renseigne = 0;

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeRegionSetData


void MaliPPWriter::writeRegionSetData (std::string name, id_type chunkSize, id_type* ids, Lima::Polyedre::PolyedreType* regionTypes)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeRegionSetData ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	id_type setId = m_regionSetNames[name];

	// what is written in the mali file is nodesIds -1
	id_type* ids_tmp = new id_type[chunkSize];
	for(id_type i=0; i<chunkSize; i++) {
		ids_tmp[i] = ids[i]-1;
	}

	HDFHelper::writeUIntDataSet1D (m_regionSetIdsDataSet[setId], ids_tmp, chunkSize,
			m_regionSetIndexCurrent[setId]);
	delete[] ids_tmp;

	m_regionSetIndexCurrent[setId] += chunkSize;

	for(id_type i=0; i<chunkSize; i++) {
		m_regionSetCompo[setId].detail[Lima::Polyedre::PolyedreNbNode[regionTypes[i]]]++;
	}

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeRegionSetData


void MaliPPWriter::writeRegionAttributes ( )
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeRegionAttributes ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbRegionAttributes = 0;

	m_regionAttributesGroup 	= m_meshGroup.openGroup (DATA_CELL_3D_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_regionAttributesGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbRegionAttributes);

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeRegionAttributes


void MaliPPWriter::writeRegionSetsAttributes ( )
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeRegionSetsAttributes ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

	m_nbRegionSetsAttributes = 0;

	m_regionSetsAttributesGroup 	= m_meshGroup.openGroup (DATA_VOLUME_GROUP_NAME);
	Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
			m_regionSetsAttributesGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
	sizeAttribute.write (PredType::NATIVE_UINT32, &m_nbRegionSetsAttributes);

	for(id_type igrp=0; igrp<m_nbRegionSet; igrp++) {
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_regionSetGroups[igrp], DATA_CELL_3D_GROUP_NAME);
		Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		id_type size = 0;
		sizeAttribute.write (PredType::NATIVE_UINT32, &size);
	}

	for(id_type igrp=0; igrp<m_nbRegionSet; igrp++) {
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_regionSetGroups[igrp], DATA_CELL_2D_GROUP_NAME);
		Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		id_type size = 0;
		sizeAttribute.write (PredType::NATIVE_UINT32, &size);
	}

	for(id_type igrp=0; igrp<m_nbRegionSet; igrp++) {
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_regionSetGroups[igrp], DATA_EDGE_GROUP_NAME);
		Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		id_type size = 0;
		sizeAttribute.write (PredType::NATIVE_UINT32, &size);
	}

	for(id_type igrp=0; igrp<m_nbRegionSet; igrp++) {
		HDF145::Group cloudGroup = HDFHelper::openOrCreateGroup (m_regionSetGroups[igrp], DATA_NODE_GROUP_NAME);
		Attribute	sizeAttribute	= HDFHelper::openOrCreateAttribute (
				cloudGroup, SIZE_ATTR_NAME, PredType::NATIVE_UINT32);
		id_type size = 0;
		sizeAttribute.write (PredType::NATIVE_UINT32, &size);
	}

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeRegionSetsAttributes


void MaliPPWriter::writeComposition (const Composition& composition, const std::string& name, CommonFG& root)
{
	BEGIN_TRY_CATCH_BLOCK("MaliPPWriter::writeComposition ")

	// Inactivation de l'affichage des messages d'erreur HDF sur le terminal :
	HDFErrorHandlerManager		errorHandlerManager;

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

	COMPLETE_TRY_CATCH_BLOCK
	if (throwExc) {
		throw write_erreur (errorMsg.str ( ));
	}
}  // MaliPPWriter::writeComposition


MaliPPWriter::MaliPPWriter ( )
{
	assert (0 && "MaliPPWriter::MaliPPWriter is not allowed.");
}	// MaliPPWriter::MaliPPWriter


MaliPPWriter::MaliPPWriter (const MaliPPWriter&)
{
	assert (0 && "MaliPPWriter copy constructor is not allowed.");
}	// MaliPPWriter::MaliPPWriter


MaliPPWriter& MaliPPWriter::operator = (const MaliPPWriter&)
{
	assert (0 && "MaliPPWriter operator = is not allowed.");
	return *this;
}	// MaliPPWriter::operator =


MaliPPWriter::~MaliPPWriter ( )
{
	assert (0 && "MaliPPWriter::~MaliPPWriter is not allowed.");
}	// MaliPPWriter::~MaliPPWriter


void MaliPPWriter::utilisation_zlib (bool& utilise, int& niveau)
{
	utilise	= HDFHelper::zlibCompressionActivated ( );
	niveau	= HDFHelper::zlibLevel ( );
}	// MaliPPWriter::utilisation_zlib


void MaliPPWriter::desactiver_compression ( )
{
	HDFHelper::deactivateCompression ( );
}	// MaliPPWriter::desactiver_compression


void MaliPPWriter::activer_compression_zlib (int niveau)
{
	HDFHelper::activateZlibCompression (niveau);
}	// MaliPPWriter::activer_compression_zlib

#endif	// __INTERNE_MALIPP_WRITER


// ===========================================================================
//                    LA CLASSE HDFAttributeDescriptorType
// ===========================================================================


HDFAttributeDescriptorType::HDFAttributeDescriptorType ( )
	: CompType (sizeof (HDFAttributeDescriptor))
{
	StrType		type (PredType::C_S1);
	type.setSize (ATTRIBUT_SIZE);
	insertMember (m_name, HOFFSET (HDFAttributeDescriptor, m_name), type);
	insertMember (m_type, HOFFSET (HDFAttributeDescriptor, m_type), 
	               PredType::NATIVE_INT16);
	insertMember (m_size, HOFFSET (HDFAttributeDescriptor, m_size), 
	               PredType::NATIVE_UINT32);
}	// HDFAttributeDescriptorType::HDFAttributeDescriptorType


HDFAttributeDescriptorType::HDFAttributeDescriptorType (const HDFAttributeDescriptorType& at)
	: CompType (at)
{
}	// HDFAttributeDescriptorType copy constructor


HDFAttributeDescriptorType& HDFAttributeDescriptorType::operator = (const HDFAttributeDescriptorType&)
{
	return *this;
}	// HDFAttributeDescriptorType::operator =


END_NAMESPACE_LIMA
#endif
