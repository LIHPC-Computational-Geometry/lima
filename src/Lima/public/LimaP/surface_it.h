#ifndef _SURFACE_IT_H
#define _SURFACE_IT_H

#include "config_it.h"
#include "groupe.h"
#include "noeud_it.h"
#include "bras_it.h"
#include "polygone_it.h"
#include "liste.h"
#include <Lima/liste_attribut.h>

BEGIN_NAMESPACE_LIMA


class _LigneInterne;


//! Surface : ensemble non triés de polygones.
class _SurfaceInterne : public _Groupe
{
public:
  typedef Liste<_NoeudInterne>::iterator iterator_noeud;
  typedef Liste<_NoeudInterne>::const_iterator const_iterator_noeud;
  typedef Liste<_BrasInterne>::iterator iterator_bras;
  typedef Liste<_BrasInterne>::const_iterator const_iterator_bras;
  typedef Liste<_PolygoneInterne>::iterator iterator_polygone;
  typedef Liste<_PolygoneInterne>::const_iterator const_iterator_polygone;

  //! Utile pour les messages d'erreur.
  static const char* nom_class() { return "SURFACE"; }

  //! Constructeur par défaut d'une surface de nom str.
  _SurfaceInterne(const IN_STD string& str);
    
  //! Destructeur.
  ~_SurfaceInterne();

  /*!
    \brief Nombre d'éléments du groupe (nombre de polygones)
  */
  size_type       nb_elm() const;

  //! Nombre de noeud.
  /*
    Accessible seulement si le calcul des noeuds des surfaces a été
    demandé au maillage.
  */
  size_type             nb_noeuds() const;
  //! Nombre de bras.
  /*
    Accessible seulement si le calcul des bras des surfaces a été
    demandé au maillage.
  */
  size_type             nb_bras() const;
  //! Nombre de polygones.
  size_type             nb_polygones() const;
  //! Nombre de noeud de la grille.
  /*
    Accessible seulement si le calcul de la structuration a été
    demandé au maillage. Peut être différent du nombre de noeuds de
    la surface si il y a des pôles.
  */
  size_type             nb_noeuds_grille() const;

  //! Noeud numéro n.
  _NoeudInterne*        noeud(size_type n) const;
  //! Bras numéro n.
  _BrasInterne*         bras(size_type n) const;
  //! Polygone numéro n.
  _PolygoneInterne*     polygone(size_type n) const;

  //! Elément (Polygone) numéro n du groupe
  _PolygoneInterne*      elm(size_type n) const;

  //! Noeud grille numéro n.
  _NoeudInterne*        noeud_grille(size_type n) const;

  //! Noeud d'Id n.
  _NoeudInterne*        noeud_id(size_type n) const;
  //! Bras d'Id n.
  _BrasInterne*         bras_id(size_type n) const;
  //! Polygone d'Id n.
  _PolygoneInterne*     polygone_id(size_type n) const;

  //! Reserve la place pour les noeuds.
  void                  reserver_noeuds(size_type n);
  //! Reserve la place pour les bras.
  void                  reserver_bras(size_type n);
  //! Reserve la place pour les polygones.
  void                  reserver(size_type n);

  //! Ajoute un polygone.
  void    ajouter(_PolygoneInterne* pg);
  //! Retire un polygone.
  void    retirer(_PolygoneInterne* pg);
  
  //! Vérifie l'appartenance du polygone a l'ensemble.
  bool    contenir(_PolygoneInterne* pg) const;
  //! Vérifie l'appartenance du bras a l'ensemble.
  bool    contenir(_BrasInterne* br) const;
  //! Vérifie l'appartenance du noeud a l'ensemble.
  bool    contenir(_NoeudInterne* nd) const;

  //! Calcul des noeuds.
  void    extraire_noeuds();
  //! Calcul des bras.
  void    extraire_bras();
  //! Calcul du contour.
  void    extraire_contour();
    
  size_t          longueur_i() const;
  size_t          longueur_j() const;
  void            structurer();

  //! Le contour.
  _LigneInterne* contour() const;

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

  //! Iterator premier polygone.
  iterator_polygone     premier_polygone() 
    { return m_polygones.premier(); }
  const_iterator_polygone premier_polygone() const 
    { return m_polygones.premier(); }

  //! Iterator dernier polygone.
  iterator_polygone     dernier_polygone() 
    { return m_polygones.dernier(); }
  const_iterator_polygone dernier_polygone() const 
    { return m_polygones.dernier(); }

  //! Liste des attributs attachés aux noeuds de la surface
  Liste_attribut&    att_noeuds();
  //! Liste des attributs attachés aux bras de la surface
  Liste_attribut&    att_bras();
  //! Liste des attributs attachés aux polygones de la surface
  Liste_attribut&    att_polygones();

  //! Initialisation des attributs aux noeuds de la surface
  void               init_att_noeuds();
  //! Initialisation des attributs aux bras de la surface
  void               init_att_bras();
  //! Initialisation des attributs aux polygones de la surface
  void               init_att_polygones();
  //! Initialisation de tous les attributs de la surface
  void               init_att();

private:
  void chainer(Liste<_PolygoneInterne>& les_polygones, 
	       Liste<_BrasInterne>& les_bras);

  _LigneInterne* m_contour;

  Liste<_NoeudInterne>       m_noeuds;
  Liste<_BrasInterne>        m_bras;
  Liste<_PolygoneInterne>    m_polygones;
  size_type          m_i;
  size_type          m_j;
  Liste<_NoeudInterne>       m_grille;

  // Liste des attributs attaches aux noeuds de la surface
  Liste_attribut             m_liste_att_noeuds;
  // Liste des attributs attaches aux bras de la surface
  Liste_attribut             m_liste_att_bras;
  // Liste des attributs attaches aux polygones de la surface
  Liste_attribut             m_liste_att_polygones;

};
  
END_NAMESPACE_LIMA

#endif
