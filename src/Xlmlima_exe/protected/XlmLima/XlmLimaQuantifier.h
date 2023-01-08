#ifndef XLMLIMA_QUANTIFIER_H
#define XLMLIMA_QUANTIFIER_H


#include "XlmLima/xlmlima_config.h"
#include "XlmLima/XlmLimaAction.h"

#include <Lima/maillage.h>


BEGIN_NAMESPACE_XLMLIMA

/**
 * Cette classe d'action permet d'afficher des informations sur un maillage.
 */
class XlmLimaQuantifier : public XlmLimaAction 
{
	public :

	/**
	 * Constructeur.
	 * @param		Nom du fichier en entrée.
	 * @param		Format du fichier en entrée.
	 */
	XlmLimaQuantifier (const IN_STD string& inputFile, IN_LIMA format_t format);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~XlmLimaQuantifier ( );

	/**
	 * Affiche les informations concernant le maillage associé.
	 */
	virtual void run ( );

	/**
	 * @return		Le nombre de connexions bras vers noeud du maillage reçu
	 *				en argument.
	 */
	static size_t numberOfConnectionsEdgeNode (const IN_LIMA Maillage& mesh);

	/**
	 * @return		Le nombre de connexions polygone vers noeud du maillage 
	 *				reçu en argument.
	 */
	static size_t numberOfConnectionsPolygonNode (const IN_LIMA Maillage& mesh);

	/**
	 * @return		Le nombre de connexions polyèdre vers noeud du maillage 
	 *				reçu en argument.
	 */
	static size_t numberOfConnectionsPolyedronNode (const IN_LIMA Maillage& mesh);

	/**
	 * @return		Le nombre de connexions nuage vers noeud du maillage 
	 *				reçu en argument.
	 */
	static size_t numberOfConnectionsCloudNode (const IN_LIMA Maillage& mesh);

	/**
	 * @return		Le nombre de connexions ligne vers arête du maillage 
	 *				reçu en argument.
	 */
	static size_t numberOfConnectionsLineEdge (const IN_LIMA Maillage& mesh);

	/**
	 * @return		Le nombre de connexions surface vers polygone du maillage 
	 *				reçu en argument.
	 */
	static size_t numberOfConnectionsFacePolygon (const IN_LIMA Maillage& mesh);

	/**
	 * @return		Le nombre de connexions volume vers polyèdre du maillage 
	 *				reçu en argument.
	 */
	static size_t numberOfConnectionsVolumePolyedron (const IN_LIMA Maillage& mesh);

	/**
	 * @return		Une chaine de caractères représentant la 
	 *				géométrie	du maillage reçu en argument.
	 */
	static IN_STD string meshGeometry (const IN_LIMA Maillage& mesh);


	private :

	XlmLimaQuantifier (const XlmLimaQuantifier&)
		: XlmLimaAction ("", IN_LIMA SUFFIXE)
	{ }

	XlmLimaQuantifier& operator = (const XlmLimaQuantifier&)
	{ return *this; }
};	//  class XlmLimaQuantifier


END_NAMESPACE_XLMLIMA

#endif	// XLMLIMA_QUANTIFIER_H
