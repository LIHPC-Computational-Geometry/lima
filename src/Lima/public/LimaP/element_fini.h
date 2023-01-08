#ifndef _ELEMENT_FINI_H
#define _ELEMENT_FINI_H

#include "element_maillage.h"
#include <Lima/size.h>

BEGIN_NAMESPACE_LIMA
  
//! Classe de base des éléments finis du maillage.
/*!
  A chaque élément fini est associé un numéro d'identification. La
  numérotation n'a pas besoin d'être continue.
*/
class _ElementFini : public _ElementMaillage
{
public:
  typedef size_type id_type;

  //! Un élément fini de numéro n.
  _ElementFini(size_type n=0);
    
  //! Le numéro d'identification de l'élément fini. 
  size_type                 id() const;  
    
  //! Affecte le numéro n à l'identificateur.
  void                      set_id(size_type n = 0);

  //! Fonction de comparaison d'éléments finis pour les algos de recherche
  static bool comparer_id(const _ElementFini* e1, 
			  const _ElementFini* e2);
  
  //! Classe functor de recherche d'un element par sont id.
  class chercher
  {
  private:
    //! L'id de l'element.
    size_type m_id;
    
  public:
    //! Initialisation à partir du nom du groupe.
    chercher(const size_type& id): m_id(id) {;};

    //! Fonction de comparaison.
    bool operator () (const _ElementFini* e);
  };

  //! Nombre de noeuds.
  virtual size_type             nb_noeuds() const;

private:
  //! L'identificateur.
  size_type                 m_id;
};
 
inline
_ElementFini::_ElementFini(size_type n):
  m_id(n)
{
}

inline
size_type _ElementFini::id() const
{
  return m_id;
}

inline
bool _ElementFini::chercher::operator () (const _ElementFini* e) 
{
  return  e->id()==m_id;
};

inline
void _ElementFini::set_id(size_type n)
{
  m_id = n;
}

inline
bool _ElementFini::comparer_id(const _ElementFini* e1, const _ElementFini* e2)
{
  return e1->id() < e2->id();
}

inline
size_type _ElementFini::nb_noeuds()const {return 0;}


END_NAMESPACE_LIMA

#endif
