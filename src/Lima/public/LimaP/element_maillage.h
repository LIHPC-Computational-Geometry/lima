#ifndef _ELEMENT_MAILLAGE_H
#define _ELEMENT_MAILLAGE_H

#include "config_it.h"
#include "reference.h"

BEGIN_NAMESPACE_LIMA

class _MaillageInterne;
  
//! Classe de base des entités du maillage.
/*!
  Les éléments d'un maillage (éléments finis ou groupes) sont attachés
  à un maillage et peuvent référencer des données utilisateurs.
 */
class _ElementMaillage : public _ObjetReference
{
public:
  //! Un élément maillage.
  _ElementMaillage();
    
  //! Maillage auquel est attache l'élément.
  const _MaillageInterne*   maillage() const;

  //! Rattache l'élément au maillage.
  void                      attacher(const  _MaillageInterne* mai);

  //! Détache l'élément du maillage.
  void                      detacher();

  //! Lie une donnée utilisateur à l'élément.
  void                      set_udata(void* data);
  //! Retourne la donnée utilisateur.
  void*                     udata() const;

private:
  //! Lien sur le maillage.
  const _MaillageInterne*   m_mai;
  //! Lien sur les données membres.
  void*                     m_udata;
};
 
inline
_ElementMaillage::_ElementMaillage():
  m_mai(0), m_udata(0)
{
}

inline
const _MaillageInterne* _ElementMaillage::maillage() const
{
  return m_mai;
}

inline
void _ElementMaillage::attacher(const _MaillageInterne* mai)
{
  m_mai = mai;
}

inline
void _ElementMaillage::detacher()
{
  m_mai = 0;
}

inline
void _ElementMaillage::set_udata(void* data)
{
  m_udata = data;
}

inline
void* _ElementMaillage::udata() const
{
  return m_udata;
}


END_NAMESPACE_LIMA

#endif
