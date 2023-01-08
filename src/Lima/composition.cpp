#include <Lima/composition.h>
#include "LimaP/using_it.h"


BEGIN_NAMESPACE_LIMA


Composition::Composition ( )
	: renseigne (false), nb_elements (0)
{
	for (size_type i = 0; i < MAX_NOEUDS; i++)
		detail [i]	= 0;
}	// Composition::Composition


Composition::Composition (const Composition& composition)
	: renseigne (composition.renseigne), nb_elements (composition.nb_elements)
{
	for (size_type i = 0; i < MAX_NOEUDS; i++)
		detail [i]	= composition.detail [i];
}	// Composition::Composition


Composition& Composition::operator = (const Composition& composition)
{
	if (&composition != this)
	{
		renseigne	= composition.renseigne;
		nb_elements	= composition.nb_elements;
		for (size_type i = 0; i < MAX_NOEUDS; i++)
			detail [i]	= composition.detail [i];
	}	// if (&composition != this)

	return *this;
}	// Composition::operator =


void Composition::reinitialise ( )	// v 6.38.0
{
	renseigne	= false;
	nb_elements	= 0;
	for (size_type i = 0; i < MAX_NOEUDS; i++)
		detail [i]	= 0;
}	// Composition::reinitialise


END_NAMESPACE_LIMA
