#ifndef XLMLIMA_FACTORY_H
#define XLMLIMA_FACTORY_H


#include "XlmLima/xlmlima_config.h"
#include "XlmLima/XlmLimaCopier.h"
#include "XlmLima/XlmLimaQuantifier.h"
#include "XlmLima/XlmLimaAssistant.h"
#include "XlmLima/XlmLimaComparer.h"
#include "XlmLima/XlmLimaMetricSystem.h"
#include "XlmLima/XlmLimaException.h"



BEGIN_NAMESPACE_XLMLIMA

/**
 * Créé une instance de la classe XlmLimaAction selon le paramétrage
 * transmis.
 */
class XlmLimaFactory
{
	public :

	/**
	 * Créé l'action associée à la première chaine de 
	 * caractères du tableau reçu en argument :
	 * -cp : XlmLimaCopier
	 * -qte : XlmLimaQuantifier
	 * -prepa : XlmLimaAssistant
	 * Lève une exception si les arguments ne conviennent à aucune
	 * action.
	 */
	static XlmLimaAction* createAction (int argc, char* argv []);

	/**
	 * @return		La version du produit.
	 */
	static const IN_STD string& getVersion ( );


	protected :

	/**
	 * Création d'une instance de la classe XlmLimaCopier.
	 * @param		Nom du fichier en entrée
	 * @param		Format du fichier en entrée
	 * @param		Nom du fichier en sortie
	 * @param		Format du fichier en sortie
	 * @return		L'instance de copier créé. A détruire par 
	 *				l'appelant.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	static XlmLimaCopier* createCopier (const IN_STD string& infile,
	                                    IN_LIMA format_t inputFormat,
	                                    const IN_STD string& outfile,
	                                    IN_LIMA format_t outputFormat);


	/**
	 * Création d'une instance de la classe XlmLimaQuantifier.
	 * @param		Nom du fichier en entrée
	 * @param		Format du fichier en entrée
	 * @return		L'instance de quantifier créé. A détruire par 
	 *				l'appelant.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	static XlmLimaQuantifier* createQuantifier (const IN_STD string& infile,
	                                            IN_LIMA format_t format);

	/**
	 * Création d'une instance de la classe XlmLimaAssistant.
	 * @param		Nom du fichier en entrée
	 * @param		Format du fichier en entrée
	 * @param		Préparation à effectuer
	 * @param		Nom du fichier en sortie (chaine vide si pas de sortie).
	 * @param		Format du fichier en sortie
	 * @return		L'instance d'assistant créée. A détruire par 
	 *				l'appelant.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	static XlmLimaAssistant* createAssistant (const IN_STD string& infile,
	                                          IN_LIMA format_t inputFormat,
	                                          const IN_STD string& prepa,
	                                          const IN_STD string& outfile,
	                                          IN_LIMA format_t outputFormat);

	/**
	 * Création d'une instance de la classe XlmLimaMetricSystem.
	 * @param		Nom du fichier en entrée
	 * @param		Format du fichier en entrée
	 * @param		Système métrique cible.
	 * @param		Nom du fichier en sortie.
	 * @param		Format du fichier en sortie
	 * @return		L'instance créée. A détruire par 
	 *				l'appelant.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	static XlmLimaMetricSystem* createMetricSystem (const IN_STD string& infile,
	                                          IN_LIMA format_t inputFormat,
	                                          XlmLimaMetricSystem::UNIT unit,
	                                          const IN_STD string& outfile,
	                                          IN_LIMA format_t outputFormat);

	/**
	 * Lève une exception contenant la syntaxe d'utilisation de l'outil.
	 */
	static void syntax ( );

	/**
	 * @return		Le réel correspondant à la chaine de caractères transmise en
	 *				argument.
	 * @exception	Une exception est levée en cas d'erreur de conversion.
	 */
	static double stringToDouble (const IN_STD string& str);

	/**
	 * @return		La préparation lima correspondant à la chaine de
	 *				caractères reçue.
	 * @exception	Une exception est levée si la préparation est
	 *				inconnue.
	 */
	static IN_LIMA preparation_t stringToPreparation (const IN_STD string& prepa);

	/**
	 * @return		Le format de fichier correspondant à la chaine de
	 *				caractères reçue.
	 * @exception	Une exception est levée si le format de fichier est
	 *				inconnu.
	 */
	static IN_LIMA format_t stringToFormat (const IN_STD string& format);

	/**
	 * @param		Le système métrique sous forme de chaîne de 
	 *				caractères ("m", "cm", "mm" ou "um").
	 * @return		Le système métrique cible.
	 */
	static XlmLimaMetricSystem::UNIT stringToMetricSystem (const IN_STD string& system);

	/**
	 * @return		La liste des formats supportés en lecture
	 * @since		7.5.0
	 */
	static std::string readableFormats ( ); 

	/**
	 * @return		La liste des formats supportés en écriture
	 * @since		7.5.0
	 */
	static std::string writableFormats ( ); 	
	

	private :

	/**
	 * Constructeurs et destructeurs interdits.
	 */
	XlmLimaFactory ( )
	{ }
	XlmLimaFactory (const XlmLimaFactory&)
	{ }
	XlmLimaFactory& operator = (const XlmLimaFactory&)
	{ return *this; }
	~XlmLimaFactory ( )
	{ }
};	// class XlmLimaFactory


END_NAMESPACE_XLMLIMA

#endif	// XLMLIMA_FACTORY_H
