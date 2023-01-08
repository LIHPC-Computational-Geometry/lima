#ifndef _POLYGONE_IT_H
#define _POLYGONE_IT_H

#include "config_it.h"
#include "element_fini.h"
#include "tableau.h"
#include "allocation.h"
#include <Lima/erreur.h>


BEGIN_NAMESPACE_LIMA


class _NoeudInterne;
class _BrasInterne;
class _PolyedreInterne;

//! Polygone du maillage.
/*!
  Cette classe décrit un polygone quelconque. Pour des raisons
  d'efficacité, elle est spécialisée dans le cas du triangle et du
  quadrangle afin de redéfinir le calcul de la surface ou l'extraction
  des côtés. Afin d'instancier le bon type suivant le nombre de
  noeuds, il faut passer par la fonction create.
*/
class _PolygoneInterne : public _ElementFini
{
public:
  typedef Tableau<_NoeudInterne*>::iterator iterator_noeud;
  typedef Tableau<_NoeudInterne*>::const_iterator const_iterator_noeud;
  typedef Tableau<_BrasInterne*>::iterator iterator_bras;
  typedef Tableau<_BrasInterne*>::const_iterator const_iterator_bras;
  typedef Tableau<_PolyedreInterne*>::iterator iterator_polyedre;
  typedef Tableau<_PolyedreInterne*>::const_iterator const_iterator_polyedre;

  //! Utile pour les messages d'erreur.
  static const char* nom_class() { return "POLYGONE"; }

  //! Constructeur par défaut d'un triangle.
  static _PolygoneInterne* create(_NoeudInterne* n0, _NoeudInterne* n1,
				  _NoeudInterne* n2);
  //! Constructeur par défaut d'un quadrangle.
  static _PolygoneInterne* create(_NoeudInterne* n0, _NoeudInterne* n1,
				  _NoeudInterne* n2, _NoeudInterne* n3);
  static _PolygoneInterne* create(_NoeudInterne* n0, _NoeudInterne* n1,
				  _NoeudInterne* n2, _NoeudInterne* n3,
				  _NoeudInterne* n4);
  static _PolygoneInterne* create(_NoeudInterne* n0, _NoeudInterne* n1,
				  _NoeudInterne* n2, _NoeudInterne* n3,
				  _NoeudInterne* n4, _NoeudInterne* n5);
  //! Constructeur par défaut d'un polygone quelconque.
  static _PolygoneInterne* create(const Tableau<_NoeudInterne*>& nds);
  
  //! Constructeur avec indice d'un triangle.
  static _PolygoneInterne* create(size_type n, _NoeudInterne* n0, 
				  _NoeudInterne* n1, _NoeudInterne* n2);
  //! Constructeur avec indice d'un quadrangle.
  static _PolygoneInterne* create(size_type n, _NoeudInterne* n0, 
				  _NoeudInterne* n1, _NoeudInterne* n2, 
				  _NoeudInterne* n3);
  static _PolygoneInterne* create(size_type n, _NoeudInterne* n0, 
				  _NoeudInterne* n1, _NoeudInterne* n2, 
				  _NoeudInterne* n3, _NoeudInterne* n4);
  static _PolygoneInterne* create(size_type n, _NoeudInterne* n0, 
				  _NoeudInterne* n1, _NoeudInterne* n2, 
				  _NoeudInterne* n3, _NoeudInterne* n4,
				  _NoeudInterne* n5);
  //! Constructeur avec indice d'un polygone quelconque.
  static _PolygoneInterne* create(size_type n, 
				  const Tableau<_NoeudInterne*>& nds);  

  //! Redéfinition de l'allocation mémoire.
  void* operator new(size_t size);
  //! Redéfinition de la libération mémoire.
  void operator delete(void* ptr);

  //! Nombre de noeud.
  size_type             nb_noeuds() const;
  //! Nombre de bras.
  size_type             nb_bras() const;
  //! Nombre de polyèdres.
  size_type             nb_polyedres() const;

  //! Noeud numéro n.
  _NoeudInterne*        noeud(size_type n) const;
  //! Bras numéro n.
  _BrasInterne*         bras(size_type n) const;
  //! Polyèdre numéro n.
  _PolyedreInterne*     polyedre(size_type n) const;

  //! Nombre potentiel d'arêtes.
  size_type             nb_aretes() const;

  //! Reserve la place pour les bras.
  void                  reserver_bras();
  //! Connecte le bras à la position pos.
  void                  connecter(_BrasInterne* br, size_type pos);
  //! Connecte le polyèdre.
  void                  connecter(_PolyedreInterne* pd);

  //! Crée l'arête n.
  _BrasInterne*         extraire_arete(size_type n) const;

  //! R.A.Z. des connexions.
  void                  invalider_bras();
  //! R.A.Z. des connexions.
  void                  invalider_polyedres();

  //! Surface du polygone.
  virtual double        surface2D() const;

  //! Inverse le sens de parcours du polygone.
  virtual void          retourner();

  //! Recopie des noeuds seuls.
  void                  copier(const _PolygoneInterne* br);

  //! Retourne l'indice local du bras, -1 sinon.
  virtual int           contenir(const _BrasInterne* br) const;

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

  //! Iterator premier bras.
  iterator_bras         premier_bras() 
    { return m_bras.begin(); }
  const_iterator_bras   premier_bras() const 
    { return m_bras.begin(); }

  //! Iterator dernier bras.
  iterator_bras         dernier_bras() 
    { return m_bras.end(); }
  const_iterator_bras dernier_bras() const 
    { return m_bras.end(); }

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
  static bool comparer(const _PolygoneInterne* p1, 
		       const _PolygoneInterne* p2);  

  virtual _BrasInterne*       element_oppose(const _BrasInterne* br) const;
  _BrasInterne*               bras_suivant(const _BrasInterne* br) const;
  _BrasInterne*               bras_precedent(const _BrasInterne* br) const;  

  //! Remplace le noeud n1 par le noeud n2 pour chaque occurence existante de n1.
  void remplacer_noeud (_NoeudInterne* n1, _NoeudInterne* n2);

protected:
  _PolygoneInterne(_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2);
  _PolygoneInterne(_NoeudInterne* n0, _NoeudInterne* n1,
		   _NoeudInterne* n2, _NoeudInterne* n3);
  _PolygoneInterne(const Tableau<_NoeudInterne*>& nds);
  
  _PolygoneInterne(size_type n, _NoeudInterne* n0, 
		   _NoeudInterne* n1, _NoeudInterne* n2);
  _PolygoneInterne(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
		   _NoeudInterne* n2, _NoeudInterne* n3);
  _PolygoneInterne(size_type n, const Tableau<_NoeudInterne*>& nds);  

protected:
#ifndef LIMA_MT
  static size_type              m_max_id;
#endif	// LIMA_MT

  Tableau<_NoeudInterne*>       m_noeuds;
  Tableau<_BrasInterne*>        m_bras;
  Tableau<_PolyedreInterne*>    m_polyedres;
};


END_NAMESPACE_LIMA


#endif
