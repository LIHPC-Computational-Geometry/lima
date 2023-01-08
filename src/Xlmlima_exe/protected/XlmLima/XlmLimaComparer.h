#ifndef XLMLIMA_COMPARER_H
#define XLMLIMA_COMPARER_H


#include "XlmLima/xlmlima_config.h"
#include "XlmLima/XlmLimaAction.h"

#include <Lima/maillage.h>


BEGIN_NAMESPACE_XLMLIMA

/**
 * Cette classe d'action permet de comparer deux maillages.
 */
class XlmLimaComparer : public XlmLimaAction 
{
	public :

	/**
	 * Constructeur.
	 * @param		Nom du premier fichier à comparer.
	 * @param		Format du premier fichier à comparer.
	 * @param		Nom du second fichier à comparer.
	 * @param		Format du second fichier à comparer.
	 */
	XlmLimaComparer (
			const IN_STD string& inputFile1, IN_LIMA format_t inputFormat1,
			const IN_STD string& inputFile2, IN_LIMA format_t inputFormat2);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~XlmLimaComparer ( );

	/**
	 * Affiche le résultat de la comparaison.
	 */
	virtual void run ( );

	/**
	 * @return		Le nom du second fichier à comparer.
	 */
	virtual const IN_STD string& getInputFile2 ( ) const
	{ return _inputFile2; }

	/**
	 * @return		Le format du second fichier à comparer.
	 */
	virtual IN_LIMA format_t getInputFormat2 ( ) const
	{ return _inputFormat2; }


	private :

	XlmLimaComparer (const XlmLimaComparer&)
		: XlmLimaAction ("", IN_LIMA SUFFIXE)
	{ }

	XlmLimaComparer& operator = (const XlmLimaComparer&)
	{ return *this; }

	/** Le second fichier à comparer. */
	IN_STD string		_inputFile2;

	/** Le format du second fichier à comparer. */
	IN_LIMA format_t	_inputFormat2;
};	//  class XlmLimaComparer


END_NAMESPACE_XLMLIMA

#endif	// XLMLIMA_COMPARER_H
