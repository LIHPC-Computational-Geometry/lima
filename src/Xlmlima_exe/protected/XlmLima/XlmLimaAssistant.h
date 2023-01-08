#ifndef XLMLIMA_ASSISTANT_H
#define XLMLIMA_ASSISTANT_H


#include "XlmLima/xlmlima_config.h"
#include "XlmLima/XlmLimaAction.h"

#include <Lima/enum.h>


BEGIN_NAMESPACE_XLMLIMA

/**
 * Cette classe d'action permet de préparer un maillage et d'enregistrer
 * la préparation dans un autre fichier.
 */
class XlmLimaAssistant : public XlmLimaAction 
{
	public :

	/**
	 * Constructeur 1 : préparation unitaire.
	 * @param		Nom du fichier en entrée.
	 * @param		Format du fichier en entrée.
	 * @param		Préparation à effectuer
	 * @param		Nom du fichier en sortie.
	 * @param		Format du fichier en sortie.
	 */
	XlmLimaAssistant (const IN_STD string& inputFile, 
	                  IN_LIMA format_t inputFormat,
	                  IN_LIMA preparation_t prepa,
	                  const IN_STD string& outputFile,
	                  IN_LIMA format_t outputFormat);

	/**
	 * Constructeur 2 : 2 préparations successives à base de 
	 * préparations élémentaires.
	 * @param		Nom du fichier en entrée.
	 * @param		Format du fichier en entrée.
	 * @param		Préparation 1 à effectuer
	 * @param		Préparation 2 à effectuer
	 * @param		Nom du fichier en sortie.
	 * @param		Format du fichier en sortie.
	 */
	XlmLimaAssistant (const IN_STD string& inputFile, 
	                  IN_LIMA format_t inputFormat,
	                  IN_LIMA TypeMasque prepa1,
	                  IN_LIMA TypeMasque prepa2,
	                  const IN_STD string& outputFile,
	                  IN_LIMA format_t outputFormat);

	/**
	 * Constructeur 3 : préparation unitaire, puis 2 préparations 
	 * successives à base de préparations élémentaires.
	 * @param		Nom du fichier en entrée.
	 * @param		Format du fichier en entrée.
	 * @param		Préparation unitaire à effectuer
	 * @param		Préparation 1 à effectuer
	 * @param		Préparation 2 à effectuer
	 * @param		Nom du fichier en sortie.
	 * @param		Format du fichier en sortie.
	 */
	XlmLimaAssistant (const IN_STD string& inputFile, 
	                  IN_LIMA format_t inputFormat,
	                  IN_LIMA preparation_t prepa,
	                  IN_LIMA TypeMasque prepa1,
	                  IN_LIMA TypeMasque prepa2,
	                  const IN_STD string& outputFile,
	                  IN_LIMA format_t outputFormat);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~XlmLimaAssistant ( );

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
	 * Effectue la préparation. Lève une exception en cas d'échec.
	 */
	virtual void run ( );

	
	private :

	XlmLimaAssistant (const XlmLimaAssistant&)
		: XlmLimaAction ("", IN_LIMA SUFFIXE)
	{ }

	XlmLimaAssistant& operator = (const XlmLimaAssistant&)
	{ return *this; }

	/** Le nom du fichier en sortie. */
	IN_STD string			_outputFile;

	/** Le format du fichier en sortie. */
	IN_LIMA format_t		_outputFormat;

	/**
	 * Le type de préparation à effectuer :
	 * - Unitaire,
	 * - A base de préparations élémentaires,
	 * Unitaire puis à base de préparations élémentaires.
	 */
	enum PREPA_TYPE { UNITARY, ELEMENTARY, UNITARY_THEN_ELEMENTARY };

	/** Type de préparation à effectuer. */
	PREPA_TYPE				_prepaType;

	/** L'éventuelle préparation à effectuer. */
	IN_LIMA preparation_t	_prepa;

	/** Les éventuelles préparations à base de préparations 
	 * élémentaires à effectuer. */
	IN_LIMA TypeMasque	_prepa1, _prepa2;
};	//  class XlmLimaAssistant


END_NAMESPACE_XLMLIMA

#endif	// XLMLIMA_ASSISTANT_H
