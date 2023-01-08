#ifndef _VOLUME_IT_H
#define _VOLUME_IT_H

#include "config_it.h"
#include "groupe.h"
#include "liste.h"
#include "noeud_it.h"
#include "bras_it.h"
#include "polygone_it.h"
#include "polyedre_it.h"

#include <Lima/liste_attribut.h>

BEGIN_NAMESPACE_LIMA


class _SurfaceInterne;


//! Volume : ensemble non triés de polygedres.
class _VolumeInterne : public _Groupe
{
public:
  typedef Liste<_NoeudInterne>::iterator iterator_noeud;
  typedef Liste<_NoeudInterne>::const_iterator const_iterator_noeud;
  typedef Liste<_BrasInterne>::iterator iterator_bras;
  typedef Liste<_BrasInterne>::const_iterator const_iterator_bras;
  typedef Liste<_PolygoneInterne>::iterator iterator_polygone;
  typedef Liste<_PolygoneInterne>::const_iterator const_iterator_polygone;
  typedef Liste<_PolyedreInterne>::iterator iterator_polyedre;
  typedef Liste<_PolyedreInterne>::const_iterator const_iterator_polyedre;

  //! Utile pour les messages d'erreur.
  static const char* nom_class() { return "VOLUME"; }

  //! Constructeur par défaut d'un volume.
  _VolumeInterne(const IN_STD string& str);
    
  //! Destructeur.
  ~_VolumeInterne();
  
  /*!
    \brief Nombre d'éléments du groupe (nombre de polyedres)
  */
  size_type       nb_elm() const;

   //! Nombre de noeud.
  /*
    Accessible seulement si le calcul des noeuds des volumes a été
    demandé au maillage.
  */
  size_type             nb_noeuds() const;
  //! Nombre de bras.
  /*
    Accessible seulement si le calcul des bras des volumes a été
    demandé au maillage.
  */
  size_type             nb_bras() const;
  //! Nombre de polygones.
  /*
    Accessible seulement si le calcul des polygones des volumes a été
    demandé au maillage.
  */
  size_type             nb_polygones() const;
  //! Nombre de polyèdres.
  size_type             nb_polyedres() const;

  //! Noeud numéro n.
  _NoeudInterne*        noeud(size_type n) const;
  //! Bras numéro n.
  _BrasInterne*         bras(size_type n) const;
  //! Polygone numéro n.
  _PolygoneInterne*     polygone(size_type n) const;
  //! Polyèdre numéro n.
  _PolyedreInterne*     polyedre(size_type n) const;
  //! Elèment (Polyèdre) numéro n.
  _PolyedreInterne*     elm(size_type n) const;
  //! Noeud d'Id n.
  _NoeudInterne*        noeud_id(size_type n) const;
  //! Bras d'Id n.
  _BrasInterne*         bras_id(size_type n) const;
  //! Polygone d'Id n.
  _PolygoneInterne*     polygone_id(size_type n) const;
  //! Polyèdre d'Id n.
  _PolyedreInterne*     polyedre_id(size_type n) const;

  //! Reserve la place pour les noeuds.
  void                  reserver_noeuds(size_type n);
  //! Reserve la place pour les bras.
  void                  reserver_bras(size_type n);
  //! Reserve la place pour les polygones.
  void                  reserver_polygones(size_type n);
  //! Reserve la place pour les polyèdres.
  void                  reserver(size_type n);

  //! Ajoute un polyèdre.
  void    ajouter(_PolyedreInterne* pd);
  //! Retire un polyèdre.
  void    retirer(_PolyedreInterne* pd);
  
  //! Vérifie l'appartenance du polyèdre a l'ensemble.
  bool    contenir(_PolyedreInterne* pd) const;
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
  //! Calcul des polygones.
  void    extraire_polygones();
  //! Calcul du contour.
  void    extraire_contour();
    
  //! Le contour.
  _SurfaceInterne* contour() const;

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

  //! Iterator premier polyèdre.
  iterator_polyedre     premier_polyedre() 
    { return m_polyedres.premier(); }
  const_iterator_polyedre premier_polyedre() const 
    { return m_polyedres.premier(); }

  //! Iterator dernier polyèdre.
  iterator_polyedre     dernier_polyedre() 
    { return m_polyedres.dernier(); }
  const_iterator_polyedre dernier_polyedre() const 
    { return m_polyedres.dernier(); }

  //! Liste des attributs attachés aux noeuds du volume
  Liste_attribut&    att_noeuds();
  //! Liste des attributs attachés aux bras du volume
  Liste_attribut&    att_bras();
  //! Liste des attributs attachés aux polygones du volume
  Liste_attribut&    att_polygones();
  //! Liste des attributs attachés aux polyèdres du volume
  Liste_attribut&    att_polyedres();

  //! Initialisation des attributs aux noeuds du volume
  void               init_att_noeuds();
  //! Initialisation des attributs aux bras du volume
  void               init_att_bras();
  //! Initialisation des attributs aux polygones du volume
  void               init_att_polygones();
  //! Initialisation des attributs aux polyedres du volume
  void               init_att_polyedres();
  //! Initialisation de tous les attributs du volume
  void               init_att();

private:
  _SurfaceInterne* m_contour;

  Liste<_NoeudInterne>       m_noeuds;
  Liste<_BrasInterne>        m_bras;
  Liste<_PolygoneInterne>    m_polygones;
  Liste<_PolyedreInterne>    m_polyedres;

  // Liste des attributs attaches aux noeuds du volume
  Liste_attribut             m_liste_att_noeuds;
  // Liste des attributs attaches aux bras du volume
  Liste_attribut             m_liste_att_bras;
  // Liste des attributs attaches aux polygones du volume
  Liste_attribut             m_liste_att_polygones;
  // Liste des attributs attaches aux polyedres du volume
  Liste_attribut             m_liste_att_polyedres;
};
  


END_NAMESPACE_LIMA

#endif
