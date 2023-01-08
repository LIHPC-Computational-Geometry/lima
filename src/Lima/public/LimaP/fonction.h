#ifndef _FONCTION_H
#define _FONCTION_H

#include "config_it.h"

BEGIN_NAMESPACE_LIMA

class _NoeudInterne;

template <class T>
struct _les_noeuds
{
  typedef typename T::iterator_noeud iterator;
  typedef typename T::const_iterator_noeud const_iterator;
  typedef _NoeudInterne* value_type;
  typedef const _NoeudInterne* const_value_type;
    
  iterator premier(T* elem) const
    { return elem->premier_noeud(); };

  const_iterator premier(const T* elem) const
    { return elem->premier_noeud(); };

  iterator dernier(T* elem) const
    { return elem->dernier_noeud(); };

  const_iterator dernier(const T* elem) const
    { return elem->dernier_noeud(); };
};
  
class _BrasInterne;

template <class T>
struct _les_bras
{
  typedef typename T::iterator_bras iterator;
  typedef typename T::const_iterator_bras const_iterator;
  typedef _BrasInterne* value_type;
  typedef const _BrasInterne* const_value_type;
    
  iterator premier(T* elem) const
    { return elem->premier_bras(); };

  const_iterator premier(const T* elem) const
    { return elem->premier_bras(); };

  iterator dernier(T* elem) const
    { return elem->dernier_bras(); };

  const_iterator dernier(const T* elem) const
    { return elem->dernier_bras(); };
};

class _PolygoneInterne;

template <class T>
struct _les_polygones
{
  typedef typename T::iterator_polygone iterator;
  typedef typename T::const_iterator_polygone const_iterator;
  typedef _PolygoneInterne* value_type;
  typedef const _PolygoneInterne* const_value_type;
    
  iterator premier(T* elem) const
    { return elem->premier_polygone(); };

  const_iterator premier(const T* elem) const
    { return elem->premier_polygone(); };

  iterator dernier(T* elem) const
    { return elem->dernier_polygone(); };

  const_iterator dernier(const T* elem) const
    { return elem->dernier_polygone(); };
};
  
class _PolyedreInterne;

template <class T>
struct _les_polyedres
{
  typedef typename T::iterator_polyedre iterator;
  typedef typename T::const_iterator_polyedre const_iterator;
  typedef _PolyedreInterne* value_type;
  typedef const _PolyedreInterne* const_value_type;
    
  iterator premier(T* elem) const
    { return elem->premier_polyedre(); };

  const_iterator premier(const T* elem) const
    { return elem->premier_polyedre(); };

  iterator dernier(T* elem) const
    { return elem->dernier_polyedre(); };

  const_iterator dernier(const T* elem) const
    { return elem->dernier_polyedre(); };
};
 
template <class T>
struct _invalider_bras
{
  void operator () (T* elem) { elem->invalider_bras(); };
};

template <class T>
struct _invalider_polygones
{
  void operator () (T* elem) { elem->invalider_polygones(); };
};

template <class T>
struct _invalider_polyedres
{
  void operator () (T* elem) { elem->invalider_polyedres(); };
};

template <class T>
struct _extraire_noeuds
{
  void operator () (T* elem) { elem->extraire_noeuds(); };
};

template <class T>
struct _extraire_bras
{
  void operator () (T* elem) { elem->extraire_bras(); };
};

template <class T>
struct _extraire_polygones
{
  void operator () (T* elem) { elem->extraire_polygones(); };
};

template <class T>
struct _extraire_contour_
{
  void operator () (T* elem) { elem->extraire_contour(); };
};

template <class T>
struct _extraire_noeuds_contour
{
  void operator () (T* elem) { elem->contour()->extraire_noeuds(); };
};

template <class T>
struct _extraire_bras_contour
{
  void operator () (T* elem) { elem->contour()->extraire_bras(); };
};

template <class T>
struct _structurer
{
  void operator () (T* elem) { elem->structurer(); };
};

template <class T>
struct _structurer_contour
{
  void operator () (T* elem) { elem->contour()->structurer(); };
};

//! Structure dédiée à l'orientation de contours d'éléments.
template <class T> struct _orienter_contour
{
/*! Oriente le contour de l'élément par retournement si sa méthode
    calcul_sens_rotation retourne une valeur négative.
    Précondition : l'élément passé en argument ne doit pas être
    nul.
*/
  void operator ( ) (T* elem) 
  { 
     if (0 > elem->contour ( )->calcul_sens_rotation ( ))
        elem->contour()->retourner ( ); 
  };
};

END_NAMESPACE_LIMA

#endif
