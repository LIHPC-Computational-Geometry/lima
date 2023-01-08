#ifndef XLMLIMA_ACTION_H
#define XLMLIMA_ACTION_H


#include "XlmLima/xlmlima_config.h"

#include <Lima/erreur.h>
#include <Lima/enum.h>

#include <string>


BEGIN_NAMESPACE_XLMLIMA

/**
 * Cette classe de base décrit une action xlmlima (vérification,
 * copie, ...". Sa méthode run, à surcharger, exécute l'action et
 * lève une exception en cas d'erreur.
 * Services de bases proposés :
 * - Gestion du nom du fichier en entrée,
 * - Gestion du format du fichier en entrée,
 * - Levée d'exception personnalisées
 */
class XlmLimaAction 
{
	public :

	/**
	 * Destructeur. RAS.
	 */
	virtual ~XlmLimaAction ( );

	/**
	 * @return		Le nom du fichier en entrée
	 */
	virtual const IN_STD string& getInputFile ( ) const
	{ return _inputFile; }

	/**
	 * @return		Le format du fichier en entrée
	 */
	virtual IN_LIMA format_t getInputFormat ( ) const
	{ return _inputFormat; }

	/**
	 * @return		Précision à utiliser lors de l'exécution de l'action.
	 */
	virtual double getPrecision ( )	const
	{ return _precision; }

	/**
	 * @param		Précision à utiliser lors de l'exécution de l'action.
	 * @exception	Lève une exception si la précision est négative.
	 */
	virtual void setPrecision (double precision);

	/**
	 * Exécute l'action. Lève une exception en cas d'échec.
	 */
	virtual void run ( ) = 0;


	protected :

	/**
	 * Constructeur.
	 * @param		Nom du fichier en entrée.
	 * @param		Format du fichier en entrée.
	 */
	XlmLimaAction (const IN_STD string& inputFile, IN_LIMA format_t format);

	/**
	 * Lève une exception dont le message signale qu'elle vient de lima
	 * et contient le message apporté par la chaine de caractères
	 * limaError.
	 */
	virtual void raiseLimaError (const IN_LIMA erreur& limaError);

	/**
	 * Retourne la chaine de caractères destinée à préfixer
	 * les messages d'erreurs issus de Lima.
	 */
	virtual IN_STD string limaErrorString ( );

	/**
	 * Lève une exception dont le message signale qu'elle est d'origine
	 * inconnue et contient le message apporté par la chaine de 
	 * caractères error.
	 */
	virtual void raiseUnexpectedError (const IN_STD string& error);

	
	private :

	XlmLimaAction (const XlmLimaAction&)
	{ }

	XlmLimaAction& operator = (const XlmLimaAction&)
	{ return *this; }

	/** Le nom du fichier en entrée. */
	IN_STD string			_inputFile;

	/** Le format du fichier en entrée. */
	IN_LIMA format_t		_inputFormat;

	/** Précision à utiliser lors de l'exécution de l'action. */
	double					_precision;
};	//  class XlmLimaAction


END_NAMESPACE_XLMLIMA

#endif	// XLMLIMA_ACTION_H
