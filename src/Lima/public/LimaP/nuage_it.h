#ifndef _NUAGE_IT_H
#define _NUAGE_IT_H

#include "config_it.h"
#include "groupe.h"
#include "liste.h"
#include <Lima/liste_attribut.h>
#include "noeud_it.h"


BEGIN_NAMESPACE_LIMA

//! Nuage : ensemble non triés de noeuds.
class _NuageInterne : public _Groupe
{
public:
  typedef Liste<_NoeudInterne>::iterator iterator_noeud;
  typedef Liste<_NoeudInterne>::const_iterator const_iterator_noeud;

  //! Utile pour les messages d'erreur.
  static const char* nom_class() { return "NUAGE"; }

  //! Constructeur par défaut d'un nuage de nom str.
  _NuageInterne(const IN_STD string& str);
    
  /*!
    \brief Nombre d'éléments du groupe (nombre de noeuds)
  */
  size_type       nb_elm() const;

  //! Nombre de noeud.
  size_type             nb_noeuds() const;

  //! Noeud numéro n.
  _NoeudInterne*        noeud(size_type n) const;
  _NoeudInterne*        elm(size_type n) const;

  //! Noeud d'Id n.
  _NoeudInterne*        noeud_id(size_type n) const;

  //! Reserve la place pour les noeuds.
  void                  reserver(size_type n);

  //! Ajoute un noeud.
  void    ajouter(_NoeudInterne* nd);
  //! Retire un noeud.
  void    retirer(_NoeudInterne* nd);
  
  //! Vérifie l'appartenance du noeud a l'ensemble.
  bool    contenir(_NoeudInterne* nd) const;

  //! Iterator premier noeud.
  iterator_noeud        premier_noeud() 
    { return m_noeuds.premier(); }
  const_iterator_noeud  premier_noeud() const 
    { return m_noeuds.premier(); }

  //! Iterator dernier noeud.
  iterator_noeud        dernier_noeud() 
    { return m_noeuds.dernier(); }
  const_iterator_noeud  dernier_noeud() const 
    { return m_noeuds.dernier(); }

  //! Liste des attributs attachés aux noeuds du nuage
  Liste_attribut&    att_noeuds();

  //! Initialisation des attributs aux noeuds du nuage
  void               init_att_noeuds();
  //! Initialisation de tous les attributs du nuage
  void               init_att();

private:
  Liste<_NoeudInterne>       m_noeuds;

  // Liste des attributs attaches aux noeuds du nuage
  Liste_attribut             m_liste_att_noeuds;
};
  

END_NAMESPACE_LIMA

#endif
