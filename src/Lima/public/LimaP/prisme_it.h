#ifndef _PRISME_IT_H
#define _PRISME_IT_H

#include "config_it.h"
#include "polyedre_it.h"

BEGIN_NAMESPACE_LIMA

//! Prisme.
/*!
  Spécialisation de la classe _PolyedreInterne dans le cas du prisme.
*/
class _PrismeInterne : public _PolyedreInterne
{
public:
  //! Volume orienté du polyèdre.
  double                volume() const;
  //! Inverse le sens de parcours du polyèdre.
  void                  retourner();

  //! Nombre potentiel de faces.
  size_type             nb_faces() const;
  //! Nombre potentiel d'arêtes.
  size_type             nb_aretes() const;

  // Crée l'arête n.
  _BrasInterne*         extraire_arete(size_type n) const;
  // Crée l'arête n.
  _PolygoneInterne*     extraire_face(size_type n) const;

  // Retourne l'indice local du bras, -1 sinon.
  int                   contenir(const _BrasInterne* br) const;
  // Retourne l'indice local du polygone, -1 sinon.
  int                   contenir(const _PolygoneInterne* pg) const;

protected:
  _PrismeInterne(_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
		 _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5);
  _PrismeInterne(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
		 _NoeudInterne* n2, _NoeudInterne* n3, _NoeudInterne* n4,
		 _NoeudInterne* n5);

  static size_type POS[12][6];
  bool comparer(const _PolyedreInterne* p) const;

  friend class _PolyedreInterne;

  // Type de polyedre
  virtual Polyedre::PolyedreType type() {return Polyedre::PRISME;};

private:
  enum {NB_NOEUDS=6, NB_ARETES=9, NB_FACES=5};
  
  static size_type      m_noeud_aretes[NB_NOEUDS][4][2];
  static size_type      m_noeud_faces[NB_NOEUDS][7][5];
  static size_type      m_faces[NB_FACES][5];
  static size_type      m_aretes[NB_ARETES][2];
};
  
inline 
_PrismeInterne::_PrismeInterne(_NoeudInterne* n0, _NoeudInterne* n1, 
			       _NoeudInterne* n2, _NoeudInterne* n3,
			       _NoeudInterne* n4, _NoeudInterne* n5):
  _PolyedreInterne(n0, n1, n2, n3, n4, n5)
{
}

inline 
_PrismeInterne::_PrismeInterne(size_type n, _NoeudInterne* n0, 
			       _NoeudInterne* n1, _NoeudInterne* n2, 
			       _NoeudInterne* n3, _NoeudInterne* n4,  
			       _NoeudInterne* n5):
  _PolyedreInterne(n, n0, n1, n2, n3, n4, n5)
{
}


END_NAMESPACE_LIMA

#endif
