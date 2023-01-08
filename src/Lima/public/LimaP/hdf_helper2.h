#ifdef __INTERNE_MALIPP2
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : hdf_helper2.h
 Cree le : 05/06/2019
 Derniere modification le :
 Auteur  : C. Pignerol
******************************************************************************/

/*!
  \file hdf_helper.h
  \brief Surcouche de l'API C++ de HDF proposant des services spécialisés.
*/

#ifndef _HDF_HELPER2_H
#define _HDF_HELPER2_H

#include "LimaP/config_it.h"
#include "Lima/size.h"
#include <vector>
#include <H5Ipublic.h>	// => hid_t ...
#include <H5Cpp.h>

#define IN_H5_1105

/**
 * Classe d'aide à l'utilisation de HDF.
 */
class HDFHelper2
{

	public:

	// Services lies aux groupes :
	static H5::Group openOrCreateGroup (H5::H5Location& root, const IN_STD string& name);
	static void removeGroup (H5::Group& root, const IN_STD string& groupName);

	// Services lies aux attributs :
	static H5::Attribute openOrCreateAttribute (H5::H5Object& root, 
						const IN_STD string& name, const H5::PredType& type);
	static H5::Attribute openOrCreateStringAttribute (H5::H5Object& root, 
						const IN_STD string& name, IN_H5_1105 hsize_t size);
	static H5::Attribute openOrCreateArrayAttribute (H5::H5Object& root,
						const IN_STD string& name, const H5::DataType& type,
						IN_H5_1105 hsize_t rank, IN_H5_1105 hsize_t dims[]);
	static void removeAttribute (H5::H5Object& root, const IN_STD string& name);
	static void writeStringAttribute (H5::Attribute& attribute, 
						const IN_STD string& str);
	static IN_STD string readStringAttribute (H5::H5Object& root, 
						const IN_STD string& name);

	// Services lies aux datasets :
	static H5::DataSet openOrCreateStringDataSet (H5::Group& root,
						const IN_STD string& name, IN_H5_1105 hsize_t size,
						bool extensible = true, IN_H5_1105 hsize_t chunkSize = 1024);
	static H5::DataSet openOrCreateDataSet1D (H5::Group& root,
						const IN_STD string& name, IN_H5_1105 hsize_t size,
						const H5::DataType& type, bool extensible = true,
						IN_H5_1105 hsize_t chunkSize = 1024);
	static H5::DataSet openOrCreateDataSet2D (H5::Group& root,
						const IN_STD string& name, IN_H5_1105 hsize_t size,
						const H5::DataType& type, bool extensible = true,
						IN_H5_1105 hsize_t chunkSize = 1024);
	static H5::DataSet openOrCreateDataSet3D (H5::Group& root,
						const IN_STD string& name, IN_H5_1105 hsize_t size,
						const H5::DataType& type, bool extensible = true,
						IN_H5_1105 hsize_t chunkSize = 1024);
	static void removeDataSet (H5::Group& root, 
						const IN_STD string& dataSetName);
	static void writeIntDataSet1D (H5::DataSet& dataSet, int_4* data, 
						size_t dataCount, size_t from = 0);
	static void writeUIntDataSet1D (H5::DataSet& dataSet, u_int_4* data, 
						size_t dataCount, size_t from = 0);
	static void writeDoubleDataSet1D (H5::DataSet& dataSet, double* data, 
						size_t dataCount, size_t from = 0);
	static void writeStringDataSet1D (H5::DataSet& dataSet, char* data, 
						size_t dataCount, size_t from = 0);
	static void writeDoubleDataSet2D (H5::DataSet& dataSet, double* data, 
						size_t dataCount, size_t from = 0);
	static void writeDoubleDataSet3D (H5::DataSet& dataSet, double* data, 
						size_t dataCount, size_t from = 0);
	static void writeDataSet1D (H5::DataSet& dataSet, void* data, 
						size_t dataCount, size_t from = 0);
	static void writeDataSet2D (H5::DataSet& dataSet, void* data, 
						size_t dataCount, size_t from = 0);
	static void writeDataSet3D (H5::DataSet& dataSet, void* data, 
						size_t dataCount, size_t from = 0);
	static void readStringDataSet (H5::DataSet& dataSet, 
	                    IN_STD vector<IN_STD string>& str, size_t dataCount, 
						size_t from = 0);
	static void readDoubleDataSet1D (H5::DataSet& dataSet, const double* data,
						size_t dataCount, size_t from = 0);
	static void readIntDataSet1D (H5::DataSet& dataSet, const int_4* data,
						size_t dataCount, size_t from = 0);
	static void readUIntDataSet1D (H5::DataSet& dataSet, const u_int_4* data,
						size_t dataCount, size_t from = 0);
	static void readDoubleDataSet2D (H5::DataSet& dataSet, const double* data,
						size_t dataCount, size_t from = 0);
	static void readDoubleDataSet3D (H5::DataSet& dataSet, const double* data,
						size_t dataCount, size_t from = 0);

	protected :

	static H5::DataSet openOrCreateDataSetnD (size_t dim, 
						H5::Group& root,
						const IN_STD string& name, IN_H5_1105 hsize_t size,
						const H5::DataType& type, bool extensible = true,
						IN_H5_1105 hsize_t chunkSize = 1024);
	static void writeDataSetnD (size_t dim, H5::DataSet& dataSet, 
						const H5::DataType& type, void* data, 
						size_t dataCount, size_t from);
	static void readDataSetnD (size_t dim, H5::DataSet& dataSet, 
						const H5::DataType& type, void* data, 
						size_t dataCount,
						size_t from);
	static void applyCreationProperties (H5::DSetCreatPropList& properties);

	// Services liés à la compression :
	static void deactivateCompression ( );
	static bool zlibCompressionActivated ( );
	static int zlibLevel ( );
	static void activateZlibCompression (int level);


	private :

	static bool	compressionEnabled;
	static bool	zlibCompressionEnabled;
	static int	zlibCompressionLevel;
};	// class HDFHelper2


class HDFErrorHandlerManager2
{
	public :

	HDFErrorHandlerManager2 (IN_H5_1105 H5E_auto2_t function = 0, void* clientData = 0);
	~HDFErrorHandlerManager2 ( );


	private :

	HDFErrorHandlerManager2 (const HDFErrorHandlerManager2&)
	{ }

	HDFErrorHandlerManager2& operator = (const HDFErrorHandlerManager2&)
	{ return *this; }

	/*! Ancien handler d'affichage des erreurs, remis en service par le 
	destructeur. */
	IN_H5_1105 H5E_auto2_t		m_function;

	//! Anciennes données clientes.
	void*			m_clientData;
};	// class HDFErrorHandlerManager2


#endif	// _HDF_HELPER2_H
#endif	// __INTERNE_MALIPP2
