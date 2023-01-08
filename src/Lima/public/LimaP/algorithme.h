#ifndef _ALGORITHME_H
#define _ALGORITHME_H

#include "config_it.h"
#include <Lima/size.h>

#if defined(__IBMCPP__)
#pragma implementation ("algorithme_t.h")
#endif

BEGIN_NAMESPACE_LIMA

class _MaillageInterne;

template <class Iter, class Func, class Cont> void
extraire_elements(Iter debut, Iter fin, Func extracteur, Cont& les_elements);

template <class Iter, class Func, class Grp> void
_extraire_contour(Iter debut, Iter fin, Func extracteur, Grp le_contour);

template <class Iter, class Func> void
connexions_inverses(Iter debut, Iter fin, Func extracteur);

template <class Iter, class E> void
orienter(Iter debut, Iter fin, double (E::*encombrement)() const);

template <class Iter> void
ajouter_bras(_MaillageInterne* mai, Iter debut, Iter fin, size_type ind);

template <class Iter> void
ajouter_polygone(_MaillageInterne* mai, Iter debut, Iter fin, size_type ind);

template <class Iter> void
ajouter_polyedre(_MaillageInterne* mai, Iter debut, Iter fin, size_type ind);

template <class Iter, class Func> size_type
nombre_connexions(Iter debut, Iter fin, Func extracteur);

template <class Int, class Iter, class Func> void
lire_connexions(Int* pointage, Int* liste, Iter debut, Iter fin, Func extracteur);

template <class Int, class Iter> void
lire_elements(Int* liste, Iter debut, Iter fin);

template <class Cont, class Elem>
Elem* element_oppose_(const Cont& les_elements, const Elem* target);

template <class Cont, class Elem>
Elem* element_suivant(const Cont& les_elements, const Elem* target);

template <class Cont, class Elem>
Elem* element_precedent(const Cont& les_elements, const Elem* target);

template <class T>
inline bool impaire(T a)
{
  return (a) & 1;
}

template <class T>
inline bool lm_paire(T a)
{
  return impaire(a);
}

END_NAMESPACE_LIMA

#if defined(_IBMR2) || defined(__GNUC__) || defined(CRAY) || defined(INCLUDE_TEMPLATE)
#include "algorithme_t.h"
#endif

#endif
