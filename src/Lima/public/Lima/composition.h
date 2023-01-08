#ifndef _COMPOSITION_H
#define _COMPOSITION_H

#include "config.h"
#include "size.h"


#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif


/** Le nombre de noeuds maximum d'un élément de maillage. */
#define MAX_NOEUDS		16

/** 
 * Décrit la composition d'un groupe de données.
 */
struct Composition
{
	/**
	 * Constructeur. Initialise renseigne a false et les autres champs à 0.
	 */
	Composition ( );

	/**
	 * Constructeur de copie. RAS.
	 */
	Composition (const Composition&);

	/**
	 * Opérateur =. RAS.
	 */
	Composition& operator = (const Composition&);

	/**
	 * Réinitialise les champs à 0.
	 */
	void reinitialise ( );	// v 6.38.0

	/**
	 * Si renseigne vaut false les informations contenues par l'instance ne
	 * sont pas a prendre en compte.
	 */
	bool			renseigne;

	/**
	 * Le nombre d'éléments du groupe.
	 */
	size_type		nb_elements;

	/** Contient le nombre d'éléments pour chaque type, un type 
	 * d'élement étant caractérisé par son nombre de noeuds. 
	 */
	size_type		detail [MAX_NOEUDS];
};	// struct Composition


#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif	// _COMPOSITION_H
