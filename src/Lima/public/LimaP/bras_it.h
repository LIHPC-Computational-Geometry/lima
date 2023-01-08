#ifndef _BRAS_IT_H
#define _BRAS_IT_H

#include "config_it.h"
#include "element_fini.h"
#include "allocation.h"
#include <Lima/erreur.h>
#include "tableau.h"

BEGIN_NAMESPACE_LIMA


class _NoeudInterne;
class _PolygoneInterne;
class _PolyedreInterne;

//! Bras du maillage.
class _BrasInterne : public _ElementFini
{
public:
  typedef Tableau<_NoeudInterne*>::iterator iterator_noeud;
  typedef Tableau<_NoeudInterne*>::const_iterator const_iterator_noeud;
  typedef Tableau<_PolygoneInterne*>::iterator iterator_polygone;
  typedef Tableau<_PolygoneInterne*>::const_iterator const_iterator_polygone;
  typedef Tableau<_PolyedreInterne*>::iterator iterator_polyedre;
  typedef Tableau<_PolyedreInterne*>::const_iterator const_iterator_polyedre;

  //! Utile pour les messages d'erreur.
  static const char* nom_class() { return "BRAS"; }

  //! Constructeur par défaut.
  static _BrasInterne* create(_NoeudInterne* n0, _NoeudInterne* n1);
  //! Constructeur avec indice.
  static _BrasInterne* create(size_type n, 
			      _NoeudInterne* n0, _NoeudInterne* n1);

  //! Redéfinition de l'allocation mémoire.
  void* operator new(size_t size);
  //! Redéfinition de la libération mémoire.
  void operator delete(void* ptr);

  //! Nombre de noeud.
  size_type             nb_noeuds() const;
  //! Nombre de polygones.
  size_type             nb_polygones() const;
  //! Nombre de polyèdres.
  size_type             nb_polyedres() const;

  //! Noeud numéro n.
  _NoeudInterne*        noeud(size_type n) const;
  //! Polygone numéro n.
  _PolygoneInterne*     polygone(size_type n) const;
  //! Polyèdre numéro n.
  _PolyedreInterne*     polyedre(size_type n) const;

  //! Connecte le polygone.
  void                  connecter(_PolygoneInterne* pg);
  //! Connecte le polyèdre.
  void                  connecter(_PolyedreInterne* pd);

  //! R.A.Z. des connexions.
  void                  invalider_polygones();
  //! R.A.Z. des connexions.
  void                  invalider_polyedres();

  //! Longueur du bras.
  double                longueur() const;

  //! Inverse le sens de parcours du bras.
  void                  retourner();

  //! Recopie des noeuds seuls.
  void                  copier(const _BrasInterne* br);

  //! Iterator premier noeud.
  iterator_noeud        premier_noeud() 
    { return m_noeuds.begin(); }
  const_iterator_noeud  premier_noeud() const 
    { return m_noeuds.begin(); }

  //! Iterator dernier noeud.
  iterator_noeud        dernier_noeud() 
    { return m_noeuds.end(); }
  const_iterator_noeud  dernier_noeud() const 
    { return m_noeuds.end(); }

  //! Iterator premier polygone.
  iterator_polygone     premier_polygone() 
    { return m_polygones.begin(); }
  const_iterator_polygone premier_polygone() const 
    { return m_polygones.begin(); }

  //! Iterator dernier polygone.
  iterator_polygone     dernier_polygone() 
    { return m_polygones.end(); }
  const_iterator_polygone dernier_polygone() const 
    { return m_polygones.end(); }

  //! Iterator premier polyèdre.
  iterator_polyedre     premier_polyedre() 
    { return m_polyedres.begin(); }
  const_iterator_polyedre premier_polyedre() const 
    { return m_polyedres.begin(); }

  //! Iterator dernier polyèdre.
  iterator_polyedre     dernier_polyedre() 
    { return m_polyedres.end(); }
  const_iterator_polyedre dernier_polyedre() const 
    { return m_polyedres.end(); }
    
  //! Comparaison des noeuds seuls.
  static bool comparer(const _BrasInterne* b1, const _BrasInterne* b2);  
 
  static _NoeudInterne* frontiere(const _BrasInterne* b1, const _BrasInterne* b2);

  //! Retourne l'élément opposé.
  _NoeudInterne*      element_oppose(const _NoeudInterne* nd) const;
  //! Retourne l'élément opposé.
  _PolygoneInterne*   element_oppose(const _PolygoneInterne* pg) const;

  //! Remplace le noeud n1 par le noeud n2 pour chaque occurence existante de n1.
  void remplacer_noeud (_NoeudInterne* n1, _NoeudInterne* n2);

protected:
  _BrasInterne(_NoeudInterne* n0, _NoeudInterne* n1);
  _BrasInterne(size_type n, _NoeudInterne* n0, _NoeudInterne* n1);

protected:
#ifndef LIMA_MT
  //! Permet de générer automatiquement un id unique.
  static size_type              m_max_id;
#endif	// LIMA_MT

  //! Les noeuds du bras.
  Tableau<_NoeudInterne*>       m_noeuds;
  //! Connexions vers les polygones.
  Tableau<_PolygoneInterne*>    m_polygones;
  //! Connexions vers les polyèdres.
  Tableau<_PolyedreInterne*>    m_polyedres;
};

END_NAMESPACE_LIMA



#endif
