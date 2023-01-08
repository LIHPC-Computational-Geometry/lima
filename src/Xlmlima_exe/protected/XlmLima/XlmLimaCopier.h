#ifndef XLMLIMA_COPIER_H
#define XLMLIMA_COPIER_H


#include "XlmLima/xlmlima_config.h"
#include "XlmLima/XlmLimaAction.h"


BEGIN_NAMESPACE_XLMLIMA

/**
 * Cette classe d'action permet de copier un maillage, à un format donné,
 * dans un autre fichier, de même format ou non.
 */
class XlmLimaCopier : public XlmLimaAction 
{
	public :

	/**
	 * Constructeur.
	 * @param		Nom du fichier en entrée.
	 * @param		Format du fichier en entrée.
	 * @param		Nom du fichier en sortie.
	 * @param		Format du fichier en sortie.
	 */
	XlmLimaCopier (const IN_STD string& inputFile, 
	               IN_LIMA format_t inputFormat,
	               const IN_STD string& outputFile,
	               IN_LIMA format_t outputFormat);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~XlmLimaCopier ( );

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
	 * Effectue la copie. Lève une exception en cas d'échec.
	 */
	virtual void run ( );

	
	private :

	XlmLimaCopier (const XlmLimaCopier&)
		: XlmLimaAction ("", IN_LIMA SUFFIXE)
	{ }

	XlmLimaCopier& operator = (const XlmLimaCopier&)
	{ return *this; }

	/** Le nom du fichier en sortie. */
	IN_STD string			_outputFile;

	/** Le format du fichier en sortie. */
	IN_LIMA format_t		_outputFormat;
};	//  class XlmLimaCopier


END_NAMESPACE_XLMLIMA

#endif	// XLMLIMA_COPIER_H
