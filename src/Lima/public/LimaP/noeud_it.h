#ifndef _NOEUD_IT_H
#define _NOEUD_IT_H

#include "config_it.h"
#include "element_fini.h"
#include "point.h"
#include "tableau.h"
#include "allocation.h"
#include <Lima/erreur.h>

BEGIN_NAMESPACE_LIMA

class _PolyedreInterne;
class _BrasInterne;
class _PolygoneInterne;

//! Noeud du maillage.
class _NoeudInterne : public _ElementFini,  public _Point
{
public:
  typedef Tableau<_BrasInterne*>::iterator iterator_bras;
  typedef Tableau<_BrasInterne*>::const_iterator const_iterator_bras;
  typedef Tableau<_PolygoneInterne*>::iterator iterator_polygone;
  typedef Tableau<_PolygoneInterne*>::const_iterator const_iterator_polygone;
  typedef Tableau<_PolyedreInterne*>::iterator iterator_polyedre;
  typedef Tableau<_PolyedreInterne*>::const_iterator const_iterator_polyedre;


  //! Utile pour les messages d'erreur.
  static const char* nom_class() { return "NOEUD"; }
  
  //! Constructeur par défaut.
  static _NoeudInterne* create(double x=0, double y=0, double z=0);
  //! Constructeur avec indice.
  static _NoeudInterne* create(size_type n, 
			       double x=0, double y=0, double z=0);

  //! Redéfinition de l'allocation mémoire.
  void* operator new(size_t size);
  //! Redéfinition de la libération mémoire.
  void operator delete(void* ptr);

  //! Nombre de noeuds. (juste pour implementer la fct virtuelle pure de _ElementFini)
  size_type             nb_noeuds() const;

  //! Nombre de bras.
  size_type             nb_bras() const;
  //! Nombre de polygones.
  size_type             nb_polygones() const;
  //! Nombre de polyèdres.
  size_type             nb_polyedres() const;

  //! Bras numéro n.
  _BrasInterne*         bras(size_type n) const;
  //! Polygone numéro n.
  _PolygoneInterne*     polygone(size_type n) const;
  //! Polyèdre numéro n.
  _PolyedreInterne*     polyedre(size_type n) const;

  //! Connecte le bras.
  void                  connecter(_BrasInterne* br);
  //! Connecte le polygone.
  void                  connecter(_PolygoneInterne* pg);
  //! Connecte le polyèdre.
  void                  connecter(_PolyedreInterne* pd);

  //! R.A.Z. des connexions.
  void                  invalider_bras();
  //! R.A.Z. des connexions.
  void                  invalider_polygones();
  //! R.A.Z. des connexions.
  void                  invalider_polyedres();

  //! Iterator premier bras.
  iterator_bras         premier_bras() 
    { return m_bras.begin(); }
  const_iterator_bras   premier_bras() const 
    { return m_bras.begin(); }
  //! Iterator dernier bras.
  iterator_bras         dernier_bras() 
    { return m_bras.end(); }
  const_iterator_bras   dernier_bras() const 
    { return m_bras.end(); }
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
    
  //! Retourne l'élément opposé.
  _BrasInterne*       element_oppose(const _BrasInterne* br) const;

protected:
  _NoeudInterne(double x=0, double y=0, double z=0);
  _NoeudInterne(size_type n, double x=0, double y=0, double z=0);

protected:
#ifndef LIMA_MT
  //! Permet de générer automatiquement un id unique.
  static size_type              m_max_id;
#endif	// LIMA_MT

  //! Connexions vers les bras.
  Tableau<_BrasInterne*>        m_bras;
  //! Connexions vers les polygones.
  Tableau<_PolygoneInterne*>    m_polygones;
  //! Connexions vers les polyèdres.
  Tableau<_PolyedreInterne*>    m_polyedres;
};

END_NAMESPACE_LIMA


#endif
