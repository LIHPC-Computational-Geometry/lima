#ifndef _CALLBACK_H
#define _CALLBACK_H

#include "config.h"

#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

/** Décrit des instances appelables lors de la lecture du maillage afin
 * de connaitre l'état d'avancement de la lecture et d'éventuellement
 * pouvoir l'interrompre.
 */
class ReaderCallback
{
	public :

	/**
	 * Les types de données lues.
	 * NOEUD	: lecture des noeuds du maillage
	 * BRAS		: lecture des bras du maillage
	 * POLYGONE	: lecture des polygônes du maillage
	 * POLYEDRE	: lecture des polyèders du maillage
	 * GROUPE	: lecture des données de groupes. Dans certains formats de 
	 *			  fichier les données ne sont pas regroupées par 
	 *			  dimension (ex, avec IDEAS, noeuds, bras, polygônes et 
	 *			  polyèdres peuvent faire partie d'un même groupe).
	 * NUAGE	: lecture des noeuds d'un nuage
	 * LIGNE	: lecture des bras d'une ligne
	 * SURFACE	: lecture des polygônes d'une surface
	 * VOLUME	: lecture des polyèdres d'un volume
	 * ATTR_MAILLAGE	: lecture des attributs du maillage
	 * ATTR_NOEUD		: lecture des attributs des noeuds du maillage ou d'un 
	 *					  nuage
	 * ATTR_BRAS		: lecture des attributs des bras du maillage ou d'une 
	 *					  ligne
	 * ATTR_POLYGONE	: lecture des attributs des polygônes du maillage
	 *					  ou d'une surface
	 * ATTR_POLYEDRE	: lecture des attributs des polyèdres du maillage
	 *					  ou d'un volume
	 * ATTR_NUAGE		: lecture des attributs des nuages
	 * ATTR_LIGNE		: lecture des attributs des lignes
	 * ATTR_SURFACE		: lecture des attributs des surfaces
	 * ATTR_VOLUME		: lecture des attributs des volumes
	 */
	enum Type { NOEUD, BRAS, POLYGONE, POLYEDRE, GROUPE, NUAGE, LIGNE, SURFACE,
	            VOLUME, ATTR_MAILLAGE, ATTR_NOEUD, ATTR_BRAS, ATTR_POLYGONE, 
	            ATTR_POLYEDRE, ATTR_NUAGE, ATTR_LIGNE, ATTR_SURFACE, 
	            ATTR_VOLUME };

	/**
	 * Constructeur. RAS.
	 */
	ReaderCallback ( );

	/**
	 * Destructeur. RAS.
	 */
	virtual ~ReaderCallback ( );

	/**
	 * Invoqué au début d'une session de lecture d'un maillage.
	 * @param		Nom du fichier contenant le maillage
	 * @param		Numéro du maillage lu
	 */
	virtual void starting (const IN_STD string& fileName, size_type num);

	/**
	 * Invoqué à la fin d'une session de lecture d'un maillage.
	 * @param		Nom du fichier contenant le maillage
	 * @param		Numéro du maillage lu
	 */
	virtual void atCompletion (const IN_STD string& fileName, size_type num);

	/**
	 * Invoqué lors de la lecture des données du maillage.
	 * @param		Nom du fichier contenant le maillage
	 * @param		Numéro du maillage lu
	 * @param		Type de données lues
	 * @param		Nom du groupe de données
	 * @param		Pourcentage des données lues. Une valeur négative
	 *				signifie que cette valeur n'est pas renseignée.
	 * @param		En retour, la lecture doit s'arrêter si cette valeur vaut
	 *				true, et continuer si elle vaut false.
	 */
	virtual void dataRead (const IN_STD string& fileName, size_type num,
	                       Type type, const IN_STD string& nom,
	                       double pourcent, bool& arreter);


	private :

	/**
	 * Constructeur de copie. Interdit.
	 */
	ReaderCallback (const ReaderCallback&);

	/**
	 * Opérateur =. Interdit.
	 */
	ReaderCallback& operator = (const ReaderCallback&);
};	// class ReaderCallback


inline ReaderCallback::ReaderCallback ( )
{
}	// ReaderCallback::ReaderCallback


inline ReaderCallback::ReaderCallback (const ReaderCallback&)
{
}	// ReaderCallback::ReaderCallback


inline ReaderCallback& ReaderCallback::operator = (const ReaderCallback&)
{
	return *this;
}	// ReaderCallback::operator =


inline ReaderCallback::~ReaderCallback ( )
{
}	// ReaderCallback::~ReaderCallback


inline void ReaderCallback::starting (
						const IN_STD string& fileName, size_type num)
{
}	// ReaderCallback::starting


inline void ReaderCallback::atCompletion (
						const IN_STD string& fileName, size_type num)
{
}	// ReaderCallback::atCompletion


inline void ReaderCallback::dataRead (
	const IN_STD string& fileName, size_type num,
	ReaderCallback::Type type, const IN_STD string& nom, double pourcent, 
	bool& arreter)
{
}	// ReaderCallback::dataRead


#ifndef SWIG
END_NAMESPACE_LIMA
#endif


#endif	// _CALLBACK_H
