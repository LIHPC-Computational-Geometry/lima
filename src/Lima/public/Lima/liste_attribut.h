#ifndef _LISTE_ATTRIBUT_H
#define _LISTE_ATTRIBUT_H

#include "config.h"
#include "size.h"
#include "erreur.h"
#include "attribut.h"

#if defined(__IBMCPP__)
#pragma implementation ("liste_attribut.hpp")
#endif


#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

#ifndef SWIG
typedef IN_STD vector<Attribut> vector_Attribut;
#endif

//! Liste d'attributs.
/*!
  Liste des attributs (pouvant etre heterogenes) pour un point
  d'attache donne.
*/
class Liste_attribut : private vector_Attribut
{
  public:
#ifndef SWIG
  typedef IN_STD vector<Attribut>::iterator iterator;
  typedef IN_STD vector<Attribut>::const_iterator const_iterator;
#endif

  //! Constructeur par défaut.
  Liste_attribut();

  //! Destructeur.
  ~Liste_attribut();

  //! Nombre d'éléments (attributs attaches).
  size_type             nb_elements() const;

  //! Élément numéro n.
  Attribut              element(size_type n) const;

  //! Vérifie l'appartenance a la liste.
  /*!
    La recherche est en N.
  */
  bool                  contenir(const Attribut& a) const;
  bool                  contenir(const IN_STD string& s) const;

  //! Ajoute un élément.
  void                  attacher(const Attribut& a);

  //! Retire et efface l'attribut de la liste.
  void                  retirer(const Attribut& a);

  //! Retire et efface de la liste l'attribut nomme 's'.
  void                  retirer(const IN_STD string& s);

  //! Retire pour chaque attribut de la liste la valeur d'indice i
  void                  retirer(size_type i);

  //! Augmente de 1 le nombre de valeurs de chaque attribut de la liste
  void                  augmenter();

  //! Detache un element de la liste.
#ifndef SWIG
  Attribut              detacher(const IN_STD string& s);
#else
  Attribut              detacher(const char* s);
#endif	// SWIG

  //! Retourne l'element de la liste nomme 's'
#ifndef SWIG
  Attribut              element(const IN_STD string& s);
#else
  Attribut              element(const char* s);
#endif	// SWIG

  //! Vide la liste.
  void                  vider();

#ifndef SWIG
  //! Liste des noms des attributs.
  IN_STD vector<IN_STD string>        liste() const;

  //! Iterator premier élément.
  iterator              premier() { return begin(); }
  const_iterator        premier() const { return begin(); }

  //! Iterator dernier élément.
  iterator              dernier() { return end(); }
  const_iterator        dernier() const { return end(); }

  Attribut            front() const
  { return IN_STD vector<Attribut>::front(); };
  Attribut            back() const { return IN_STD vector<Attribut>::back(); };
  void                push_back(const Attribut& x)
  { IN_STD vector<Attribut>::push_back(x); };

  //! Mise a jour de m_max_val
  void                  set_max_val(size_type n);
  size_type             max_val();

private:
  //! Nombre de valeurs maximales d'un attribut de la liste
  //  (correspond au nombre d'entites auxquelles se refere la liste)
  size_type             m_max_val;
#endif
};

#ifndef SWIG

inline
void Liste_attribut::set_max_val(size_type n)
{
  m_max_val=n;
}

inline
Liste_attribut::Liste_attribut():
  IN_STD vector<Attribut>()
{
  set_max_val(0);
}

inline
Liste_attribut::~Liste_attribut()
{
  clear();
}

inline
size_type Liste_attribut::nb_elements() const
{
  return size();
}

inline
Attribut Liste_attribut::element(size_type n) const
{
  return operator[](n);
}

inline
size_type Liste_attribut::max_val()
{
  return m_max_val;
}

END_NAMESPACE_LIMA
#endif

#if defined(_IBMR2) || defined(__GNUC__)
//#include "liste_attribut.hpp"
#endif

#endif
