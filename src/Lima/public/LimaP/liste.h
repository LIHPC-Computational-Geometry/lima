#ifndef _LISTE_H
#define _LISTE_H

#include "config_it.h"
#include <Lima/size.h>
#include <Lima/erreur.h>

#if defined(__IBMCPP__)
#pragma implementation ("liste_t.h")
#endif


BEGIN_NAMESPACE_LIMA


//! Liste d'éléments non-triés.
/*!
  Liste de groupes ou d'éléments finis.
*/
template<class T> class Liste : private IN_STD vector<T*>
{
public:
  typedef typename IN_STD vector<T*>::iterator iterator;
  typedef typename IN_STD vector<T*>::const_iterator const_iterator;

  //! Constructeur par défaut.
  Liste();

  //! Nombre d'éléments.
//#if defined(linux)
//  Lima::size_type       nb_elements() const;
//#else
  size_type             nb_elements() const;
//#endif
  //! Élément numéro n.
  T*                    element(size_type n) const;

  //! Élément numéro n.
  T*&                   element(size_type n);

  //! Élément par son ID.
  /*!
    L'ID est soit un int pour les éléments finis, soit un string pour
    les groupes. La recherche est en N.
  */
  T*                    element_id(const typename T::id_type& id) const;

  //! Vérifie l'appartenance a la liste.
  /*!
    La recherche est en N.
  */
  bool                  contenir(T* e) const;

  //! Ajoute un élément.
  void                  ajouter(T* e);

  //! Retire l'élément
  void                  retirer(T* e);

  //! Position de l'élément dans la liste
  size_type             indice(T* e);

  //! Vide la liste.
  void                  vider();

  //! Reserve la place pour n éléments.
  void                  reserver(size_type n);

  //! Redimensionne pour n éléments.
  void                  redimensionner(size_type n);

  //! Iterator premier élément.
  iterator              premier() { return this->begin(); }
  const_iterator        premier() const { return this->begin(); }

  //! Iterator dernier élément.
  iterator              dernier() { return this->end(); }
  const_iterator        dernier() const { return this->end(); }

  T*&                   front() { return IN_STD vector<T*>::front(); };
  T*&                   back() { return IN_STD vector<T*>::back(); };
  T*                    front() const { return IN_STD vector<T*>::front(); };
  T*                    back() const { return IN_STD vector<T*>::back(); };
  void                  push_back(T* x) { IN_STD vector<T*>::push_back(x); };
};

template<class T> inline
Liste<T>::Liste():
  IN_STD vector<T*>()
{
}

template<class T> inline
size_type Liste<T>::nb_elements() const
{
  return this->size();
}

template<class T> inline
T* Liste<T>::element(size_type n) const
{
  return this->operator[](n);
}

template<class T> inline
T*& Liste<T>::element(size_type n)
{
  return this->operator[](n);
}

template<class T> inline
void Liste<T>::vider()
{
  this->resize(0);
}

template<class T> inline
void Liste<T>::reserver(size_type n)
{
  this->reserve(n);
}

template<class T> inline
void Liste<T>::redimensionner(size_type n)
{
  this->resize(n, NULL);
}

END_NAMESPACE_LIMA

#if defined(_IBMR2) || defined(__GNUC__) || defined(CRAY) || defined(INCLUDE_TEMPLATE)
#include "liste_t.h"
#endif

#endif
