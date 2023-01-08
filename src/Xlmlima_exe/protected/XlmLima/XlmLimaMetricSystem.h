#ifndef XLMLIMA_METRIC_SYSTEM_H
#define XLMLIMA_METRIC_SYSTEM_H


#include "XlmLima/xlmlima_config.h"
#include "XlmLima/XlmLimaAction.h"


BEGIN_NAMESPACE_XLMLIMA

/**
 * Cette classe d'action permet de passer un maillage d'un système
 * métrique à un autre (ex : mètre -> centimètre).
 */
class XlmLimaMetricSystem : public XlmLimaAction 
{
	public :

	/**
	 * Les unités de système métrique cibles.
	 */
	enum UNIT {METER, CENTIMETER, MILLIMETER, MICROMETER};

	/**
	 * Constructeur.
	 * @param		Nom du fichier en entrée.
	 * @param		Format du fichier en entrée.
	 * @param		Nom du fichier en sortie.
	 * @param		Format du fichier en sortie.
	 * @param		Unité métrique cible.
	 */
	XlmLimaMetricSystem (const IN_STD string& inputFile, 
	                     IN_LIMA format_t inputFormat,
	                     const IN_STD string& outputFile,
	                     IN_LIMA format_t outputFormat,
	                     UNIT unit);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~XlmLimaMetricSystem ( );

	/**
	 * @return		Le nom du fichier en sortie
	 */
	virtual const IN_STD string& getOutputFile ( ) const
	{ return _outputFile; }

	/**
	 * @return		Le format du fichier en sortie
	 */
	virtual const IN_LIMA format_t getOutputFormat ( ) const
	{ return _outputFormat; }

	/**
	 * @return		L'unité cible.
	 */
	virtual UNIT getUnit ( ) const
	{ return _unit; }

	/**
	 * Effectue la conversion. Lève une exception en cas d'échec.
	 */
	virtual void run ( );

	/**
	 * @param		Coefficient multiplicatif Lima à appliquer sur les 
	 *				coordonnées pour obtenir des mètres.
	 * @return		L'unité correspondant au coefficient multiplicatif 
	 *				Lima à appliquer sur les coordonnées pour obtenir
	 *				des mètres.
	 * @exception	Susceptible de lever une exception si le coefficient ne
	 *				correspond pas à une valeur de UNIT.
	 */
	static UNIT coefToUnit (double coef);
	
	/**
	 * @return		Le facteur à appliquer aux coordonnées des noeuds pour
	 *				passer du système d'unité unit1 au système unit2.
	 */
	static double multiplier (UNIT unit1, UNIT unit2);

	/**
	 * @param		Le système métrique concerné.
	 * @return		Le coefficient à appliquer aux coordonnées des noeuds
	 *				pour obtenir des mètres.
	 */
	static double limaMultiplier (UNIT unit);

	
	private :

	XlmLimaMetricSystem (const XlmLimaMetricSystem&)
		: XlmLimaAction ("", IN_LIMA SUFFIXE)
	{ }

	XlmLimaMetricSystem& operator = (const XlmLimaMetricSystem&)
	{ return *this; }

	/** Le nom du fichier en sortie. */
	IN_STD string			_outputFile;

	/** Le format du fichier en sortie. */
	IN_LIMA format_t		_outputFormat;

	/** L'unité cible. */
	UNIT					_unit;
};	//  class XlmLimaMetricSystem


END_NAMESPACE_XLMLIMA

#endif	// XLMLIMA_METRIC_SYSTEM_H
