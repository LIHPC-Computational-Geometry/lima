#ifdef __INTERNE_MALIPP
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : hdf_helper.h
 Cree le : Fri Sep  6 11:11:22 CEST 2002
 Derniere modification le : Wed Apr 30 08:05:45 MEST 2003
 Auteur  : C. Pignerol
******************************************************************************/

/*!
  \file hdf_helper.h
  \brief Surcouche de l'API C++ de HDF proposant des services spécialisés.
*/

#ifndef _HDF_HELPER_H
#define _HDF_HELPER_H

#include "LimaP/config_it.h"
#include "Lima/size.h"
#include <vector>
#include <HDF145/H5Cpp.h>


/*
#ifndef H5_NO_NAMESPACE
#define IN_H5 H5::
#else
#define IN_H5
#endif	// H5_NO_NAMESPACE
*/
#define IN_H5 HDF145::


/**
 * Classe d'aide à l'utilisation de HDF.
 * <B>Attention</B> : pour les opérations d'écriture la modification des
 * propriétés de compression ne sont pas possibles dès lors que le
 * <I>DataSet</I> est créé (avec HDF 1.6.5). Cela concerne notamment les
 * réécritures dans un fichier.
 */
class HDFHelper
{

	public:

	// Services lies aux groupes :
	static IN_H5 Group openOrCreateGroup (
						IN_H5 CommonFG& root, const IN_STD string& name);
	static void removeGroup (IN_H5 CommonFG& root, 
						const IN_STD string& groupName);

	// Services lies aux attributs :
	static IN_H5 Attribute openOrCreateAttribute (IN_H5 H5Object& root, 
						const IN_STD string& name, const IN_H5 PredType& type);
	static IN_H5 Attribute openOrCreateStringAttribute (IN_H5 H5Object& root, 
						const IN_STD string& name, IN_H5 hsize_t size);
	static IN_H5 Attribute openOrCreateArrayAttribute (IN_H5 H5Object& root,
						const IN_STD string& name, const IN_H5 DataType& type,
						IN_H5 hsize_t rank, IN_H5 hsize_t dims[]);
	static void writeStringAttribute (IN_H5 Attribute& attribute, 
						const IN_STD string& str);
	static IN_STD string readStringAttribute (IN_H5 H5Object& root, 
						const IN_STD string& name);

	// Services lies aux datasets :
	static IN_H5 DataSet openOrCreateStringDataSet (IN_H5 CommonFG& root,
						const IN_STD string& name, IN_H5 hsize_t size,
						bool extensible = true, IN_H5 hsize_t chunkSize = 1024);
	static IN_H5 DataSet openOrCreateDataSet1D (IN_H5 CommonFG& root,
						const IN_STD string& name, IN_H5 hsize_t size,
						const IN_H5 DataType& type, bool extensible = true,
						IN_H5 hsize_t chunkSize = 1024);
	static IN_H5 DataSet openOrCreateDataSet2D (IN_H5 CommonFG& root,
						const IN_STD string& name, IN_H5 hsize_t size,
						const IN_H5 DataType& type, bool extensible = true,
						IN_H5 hsize_t chunkSize = 1024);
	static IN_H5 DataSet openOrCreateDataSet3D (IN_H5 CommonFG& root,
						const IN_STD string& name, IN_H5 hsize_t size,
						const IN_H5 DataType& type, bool extensible = true,
						IN_H5 hsize_t chunkSize = 1024);
	static void writeIntDataSet1D (IN_H5 DataSet& dataSet, int_4* data, 
						size_t dataCount, size_t from = 0);
	static void writeUIntDataSet1D (IN_H5 DataSet& dataSet, u_int_4* data, 
						size_t dataCount, size_t from = 0);
	static void writeDoubleDataSet1D (IN_H5 DataSet& dataSet, double* data, 
						size_t dataCount, size_t from = 0);
	static void writeStringDataSet1D (IN_H5 DataSet& dataSet, char* data, 
						size_t dataCount, size_t from = 0);
	static void writeDoubleDataSet2D (IN_H5 DataSet& dataSet, double* data, 
						size_t dataCount, size_t from = 0);
	static void writeDoubleDataSet3D (IN_H5 DataSet& dataSet, double* data, 
						size_t dataCount, size_t from = 0);
	static void writeDataSet1D (IN_H5 DataSet& dataSet, void* data, 
						size_t dataCount, size_t from = 0);
	static void writeDataSet2D (IN_H5 DataSet& dataSet, void* data, 
						size_t dataCount, size_t from = 0);
	static void writeDataSet3D (IN_H5 DataSet& dataSet, void* data, 
						size_t dataCount, size_t from = 0);
	static void readStringDataSet (IN_H5 DataSet& dataSet, 
	                    IN_STD vector<IN_STD string>& str, size_t dataCount, 
						size_t from = 0);
	static void readDoubleDataSet1D (IN_H5 DataSet& dataSet, const double* data,
						size_t dataCount, size_t from = 0);
	static void readIntDataSet1D (IN_H5 DataSet& dataSet, const int_4* data,
						size_t dataCount, size_t from = 0);
	static void readUIntDataSet1D (IN_H5 DataSet& dataSet, const u_int_4* data,
						size_t dataCount, size_t from = 0);
	static void readDoubleDataSet2D (IN_H5 DataSet& dataSet, const double* data,
						size_t dataCount, size_t from = 0);
	static void readDoubleDataSet3D (IN_H5 DataSet& dataSet, const double* data,
						size_t dataCount, size_t from = 0);

	// Services liés à la compression :
	static void deactivateCompression ( );
	static bool zlibCompressionActivated ( );
	static int zlibLevel ( );
	static void activateZlibCompression (int level);
	

	protected :

	static IN_H5 DataSet openOrCreateDataSetnD (size_t dim, 
						IN_H5 CommonFG& root,
						const IN_STD string& name, IN_H5 hsize_t size,
						const IN_H5 DataType& type, bool extensible = true,
						IN_H5 hsize_t chunkSize = 1024);
	static void writeDataSetnD (size_t dim, IN_H5 DataSet& dataSet, 
						const IN_H5 DataType& type, void* data, 
						size_t dataCount, size_t from);
	static void readDataSetnD (size_t dim, IN_H5 DataSet& dataSet, 
						const IN_H5 DataType& type, void* data, 
						size_t dataCount,
						size_t from);
	static void applyCreationProperties (IN_H5 DSetCreatPropList& properties);


	private :

	static bool	compressionEnabled;
	static bool	zlibCompressionEnabled;
	static int	zlibCompressionLevel;
};	// class HDFHelper


class HDFErrorHandlerManager
{
	public :

#if defined(HDF5_1_4_3) || defined(HDF5_1_6_5)
	HDFErrorHandlerManager (IN_H5 H5E_auto_t function = 0, void* clientData = 0);
#else	// => HDF5 > 1.6.5
	HDFErrorHandlerManager (IN_H5 H5E_auto2_t function = 0, void* clientData = 0);
#endif	// HDF5_1_4_3
	~HDFErrorHandlerManager ( );


	private :

	HDFErrorHandlerManager (const HDFErrorHandlerManager&)
	{ }

	HDFErrorHandlerManager& operator = (const HDFErrorHandlerManager&)
	{ return *this; }

	/*! Ancien handler d'affichage des erreurs, remis en service par le 
	destructeur. */
#if defined(HDF5_1_4_3) || defined(HDF5_1_6_5)
	IN_H5 H5E_auto_t		m_function;
#else	// => HDF5 > 1.6.5
	IN_H5 H5E_auto2_t		m_function;
#endif	// HDF5_1_4_3

	//! Anciennes données clientes.
	void*			m_clientData;
};	// class HDFErrorHandlerManager


#ifdef HDF5_1_4_3
class ArrayType : public IN_H5 DataType
{
	public :

	ArrayType (IN_H5 hid_t base, int rank, const IN_H5 hsize_t dims[/*rank*/], 
	           const int perm[/*rank*/]);
	ArrayType (const ArrayType&);
	ArrayType& operator = (const ArrayType&);
	virtual ~ArrayType ( );


	protected :

	ArrayType ( );
	ArrayType (const IN_H5 hid_t existing_id);
};	// class ArrayType
#endif	// HDF5_1_4_3


#endif	// _HDF_HELPER_H
#endif	// __INTERNE_MALIPP
