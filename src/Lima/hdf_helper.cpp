#ifdef __INTERNE_MALIPP
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : hdf_helper.C
 Cree le : Fri Sep  6 11:11:22 CEST 2002
 Derniere modification le : Wed Apr 30 08:05:45 MEST 2003
 Auteur  : C. Pignerol
******************************************************************************/

/*!   
  \file hdf_helper.C
  \brief Surcouche de l'API C++ de HDF proposant des services spécialisés.
*/

#include "LimaP/hdf_helper.h"
#include "LimaP/using_it.h"

using namespace std;
#include <iostream>
#include <string.h>

#ifndef HDF5_1_4_3
#include <HDF145/H5ArrayType.h>
#endif	// HDF5_1_4_3


//#ifndef H5_NO_NAMESPACE
using namespace HDF145;
//#endif  // H5_NO_NAMESPACE

bool HDFHelper::compressionEnabled		= false;
bool HDFHelper::zlibCompressionEnabled	= false;
int HDFHelper::zlibCompressionLevel		= 0;


//////////////////////////////////////////////////////////////////////////////
//! Ouvre un groupe ou le crée si nécéssaire
Group HDFHelper::openOrCreateGroup (CommonFG& root, const string& name)
{
	Group	group;

	try
	{
		group	= root.openGroup (name);
	}
	catch (const Exception&)
	{
		group	= root.createGroup (name);
	}

	return group;
}	// HDFHelper::openOrCreateGroup

//////////////////////////////////////////////////////////////////////////////
//! Supprime, s'il existe, le groupe groupName de l'objet root.
void HDFHelper::removeGroup (CommonFG& root, const string& groupName)
{
	try
	{
		root.unlink (groupName);
	}
	catch (const Exception&)
	{
	}
}	// HDFHelper::removeGroup

//////////////////////////////////////////////////////////////////////////////
//! Ouvre un attribut de type entier ou le crée si nécessaire.
Attribute HDFHelper::openOrCreateAttribute (
			H5Object& root, const string& name, const PredType& type)
{
#ifdef HDF5_1_4_3
	Attribute	attribute (0);
#else	// => HDF5 > 1.4.3
	Attribute	attribute;
#endif	// HDF5_1_4_3

	try
	{
		attribute	= root.openAttribute (name);
	}
	catch (const Exception&)
	{
#ifdef HDF5_1_4_3
		DataSpace	dataSpace (H5S_SIMPLE);
#else	// => HDF5 > 1.4.3
		hsize_t dims [1]	= {1};
		DataSpace	dataSpace (H5S_SIMPLE);
		dataSpace.setExtentSimple (1, dims, dims);
#endif	// HDF5_1_4_3
		attribute	= root.createAttribute (name, type, dataSpace);
	}

	return attribute;
}	// HDFHelper::openOrCreateAttribute

//////////////////////////////////////////////////////////////////////////////
/*! Ouvre un attribut de type chaine de caractères ou le crée si 
   nécessaire. */
Attribute HDFHelper::openOrCreateStringAttribute (
				H5Object& root, const string& name, hsize_t size)
{
#ifdef HDF5_1_4_3
	Attribute	attribute (0);
#else	// => HDF5 > 1.4.3
	Attribute	attribute;	// NEW
#endif	// HDF5_1_4_3
	StrType		type (PredType::C_S1);
	type.setSize (size);

	try
	{
		attribute	= root.openAttribute (name);
#ifdef DEBUG_HDF_HELPER
		hsize_t	storageSize	= attribute.getDataType ( ).getSize ( );
		char*	buffer	= new char [storageSize];
		attribute.read (type, buffer);
		printf ("%s %d NAME = %s SIZE = %d VALUE = %s\n", __FILE__, __LINE__,
		        name.c_str ( ), (int)storageSize, buffer);
		delete [] buffer;
#endif	// DEBUG_HDF_HELPER
	}
	catch (const Exception&)
	{
#ifdef HDF5_1_4_3
		DataSpace	dataSpace (H5S_SIMPLE);
#else	// => HDF5 > 1.4.3
		hsize_t dims [1]	= {1};
		DataSpace	dataSpace (H5S_SIMPLE);
		dataSpace.setExtentSimple (1, dims, dims);
#endif	// HDF5_1_4_3
		attribute	= root.createAttribute (name, type, dataSpace);
	}

	return attribute;
}	// HDFHelper::openOrCreateStringAttribute

//////////////////////////////////////////////////////////////////////////////
/*! Ouvre un attribut de type tableau de données de type "type". La dimension
	du tableau est renseignée par rank, et la taille de chaque dimension par
	dims [rank]. Les valeurs de ce tableau doivent être strictement 
	positives.
    Si un tel attribut existe déjà il est préalablement détruit.
	Attention : les attributs sont stockés dans l'en-tête de l'objet dont la
	taille est limitée à environ 64 ko.
*/
Attribute HDFHelper::openOrCreateArrayAttribute (
				H5Object& root, const string& name, const DataType& type,
				hsize_t rank, hsize_t dims[])
{
	try
	{
		root.openAttribute (name);
#ifdef DEBUG_HDF_HELPER
		printf ("%s %d L'attribut %s existe deja -> destruction.\n",
		        __FILE__, __LINE__, name.c_str ( ));
#endif	// DEBUG_HDF_HELPER
		root.removeAttr (name);
#ifdef DEBUG_HDF_HELPER
		printf ("%s %d Attribut %s detruit.\n",__FILE__,__LINE__,name.c_str( ));
#endif	// DEBUG_HDF_HELPER
	}
	catch (const Exception&)
	{
	}

#ifdef DEBUG_HDF_HELPER
	printf ("%s %d Creation du type tableau de taille %d dims[0] = %d\n",
	        __FILE__,__LINE__, rank, dims [0]);
#endif	// DEBUG_HDF_HELPER

#ifdef HDF5_1_4_3
	ArrayType	dataType (type.getId ( ), rank, dims, NULL);
	Attribute	attribute (0);
#else	// => HDF5 > 1.4.3
	ArrayType	dataType (type, rank, dims);
	Attribute	attribute;
#endif	// HDF5_1_4_3

	try
	{
		attribute	= root.openAttribute (name);
#ifdef DEBUG_HDF_HELPER
		printf ("%s %d ERREUR : l'attribut %d existe.\n",
		        __FILE__, __LINE__, name.c_str ( ));
#endif	// DEBUG_HDF_HELPER
	}
	catch (const Exception&)
	{
#ifdef DEBUG_HDF_HELPER
		printf ("%s %d Creation de l'attribut %s ...\n",
		        __FILE__, __LINE__, name.c_str ( ));
#endif	// DEBUG_HDF_HELPER
#ifdef HDF5_1_4_3
		DataSpace	dataSpace (H5S_SIMPLE);
#else	// => HDF5 > 1.4.3
		hsize_t dims [1]	= {1};
		DataSpace	dataSpace (H5S_SIMPLE);
		dataSpace.setExtentSimple (1, dims, dims);
#endif	// HDF5_1_4_3
		attribute	= root.createAttribute (name, dataType, dataSpace);
#ifdef DEBUG_HDF_HELPER
		printf ("%s %d Attribut %s cree.\n", __FILE__, __LINE__, name.c_str ( ));
#endif	// DEBUG_HDF_HELPER
	}

	return attribute;
}	// HDFHelper::openOrCreateArrayAttribute

//////////////////////////////////////////////////////////////////////////////
/*! Ecrit la chaine de caractères reçue en second argument dans
    l'attribut reçu en premier argument. attribute doit être de type
    PredType::C_S1. La chaine str est tronquée si cela s'avère
	nécessaire. */
void HDFHelper::writeStringAttribute (Attribute& attribute, const string& str)
{
	hsize_t		storageSize	= attribute.getDataType ( ).getSize ( );
	char*		buffer		= new char [storageSize];
	strncpy (buffer, str.c_str ( ), storageSize);
	buffer [storageSize - 1]	= '\0';

	try
	{
		attribute.write (attribute.getDataType ( ), buffer);
	}
	catch (...)
	{
		delete [] buffer;
		throw;
	}

	delete [] buffer;
}	// HDFHelper::writeStringAttribute

//////////////////////////////////////////////////////////////////////////////
//! Retourne la chaine lue dans l'attribut name de l'objet root.
string HDFHelper::readStringAttribute (H5Object& root, const string& name)
{
	Attribute	attribute	= root.openAttribute (name);
	hsize_t		storageSize	= attribute.getDataType ( ).getSize ( );
	char*		buffer		= new char [storageSize];

	try
	{
		attribute.read (attribute.getDataType ( ), buffer);
	}
	catch (...)
	{
		delete [] buffer;
		throw;
	}

	string		attributeValue (buffer);
#ifdef DEBUG_HDF_HELPER
	printf ("%s %d NAME = %s SIZE = %d VALUE = %s\n", __FILE__, __LINE__,
	        name.c_str ( ), (int)storageSize, buffer);
#endif	// DEBUG_HDF_HELPER
	delete [] buffer;

	return attributeValue;
}

//////////////////////////////////////////////////////////////////////////////
/*! Ouvre un dataset de type NATIVE_CHAR ou le crée si nécessaire. Si
	extensible vaut true ce data set sera extensible, et chunkSize fixe alors
	l'espace ajouté à chaque extension. */
DataSet HDFHelper::openOrCreateStringDataSet (
			CommonFG& root, const string& name, hsize_t size, bool extensible, 
			hsize_t chunkSize)
{
// Rem : PredType::C_S1 n'est pas utilise car longueur fixe.
	DataSet	dataSet;
	hsize_t	dims [1];

	dims [0] = size + 1;

	try
	{
		dataSet	= root.openDataSet (name);
#ifdef DEBUG_HDF_HELPER
		hsize_t	currentDims [1];
		dataSet.getSpace ( ).getSimpleExtentDims (currentDims);
		if (currentDims [0] >= 1)
		{
			char*	buffer	= new char [currentDims [0]];
			dataSet.read (buffer, PredType::NATIVE_CHAR);
			buffer [currentDims [0] - 1]	= '\0';
			printf("%s %d NAME = %s SIZE = %d VALUE = %s\n", __FILE__, __LINE__,
			        name.c_str ( ), (int)currentDims [0], buffer);
			delete [] buffer;
		}
#endif	// DEBUG_HDF_HELPER
	}
	catch (const Exception&)
	{
		hsize_t				maxDims [1];
		maxDims [0]	= (true == extensible ? H5S_UNLIMITED : size);
		DataSpace			dataSpace (1, dims, maxDims);
		DSetCreatPropList	properties;

		if (false == extensible)
		{
			dataSet	= root.createDataSet (
							name, PredType::NATIVE_CHAR, dataSpace, 0);
		}
		else
		{
			hsize_t	chunk_dims [1]	= {chunkSize};
			properties.setChunk (1, chunk_dims);
			applyCreationProperties (properties);
			dataSet	= root.createDataSet (
							name, PredType::NATIVE_CHAR, dataSpace, properties);
		}	// else if (false == extensible)
	}

	if (true == extensible)
	{
		try
		{
			hsize_t	storageSize	= dataSet.getStorageSize ( );
			if (storageSize < dims [0])
				dataSet.extend (dims);
		}
		catch (const Exception& exc)
		{
			dataSet.extend (dims);
		}
	}	// if (true == extensible)

	return dataSet;
}	// HDFHelper::openOrCreateStringDataSet

//////////////////////////////////////////////////////////////////////////////
/* Ouvre un dataset de type "type" et de "size" éléments, ou le créé si
	nécessaire. Si extensible vaut true ce dataset sera extensible, et chunkSize f
	fixe alors l'espace ajouté à chaque extension. */
DataSet HDFHelper::openOrCreateDataSet1D (
	CommonFG& root, const string& name, hsize_t size, const DataType& type,
	bool extensible, hsize_t chunkSize)
{
	return openOrCreateDataSetnD (1, root, name, size, type, extensible, 
	                              chunkSize);
/*
	DataSet	dataSet;
	hsize_t	dims [1];
	dims [0] = size;

	try
	{
		dataSet	= root.openDataSet (name);
#ifdef DEBUG_HDF_HELPER
		hsize_t	currentDims [1];
		dataSet.getSpace ( ).getSimpleExtentDims (currentDims);
		printf ("%s %d NAME = %s SIZE = %d\n", __FILE__, __LINE__,
			    name.c_str ( ), (int)currentDims [0]);
#endif	// DEBUG_HDF_HELPER
	}
	catch (const Exception&)
	{
		hsize_t				maxDims [1];
		maxDims [0]	= (true == extensible ? H5S_UNLIMITED : size);
		DataSpace			dataSpace (1, dims, maxDims);
		DSetCreatPropList	properties;
		hsize_t				chunk_dims [1];
		chunk_dims [0]	= chunkSize;
		properties.setChunk (1, chunk_dims);
		dataSet	= root.createDataSet (name, type, dataSpace, 
				true == extensible ? properties : 0);
	}

	if (true == extensible)
	{
		try
		{
//			hsize_t	storageSize	= dataSet.getStorageSize ( );
//			if (storageSize < dims [0])
				dataSet.extend (dims);
		}
		catch (const Exception& exc)
		{
			dataSet.extend (dims);
		}
	}	// if (true == extensible)

	return dataSet;
*/
}	// HDFHelper::openOrCreateDataSet1D

//////////////////////////////////////////////////////////////////////////////
/* Ouvre un dataset de type "type" et de "size" éléments de dimension 2, ou le
	crée si nécessaire. Si extensible vaut true ce dataset sera extensible, et
	chunkSize fixe alors l'espace ajouté à chaque extension. */
DataSet HDFHelper::openOrCreateDataSet2D (
	CommonFG& root, const string& name, hsize_t size, const DataType& type,
	bool extensible, hsize_t chunkSize)
{
	return openOrCreateDataSetnD (2, root, name, size, type, extensible, 
	                              chunkSize);
}	// HDFHelper::openOrCreateDataSet2D

//////////////////////////////////////////////////////////////////////////////
/* Ouvre un dataset de type "type" et de "size" éléments de dimension 3, ou le
	crée si nécessaire. Si extensible vaut true ce dataset sera extensible, et
	chunkSize fixe alors l'espace ajouté à chaque extension. */
DataSet HDFHelper::openOrCreateDataSet3D (
	CommonFG& root, const string& name, hsize_t size, const DataType& type,
	bool extensible, hsize_t chunkSize)
{
	return openOrCreateDataSetnD (3, root, name, size, type, extensible, 
	                              chunkSize);
}	// HDFHelper::openOrCreateDataSet3D

//////////////////////////////////////////////////////////////////////////////
/*! Ecrit data dans dataSet, où dataCount est le nombre de données
   contenues dans data. L'espace de dataSet doit être au moins aussi
   grand que dataCount. dataSet doit être de dimension 1. L'écriture
   commence à la position from. */
void HDFHelper::writeIntDataSet1D (DataSet& dataSet, int_4* data, 
                                   size_t dataCount, size_t from)
{
	writeDataSetnD (1, dataSet, PredType::NATIVE_INT32, data, dataCount, from);
}	// HDFHelper::writeIntDataSet1D

//////////////////////////////////////////////////////////////////////////////
/*! Ecrit data dans dataSet, où dataCount est le nombre de données
   contenues dans data. L'espace de dataSet doit être au moins aussi
   grand que dataCount. dataSet doit être de dimension 1. L'écriture
   commence à la position from. */
void HDFHelper::writeUIntDataSet1D (DataSet& dataSet, u_int_4* data, 
                                    size_t dataCount, size_t from)
{
	writeDataSetnD (1, dataSet, PredType::NATIVE_UINT32, data, dataCount, from);
}	// HDFHelper::writeUIntDataSet1D

//////////////////////////////////////////////////////////////////////////////
/*! Ecrit data dans dataSet, où dataCount est le nombre de données
   contenues dans data. L'espace de dataSet doit être au moins aussi
   grand que dataCount. dataSet doit être de dimension 1. L'écriture
   commence à la position from. */
void HDFHelper::writeDoubleDataSet1D (DataSet& dataSet, double* data, 
                                      size_t dataCount, size_t from)
{
	writeDataSetnD (1, dataSet, PredType::NATIVE_DOUBLE, data, dataCount, from);
}	// HDFHelper::writeDoubleDataSet1D

//////////////////////////////////////////////////////////////////////////////
/*! Ecrit data dans dataSet, où dataCount est le nombre de données
   contenues dans data. L'espace de dataSet doit être au moins aussi
   grand que dataCount. dataSet doit être de dimension 2. Le type des
   donnees de l'espace doit être PredType::C_S1. Les chaines transmises 
   dans data doivent toutes être de même longueur (définie par
   dataSet.getDataType ( ). L'écriture commence à la position from. */
void HDFHelper::writeStringDataSet1D (DataSet& dataSet, char* data, 
                                      size_t dataCount, size_t from)
{
	// Rem : data est une chaine unique contenant les dataCount chaines.
	writeDataSetnD (1, dataSet, PredType::C_S1, data, 1, from);
}	// HDFHelper::writeStringDataSet1D

//////////////////////////////////////////////////////////////////////////////
/*! Ecrit data dans dataSet, où dataCount est le nombre de données
   contenues dans data. L'espace de dataSet doit être au moins aussi
   grand que dataCount. dataSet doit être de dimension 2. L'écriture
   commence à la position from. */
void HDFHelper::writeDoubleDataSet2D (DataSet& dataSet, double* data, 
                                      size_t dataCount, size_t from)
{
	writeDataSetnD (2, dataSet, PredType::NATIVE_DOUBLE, data, dataCount, from);
}	// HDFHelper::writeDoubleDataSet2D

//////////////////////////////////////////////////////////////////////////////
/*! Ecrit data dans dataSet, où dataCount est le nombre de données
   contenues dans data. L'espace de dataSet doit être au moins aussi
   grand que dataCount. dataSet doit être de dimension 3. L'écriture
   commence à la position from. */
void HDFHelper::writeDoubleDataSet3D (DataSet& dataSet, double* data, 
                                      size_t dataCount, size_t from)
{
	writeDataSetnD (3, dataSet, PredType::NATIVE_DOUBLE, data, dataCount, from);
}	// HDFHelper::writeDoubleDataSet3D

//////////////////////////////////////////////////////////////////////////////
/*! Ecrit data dans dataSet, où dataCount est le nombre de données
   contenues dans data. Les données data doivent être du même type
   que celle de dataSet, et l'espace de dataSet doit être au moins aussi
   grand que dataCount. dataSet doit être de dimension 1. L'écriture
   commence à la position from. */
void HDFHelper::writeDataSet1D (DataSet& dataSet, void* data, size_t dataCount,
                                size_t from)
{
	writeDataSetnD (1, dataSet, dataSet.getDataType ( ), data, dataCount, from);
}	// HDFHelper::writeDataSet1D

//////////////////////////////////////////////////////////////////////////////
/*! Ecrit data dans dataSet, où dataCount est le nombre de données
   contenues dans data. Les données data doivent être du même type
   que celle de dataSet, et l'espace de dataSet doit être au moins aussi
   grand que dataCount. dataSet doit être de dimension 2. L'écriture
   commence à la position from. */
void HDFHelper::writeDataSet2D (DataSet& dataSet, void* data, size_t dataCount,
                                size_t from)
{
	writeDataSetnD (2, dataSet, dataSet.getDataType ( ), data, dataCount, from);
}	// HDFHelper::writeDataSet2D

//////////////////////////////////////////////////////////////////////////////
/*! Ecrit data dans dataSet, où dataCount est le nombre de données
   contenues dans data. Les données data doivent être du même type
   que celle de dataSet, et l'espace de dataSet doit être au moins aussi
   grand que dataCount. dataSet doit être de dimension 3. L'écriture
   commence à la position from. */
void HDFHelper::writeDataSet3D (DataSet& dataSet, void* data, size_t dataCount,
                                size_t from)
{
	writeDataSetnD (3, dataSet, dataSet.getDataType ( ), data, dataCount, from);
}	// HDFHelper::writeDataSet3D

//////////////////////////////////////////////////////////////////////////////
/*! Lit data dans dataSet, où dataCount est le nombre de données
   à lire. Les données data doivent être du même type
   que celle de dataSet (entiers signés sur 32 bits), et l'espace de 
   dataSet doit être au moins aussi grand que dataCount. dataSet doit 
   être de dimension 1. La lecture commence à la position from. */
void HDFHelper::readIntDataSet1D (DataSet& dataSet, const int_4* data, 
                                  size_t dataCount, size_t from)
{
	if (0 == dataCount)	// Eviter un plantage
		return;

	hsize_t			dims [1];
	dims [0]	= dataCount;
#ifdef HDF5_1_4_3
//	hssize_t		start [1] = {from};		// CP, version 5.4.3
	hssize_t		start [1] = {(hssize_t)from};	// CP, version 6.37.0, suite armclang++ v 19
#else	// => HDF5 > 1.4.3
	hsize_t			start [1] = {from};		// CP, version 6.2.0
#endif	// HDF5_1_4_3
	DataSpace		memorySpace (1, dims);
	DataSpace		fileSpace = dataSet.getSpace ( );
	fileSpace.selectHyperslab (H5S_SELECT_SET, dims, start);
	dataSet.read ((void*)data, PredType::NATIVE_INT32, memorySpace, fileSpace);
}	// HDFHelper::readIntDataSet1D

//////////////////////////////////////////////////////////////////////////////
/*! Lit data dans dataSet, où dataCount est le nombre de données
   à lire. Les données data doivent être du même type
   que celle de dataSet (entiers non signés sur 32 bits), et l'espace de 
   dataSet doit être au moins aussi grand que dataCount. dataSet doit 
   être de dimension 1. La lecture commence à la position from. */
void HDFHelper::readUIntDataSet1D (DataSet& dataSet, const u_int_4* data, 
                                   size_t dataCount, size_t from)
{
	if (0 == dataCount)	// Eviter un plantage
		return;

	hsize_t			dims [1];
	dims [0]	= dataCount;
#ifdef HDF5_1_4_3
//	hssize_t		start [1] = {from};		// CP, version 5.4.3
	hssize_t		start [1] = {(hssize_t)from};	// CP, version 6.27.0, suite armclang++ v 19
#else	// => HDF5 > 1.4.3
	hsize_t			start [1] = {from};		// CP, version 6.2.0
#endif	// HDF5_1_4_3
	DataSpace		memorySpace (1, dims);
	DataSpace		fileSpace = dataSet.getSpace ( );
	fileSpace.selectHyperslab (H5S_SELECT_SET, dims, start);
	dataSet.read ((void*)data, PredType::NATIVE_UINT32, memorySpace, fileSpace);
}	// HDFHelper::readUIntDataSet1D

//////////////////////////////////////////////////////////////////////////////
/*! Lit dataCount chaines de caractères dans dataSet et les affecte au
	vecteur donné en argument. dataSet doit être un dataset de stockage
	de chaines de caractères de type PredType::C_S1. La lecture commence à
    la position from. */
void HDFHelper::readStringDataSet (DataSet& dataSet, vector<string>& strings,
                                   size_t dataCount, size_t from)
{
	if (0 == dataCount)	// Eviter un plantage
		return;

	strings.resize (dataCount);

	DataType		strType	= dataSet.getDataType ( );
	hsize_t			dims [1];
	dims [0]	= dataCount;
#ifdef HDF5_1_4_3
//	hssize_t		start [1] = {from};	// CP, version 5.4.3
	hssize_t		start [1] = {(hssize_t)from};	// CP, version 6.37.0, suite armclang++ v 19
#else	// => HDF5 > 1.4.3
	hsize_t			start [1] = {from};	// CP, version 6.2.0
#endif	// HDF5_1_4_3
	DataSpace		memorySpace (1, dims);
	DataSpace		fileSpace = dataSet.getSpace ( );
	fileSpace.selectHyperslab (H5S_SELECT_SET, dims, start);
	char*			buffer	= new char [dataCount * strType.getSize ( ) + 1];
	memset (buffer, '\0', dataCount * strType.getSize ( ) + 1);

	try
	{
		dataSet.read ((void*)buffer, strType, memorySpace, fileSpace);
		char*	pos		= buffer;
		for (size_t count = 0; count < dataCount; 
		     count++, pos += strType.getSize ( ))
		{
			// ATTENTION : HDF met le caractere 0 en fin de chaine si celle-ci
			// est plus courte que strType.getSize ( ). Il ne faut pas le
			// recopier car sinon l'operateur == de la classe string ne
			// retourne pas true en cas d'egalite avec une autre chaine
			// identique mais ne comportant pas ce caractere 0.
			size_t	length	= strType.getSize ( ) < strlen (pos) ? 
							  strType.getSize ( )  : strlen (pos);
			strings [count].assign (pos, length);
		}
		delete [] buffer;
	}
	catch (...)
	{
		delete [] buffer;
		throw;
	}
}	// HDFHelper::readStringDataSet

//////////////////////////////////////////////////////////////////////////////
/*! Lit data dans dataSet, où dataCount est le nombre de données
   à lire. Les données data doivent être du même type
   que celle de dataSet (double), et l'espace de dataSet doit être au moins 
   aussi grand que dataCount. dataSet doit être de dimension 1. La lecture 
   commence à la position from. */
void HDFHelper::readDoubleDataSet1D (DataSet& dataSet, const double* data, 
                                     size_t dataCount, size_t from)
{
	readDataSetnD (1, dataSet, PredType::NATIVE_DOUBLE, (void*)data, 
	               dataCount, from);
}	// HDFHelper::readDoubleDataSet1D

//////////////////////////////////////////////////////////////////////////////
/*! Lit data dans dataSet, où dataCount est le nombre de données
   à lire. Les données data doivent être du même type
   que celle de dataSet (double), et l'espace de dataSet doit être au moins 
   aussi grand que dataCount. dataSet doit être de dimension 2. La lecture 
   commence à la position from. */
void HDFHelper::readDoubleDataSet2D (DataSet& dataSet, const double* data, 
                                     size_t dataCount, size_t from)
{
	readDataSetnD (2, dataSet, PredType::NATIVE_DOUBLE, (void*)data, 
	               dataCount, from);
}	// HDFHelper::readDoubleDataSet2D

//////////////////////////////////////////////////////////////////////////////
/*! Lit data dans dataSet, où dataCount est le nombre de données
   à lire. Les données data doivent être du même type
   que celle de dataSet (double), et l'espace de dataSet doit être au moins 
   aussi grand que dataCount. dataSet doit être de dimension 3. La lecture 
   commence à la position from. */
void HDFHelper::readDoubleDataSet3D (DataSet& dataSet, const double* data, 
                                     size_t dataCount, size_t from)
{
	readDataSetnD (3, dataSet, PredType::NATIVE_DOUBLE, (void*)data, 
	               dataCount, from);
}	// HDFHelper::readDoubleDataSet3D

//////////////////////////////////////////////////////////////////////////////
/*! Annule l'utilisation de tout compression.
 */
void HDFHelper::deactivateCompression ( )
{
	compressionEnabled		= false;
	zlibCompressionEnabled	= false;
}	// HDFHelper::deactivateCompression

//////////////////////////////////////////////////////////////////////////////
/*! Retourne true si la compression par la Zlib est activée, false dans le cas
 * contraire.
 */
bool HDFHelper::zlibCompressionActivated ( )
{
	return zlibCompressionEnabled;
}	// HDFHelper::zlibCompressionActivated

//////////////////////////////////////////////////////////////////////////////
/*! Retourne le niveau de compression par la Zlib utilisé.
 */
int HDFHelper::zlibLevel ( )
{
	return zlibCompressionLevel;
}	// HDFHelper::zlibLevel

//////////////////////////////////////////////////////////////////////////////
/*! Annule l'utilisation de tout compression.
 */
void HDFHelper::activateZlibCompression (int level)
{
	compressionEnabled		= true;
	zlibCompressionEnabled	= true;
	zlibCompressionLevel	= level;
}	// HDFHelper::activateZlibCompression

//////////////////////////////////////////////////////////////////////////////
/*! Ouvre un dataset de dimension dim et de type "type" ou le crée si 
    nécessaire. Si extensible vaut true ce data set sera extensible, et 
    chunkSize fixe alors l'espace ajouté à chaque extension. */
DataSet HDFHelper::openOrCreateDataSetnD (
		size_t dim, CommonFG& root, const string& name, hsize_t size, 
		const DataType& type,
		bool extensible, hsize_t chunkSize)
{
	DataSet	dataSet;
	hsize_t	dims [2];
	dims [0]	= size;
	dims [1]	= dim;

	try
	{
		dataSet	= root.openDataSet (name);
#ifndef HDF5_1_4_3
		// A-t-il les facteurs de compression souhaités ?
#endif	// HDF5_1_4_3
#ifdef DEBUG_HDF_HELPER
		hsize_t	currentDims [2]	= {0, 0};
		dataSet.getSpace ( ).getSimpleExtentDims (currentDims);
		printf ("%s %d NAME = %s SIZE = %d DIM = \n", __FILE__, __LINE__,
			    name.c_str ( ), (int)currentDims [0], (int)currentDims [1]);
#endif	// DEBUG_HDF_HELPER
	}
	catch (const Exception&)
	{
		hsize_t				maxDims [2];
		maxDims [0]	= (true == extensible ? H5S_UNLIMITED : size);
		maxDims [1]	= dim;
		int					rank	= dim > 1 ? 2 : 1;
		DataSpace			dataSpace (rank, dims, maxDims);
		DSetCreatPropList	properties;

		if (false == extensible)
		{
			dataSet	= root.createDataSet (name, type, dataSpace, 0);
		}
		else
		{
			hsize_t	chunk_dims [2]	= {chunkSize, dim};
			properties.setChunk (rank, chunk_dims);
			applyCreationProperties (properties);
			dataSet	= root.createDataSet (name, type, dataSpace, properties);
		}	// else if (false == extensible)
	}

	if (true == extensible)
		dataSet.extend (dims);

	return dataSet;
}	// HDFHelper::openOrCreateDataSetnD

//////////////////////////////////////////////////////////////////////////////
/*! Ecrit data dans dataSet, où dataCount est le nombre de données de 
   dimension dim contenues dans data. Les données data doivent être du 
   même type que celle de dataSet, et l'espace de dataSet doit être au 
   moins aussi grand que dataCount. L'écriture commence à la position
   from. */
void HDFHelper::writeDataSetnD (size_t dim, DataSet& dataSet,  
                                const DataType& type, void* data, 
                                size_t dataCount, size_t from)
{
	if (0 == dataCount)	// Eviter un plantage
		return;

	int				rank	= dim > 1 ? 2 : 1;
	hsize_t			dims [2];
	dims [0]	= dataCount;
	dims [1]	= dim;
#ifdef HDF5_1_4_3
//	hssize_t		start [2] = {from, 0};		// CP, version 5.4.3
	hssize_t		start [2] = {(hssize_t)from, 0};	// CP, version 6.37.0 suite armclang++ v 19
#else	// => HDF5 > 1.4.3
	hsize_t			start [2] = {from, 0};		// CP, version 6.2.0
#endif	// HDF5_1_4_3
	DataSpace		memorySpace (rank, dims);
	DataSpace		fileSpace = dataSet.getSpace ( );
	fileSpace.selectHyperslab (H5S_SELECT_SET, dims, start);
	dataSet.write (data, type, memorySpace, fileSpace);
}	// HDFHelper::writeDataSetnD

//////////////////////////////////////////////////////////////////////////////
/*! Lit data dans dataSet, où dataCount est le nombre de données
   de dimension dim à lire. Les données data doivent être du même
   type que celles de dataSet, et l'espace de dataSet doit être au moins
   aussi grand que dataCount. La lecture commence à la position from. */
void HDFHelper::readDataSetnD (size_t dim, DataSet& dataSet, 
                               const DataType& type, void* data, 
                               size_t dataCount, size_t from)
{
	if (0 == dataCount)	// Eviter un plantage
		return;

	int				rank	= dim > 1 ? 2 : 1;
	hsize_t			dims [2];
	dims [0]	= dataCount;
	dims [1]	= dim;
#ifdef HDF5_1_4_3
//	hssize_t		start [2] = {from, 0};	
	hssize_t		start [2] = {(hssize_t)from, 0};	// CP v 6.37.0 suite armclang++ v 19
#else	// => HDF5 > 1.4.3
	hsize_t			start [2] = {from, 0};	// CP V 6.2.0
#endif	// HDF5_1_4_3
	DataSpace		memorySpace (rank, dims);
	DataSpace		fileSpace = dataSet.getSpace ( );
	fileSpace.selectHyperslab (H5S_SELECT_SET, dims, start);
	dataSet.read ((void*)data, type, memorySpace, fileSpace);
}	// HDFHelper::readDataSetnD

//////////////////////////////////////////////////////////////////////////////
/*! Applique à <I>properties</I> les proriétés de création d'un <I>DataSet</I>
    en vigueur.  */
void HDFHelper::applyCreationProperties (DSetCreatPropList& properties)
{
	if (true == compressionEnabled)
	{
		if (true == zlibCompressionEnabled)
		{
			properties.setDeflate (zlibCompressionLevel);
		}	// if (true == zlibCompressionEnabled)
	}	// if (true == compressionEnabled)
}	// HDFHelper::applyCreationProperties

//////////////////////////////////////////////////////////////////////////////
/*! Recupère l'ancien handler d'affichage des erreurs et ses données
   clientes. Affecte le handler recu en argument à HDF. Si function est nul,
   les erreurs rencontrées ne seront pas affichées. */
HDFErrorHandlerManager::HDFErrorHandlerManager (
#if defined(HDF5_1_4_3) || defined(HDF5_1_6_5)
	H5E_auto_t function, void* clientData)
#else	// => HDF5 > 1.6.5
	H5E_auto2_t function, void* clientData)	// V 6.2.0
#endif	// HDF5_1_4_3
	: m_function (0), m_clientData (0)
{
#ifndef NDEBUG_HDF
	Exception::getAutoPrint (m_function, &m_clientData);

	if (0 == function)
		Exception::dontPrint ( );
	else
		Exception::setAutoPrint (function, clientData);
#endif	// NDEBUG_HDF
}	// HDFErrorHandlerManager::HDFErrorHandlerManager

//////////////////////////////////////////////////////////////////////////////
//! Réarme l'ancien handler d'affichage.
HDFErrorHandlerManager::~HDFErrorHandlerManager ( )
{
#ifndef NDEBUG_HDF
	Exception::setAutoPrint (m_function, m_clientData);
#endif	// NDEBUG_HDF
}	// HDFErrorHandlerManager::~HDFErrorHandlerManager


//////////////////////////////////////////////////////////////////////////////
/*! Constructeur de création du type. base est le type des données du
    tableau (ex : H5T_NATIVE_INT), rank le nombre de dimensions, dims un
    tableau de taille rank renseignat sur la taille de chaque dimension. perm
	n'est actuellement pas utilisé par HDF (fonction H5Tarray_create).
*/
#ifdef HDF5_1_4_3
ArrayType::ArrayType (hid_t base, int rank, const hsize_t dims[/*rank*/], 
                      const int perm[/*rank*/])
	: DataType ( )
{
	id	= H5Tarray_create (base, rank, dims, perm);

	if (0 >= id)
		throw DataSetIException ("ArrayType constructor", 
		                         "H5Tarray_create failed.");
}	// ArrayType::ArrayType ( )
#endif	// HDF5_1_4_3


//////////////////////////////////////////////////////////////////////////////
//! Constructeur par défaut : ne fait rien.
#ifdef HDF5_1_4_3
ArrayType::ArrayType ( )
	: DataType ( )
{
}	// ArrayType::ArrayType ( )
#endif	// HDF5_1_4_3


//////////////////////////////////////////////////////////////////////////////
//! Constructeur avec id : appelle DataType (existing_id).
#ifdef HDF5_1_4_3
ArrayType::ArrayType (const hid_t existing_id)
	: DataType (existing_id)
{
}	// ArrayType::ArrayType (const hid_t existing_id)
#endif	// HDF5_1_4_3


//////////////////////////////////////////////////////////////////////////////
//! Constructeur de copie.
#ifdef HDF5_1_4_3
ArrayType::ArrayType (const ArrayType& copied)
	: DataType (copied)
{
}	// ArrayType::ArrayType (const ArrayType& copied)
#endif	// HDF5_1_4_3


//////////////////////////////////////////////////////////////////////////////
//! Opérateur =.
#ifdef HDF5_1_4_3
ArrayType& ArrayType::operator = (const ArrayType& copied)
{
	if (this != &copied)
	{
		DataType::operator = ((DataType&)copied);
	}	// if (this != &copied)

	return *this;
}	// ArrayType::operator =
#endif	// HDF5_1_4_3


//////////////////////////////////////////////////////////////////////////////
//! Destructeur : RAS.
#ifdef HDF5_1_4_3
ArrayType::~ArrayType ( )
{
}	// ArrayType::~ArrayType ( )
#endif	// HDF5_1_4_3



#endif	// __INTERNE_MALIPP
