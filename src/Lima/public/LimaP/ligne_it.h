#ifndef _LIGNE_IT_H
#define _LIGNE_IT_H

#include "config_it.h"
#include "groupe.h"
#include "liste.h"
#include "bras_it.h"
#include "noeud_it.h"

#include <Lima/liste_attribut.h>

BEGIN_NAMESPACE_LIMA

class _NuageInterne;

//! Ligne : ensemble non triés de bras.
class _LigneInterne : public _Groupe
{
public:
  typedef Liste<_NoeudInterne>::iterator iterator_noeud;
  typedef Liste<_NoeudInterne>::const_iterator const_iterator_noeud;
  typedef Liste<_BrasInterne>::iterator iterator_bras;
  typedef Liste<_BrasInterne>::const_iterator const_iterator_bras;

  //! Utile pour les messages d'erreur.
  static const char* nom_class() { return "LIGNE"; }

  //! Constructeur par défaut d'une ligne de nom str.
  _LigneInterne(const IN_STD string& str);
    
  //! Destructeur.
  ~_LigneInterne();

  /*!
    \brief Nombre d'éléments du groupe (nombre de bras)
  */
  size_type       nb_elm() const;
  /*!
    \brief Elément numéro n du groupe
  */
  _BrasInterne*      elm(size_type n) const;

  //! Nombre de noeud.
  /*
    Accessible seulement si le calcul des noeuds des lignes a été
    demandé au maillage.
  */
  size_type             nb_noeuds() const;
  //! Nombre de bras.
  size_type             nb_bras() const;

  //! Noeud numéro n.
  _NoeudInterne*        noeud(size_type n) const;
  //! Bras numéro n.
  _BrasInterne*         bras(size_type n) const;

  //! Noeud d'Id n.
  _NoeudInterne*        noeud_id(size_type n) const;
  //! Bras d'Id n.
  _BrasInterne*         bras_id(size_type n) const;

  //! Reserve la place pour les noeuds.
  void                  reserver_noeuds(size_type n);
  //! Reserve la place pour les bras.
  void                  reserver(size_type n);

  //! Ajoute un bras.
  void    ajouter(_BrasInterne* br);
  //! Retire un bras.
  void    retirer(_BrasInterne* br);
  
  //! Vérifie l'appartenance du bras a l'ensemble.
  bool    contenir(_BrasInterne* br) const;
  //! Vérifie l'appartenance du noeud a l'ensemble.
  bool    contenir(_NoeudInterne* nd) const;

  //! Calcul des noeuds.
  void    extraire_noeuds();
  //! Calcul du contour.
  void    extraire_contour();
    
  //! Le contour.
  /*
    Zéro noeuds si contour fermé, 2 si un seul morceaux plus si
    plusieurs morceaux. 
  */
  _NuageInterne* contour() const;

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

  //! Iterator premier bras.
  iterator_bras         premier_bras() 
    { return m_bras.premier(); }
  const_iterator_bras   premier_bras() const 
    { return m_bras.premier(); }

  //! Iterator dernier bras.
  iterator_bras         dernier_bras() 
    { return m_bras.dernier(); }
  const_iterator_bras dernier_bras() const 
    { return m_bras.dernier(); }

  void            structurer();
  size_t          longueur_i() const;

  /*! Inversion de l'ordre des bras et de l'ordre des noeuds constituant la
  ligne. */
  void            retourner ( );

  /*! Retourne une valeur négative si le sens de rotation de la ligne est le
      sens horaire et une valeur positive si le sens de rotation est le sens 
      trigonométrique. 
      La valeur retournée est la surface englobée par la ligne par 
     developpement de la formule de Green. Cette surface n'a de sens qu'en cas 
     de ligne fermée. */
  double          calcul_sens_rotation ( ) const;

  //! Liste des attributs attachés aux noeuds de la ligne
  Liste_attribut&    att_noeuds();
  //! Liste des attributs attachés aux bras de la ligne
  Liste_attribut&    att_bras();

  //! Initialisation des attributs aux noeuds de la ligne
  void               init_att_noeuds();
  //! Initialisation des attributs aux bras de la ligne
  void               init_att_bras();
  //! Initialisation de tous les attributs de la ligne
  void               init_att();
 
private:
  static void chainer(Liste<_BrasInterne>& les_bras, 
		      Liste<_NoeudInterne>& les_noeuds);

  _NuageInterne* m_contour;

  Liste<_NoeudInterne>       m_noeuds;
  Liste<_BrasInterne>        m_bras;
  size_type          m_i;

  // Liste des attributs attaches aux noeuds de la ligne
  Liste_attribut             m_liste_att_noeuds;
  // Liste des attributs attaches aux bras de la ligne
  Liste_attribut             m_liste_att_bras;

};
  
END_NAMESPACE_LIMA

#endif
