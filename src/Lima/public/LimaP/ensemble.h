#ifndef _ENSEMBLE_H
#define _ENSEMBLE_H

#include <limits.h>
#include "config_it.h"
#include <Lima/size.h>
#include <Lima/erreur.h>

#if defined(__IBMCPP__)
#pragma implementation ("ensemble_t.h")
#endif

BEGIN_NAMESPACE_LIMA


//! Ensemble triés d'éléments finis.
/*!
  Liste d'éléments finis.
*/

template<class T> class Ensemble : private IN_STD vector<T*>
{
public:
  typedef typename IN_STD vector<T*>::iterator iterator;
  typedef typename IN_STD vector<T*>::const_iterator const_iterator;

  //! Constructeur par défaut.
  Ensemble();

  //! Nombre d'éléments.
//#if defined(linux)
//  Lima::size_type       nb_elements() const;
//#else
  size_type             nb_elements() const;
//#endif

  //! Élément numéro n.
  T*                    element(size_type n) const;

  //! Élément par son ID.
  /*!
    L'ID est un int pour les éléments finis. La recherche est en logN.
  */
  T*                    element_id(const typename T::id_type& id) const;

  //! Vérifie l'appartenance a la liste.
  /*!
    La recherche est en logN.
  */
  bool                  contenir(T* e) const;

  //! Ajoute un élément.
//  void                  ajouter(T* e);	// v 6.6.0
  bool                  ajouter(T* e);		// v 6.6.0

  //! Retire l'élément
//  void                  retirer(T* e);	// v 6.6.0
  bool                  retirer (T* e);		// v 6.6.0

  //! Vide l'ensemble.
  void                  vider();

  //! Reserve la place pour n éléments.
  void                  reserver(size_type n);

  //! Iterator premier élément.
  iterator              premier() { return this->begin(); }
  const_iterator        premier() const { return this->begin(); }

  //! Iterator dernier élément.
  iterator              dernier() { return this->end(); }
  const_iterator        dernier() const { return this->end(); }

  //! Retourne la position (indice dans l'ensemble) d'un élément.
  size_type             indice(size_type id);

private:
  //! Vérifie si la numérotation est compacte.
  bool                  compacte() const;
  //! Retourne la position (itérateur sur la position) d'un élément.
  iterator              position(size_type id);
  const_iterator        position(size_type id) const;

private:
  size_type             m_min_id;
  size_type             m_max_id;
};

template<class T> inline
Ensemble<T>::Ensemble():
  IN_STD vector<T*>(), m_min_id(ULONG_MAX), m_max_id(0)
{
}

template<class T> inline
size_type Ensemble<T>::nb_elements() const
{
  return this->size();
}

template<class T> inline
T* Ensemble<T>::element(size_type n) const
{
  return this->operator[](n);
}

template<class T> inline
void Ensemble<T>::vider()
{
  this->resize(0);
}

template<class T> inline
bool Ensemble<T>::compacte() const
{
  return this->size() == (m_max_id-m_min_id+1);
}

template<class T> inline
void Ensemble<T>::reserver(size_type n)
{
  this->reserve(n);
}

END_NAMESPACE_LIMA

#if defined(_IBMR2) || defined(__GNUC__) || defined(CRAY) || defined(INCLUDE_TEMPLATE)
#include "ensemble_t.h"
#endif

#endif
