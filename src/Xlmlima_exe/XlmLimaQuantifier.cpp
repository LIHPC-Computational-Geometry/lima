#include "XlmLima/XlmLimaQuantifier.h"
#include "XlmLima/XlmLimaException.h"

#include <Lima/bras.h>
#include <Lima/polygone.h>
#include <Lima/polyedre.h>
#include <Lima/nuage.h>
#include <Lima/ligne.h>
#include <Lima/surface.h>
#include <Lima/volume.h>

#ifdef __hpux
#include <iostream.h>
#else
#include <iostream>
#endif	// __hpux


USING_STD
USING_LIMA


BEGIN_NAMESPACE_XLMLIMA

XlmLimaQuantifier::XlmLimaQuantifier (const string& inputFile, format_t format)
	: XlmLimaAction (inputFile, format)
{
}	// XlmLimaQuantifier::XlmLimaQuantifier


XlmLimaQuantifier::~XlmLimaQuantifier ( )
{
}	// XlmLimaQuantifier::~XlmLimaQuantifier


void XlmLimaQuantifier::run ( )
{
	try
	{
		Maillage	mesh;
		cout << "Lecture du maillage " << getInputFile ( ) << " ..." << endl;
		mesh.lire (getInputFile ( ), getInputFormat ( ));

		// Affichage des informations :
		cout << "Titre maillage = '" << mesh.titre ( ) << "'" << endl;
		cout << "Date maillage = '" << mesh.date ( ) << "'";
		cout << "\tUnite longueur maillage =" << mesh.unite_longueur ( )
		     << endl;
		cout << "Geometrie maillage = " << meshGeometry (mesh)
		     << "\tDim = " << mesh.dimension ( )
		     << "\tnombre de noeuds = " << mesh.nb_noeuds ( ) << endl;
		cout << "Qte bras =" << mesh.nb_bras ( )
		     << "\t\t\tQte connexions bras->noeuds =" 
		     << numberOfConnectionsEdgeNode (mesh) << endl;
		cout << "Qte mailles =" << mesh.nb_polygones ( )
		     << "\t\tQte connexions mailles->noeuds ="
		     << numberOfConnectionsPolygonNode (mesh) << endl;
		if (3 <= mesh.dimension ( ))
			cout << "Qte polyedres =" << mesh.nb_polyedres ( )
			     << "\t\tQte connexions polyedres->noeuds ="
			     << numberOfConnectionsPolyedronNode (mesh) << endl;
		cout << "Qte groupes de noeuds =" << mesh.nb_nuages ( )
		     << "\tQte connexions groupe->noeuds ="
		     << numberOfConnectionsCloudNode (mesh) << endl;
		cout << "Qte groupes de bras =" << mesh.nb_lignes ( )
		     << "\tQte connexions groupe->bras ="
		     << numberOfConnectionsLineEdge (mesh) << endl;
		cout << "Qte groupes de mailles =" << mesh.nb_surfaces ( )
		     << "\tQte connexions groupe->mailles ="
		     << numberOfConnectionsFacePolygon (mesh) << endl;
		if (3 <= mesh.dimension ( ))
			cout << "Qte groupes de polyedres =" << mesh.nb_volumes ( )
			     << "\tQte connexions groupe->polyedres ="
			     << numberOfConnectionsVolumePolyedron (mesh) << endl;
	}
	catch (const erreur& limaError)
	{	
		raiseLimaError (limaError);
	}
	catch (const exception& exc)
	{
		raiseUnexpectedError (exc.what ( ));
	}
	catch (...)
	{
		raiseUnexpectedError ("Erreur non documentée.");
	}
}	// XlmLimaQuantifier::raiseLimaError


size_t XlmLimaQuantifier::numberOfConnectionsEdgeNode (const Maillage& mesh)
{
	size_t	connections	= 0;

	for (size_t b = 0; b < mesh.nb_bras ( ); b++)
	{
		Bras	bras	= mesh.bras (b);
		connections		+= bras.nb_noeuds ( );
	}	// for (size_t b = 0; b < mesh.nb_bras ( ); b++)

	return connections;
}	// XlmLimaQuantifier::numberOfConnectionsEdgeNode


size_t XlmLimaQuantifier::numberOfConnectionsPolygonNode (const Maillage& mesh)
{
	size_t	connections	= 0;

	for (size_t p = 0; p < mesh.nb_polygones ( ); p++)
	{
		Polygone	poly	= mesh.polygone (p);
		connections			+= poly.nb_noeuds ( );
	}	// for (size_t p = 0; p < mesh.nb_polygones ( ); p++)

	return connections;
}	// XlmLimaQuantifier::numberOfConnectionsPolygonNode


size_t XlmLimaQuantifier::numberOfConnectionsPolyedronNode (const Maillage& mesh)
{
	size_t	connections	= 0;

	for (size_t p = 0; p < mesh.nb_polyedres ( ); p++)
	{
		Polyedre	poly	= mesh.polyedre (p);
		connections			+= poly.nb_noeuds ( );
	}	// for (size_t p = 0; p < mesh.nb_polyedres ( ); p++)

	return connections;
}	// XlmLimaQuantifier::numberOfConnectionsPolyedronNode


size_t XlmLimaQuantifier::numberOfConnectionsCloudNode (const Maillage& mesh)
{
	size_t	connections	= 0;

	for (size_t n = 0; n < mesh.nb_nuages ( ); n++)
	{
		Nuage	cloud	= mesh.nuage (n);
		connections		+= cloud.nb_noeuds ( );
	}	// for (size_t n = 0; n < mesh.nb_nuages ( ); n++)

	return connections;
}	// XlmLimaQuantifier::numberOfConnectionsCloudNode


size_t XlmLimaQuantifier::numberOfConnectionsLineEdge (const Maillage& mesh)
{
	size_t	connections	= 0;

	for (size_t l = 0; l < mesh.nb_lignes ( ); l++)
	{
		Ligne	line	= mesh.ligne (l);
		connections		+= line.nb_bras ( );
	}	// for (size_t l = 0; l < mesh.nb_lignes ( ); l++)

	return connections;
}	// XlmLimaQuantifier::numberOfConnectionsLineEdge


size_t XlmLimaQuantifier::numberOfConnectionsFacePolygon (const Maillage& mesh)
{
	size_t	connections	= 0;

	for (size_t s = 0; s < mesh.nb_surfaces ( ); s++)
	{
		Surface	face	= mesh.surface (s);
		connections		+= face.nb_polygones ( );
	}	// for (size_t s = 0; s < mesh.nb_surfaces ( ); s++)

	return connections;
}	// XlmLimaQuantifier::numberOfConnectionsFacePolygon


size_t XlmLimaQuantifier::numberOfConnectionsVolumePolyedron (const Maillage& mesh)
{
	size_t	connections	= 0;

	for (size_t v = 0; v < mesh.nb_volumes ( ); v++)
	{
		Volume	volume	= mesh.volume (v);
		connections		+= volume.nb_polyedres ( );
	}	// for (size_t v = 0; v < mesh.nb_volume ( ); v++)

	return connections;
}	// XlmLimaQuantifier::numberOfConnectionsVolumePolyedron


string XlmLimaQuantifier::meshGeometry (const Maillage& mesh)
{
	string	geometry ("NON DEFINI");

	switch (mesh.type_geometrie ( ))
	{
		case NORMAL			: geometry	= "NORMAL";			break;
		case AXISYMETRIQUE	: geometry	= "AXISYMETRIQUE";	break;
		case PERIODIQUE		: geometry	= "PERIODIQUE";		break;
	}	// switch (mesh.type_geometrie ( ))

	return geometry;
}	// XlmLimaQuantifier::meshGeometry


END_NAMESPACE_XLMLIMA
