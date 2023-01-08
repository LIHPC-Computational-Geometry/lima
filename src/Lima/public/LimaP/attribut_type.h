#ifndef _ATTRIBUT_TYPE_H
#define _ATTRIBUT_TYPE_H

#include "attribut_it.h"
#include <Lima/size.h>
#include <Lima/erreur.h>
#include <Lima/non_defini.h>


#if defined(__IBMCPP__)
#pragma implementation ("attribut_type_t.h")
#endif


BEGIN_NAMESPACE_LIMA

template <class T> class _AttributInterne: public _Attribut
{
public:

  //! Constructeur
  _AttributInterne(const IN_STD string& nom);
  //! Destructeur
  ~_AttributInterne() { }

  //! Nom de la classe typee
  Attribut::TYPE type() const;

  //! Affectation d'une valeur constante a l'attribut
  void                set_val(const T& val);
  //! Affectation d'une valeur pour un element de l'attribut
  void                set_val(size_type indice,const T& val);
  //! Affectation d'un tableau de valeurs a l'attribut
  void                set_val(const IN_STD vector<T>& tab_val);
  //! Lecture de la valeur constante d'un attribut
  void                get_val(T& val);
  //! Lecture d'une valeur de l'attribut
  void                get_val(size_type indice,T& val);
  //! Lecture de l'ensemble des valeurs d'un attribut
  void                get_val(IN_STD vector<T>& tab_val);

  //! Retrait de l'element d'indice i
  void                retirer(size_type i);
  //! Ajout d'un element de valeur non definie a l'attribut
  void                augmenter();

  //! Nombre d'elements de l'attribut
  size_type           nb_elements() const;
  _Attribut*          copy() const;
  //! Valeur d'un element non defini de l'attribut
  T                   val_non_def() const;

private:

  _AttributInterne (const _AttributInterne& a)
	: _Attribut (a)
  { }
  _AttributInterne& operator = (const _AttributInterne&)
  { return *this; };

  IN_STD vector<T> m_tab;
};

END_NAMESPACE_LIMA


#if defined(_IBMR2) || defined(__GNUC__) || defined(CRAY) || defined(INCLUDE_TEMPLATE)
#include "attribut_type_t.h"
#endif


#endif
